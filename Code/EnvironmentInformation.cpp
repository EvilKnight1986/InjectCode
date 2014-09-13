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
// 2014/9/11 0.1      EvilKnight        GeOSBit
// 2014/9/12 0.1	ray                   GetProcessBit()
//  YYYY/MM/DD    X.Y     <作者或修改者名>    <修改内容>
//
//*************************************************************************
#include "EnvironmentInformation.h"
#include "ErrorInformation.h"
#include <Psapi.h>
#include <tchar.h>
#include <strsafe.h>
#pragma comment(lib, "psapi.lib")

typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(
                                        GetModuleHandle(TEXT("kernel32")),"IsWow64Process");

/*******************************************************************************
*
*   函 数 名 : GetOSBit
*  功能描述 : 取得操作系统位数
*  参数列表 : 无
*   说      明 : 
*  返回结果 :  如果成功，返回操作系统位数, 否则返回0
*
*******************************************************************************/
ULONG   GetOSBit(VOID)
{
        SYSTEM_INFO si = {0} ;
        GetNativeSystemInfo(&si) ;
        if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
                si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64 )
        {
                return 64;
        }
        return 32;
}


/*******************************************************************************
*
*   函 数 名 : GetProcessBit
*  功能描述 : 取得指定进程位数
*  参数列表 : dwPID          --             目标进程ID
*   说      明 : 
*  返回结果 :  如果成功，返回进程位数, 否则返回0
*
*******************************************************************************/
ULONG   GetProcessBit(__in CONST DWORD dwPID)
{
        ULONG uResult =  0 ;
        BOOL bIsWow64 = FALSE ;
        HANDLE hProcess = INVALID_HANDLE_VALUE ;
        TCHAR szFileName[MAX_PATH] = {0} ;
        TCHAR szDriveName[MAX_PATH] = {0} ;

        __try
        {
                hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, 
                                                                FALSE,
                                                                dwPID) ;

                if (INVALID_HANDLE_VALUE == hProcess)
                {
                        OutputErrorInformation(TEXT("GetProcessBit"), TEXT("OpenProcess")) ;
                        __leave ;
                }

                // 可以调用IsWow64Process
                //if (NULL != fnIsWow64Process)
                //{
                //        // 如果成功了，闪人
                //        if (fnIsWow64Process(hProcess, &bIsWow64))
                //        {
                //                // 如果bIsWow64等于TRUE的话，说明运行在WOW64下，为32位
                //                uResult = bIsWow64 ? 32:64 ;
                //                __leave ;
                //        }
                //}

                // 或GetProcessImageFileName
                // QueryFullProcessImageName只支持vista以及之后的系统
                if(0 == GetProcessImageFileName(hProcess, szDriveName, MAX_PATH))
                {
                        OutputErrorInformation(TEXT("GetProcessBit"), TEXT("GetModuleFileNameEx")) ;
                        __leave ;
                }

                // 这里你要转换路径
                if(VolumeDeviceToNtPath(szDriveName, szFileName))
                {
                        uResult = GetPEFileBit(szFileName) ;
                }
        }

        __finally
        {
                if (INVALID_HANDLE_VALUE != hProcess)
                {
                        CloseHandle(hProcess) ;
                        hProcess = INVALID_HANDLE_VALUE ;
                }
        }
        return uResult ;
}

/*******************************************************************************
*
*   函 数 名 : GetMyselfBit
*  功能描述 : 取得当前进程的位数
*  参数列表 : 无
*   说      明 : 无
*  返回结果 :  返回当前进程位数
*
*******************************************************************************/
ULONG   GetMyselfBit(VOID)
{
        return sizeof(PVOID) * 8;
}

/*******************************************************************************
*
*   函 数 名 : GetPEFileBit
*  功能描述 : 取得可执行文件的位数
*  参数列表 : pPEFilePath  --     pe文件路径
*   说      明 : 读pe文件的头进内存，再调用GetBitByPEHeader去判断
*  返回结果 :  返回当前进程位数
*
*******************************************************************************/
ULONG   GetPEFileBit(__in_z CONST PTCHAR pPEFilePath)
{
        return 0 ;
}

/*******************************************************************************
*
*   函 数 名 : GetBitByPEHeader
*  功能描述 :通过内存中的PE头来取得程序位数
*  参数列表 : pPE  --     指向存储pe头的内存起始地址
*                   uSize  --  指明存储pe头内存区域大小
*   说      明 : 读pe文件的头进内存，再调用GetBitByPEHeader去判断
*  返回结果 :  返回当前进程位数
*
*******************************************************************************/
ULONG   GetBitByPEHeader(__in_bcount(uSize) CONST PVOID pPE,
                         __in CONST ULONG uSize)
{
        // 可以通过IMAGE_NT_OPTIONAL_HDR_MAGIC来判断
        return  0 ;
}

/*******************************************************************************
*
*   函 数 名 : VolumeDeviceToNtPath
*  功能描述 :逻辑路径转化成NT路径
*  参数列表 : lpszDevicePath  --    逻辑路径
*                   lpszFilePath  --          NT路径
*   说      明 : 哎，微软的示例代码.....我表示有点无语，就先这样吧！
*  返回结果 :  执行成功，返回TRUE
*
*******************************************************************************/
BOOL VolumeDeviceToNtPath(__in_z LPCTSTR lpszDevicePath,
                                                __inout_z LPWSTR lpszFilePath)
{
        lpszFilePath[0] = 0x00; 
        // Translate path with device name to drive letters.
        TCHAR szTemp[MAX_PATH] = {0};

        if (GetLogicalDriveStrings(MAX_PATH - 1, szTemp)) 
        {
                TCHAR szName[MAX_PATH] = {0};
                TCHAR szDrive[3] = TEXT(" :");
                BOOL bFound = FALSE;
                TCHAR* p = szTemp;
                do 
                {
                        // Copy the drive letter to the template string
                        *szDrive = *p;

                        // Look up each device name
                        if (QueryDosDevice(szDrive, szName, MAX_PATH))
                        {
                                size_t uNameLen = _tcslen(szName);

                                if (uNameLen < MAX_PATH) 
                                {
                                        bFound = _tcsnicmp(lpszDevicePath, szName, uNameLen) == 0
                                                && *(lpszDevicePath + uNameLen) == _T('\\');

                                        if (bFound) 
                                        {
                                                // Reconstruct pszFilename using szTempFile
                                                // Replace device path with DOS path
                                                TCHAR szTempFile[MAX_PATH];
                                                StringCchPrintf(szTempFile,
                                                        MAX_PATH,
                                                        TEXT("%s%s"),
                                                        szDrive,
                                                        lpszDevicePath+uNameLen);
                                                StringCchCopyN(lpszFilePath, MAX_PATH+1, szTempFile, _tcslen(szTempFile));
                                                return TRUE;
                                        }
                                }
                        }

                        // Go to the next NULL character.
                        while (*p++);
                } while (!bFound && *p); // end of string
        }
        return FALSE;
}
