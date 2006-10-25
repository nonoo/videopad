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

// this thread class calls the encoder to periodically (per fps) encode grabbed
// frames from the videograph
//
class CTheoraThread : public CWinThread
{
	DECLARE_DYNCREATE(CTheoraThread);

public:
	virtual BOOL	InitInstance();
	virtual int		ExitInstance();

	// call this to set up thread parameters
	//
	void			Create( CTheora* pTheora, CVideoGraph* pVideoGraph, UINT nEncoderFPS );

private:
	afx_msg void	OnTimer( WPARAM wp, LPARAM lp );
	afx_msg void	OnTerminate( WPARAM wp, LPARAM lp );

	UINT_PTR		m_pTimer;

	CTheora*		m_pTheora;
	CVideoGraph*	m_pVideoGraph;
	UINT			m_nEncoderFPS;

	bool			m_bStopped;

	DECLARE_MESSAGE_MAP()
};
