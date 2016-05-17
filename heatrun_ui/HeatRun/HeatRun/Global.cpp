#include "stdafx.h"



BYTE Comm_2NumCharToHex ( BYTE uChar_H , BYTE uChar_L )
{
	if ( ( uChar_H >= '0' ) && ( uChar_H <= '9' ) )
		uChar_H -= 0x30 ;
	else
	{
		if ( ( uChar_H >= 'A' ) && ( uChar_H <= 'F' ) )
			uChar_H -= 0x37 ;
		else
		{
			if ( ( uChar_H >= 'a' ) && ( uChar_H <= 'f' ) )
				uChar_H -= 0x57 ;
			else
				return (0) ;
		}
	}

	if ( ( uChar_L >= '0' ) && ( uChar_L <= '9' ) )
		uChar_L -= 0x30 ;
	else
	{
		if ( ( uChar_L >= 'A' ) && ( uChar_L <= 'F' ) )
			uChar_L -= 0x37 ;
		else
		{
			if ( ( uChar_L >= 'a' ) && ( uChar_L <= 'f' ) )
				uChar_L -= 0x57 ;
			else
				return (0) ;
		}
	}

	return ( 16*uChar_H + uChar_L ) ;
}

CString HexToDem(CString str)
{
	int dem = 0;

	for (int i=0;i<str.GetLength();i++)
	{
		dem = dem*16;
		if ((str[i]<='9')&&(str[i])>='0')
			dem += str[i] - '0';
		else if ((str[i]<='F')&&(str[i])>='A')
			dem += str[i] - 'A' + 10;
		else if ((str[i]<='f')&&(str[i])>='a')
			dem += str[i] - 'a' + 10;
		else
			return FALSE;
	}
	str.Format(_T("%d"), dem);
	return str;
}

CString Two2One(char a, char b)
{
	CString stra(a);
	CString strb(b);
	CString strSum;

	strSum = stra + strb;

	return strSum;

}

//轮循判断条件（对暂时没有命令的轮循条件进行提示）
BOOL PatternSwitch(INT nSelect)
{
	BOOL nSelectReturn = FALSE;
	if (nSelect==2 || nSelect==3 || nSelect==4 || nSelect==6 || nSelect==7 || nSelect==84 || nSelect==85 || nSelect==86 || nSelect==87)
	{
		nSelectReturn = TRUE;
		return nSelectReturn;
	}
	else if (nSelect>23 && nSelect<36)
	{
		nSelectReturn = TRUE;
		return nSelectReturn;
	}
	else
		return nSelectReturn;
}