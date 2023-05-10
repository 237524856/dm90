/*******************************************************
 HIDAPI - Multi-Platform library for
 communication with HID devices.

 Alan Ott
 Signal 11 Software

 8/22/2009

 Copyright 2009, All Rights Reserved.

 At the discretion of the user of this library,
 this software may be licensed under the terms of the
 GNU General Public License v3, a BSD-Style license, or the
 original HIDAPI license as outlined in the LICENSE.txt,
 LICENSE-gpl3.txt, LICENSE-bsd.txt, and LICENSE-orig.txt
 files located at the root of the source distribution.
 These files may also be found in the public source
 code repository located at:
        https://github.com/libusb/hidapi .
********************************************************/
#include <QtGlobal>
#include<stdio.h>
#include<stdlib.h>
#define BUFFER_SIZE 256


#if defined(Q_OS_WIN32)

#include <windows.h>
#include <Dbt.h>


#ifndef _NTDEF_
typedef LONG NTSTATUS;
#endif

#ifdef __MINGW32__
#include <ntdef.h>
#include <winbase.h>
#endif

#ifdef __CYGWIN__
#include <ntdef.h>
#define _wcsdup wcsdup
#endif

/* The maximum number of characters that can be passed into the
   HidD_Get*String() functions without it failing.*/
#define MAX_STRING_WCHARS 0xFFF

/*#define HIDAPI_USE_DDK*/

#ifdef __cplusplus
extern "C" {
#endif
    #include <setupapi.h>
    #include <winioctl.h>
    #ifdef HIDAPI_USE_DDK
        #include <hidsdi.h>
    #endif

    /* Copied from inc/ddk/hidclass.h, part of the Windows DDK. */
    #define HID_OUT_CTL_CODE(id)  \
        CTL_CODE(FILE_DEVICE_KEYBOARD, (id), METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
    #define IOCTL_HID_GET_FEATURE                   HID_OUT_CTL_CODE(100)
    #define IOCTL_HID_GET_INPUT_REPORT              HID_OUT_CTL_CODE(104)

#ifdef __cplusplus
} /* extern "C" */
#endif

#include <stdio.h>
#include <stdlib.h>


#include "HID/hidapi.h"
#pragma warning(disable:4996)

#undef MIN
#define MIN(x,y) ((x) < (y)? (x): (y))

#ifdef _MSC_VER
    /* Thanks Microsoft, but I know how to use strncpy(). */
    #pragma warning(disable:4996)
#endif

#ifdef __cplusplus
extern "C" {
#endif

static struct hid_api_version api_version = {
    .major = HID_API_VERSION_MAJOR,
    .minor = HID_API_VERSION_MINOR,
    .patch = HID_API_VERSION_PATCH
};

#ifndef HIDAPI_USE_DDK
    /* Since we're not building with the DDK, and the HID header
       files aren't part of the SDK, we have to define all this
       stuff here. In lookup_functions(), the function pointers
       defined below are set. */
    typedef struct _HIDD_ATTRIBUTES{
        ULONG Size;
        USHORT VendorID;
        USHORT ProductID;
        USHORT VersionNumber;
    } HIDD_ATTRIBUTES, *PHIDD_ATTRIBUTES;

    typedef USHORT USAGE;
    typedef struct _HIDP_CAPS {
        USAGE Usage;
        USAGE UsagePage;
        USHORT InputReportByteLength;
        USHORT OutputReportByteLength;
        USHORT FeatureReportByteLength;
        USHORT Reserved[17];
        USHORT fields_not_used_by_hidapi[10];
    } HIDP_CAPS, *PHIDP_CAPS;
    typedef void* PHIDP_PREPARSED_DATA;
    #define HIDP_STATUS_SUCCESS 0x110000

    typedef BOOLEAN (__stdcall *HidD_GetAttributes_)(HANDLE device, PHIDD_ATTRIBUTES attrib);
    typedef BOOLEAN (__stdcall *HidD_GetSerialNumberString_)(HANDLE device, PVOID buffer, ULONG buffer_len);
    typedef BOOLEAN (__stdcall *HidD_GetManufacturerString_)(HANDLE handle, PVOID buffer, ULONG buffer_len);
    typedef BOOLEAN (__stdcall *HidD_GetProductString_)(HANDLE handle, PVOID buffer, ULONG buffer_len);
    typedef BOOLEAN (__stdcall *HidD_SetFeature_)(HANDLE handle, PVOID data, ULONG length);
    typedef BOOLEAN (__stdcall *HidD_GetFeature_)(HANDLE handle, PVOID data, ULONG length);
    typedef BOOLEAN (__stdcall *HidD_GetInputReport_)(HANDLE handle, PVOID data, ULONG length);
    typedef BOOLEAN (__stdcall *HidD_GetIndexedString_)(HANDLE handle, ULONG string_index, PVOID buffer, ULONG buffer_len);
    typedef BOOLEAN (__stdcall *HidD_GetPreparsedData_)(HANDLE handle, PHIDP_PREPARSED_DATA *preparsed_data);
    typedef BOOLEAN (__stdcall *HidD_FreePreparsedData_)(PHIDP_PREPARSED_DATA preparsed_data);
    typedef NTSTATUS (__stdcall *HidP_GetCaps_)(PHIDP_PREPARSED_DATA preparsed_data, HIDP_CAPS *caps);
    typedef BOOLEAN (__stdcall *HidD_SetNumInputBuffers_)(HANDLE handle, ULONG number_buffers);

    static HidD_GetAttributes_ HidD_GetAttributes;
    static HidD_GetSerialNumberString_ HidD_GetSerialNumberString;
    static HidD_GetManufacturerString_ HidD_GetManufacturerString;
    static HidD_GetProductString_ HidD_GetProductString;
    static HidD_SetFeature_ HidD_SetFeature;
    static HidD_GetFeature_ HidD_GetFeature;
    static HidD_GetInputReport_ HidD_GetInputReport;
    static HidD_GetIndexedString_ HidD_GetIndexedString;
    static HidD_GetPreparsedData_ HidD_GetPreparsedData;
    static HidD_FreePreparsedData_ HidD_FreePreparsedData;
    static HidP_GetCaps_ HidP_GetCaps;
    static HidD_SetNumInputBuffers_ HidD_SetNumInputBuffers;

    static HMODULE lib_handle = NULL;
    static BOOLEAN initialized = FALSE;
#endif /* HIDAPI_USE_DDK */


static hid_pnp_callback pnp_callback = NULL;
static void* callback_user_data = NULL;
static HDEVNOTIFY device_notify = NULL;
static HWND device_notify_hwnd = 0;


struct hid_device_ {
        HANDLE device_handle;
        BOOL blocking;
        USHORT output_report_length;
        size_t input_report_length;
        void *last_error_str;
        DWORD last_error_num;
        BOOL read_pending;
        char *read_buf;
        OVERLAPPED ol;
        OVERLAPPED write_ol;
};

static hid_device *new_hid_device()
{
    hid_device *dev = (hid_device*) calloc(1, sizeof(hid_device));
    dev->device_handle = INVALID_HANDLE_VALUE;
    dev->blocking = TRUE;
    dev->output_report_length = 0;
    dev->input_report_length = 0;
    dev->last_error_str = NULL;
    dev->last_error_num = 0;
    dev->read_pending = FALSE;
    dev->read_buf = NULL;
    memset(&dev->ol, 0, sizeof(dev->ol));
    dev->ol.hEvent = CreateEvent(NULL, FALSE, FALSE /*initial state f=nonsignaled*/, NULL);
    memset(&dev->write_ol, 0, sizeof(dev->write_ol));
    dev->write_ol.hEvent = CreateEvent(NULL, FALSE, FALSE /*inital state f=nonsignaled*/, NULL);

    return dev;
}

static void free_hid_device(hid_device *dev)
{
    CloseHandle(dev->ol.hEvent);
    CloseHandle(dev->write_ol.hEvent);
    CloseHandle(dev->device_handle);
    LocalFree(dev->last_error_str);
    free(dev->read_buf);
    free(dev);
}

static void register_error(hid_device *dev, const char *op)
{
    WCHAR *ptr, *msg;
    (void)op; // unreferenced  param
    FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR)&msg, 0/*sz*/,
        NULL);

    /* Get rid of the CR and LF that FormatMessage() sticks at the
       end of the message. Thanks Microsoft! */
    ptr = msg;
    while (*ptr) {
        if (*ptr == '\r') {
            *ptr = 0x0000;
            break;
        }
        ptr++;
    }

    /* Store the message off in the Device entry so that
       the hid_error() function can pick it up. */
    LocalFree(dev->last_error_str);
    dev->last_error_str = msg;
}

#ifndef HIDAPI_USE_DDK
static int lookup_functions()
{
    lib_handle = LoadLibraryA("hid.dll");
    if (lib_handle) {
#if defined(__GNUC__)
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wcast-function-type"
#endif
#define RESOLVE(x) x = (x##_)GetProcAddress(lib_handle, #x); if (!x) return -1;
        RESOLVE(HidD_GetAttributes);
        RESOLVE(HidD_GetSerialNumberString);
        RESOLVE(HidD_GetManufacturerString);
        RESOLVE(HidD_GetProductString);
        RESOLVE(HidD_SetFeature);
        RESOLVE(HidD_GetFeature);
        RESOLVE(HidD_GetInputReport);
        RESOLVE(HidD_GetIndexedString);
        RESOLVE(HidD_GetPreparsedData);
        RESOLVE(HidD_FreePreparsedData);
        RESOLVE(HidP_GetCaps);
        RESOLVE(HidD_SetNumInputBuffers);
#undef RESOLVE
#if defined(__GNUC__)
# pragma GCC diagnostic pop
#endif
    }
    else
        return -1;

    return 0;
}
#endif

static HANDLE open_device(const char *path, BOOL open_rw)
{
    HANDLE handle;
    DWORD desired_access = (open_rw)? (GENERIC_WRITE | GENERIC_READ): 0;
    DWORD share_mode = FILE_SHARE_READ|FILE_SHARE_WRITE;

    handle = CreateFileA(path,
        desired_access,
        share_mode,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_OVERLAPPED,/*FILE_ATTRIBUTE_NORMAL,*/
        0);

    return handle;
}

HID_API_EXPORT const struct hid_api_version* HID_API_CALL hid_version()
{
    return &api_version;
}

HID_API_EXPORT const char* HID_API_CALL hid_version_str()
{
    return HID_API_VERSION_STR;
}

int HID_API_EXPORT hid_init(void)
{
#ifndef HIDAPI_USE_DDK
    if (!initialized) {
        if (lookup_functions() < 0) {
            hid_exit();
            return -1;
        }
        initialized = TRUE;
    }
#endif
    return 0;
}

int HID_API_EXPORT hid_exit(void)
{
#ifndef HIDAPI_USE_DDK
    if (lib_handle)
        FreeLibrary(lib_handle);
    lib_handle = NULL;
    initialized = FALSE;
#endif

    pnp_callback = NULL;
    hid_register_device_notification(NULL, 0);
    return 0;
}

struct hid_device_info HID_API_EXPORT * HID_API_CALL hid_enumerate(unsigned short vendor_id, unsigned short product_id)
{
    BOOL res;
    struct hid_device_info *root = NULL; /* return object */
    struct hid_device_info *cur_dev = NULL;

    /* Windows objects for interacting with the driver. */
    GUID InterfaceClassGuid = {0x4d1e55b2, 0xf16f, 0x11cf, {0x88, 0xcb, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30} };
    SP_DEVINFO_DATA devinfo_data;
    SP_DEVICE_INTERFACE_DATA device_interface_data;
    SP_DEVICE_INTERFACE_DETAIL_DATA_A *device_interface_detail_data = NULL;
    HDEVINFO device_info_set = INVALID_HANDLE_VALUE;
    int device_index = 0;
    int i;

    if (hid_init() < 0)
        return NULL;

    /* Initialize the Windows objects. */
    memset(&devinfo_data, 0x0, sizeof(devinfo_data));
    devinfo_data.cbSize = sizeof(SP_DEVINFO_DATA);
    device_interface_data.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

    /* Get information for all the devices belonging to the HID class. */
    device_info_set = SetupDiGetClassDevsA(&InterfaceClassGuid, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

    /* Iterate over each device in the HID class, looking for the right one. */

    for (;;) {
        HANDLE write_handle = INVALID_HANDLE_VALUE;
        DWORD required_size = 0;
        HIDD_ATTRIBUTES attrib;

        res = SetupDiEnumDeviceInterfaces(device_info_set,
            NULL,
            &InterfaceClassGuid,
            device_index,
            &device_interface_data);

        if (!res) {
            /* A return of FALSE from this function means that
               there are no more devices. */
            break;
        }

        /* Call with 0-sized detail size, and let the function
           tell us how long the detail struct needs to be. The
           size is put in &required_size. */
        res = SetupDiGetDeviceInterfaceDetailA(device_info_set,
            &device_interface_data,
            NULL,
            0,
            &required_size,
            NULL);

        /* Allocate a long enough structure for device_interface_detail_data. */
        device_interface_detail_data = (SP_DEVICE_INTERFACE_DETAIL_DATA_A*) malloc(required_size);
        device_interface_detail_data->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA_A);

        /* Get the detailed data for this device. The detail data gives us
           the device path for this device, which is then passed into
           CreateFile() to get a handle to the device. */
        res = SetupDiGetDeviceInterfaceDetailA(device_info_set,
            &device_interface_data,
            device_interface_detail_data,
            required_size,
            NULL,
            NULL);

        if (!res) {
            /* register_error(dev, "Unable to call SetupDiGetDeviceInterfaceDetail");
               Continue to the next device. */
            goto cont;
        }

        /* Make sure this device is of Setup Class "HIDClass" and has a
           driver bound to it. */
        for (i = 0; ; i++) {
            char driver_name[256];

            /* Populate devinfo_data. This function will return failure
               when there are no more interfaces left. */
            res = SetupDiEnumDeviceInfo(device_info_set, i, &devinfo_data);
            if (!res)
                goto cont;

            res = SetupDiGetDeviceRegistryPropertyA(device_info_set, &devinfo_data,
                           SPDRP_CLASS, NULL, (PBYTE)driver_name, sizeof(driver_name), NULL);
            if (!res)
                goto cont;

            if ((strcmp(driver_name, "HIDClass") == 0) ||
                (strcmp(driver_name, "Mouse") == 0) ||
                (strcmp(driver_name, "Keyboard") == 0)) {
                /* See if there's a driver bound. */
                res = SetupDiGetDeviceRegistryPropertyA(device_info_set, &devinfo_data,
                           SPDRP_DRIVER, NULL, (PBYTE)driver_name, sizeof(driver_name), NULL);
                if (res)
                    break;
            }
        }

        //wprintf(L"HandleName: %s\n", device_interface_detail_data->DevicePath);

        /* Open a handle to the device */
        write_handle = open_device(device_interface_detail_data->DevicePath, FALSE);

        /* Check validity of write_handle. */
        if (write_handle == INVALID_HANDLE_VALUE) {
            /* Unable to open the device. */
            //register_error(dev, "CreateFile");
            goto cont_close;
        }


        /* Get the Vendor ID and Product ID for this device. */
        attrib.Size = sizeof(HIDD_ATTRIBUTES);
        HidD_GetAttributes(write_handle, &attrib);
        //wprintf(L"Product/Vendor: %x %x\n", attrib.ProductID, attrib.VendorID);

        /* Check the VID/PID to see if we should add this
           device to the enumeration list. */
        if ((vendor_id == 0x0 || attrib.VendorID == vendor_id) &&
            (product_id == 0x0 || attrib.ProductID == product_id)) {

            #define WSTR_LEN 512
            const char *str;
            struct hid_device_info *tmp;
            PHIDP_PREPARSED_DATA pp_data = NULL;
            HIDP_CAPS caps;
            NTSTATUS nt_res;
            wchar_t wstr[WSTR_LEN]; /* TODO: Determine Size */
            size_t len;

            /* VID/PID match. Create the record. */
            tmp = (struct hid_device_info*) calloc(1, sizeof(struct hid_device_info));
            if (cur_dev) {
                cur_dev->next = tmp;
            }
            else {
                root = tmp;
            }
            cur_dev = tmp;

            /* Get the Usage Page and Usage for this device. */
            res = HidD_GetPreparsedData(write_handle, &pp_data);
            if (res) {
                nt_res = HidP_GetCaps(pp_data, &caps);
                if (nt_res == HIDP_STATUS_SUCCESS) {
                    cur_dev->usage_page = caps.UsagePage;
                    cur_dev->usage = caps.Usage;
                }

                HidD_FreePreparsedData(pp_data);
            }

            /* Fill out the record */
            cur_dev->next = NULL;
            str = device_interface_detail_data->DevicePath;
            if (str) {
                len = strlen(str);
                cur_dev->path = (char*) calloc(len+1, sizeof(char));
                strncpy(cur_dev->path, str, len+1);
                cur_dev->path[len] = '\0';
            }
            else
                cur_dev->path = NULL;

            /* Serial Number */
            wstr[0]= 0x0000;
            res = HidD_GetSerialNumberString(write_handle, wstr, sizeof(wstr));
            wstr[WSTR_LEN-1] = 0x0000;
            if (res) {
                cur_dev->serial_number = _wcsdup(wstr);
            }

            /* Manufacturer String */
            wstr[0]= 0x0000;
            res = HidD_GetManufacturerString(write_handle, wstr, sizeof(wstr));
            wstr[WSTR_LEN-1] = 0x0000;
            if (res) {
                cur_dev->manufacturer_string = _wcsdup(wstr);
            }

            /* Product String */
            wstr[0]= 0x0000;
            res = HidD_GetProductString(write_handle, wstr, sizeof(wstr));
            wstr[WSTR_LEN-1] = 0x0000;
            if (res) {
                cur_dev->product_string = _wcsdup(wstr);
            }

            /* VID/PID */
            cur_dev->vendor_id = attrib.VendorID;
            cur_dev->product_id = attrib.ProductID;

            /* Release Number */
            cur_dev->release_number = attrib.VersionNumber;

            /* Interface Number. It can sometimes be parsed out of the path
               on Windows if a device has multiple interfaces. See
               http://msdn.microsoft.com/en-us/windows/hardware/gg487473 or
               search for "Hardware IDs for HID Devices" at MSDN. If it's not
               in the path, it's set to -1. */
            cur_dev->interface_number = -1;
            if (cur_dev->path) {
                char *interface_component = strstr(cur_dev->path, "&mi_");
                if (interface_component) {
                    char *hex_str = interface_component + 4;
                    char *endptr = NULL;
                    cur_dev->interface_number = strtol(hex_str, &endptr, 16);
                    if (endptr == hex_str) {
                        /* The parsing failed. Set interface_number to -1. */
                        cur_dev->interface_number = -1;
                    }
                }
            }
        }

cont_close:
        CloseHandle(write_handle);
cont:
        /* We no longer need the detail data. It can be freed */
        free(device_interface_detail_data);

        device_index++;

    }

    /* Close the device information handle. */
    SetupDiDestroyDeviceInfoList(device_info_set);

    return root;

}

void  HID_API_EXPORT HID_API_CALL hid_free_enumeration(struct hid_device_info *devs)
{
    /* TODO: Merge this with the Linux version. This function is platform-independent. */
    struct hid_device_info *d = devs;
    while (d) {
        struct hid_device_info *next = d->next;
        free(d->path);
        free(d->serial_number);
        free(d->manufacturer_string);
        free(d->product_string);
        free(d);
        d = next;
    }
}


HID_API_EXPORT hid_device * HID_API_CALL hid_open(unsigned short vendor_id, unsigned short product_id, const wchar_t *serial_number)
{
    /* TODO: Merge this functions with the Linux version. This function should be platform independent. */
    struct hid_device_info *devs, *cur_dev;
    const char *path_to_open = NULL;
    hid_device *handle = NULL;

    devs = hid_enumerate(vendor_id, product_id);
    cur_dev = devs;
    while (cur_dev) {
        if (cur_dev->vendor_id == vendor_id &&
            cur_dev->product_id == product_id) {
            if (serial_number) {
                if (cur_dev->serial_number && wcscmp(serial_number, cur_dev->serial_number) == 0) {
                    path_to_open = cur_dev->path;
                    break;
                }
            }
            else {
                path_to_open = cur_dev->path;
                break;
            }
        }
        cur_dev = cur_dev->next;
    }

    if (path_to_open) {
        /* Open the device */
        handle = hid_open_path(path_to_open);
    }

    hid_free_enumeration(devs);

    return handle;
}

HID_API_EXPORT hid_device * HID_API_CALL hid_open_path(const char *path)
{
    hid_device *dev;
    HIDP_CAPS caps;
    PHIDP_PREPARSED_DATA pp_data = NULL;
    BOOLEAN res;
    NTSTATUS nt_res;

    if (hid_init() < 0) {
        return NULL;
    }

    dev = new_hid_device();

    /* Open a handle to the device */
    dev->device_handle = open_device(path, TRUE);

    /* Check validity of write_handle. */
    if (dev->device_handle == INVALID_HANDLE_VALUE) {
        /* System devices, such as keyboards and mice, cannot be opened in
           read-write mode, because the system takes exclusive control over
           them.  This is to prevent keyloggers.  However, feature reports
           can still be sent and received.  Retry opening the device, but
           without read/write access. */
        dev->device_handle = open_device(path, FALSE);

        /* Check the validity of the limited device_handle. */
        if (dev->device_handle == INVALID_HANDLE_VALUE) {
            /* Unable to open the device, even without read-write mode. */
            register_error(dev, "CreateFile");
            goto err;
        }
    }

    /* Set the Input Report buffer size to 64 reports. */
    res = HidD_SetNumInputBuffers(dev->device_handle, 11);
    if (!res) {
        register_error(dev, "HidD_SetNumInputBuffers");
        goto err;
    }

    /* Get the Input Report length for the device. */
    res = HidD_GetPreparsedData(dev->device_handle, &pp_data);
    if (!res) {
        register_error(dev, "HidD_GetPreparsedData");
        goto err;
    }
    nt_res = HidP_GetCaps(pp_data, &caps);
    if (nt_res != HIDP_STATUS_SUCCESS) {
        register_error(dev, "HidP_GetCaps");
        goto err_pp_data;
    }
    dev->output_report_length = caps.OutputReportByteLength;
    dev->input_report_length = caps.InputReportByteLength;
    HidD_FreePreparsedData(pp_data);

    dev->read_buf = (char*) malloc(dev->input_report_length);

    return dev;

err_pp_data:
        HidD_FreePreparsedData(pp_data);
err:
        free_hid_device(dev);
        return NULL;
}

int HID_API_EXPORT HID_API_CALL hid_write(hid_device *dev, const unsigned char *data, size_t length)
{
    DWORD bytes_written = 0;
    int function_result = -1;
    BOOL res;
    BOOL overlapped = FALSE;

    unsigned char *buf;

    /* Make sure the right number of bytes are passed to WriteFile. Windows
       expects the number of bytes which are in the _longest_ report (plus
       one for the report number) bytes even if the data is a report
       which is shorter than that. Windows gives us this value in
       caps.OutputReportByteLength. If a user passes in fewer bytes than this,
       create a temporary buffer which is the proper size. */
    if (length >= dev->output_report_length) {
        /* The user passed the right number of bytes. Use the buffer as-is. */
        buf = (unsigned char *) data;
    } else {
        /* Create a temporary buffer and copy the user's data
           into it, padding the rest with zeros. */
        buf = (unsigned char *) malloc(dev->output_report_length);
        memcpy(buf, data, length);
        memset(buf + length, 0, dev->output_report_length - length);
        length = dev->output_report_length;
    }

    res = WriteFile(dev->device_handle, buf, (DWORD) length, NULL, &dev->write_ol);

    if (!res) {
        if (GetLastError() != ERROR_IO_PENDING) {
            /* WriteFile() failed. Return error. */
            register_error(dev, "WriteFile");
            goto end_of_function;
        }
        overlapped = TRUE;
    }

    if (overlapped) {
        /* Wait for the transaction to complete. This makes
           hid_write() synchronous. */
        res = WaitForSingleObject(dev->write_ol.hEvent, 1000);
        if (res != WAIT_OBJECT_0) {
            /* There was a Timeout. */
            register_error(dev, "WriteFile/WaitForSingleObject Timeout");
            goto end_of_function;
        }

        /* Get the result. */
        res = GetOverlappedResult(dev->device_handle, &dev->write_ol, &bytes_written, FALSE/*wait*/);
        if (res) {
            function_result = bytes_written;
        }
        else {
            /* The Write operation failed. */
            register_error(dev, "WriteFile");
            goto end_of_function;
        }
    }

end_of_function:
    if (buf != data)
        free(buf);

    return function_result;
}


int HID_API_EXPORT HID_API_CALL hid_read_timeout(hid_device *dev, unsigned char *data, size_t length, int milliseconds)
{
    DWORD bytes_read = 0;
    size_t copy_len = 0;
    BOOL res = FALSE;
    BOOL overlapped = FALSE;

    /* Copy the handle for convenience. */
    HANDLE ev = dev->ol.hEvent;

    if (!dev->read_pending) {
        /* Start an Overlapped I/O read. */
        dev->read_pending = TRUE;
        memset(dev->read_buf, 0, dev->input_report_length);
        ResetEvent(ev);
        res = ReadFile(dev->device_handle, dev->read_buf, (DWORD) dev->input_report_length, &bytes_read, &dev->ol);

        if (!res) {
            if (GetLastError() != ERROR_IO_PENDING) {
                /* ReadFile() has failed.
                   Clean up and return error. */
                CancelIo(dev->device_handle);
                dev->read_pending = FALSE;
                goto end_of_function;
            }
            overlapped = TRUE;
        }
    }
    else {
        overlapped = TRUE;
    }

    if (overlapped) {
        if (milliseconds >= 0) {
            /* See if there is any data yet. */
            res = WaitForSingleObject(ev, milliseconds);
            if (res != WAIT_OBJECT_0) {
                /* There was no data this time. Return zero bytes available,
                   but leave the Overlapped I/O running. */
                return 0;
            }
        }
        /* Either WaitForSingleObject() told us that ReadFile has completed, or
           we are in non-blocking mode. Get the number of bytes read. The actual
           data has been copied to the data[] array which was passed to ReadFile(). */
        res = GetOverlappedResult(dev->device_handle, &dev->ol, &bytes_read, TRUE/*wait*/);
    }
    /* Set pending back to false, even if GetOverlappedResult() returned error. */
    dev->read_pending = FALSE;

    if (res && bytes_read > 0) {
        if (dev->read_buf[0] == 0x0) {
            /* If report numbers aren't being used, but Windows sticks a report
               number (0x0) on the beginning of the report anyway. To make this
               work like the other platforms, and to make it work more like the
               HID spec, we'll skip over this byte. */
            bytes_read--;
            copy_len = length > bytes_read ? bytes_read : length;
            memcpy(data, dev->read_buf+1, copy_len);
        }
        else {
            /* Copy the whole buffer, report number and all. */
            copy_len = length > bytes_read ? bytes_read : length;
            memcpy(data, dev->read_buf, copy_len);
        }
    }

end_of_function:
    if (!res) {
        register_error(dev, "GetOverlappedResult");
        return -1;
    }

    return (int) copy_len;
}

int HID_API_EXPORT HID_API_CALL hid_read(hid_device *dev, unsigned char *data, size_t length)
{
    return hid_read_timeout(dev, data, length, (dev->blocking)? -1: 0);
}

int HID_API_EXPORT HID_API_CALL hid_set_nonblocking(hid_device *dev, int nonblock)
{
    dev->blocking = !nonblock;
    return 0; /* Success */
}

int HID_API_EXPORT HID_API_CALL hid_send_feature_report(hid_device *dev, const unsigned char *data, size_t length)
{
    BOOL res = HidD_SetFeature(dev->device_handle, (PVOID)data, (DWORD) length);
    if (!res) {
        register_error(dev, "HidD_SetFeature");
        return -1;
    }

    return (int) length;
}


int HID_API_EXPORT HID_API_CALL hid_get_feature_report(hid_device *dev, unsigned char *data, size_t length)
{
    BOOL res;
#if 0
    res = HidD_GetFeature(dev->device_handle, data, length);
    if (!res) {
        register_error(dev, "HidD_GetFeature");
        return -1;
    }
    return 0; /* HidD_GetFeature() doesn't give us an actual length, unfortunately */
#else
    DWORD bytes_returned;

    OVERLAPPED ol;
    memset(&ol, 0, sizeof(ol));

    res = DeviceIoControl(dev->device_handle,
        IOCTL_HID_GET_FEATURE,
        data, (DWORD) length,
        data, (DWORD) length,
        &bytes_returned, &ol);

    if (!res) {
        if (GetLastError() != ERROR_IO_PENDING) {
            /* DeviceIoControl() failed. Return error. */
            register_error(dev, "Send Feature Report DeviceIoControl");
            return -1;
        }
    }

    /* Wait here until the write is done. This makes
       hid_get_feature_report() synchronous. */
    res = GetOverlappedResult(dev->device_handle, &ol, &bytes_returned, TRUE/*wait*/);
    if (!res) {
        /* The operation failed. */
        register_error(dev, "Send Feature Report GetOverLappedResult");
        return -1;
    }

    /* bytes_returned does not include the first byte which contains the
       report ID. The data buffer actually contains one more byte than
       bytes_returned. */
    bytes_returned++;

    return bytes_returned;
#endif
}


int HID_API_EXPORT HID_API_CALL hid_get_input_report(hid_device *dev, unsigned char *data, size_t length)
{
    BOOL res;
#if 0
    res = HidD_GetInputReport(dev->device_handle, data, length);
    if (!res) {
        register_error(dev, "HidD_GetInputReport");
        return -1;
    }
    return length;
#else
    DWORD bytes_returned;

    OVERLAPPED ol;
    memset(&ol, 0, sizeof(ol));

    res = DeviceIoControl(dev->device_handle,
        IOCTL_HID_GET_INPUT_REPORT,
        data, (DWORD) length,
        data, (DWORD) length,
        &bytes_returned, &ol);

    if (!res) {
        if (GetLastError() != ERROR_IO_PENDING) {
            /* DeviceIoControl() failed. Return error. */
            register_error(dev, "Send Input Report DeviceIoControl");
            return -1;
        }
    }

    /* Wait here until the write is done. This makes
       hid_get_feature_report() synchronous. */
    res = GetOverlappedResult(dev->device_handle, &ol, &bytes_returned, TRUE/*wait*/);
    if (!res) {
        /* The operation failed. */
        register_error(dev, "Send Input Report GetOverLappedResult");
        return -1;
    }

    /* bytes_returned does not include the first byte which contains the
       report ID. The data buffer actually contains one more byte than
       bytes_returned. */
    bytes_returned++;

    return bytes_returned;
#endif
}

void HID_API_EXPORT HID_API_CALL hid_close(hid_device *dev)
{
    if (!dev)
        return;
    CancelIo(dev->device_handle);
    SetEvent(dev->ol.hEvent); //关闭的时候使阻塞读取返回，add by hzf 20210311
    //调试作用
    //free_hid_device(dev);
}

int HID_API_EXPORT_CALL HID_API_CALL hid_get_manufacturer_string(hid_device *dev, wchar_t *string, size_t maxlen)
{
    BOOL res;

    res = HidD_GetManufacturerString(dev->device_handle, string, sizeof(wchar_t) * (DWORD) MIN(maxlen, MAX_STRING_WCHARS));
    if (!res) {
        register_error(dev, "HidD_GetManufacturerString");
        return -1;
    }

    return 0;
}

int HID_API_EXPORT_CALL HID_API_CALL hid_get_product_string(hid_device *dev, wchar_t *string, size_t maxlen)
{
    BOOL res;

    res = HidD_GetProductString(dev->device_handle, string, sizeof(wchar_t) * (DWORD) MIN(maxlen, MAX_STRING_WCHARS));
    if (!res) {
        register_error(dev, "HidD_GetProductString");
        return -1;
    }

    return 0;
}

int HID_API_EXPORT_CALL HID_API_CALL hid_get_serial_number_string(hid_device *dev, wchar_t *string, size_t maxlen)
{
    BOOL res;

    res = HidD_GetSerialNumberString(dev->device_handle, string, sizeof(wchar_t) * (DWORD) MIN(maxlen, MAX_STRING_WCHARS));
    if (!res) {
        register_error(dev, "HidD_GetSerialNumberString");
        return -1;
    }

    return 0;
}

int HID_API_EXPORT_CALL HID_API_CALL hid_get_indexed_string(hid_device *dev, int string_index, wchar_t *string, size_t maxlen)
{
    BOOL res;

    res = HidD_GetIndexedString(dev->device_handle, string_index, string, sizeof(wchar_t) * (DWORD) MIN(maxlen, MAX_STRING_WCHARS));
    if (!res) {
        register_error(dev, "HidD_GetIndexedString");
        return -1;
    }

    return 0;
}


HID_API_EXPORT const wchar_t * HID_API_CALL  hid_error(hid_device *dev)
{
    if (dev) {
        if (dev->last_error_str == NULL)
            return L"Success";
        return (wchar_t*)dev->last_error_str;
    }

    // Global error messages are not (yet) implemented on Windows.
    return L"hid_error for global errors is not implemented yet";
}


int GetVidPidFromStr(const char* path, unsigned short *vid, unsigned short *pid)
{
    char szStr[512] = { 0 };
    char temp[16] = { 0 };
    char *pEnd = NULL;
    const char *str;

    //XP下有小写字母，需转大写
    //\\?\HID#Vid_2b53&Pid_0019#7&2340704c&0&0000#{4d1e55b2-f16f-11cf-88cb-001111000030}
    strcpy_s(szStr,strlen(path)+1, path);
    _strupr_s(szStr,strlen(szStr)+1);

    str = strstr(szStr, "#VID_");
    if ( !str )
        return 0;

    temp[0] = str[5];
    temp[1] = str[6];
    temp[2] = str[7];
    temp[3] = str[8];

    *vid = (unsigned short)strtol(temp, &pEnd, 16);
    if (  pEnd[0] != '\0' )
    {
        return 0;
    }

    temp[0] = str[14];
    temp[1] = str[15];
    temp[2] = str[16];
    temp[3] = str[17];

    pEnd = NULL;
    *pid = (unsigned short)strtol(temp, &pEnd, 16);
    if ( pEnd[0] != '\0' )
    {
        return 0;
    }

    return 1;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PDEV_BROADCAST_DEVICEINTERFACE pdbi = NULL;
    unsigned short vid = 0, pid = 0;


    if (WM_DEVICECHANGE == message)
    {

        // WPARAM nEventType, LPARAM dwData
        // dwData是一个指向DEV_BROADCAST_DEVICEINTERFACE结构体的指针，
        // 在该结构体中保存了设备的类型、路径名等参数。通过跟我们指定设备
        // 的路径名比较，即可以判断是否是我们指定的设备拔下或者插入了。

        pdbi = (PDEV_BROADCAST_DEVICEINTERFACE)lParam;
        if (!pdbi)
        {
            return 0;
        }
        //wchar转char
        size_t i;
        char* pMBBuffer = (char *)malloc( BUFFER_SIZE );
        wcstombs_s(&i, pMBBuffer, (size_t)BUFFER_SIZE,
                       pdbi->dbcc_name, (size_t)BUFFER_SIZE );
        //printf("------------111----xxx---%s-----\n",pMBBuffer);
        if (DBT_DEVICEARRIVAL == wParam)
        {
            if(pnp_callback && GetVidPidFromStr(pMBBuffer, &vid, &pid)){

                pnp_callback(vid, pid, pMBBuffer, 0, callback_user_data);
            }
//            if (DBT_DEVTYP_DEVICEINTERFACE == pdbi->dbcc_devicetype)
//            {
//                if (pnp_callback && GetVidPidFromStr(pdbi->dbcc_name, &vid, &pid))
//                {
//                    pnp_callback(vid, pid, pdbi->dbcc_name, 0, callback_user_data);
//                }
//            }
        }
        else if (DBT_DEVICEREMOVECOMPLETE == wParam)
        {

            if(pnp_callback && GetVidPidFromStr(pMBBuffer, &vid, &pid)){
                pnp_callback(vid, pid, pMBBuffer, 1, callback_user_data);
            }
//            if (DBT_DEVTYP_DEVICEINTERFACE == pdbi->dbcc_devicetype)
//            {
//                if (pnp_callback && GetVidPidFromStr(pdbi->dbcc_name, &vid, &pid))
//                {
//                    pnp_callback(vid, pid, pdbi->dbcc_name, 1, callback_user_data);
//                }
//            }
        }
        free(pMBBuffer);
        pMBBuffer = NULL;
        return 0;
    }
    else
    {
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

HID_API_EXPORT void HID_API_CALL hid_register_device_notification(hid_pnp_callback fn, void* user_data)
{
    HINSTANCE hInstance;
    WNDCLASSEX wcex;

    DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;	/* un/plug notification filter */

    // This GUID is for all USB serial host PnP drivers, but you can replace it
    // with any valid device class guid.
    //GUID WceusbshGUID = { 0x25dbce51, 0x6c8f, 0x4a72, 0x8a,0x6d,0xb5,0x4c,0x2b,0x4f,0xc8,0x35 };

    ///temporarily stores Windows HID Class GUID */
    //{4D1E55B2-F16F-11CF-88CB-001111000030}
    GUID HidGuid = { 0x4D1E55B2, 0xF16F, 0x11CF, 0x88,0xCB,0x00,0x11,0x11,0x00,0x00,0x30 };

    const char* wndClassName = "{2FCD1C73-42E0-4EF3-8A0C-96FE38475572}";

    if (!fn)
    {
        if ( device_notify )
        {
            /* end HID device plug/unplug notifications */
            UnregisterDeviceNotification( device_notify );
            device_notify = NULL;
        }
        if (device_notify_hwnd)
        {
            DestroyWindow(device_notify_hwnd);
            device_notify_hwnd = NULL;
        }
        return;
    }
    else
    {
        // 已注册过，只替换回调函数
        if (pnp_callback)
        {
            pnp_callback = fn;
            callback_user_data = user_data;
            return;
        }
    }

    hInstance = GetModuleHandle(NULL);

    wcex.cbSize			= sizeof(WNDCLASSEX);
    wcex.style			= CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc	= WndProc;
    wcex.cbClsExtra		= 0;
    wcex.cbWndExtra		= 0;
    wcex.hInstance		= hInstance;
    wcex.hIcon			= NULL;
    wcex.hCursor		= NULL;
    wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName	= NULL;
    wcex.lpszClassName	= wndClassName;
    wcex.hIconSm		= NULL;
    if ( !RegisterClassEx(&wcex) )
        return;

    device_notify_hwnd = CreateWindow(wndClassName, NULL, WS_POPUP,
        0, 0, 0, 0, NULL, NULL, hInstance, NULL);
    if ( device_notify_hwnd )
    {
        ShowWindow(device_notify_hwnd, SW_HIDE);
        UpdateWindow(device_notify_hwnd);

        /* 1) get the HID GUID */
        // HidD_GetHidGuid(&HidGuid);

        /* 2) clear the notification filter */
        ZeroMemory( &NotificationFilter, sizeof(NotificationFilter));

        /* 3) assign the previously cleared structure with the correct data
        so that the application is notified of HID device un/plug events */
        NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
        NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
        NotificationFilter.dbcc_classguid = HidGuid;

        /* 4) register device notifications for this application */
        device_notify = RegisterDeviceNotification(
            device_notify_hwnd,
            &NotificationFilter,
            DEVICE_NOTIFY_WINDOW_HANDLE);

        if (device_notify)
        {
            pnp_callback = fn;
            callback_user_data = user_data;
        }
        else
        {
            DestroyWindow(device_notify_hwnd);
        }
    }
}



HID_API_EXPORT int HID_API_CALL hid_max_input_report_len(hid_device *device)
{
    if (device)
        return (int)device->input_report_length;
    else
        return 0;
}


/*#define PICPGM*/
/*#define S11*/
#define P32
#ifdef S11
  unsigned short VendorID = 0xa0a0;
    unsigned short ProductID = 0x0001;
#endif

#ifdef P32
  unsigned short VendorID = 0x04d8;
    unsigned short ProductID = 0x3f;
#endif


#ifdef PICPGM
  unsigned short VendorID = 0x04d8;
  unsigned short ProductID = 0x0033;
#endif


#if 0
int __cdecl main(int argc, char* argv[])
{
    int res;
    unsigned char buf[65];

    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

    /* Set up the command buffer. */
    memset(buf,0x00,sizeof(buf));
    buf[0] = 0;
    buf[1] = 0x81;


    /* Open the device. */
    int handle = open(VendorID, ProductID, L"12345");
    if (handle < 0)
        printf("unable to open device\n");


    /* Toggle LED (cmd 0x80) */
    buf[1] = 0x80;
    res = write(handle, buf, 65);
    if (res < 0)
        printf("Unable to write()\n");

    /* Request state (cmd 0x81) */
    buf[1] = 0x81;
    write(handle, buf, 65);
    if (res < 0)
        printf("Unable to write() (2)\n");

    /* Read requested state */
    read(handle, buf, 65);
    if (res < 0)
        printf("Unable to read()\n");

    /* Print out the returned buffer. */
    for (int i = 0; i < 4; i++)
        printf("buf[%d]: %d\n", i, buf[i]);

    return 0;
}
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif


#else

/*******************************************************
 HIDAPI - Multi-Platform library for
 communication with HID devices.

 Alan Ott
 Signal 11 Software

 2010-07-03

 Copyright 2010, All Rights Reserved.

 At the discretion of the user of this library,
 this software may be licensed under the terms of the
 GNU General Public License v3, a BSD-Style license, or the
 original HIDAPI license as outlined in the LICENSE.txt,
 LICENSE-gpl3.txt, LICENSE-bsd.txt, and LICENSE-orig.txt
 files located at the root of the source distribution.
 These files may also be found in the public source
 code repository located at:
        https://github.com/libusb/hidapi .
********************************************************/

/* See Apple Technical Note TN2187 for details on IOHidManager. */

#include <IOKit/hid/IOHIDManager.h>
#include <IOKit/hid/IOHIDKeys.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/usb/USBSpec.h>
#include <CoreFoundation/CoreFoundation.h>
#include <wchar.h>
#include <locale.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <dlfcn.h>

#include "HID/hidapi.h"


/* As defined in AppKit.h, but we don't need the entire AppKit for a single constant. */
extern const double NSAppKitVersionNumber;

/* Barrier implementation because Mac OSX doesn't have pthread_barrier.
   It also doesn't have clock_gettime(). So much for POSIX and SUSv2.
   This implementation came from Brent Priddy and was posted on
   StackOverflow. It is used with his permission. */
typedef int pthread_barrierattr_t;
typedef struct pthread_barrier {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int count;
    int trip_count;
} pthread_barrier_t;

static int pthread_barrier_init(pthread_barrier_t *barrier, const pthread_barrierattr_t *attr, unsigned int count)
{
    (void) attr;

    if(count == 0) {
        errno = EINVAL;
        return -1;
    }

    if(pthread_mutex_init(&barrier->mutex, 0) < 0) {
        return -1;
    }
    if(pthread_cond_init(&barrier->cond, 0) < 0) {
        pthread_mutex_destroy(&barrier->mutex);
        return -1;
    }
    barrier->trip_count = count;
    barrier->count = 0;

    return 0;
}

static int pthread_barrier_destroy(pthread_barrier_t *barrier)
{
    pthread_cond_destroy(&barrier->cond);
    pthread_mutex_destroy(&barrier->mutex);
    return 0;
}

static int pthread_barrier_wait(pthread_barrier_t *barrier)
{
    pthread_mutex_lock(&barrier->mutex);
    ++(barrier->count);
    if(barrier->count >= barrier->trip_count)
    {
        barrier->count = 0;
        pthread_cond_broadcast(&barrier->cond);
        pthread_mutex_unlock(&barrier->mutex);
        return 1;
    }
    else
    {
        pthread_cond_wait(&barrier->cond, &(barrier->mutex));
        pthread_mutex_unlock(&barrier->mutex);
        return 0;
    }
}

static int return_data(hid_device *dev, unsigned char *data, size_t length);

/* Linked List of input reports received from the device. */
struct input_report {
    uint8_t *data;
    size_t len;
    struct input_report *next;
};

struct hid_device_ {
    IOHIDDeviceRef device_handle;
    int blocking;
    int uses_numbered_reports;
    int disconnected;
    CFStringRef run_loop_mode;
    CFRunLoopRef run_loop;
    CFRunLoopSourceRef source;
    uint8_t *input_report_buf;
    CFIndex max_input_report_len;
    struct input_report *input_reports;

    pthread_t thread;
    pthread_mutex_t mutex; /* Protects input_reports */
    pthread_cond_t condition;
    pthread_barrier_t barrier; /* Ensures correct startup sequence */
    pthread_barrier_t shutdown_barrier; /* Ensures correct shutdown sequence */
    int shutdown_thread;

    unsigned short vid;
    unsigned short pid;
    io_string_t   path;
};

static hid_pnp_callback pnp_callback = NULL;
static void* callback_user_data = NULL;

static hid_device *new_hid_device(void)
{
    hid_device *dev = (hid_device*) calloc(1, sizeof(hid_device));
    dev->device_handle = NULL;
    dev->blocking = 1;
    dev->uses_numbered_reports = 0;
    dev->disconnected = 0;
    dev->run_loop_mode = NULL;
    dev->run_loop = NULL;
    dev->source = NULL;
    dev->input_report_buf = NULL;
    dev->input_reports = NULL;
    dev->shutdown_thread = 0;
    dev->vid = 0;
    dev->pid = 0;
    dev->path[0] = '\0';

    /* Thread objects */
    pthread_mutex_init(&dev->mutex, NULL);
    pthread_cond_init(&dev->condition, NULL);
    pthread_barrier_init(&dev->barrier, NULL, 2);
    pthread_barrier_init(&dev->shutdown_barrier, NULL, 2);

    return dev;
}

static void free_hid_device(hid_device *dev)
{
    if (!dev)
        return;

    /* Delete any input reports still left over. */
    struct input_report *rpt = dev->input_reports;
    while (rpt) {
        struct input_report *next = rpt->next;
        free(rpt->data);
        free(rpt);
        rpt = next;
    }

    /* Free the string and the report buffer. The check for NULL
       is necessary here as CFRelease() doesn't handle NULL like
       free() and others do. */
    if (dev->run_loop_mode)
        CFRelease(dev->run_loop_mode);
    if (dev->source)
        CFRelease(dev->source);
    free(dev->input_report_buf);

    /* Clean up the thread objects */
    pthread_barrier_destroy(&dev->shutdown_barrier);
    pthread_barrier_destroy(&dev->barrier);
    pthread_cond_destroy(&dev->condition);
    pthread_mutex_destroy(&dev->mutex);

    /* Free the structure itself. */
    free(dev);
}

static struct hid_api_version api_version = {
    .major = HID_API_VERSION_MAJOR,
    .minor = HID_API_VERSION_MINOR,
    .patch = HID_API_VERSION_PATCH
};

static	IOHIDManagerRef hid_mgr = 0x0;
static	int is_macos_10_10_or_greater = 0;


#if 0
static void register_error(hid_device *dev, const char *op)
{

}
#endif

static CFArrayRef get_array_property(IOHIDDeviceRef device, CFStringRef key)
{
    CFTypeRef ref = IOHIDDeviceGetProperty(device, key);
    if (ref != NULL && CFGetTypeID(ref) == CFArrayGetTypeID()) {
        return (CFArrayRef)ref;
    } else {
        return NULL;
    }
}

static int32_t get_int_property(IOHIDDeviceRef device, CFStringRef key)
{
    CFTypeRef ref;
    int32_t value;

    ref = IOHIDDeviceGetProperty(device, key);
    if (ref) {
        if (CFGetTypeID(ref) == CFNumberGetTypeID()) {
            CFNumberGetValue((CFNumberRef) ref, kCFNumberSInt32Type, &value);
            return value;
        }
    }
    return 0;
}

static CFArrayRef get_usage_pairs(IOHIDDeviceRef device)
{
    return get_array_property(device, CFSTR(kIOHIDDeviceUsagePairsKey));
}

static unsigned short get_vendor_id(IOHIDDeviceRef device)
{
    return get_int_property(device, CFSTR(kIOHIDVendorIDKey));
}

static unsigned short get_product_id(IOHIDDeviceRef device)
{
    return get_int_property(device, CFSTR(kIOHIDProductIDKey));
}

static int32_t get_max_report_length(IOHIDDeviceRef device)
{
    return get_int_property(device, CFSTR(kIOHIDMaxInputReportSizeKey));
}

static int get_string_property(IOHIDDeviceRef device, CFStringRef prop, wchar_t *buf, size_t len)
{
    CFStringRef str;

    if (!len)
        return 0;

    str = (CFStringRef) IOHIDDeviceGetProperty(device, prop);

    buf[0] = 0;

    if (str) {
        CFIndex str_len = CFStringGetLength(str);
        CFRange range;
        CFIndex used_buf_len;
        CFIndex chars_copied;

        len --;

        range.location = 0;
        range.length = ((size_t) str_len > len)? len: (size_t) str_len;
        chars_copied = CFStringGetBytes(str,
            range,
            kCFStringEncodingUTF32LE,
            (char) '?',
            FALSE,
            (UInt8*)buf,
            len * sizeof(wchar_t),
            &used_buf_len);

        if (chars_copied <= 0)
            buf[0] = 0;
        else
            buf[chars_copied] = 0;

        return 0;
    }
    else
        return -1;

}

static int get_serial_number(IOHIDDeviceRef device, wchar_t *buf, size_t len)
{
    return get_string_property(device, CFSTR(kIOHIDSerialNumberKey), buf, len);
}

static int get_manufacturer_string(IOHIDDeviceRef device, wchar_t *buf, size_t len)
{
    return get_string_property(device, CFSTR(kIOHIDManufacturerKey), buf, len);
}

static int get_product_string(IOHIDDeviceRef device, wchar_t *buf, size_t len)
{
    return get_string_property(device, CFSTR(kIOHIDProductKey), buf, len);
}


/* Implementation of wcsdup() for Mac. */
static wchar_t *dup_wcs(const wchar_t *s)
{
    size_t len = wcslen(s);
    wchar_t *ret = (wchar_t*) malloc((len+1)*sizeof(wchar_t));
    wcscpy(ret, s);

    return ret;
}

/* hidapi_IOHIDDeviceGetService()
 *
 * Return the io_service_t corresponding to a given IOHIDDeviceRef, either by:
 * - on OS X 10.6 and above, calling IOHIDDeviceGetService()
 * - on OS X 10.5, extract it from the IOHIDDevice struct
 */
static io_service_t hidapi_IOHIDDeviceGetService(IOHIDDeviceRef device)
{
    static void *iokit_framework = NULL;
    typedef io_service_t (*dynamic_IOHIDDeviceGetService_t)(IOHIDDeviceRef device);
    static dynamic_IOHIDDeviceGetService_t dynamic_IOHIDDeviceGetService = NULL;

    /* Use dlopen()/dlsym() to get a pointer to IOHIDDeviceGetService() if it exists.
     * If any of these steps fail, dynamic_IOHIDDeviceGetService will be left NULL
     * and the fallback method will be used.
     */
    if (iokit_framework == NULL) {
        iokit_framework = dlopen("/System/Library/Frameworks/IOKit.framework/IOKit", RTLD_LAZY);

        if (iokit_framework != NULL)
            dynamic_IOHIDDeviceGetService = (dynamic_IOHIDDeviceGetService_t) dlsym(iokit_framework, "IOHIDDeviceGetService");
    }

    if (dynamic_IOHIDDeviceGetService != NULL) {
        /* Running on OS X 10.6 and above: IOHIDDeviceGetService() exists */
        return dynamic_IOHIDDeviceGetService(device);
    }
    else
    {
        /* Running on OS X 10.5: IOHIDDeviceGetService() doesn't exist.
         *
         * Be naughty and pull the service out of the IOHIDDevice.
         * IOHIDDevice is an opaque struct not exposed to applications, but its
         * layout is stable through all available versions of OS X.
         * Tested and working on OS X 10.5.8 i386, x86_64, and ppc.
         */
        struct IOHIDDevice_internal {
            /* The first field of the IOHIDDevice struct is a
             * CFRuntimeBase (which is a private CF struct).
             *
             * a, b, and c are the 3 fields that make up a CFRuntimeBase.
             * See http://opensource.apple.com/source/CF/CF-476.18/CFRuntime.h
             *
             * The second field of the IOHIDDevice is the io_service_t we're looking for.
             */
            uintptr_t a;
            uint8_t b[4];
#if __LP64__
            uint32_t c;
#endif
            io_service_t service;
        };
        struct IOHIDDevice_internal *tmp = (struct IOHIDDevice_internal *) device;

        return tmp->service;
    }
}

static void devMatchingCallback(void * context, IOReturn result, void *sender, IOHIDDeviceRef device)
{
    unsigned short vid, pid;
    io_object_t iokit_dev;
    kern_return_t res;
    io_string_t path;

    if (device == NULL)
    {
        return;
    }

    if (pnp_callback)
    {
        vid = get_vendor_id(device);
        pid = get_product_id(device);

        /* Fill in the path (IOService plane) */
        iokit_dev = hidapi_IOHIDDeviceGetService(device);
        res = IORegistryEntryGetPath(iokit_dev, kIOServicePlane, path);
        if (res == KERN_SUCCESS)
        {
            pnp_callback(vid, pid, path, 0, callback_user_data);
        }
    }
}


static  void devRemovalCallback(void * context, IOReturn result, void *sender, IOHIDDeviceRef device)
{
    hid_device *dev = (hid_device*) context;

    pnp_callback(0,0,0,0,0);
    if (pnp_callback && dev)
    {
        /* Fill in the path (IOService plane) */
        // iokit_dev = hidapi_IOHIDDeviceGetService(device);
        // res = IORegistryEntryGetPath(iokit_dev, kIOServicePlane, path);
        // if (res == KERN_SUCCESS)
        // {
        // 	pnp_callback(vid, pid, path, 1, callback_user_data);
        // }
       printf("vid:pid=%04X:%04X removal\n", dev->vid, dev->pid);
        pnp_callback(dev->vid, dev->pid, dev->path, 1, callback_user_data);
    }
}


// CFMutableDictionaryRef CreateDeviceMatchingDictionary(UInt32 vid, UInt32 pid) {
// 	CFMutableDictionaryRef ret = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
//     if (!ret)
//         return NULL;

// 	CFNumberRef vidNumberRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &vid );
//     if (!vidNumberRef) {
//         CFRelease(ret);
//         return NULL;
//     }

// 	CFDictionarySetValue(ret, CFSTR(kIOHIDVendorIDKey), vidNumberRef);
//     CFRelease(vidNumberRef);

// 	CFNumberRef pidNumberRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &pid);
//     if (!pidNumberRef) {
//         CFRelease(ret);
//         return NULL;
//     }

// 	CFDictionarySetValue(ret, CFSTR(kIOHIDProductIDKey), pidNumberRef);
//     CFRelease(pidNumberRef);

// 	return ret;
// }


/* Initialize the IOHIDManager. Return 0 for success and -1 for failure. */
static int init_hid_manager(void)
{
    /* Initialize all the HID Manager Objects */
    hid_mgr = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
    if (hid_mgr) {
        IOHIDManagerSetDeviceMatching(hid_mgr, NULL);

        // CFMutableDictionaryRef dev1 = CreateDeviceMatchingDictionary(0x2b53, 0x0002);
        // CFMutableDictionaryRef dev2 = CreateDeviceMatchingDictionary(0x2b53, 0x006a);
        // CFMutableDictionaryRef matchesList[] = { dev1, dev2, };
        // CFArrayRef matches = CFArrayCreate(kCFAllocatorDefault, (const void **)matchesList, 2, NULL);

        // IOHIDManagerSetDeviceMatchingMultiple(hid_mgr, matches);

        IOHIDManagerRegisterDeviceMatchingCallback(hid_mgr, devMatchingCallback, 0);
        IOHIDManagerRegisterDeviceRemovalCallback(hid_mgr, devRemovalCallback, 0);

        IOHIDManagerScheduleWithRunLoop(hid_mgr, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
        return 0;
    }

    return -1;
}

HID_API_EXPORT const struct hid_api_version* HID_API_CALL hid_version()
{
    return &api_version;
}

HID_API_EXPORT const char* HID_API_CALL hid_version_str()
{
    return HID_API_VERSION_STR;
}

/* Initialize the IOHIDManager if necessary. This is the public function, and
   it is safe to call this function repeatedly. Return 0 for success and -1
   for failure. */
int HID_API_EXPORT hid_init(void)
{
    if (!hid_mgr) {
        is_macos_10_10_or_greater = (NSAppKitVersionNumber >= 1343); /* NSAppKitVersionNumber10_10 */
        return init_hid_manager();
    }

    /* Already initialized. */
    return 0;
}

int HID_API_EXPORT hid_exit(void)
{
    if (hid_mgr) {
        /* Close the HID manager. */
        IOHIDManagerClose(hid_mgr, kIOHIDOptionsTypeNone);
        CFRelease(hid_mgr);
        hid_mgr = NULL;
    }

    pnp_callback = NULL;
    return 0;
}

static void process_pending_events(void) {
    SInt32 res;
    do {
        res = CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0.001, FALSE);
    } while(res != kCFRunLoopRunFinished && res != kCFRunLoopRunTimedOut);
}

static struct hid_device_info *create_device_info_with_usage(IOHIDDeviceRef dev, int32_t usage_page, int32_t usage)
{
    unsigned short dev_vid;
    unsigned short dev_pid;
    int BUF_LEN = 256;
    wchar_t buf[BUF_LEN];

    struct hid_device_info *cur_dev;
    io_object_t iokit_dev;
    kern_return_t res;
    io_string_t path;

    if (dev == NULL) {
        return NULL;
    }

    cur_dev = (struct hid_device_info *)calloc(1, sizeof(struct hid_device_info));
    if (cur_dev == NULL) {
        return NULL;
    }

    dev_vid = get_vendor_id(dev);
    dev_pid = get_product_id(dev);

    cur_dev->usage_page = usage_page;
    cur_dev->usage = usage;

    /* Fill out the record */
    cur_dev->next = NULL;

    /* Fill in the path (IOService plane) */
    iokit_dev = hidapi_IOHIDDeviceGetService(dev);
    res = IORegistryEntryGetPath(iokit_dev, kIOServicePlane, path);
    if (res == KERN_SUCCESS)
        cur_dev->path = strdup(path);
    else
        cur_dev->path = strdup("");

    /* Serial Number */
    get_serial_number(dev, buf, BUF_LEN);
    cur_dev->serial_number = dup_wcs(buf);

    /* Manufacturer and Product strings */
    get_manufacturer_string(dev, buf, BUF_LEN);
    cur_dev->manufacturer_string = dup_wcs(buf);
    get_product_string(dev, buf, BUF_LEN);
    cur_dev->product_string = dup_wcs(buf);

    /* VID/PID */
    cur_dev->vendor_id = dev_vid;
    cur_dev->product_id = dev_pid;

    /* Release Number */
    cur_dev->release_number = get_int_property(dev, CFSTR(kIOHIDVersionNumberKey));

    /* Interface Number */
    /* We can only retrieve the interface number for USB HID devices.
     * IOKit always seems to return 0 when querying a standard USB device
     * for its interface. */
    int is_usb_hid = get_int_property(dev, CFSTR(kUSBInterfaceClass)) == kUSBHIDClass;
    if (is_usb_hid) {
        /* Get the interface number */
        cur_dev->interface_number = get_int_property(dev, CFSTR(kUSBInterfaceNumber));
    } else {
        cur_dev->interface_number = -1;
    }

    return cur_dev;
}

static struct hid_device_info *create_device_info(IOHIDDeviceRef device)
{
    const int32_t primary_usage_page = get_int_property(device, CFSTR(kIOHIDPrimaryUsagePageKey));
    const int32_t primary_usage = get_int_property(device, CFSTR(kIOHIDPrimaryUsageKey));

    /* Primary should always be first, to match previous behavior. */
    struct hid_device_info *root = create_device_info_with_usage(device, primary_usage_page, primary_usage);
    struct hid_device_info *cur = root;

    if (!root)
        return NULL;

    CFArrayRef usage_pairs = get_usage_pairs(device);

    if (usage_pairs != NULL) {
        struct hid_device_info *next = NULL;
        for (CFIndex i = 0; i < CFArrayGetCount(usage_pairs); i++) {
            CFTypeRef dict = CFArrayGetValueAtIndex(usage_pairs, i);
            if (CFGetTypeID(dict) != CFDictionaryGetTypeID()) {
                continue;
            }

            CFTypeRef usage_page_ref, usage_ref;
            int32_t usage_page, usage;

            if (!CFDictionaryGetValueIfPresent((CFDictionaryRef)dict, CFSTR(kIOHIDDeviceUsagePageKey), &usage_page_ref) ||
                !CFDictionaryGetValueIfPresent((CFDictionaryRef)dict, CFSTR(kIOHIDDeviceUsageKey), &usage_ref) ||
                    CFGetTypeID(usage_page_ref) != CFNumberGetTypeID() ||
                    CFGetTypeID(usage_ref) != CFNumberGetTypeID() ||
                    !CFNumberGetValue((CFNumberRef)usage_page_ref, kCFNumberSInt32Type, &usage_page) ||
                    !CFNumberGetValue((CFNumberRef)usage_ref, kCFNumberSInt32Type, &usage)) {
                    continue;
            }
            if (usage_page == primary_usage_page && usage == primary_usage)
                continue; /* Already added. */

            next = create_device_info_with_usage(device, usage_page, usage);
            cur->next = next;
            if (next != NULL) {
                cur = next;
            }
        }
    }

    return root;
}

struct hid_device_info  HID_API_EXPORT *hid_enumerate(unsigned short vendor_id, unsigned short product_id)
{
    struct hid_device_info *root = NULL; /* return object */
    struct hid_device_info *cur_dev = NULL;
    CFIndex num_devices;
    int i;

    /* Set up the HID Manager if it hasn't been done */
    if (hid_init() < 0)
        return NULL;

    /* give the IOHIDManager a chance to update itself */
    process_pending_events();

    /* Get a list of the Devices */
    CFMutableDictionaryRef matching = NULL;
    if (vendor_id != 0 || product_id != 0) {
        matching = CFDictionaryCreateMutable(kCFAllocatorDefault, kIOHIDOptionsTypeNone, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);

        if (matching && vendor_id != 0) {
            CFNumberRef v = CFNumberCreate(kCFAllocatorDefault, kCFNumberShortType, &vendor_id);
            CFDictionarySetValue(matching, CFSTR(kIOHIDVendorIDKey), v);
            CFRelease(v);
        }

        if (matching && product_id != 0) {
            CFNumberRef p = CFNumberCreate(kCFAllocatorDefault, kCFNumberShortType, &product_id);
            CFDictionarySetValue(matching, CFSTR(kIOHIDProductIDKey), p);
            CFRelease(p);
        }
    }
    IOHIDManagerSetDeviceMatching(hid_mgr, matching);
    if (matching != NULL) {
        CFRelease(matching);
    }

    CFSetRef device_set = IOHIDManagerCopyDevices(hid_mgr);
    if (device_set == NULL) {
        IOHIDManagerSetDeviceMatching(hid_mgr, NULL);
        return NULL;
    }

    /* Convert the list into a C array so we can iterate easily. */
    num_devices = CFSetGetCount(device_set);
    IOHIDDeviceRef *device_array = (IOHIDDeviceRef*) calloc(num_devices, sizeof(IOHIDDeviceRef));
    CFSetGetValues(device_set, (const void **) device_array);

    /* Iterate over each device, making an entry for it. */
    for (i = 0; i < num_devices; i++) {

        IOHIDDeviceRef dev = device_array[i];
        if (!dev) {
            continue;
        }

        struct hid_device_info *tmp = create_device_info(dev);
        if (tmp == NULL) {
            continue;
        }

        if (cur_dev) {
            cur_dev->next = tmp;
        }
        else {
            root = tmp;
        }
        cur_dev = tmp;

        /* move the pointer to the tail of returnd list */
        while (cur_dev->next != NULL) {
            cur_dev = cur_dev->next;
        }
    }

    IOHIDManagerSetDeviceMatching(hid_mgr, NULL);
    free(device_array);
    CFRelease(device_set);

    return root;
}

void  HID_API_EXPORT hid_free_enumeration(struct hid_device_info *devs)
{
    /* This function is identical to the Linux version. Platform independent. */
    struct hid_device_info *d = devs;
    while (d) {
        struct hid_device_info *next = d->next;
        free(d->path);
        free(d->serial_number);
        free(d->manufacturer_string);
        free(d->product_string);
        free(d);
        d = next;
    }
}

hid_device * HID_API_EXPORT hid_open(unsigned short vendor_id, unsigned short product_id, const wchar_t *serial_number)
{
    /* This function is identical to the Linux version. Platform independent. */
    struct hid_device_info *devs, *cur_dev;
    const char *path_to_open = NULL;
    hid_device * handle = NULL;

    devs = hid_enumerate(vendor_id, product_id);
    cur_dev = devs;
    while (cur_dev) {
        if (cur_dev->vendor_id == vendor_id &&
            cur_dev->product_id == product_id) {
            if (serial_number) {
                if (wcscmp(serial_number, cur_dev->serial_number) == 0) {
                    path_to_open = cur_dev->path;
                    break;
                }
            }
            else {
                path_to_open = cur_dev->path;
                break;
            }
        }
        cur_dev = cur_dev->next;
    }

    if (path_to_open) {
        /* Open the device */
        handle = hid_open_path(path_to_open);
    }

    hid_free_enumeration(devs);

    return handle;
}

static void hid_device_removal_callback(void *context, IOReturn result, void *sender)
{
    /* Stop the Run Loop for this device. */
    hid_device *d = (hid_device*) context;

    (void) result;
    (void) sender;

    if (d == NULL)
    {
        return;
    }
    printf("vid:pid=%04X:%04X remhhhhhhhhhhoval\n", d->vid, d->pid);
    //d->disconnected = 1;
    CFRunLoopStop(d->run_loop);

    // 不是在主线程，会卡死
     if (pnp_callback)
     {
        pnp_callback(d->vid, d->pid, d->path, 1, callback_user_data);
     }
}

/* The Run Loop calls this function for each input report received.
   This function puts the data into a linked list to be picked up by
   hid_read(). */
static void hid_report_callback(void *context, IOReturn result, void *sender,
                         IOHIDReportType report_type, uint32_t report_id,
                         uint8_t *report, CFIndex report_length)
{
    (void) result;
    (void) sender;
    (void) report_type;
    (void) report_id;

    struct input_report *rpt;
    hid_device *dev = (hid_device*) context;

    /* Make a new Input Report object */
    rpt = (struct input_report*) calloc(1, sizeof(struct input_report));
    rpt->data = (uint8_t*) calloc(1, report_length);
    memcpy(rpt->data, report, report_length);
    rpt->len = report_length;
    rpt->next = NULL;

    /* Lock this section */
    pthread_mutex_lock(&dev->mutex);

    /* Attach the new report object to the end of the list. */
    if (dev->input_reports == NULL) {
        /* The list is empty. Put it at the root. */
        dev->input_reports = rpt;
    }
    else {
        /* Find the end of the list and attach. */
        struct input_report *cur = dev->input_reports;
        int num_queued = 0;
        while (cur->next != NULL) {
            cur = cur->next;
            num_queued++;
        }
        cur->next = rpt;

        /* Pop one off if we've reached 30 in the queue. This
           way we don't grow forever if the user never reads
           anything from the device. */
        if (num_queued > 30) {
            return_data(dev, NULL, 0);
        }
    }

    /* Signal a waiting thread that there is data. */
    pthread_cond_signal(&dev->condition);

    /* Unlock */
    pthread_mutex_unlock(&dev->mutex);

}

/* This gets called when the read_thread's run loop gets signaled by
   hid_close(), and serves to stop the read_thread's run loop. */
static void perform_signal_callback(void *context)
{
    hid_device *dev = (hid_device*) context;
    CFRunLoopStop(dev->run_loop); /*TODO: CFRunLoopGetCurrent()*/
}

static void *read_thread(void *param)
{
    hid_device *dev = (hid_device*) param;
    SInt32 code;

    /* Move the device's run loop to this thread. */
    IOHIDDeviceScheduleWithRunLoop(dev->device_handle, CFRunLoopGetCurrent(), dev->run_loop_mode);

    /* Create the RunLoopSource which is used to signal the
       event loop to stop when hid_close() is called. */
    CFRunLoopSourceContext ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.version = 0;
    ctx.info = dev;
    ctx.perform = &perform_signal_callback;
    dev->source = CFRunLoopSourceCreate(kCFAllocatorDefault, 0/*order*/, &ctx);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), dev->source, dev->run_loop_mode);

    /* Store off the Run Loop so it can be stopped from hid_close()
       and on device disconnection. */
    dev->run_loop = CFRunLoopGetCurrent();

    /* Notify the main thread that the read thread is up and running. */
    pthread_barrier_wait(&dev->barrier);

    /* Run the Event Loop. CFRunLoopRunInMode() will dispatch HID input
       reports into the hid_report_callback(). */
    while (!dev->shutdown_thread && !dev->disconnected) {
        code = CFRunLoopRunInMode(dev->run_loop_mode, 1000/*sec*/, FALSE);
        /* Return if the device has been disconnected */
        if (code == kCFRunLoopRunFinished) {
            dev->disconnected = 1;
            break;
        }


        /* Break if The Run Loop returns Finished or Stopped. */
        if (code != kCFRunLoopRunTimedOut &&
            code != kCFRunLoopRunHandledSource) {
            /* There was some kind of error. Setting
               shutdown seems to make sense, but
               there may be something else more appropriate */
            dev->shutdown_thread = 1;
            break;
        }
    }

    /* Now that the read thread is stopping, Wake any threads which are
       waiting on data (in hid_read_timeout()). Do this under a mutex to
       make sure that a thread which is about to go to sleep waiting on
       the condition actually will go to sleep before the condition is
       signaled. */
    pthread_mutex_lock(&dev->mutex);
    pthread_cond_broadcast(&dev->condition);
    pthread_mutex_unlock(&dev->mutex);

    /* Wait here until hid_close() is called and makes it past
       the call to CFRunLoopWakeUp(). This thread still needs to
       be valid when that function is called on the other thread. */
    pthread_barrier_wait(&dev->shutdown_barrier);

    return NULL;
}

/* hid_open_path()
 *
 * path must be a valid path to an IOHIDDevice in the IOService plane
 * Example: "IOService:/AppleACPIPlatformExpert/PCI0@0/AppleACPIPCI/EHC1@1D,7/AppleUSBEHCI/PLAYSTATION(R)3 Controller@fd120000/IOUSBInterface@0/IOUSBHIDDriver"
 */
hid_device * HID_API_EXPORT hid_open_path(const char *path)
{
    hid_device *dev = NULL;
    io_registry_entry_t entry = MACH_PORT_NULL;
    IOReturn ret = kIOReturnInvalid;

    /* Set up the HID Manager if it hasn't been done */
    if (hid_init() < 0)
        goto return_error;

    dev = new_hid_device();

    /* Get the IORegistry entry for the given path */
    entry = IORegistryEntryFromPath(kIOMasterPortDefault, path);
    if (entry == MACH_PORT_NULL) {
        /* Path wasn't valid (maybe device was removed?) */
        goto return_error;
    }

    /* Create an IOHIDDevice for the entry */
    dev->device_handle = IOHIDDeviceCreate(kCFAllocatorDefault, entry);
    if (dev->device_handle == NULL) {
        /* Error creating the HID device */
        goto return_error;
    }

    /* Open the IOHIDDevice */
    ret = IOHIDDeviceOpen(dev->device_handle, kIOHIDOptionsTypeSeizeDevice);
    if (ret == kIOReturnSuccess) {
        char str[32];

        /* Create the buffers for receiving data */
        dev->max_input_report_len = (CFIndex) get_max_report_length(dev->device_handle);
        dev->input_report_buf = (uint8_t*) calloc(dev->max_input_report_len, sizeof(uint8_t));

        dev->vid = get_vendor_id(dev->device_handle);
        dev->pid = get_product_id(dev->device_handle);
        strcpy(dev->path, path);

        /* Create the Run Loop Mode for this device.
           printing the reference seems to work. */
        sprintf(str, "HIDAPI_%p", (void*) dev->device_handle);
        dev->run_loop_mode =
            CFStringCreateWithCString(NULL, str, kCFStringEncodingASCII);

        /* Attach the device to a Run Loop */
        IOHIDDeviceRegisterInputReportCallback(
            dev->device_handle, dev->input_report_buf, dev->max_input_report_len,
            &hid_report_callback, dev);
        IOHIDDeviceRegisterRemovalCallback(dev->device_handle, hid_device_removal_callback, dev);


        //IOHIDManagerRegisterDeviceRemovalCallback(hid_mgr, devRemovalCallback, dev);

        /* Start the read thread */
        pthread_create(&dev->thread, NULL, read_thread, dev);

        /* Wait here for the read thread to be initialized. */
        pthread_barrier_wait(&dev->barrier);

        IOObjectRelease(entry);
        return dev;
    }
    else {
        goto return_error;
    }

return_error:
    if (dev->device_handle != NULL)
        CFRelease(dev->device_handle);

    if (entry != MACH_PORT_NULL)
        IOObjectRelease(entry);

    free_hid_device(dev);
    return NULL;
}

static int set_report(hid_device *dev, IOHIDReportType type, const unsigned char *data, size_t length)
{
    const unsigned char *data_to_send = data;
    CFIndex length_to_send = length;
    IOReturn res;
    const unsigned char report_id = data[0];

    if (report_id == 0x0) {
        /* Not using numbered Reports.
           Don't send the report number. */
        data_to_send = data+1;
        length_to_send = length-1;
    }

    /* Avoid crash if the device has been unplugged. */
    if (dev->disconnected) {
        return -1;
    }

    res = IOHIDDeviceSetReport(dev->device_handle,
                               type,
                               report_id,
                               data_to_send, length_to_send);

    if (res == kIOReturnSuccess) {
        return length;
    }

    return -1;
}

static int get_report(hid_device *dev, IOHIDReportType type, unsigned char *data, size_t length)
{
    unsigned char *report = data;
    CFIndex report_length = length;
    IOReturn res = kIOReturnSuccess;
    const unsigned char report_id = data[0];

    if (report_id == 0x0) {
        /* Not using numbered Reports.
           Don't send the report number. */
        report = data+1;
        report_length = length-1;
    }

    /* Avoid crash if the device has been unplugged. */
    if (dev->disconnected) {
        return -1;
    }

    res = IOHIDDeviceGetReport(dev->device_handle,
                               type,
                               report_id,
                               report, &report_length);

    if (res == kIOReturnSuccess) {
        if (report_id == 0x0) { /* 0 report number still present at the beginning */
            report_length++;
        }
        return report_length;
    }

    return -1;
}

int HID_API_EXPORT hid_write(hid_device *dev, const unsigned char *data, size_t length)
{
    return set_report(dev, kIOHIDReportTypeOutput, data, length);
}

/* Helper function, so that this isn't duplicated in hid_read(). */
static int return_data(hid_device *dev, unsigned char *data, size_t length)
{
    /* Copy the data out of the linked list item (rpt) into the
       return buffer (data), and delete the liked list item. */
    struct input_report *rpt = dev->input_reports;
    size_t len = (length < rpt->len)? length: rpt->len;
    memcpy(data, rpt->data, len);
    dev->input_reports = rpt->next;
    free(rpt->data);
    free(rpt);
    return len;
}

static int cond_wait(const hid_device *dev, pthread_cond_t *cond, pthread_mutex_t *mutex)
{
    while (!dev->input_reports) {
        int res = pthread_cond_wait(cond, mutex);
        if (res != 0)
            return res;

        /* A res of 0 means we may have been signaled or it may
           be a spurious wakeup. Check to see that there's actually
           data in the queue before returning, and if not, go back
           to sleep. See the pthread_cond_timedwait() man page for
           details. */

        if (dev->shutdown_thread || dev->disconnected)
            return -1;
    }

    return 0;
}

static int cond_timedwait(const hid_device *dev, pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *abstime)
{
    while (!dev->input_reports) {
        int res = pthread_cond_timedwait(cond, mutex, abstime);
        if (res != 0)
            return res;

        /* A res of 0 means we may have been signaled or it may
           be a spurious wakeup. Check to see that there's actually
           data in the queue before returning, and if not, go back
           to sleep. See the pthread_cond_timedwait() man page for
           details. */

        if (dev->shutdown_thread || dev->disconnected)
            return -1;
    }

    return 0;

}

int HID_API_EXPORT hid_read_timeout(hid_device *dev, unsigned char *data, size_t length, int milliseconds)
{
    int bytes_read = -1;

    /* Lock the access to the report list. */
    pthread_mutex_lock(&dev->mutex);

    /* There's an input report queued up. Return it. */
    if (dev->input_reports) {
        /* Return the first one */
        bytes_read = return_data(dev, data, length);
        goto ret;
    }

    /* Return if the device has been disconnected. */
    if (dev->disconnected) {
        bytes_read = -1;
        goto ret;
    }

    if (dev->shutdown_thread) {
        /* This means the device has been closed (or there
           has been an error. An error code of -1 should
           be returned. */
        bytes_read = -1;
        goto ret;
    }

    /* There is no data. Go to sleep and wait for data. */

    if (milliseconds == -1) {
        /* Blocking */
        int res;
        res = cond_wait(dev, &dev->condition, &dev->mutex);
        if (res == 0)
            bytes_read = return_data(dev, data, length);
        else {
            /* There was an error, or a device disconnection. */
            bytes_read = -1;
        }
    }
    else if (milliseconds > 0) {
        /* Non-blocking, but called with timeout. */
        int res;
        struct timespec ts;
        struct timeval tv;
        gettimeofday(&tv, NULL);
        TIMEVAL_TO_TIMESPEC(&tv, &ts);
        ts.tv_sec += milliseconds / 1000;
        ts.tv_nsec += (milliseconds % 1000) * 1000000;
        if (ts.tv_nsec >= 1000000000L) {
            ts.tv_sec++;
            ts.tv_nsec -= 1000000000L;
        }

        res = cond_timedwait(dev, &dev->condition, &dev->mutex, &ts);
        if (res == 0)
            bytes_read = return_data(dev, data, length);
        else if (res == ETIMEDOUT)
            bytes_read = 0;
        else
            bytes_read = -1;
    }
    else {
        /* Purely non-blocking */
        bytes_read = 0;
    }

ret:
    /* Unlock */
    pthread_mutex_unlock(&dev->mutex);
    return bytes_read;
}

int HID_API_EXPORT hid_read(hid_device *dev, unsigned char *data, size_t length)
{
    return hid_read_timeout(dev, data, length, (dev->blocking)? -1: 0);
}

int HID_API_EXPORT hid_set_nonblocking(hid_device *dev, int nonblock)
{
    /* All Nonblocking operation is handled by the library. */
    dev->blocking = !nonblock;

    return 0;
}

int HID_API_EXPORT hid_send_feature_report(hid_device *dev, const unsigned char *data, size_t length)
{
    return set_report(dev, kIOHIDReportTypeFeature, data, length);
}

int HID_API_EXPORT hid_get_feature_report(hid_device *dev, unsigned char *data, size_t length)
{
    return get_report(dev, kIOHIDReportTypeFeature, data, length);
}

int HID_API_EXPORT HID_API_CALL hid_get_input_report(hid_device *dev, unsigned char *data, size_t length)
{
    return get_report(dev, kIOHIDReportTypeInput, data, length);
}

void HID_API_EXPORT hid_close(hid_device *dev)
{
    if (!dev)
        return;

    /* Disconnect the report callback before close.
       See comment below.
    */
    if (is_macos_10_10_or_greater || !dev->disconnected) {
        IOHIDDeviceRegisterInputReportCallback(
            dev->device_handle, dev->input_report_buf, dev->max_input_report_len,
            NULL, dev);
        IOHIDDeviceRegisterRemovalCallback(dev->device_handle, NULL, dev);
        IOHIDDeviceUnscheduleFromRunLoop(dev->device_handle, dev->run_loop, dev->run_loop_mode);
        IOHIDDeviceScheduleWithRunLoop(dev->device_handle, CFRunLoopGetMain(), kCFRunLoopDefaultMode);
    }

    /* Cause read_thread() to stop. */
    dev->shutdown_thread = 1;

    /* Wake up the run thread's event loop so that the thread can exit. */
    CFRunLoopSourceSignal(dev->source);
    CFRunLoopWakeUp(dev->run_loop);

    //删除线程
    /* Notify the read thread that it can shut down now. */
    //pthread_barrier_wait(&dev->shutdown_barrier);

    /* Wait for read_thread() to end. */
    //pthread_join(dev->thread, NULL);

    /* Close the OS handle to the device, but only if it's not
       been unplugged. If it's been unplugged, then calling
       IOHIDDeviceClose() will crash.

       UPD: The crash part was true in/until some version of macOS.
       Starting with macOS 10.15, there is an opposite effect in some environments:
       crash happenes if IOHIDDeviceClose() is not called.
       Not leaking a resource in all tested environments.
    */
    if (is_macos_10_10_or_greater || !dev->disconnected) {
        IOHIDDeviceClose(dev->device_handle, kIOHIDOptionsTypeSeizeDevice);
    }

    /* Clear out the queue of received reports. */
    pthread_mutex_lock(&dev->mutex);
    while (dev->input_reports) {
        return_data(dev, NULL, 0);
    }
    pthread_mutex_unlock(&dev->mutex);
    CFRelease(dev->device_handle);

    free_hid_device(dev);
}

int HID_API_EXPORT_CALL hid_get_manufacturer_string(hid_device *dev, wchar_t *string, size_t maxlen)
{
    return get_manufacturer_string(dev->device_handle, string, maxlen);
}

int HID_API_EXPORT_CALL hid_get_product_string(hid_device *dev, wchar_t *string, size_t maxlen)
{
    return get_product_string(dev->device_handle, string, maxlen);
}

int HID_API_EXPORT_CALL hid_get_serial_number_string(hid_device *dev, wchar_t *string, size_t maxlen)
{
    return get_serial_number(dev->device_handle, string, maxlen);
}

int HID_API_EXPORT_CALL hid_get_indexed_string(hid_device *dev, int string_index, wchar_t *string, size_t maxlen)
{
    (void) dev;
    (void) string_index;
    (void) string;
    (void) maxlen;

    /* TODO: */

    return 0;
}


HID_API_EXPORT const wchar_t * HID_API_CALL  hid_error(hid_device *dev)
{
    (void) dev;
    /* TODO: */

    return L"hid_error is not implemented yet";
}


HID_API_EXPORT void HID_API_CALL hid_register_device_notification(hid_pnp_callback fn, void* user_data)
{
    pnp_callback = fn;
    callback_user_data = user_data;
}


HID_API_EXPORT int HID_API_CALL hid_max_input_report_len(hid_device *device)
{
    if (device)
        return (device->max_input_report_len + 1);
    else
        return 0;
}


#if 0
static int32_t get_location_id(IOHIDDeviceRef device)
{
    return get_int_property(device, CFSTR(kIOHIDLocationIDKey));
}

static int32_t get_usage(IOHIDDeviceRef device)
{
    int32_t res;
    res = get_int_property(device, CFSTR(kIOHIDDeviceUsageKey));
    if (!res)
        res = get_int_property(device, CFSTR(kIOHIDPrimaryUsageKey));
    return res;
}

static int32_t get_usage_page(IOHIDDeviceRef device)
{
    int32_t res;
    res = get_int_property(device, CFSTR(kIOHIDDeviceUsagePageKey));
    if (!res)
        res = get_int_property(device, CFSTR(kIOHIDPrimaryUsagePageKey));
    return res;
}

static int get_transport(IOHIDDeviceRef device, wchar_t *buf, size_t len)
{
    return get_string_property(device, CFSTR(kIOHIDTransportKey), buf, len);
}


int main(void)
{
    IOHIDManagerRef mgr;
    int i;

    mgr = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
    IOHIDManagerSetDeviceMatching(mgr, NULL);
    IOHIDManagerOpen(mgr, kIOHIDOptionsTypeNone);

    CFSetRef device_set = IOHIDManagerCopyDevices(mgr);

    CFIndex num_devices = CFSetGetCount(device_set);
    IOHIDDeviceRef *device_array = calloc(num_devices, sizeof(IOHIDDeviceRef));
    CFSetGetValues(device_set, (const void **) device_array);

    for (i = 0; i < num_devices; i++) {
        IOHIDDeviceRef dev = device_array[i];
        printf("Device: %p\n", dev);
        printf("  %04hx %04hx\n", get_vendor_id(dev), get_product_id(dev));

        wchar_t serial[256], buf[256];
        char cbuf[256];
        get_serial_number(dev, serial, 256);


        printf("  Serial: %ls\n", serial);
        printf("  Loc: %ld\n", get_location_id(dev));
        get_transport(dev, buf, 256);
        printf("  Trans: %ls\n", buf);
        make_path(dev, cbuf, 256);
        printf("  Path: %s\n", cbuf);

    }

    return 0;
}
#endif


#endif



