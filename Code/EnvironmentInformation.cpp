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
#include "EnvironmentInformation.h"

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
        // 可以调用IsWow64Process
        // 或GetProcessImageFileName
        return 0 ;
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
