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
// 2014/9/12 0.1      EvilKnight        实现Inject by ProcessName函数
//  YYYY/MM/DD    X.Y     <作者或修改者名>    <修改内容>
//
//*************************************************************************
#include "InjectCode.h"
#include "EnvironmentInformation.h"
#include "ErrorInformation.h"
#include "ProcessInformation.h"


/*******************************************************************************
*
*   函 数 名 : Inject
*  功能描述 : 将dll注入到指定进程
*  参数列表 : dwPID          --             目标进程ID
*                   pDllPath      --             指定要注入的dll
*   说      明 : 
*  返回结果 :  如果成功，返回TRUE，失败返回FALSE
*
*******************************************************************************/
BOOL Inject(__in CONST DWORD dwPID, 
                        __in_z CONST PTCHAR pDllPath)
{
        // 这里要判断系统、自身程序与目标程序以及dll的位数
        // 判断自身的位数与要注入的dll是否有目标进程一致

        // 打开目标进程

        // 向目标进程申请内存

        // 写入dll路径

        // 创建远程线程
        // 等待线程结束

        // 释放内存

        // 关闭进程句柄
  
        return TRUE ;
}

// 将dll注入到指定进程名的进程中

/*******************************************************************************
*
*   函 数 名 : Inject
*  功能描述 : 将dll注入到指定进程名的进程中
*  参数列表 : pProcessName     --             目标进程名
*                   pDllPath              --             指定要注入的dll
*   说      明 : 注入失败，继续注入下一个
*  返回结果 :  如果全部成功，返回TRUE，有失败返回FALSE
*
*******************************************************************************/
BOOL Inject(__in_z CONST PTCHAR pProcessName,
            __in_z CONST PTCHAR pDllPath)
{
        ULONG uProcessCount(0) ;
        ULONG  uMemoryLength(0) ;
        BOOL bState(FALSE) ;
        PDWORD pArrayPID(NULL) ;
        const ULONG uFree(5) ;         // 多申请一些空间，防止在真正取得进程id的时候
                                                        // 突然多起了一些进程，但是太过于极端的情况我们
                                                        // 就不处理了

        if (NULL == pProcessName)
        {
                OutputDebugString(TEXT("Inject:: pProcessName can't NULL! \r\n")) ;
                return FALSE ;
        }

        __try
        {
                uProcessCount = GetProcessListByProcessName(pProcessName, NULL, 0) ;
                // 如果一个相同的都没有，直接退出吧！
                if (0 == uProcessCount)
                {
                        __leave ;
                }
                
                uMemoryLength = uProcessCount + uFree ;
                pArrayPID = new DWORD[uMemoryLength] ;
                uProcessCount = GetProcessListByProcessName(pProcessName, pArrayPID, uMemoryLength) ;

                // 您的电脑是中毒了吧，要不然程序怎么会启得这么快呢？而且还是同名的程序
                if (uProcessCount > uMemoryLength)
                {
                        __leave ;
                }

                bState = TRUE ;
                // 依次调用Inject向目标进程注入dll
                for (ULONG uIndex(0); uIndex < uProcessCount; ++ uIndex)
                {
                        // 如果有一个注入失败，则返回状态为失败
                        if (! Inject(pArrayPID[uIndex], pDllPath))
                        {
                                bState = FALSE ;
                        }
                }
        }

        __finally
        {
                if (NULL != pArrayPID)
                {
                        delete [] pArrayPID ;
                        pArrayPID = NULL ;
                }
        }
        
        return bState ;
}
