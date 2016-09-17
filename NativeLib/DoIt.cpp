#include "stdafx.h"
#include "DoIt.h"
#include <Windows.h>
#include <DbgHelp.h>
#include <iostream>

LONG WINAPI VectoredHandler(
    PEXCEPTION_POINTERS ExceptionInfo
)
{

    //HANDLE hFile = CreateFileA("MiniDump.dmp", GENERIC_READ | GENERIC_WRITE,
    //    0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    //if ((hFile != NULL) && (hFile != INVALID_HANDLE_VALUE))
    //{
    //    // Create the minidump 
    //    std::cout << "Opened file." << std::endl; 
    //    MINIDUMP_EXCEPTION_INFORMATION mdei;

    //    mdei.ThreadId = GetCurrentThreadId();
    //    mdei.ExceptionPointers = ExceptionInfo;
    //    mdei.ClientPointers = FALSE;

    //    MINIDUMP_TYPE mdt = MiniDumpNormal;

    //    std::cout << "write mini dump" << std::endl; 

    //    BOOL rv = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
    //        hFile, mdt, (ExceptionInfo != 0) ? &mdei : 0, 0, 0);

    //    std::cout << "Successfully wrote mini dump: " << rv << std::endl; 
    //    CloseHandle(hFile);
    //    exit(0);
    //}

    HMODULE hDbgHelp = NULL;
    HANDLE hFile = NULL;
    MINIDUMP_EXCEPTION_INFORMATION mei;
    MINIDUMP_CALLBACK_INFORMATION mci;

    // Load dbghelp.dll
    hDbgHelp = LoadLibraryA("dbghelp.dll");
    if (hDbgHelp == NULL)
    {
        // Error - couldn't load dbghelp.dll
        std::cout << "Couldn't load dbhelp.dll" << std::endl; 
        return EXCEPTION_CONTINUE_EXECUTION;
    }

    // Create the minidump file
    hFile = CreateFileA(
        "crashdump.dmp",
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        //std::cout << "Couldn't create file. Error " << ::GetLastError() << std::endl; 
        // Couldn't create file
        return EXCEPTION_CONTINUE_EXECUTION;
    }

    // Write minidump to the file
    mei.ThreadId = GetCurrentThreadId();
    mei.ExceptionPointers = ExceptionInfo;
    mei.ClientPointers = FALSE;
    mci.CallbackRoutine = NULL;
    mci.CallbackParam = NULL;

    typedef BOOL(WINAPI *LPMINIDUMPWRITEDUMP)(
        HANDLE hProcess,
        DWORD ProcessId,
        HANDLE hFile,
        MINIDUMP_TYPE DumpType,
        CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
        CONST PMINIDUMP_USER_STREAM_INFORMATION UserEncoderParam,
        CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam);

    LPMINIDUMPWRITEDUMP pfnMiniDumpWriteDump =
        (LPMINIDUMPWRITEDUMP)GetProcAddress(hDbgHelp, "MiniDumpWriteDump");
    if (!pfnMiniDumpWriteDump)
    {
        std::cout << "Bad MiniDumpWriteDump function" << std::endl; 
        // Bad MiniDumpWriteDump function
        return EXCEPTION_CONTINUE_EXECUTION;
    }

    HANDLE hProcess = GetCurrentProcess();
    DWORD dwProcessId = GetCurrentProcessId();

    std::cout << "Writing the dump file..." << std::endl; 
    BOOL bWriteDump = pfnMiniDumpWriteDump(
        hProcess,
        dwProcessId,
        hFile,
        MiniDumpNormal,
        &mei,
        NULL,
        &mci);
    std::cout << "...done" << std::endl; 

    if (!bWriteDump)
    {
        std::cout << "Error writing dump." << std::endl; 
        // Error writing dump.
        return EXCEPTION_CONTINUE_EXECUTION;
    }

    // Close file
    CloseHandle(hFile);

    // Unload dbghelp.dll
    FreeLibrary(hDbgHelp);
    std::cout << ".";

    return EXCEPTION_CONTINUE_EXECUTION;
}

void DoIt::RegisterAndThenBlowUp()
{
    // register the handler. 
    ::AddVectoredExceptionHandler(1, VectoredHandler); 
    //::SetUnhandledExceptionFilter(VectoredHandler); 

    // blow up. null reference. 
    //int* v = 0; 
    //std::cout << *v << std::endl;

    RaiseException(123, EXCEPTION_NONCONTINUABLE, 0, NULL);
}
