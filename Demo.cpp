// Demo.cpp : implementation file
//

#include "stdafx.h"
#include "NetChess.h"
#include "Demo.h"
#include "NetChessDoc.h"
#include "NetChessView.h"
 
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDemo dialog


CDemo::CDemo(CWnd* pParent /*=NULL*/)
	: CDialog(CDemo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDemo)
	//}}AFX_DATA_INIT
}


void CDemo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDemo)
	DDX_Control(pDX, IDC_COMBO_TO_NUMBER, m_combo_to_number);
	DDX_Control(pDX, IDC_COMBO_TO_LETTER, m_combo_to_letter);
	DDX_Control(pDX, IDC_COMBO_FROM_NUMBER, m_combo_from_number);
	DDX_Control(pDX, IDC_COMBO_FROM_LETTER, m_combo_from_letter);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDemo, CDialog)
	//{{AFX_MSG_MAP(CDemo)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemo message handlers


void CDemo::OnButtonAdd() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	 
	CString fromLetter,fromNumber,toLetter,toNumber;
	m_combo_from_letter.GetWindowText(fromLetter);
	m_combo_from_number.GetWindowText(fromNumber);
	m_combo_to_letter.GetWindowText(toLetter);
	m_combo_to_number.GetWindowText(toNumber);
	int fromi,fromj,toi,toj;char a,b;
	fromi = 8-atoi(&(a=fromNumber.GetAt(0)));
	fromj = fromLetter.GetAt(0) - 'a';
	toi = 8-atoi(&(b=toNumber.GetAt(0)));
	toj = toLetter.GetAt(0) -'a';

}

void CDemo::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CDemo::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

