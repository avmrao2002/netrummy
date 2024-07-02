#if !defined(AFX_CARDCOUNTDLG_H__58712059_3B24_4B5C_96F6_52F93B5847AA__INCLUDED_)
#define AFX_CARDCOUNTDLG_H__58712059_3B24_4B5C_96F6_52F93B5847AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CardCountDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCardCountDlg dialog

class CCardCountDlg : public CDialog
{
// Construction
public:
	CCardCountDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCardCountDlg)
	enum { IDD = IDD_DIALOG_CARD_COUNT };
	int		m_card_count;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCardCountDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCardCountDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CARDCOUNTDLG_H__58712059_3B24_4B5C_96F6_52F93B5847AA__INCLUDED_)
