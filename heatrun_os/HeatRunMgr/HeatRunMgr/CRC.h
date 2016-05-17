#include <Windows.h>
#ifndef _CRC_H_
#define _CRC_H_

/**********************************************************************
 * 函数名称： CRC8CalcContinuation
 * 功能描述： CRC-8算法
 * 输入参数： pacData:数据地址 wCount:数据长度 
 * 输出参数： cCRC:1字节CRC校验值
 * 返 回 值： cCRC
 * 修改日期        修改人	      修改内容
 * -----------------------------------------------
 * 2015/10/30	   陈长青		  创建
 ***********************************************************************/
BYTE CRC8CalcContinuation(PBYTE pacData, WORD wCount, BYTE cCRC);

/**********************************************************************
 * 函数名称： CRC16CalcContinuation
 * 功能描述： CRC-16计算
 * 输入参数： pacData:数据地址 wCount:数据长度 
 * 输出参数： wCRC:2字节CRC校验值
 * 返 回 值： wCRC
 * 修改日期        修改人	      修改内容
 * -----------------------------------------------
 * 2015/10/30	   陈长青		  创建
 ***********************************************************************/
WORD CRC16CalcContinuation(PBYTE pacData, WORD wCount, WORD wCRC);

#endif // _SYSTEMINIT_H_