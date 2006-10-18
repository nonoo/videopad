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

#include "stdafx.h"
#include "VideoPad.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_CONNECT, OnUpdateToolBar)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_CHANNEL, OnUpdateToolBar)
END_MESSAGE_MAP()

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( CFrameWnd::OnCreate( lpCreateStruct ) == -1 )
	{
		return -1;
	}
	
	if ( !m_wndView.Create( NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect( 0, 0, 400, 500 ), this, AFX_IDW_PANE_FIRST, NULL ) )
	//if ( !m_wndView.Create( NULL, NULL, WS_VISIBLE | WS_CHILD, CRect( 0, 0, 400, 500 ), this, IDD_FORMVIEW-1, NULL ) )
	{
		TRACE0("Failed to create view window!\n");
		return -1;
	}
	

	if (	!m_wndToolBar.CreateEx( this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC ) ||
			!m_wndToolBar.LoadToolBar( IDR_MAINFRAME ) )
	{
		TRACE0("Failed to create toolbar!\n");
		return -1;
	}
	
	return 0;
}

BOOL CMainFrame::PreCreateWindow( CREATESTRUCT& cs )
{
	if( !CFrameWnd::PreCreateWindow( cs ) )
	{
		return FALSE;
	}
	
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass( 0 );
	
	return TRUE;
}

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo )
{
	if ( m_wndView.OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ) )
	{
		return TRUE;
	}

	return CFrameWnd::OnCmdMsg( nID, nCode, pExtra, pHandlerInfo );
}

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump( CDumpContext& dc ) const
{
	CFrameWnd::Dump( dc );
}
#endif //_DEBUG

void CMainFrame::OnUpdateToolBar( CCmdUI* pCmd )
{
	switch( pCmd->m_nID )
	{
	case ID_TOOLBAR_CONNECT:
		if ( theApp.GetConnected() == true )
		{
			pCmd->SetCheck( BST_CHECKED );
		}
		else
		{
			pCmd->SetCheck( BST_UNCHECKED );
		}
		break;
		
	case ID_TOOLBAR_CHANNEL:
		pCmd->Enable ( theApp.GetConnected() );
		break;
		
	default:
		break;
		
	}
	
}