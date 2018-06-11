#pragma once

#include "string"

using namespace std;


/**
* Decodes the given string with XNOR and the specefied keyword in the function.
* @param input The string, which should get decoded.
* @return The decoded input string.
*/
string decode(const string& input);
/**
* Checks if a debugger is present, with a trapflag from the EFLAGS register.
* @return True if a debugger is present, else false.
*/
bool checkDebuggerWithTrapFlag();
/**
* The Int2DCheck function will check to see if a debugger is attached to the current process. It does
* this by using the Int 2D instruction which will only cause an exception if there is no debugger.
* Also when used in OllyDBG it will skip a byte in the disassembly and will create some havoc.
* @return True if a debugger is present, else false.
*/
bool int2DCheck();
/**
* Checks if a debugger is present with the function CheckRemoteDebuggerPresent(), which gets called
* via NtQueryInformationProcess.
* @return True if a debugger is present, else false.
*/
bool checkNtQueryInformationProcess();