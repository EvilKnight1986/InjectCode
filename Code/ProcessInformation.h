//*************************************************************************
//  
//  Copyright (c) all 2014  All rights reserved
//  D a t e  : 2014.9.11
//  作  者 : 
//  版  本 : 0.1
//  功  能 : 取得相关的进程信息
//  说  明 : 
//  备  注 :
//
//  修改记录:
//  日   期       版本    修改人              修改内容
// 2014/9/11 0.1      EvilKnight        创建
//  YYYY/MM/DD    X.Y     <作者或修改者名>    <修改内容>
//
//*************************************************************************

#ifndef PROCESSINFORMATION__H
#define PROCESSINFORMATION__H

#include <windows.h>

// 通过进程名取得进程列表
ULONG
GetProcessListByProcessName(
                                  __in_z CONST PTCHAR pProcessName,
                                   __in_bcount(uSize) CONST PDWORD pArrayPID,
                                    __in CONST ULONG uSize) ;

#endif

