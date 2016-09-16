#include "stdafx.h"
#include "DoIt.h"
#include <Windows.h>
#include <DbgHelp.h>
#include <iostream>

LONG CALLBACK VectoredHandler(
    _In_ PEXCEPTION_POINTERS ExceptionInfo
)
{
    HANDLE hFile = CreateFileA("MiniDump.dmp", GENERIC_READ | GENERIC_WRITE,
        0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if ((hFile != NULL) && (hFile != INVALID_HANDLE_VALUE))
    {
        // Create the minidump 
        std::cout << "Opened file." << std::endl; 
        MINIDUMP_EXCEPTION_INFORMATION mdei;

        mdei.ThreadId = GetCurrentThreadId();
        mdei.ExceptionPointers = ExceptionInfo;
        mdei.ClientPointers = FALSE;

        MINIDUMP_TYPE mdt = MiniDumpNormal;

        std::cout << "write mini dump" << std::endl; 

        BOOL rv = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
            hFile, mdt, (ExceptionInfo != 0) ? &mdei : 0, 0, 0);

        std::cout << "Successfully wrote mini dump: " << rv << std::endl; 
        CloseHandle(hFile);
        exit(0);
    }
    std::cout << "."; 
    return EXCEPTION_CONTINUE_EXECUTION;
}

void DoIt::RegisterAndThenBlowUp()
{
    // register the handler. 
    ::AddVectoredExceptionHandler(1, VectoredHandler); 

    // blow up. null reference. 
    int* v = 0; 
    std::cout << *v << std::endl; 
}
