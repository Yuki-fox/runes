#pragma	   once

#include "../GameMain.h"
	

// **********************************************************************************************
// Globals
class	CAccountLogin;

extern	CAccountLogin					*g_pAccountLogin;

// **********************************************************************************************

// functions
void	RegisterAccountLoginFunc		();

class CAccountLogin
{
public:
	CAccountLogin();
	~CAccountLogin();

	static void				Initial				();
	static void				Release				();

	void					SetIP				( const char *ip );
	char*					GetIP				()						{ return m_ip; }
	void					SetPort				( int port )			{ m_port = port; }
	int						GetPort				()						{ return m_port; }

protected:
	char					m_ip[24];			// ip address
	int						m_port;				// ip port
};
	

