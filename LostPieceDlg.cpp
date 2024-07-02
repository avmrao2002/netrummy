// LostPieceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetChess.h"
#include "LostPieceDlg.h"
#include "NetChessDoc.h"
#include "NetChessView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLostPieceDlg dialog


CLostPieceDlg::CLostPieceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLostPieceDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLostPieceDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CLostPieceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLostPieceDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLostPieceDlg, CDialog)
	//{{AFX_MSG_MAP(CLostPieceDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLostPieceDlg message handlers

BOOL CLostPieceDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
 
	for(int j=0, i= IDC_STATIC_W1;i<=IDC_STATIC_W16;i++,j++)
	{
		m_whitePieces[j]=i;
	}
	for(j=0, i= IDC_STATIC_B1;i<=IDC_STATIC_B16;i++,j++)
	{
		m_blackPieces[j]=i;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLostPieceDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here	for(int i=0;i<m_iHistory;i++)
	int blackcount=0;
	int whitecount=0;
	for(int i=0;i<=m_iHistory;i++)
	{
		PIECE_SIDE piece_side;
		PIECE_TYPE from_piece_type;
		COLOR_TYPE from_color_type;
		int from_pieceid;
		int from_row_id;
		int from_col_id;
		PIECE_TYPE to_piece_type;
		COLOR_TYPE to_color_type;
		int to_pieceid;
		int to_row_id;
		int to_col_id;
		m_History[i].GetHistory(
			piece_side,
			from_piece_type, from_color_type,from_pieceid,
			from_row_id,from_col_id, to_piece_type,
			to_color_type,to_pieceid,to_row_id,
			to_col_id);
		if(to_pieceid > 0 || m_History[i].GetSpecialAction() == ENPASSENT)
		{
			CStatic *wnd=NULL;
			if(m_History[i].GetSpecialAction() == ENPASSENT)
			{			 
				to_pieceid = from_color_type == WHITE ? PAWN_BLACK: PAWN_WHITE;
				to_color_type = from_color_type == WHITE ? BLACK: WHITE;
			}
			if(to_color_type == BLACK)
			{
				wnd = (CStatic*)GetDlgItem(m_blackPieces[blackcount++]); 				 
			}
			else if(to_color_type == WHITE)
			{
				wnd = (CStatic*)GetDlgItem(m_whitePieces[whitecount++]); 			 				 
			}			 
			if(wnd != NULL)
			{
				wnd->EnableWindow();
				CBitmap bitmap;				 
				bitmap.LoadBitmap(((CNetChessView*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetActiveView())->GetBitmapId(to_pieceid));
				HBITMAP hbmp = bitmap;				 
				wnd->SetBitmap(hbmp);
				UpdateData(FALSE);
			}
		}
	}

	
	// Do not call CDialog::OnPaint() for painting messages
}
void CLostPieceDlg::SetHistory(CHistory *history,int hcount)
{
	m_History = history;
	m_iHistory = hcount;
}
