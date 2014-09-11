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
#include "ProcessInformation.h"

/*******************************************************************************
*
*   函 数 名 : GetProcessListByProcessName
*  功能描述 : 通过进程名取得进程列表
*  参数列表 : pProcessName     --      指定查找的进程名
*                   pArrayPID            --       用来保存与指定名相同的进程PID
*                   uSize                    --       pArrayPID数组大小，非byte,就是可以存多少pid
*   说      明 :  如果pArrayPID没有指定的话，函数依然可以工作
*  返回结果 :  如果与指定进程名相同的进程数
*
*******************************************************************************/
ULONG GetProcessListByProcessName(
                            __in_z PTCHAR pProcessName,
                            __in_bcount_z_opt(uSize)PDWORD pArrayPID,
                            __in_opt ULONG uSize)
{
        // 遍历进程，再进程名比较
        // 相同的话计数器加一，再判断pArrayPID是否为空，如果不为空，把pid写进去
        // 注意pArrayPID too small的问题
        return 0 ;
}