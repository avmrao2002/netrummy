#if !defined(HISTORY_H)
#define HISTORY_H

class CHistory
{
private:
	int m_serverScore;
	int m_clientScore;
	  
	  
public:
	void SetHistory(int serverScore, int clientScore );
	void GetHistory(int &serverScore, int &clientScore );

};
#endif