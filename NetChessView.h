// NetChessView.h : interface of the CNetChessView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETCHESSVIEW_H__5A680802_7FFF_40BB_B3DF_22486247A285__INCLUDED_)
#define AFX_NETCHESSVIEW_H__5A680802_7FFF_40BB_B3DF_22486247A285__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChessBoard.h"
#include "Options.h"
#include "History.h"
 
class CNetChessView : public CView
{
	int i;
	long m_blackTime, m_whiteTime,m_startTime;
	COLOR_TYPE  m_pieceSide;
	bool m_mouseMoveFlag;
	CRect m_moveRect;
	CPoint m_point;
	int m_SpecialAction;
	CAsyncSocket *m_pClientSocket;
	CAsyncSocket *m_pServerSocket;
	CDialog *m_ChatDlg;	 
	bool m_player_turn;
	int m_topHistory;
	int m_squareWidth;
	int m_squareHeight;
	 
protected: // create from serialization only
	CNetChessView();
	DECLARE_DYNCREATE(CNetChessView)
	 

// Attributes
public:

	CString m_serverName, m_clientName;
	bool m_timerFlag;	 
	int m_deckIndex ;
	int m_discardIndex;
	bool m_player_deck_open_flag;
	int m_game_type;
	int m_game_total;
	int m_game_count;
	COptions m_optDlg;
	void DrawBoard();
	CNetChessDoc* GetDocument();
	void SetClientSocket(CAsyncSocket*);
	void SetData(char* data, int length);
	void SendSockData(unsigned char *data,int length);	 
	void KillTimerEvent();
 
	CAsyncSocket* GetClientSocket();
	void Initialize();
	int GetBitmapId(int piece_id);
	void SetPieceSide(COLOR_TYPE piecetype);
	COLOR_TYPE GetPlayerSide();
	void CheckWin();
	CChessBoard cb[2][16];
	CChessBoard deck[52];
	CChessBoard discard[52];
	CHistory  m_History[MAXHISTORY];
	int m_iHistory; 

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetChessView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNetChessView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CNetChessView)
	afx_msg void OnEditOptions();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnToolsClient();
	afx_msg void OnToolsServer();
	afx_msg void OnToolsSendmessage();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileSavehistory();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnUpdateToolsServer(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolsClient(CCmdUI* pCmdUI);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnViewHistory();
	afx_msg void OnHelpHowtoplay();
	afx_msg void OnToolsDisconnect();
	afx_msg void OnEditProperties();
	afx_msg void OnToolsResign();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnToolsShow();
	afx_msg void OnFileNew();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	LRESULT OnMessageColorData(WPARAM wParam,LPARAM lParam);
	void OnLButtonDownAction(UINT nFlags, CPoint point);
	void OnLButtonUpAction(UINT nFlags, CPoint point);
	void OnMouseMoveAction(UINT nFlags, CPoint point);
	void OnFileNewAction();
	CString GetHistoryString();
	void SetShellIconData(char* data, int id);
	void FillTheCards();
	void WriteStatus(CString str);

};

#ifndef _DEBUG  // debug version in NetChessView.cpp
inline CNetChessDoc* CNetChessView::GetDocument()
   { return (CNetChessDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETCHESSVIEW_H__5A680802_7FFF_40BB_B3DF_22486247A285__INCLUDED_)
