// CardCountDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetChess.h"
#include "CardCountDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCardCountDlg dialog


CCardCountDlg::CCardCountDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCardCountDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCardCountDlg)
	m_card_count = 0;
	//}}AFX_DATA_INIT
}


void CCardCountDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCardCountDlg)
	DDX_Text(pDX, IDC_EDIT_COUNT, m_card_count);
	DDV_MinMaxInt(pDX, m_card_count, 2, 65555);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCardCountDlg, CDialog)
	//{{AFX_MSG_MAP(CCardCountDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCardCountDlg message handlers

void CCardCountDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}
