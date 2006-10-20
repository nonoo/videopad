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

// config file settings manager class
//
class CSettingsFile
{
public:
    CSettingsFile();

    void	LoadConfig();
    void	SaveConfig();

    CString	Get( CString Section, CString Key, CString DefaultValue );
	CString	Get( CString Section, int Key, CString DefaultValue );
    int		GetInt( CString Section, CString Key, const int& DefaultValue );
    void	Set( CString Section, CString Key, CString Value );
	void	Set( CString Section, int Key, CString Value );

private:
    // removes whitespaces
    //
    CString	TrimLeft( CString szString );
    CString	TrimRight( CString szString );

    void	SearchForConfigFile();




    CString	m_szConfigFile;

    // ini structure in memory
    // m_Settings[section][key] == value
    //
    typedef map< CString, CString >	t_mKeys;
    map< CString, t_mKeys >			m_Settings;
};
