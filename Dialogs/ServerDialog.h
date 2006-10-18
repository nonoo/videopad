#pragma once


class CServerDialog : public CDialog
{

protected:
	CEdit			m_editMessages;
	CEdit			m_editMessage;
	bool			m_bExist;
	
public:
	CServerDialog();
	~CServerDialog();
	virtual BOOL	Create(UINT nIDTemplate, CWnd* pParentWnd  = NULL );
	void			AddText( CString strText );  // sorvége: "\r\n"

protected:
	virtual void	DoDataExchange( CDataExchange* pDX );
	virtual BOOL	PreTranslateMessage( MSG* pMsg );
	virtual void	OnOK();
				
	DECLARE_MESSAGE_MAP()
	afx_msg void	OnClose();
};
