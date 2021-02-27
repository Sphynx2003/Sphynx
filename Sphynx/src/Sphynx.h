#pragma once

//--------------OS identificaton-------------------------
//Mac won't be supported.
/* Windows x64/x86 */
#ifdef _WIN32
	/* Windows x64  */
	#ifdef _WIN64
		#define Platform_Win
		#define WIN32_LEAN_AND_MEAN
		#include <Windows.h>
		#include <d3d.h>
		#include <d2d1.h>
	#else
		/* Windows x86 */
		#error "x86 Builds are not supported!"
	#endif
#elif defined(__linux__)
	#define Platform_Linux
#endif

//For Application Use only
#include "Sphynx/Application.h"
#include "Sphynx/Logger.h"
#include "Sphynx/Events/Event.h"
#include "Sphynx/Core/Imgui.h"
//--------------------EntryPoint Header-------------------------
#include "Sphynx/EntryPoint.h"