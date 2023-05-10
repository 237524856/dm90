#include "Protocol.h"
#include "CRC.h"
#include <QDebug>

CProtocol::CProtocol(int nPackLen)
    : m_nPackLen(nPackLen)
    , m_pCommEncrypt(NULL)
{
}

CProtocol::~CProtocol(void)
{

}

bool CProtocol::CheckReply(PACKDATA& package, uint8_t command)
{
   // 验证命令编码
//	if ( package.command != command )
//	{
//        //log_error("CheckReply error: reply cmd 0x%02x != 0x%02x", package.command, command);
//		return false;
//	}

	// 验证应答标志
//	if ( package.reply_flag != REPLY_OK)
//	{
//        //log_error("CheckReply error: reply falg 0x%02x != 0x%02x  cmd=0x%02x", package.reply_flag, REPLY_OK, package.command);
//		return false;
//	}

	// 验证应答包的CRC，m_nPackLen-1-1，除去首字节报文ID和最后一个字节存放CRC值
//	uint8_t crc = CCRC::Crc8(package.buff, m_nPackLen-1-1);
//	if ( crc != package.buff[m_nPackLen-1-1] )
//	{
//        //log_error("CheckReply error: reply crc 0x%02x != 0x%02x  cmd=0x%02x", package.buff[m_nPackLen-1-1], crc, package.command);
//		return false;
//	}

	return true;
}


void CProtocol::CalcCRC(PACKDATA &data)
{
    // 计算CRC，m_nPackLen-1-1，除去首字节报文ID和最后一个字节存放CRC值
    data.buff[m_nPackLen - 1 -1] = CCRC::Crc8(data.buff, m_nPackLen - 1 - 1);
}
bool CProtocol::maonoCalcCRC(mypc_random_write &data){


    const uint8_t *buf = (uint8_t *)&data;
    uint16_t CP_CRC_16 = 0;
    int size =0;
    if(data.command == 0x04){
        size = 10;
    }

    for(int i=1;i<size;i++){

        CP_CRC_16 += buf[i];
       //CP_CRC_16 += (*buf++);
    }
//    while(size--)
//        CP_CRC_16 += (*buf++);
        CP_CRC_16 = ~CP_CRC_16 + 1;
        uint8_t crc_L = (CP_CRC_16) & 0xFF;
        uint8_t crc_H = (CP_CRC_16 >> 8) & 0xFF;
        //qDebug()<<"ccccccccc"<<QString::number(crc_L,16)<<QString::number(crc_H,16);
        if(crc_L == data.crc_L && crc_H == data.crc_H){
            return true;
        }


    return false;
}

void CProtocol::Encrypt(PACKDATA &data)
{
    if ( m_pCommEncrypt )
    {
        m_pCommEncrypt->Encrypt(data);
    }
}


void CProtocol::Decrypt(PACKDATA &data)
{
    if ( m_pCommEncrypt )
    {
        m_pCommEncrypt->Decrypt(data);
    }
}

int CProtocol::GetPackLength()
{
    return m_nPackLen;
}

void CProtocol::SetPackLength(int nPackLength)
{
	m_nPackLen = nPackLength;
}

