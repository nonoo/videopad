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
class CSpeexThread : public CWinThread
{
	DECLARE_DYNCREATE(CSpeexThread);

public:
	virtual BOOL	InitInstance();
	virtual int		ExitInstance();

					// call this to set up thread parameters
					//
	void			Create( CSpeex* Speex, CAudioGraph* AudioGraph );

	void			Start();
	void			Stop();

private:
	afx_msg void	Process( WPARAM wp, LPARAM lp );
	afx_msg void	OnTerminate( WPARAM wp, LPARAM lp );
	afx_msg void	Stop( WPARAM wp, LPARAM lp );



					// indicates that the thread should stop itself
	bool			m_bEndThread;
					// when it's true, the thread hangs
	bool			m_bStopThread;

	CSpeex*			m_pSpeex;
	CAudioGraph*	m_pAudioGraph;

	DECLARE_MESSAGE_MAP()
};
