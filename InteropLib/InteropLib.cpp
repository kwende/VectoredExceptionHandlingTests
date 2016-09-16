// This is the main DLL file.

#include "stdafx.h"

#include "InteropLib.h"
#include "DoIt.h"

void InteropLib::DoItManaged::RegisterAndThenBlowUp()
{
    ::DoIt::RegisterAndThenBlowUp(); 
}

