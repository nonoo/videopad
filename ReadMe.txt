    VideoPad
    Copyright 2006 Norbert "Nonoo" Varga <nonoo@nonoo.hu>
                   Tamás "BlooD2oo1" Mélykuti <melykuti.tamas@freemail.hu>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA



WHAT IS VIDEOPAD?
-----------------
VideoPad is a simple, client-server based video conferencing system. Users
connect and send audio/video data to the central server which distributes
received data back to them. This way a client doesn't have to have large upload
bandwidth to communicate with other people.

VideoPad is like IRC: the server has channels, users can join them and talk to
other people on the same channel.



DEVELOPER NOTES
---------------
You'll need a few things to set up before you can compile VideoPad source.
First, you'll need DirectX SDK. You can download it from here:
http://msdn.microsoft.com/directx/directxdownloads/default.aspx

You'll need DirectShow SDK too, because it's not in the DirectX SDK anymore.
Now it's a part of the Platform SDK. You can download it from here:
http://www.microsoft.com/downloads/details.aspx?FamilyID=eba0128f-a770-45f1-86f3-7ab010b398a3&DisplayLang=en

After installing the Platform SDK, open a command line running cmd.exe, then type in
these commands:

cd "c:\Program Files\Microsoft Platform SDK"
setenv.cmd /XP32 /RETAIL
cd Samples\Multimedia\DirectShow\BaseClasses
nmake.exe
cd "c:\Program Files\Microsoft Platform SDK"
setenv.cmd /XP32 /DEBUG
cd Samples\Multimedia\DirectShow\BaseClasses
nmake.exe

Then you'll have baseclasses compiled into a .lib in XP32_RETAIL and XP32_DEBUG folders.

You'll need libogg, download it from http://www.xiph.org/
- Place libogg source in VideoPad/libogg subfolder, then open libogg/ogg.dsw in
  Visual Studio.
- Build it, then switch to the Release version in the project properties->
  configuration manager.
- Build it again. Now you have debug and release versions of
  libogg, both static and dynamic.

You can download libtheora from http://www.xiph.org
- Place libtheora source in VideoPad/libtheora subfolder, then open
  libtheora/win32/vs2003/libtheora/libtheora.vcproj
- Make sure that every .c and .h file in libtheora/lib directory is in the Visual Studio
  project's source file list, otherwise you'll get a bunch of unresolved externals.
- In the project properties, switch Configuration Type from Dynamic DLL to Static Library.
- Now build the project, then switch to Release version in the Configuration Manager.
  Change the Configuration Type to Static Library again, and build the project.
  Now you have static debug and release versions of libtheora.

You'll need libspeex, download it from http://www.speex.org/
VideoPad is compiled with libspeex 1.2beta, so make sure you download the latest
version, no matter if it is a beta release.
- Place libspeex source in VideoPad/libspeex subfolder, then open
  libspeex/win32/libspeex/libspeex.dsw in Visual Studio.
- Delete libspeex_dynamic from the project browser.
- Make sure that every .c and .h file in libspeex/src directory is in the Visual Studio
  project's source file list, in both libspeex and libspeex_dynamic, otherwise you'll get a
  bunch of unresolved externals.
- Build it, then switch to the Release version in the project properties->
  configuration manager. Build it again. Now you have debug and release versions of
  libspeex, both static and dynamic.



TROUBLESHOOTING
---------------
If you get "Debugger detected - please close it down and restart" message when trying
to run VideoPad from a debugger, you should check if Nero CD burner is installed, because
one of it's DirectShow filters causing this error. In this case, try to delete/rename
"C:\Program Files\Common Files\Ahead\DSFilter\NeVideo.ax"
