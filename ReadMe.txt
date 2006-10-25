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
You can download DirectX SDK from:
http://msdn.microsoft.com/directx/directxdownloads/default.aspx
You'll need DirectShow SDK too, it's a part of the Platform SDK:
http://www.microsoft.com/downloads/details.aspx?FamilyID=eba0128f-a770-45f1-86f3-7ab010b398a3&DisplayLang=en

After installing Platform SDK, open a command line running cmd.exe, then follow
these instructions:

cd "c:\Program Files\Microsoft Platform SDK"
setenv.cmd /XP32 /RETAIL
cd Samples\Multimedia\DirectShow\BaseClasses
nmake.exe
cd "c:\Program Files\Microsoft Platform SDK"
setenv.cmd /XP32 /DEBUG
cd Samples\Multimedia\DirectShow\BaseClasses
nmake.exe

Then you'll have baseclasses compiled into a .lib in XP32_RETAIL and XP32_DEBUG folders.

If you get "Debugger detected - please close it down and restart" message when trying
to run VideoPad from a debugger, you should check if Nero CD burner is installed, because
one of it's DirectShow filters causing this error. In this case, try to delete/rename
"C:\Program Files\Common Files\Ahead\DSFilter\NeVideo.ax"
