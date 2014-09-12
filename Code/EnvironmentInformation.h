//*************************************************************************
//  
//  Copyright (c) all 2014  All rights reserved
//  D a t e  : 2014.9.11
//  作  者 : 
//  版  本 : 0.1
//  功  能 : 运行环境相关信息函数
//  说  明 : 
//  备  注 :
//
//  修改记录:
//  日   期       版本    修改人              修改内容
// 2014/9/11 0.1      EvilKnight        创建
//  YYYY/MM/DD    X.Y     <作者或修改者名>    <修改内容>
//
//*************************************************************************

#ifndef ENVIRONMENTINFORMATION__H
#define ENVIRONMENTINFORMATION__H

#include <Windows.h>

// 取得操作系统位数
ULONG   GetOSBit(VOID) ;

// 取得指定进程位数，32位或者64位，如果不能取到正常的值，返回0
ULONG   GetProcessBit(__in CONST DWORD dwPID) ;

// 取得当前进程的位数
ULONG   GetMyselfBit(VOID) ;

// 取得可执行文件的位数
ULONG   GetPEFileBit(__in_z CONST PTCHAR pPEFilePath) ;

// 通过内存中的PE头来取得程序位数
ULONG   GetBitByPEHeader(__in_bcount(uSize) CONST PVOID pPE,
                                                __in CONST ULONG uSize) ;

#endif