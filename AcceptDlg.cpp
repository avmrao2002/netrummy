// AcceptDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetChess.h"
#include "AcceptDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAcceptDlg dialog


CAcceptDlg::CAcceptDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAcceptDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAcceptDlg)
	m_AcceptString = _T("");
	m_edit_value = 0;
	m_radio_number = -1;
	m_radio_total = -1;
	//}}AFX_DATA_INIT
	m_pieceSide = WHITE;
	m_tossFlag = false;
}


void CAcceptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAcceptDlg)
	DDX_Text(pDX, IDC_STATIC_ACCEPT, m_AcceptString);
	DDX_Text(pDX, IDC_EDIT_VALUE, m_edit_value);
	DDV_MinMaxInt(pDX, m_edit_value, 0, 50000);
	DDX_Radio(pDX, IDC_RADIO_NUMBER, m_radio_number);
	DDX_Radio(pDX, IDC_RADIO_TOTAL, m_radio_total);
	//}}AFX_DATA_MAP
 
}


BEGIN_MESSAGE_MAP(CAcceptDlg, CDialog)
	//{{AFX_MSG_MAP(CAcceptDlg)
	ON_BN_CLICKED(IDC_RADIO_WHITE, OnRadioWhite)
	ON_BN_CLICKED(IDC_RADIO_BLACK, OnRadioBlack)
	ON_BN_CLICKED(IDC_RADIO_TOSS, OnRadioToss)
	ON_BN_CLICKED(IDC_RADIO_NUMBER, OnRadioNumber)
	ON_BN_CLICKED(IDC_RADIO_TOTAL, OnRadioTotal)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAcceptDlg message handlers

void CAcceptDlg::OnRadioWhite() 
{
	// TODO: Add your control notification handler code here
	if(m_tossFlag  == false)
		m_pieceSide = WHITE;	
}

void CAcceptDlg::OnRadioBlack() 
{
	// TODO: Add your control notification handler code here
	if(m_tossFlag == false)
		m_pieceSide = BLACK;
}

BOOL CAcceptDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CWnd* wnd = GetDlgItem(IDC_RADIO_WHITE);
	wnd->ShowWindow(SW_SHOW);
	CButton* btn = (CButton*)wnd;
	btn->SetCheck(1);	 
	btn = (CButton*)GetDlgItem(IDC_RADIO_NUMBER);
	btn->SetCheck(1);	 

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAcceptDlg::OnRadioToss() 
{
	int randv = rand();	 
	if(m_tossFlag == true)
	{
		AfxMessageBox("No second chance for toss");
		return;
	}
	if(randv %2 == 0)
	{
		m_pieceSide = WHITE;	
		CButton* wnd = (CButton*)GetDlgItem(IDC_RADIO_WHITE);
		wnd->SetCheck(1);
		wnd->EnableWindow(FALSE);
		wnd = (CButton*)GetDlgItem(IDC_RADIO_BLACK);
		wnd->SetCheck(0);
		wnd->EnableWindow(FALSE);
		 
	}
	else
	{
		m_pieceSide = BLACK;	
		CButton* wnd = (CButton*)GetDlgItem(IDC_RADIO_BLACK);
		wnd->SetCheck(1);
		wnd->EnableWindow(FALSE);
		wnd = (CButton*)GetDlgItem(IDC_RADIO_WHITE);
		wnd->SetCheck(0);
		wnd->EnableWindow(FALSE);
	}
	m_tossFlag = true;	 
}

void CAcceptDlg::OnRadioNumber() 
{
	// TODO: Add your control notification handler code here
	CButton* wnd = (CButton*)GetDlgItem(IDC_RADIO_NUMBER);
	wnd->SetCheck(1);	 
	wnd = (CButton*)GetDlgItem(IDC_RADIO_TOTAL);	
	wnd->SetCheck(0);
	
	m_radio_number = true;
	m_radio_total = false;
}

void CAcceptDlg::OnRadioTotal() 
{
	// TODO: Add your control notification handler code here
	CButton* wnd = (CButton*)GetDlgItem(IDC_RADIO_TOTAL);
	wnd->SetCheck(1);	 
	wnd = (CButton*)GetDlgItem(IDC_RADIO_NUMBER);	
	wnd->SetCheck(0);
	m_radio_number = false;
	m_radio_total = true;
}
