#ifndef _H_PROTOCOL__H__
#define _H_PROTOCOL__H__

#include "CommunicateInterface.h"



// 应答标志
#define REPLY_OK			0xFF
#define REPLY_DEFAULT		0x00
#define REPLY_ERR_COMMAND	0x01
#define REPLY_ERR_INDEX		0x02
#define REPLY_ERR_CRC		0x03
#define REPLY_ERR_RESERVED1	0x04
#define REPLY_ERR_RESERVED2	0x05
#define	REPLY_ERR_OTHER		0xA0


//
// 通讯协议的抽象类，不同的通讯协议需要派生出不同的派生类
//
class CProtocol : public ICommProtocol
{
public:
	CProtocol(int nPackLen);
	virtual ~CProtocol(void);
    

    /**
     @brief 验证应答数据包
     @param[in] package -- 要验证的应答包
     @param[in] command -- 期待应答的命令编码，例如：下发0x02的命令，期待应答的命令编码是0x03
     @return  bool_t
     @sa   
     @retval  验证成功返回TRUE, 失败返回FALSE
     @note  
     @author hzf
     */
    virtual bool CheckReply(PACKDATA& package, uint8_t command);
        
    
    /**
     @brief 计算数据包的CRC值
     @param[in out] data:数据包
     @return  void
     @sa   
     @retval 
     @note
     @author hzf
    */
    virtual void CalcCRC(PACKDATA &data);


    /**
     @brief CRC值的验证
     @param[in out] data:数据包
     @return  void
     @note
     @author hzf
    */
    virtual bool maonoCalcCRC(mypc_random_write &data);


    /**
     @brief 加密数据
     @param[in out] data:要加密的数据包
     @return  void
     @sa   
     @retval 
     @note 
     @author hzf
    */
    virtual void Encrypt(PACKDATA &data);


    /**
     @brief 解密数据
     @param[in out] data:要解密的数据包
     @return  void
     @sa   
     @retval 
     @note 
     @author hzf
    */
    virtual void Decrypt(PACKDATA &data);


    /**
     @brief 获取数据包中有效数据的字节数
     @return  int
     @sa   
     @retval 返回数据包中有效数据的字节数
     @note 
     @author hzf
    */
    virtual int GetPackLength();


	/**
     @brief 设置数据包中有效数据的字节数
     @return  int nPackLength 数据包长度
     @sa   
     @retval 
     @note 
     @author hj
    */
    virtual void SetPackLength(int nPackLength);

protected:
    int                 m_nPackLen;
    ICommEncrypt        *m_pCommEncrypt;

};

#endif//_H_PROTOCOL__H__
