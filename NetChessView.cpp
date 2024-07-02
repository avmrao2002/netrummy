// NetChessView.cpp : implementation of the CNetChessView class
//

#include "stdafx.h"
#include "NetChess.h"
#include "Options.h"
#include "ChessBoard.h"
#include "ServerInfoDlg.h"
#include "ServerSocket.h"
#include "ClientSocket.h"
#include "MessageSend.h"
#include "History.h"
#include "HistoryDlg.h"
#include "AcceptDlg.h"
 
#include "PropertiesDlg.h"
#include "MainFrm.h"
#include "LostPieceDlg.h"
#include "CardCountDlg.h"

#include "NetChessDoc.h"
#include "NetChessView.h"
#include "HelpDlg.h"
 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNetChessView
void writeMessage(char *str,...);
IMPLEMENT_DYNCREATE(CNetChessView, CView)

BEGIN_MESSAGE_MAP(CNetChessView, CView)
	//{{AFX_MSG_MAP(CNetChessView)
	ON_COMMAND(ID_EDIT_OPTIONS, OnEditOptions)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_TOOLS_CLIENT, OnToolsClient)
	ON_COMMAND(ID_TOOLS_SERVER, OnToolsServer)
	ON_COMMAND(ID_TOOLS_SENDMESSAGE, OnToolsSendmessage)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVEHISTORY, OnFileSavehistory)
	ON_WM_RBUTTONDOWN()
	ON_UPDATE_COMMAND_UI(ID_TOOLS_SERVER, OnUpdateToolsServer)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_CLIENT, OnUpdateToolsClient)
	ON_WM_TIMER()	 
	ON_COMMAND(ID_VIEW_HISTORY, OnViewHistory)
	ON_COMMAND(ID_HELP_HOWTOPLAY, OnHelpHowtoplay)
	ON_COMMAND(ID_TOOLS_DISCONNECT, OnToolsDisconnect)
	ON_COMMAND(D_EDIT_PROPERTIES, OnEditProperties)
	ON_COMMAND(ID_TOOLS_RESIGN, OnToolsResign)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_TOOLS_SHOW, OnToolsShow)
	ON_COMMAND(ID_EDIT_PROPERTIES, OnEditProperties)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_MESSAGE(ID_MY_MESSAGE_COLORDATA,OnMessageColorData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetChessView construction/destruction

CNetChessView::CNetChessView()
{
	// TODO: add construction code here
	m_mouseMoveFlag = false;
	m_moveRect =0;
	m_pClientSocket = NULL;
	m_pServerSocket = NULL;
	m_iHistory = -1;
	m_game_count = 0;
	m_player_turn = false;
	m_topHistory = -1;
	m_timerFlag = false;
	m_squareWidth = 80;
	m_squareHeight = 100;
	m_pieceSide = WHITE;
	m_blackTime = m_whiteTime = 0;
	m_startTime = time(0);
 	m_SpecialAction = NOTHING;
	m_player_deck_open_flag = true;	 
	m_discardIndex=0;
}

CNetChessView::~CNetChessView()
{
}

int checkExisting(int n, int *arr, int rand)
{
	for(int i=0;i<n;i++)
	{
		if(arr[i] == rand)
			return 1;
	}
	return 0;
}

int getExisting(int n,int* arr, int* data)
{	 
	int foundFlag = 0; 	 	 
	for(int i=0;i<n;i++)
	{		 
		if(data[i] == 0) continue;
 		for(int j =0;j<n;j++)
		{
 			if(arr[j]==0)
			{
			 			 
				return data[i];
			}
				
			if(arr[j] == data[i])
			{				 
				foundFlag = 1;
				break;
			}
		}
		if(foundFlag == 0)
		{	 			 
			return data[i];
		}
	}
 	return -1;
}
void getRandom(int num,int arr[100])
{
	int i=0;
	srand( (unsigned)time( NULL ) );
	int data[100];
	for(int i=0;i<100;i++)
	{
		data[i] = i+1;
	}

	for(i=0;i<100;i++)
	{
		arr[i]=0;
	}
	int trycount = 0;
	for(i=0;i<num;i++)
	{
			 
			int rnd = (rand() % ((num)+1));
 			if( rnd == 0 || checkExisting(num,arr,rnd) == 1)
			{//try once again
				if(trycount==1000)
				{
					int retdata = getExisting(num,arr,data);
					if(retdata < 0)
					{
						return;
					}
					else
					{
						trycount = 0;
						arr[i] = retdata;
					}
				}
				else
				{
					trycount++;
					i--;
				}
				 
			}
			else
			{
				trycount=0;
				arr[i] = rnd;
			}
	} 
}


BOOL CNetChessView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CNetChessView drawing

void CNetChessView::OnDraw(CDC* pDC)
{
	CNetChessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	DrawBoard(); 
}

/////////////////////////////////////////////////////////////////////////////
// CNetChessView printing

BOOL CNetChessView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CNetChessView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CNetChessView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CNetChessView diagnostics

#ifdef _DEBUG
void CNetChessView::AssertValid() const
{
	CView::AssertValid();
}

void CNetChessView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CNetChessDoc* CNetChessView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNetChessDoc)));
	return (CNetChessDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNetChessView message handlers

void CNetChessView::OnEditOptions() 
{
	m_optDlg.DoModal();	 
	DrawBoard();
}

void CNetChessView::FillTheCards()
{
	int i=1;
	for(int j=0;j<13;j++)
	{
		cb[0][j].SetPieceData(deck[i].GetPieceId(),WHITE,ROOK,PIECE_NOT_MOVING);
		i+=2;
	}
	cb[0][15].SetPieceData(deck[26].GetPieceId(),WHITE,ROOK,PIECE_NOT_MOVING);
	discard[m_discardIndex++].SetPieceData(deck[26].GetPieceId(),WHITE,ROOK,PIECE_NOT_MOVING);
	char data[50];
	memset(data,-1,50);
	int count=0;
	data[count++] = DECK_SETUP;
	data[count++] = m_pieceSide;
	for(i=0;i<26;i+=2)
	{
		data[count++] = deck[i].GetPieceId() % 200;
	}
	for(i=26;i<52;i++)
	{
		data[count++] = deck[i].GetPieceId() % 200;
	}	 
	for(i=27;i<52;i++)
	{
		deck[i-27].SetPieceId(deck[i].GetPieceId());
		deck[i].SetPieceData(-1,NONE,BLANK,PIECE_NOT_MOVING);
	}
	deck[25].SetPieceData(-1,NONE,BLANK,PIECE_NOT_MOVING);
	deck[26].SetPieceData(-1,NONE,BLANK,PIECE_NOT_MOVING);
	m_deckIndex = 24;
 
	SendSockData((unsigned char*)data,count);						 


//	cb[0][13].SetPieceData(-1,NONE,BLANK,PIECE_NOT_MOVING);*/
	//cb[0][14].SetPieceData(200,WHITE,ROOK,PIECE_NOT_MOVING);
	///cb[0][15].SetPieceData(200,WHITE,ROOK,PIECE_NOT_MOVING);
	
}

void CNetChessView::DrawBoard()
{
//(CMainFrame*)((CNetChessApp*)AfxGetApp())->m_pMainWnd)->m_wndStatusBar.SetPaneText(2,m_serverName);	((CMainFrame*)((CNetChessApp*)AfxGetApp())->m_pMainWnd)->m_wndStatusBar.SetPaneText(4,m_clientName);
	CClientDC dc(this);	 
	CBitmap localbmp;
	localbmp.LoadBitmap(IDB_BITMAP_BASE);
	//BITMAP bp={0,1000,490,3000,1,24,0x00000000};
	//localbmp.CreateBitmapIndirect(&bp);

	CDC ldc;
	ldc.CreateCompatibleDC(&dc);
	ldc.SelectObject(&localbmp); 
	COLORREF cr(RGB(20,140,17));	 	 
	CBrush brush(m_optDlg.m_crefWhiteColor);
	ldc.SelectObject(brush);
	CRect crect;
	GetClientRect(&crect);
	 
 
	ldc.FillRect(&crect,&brush);
	COLORREF bkcrRef(RGB(255,0,0));	 	 
	COLORREF redcr(RGB(255,0,0));

	CBrush redbrush;
	redbrush.CreateSolidBrush(redcr);
	
	COLORREF redbluecr(RGB(255,0,255));
	CBrush redbluebrush;
	redbluebrush.CreateSolidBrush(redbluecr);

	CBrush* pbrush = ldc.SelectObject(&redbluebrush);

	ldc.Rectangle(crect.left + 4, crect.top + 4,
		crect.right - 4, crect.bottom - 4);

	 
	COLORREF bluecr(RGB(0,255,255));//ball color
	CBrush bluebrush;
	bluebrush.CreateSolidBrush(bluecr);

	COLORREF greencr(RGB(0,255,0));
	CBrush greenbrush;
	greenbrush.CreateSolidBrush(greencr);

	ldc.SelectObject(brush);

	ldc.Rectangle(crect.left +21, crect.top +21,
		crect.right - 21, crect.bottom -21);

	pbrush =  ldc.SelectObject(&redbluebrush);
	
	 
	for(int i = 0; i < 2; i++)
	{
		for( int j = 0; j < 16; j++)
		{
			CRect rect = cb[i][j].GetRect();		 
			 
			if(cb[i][j].GetPieceType() != BLANK && cb[i][j].GetPieceState() != PIECE_MOVING)
			{
				int piece_id;
				COLOR_TYPE  piece_color;
				PIECE_TYPE  piece_type;
				STATE piece_state;
				cb[i][j].GetPieceData(piece_id,piece_color,piece_type,piece_state);
				if(piece_id <= 0)
				{
					break;
				}
				CBitmap bitmap;
				//bitmap.LoadBitmap(piece_id);			 
				bitmap.LoadBitmap(piece_id);			 
				CDC bmpdc;
				bmpdc.CreateCompatibleDC(&ldc);
				bmpdc.SelectObject(&bitmap); 

				BITMAP bmp;
				bitmap.GetBitmap(&bmp);
				//ldc.SetStretchBltMode(HALFTONE);
				//POINT pt;
				//SetBrushOrgEx(ldc.GetSafeHdc(),0,0,&pt);
				ldc.BitBlt(rect.left,rect.top,m_squareWidth,m_squareHeight,&bmpdc,0,0, SRCCOPY);
				//ldc.StretchBlt(rect.left,rect.top,m_squareWidth,m_squareHeight,&bmpdc,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY); 
			}
			///ldc.SelectObject(&redbluebrush);
		}			
	}	 

	COLORREF cr1(RGB(255,255,255));
	ldc.SetBkColor(cr1);
	 
	if(m_mouseMoveFlag == true)
	{
		CRect rect = m_moveRect;
		int piece_id;
		ldc.SetBkColor(bkcrRef);
		COLOR_TYPE  piece_color;
		PIECE_TYPE  piece_type;
		STATE piece_state;
		cb[m_point.x][m_point.y].GetPieceData(piece_id,piece_color,piece_type,piece_state);
		if(piece_id < 0)
		{
			dc.BitBlt(0,0,600,700,&ldc,0,0,SRCCOPY);  
			return;
		}

		CBitmap bitmap;
		//bitmap.LoadBitmap(piece_id);
		bitmap.LoadBitmap(piece_id);
		CDC bmpdc;
		bmpdc.CreateCompatibleDC(&ldc);
		bmpdc.SelectObject(&bitmap); 
		bmpdc.SelectObject(&bkcrRef);

		BITMAP bmp;
		bitmap.GetBitmap(&bmp);		 
		ldc.BitBlt(rect.left,rect.top,m_squareWidth,m_squareHeight,&bmpdc,0,0, SRCCOPY);
		//ldc.StretchBlt(rect.left,rect.top,m_squareWidth,m_squareWidth,&bmpdc,0,0,bmp.bmWidth,bmp.bmHeight,MERGECOPY); 		 		 
		CRect rect1=cb[0][15].GetRect();
		/*CBitmap bitmap1;
		//bitmap.LoadBitmap(piece_id);
		bitmap1.LoadBitmap(255);
		CDC bmpdc1;
		bmpdc1.CreateCompatibleDC(&ldc);
		bmpdc1.SelectObject(&bitmap); 
		bmpdc1.SelectObject(&bkcrRef);		 
		ldc.BitBlt(rect1.left,rect1.top,m_squareWidth,m_squareHeight,&bmpdc,0,0, SRCCOPY);*/
	}
	dc.BitBlt(0,0,600,700,&ldc,0,0,SRCCOPY);  
}

void CNetChessView::Initialize()
{
	m_mouseMoveFlag = false;
	m_moveRect =0;	  
	m_topHistory = -1;
	m_iHistory = -1;
	m_timerFlag = false;

	m_squareWidth = 80;
	m_squareHeight = 100;
 	//m_blackTime = m_whiteTime  = 0;
	//m_startTime = time(0);
	m_SpecialAction = NOTHING;
	CString str;
	str.Format("0:0:0");	 
	//CMainFrame*)((CNetChessApp*)AfxGetApp())->m_pMainWnd)->m_wndStatusBar.SetPaneText(3,str);
	//CMainFrame*)((CNetChessApp*)AfxGetApp())->m_pMainWnd)->m_wndStatusBar.SetPaneText(5,str);
	m_player_turn = false;	
	m_player_deck_open_flag = true;
	m_discardIndex=0;
}

int CNetChessView::GetBitmapId(int piece_id)
{	 
	return 1;
}

void CNetChessView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	if(m_ChatDlg == NULL)
	{
		m_ChatDlg = new CMessageSend();
		m_ChatDlg->Create(IDD_DIALOG_MESSAGE,this);
	}
	

	int xstart = 30;
	int ystart = 30;
	int x,y=ystart;
	for ( int i = 0; i < 2; i++)
	{
		x = xstart;
		for( int j = 0; j <= 13; j++)
		{
			CRect rect(x,y, x+ m_squareWidth, y+m_squareHeight);

			cb[i][j].SetRect(rect);
			cb[i][j].SetPieceType(BLANK);			 
			cb[i][j].SetPieceData(-1,NONE,BLANK,PIECE_NOT_MOVING);
			 
			x += 35;
		}
		y += 300;
	}
	 
	{
		CRect rect(100,200, 100+ m_squareWidth, 200+m_squareHeight);
		cb[0][14].SetRect(rect);
		cb[0][14].SetPieceType(BLANK);			 
		cb[0][14].SetPieceData(-1,NONE,BLANK,PIECE_NOT_MOVING);
	}

	{
		CRect rect(200,200, 200+ m_squareWidth, 200+m_squareHeight);
		cb[0][15].SetRect(rect);
		cb[0][15].SetPieceType(BLANK);			 
		cb[0][15].SetPieceData(-1,NONE,BLANK,PIECE_NOT_MOVING);
	}

	COLOR_TYPE typex = BLACK;
	 
	 
	/*for(int j=0;j<13;j++)
	{
		cb[0][j].SetPieceData(201+j,WHITE,ROOK,PIECE_NOT_MOVING);
	}
	cb[0][13].SetPieceData(-1,NONE,BLANK,PIECE_NOT_MOVING);*/
	cb[0][14].SetPieceData(200,WHITE,ROOK,PIECE_NOT_MOVING);
	cb[0][15].SetPieceData(255,WHITE,ROOK,PIECE_NOT_MOVING);
	 
	int arr[100];

	getRandom(52,arr);

	for(i=0;i<52;i++)
	{
		deck[i].SetPieceData(200+arr[i],WHITE,ROOK,PIECE_NOT_MOVING);
		 
	}
	
	SetTimer(PIECE_SIDE_TIMER_EVENT_ID,1000,NULL);	 
}

void CNetChessView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	OnLButtonDownAction(nFlags,point);	 
 	CView::OnLButtonDown(nFlags, point);
}

void CNetChessView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	OnLButtonUpAction(nFlags,point);	 
	CView::OnLButtonUp(nFlags, point);
}

void CNetChessView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	 
	OnMouseMoveAction(nFlags,point);
	CView::OnMouseMove(nFlags, point);
}

void CNetChessView::OnToolsClient() 
{
	// TODO: Add your command handler code here
	if(m_pServerSocket != NULL)
	{
		AfxMessageBox("Running as Server, cannot run as client");
		return;
	}
	if(m_pClientSocket != NULL)
	{
		AfxMessageBox("Already running as client");
		return;
	}
	if(m_iHistory > -1)
	{
		if(AfxMessageBox("Do you want to save the playing game",MB_YESNO) == IDYES)
		{
			OnFileSave();
			 
		}
		Initialize();
		OnInitialUpdate();
		DrawBoard();
	}
	CServerInfoDlg dlg(this,CLIENT);
	if(dlg.DoModal()==IDOK)
	{
		m_pClientSocket = new CClientSocket();
		CString str;
		((CClientSocket*)m_pClientSocket)->SetInfo(dlg.m_strIPAddress, dlg.m_port_nunber);
		m_pClientSocket->Create();
		m_pClientSocket->Connect(dlg.m_strIPAddress, dlg.m_port_nunber) !=0  || GetLastError();
		m_pieceSide = dlg.m_pieceSide; 
	}		
}

void CNetChessView::OnToolsServer() 
{
	// TODO: Add your command handler code here
	if(m_pServerSocket != NULL)
	{
		AfxMessageBox("Already running as server");
		return;
	}
	if(m_pClientSocket != NULL)
	{
		AfxMessageBox("Already running as client");
		return;
	}
	if(m_iHistory > -1)
	{
		if(AfxMessageBox("Do you want to save the playing game",MB_YESNO) == IDYES)
		{
			OnFileSave();
			 
		}
		Initialize();
		OnInitialUpdate();
		DrawBoard();
	}
	CServerInfoDlg dlg(this,SERVER);
	if(dlg.DoModal()==IDOK)
	{ 
		m_pServerSocket = new CServerSocket();
		((CServerSocket*)m_pServerSocket)->SetInfo(dlg.m_port_nunber);
		((CServerSocket*)m_pServerSocket)->CreateServer(dlg.m_port_nunber);		 
	 
	}
}

void CNetChessView::SetClientSocket(CAsyncSocket* sock)
{
	if(m_iHistory > -1)
	{
		if(AfxMessageBox("Do you want to save the playing game",MB_YESNO) == IDYES)
		{
			OnFileSave();		 	 
		}
		Initialize();
		OnInitialUpdate();
		DrawBoard();
	}
	m_pClientSocket = sock;
}

CAsyncSocket* CNetChessView::GetClientSocket()
{	 
	return m_pClientSocket;
}

void CNetChessView::SetShellIconData(char* data, int id)
{
	NOTIFYICONDATA nicondata;					
	nicondata.hWnd = AfxGetApp()->m_pMainWnd->GetSafeHwnd();
	nicondata.uID = 10;
	nicondata.uFlags = NIF_ICON |NIF_MESSAGE | NIF_TIP; 
	nicondata.cbSize = sizeof(nicondata);
	nicondata.uCallbackMessage = MY_MESSAGE_SHELLNOTIFY;
	nicondata.hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	strcpy(nicondata.szTip,data);
	Shell_NotifyIcon(id,&nicondata);
}

void CNetChessView::SetData(char* data,int length)
{
	switch(data[0])
	{
		case MOVE:
			{				 
				m_player_turn = true;
				int piece_id;				 
				memcpy(&piece_id, &data[1],4);
				cb[0][15].SetPieceData(piece_id,WHITE,ROOK,PIECE_NOT_MOVING);									 
 				DrawBoard();
				SetShellIconData("Changed the move state",NIM_ADD);				 
				KillTimer(SHELL_ICON_TIMER_EVENT_ID);
				SetTimer(SHELL_ICON_TIMER_EVENT_ID,1000,NULL);
				m_timerFlag = true;
				m_player_deck_open_flag = true; 
				WriteStatus("Recieved Move action");

			}
			break;
		case DECK_MOVE:
			{
				int piece_id;
				COLOR_TYPE  piece_color;
				PIECE_TYPE  piece_type;
				STATE piece_state;
				int to_piece_id;
				COLOR_TYPE  to_piece_color;
				PIECE_TYPE  to_piece_type;
				STATE to_piece_state;				 
				 
				deck[m_deckIndex--].GetPieceData(piece_id,piece_color,piece_type,piece_state);		
				discard[m_discardIndex++].SetPieceData(piece_id,piece_color,piece_type,piece_state);							 
				SetShellIconData("Changed the move state",NIM_ADD);				 
				KillTimer(SHELL_ICON_TIMER_EVENT_ID);
				SetTimer(SHELL_ICON_TIMER_EVENT_ID,1000,NULL);
			}
			break;
		case DECK_SETUP:
			{
				 
				for(int i=0;i<52;i++)
				{
					deck[i].SetPieceData(-1,NONE,BLANK,PIECE_NOT_MOVING);
				}
				m_pieceSide = (COLOR_TYPE)data[1];
				SetPieceSide((COLOR_TYPE)data[1]);				 
				for(i=0;i<13;i++)
				{
					 
					cb[0][i].SetPieceData(200+data[i+2],WHITE,ROOK,PIECE_NOT_MOVING);					
				}
				cb[0][15].SetPieceData(200+data[i+2],WHITE,ROOK,PIECE_NOT_MOVING);					
				discard[m_discardIndex++].SetPieceData(200+data[i+1],WHITE,ROOK,PIECE_NOT_MOVING);
				for(i=14;i<length;i++)
				{
					deck[i-14].SetPieceData(200+data[i+2],WHITE,ROOK,PIECE_NOT_MOVING);					
					
				}
				for(i=25;i<52;i++)
				{					 
					deck[i].SetPieceData(-1,NONE,BLANK,PIECE_NOT_MOVING);
				}
			 
				m_deckIndex = 24;

				DrawBoard();
			}
			break;
		case DECK_RESHUFFLE:
			{
				for(int i=0;i<length-1;i++)
				{
					deck[i].SetPieceData(200+data[i+1],WHITE,ROOK,PIECE_NOT_MOVING);
					writeMessage("reshuffled received data is %d",data[i+1]);
				}
				m_deckIndex = length-1;
				m_discardIndex = 0;
			}
			break;
		case SHOW:
			{
				for(int i=0;i<13;i++)
				{
					cb[1][i].SetPieceData(200+data[i+1],WHITE,ROOK,PIECE_NOT_MOVING);	
				}
				DrawBoard();
				char data1[15];
				if(AfxMessageBox("Received Show message, check the cards, Accept",MB_YESNO)== IDYES)
				{
					data1[1] = 1;
				}
				else
				{
					data1[1] = 0;
					if(m_pServerSocket != NULL)
					{
						m_iHistory++;
						m_History[m_iHistory].SetHistory(0,80);
					}
					else
					{
						m_iHistory++;
						m_History[m_iHistory].SetHistory(80,0);
					}
				}
				data1[0]=SHOW_REPLY;
				for(i=1;i<14;i++)
				{
					data1[i+1] = cb[0][i-1].GetPieceId() % 200;
				}
				SendSockData((unsigned char*)data1,15);		 	 
				 
			}
			WriteStatus("Recieved Show message");
			break;
		case CARD_COUNT:
			{
				int count=0;
				memcpy(&count,&data[1],4);
				if(m_pServerSocket != NULL)
				{
					m_iHistory++;
					m_History[m_iHistory].SetHistory(count,0);
				}
				else
				{
					m_iHistory++;
					m_History[m_iHistory].SetHistory(0,count);
				}
				CheckWin();
			}
			break;
		case SHOW_REPLY:
			{
				if(data[1] == 0)
				{
					AfxMessageBox("Rejected  Show offer, count 80 will be added into your total");
					if(m_pServerSocket != NULL)
					{
						m_iHistory++;
						m_History[m_iHistory].SetHistory(80,0);
					}
					else
					{
						m_iHistory++;
						m_History[m_iHistory].SetHistory(0,80);
					}
				}
				else
				{
					for(int i=0;i<13;i++)
					{
						cb[1][i].SetPieceData(200+data[i+2],WHITE,ROOK,PIECE_NOT_MOVING);	
					}
					DrawBoard();				 
					CCardCountDlg dlg;
					dlg.DoModal();
					char data2[5];
					memset(data2,'\0',5);
					data2[0]=CARD_COUNT;
					int c=dlg.m_card_count;
					memcpy(&data2[1],&c,4);
					SendSockData((unsigned char*)data2,5);		 	 
					if(m_pServerSocket != NULL)
					{
						m_iHistory++;
						m_History[m_iHistory].SetHistory(0,dlg.m_card_count);
					}
					else
					{
						m_iHistory++;
						m_History[m_iHistory].SetHistory(dlg.m_card_count,0);
					}
					CheckWin();
				}

			}
			break;
		case TEXT:
			{	
				((CMessageSend*)m_ChatDlg)->SetReceiveData(&data[1]);	
				DrawBoard();
				SetShellIconData("Message received",NIM_ADD);				 				 
				KillTimer(SHELL_ICON_TIMER_EVENT_ID);
				SetTimer(SHELL_ICON_TIMER_EVENT_ID,1000,NULL);
				m_timerFlag = true;				 
			}
			break;
	 
		case FILEDATA:
							 
			break;
		 
		case NEWGAME:
				{
					SetShellIconData("Received new game request",NIM_ADD);				 
					//DrawBoard();
					m_blackTime = m_whiteTime = 0;
					m_startTime = time(0);
					KillTimer(SHELL_ICON_TIMER_EVENT_ID);
					m_timerFlag = true;
					SetTimer(SHELL_ICON_TIMER_EVENT_ID,1000,NULL);
					CAcceptDlg dlg;
					dlg.m_AcceptString = "Your opponent has requested for new game";
					if(dlg.DoModal() == IDOK)
					{							
						OnFileNewAction();
						SetPieceSide((COLOR_TYPE)dlg.m_pieceSide);
						m_pieceSide = dlg.m_pieceSide;
						FillTheCards();						 
						DrawBoard();
					}
					else
					{
						unsigned char data1[1];
						data1[0] = NOTACCEPTED;
						SendSockData(data1,1);
					}
				}
				break;
	
		case NOTACCEPTED:
				AfxMessageBox("Not accepted for playing the game");
				SetShellIconData("Server not accepted",NIM_ADD);				 
				break;

		case RESIGN_REQUEST:
				{
					WriteStatus("Recieved Resign request");
					SetShellIconData("Received Drop request",NIM_ADD);
					int count=0;
					memcpy(&count,&data[1],4);
					if(m_pServerSocket != NULL)
					{
						m_iHistory++;
						m_History[m_iHistory].SetHistory(0,count);
					}
					else
					{
						m_iHistory++;
						m_History[m_iHistory].SetHistory(count,0);
					}
					CString str = (CString)&data[5] + (CString)" resigned from the game";
					AfxMessageBox(str);
					CheckWin();
					//OnFileNew();
				}

				break;		
		case ACCEPTED:
			{
				((CNetChessView*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetActiveView())->m_serverName = (char*)&data[1];		 
				char comp[50];
				memset(comp,'\0',50);
				gethostname(comp,50);				 				 
				((CNetChessView*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetActiveView())->m_clientName = comp; 
				CString side;
				SetPieceSide((COLOR_TYPE)data[1]);				 
				 
				DrawBoard();
				m_blackTime = m_whiteTime = 0;
				m_startTime = time(0);
				CString msg = (CString)&data[2] + (CString)" Accepted for plaing chess with " + side + "Side";

				 
				memset(comp,'\0',50);
				gethostname(comp,50);
				unsigned char data[50];
				memset(data,'\0',50);
				data[0] = ACCEPTED_WITH_INFO;				 
				strcpy((char*)&data[1],comp);	 	 				 
				int length = strlen(comp)+3;				 
				SendSockData(data,length);
				///AfxMessageBox(msg);				 
			}
			break;
		case ACCEPTED_WITH_INFO:
			{
				 
				m_clientName = (char*)&data[1]; 
			}
			break;
		case YOU_WON:
			{
				WriteStatus("You won the game");
				int servercount=0, clientcount=0;
				for(int i=0;i<=m_iHistory;i++)
				{
					int k,l;
					m_History[i].GetHistory(k,l);
					 
					servercount += k;
					clientcount += l;
				}
				CString str;
				str.Format("%s=%d;\t%s=%d",m_serverName, servercount, m_clientName, clientcount);
				AfxMessageBox("Congratulation! you won the game " + str);
			}
			break;
		case YOU_LOST:
			{	
				WriteStatus("Recieved Lose the game");
				int servercount=0, clientcount=0;
				for(int i=0;i<=m_iHistory;i++)
				{
					int k,l;
					m_History[i].GetHistory(k,l);
					 
					servercount += k;
					clientcount += l;
				}
				CString str;
				str.Format("%s=%d;\t%s=%d",m_serverName, servercount, m_clientName, clientcount);
				AfxMessageBox("Sorry! you lost the game " + str);
			}
			break;
		case MATCH_DRAWN:
			{	
				WriteStatus("Recieved game drawn");
				int servercount=0, clientcount=0;
				for(int i=0;i<=m_iHistory;i++)
				{
					int k,l;
					m_History[i].GetHistory(k,l);
					 
					servercount += k;
					clientcount += l;
				}
				CString str;
				str.Format("%s=%d;\t%s=%d",m_serverName, servercount, m_clientName, clientcount);
				AfxMessageBox("Game drawn " + str);
			}
			break;
		default:
			break;
	}
 
}
void CNetChessView::SendSockData(unsigned char *data,int length)
{
	char clength[5];	 
	memcpy(clength,&length,4);
	if(m_pClientSocket != NULL)
	{	 
		 
		m_pClientSocket->Send(clength,4);	 
		m_pClientSocket->Send(data,length);	 	
	}
	else
	{
		 
	}
}

void CNetChessView::OnToolsSendmessage() 
{
	if(m_pClientSocket != NULL)
	{	
			m_ChatDlg->ShowWindow(SW_SHOW);	
	}
	else
	{
		AfxMessageBox("Not connected to network");
	}
}

void CNetChessView::OnFileOpen() 
{
	if(m_iHistory > -1)
	{
		if(AfxMessageBox("Do you want to save the playing game",MB_YESNO) == IDYES)
		{
			OnFileSave();		 
		}
	}
	CFileDialog fdialog(TRUE);
	if(fdialog.DoModal() == IDOK)
	{
	 
	}
	DrawBoard();	
}

void CNetChessView::OnFileSave() 
{
	// TODO: Add your command handler code here
	if(m_iHistory <0)
	{
		AfxMessageBox("Game not started");
		return;
	}

	CFileDialog fdialog(FALSE);
	if(fdialog.DoModal() == IDOK)
	{ 
		CString file = fdialog.GetPathName(); 
		FILE *fp = fopen(file,"w");
		CString str = GetHistoryString(); 
	 
		fprintf(fp,"%s\n",str.GetBuffer(0));
		str.ReleaseBuffer(0);	 
		fclose(fp);
	 
		AfxMessageBox("Game is saved");
	}	 
}
 
LRESULT CNetChessView::OnMessageColorData(WPARAM wParam,LPARAM lParam)
{	 
	DrawBoard();
	return 0;
}


void CNetChessView::OnFileSavehistory() 
{
	// TODO: Add your command handler code here
	if(m_iHistory < 0)
	{
		AfxMessageBox("No history to save");
		return;
	}
	CFileDialog fdialog(FALSE);
	if(fdialog.DoModal() == IDOK)
	{
		CString file = fdialog.GetPathName(); 
		FILE *fp = fopen(file,"w");		 
		CString m_edit_history = GetHistoryString();;		 
		fprintf(fp,"%s",m_edit_history.GetBuffer(0));
		m_edit_history.ReleaseBuffer(0);
		fclose(fp);
	}
	DrawBoard();
}
void CNetChessView::CheckWin()
{
	if(m_pServerSocket == NULL)
		return;

	int servercount=0, clientcount=0;
	for(int i=0;i<=m_iHistory;i++)
	{
		int k,l;
		m_History[i].GetHistory(k,l);
		 
		servercount += k;
		clientcount += l;
	}
	CString str;
	str.Format("%s=%d, %s=%d",m_serverName,servercount, m_clientName, clientcount);
	if(m_game_type = FIXED_TYPE)
	{
	 	if(servercount > m_game_total)
		{	
			unsigned char data[1];
			memset(data,'\0',1);
			data[0] = YOU_WON;		 
			SendSockData(data,1);
			AfxMessageBox("Sorry! you lost the game " + str);	 		 
			 
		}
		else if(clientcount > m_game_total)
		{
			unsigned char data[1];
			memset(data,'\0',1);
			data[0] = YOU_LOST;		 
			SendSockData(data,1);	 
			AfxMessageBox("Congratulation! you won the game " + str);		 
		}
	}
	else if(m_game_type == NUMBER_TYPE && m_game_count >= m_game_total)
	{
		if(servercount > clientcount)
		{	
			unsigned char data[1];
			memset(data,'\0',1);
			data[0] = YOU_WON;		 
			SendSockData(data,1);
			AfxMessageBox("Sorry! you lost the game " + str);
	 		 
			 
		}
		else if(servercount < clientcount  )
		{
			unsigned char data[1];
			memset(data,'\0',1);
			data[0] = YOU_LOST;		 
			SendSockData(data,1);	 
			AfxMessageBox("Congratulation! you won the game " + str);		 
		}
		else if(servercount == clientcount)
		{
			unsigned char data[1];
			memset(data,'\0',1);
			data[0] = MATCH_DRAWN;		 
			SendSockData(data,1);	 
			AfxMessageBox("Match drawn " + str);		 
		}
	}
}

CString CNetChessView::GetHistoryString()
{
	CString edit_string = m_serverName + "    " + m_clientName + "\r\n";
	writeMessage("ihistory is %d",m_iHistory);
	int servercount=0, clientcount=0;
	for(int i=0;i<=m_iHistory;i++)
	{
		int k,l;
		m_History[i].GetHistory(k,l);
		CString str;
		str.Format("%d) %d\t%d\r\n",i+1,k,l);
		edit_string += str;
		servercount += k;
		clientcount += l;
	}
	edit_string += "\r\n****Total****\r\n";
	CString str;
	str.Format("%d\t%d\r\n",servercount,clientcount);
	edit_string += str;
 	return edit_string;
}

 
 
void CNetChessView::OnLButtonDownAction(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SetShellIconData("NetChess by A.V.Maheswara Rao",NIM_DELETE);				 	 
	KillTimer(SHELL_ICON_TIMER_EVENT_ID);
	m_timerFlag = false;
	 
	if(m_pClientSocket == NULL)
		return;
	 
	for(int i = 0; i < 1; i++)
	{
		for( int j = 0; j <= 13; j++)
		{
			if(cb[i][j].GetPieceId() == -1)
				continue;
			CRgn rgn;
			CRect rect = cb[i][j].GetRect();
			rgn.CreateEllipticRgn(rect.left, rect.top, rect.right, rect.bottom);
			if(rgn.PtInRegion(point))
			{
			
			//	if(m_pieceSide == cb[i][j].GetPieceColor())
				{		 				 
					 
					m_point.x = i; m_point.y = j;
					m_moveRect = rect;					 
			 		m_mouseMoveFlag = true;					 					 
				}
			}
		}
	}
	if(m_mouseMoveFlag == true)
	{
		cb[m_point.x][m_point.y].SetPieceState(PIECE_MOVING);
		return;
	}
	CRgn rgn;
	CRect rect = cb[0][15].GetRect();
	rgn.CreateEllipticRgn(rect.left, rect.top, rect.right, rect.bottom);
	if(rgn.PtInRegion(point))
	{
	 	m_point.x = 0; m_point.y = 15;
		m_moveRect = rect;
		cb[0][15].SetPieceState(PIECE_MOVING);
		m_mouseMoveFlag = true;			 
		m_player_deck_open_flag = false;
		return;	 
	} 
 
 	CView::OnLButtonDown(nFlags, point);
}

void CNetChessView::OnLButtonUpAction(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_mouseMoveFlag == false)
	{	 
		return;
	}	
	 
	m_mouseMoveFlag = false;
	if(m_pClientSocket == NULL)
		return;
	m_moveRect =0;
	cb[m_point.x][m_point.y].SetPieceState(PIECE_NOT_MOVING);
 
    for(int i = 0; i < 1; i++)
	{
		for( int j = 0; j <= 13; j++)
		{			 
			if(cb[i][j].GetPieceId() == -1)
				continue;
 			CRgn rgn;
			CRect rect = cb[i][j].GetRect();
			rgn.CreateEllipticRgn(rect.left, rect.top, rect.right, rect.bottom);	
			if(rgn.PtInRegion(point)&&rect != cb[m_point.x][m_point.y].GetRect())
			{
				int piece_id;
				COLOR_TYPE  piece_color;
				PIECE_TYPE  piece_type;
				STATE piece_state;
				int to_piece_id;
				COLOR_TYPE  to_piece_color;
				PIECE_TYPE  to_piece_type;
				STATE to_piece_state;
				cb[m_point.x][m_point.y].GetPieceData(piece_id,piece_color,piece_type,piece_state);
				cb[i][j].GetPieceData(to_piece_id,to_piece_color,to_piece_type,to_piece_state);					 
				if(m_point.y < 14) 
				{
					if(m_point.y < j)
					{
						for(int k=m_point.y+1;k <= j;k++)
						{
							PIECE_SIDE piece_side;
							PIECE_TYPE from_piece_type;
							COLOR_TYPE from_color_type;
							int from_pieceid;
							STATE from_piece_state; 
							cb[0][k].GetPieceData(from_pieceid,from_color_type,from_piece_type,from_piece_state);
							cb[0][k-1].SetPieceData(from_pieceid,from_color_type,from_piece_type,from_piece_state);
						}					 
					}
					else
					{
						for(int k=m_point.y-1;k >= j;k--)
						{
							PIECE_SIDE piece_side;
							PIECE_TYPE from_piece_type;
							COLOR_TYPE from_color_type;
							int from_pieceid;
							STATE from_piece_state; 
							cb[0][k].GetPieceData(from_pieceid,from_color_type,from_piece_type,from_piece_state);
							cb[0][k+1].SetPieceData(from_pieceid,from_color_type,from_piece_type,from_piece_state);
						}
					}
					cb[i][j].SetPieceData(piece_id,piece_color,piece_type,piece_state);
					cb[i][j].SetPieceState(PIECE_NOT_MOVING);
				}
				else if(m_point.y == 15 && m_player_turn == true)
				{			
					if(cb[0][13].GetPieceId() == -1)
					{
						for(int k=12;k >=j;k--)
						{
							PIECE_SIDE piece_side;
							PIECE_TYPE from_piece_type;
							COLOR_TYPE from_color_type;
							int from_pieceid;
							STATE from_piece_state; 
							cb[0][k].GetPieceData(from_pieceid,from_color_type,from_piece_type,from_piece_state);
							cb[0][k+1].SetPieceData(from_pieceid,from_color_type,from_piece_type,from_piece_state);
						}
						cb[i][j].SetPieceData(piece_id,piece_color,piece_type,piece_state);
						//cb[i][j].SetPieceData(-1,NONE,BLANK,PIECE_NOT_MOVING);
						cb[i][j].SetPieceState(PIECE_NOT_MOVING);
						cb[m_point.x][m_point.y].SetPieceData(-1,NONE,BLANK,PIECE_NOT_MOVING);						 
					}					
				}
				 
				m_SpecialAction = NOTHING;
			 
				m_point.x = m_point.y = -1;
				DrawBoard();	 			 
				return;
			}
		}
	}			 
	CRgn rgn;
	CRect rect = cb[0][15].GetRect();
	rgn.CreateEllipticRgn(rect.left, rect.top, rect.right, rect.bottom);
	if(rgn.PtInRegion(point) && cb[0][13].GetPieceId() != -1 && m_point.x >= 0)		 
	{
		int piece_id;
		COLOR_TYPE  piece_color;
		PIECE_TYPE  piece_type;
		STATE piece_state;
		int to_piece_id;
		COLOR_TYPE  to_piece_color;
		PIECE_TYPE  to_piece_type;
		STATE to_piece_state;
		cb[m_point.x][m_point.y].GetPieceData(piece_id,piece_color,piece_type,piece_state);
		//cb[i][j].GetPieceData(to_piece_id,to_piece_color,to_piece_type,to_piece_state);					 
		 
		for(int k=m_point.y+1;k <= 13;k++)
		{
				PIECE_SIDE piece_side;
				PIECE_TYPE from_piece_type;
				COLOR_TYPE from_color_type;
				int from_pieceid;
				STATE from_piece_state; 
				cb[0][k].GetPieceData(from_pieceid,from_color_type,from_piece_type,from_piece_state);
				cb[0][k-1].SetPieceData(from_pieceid,from_color_type,from_piece_type,from_piece_state);
		}
		cb[0][15].SetPieceData(piece_id,piece_color,piece_type,piece_state);
		cb[0][15].SetPieceState(PIECE_NOT_MOVING);
		cb[0][13].SetPieceData(-1,NONE,BLANK,PIECE_NOT_MOVING);
		char data[5];
		memset(data,-1,5);
		data[0] = MOVE;						 
		memcpy(&data[1],&piece_id,4);						 						 
		SendSockData((unsigned char*)data,5);						 
		m_player_turn = false;
		m_player_deck_open_flag = true;	
	}	 
	 
	m_point.x = m_point.y = -1;
	DrawBoard();	 
}

void CNetChessView::OnMouseMoveAction(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_mouseMoveFlag == false)
		return;	 	 
	if(m_point.y==14)
		return;
	CRect rect( point.x -40, point.y - 40, point.x + 25, point.y + 25);				  
	m_moveRect = rect;	 
	DrawBoard();	 
} 
 
void CNetChessView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_mouseMoveFlag == true || m_player_turn == false)
	{
		return;
	}
	bool flag = false;
	int foundi=0,foundj;
	for(int i = 0; i < 1; i++)
	{
		for( int j = 0; j < 14; j++)
		{
			CRgn rgn;
			CRect rect = cb[i][j].GetRect();
			rgn.CreateEllipticRgn(rect.left, rect.top, rect.right, rect.bottom);
			if(rgn.PtInRegion(point))			 
			{	
				flag = true;
				foundi=0;
				foundj=j;				 
			}
		}
	}
	if(flag == true)
	{
		CRgn rgn;
		CRect rect = cb[0][15].GetRect();
		rgn.CreateEllipticRgn(rect.left, rect.top, rect.right, rect.bottom);
		if(cb[0][13].GetPieceId() != -1&&m_player_turn == true)		 
		{
			int piece_id;
			COLOR_TYPE  piece_color;
			PIECE_TYPE  piece_type;
			STATE piece_state;
			int to_piece_id;
			COLOR_TYPE  to_piece_color;
			PIECE_TYPE  to_piece_type;
			STATE to_piece_state;
			cb[foundi][foundj].GetPieceData(piece_id,piece_color,piece_type,piece_state);
			//cb[i][j].GetPieceData(to_piece_id,to_piece_color,to_piece_type,to_piece_state);					 
			 
			for(int k=foundj+1;k <= 13;k++)
			{
					PIECE_SIDE piece_side;
					PIECE_TYPE from_piece_type;
					COLOR_TYPE from_color_type;
					int from_pieceid;
					STATE from_piece_state; 
					cb[0][k].GetPieceData(from_pieceid,from_color_type,from_piece_type,from_piece_state);
					cb[0][k-1].SetPieceData(from_pieceid,from_color_type,from_piece_type,from_piece_state);
			}
			cb[0][15].SetPieceData(piece_id,piece_color,piece_type,piece_state);
			cb[0][15].SetPieceState(PIECE_NOT_MOVING);
			cb[0][13].SetPieceData(-1,NONE,BLANK,PIECE_NOT_MOVING);
			char data[5];
			memset(data,-1,5);
			data[0] = MOVE;						 
			memcpy(&data[1],&piece_id,4);						 						 
			SendSockData((unsigned char*)data,5);						 
			m_player_turn = false;
			m_player_deck_open_flag = true;
		}
	}
	else
	{
		CRgn rgn;
		CRect rect = cb[0][15].GetRect();
		rgn.CreateEllipticRgn(rect.left, rect.top, rect.right, rect.bottom);
		if(rgn.PtInRegion(point) && cb[0][13].GetPieceId() == -1)		 
		{
			int piece_id;
			COLOR_TYPE  piece_color;
			PIECE_TYPE  piece_type;
			STATE piece_state;
			int to_piece_id;
			COLOR_TYPE  to_piece_color;
			PIECE_TYPE  to_piece_type;
			STATE to_piece_state;
			cb[0][15].GetPieceData(piece_id,piece_color,piece_type,piece_state);
			//cb[i][j].GetPieceData(to_piece_id,to_piece_color,to_piece_type,to_piece_state);					 	 
			char data[5];
			memset(data,-1,5);
			data[0] = MOVE;						 
			memcpy(&data[1],&piece_id,4);						 						 
			SendSockData((unsigned char*)data,5);						 
			m_player_turn = false;
			m_player_deck_open_flag = true;
		}
	}
 
	DrawBoard();
	CView::OnRButtonDown(nFlags, point);
}

void CNetChessView::OnUpdateToolsServer(CCmdUI* pCmdUI) 
{	 
	
}

void CNetChessView::OnUpdateToolsClient(CCmdUI* pCmdUI) 
{
	 
}

void CNetChessView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	DrawBoard();
	CView::OnPrint(pDC, pInfo);
}

void CNetChessView::OnTimer(UINT nIDEvent) 
{
	static int state=1;	
	if(m_player_turn == true)
		WriteStatus("Your turn to move");
	else
	{
		WriteStatus("Wait for yor turn");
	}
	switch(nIDEvent)
	{
		case SHELL_ICON_TIMER_EVENT_ID:
			{				
				NOTIFYICONDATA nicondata;
				char data[64] = "NetChess by A.V.Maheswara Rao";
				 
				nicondata.hWnd = AfxGetApp()->m_pMainWnd->GetSafeHwnd();
				nicondata.uID = 10;
				nicondata.uFlags = NIF_ICON |NIF_MESSAGE | NIF_TIP; 
				nicondata.cbSize = sizeof(nicondata);
				nicondata.uCallbackMessage = MY_MESSAGE_SHELLNOTIFY;
				nicondata.hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
				strcpy(nicondata.szTip,data);				
				if(state == 1)
				{
					nicondata.hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
					Shell_NotifyIcon(NIM_MODIFY,&nicondata);
					state = 0;
				}
				else
				{
					nicondata.hIcon = AfxGetApp()->LoadIcon(IDI_ICON_MAINFRAME_OPPOSITE);
					state = 1;
					Shell_NotifyIcon(NIM_MODIFY,&nicondata);
				}
			}
			break;
		case PIECE_SIDE_TIMER_EVENT_ID:
			{
				if(m_pClientSocket == NULL)
				{
					if(m_pieceSide == BLACK && m_player_turn == true)
					{	
						CTime tb(time(0)  -  m_whiteTime );
						CTime st(m_startTime);
						CTimeSpan ts = tb - st; 
						m_blackTime++;
						CString str;
						str.Format("%d:%d:%d",ts.GetHours(),ts.GetMinutes(),ts.GetSeconds());					 
						//str.Format("%d:%d:%d",t->tm_hour,t->tm_min,t->tm_sec);

					//(CMainFrame*)((CNetChessApp*)AfxGetApp())->m_pMainWnd)->m_wndStatusBar.SetPaneText(5,str);
					}
					else if(m_pieceSide == WHITE && m_player_turn == true)
					{					 
						CTime tb( time(0)- m_blackTime );
						CTime st(m_startTime);
						CTimeSpan ts = tb - st; 
						m_whiteTime++;
						CString str;
						str.Format("%d:%d:%d",ts.GetHours(),ts.GetMinutes(),ts.GetSeconds());					 
					//(CMainFrame*)((CNetChessApp*)AfxGetApp())->m_pMainWnd)->m_wndStatusBar.SetPaneText(3,str);
					}
				}
				else
				{
					if(m_pieceSide == BLACK && m_player_turn == true || (m_pieceSide == WHITE && m_player_turn == false))
					{	
						CTime tb(time(0)  -  m_whiteTime );
						CTime st(m_startTime);
						CTimeSpan ts = tb - st; 
						m_blackTime++;
						CString str;
						str.Format("%d:%d:%d",ts.GetHours(),ts.GetMinutes(),ts.GetSeconds());					 
						//str.Format("%d:%d:%d",t->tm_hour,t->tm_min,t->tm_sec);

//						((CMainFrame*)((CNetChessApp*)AfxGetApp())->m_pMainWnd)->m_wndStatusBar.SetPaneText(5,str);
					}
					else if(m_pieceSide == WHITE && m_player_turn == true || (m_pieceSide == BLACK && m_player_turn == false))
					{					 
						CTime tb( time(0)- m_blackTime );
						CTime st(m_startTime);
						CTimeSpan ts = tb - st; 
						m_whiteTime++;
						CString str;
						str.Format("%d:%d:%d",ts.GetHours(),ts.GetMinutes(),ts.GetSeconds());					 
//						((CMainFrame*)((CNetChessApp*)AfxGetApp())->m_pMainWnd)->m_wndStatusBar.SetPaneText(3,str);
					}
				}
				

			}
		default:
			break;
		 
	}
	CView::OnTimer(nIDEvent);
}
void CNetChessView::KillTimerEvent()
{
	 
	KillTimer(SHELL_ICON_TIMER_EVENT_ID);
	m_timerFlag = false;
}

void CNetChessView::OnFileNew() 
{
	m_blackTime = m_whiteTime = 0;
	m_startTime = time(0);
 
	OnFileNewAction();  
	unsigned char data=NEWGAME;
	SendSockData(&data,1);
}

void CNetChessView::OnFileNewAction()
{
	//if(m_iHistory > -1)
	{			
		Initialize();
		OnInitialUpdate();		
		DrawBoard();
	}		
}

void CNetChessView::OnViewHistory() 
{
	 if(m_iHistory < 0)
	 {
		 AfxMessageBox("History not found");
		 return;
	 }
	  CHistoryDlg dlg(this,m_History,m_iHistory);
	  dlg.DoModal();
}

 

 
void CNetChessView::OnHelpHowtoplay() 
{
	CHelpDlg dlg;
	dlg.DoModal(); 
}

void CNetChessView::OnToolsDisconnect() 
{
	// TODO: Add your command handler code here
	 
	if(m_pClientSocket == NULL)
	{
		AfxMessageBox("Not connected to network");
		if(m_pServerSocket != NULL)
		{
			if(AfxMessageBox("Are you sure, you want to disconnect",MB_YESNO)==IDYES)
			{
				delete m_pServerSocket;
				m_pServerSocket = NULL;
			}
		}
		return;
	}
	if(m_pClientSocket != NULL)
	{ 
		if(AfxMessageBox("Are you sure, you want to disconnect",MB_YESNO)==IDYES)
		{
			m_pClientSocket->ShutDown(2);
			m_pClientSocket->Close();
			delete m_pClientSocket;
			m_pClientSocket = NULL; 
		}
	}
}

void CNetChessView::OnEditProperties() 
{
	// TODO: Add your command handler code here
	CPropertiesDlg dlg(this);
	if(m_pServerSocket != NULL)
	{
		if(m_pClientSocket != NULL)
		{
			CString ipaddr;int port;
			((CClientSocket*)m_pClientSocket)->GetInfo(ipaddr,port);
			dlg.SetInfo(SERVER,ipaddr,port,((CServerSocket*)m_pServerSocket)->m_portnumber);
		}
		else
		{
			dlg.SetInfo(SERVER," ",0,((CServerSocket*)m_pServerSocket)->m_portnumber);			 
		}
	}
	else if(m_pClientSocket != NULL)
	{
		CString ipaddr;int port;
		((CClientSocket*)m_pClientSocket)->GetInfo(ipaddr,port);
		dlg.SetInfo(CLIENT,ipaddr,port,0);
	}
	else
	{
		dlg.SetInfo(-1,"",0,0);
	}
	dlg.DoModal();	
}


 
void CNetChessView::OnToolsResign() 
{
	// TODO: Add your command handler code here
	char comp[50];
	memset(comp,'\0',50);
	gethostname(comp,50);
	unsigned char data[50];
	data[0]=RESIGN_REQUEST;
	int count=80;
	memcpy(&data[1],&count,4);
	strcpy((char*)&data[5],comp);
	SendSockData(data,strlen(comp)+2);
	if(m_pServerSocket != NULL)
	{
		m_iHistory++;
		m_History[m_iHistory].SetHistory(count,0);
	}
	else
	{
		m_iHistory++;
		m_History[m_iHistory].SetHistory(0,count);
	}
	CheckWin();
	//OnFileNew();
}
void CNetChessView::SetPieceSide(COLOR_TYPE pieceside)
{
	//m_pieceSide = pieceside;	 
	if(pieceside == BLACK )
	{
		if(m_pServerSocket !=  NULL)
		{
			m_player_turn = false;
		}
		else
		{
			m_player_turn = true;
		}
	}
	else
	{
		if(m_pServerSocket !=  NULL)
		{
			m_player_turn = true;
		}
		else
		{
			m_player_turn = false;
		}
	}
	/*if(m_pieceSide == WHITE && m_pServerSocket != NULL)
	{
		m_player_turn = true;
	 
		//CMainFrame*)((CNetChessApp*)AfxGetApp())->m_pMainWnd)->m_wndStatusBar.SetPaneText(1,m_serverName);				 
	}
	else if(m_pieceSide == BLACK && m_pServerSocket == NULL)
	{
		m_player_turn = true;
	 
//		((CMainFrame*)((CNetChessApp*)AfxGetApp())->m_pMainWnd)->m_wndStatusBar.SetPaneText(1,m_clientName);				 
	}	 
	else
	{
			m_player_turn = false;
	}*/
	m_blackTime = m_whiteTime = 0;
	m_startTime = time(0);
	DrawBoard();
}

COLOR_TYPE CNetChessView::GetPlayerSide()
{
	return m_pieceSide;
}

 
 
void writeMessage(char *str,...)
{
	char arr[9999];
	va_list va;     
     va_start(va,str);
     vsprintf(arr,str,va);       

	 FILE* fp=fopen(".\\data.dat","a+");
	//fprintf(fp,"%s\n",arr);
	 fclose(fp);
}


void CNetChessView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect = cb[0][14].GetRect();
	CRgn rgn1;
	rgn1.CreateEllipticRgn(rect.left, rect.top, rect.right, rect.bottom);
	if(rgn1.PtInRegion(point)&&m_player_turn == true&&m_player_deck_open_flag == true)		 
	{
		int piece_id;
		COLOR_TYPE  piece_color;
		PIECE_TYPE  piece_type;
		STATE piece_state;
		int to_piece_id;
		COLOR_TYPE  to_piece_color;
		PIECE_TYPE  to_piece_type;
		STATE to_piece_state;		 
		 
		deck[m_deckIndex--].GetPieceData(piece_id,piece_color,piece_type,piece_state);		
		discard[m_discardIndex++].SetPieceData(piece_id,piece_color,piece_type,piece_state);		 
				
		cb[0][15].SetPieceData(piece_id,piece_color,piece_type,piece_state);
		cb[0][15].SetPieceState(PIECE_NOT_MOVING);
		m_player_deck_open_flag = false;
		unsigned char data[1];
		data[0]=DECK_MOVE;		 
		SendSockData(data,1);		 	 

		if(m_deckIndex < 0)
		{
			int piece_id;
			COLOR_TYPE  piece_color;
			PIECE_TYPE  piece_type;
			STATE piece_state;
			int to_piece_id;
			COLOR_TYPE  to_piece_color;
			PIECE_TYPE  to_piece_type;
			STATE to_piece_state;		 
			int arr[100];
			writeMessage("discard index is discard index %d m_deckIndex %d",m_discardIndex,m_deckIndex);
			int tempIndex = m_discardIndex;
			getRandom(tempIndex,arr);
			m_deckIndex = m_discardIndex-1;
			writeMessage("descard index is discard index %d m_deckIndex %d",m_discardIndex,m_deckIndex);
			for(int k=0;k<m_discardIndex;k++)
			{			
				writeMessage("shuffled data is %d data is %d",arr[k],discard[k].GetPieceId());
			}
			for(int i=0;i<m_discardIndex;i++)
			{
				discard[arr[i]-1].GetPieceData(piece_id,piece_color,piece_type,piece_state);
				deck[i].SetPieceData(piece_id,piece_color,piece_type,piece_state);						
			}
			m_discardIndex=0;
			unsigned char data[52];
			memset(data,'0',52);
			int count=0;
			data[count++]=DECK_RESHUFFLE;
			for(i=0;i<=m_deckIndex;i++)
			{
				data[count++] = deck[i].GetPieceId() % 200;
				writeMessage("reshuffled data is %d",data[count-1]);
			}
			SendSockData(data,count);
		}
	}	
	DrawBoard();
	CView::OnLButtonDblClk(nFlags, point);
}


void CNetChessView::OnToolsShow() 
{
	// TODO: Add your command handler code here
	if(AfxMessageBox("Are you sure,  you want to send Show",MB_YESNO) == IDYES)
	{
		char data[14];
		data[0]=SHOW;
		for(int i=0;i<13;i++)
		{
			data[i+1] = cb[0][i].GetPieceId() % 200;
		}
		SendSockData((unsigned char*)data,14);		 	 
	}
}
void CNetChessView::WriteStatus(CString str)
{
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->WriteStatus(str);
}



