//*************************************************************************
//  
//  Copyright (c) all 2014  All rights reserved
//  D a t e  : 2014.9.11
//  作  者 : 
//  版  本 : 0.1
//  功  能 : 错误信息相关函数
//  说  明 : 
//  备  注 :
//
//  修改记录:
//  日   期       版本    修改人              修改内容
// 2014/9/11 0.1      EvilKnight        创建
//  YYYY/MM/DD    X.Y     <作者或修改者名>    <修改内容>
//
//*************************************************************************

#include "ErrorInformation.h"
#include <tchar.h>
#include <assert.h>

/*******************************************************************************
*
*   函 数 名 :     OutputErrorInformation
*  功能描述 :     输出错误信息
*  参数列表 :     pFunctionName          --             目标进程ID
*                      pTipsInformation      --             指定要注入的dll
*   说      明 :     有些要判断函数是否成功执行的，如果出错的话，调用这个函数输出错
*                      误信息
*  返回结果 :     如果成功，返回TRUE，失败返回FALSE
*
*******************************************************************************/
VOID OutputErrorInformation(__in_z CONST PTCHAR pFunctionName,
                                                __in_z CONST PTCHAR pTipsInformation)
{
        TCHAR szBuffer[MAX_PATH] = {0} ;

        assert(NULL != pFunctionName) ;
        assert(NULL != pTipsInformation) ;

        _stprintf_s(szBuffer, sizeof(szBuffer), TEXT("%s::%s ErrorCode:%p\r\n"), 
                                pFunctionName, pTipsInformation, GetLastError()) ;
        OutputDebugString(szBuffer) ;
}