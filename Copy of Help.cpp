// Help.cpp : implementation file
//

#include "stdafx.h"
#include "NetChess.h"
#include "Help.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Help dialog


Help::Help(CWnd* pParent /*=NULL*/)
	: CDialog(Help::IDD, pParent)
{
	//{{AFX_DATA_INIT(Help)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void Help::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Help)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Help, CDialog)
	//{{AFX_MSG_MAP(Help)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Help message handlers
