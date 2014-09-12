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

#ifndef ERRORINFORMATION__H
#define ERRORINFORMATION__H

#include <Windows.h>


// 输出错误信息
VOID OutputErrorInformation(__in_z CONST PTCHAR pFunctionName,
                                                 __in_z CONST PTCHAR pTipsInformation) ;

#endif