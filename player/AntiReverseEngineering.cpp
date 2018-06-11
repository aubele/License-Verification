#include "AntiReverseEngineering.h"

#include <excpt.h>
#include <windows.h>


///////////////////////////////////////////////////////////////////////////////////////////
// ### ANTI REVERSE ENGINEERING ### 

string decode(const string& input)
{
	// Use std::string, cause bit operations are easier then
	// choose a power of two length => then compiler can replace "modulo x" by much faster "and (x-1)"   
	const size_t passwordLength = 16;
	// at least as long as passwordLength, can be longer, too ...   
	static const char password[passwordLength] = "invalid pointer";
	// out = in XOR NOT(password)   
	string result = input;
	for (size_t i = 0; i < input.length(); i++)
		result[i] ^= ~password[i % passwordLength];

	return result;
}

bool checkDebuggerWithTrapFlag()
{
	bool isDebugged = true;
	__try
	{
		// Get the eflagas register, set the Trap Flag and load the value into EFLAGS register in assembler
		__asm
		{
			pushfd
			or dword ptr[esp], 0x100
			popfd
			nop
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		// If an exception has been raised debugger is not present
		isDebugged = false;
	}
	return isDebugged;
}

bool int2DCheck()
{
	__try
	{
		// Define the instruction in assembler
		__asm
		{
			int 0x2d
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		// Debugger is not present
		return false;
	}
	// Debugger is present
	return true;
}

/**
* Needed typdef for the NtQueryInformationProcess
*/
typedef NTSTATUS(NTAPI *pfnNtQueryInformationProcess)(
	_In_      HANDLE           ProcessHandle,
	_In_      UINT             ProcessInformationClass,
	_Out_     PVOID            ProcessInformation,
	_In_      ULONG            ProcessInformationLength,
	_Out_opt_ PULONG           ReturnLength
	);

bool checkNtQueryInformationProcess()
{
	// Port for the NtQueryInformationProcess function
	const UINT ProcessDebugPort = 7;
	pfnNtQueryInformationProcess NtQueryInformationProcess = NULL;
	NTSTATUS status;
	DWORD isDebuggerPresent = 0;

	// Get the ntdll.dll, which hold the NtQueryInformationProcess function
	// "ntdll.dll"
	string dllName = decode("\xf8\xe5\xed\xf2\xff\xb8\xff\xb3\xe3");
	// string casting made by microsoft :)
	std::wstring stemp = std::wstring(dllName.begin(), dllName.end());
	LPCWSTR ntdll = stemp.c_str();
	HMODULE hNtDll = LoadLibrary(ntdll);

	if (NULL != hNtDll)
	{
		// Load the NtQueryInformationProcess function via ntdll.dll
		// "NtQueryInformationProcess"
		string ntquery = decode("\xd8\xe5\xd8\xeb\xf6\xe4\xe2\x96\xe1\xf6\xf9\xe3\xe6\xfb\xf9\x96\xf9\xff\xd9\xec\xfc\xf5\xfe\xac\xfc");
		NtQueryInformationProcess = (pfnNtQueryInformationProcess)GetProcAddress(hNtDll, ntquery.c_str());
		if (NULL != NtQueryInformationProcess)
		{
			// Call the debugger function and check if a debugger is present
			status = NtQueryInformationProcess(
				GetCurrentProcess(),
				ProcessDebugPort,
				&isDebuggerPresent,
				sizeof(DWORD),
				NULL);
			if (status == 0x00000000 && isDebuggerPresent != 0)
			{
				// Debugger is present
				return true;
			}
		}
	}
	// Debugger is not present
	return false;
}