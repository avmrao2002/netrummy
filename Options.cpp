// Options.cpp : implementation file
//

#include "stdafx.h"
#include "NetChess.h"
#include "MyColorDialog.h"
#include "Options.h"
#include "NetChessDoc.h"
#include "NetChessView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptions dialog


COptions::COptions(CWnd* pParent /*=NULL*/)
	: CDialog(COptions::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptions)
	//}}AFX_DATA_INIT
	COLORREF w(RGB(20,140,17)),b(RGB(128,128,128));
	m_crefWhiteColor =  w;
	m_crefBlackColor =  b;
	m_pieceType = 1;
	m_whiteClrDlg = false;
	m_blackClrDlg = false;
}


void COptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptions)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptions, CDialog)
	//{{AFX_MSG_MAP(COptions)
	ON_BN_CLICKED(IDC_BUTTON_WHITE_COLOR, OnButtonWhiteColor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptions message handlers

 
void COptions::OnButtonWhiteColor() 
{
	// TODO: Add your control notification handler code here
	CMyColorDialog cdlg;
	if(cdlg.DoModal() == IDOK)
	{
		m_crefWhiteColor = cdlg.GetColor();
		CStatic *pwhite = (CStatic*)GetDlgItem(IDC_STATIC_WHITE);
		CRect rect;
		CClientDC dc((CWnd*)pwhite);
		pwhite->GetClientRect(rect);		 
		CBrush brush(m_crefWhiteColor);
		CBrush *pOldBrush = (CBrush*)dc.SelectObject(brush); 
		dc.FillRect(&rect,&brush);
		UpdateData(FALSE);
		m_whiteClrDlg = true;
		m_blackClrDlg = false;
		//dc.SelectObject(*pOldBrush);
	}	
	((CNetChessView*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetActiveView())->PostMessage(ID_MY_MESSAGE_COLORDATA,0,0);
}

BOOL COptions::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CStatic *pwhite = (CStatic*)GetDlgItem(IDC_STATIC_WHITE);

	CRect rect;
	CClientDC dc((CWnd*)pwhite);
	pwhite->GetClientRect(rect);		 
	CBrush brush(m_crefWhiteColor);
	CBrush *pOldBrush = (CBrush*)dc.SelectObject(brush); 
	dc.FillRect(&rect,&brush);	

	/*CStatic *pblack = (CStatic*)GetDlgItem(IDC_STATIC_BLACK);	 
	CClientDC bdc((CWnd*)pblack);
	pblack->GetClientRect(rect);		 
	CBrush bbrush(m_crefBlackColor);
	CBrush *pOldBBrush = (CBrush*)bdc.SelectObject(bbrush); 
	bdc.FillRect(&rect,&bbrush);*/
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

 
void COptions::OnOK() 
{
	m_whiteClrDlg = false;
	m_blackClrDlg = false;	
	CDialog::OnOK();
}
void COptions ::SetColor(COLORREF cr)
{	
	UpdateData(TRUE);
	if(m_whiteClrDlg == true)
	{
		m_crefWhiteColor = cr;
	}
	else 
	{
		m_crefBlackColor = cr;
	}
	((CNetChessView*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetActiveView())->PostMessage(ID_MY_MESSAGE_COLORDATA,0,0);
}

