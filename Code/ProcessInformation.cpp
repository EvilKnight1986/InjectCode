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
// 2014/9/11 0.1      EvilKnight        实现GetProcessListByProcessName
//  YYYY/MM/DD    X.Y     <作者或修改者名>    <修改内容>
//
//*************************************************************************
#include "ProcessInformation.h"
#include "ErrorInformation.h"
#include <tlhelp32.h>
#include <Windows.h>
#include <tchar.h>

/*******************************************************************************
*
*   函 数 名 : GetProcessListByProcessName
*  功能描述 : 通过进程名取得进程列表
*  参数列表 : pProcessName     --      指定查找的进程名
*                   pArrayPID            --       用来保存与指定名相同的进程PID
*                   uSize                    --       pArrayPID数组大小，非byte,就是可以存多少pid
*   说      明 :  如果pArrayPID没有指定的话，函数依然可以工作，如果在程序中是通过申请
*                    内存保存返回结果的话，可以先调用一次这个函数获取进程数，以便申请要应
*                    大小的内存
*  返回结果 :  返回与指定进程名相同的进程数
*
*******************************************************************************/
ULONG GetProcessListByProcessName(
                            __in_z CONST PTCHAR pProcessName,
                            __in_bcount(uSize) CONST PDWORD pArrayPID,
                            __in CONST ULONG uSize)
{
        ULONG uProcessCount(0) ;
        HANDLE hProcessSnap = INVALID_HANDLE_VALUE ;
        PROCESSENTRY32 pe32;

        if (NULL == pProcessName)
        {
                OutputDebugString(TEXT("GetProcessListByProcessName pProcessName can't NULL!\r\n")) ;
                return 0 ;
        }

        __try
        {
                hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
                if (INVALID_HANDLE_VALUE == hProcessSnap)
                {
                        OutputErrorInformation(TEXT("GetProcessListByProcessName"),
                                                                TEXT("CreateToolhelp32Snapshot")) ;
                        __leave ;
                }

                pe32.dwSize = sizeof(PROCESSENTRY32) ;

                if( !Process32First( hProcessSnap, &pe32 ))
                {
                        OutputErrorInformation(TEXT("GetProcessListByProcessName"),
                                                                TEXT("Process32First")) ;
                        __leave ;
                }

                do 
                {
                        // 最长就是比较MAX_PATH个字符了
                        if (0 == _tcsnicmp(pe32.szExeFile, pProcessName, MAX_PATH))
                        {
                                if (NULL != pArrayPID && uProcessCount < uSize)
                                {
                                        pArrayPID[uProcessCount] = pe32.th32ProcessID ;
                                }
                                uProcessCount++ ;
                        }

                } while (Process32Next( hProcessSnap, &pe32 ));
        }

        __finally
        {
                if (INVALID_HANDLE_VALUE != hProcessSnap)
                {
                        CloseHandle(hProcessSnap) ;
                        hProcessSnap = INVALID_HANDLE_VALUE ;
                }
        }
        return uProcessCount ;
}