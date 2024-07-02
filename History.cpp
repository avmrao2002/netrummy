#include "stdafx.h"
#include "History.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CHistory::SetHistory( int serverScore,int clientScore  )

{
	m_serverScore = serverScore;
	m_clientScore = clientScore;	 
}

void CHistory::GetHistory(
		int &serverScore,int &clientScore  )
{
	serverScore = m_serverScore;
	clientScore = m_clientScore;	 
}