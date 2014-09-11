#ifdef _EXE
#pragma comment( linker, "/subsystem:console /entry:mainCRTStartup" )

/*
#pragma comment( linker, "/subsystem:windows /entry:WinMainCRTStartup" )
#pragma comment( linker, "/subsystem:windows /entry:mainCRTStartup" )
#pragma comment( linker, "/subsystem:console /entry:mainCRTStartup" )
#pragma comment( linker, "/subsystem:console /entry:WinMainCRTStartup" )
*/

#include <stdio.h>
#include <Windows.h>

int main(void)
{
        printf ("Hello, InjectCode\r\n") ;
        return 0 ;
}

#endif