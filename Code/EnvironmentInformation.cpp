//*************************************************************************
//  
//  Copyright (c) all 2014  All rights reserved
//  D a t e  : 2014.9.11
//  ��  �� : 
//  ��  �� : 0.1
//  ��  �� : ���л��������Ϣ����
//  ˵  �� : 
//  ��  ע :
//
//  �޸ļ�¼:
//  ��   ��       �汾    �޸���              �޸�����
// 2014/9/11 0.1      EvilKnight        ����
// 2014/9/11 0.1      EvilKnight        GeOSBit
// 2014/9/12 0.1	ray                   GetProcessBit()
// 2014/9/13 0.1        eoishf          GetPEFileBit()  GetBitByPEHeader()
//  YYYY/MM/DD    X.Y     <���߻��޸�����>    <�޸�����>
//
//*************************************************************************
#include "EnvironmentInformation.h"
#include "ErrorInformation.h"
#include <Psapi.h>
#include <tchar.h>
#include <strsafe.h>
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib, "psapi.lib")

typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(
                                        GetModuleHandle(TEXT("kernel32")),"IsWow64Process");

/*******************************************************************************
*
*   �� �� �� : GetOSBit
*  �������� : ȡ�ò���ϵͳλ��
*  �����б� : ��
*   ˵      �� : 
*  ���ؽ�� :  ����ɹ������ز���ϵͳλ��, ���򷵻�0
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
*   �� �� �� : GetProcessBit
*  �������� : ȡ��ָ������λ��
*  �����б� : dwPID          --             Ŀ�����ID
*   ˵      �� : 
*  ���ؽ�� :  ����ɹ������ؽ���λ��, ���򷵻�0
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

                // ���Ե���IsWow64Process
                //if (NULL != fnIsWow64Process)
                //{
                //        // ����ɹ��ˣ�����
                //        if (fnIsWow64Process(hProcess, &bIsWow64))
                //        {
                //                // ���bIsWow64����TRUE�Ļ���˵��������WOW64�£�Ϊ32λ
                //                uResult = bIsWow64 ? 32:64 ;
                //                __leave ;
                //        }
                //}

                // ��GetProcessImageFileName
                // QueryFullProcessImageNameֻ֧��vista�Լ�֮���ϵͳ
                if(0 == GetProcessImageFileName(hProcess, szDriveName, MAX_PATH))
                {
                        OutputErrorInformation(TEXT("GetProcessBit"), TEXT("GetModuleFileNameEx")) ;
                        __leave ;
                }

                // ������Ҫת��·��
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
*   �� �� �� : GetMyselfBit
*  �������� : ȡ�õ�ǰ���̵�λ��
*  �����б� : ��
*   ˵      �� : ��
*  ���ؽ�� :  ���ص�ǰ����λ��
*
*******************************************************************************/
ULONG   GetMyselfBit(VOID)
{
        return sizeof(PVOID) * 8;
}

/*******************************************************************************
*
*   �� �� �� : GetPEFileBit
*  �������� : ȡ�ÿ�ִ���ļ���λ��
*  �����б� : pPEFilePath  --     pe�ļ�·��
*   ˵      �� : ��pe�ļ���ͷ���ڴ棬�ٵ���GetBitByPEHeaderȥ�ж�
*  ���ؽ�� :  ���ص�ǰ����λ��
*
*******************************************************************************/
ULONG   GetPEFileBit(__in_z CONST PTCHAR pPEFilePath)
{
        FILE *peFile = NULL;
        _wfopen_s(&peFile, pPEFilePath, L"rb");
        if (NULL == peFile)
        {
                fclose(peFile);
                return 0;
        }

        PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)malloc(sizeof(IMAGE_DOS_HEADER));
        if (NULL == pDosHeader)
        {
                OutputErrorInformation(L"malloc", L"malloc failed");
                return 0;
        }

        //��ȡPE�ļ���DOSͷ
        fread(pDosHeader, sizeof(IMAGE_DOS_HEADER), 1, peFile);
        if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
        {
                fclose(peFile);
                return 0;
        }

        //���ļ�ָ��ƫ�Ƶ�PEͷ��
        fseek(peFile, pDosHeader->e_lfanew, SEEK_SET);
        free(pDosHeader);

        IMAGE_NT_HEADERS imageNtHeaders;
        //��ȡPEͷ
        fread(&imageNtHeaders, sizeof(IMAGE_NT_HEADERS), 1, peFile);
        if (imageNtHeaders.Signature != IMAGE_NT_SIGNATURE)
        {
                fclose(peFile);
                return 0;
        }
        fclose(peFile);

        return GetBitByPEHeader(&imageNtHeaders, sizeof(IMAGE_NT_HEADERS));
}

/*******************************************************************************
*
*   �� �� �� : GetBitByPEHeader
*  �������� :ͨ���ڴ��е�PEͷ��ȡ�ó���λ��
*  �����б� : pPE  --     ָ��洢peͷ���ڴ���ʼ��ַ
*                   uSize  --  ָ���洢peͷ�ڴ������С
*   ˵      �� : ��pe�ļ���ͷ���ڴ棬�ٵ���GetBitByPEHeaderȥ�ж�
*  ���ؽ�� :  ���ص�ǰ����λ��
*
*******************************************************************************/
ULONG   GetBitByPEHeader(__in_bcount(uSize) CONST PVOID pPE,
                         __in CONST ULONG uSize)
{
        if (NULL == pPE)
        {
                return 0;
        }

        IMAGE_NT_HEADERS *pNtHeader = (IMAGE_NT_HEADERS *)pPE;
        if (pNtHeader->FileHeader.Machine == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
        {
                return 32;
        }

        if (pNtHeader->FileHeader.Machine == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
        {
                return 64;
        }

        //���ΪIMAGE_ROM_OPTIONAL_HDR_MAGIC������0
        return  0 ;
}

/*******************************************************************************
*
*   �� �� �� : VolumeDeviceToNtPath
*  �������� :�߼�·��ת����NT·��
*  �����б� : lpszDevicePath  --    �߼�·��
*                   lpszFilePath  --          NT·��
*   ˵      �� : ����΢���ʾ������.....�ұ�ʾ�е�������������ɣ�
*  ���ؽ�� :  ִ�гɹ�������TRUE
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
