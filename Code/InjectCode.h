//*************************************************************************
//  
//  Copyright (c) all 2014  All rights reserved
//  D a t e  : 2014.9.11
//  作  者 : 
//  版  本 : 0.1
//  功  能 : 将指定的dll注入到目标进程
//  说  明 : 
//  备  注 :
//
//  修改记录:
//  日   期       版本    修改人              修改内容
// 2014/9/11 0.1      EvilKnight        创建
//  YYYY/MM/DD    X.Y     <作者或修改者名>    <修改内容>
//
//*************************************************************************
#ifndef INJECTCODE__H
#define INJECTCODE__H

#include <Windows.h>

// 将dll注入到指定进程
BOOL Inject(__in CONST DWORD dwPID,
                        __in_z CONST PTCHAR pDllPath) ;

// 将dll注入到指定进程名的进程中
BOOL Inject(__in_z CONST PTCHAR pProcessName,
                        __in_z CONST PTCHAR pDllPath) ;

#endif