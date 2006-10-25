//  This file is part of VideoPad.
//
//  VideoPad is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  VideoPad is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with VideoPad; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#pragma once

#pragma warning( disable : 4127 ) // endless loops
#pragma warning( disable : 4201 ) // nameless struct/union in mmsystem.h
#pragma warning( disable : 4995 ) // include/xlocnum, deprecated
#pragma warning( disable : 4706 ) // wspapi.h, assigment within conditional expression

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0501		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0501		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0501 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE 0x0501	// Change this to the appropriate value to target IE 5.0 or later.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxtempl.h>		// for CArray

// winsock
//
#include <afxsock.h>
#include <ws2tcpip.h>
#include <wspiapi.h>

// directshow
//
#include <qedit.h>
#include <dshow.h>
#include <objbase.h>
#include <streams.h>
#include <mtype.h>

#include <map>
using namespace std;

// user defined messages
//
#define WU_CONTROLSOCKET_EVENT WM_APP+1
#define WU_DATASOCKET_EVENT WM_APP+2
#define WU_THREAD_STOP WM_APP+3
#define WU_THREAD_PROCESS WM_APP+4

// max length of one line sent to the control port
#define MAXMESSAGELENGTH 1024

#include "CommonDefines.h"

#include "resource.h"
#include "Dialogs/ChannelDialog.h"
#include "Dialogs/ConnectingDialog.h"
#include "Dialogs/VideoDialog.h"
#include "Dialogs/ChatDialog.h"

#include "Utils/Server/Server.h"
#include "Utils/Channel.h"
#include "Utils/SettingsFile.h"
#include "Utils/Client.h"

#include "Utils/Sockets/Connection.h"
#include "Utils/Sockets/TCPConnection.h"
#include "Utils/Sockets/UDPConnection.h"

#include "DirectShow/DirectShow.h"
#include "DirectShow/DirectShowGraph.h"
#include "DirectShow/AudioCaptureDevice.h"
#include "DirectShow/AudioGraph.h"
#include "DirectShow/CaptureDevice.h"
#include "DirectShow/GrabberCallBack.h"
#include "DirectShow/VideoCaptureDevice.h"
#include "DirectShow/VideoGraph.h"

#include "ogg/ogg.h"
#include "theora/theora.h"
#include "speex/speex.h"
#include "speex/speex_preprocess.h"
#include "speex/speex_header.h"

#include "Ogg/OggStream.h"

#include "Encoder/TheoraEncoder.h"
#include "Encoder/Theora.h"
#include "Encoder/TheoraThread.h"
#include "Encoder/SpeexEncoder.h"
#include "Encoder/Speex.h"
#include "Encoder/SpeexThread.h"
