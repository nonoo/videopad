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

#include "ChildView.h"

class CMainFrame : public CFrameWnd
{
protected:
	CToolBar		m_wndToolBar;
	CChildView		m_wndView;
	
public:
	CMainFrame();
	virtual ~CMainFrame();
	
public:
	virtual BOOL		PreCreateWindow( CREATESTRUCT& cs );
	virtual BOOL		OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo );
	
#ifdef _DEBUG
	virtual void		AssertValid() const;
	virtual void		Dump( CDumpContext& dc ) const;
#endif

protected: 
	DECLARE_DYNAMIC( CMainFrame )
	
protected:
	afx_msg int			OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void		OnSetFocus( CWnd *pOldWnd );
	afx_msg void		OnUpdateToolBar( CCmdUI* pCmd );
	afx_msg void		OnClose();

	DECLARE_MESSAGE_MAP()
};


