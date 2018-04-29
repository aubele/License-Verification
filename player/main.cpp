/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "player.h"
#include "LicenseVerification.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDir>

#include <exception>
#include <windows.h>

using namespace std;


// Explained in p. 2 below
void NTAPI tls_callback(PVOID DllHandle, DWORD dwReason, PVOID)
{
	__asm
	{
		mov eax, dword ptr fs:[0x18]
		mov eax, dword ptr[eax + 0x30]
		cmp byte ptr[eax + 2], 0

		jne Exit
		jmp Run
	}
	Exit:
		TerminateProcess(GetCurrentProcess(), 1);
	Run:
		;
}

#ifdef _WIN64
#pragma comment (linker, "/INCLUDE:_tls_used")  // See p. 1 below
#pragma comment (linker, "/INCLUDE:tls_callback_func")  // See p. 3 below
#else
#pragma comment (linker, "/INCLUDE:__tls_used")  // See p. 1 below
#pragma comment (linker, "/INCLUDE:_tls_callback_func")  // See p. 3 below
#endif

// Explained in p. 3 below
#ifdef _WIN64
#pragma const_seg(".CRT$XLF")
EXTERN_C const
#else
#pragma data_seg(".CRT$XLF")
EXTERN_C
#endif
PIMAGE_TLS_CALLBACK tls_callback_func = tls_callback;
#ifdef _WIN64
#pragma const_seg()
#else
#pragma data_seg()
#endif //_WIN64


int main(int argc, char *argv[])
{
	if (checkNtQueryInformationProcess())
	{
		return 1;
	}

	QApplication app(argc, argv);

	QCoreApplication::setApplicationName("Player Example");
	QCoreApplication::setOrganizationName("QtProject");
	QCoreApplication::setApplicationVersion(QT_VERSION_STR);

	__asm
	{
		mov eax, dword ptr fs : [0x18]
		mov eax, dword ptr[eax + 0x30]
		cmp byte ptr[eax + 2], 0

		je RunProgram
		jmp ExitProgram
	}
ExitProgram:
	return 1;

RunProgram:
	QCommandLineParser parser;
	parser.setApplicationDescription("Qt MultiMedia Player Example");
	parser.addHelpOption();
	parser.addVersionOption();
	parser.addPositionalArgument("url", "The URL to open.");
	parser.process(app);

	SYSTEMTIME systime1;
	GetSystemTime(&systime1);
	FILETIME time1;
	SystemTimeToFileTime(&systime1, &time1);

	// Still need to call processLicense() after init
	LicenseVerification* verification = new LicenseVerification;
	try
	{
		if (checkDebuggerWithTrapFlag())
		{
			return 1;
		}

		SYSTEMTIME systime2;
		GetSystemTime(&systime2);
		FILETIME time2;
		SystemTimeToFileTime(&systime2, &time2);

		// In milliseconds
		double timeExecution = (time2.dwLowDateTime - time1.dwLowDateTime) / 10000.0;
		if (timeExecution > 5)
		{
			return 1;
		}

		if (int2DCheck())
		{
			return 1;
		}

		if (checkNtQueryInformationProcess())
		{
			return 1;
		}

		if (!verification->processLicense())
		{
			return 1;
		}

		SYSTEMTIME systime3;
		GetSystemTime(&systime3);
		FILETIME time3;
		SystemTimeToFileTime(&systime3, &time3);

		timeExecution = (time3.dwLowDateTime - time2.dwLowDateTime) / 10000.0;
		if (timeExecution > 75)
		{
			return 1;
		}
	}
	catch (LicenseException& licExcp)
	{
		// Show the description in a critical messagebox

		// Different part of the exception description
		string message1 = decode(licExcp.what());
		//"Bitte lesen Sie das beilegende Dokument unter 'Help' zur Lizenzierung durch. Dieses beinhaltet auch moegliche Fehlerloesungen und Informationen, die nuetzlich sein koennten."
		string message2 = decode("\xd4\xf8\xfd\xea\xf6\xb6\xf7\xba\xfc\xf5\xf8\xb1\xd8\xf3\xe8\xdf\xf2\xf0\xfa\xbe\xf1\xf3\xf2\xb3\xea\xf7\xf3\xff\xef\xff\xad\xbb\xf9\xfa\xfc\xf3\xf6\xf8\xef\xff\xfa\xfe\xe2\xf4\xf9\xba\xaa\xb7\xf3\xfd\xf9\xb9\xb3\xec\xee\xad\xaf\xdc\xff\xeb\xee\xf4\xf7\x96\xf3\xe3\xfc\xf0\xf4\xb6\xff\xaa\xfd\xf3\xfe\xbf\xab\xde\xe4\x9a\xe5\xf4\xfa\xbe\xf1\xf3\xf2\xb1\xe7\xf1\xfa\xe5\xee\xee\xad\x9e\xe3\xf2\xe1\xbe\xfe\xf9\xfe\xb8\xe3\xf9\xf5\xf9\xee\xba\xcb\x9a\xfe\xfd\xec\xec\xff\xf9\xfe\xac\xfa\xfe\xf1\xf4\xe5\xba\xf8\x91\xf2\xb1\xc0\xf0\xf5\xf9\xe9\xb2\xee\xe4\xff\xfe\xe5\xff\xe3\xd3\xb6\xf5\xe0\xfb\xb3\xf8\xee\xba\xfb\xea\xfa\xf8\xe8\xf2\xad\x8c\xf3\xf8\xe7\xbe\xf8\xf9\xfe\xb1\xe1\xe4\xf3\xff\xa5");
		//"Wenn Sie keine passende Loesung fuer das Problem finden koennen, melden Sie sich bitte beim Support!"
		string message3 = decode("\xc1\xf4\xe7\xf0\xb3\xc5\xf2\xba\xaf\xfb\xf3\xf8\xe5\xff\xad\x8f\xf7\xe2\xfa\xfb\xfd\xf2\xfe\xff\xc3\xff\xf3\xe2\xfe\xf4\xea\xdf\xf0\xe4\xec\xec\xb3\xf2\xfa\xac\xaf\xc0\xe4\xfe\xe9\xf6\xe8\x92\xb6\xf7\xe0\xf0\xf7\xf3\xf5\xff\xe4\xff\xf3\xff\xe5\xff\xe3\xd3\xb6\xfc\xec\xf2\xf7\xf3\xf5\xff\xdc\xf9\xf3\xb1\xf8\xf3\xee\x97\xb6\xf3\xe0\xea\xe7\xf3\xbb\xbd\xea\xf9\xfb\xb1\xd8\xef\xfd\x8f\xf9\xe3\xfd\xbf");
		string all = message1 + "\n\n" + message2 + "\n\n" + message3;

		//"Lizenz ungueltig"
		verification->showMessageBox(decode("\xda\xf8\xf3\xfb\xfd\xec\xbb\xaa\xe1\xf7\xe3\xf4\xe7\xee\xe4\x98").c_str(), all.c_str());
		return 1;
	}

	if (checkNtQueryInformationProcess())
	{
		return 1;
	}

	// Before you overgive the verification object be sure to call processLicense() once
	Player player(verification);

	if (!parser.positionalArguments().isEmpty() && player.isPlayerAvailable()) {
		QList<QUrl> urls;
		foreach(const QString &a, parser.positionalArguments())
			urls.append(QUrl::fromUserInput(a, QDir::currentPath(), QUrl::AssumeLocalFile));
		player.addToPlaylist(urls);
	}

	if (!player.getPassedTest())
	{
		return 1;
	}

#if defined(Q_WS_SIMULATOR)
	player.setAttribute(Qt::WA_LockLandscapeOrientation);
	player.showMaximized();
#else
	player.show();
#endif
	return app.exec();
}