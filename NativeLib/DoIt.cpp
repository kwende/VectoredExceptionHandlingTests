#include "stdafx.h"
#include "DoIt.h"
#include <Windows.h>
#include <DbgHelp.h>
#include <iostream>

LONG CALLBACK VectoredHandler(
    _In_ PEXCEPTION_POINTERS ExceptionInfo
)
{
    std::cout << "VectoredHandler caught exception." << std::endl; 
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
