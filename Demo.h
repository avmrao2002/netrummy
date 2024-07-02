#if !defined(AFX_DEMO_H__442E7092_6553_4011_8D06_D1930B9883A2__INCLUDED_)
#define AFX_DEMO_H__442E7092_6553_4011_8D06_D1930B9883A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Demo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDemo dialog

class CDemo : public CDialog
{
// Construction
public:
	CDemo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDemo)
	enum { IDD = IDD_DIALOG_DEMO_FEED };
	CComboBox	m_combo_to_number;
	CComboBox	m_combo_to_letter;
	CComboBox	m_combo_from_number;
	CComboBox	m_combo_from_letter;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDemo)
	afx_msg void OnButtonAdd();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMO_H__442E7092_6553_4011_8D06_D1930B9883A2__INCLUDED_)
