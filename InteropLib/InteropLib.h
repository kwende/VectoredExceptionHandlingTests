// InteropLib.h

#pragma once

using namespace System;

namespace InteropLib {

	public ref class DoItManaged
	{
    public:
        static void RegisterAndThenBlowUp(); 
	};
}
