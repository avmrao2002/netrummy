#if !defined(AFX_ACCEPTDLG_H__D31BBA34_39E2_4ECA_8126_43EC3B7432A5__INCLUDED_)
#define AFX_ACCEPTDLG_H__D31BBA34_39E2_4ECA_8126_43EC3B7432A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AcceptDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAcceptDlg dialog

class CAcceptDlg : public CDialog
{
// Construction
public:
	CAcceptDlg(CWnd* pParent = NULL);   // standard constructor
	COLOR_TYPE m_pieceSide;
	bool m_tossFlag;
// Dialog Data
	//{{AFX_DATA(CAcceptDlg)
	enum { IDD = IDD_DIALOG_ACCEPT };
	CString	m_AcceptString;
	int		m_edit_value;
	int		m_radio_number;
	int		m_radio_total;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAcceptDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAcceptDlg)
	afx_msg void OnRadioWhite();
	afx_msg void OnRadioBlack();
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioToss();
	afx_msg void OnRadioNumber();
	afx_msg void OnRadioTotal();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACCEPTDLG_H__D31BBA34_39E2_4ECA_8126_43EC3B7432A5__INCLUDED_)
