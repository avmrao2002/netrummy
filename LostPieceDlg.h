#if !defined(AFX_LOSTPIECEDLG_H__DD365066_55E5_4494_93F3_F1BAAAE5EFDE__INCLUDED_)
#define AFX_LOSTPIECEDLG_H__DD365066_55E5_4494_93F3_F1BAAAE5EFDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LostPieceDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLostPieceDlg dialog
#include"History.h"
class CLostPieceDlg : public CDialog
{
// Construction
public:
	CLostPieceDlg(CWnd* pParent = NULL);   // standard constructor
	int m_whitePieces[16];
	int m_blackPieces[16];
	CHistory *m_History;
	int m_iHistory;
	void SetHistory(CHistory *history,int hcount);
// Dialog Data
	//{{AFX_DATA(CLostPieceDlg)
	enum { IDD = IDD_DIALOG_LOST_PIECE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLostPieceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLostPieceDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOSTPIECEDLG_H__DD365066_55E5_4494_93F3_F1BAAAE5EFDE__INCLUDED_)
