// ServerSocket.cpp : implementation file
//

#include "stdafx.h"
#include "NetChess.h"
#include "NetChessDoc.h"
#include "NetChessView.h"
#include "ClientSocket.h"
#include "ServerSocket.h"
#include "AcceptDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerSocket

CServerSocket::CServerSocket()
{
}

CServerSocket::~CServerSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CServerSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CServerSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CServerSocket member functions

void CServerSocket::OnAccept(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	 
	CClientSocket *ClientSocket = new CClientSocket();
	Accept(*ClientSocket);
	CString name; unsigned int port;
 
	if(((CNetChessView*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetActiveView())->GetClientSocket() != NULL)
	{
		unsigned char data= NOTACCEPTED;
		char clength[5];
		int length = 1;
		memcpy(clength,&length,4);
		ClientSocket->Send(clength,4);	 
		ClientSocket->Send(&data,length);	 			 
		ClientSocket->ShutDown(2);
		ClientSocket->Close();
		delete ClientSocket;
		ClientSocket = NULL;
		return;
	}
	ClientSocket->GetPeerName(name,port);
	ClientSocket->m_ipaddress = name;
	ClientSocket->m_port = port;
	CString msg;
	msg.Format(" requested for playing Rummy");
	msg = name + msg;
	CAcceptDlg dlg;
	dlg.m_AcceptString = msg;
	if(dlg.DoModal() == IDOK)
	{			
		((CNetChessView*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetActiveView())->SetClientSocket(ClientSocket);
		ClientSocket->AsyncSelect(FD_READ | FD_CLOSE);		 
		char comp[50];
		memset(comp,'\0',50);
		gethostname(comp,50);
		unsigned char data[50];
		memset(data,'\0',50);
		data[0] = ACCEPTED;
		data[1] = dlg.m_pieceSide; 
		strcpy((char*)&data[2],comp);	 	 
		char clength[6];
		int length = strlen(comp)+3;
		memcpy(clength,&length,4);
		ClientSocket->Send(clength,4);	 
		ClientSocket->Send(&data,length);	 			 
		 
		((CNetChessView*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetActiveView())->FillTheCards();
		((CNetChessView*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetActiveView())->m_serverName = comp;		 
		((CNetChessView*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetActiveView())->m_clientName = name;		 
		((CNetChessView*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetActiveView())->SetPieceSide(dlg.m_pieceSide);
		if(dlg.m_radio_number == true)
			((CNetChessView*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetActiveView())->m_game_type=NUMBER_TYPE;
		else
			((CNetChessView*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetActiveView())->m_game_type=FIXED_TYPE;
		((CNetChessView*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetActiveView())->m_game_total=dlg.m_edit_value;
		((CNetChessView*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetActiveView())->m_game_count=1;
	}
	else
	{
		unsigned char data= NOTACCEPTED;
		char clength[5];
		int length = 1;
		memcpy(clength,&length,4);
		ClientSocket->Send(clength,4);	 
		ClientSocket->Send(&data,length);	 			 
		ClientSocket->ShutDown(2);
		ClientSocket->Close();
		delete ClientSocket;
		ClientSocket = NULL;
		return;
	}
	
	CAsyncSocket::OnAccept(nErrorCode);
}

void CServerSocket::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	AfxMessageBox("Connection closed");	
	CAsyncSocket::OnClose(nErrorCode);
}

void CServerSocket::OnConnect(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CAsyncSocket::OnConnect(nErrorCode);
}

void CServerSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CAsyncSocket::OnReceive(nErrorCode);
}

void CServerSocket::OnSend(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CAsyncSocket::OnSend(nErrorCode);
}

int CServerSocket::Receive(void* lpBuf, int nBufLen, int nFlags) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CAsyncSocket::Receive(lpBuf, nBufLen, nFlags);
}

int CServerSocket::Send(const void* lpBuf, int nBufLen, int nFlags) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CAsyncSocket::Send(lpBuf, nBufLen, nFlags);
}
void CServerSocket::CreateServer(int Port)
{	 	 
	if(	this->Create(Port) == 0)
	{
		AfxMessageBox("Chess is already running");
		exit(0);
	}
	Listen(); 
	AsyncSelect(FD_ACCEPT | FD_CLOSE);	 
}
void CServerSocket::SetInfo(int portnumber)
{
	m_portnumber = portnumber;
}