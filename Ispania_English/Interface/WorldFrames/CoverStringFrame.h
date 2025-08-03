#pragma once
#include "../interface.h"
#include <string>
#include <map>
#include <vector>

using namespace std;
//******************************************************************************
class CCoverStringFrame;
extern CCoverStringFrame*	g_pCoverStringFrame;

struct CoverStringstruct
{
	string cstring;
};

struct CoverStringstructSave
{
	char buf[128];
};





class CCoverStringFrame : public CInterfaceBase
{
public:
	CCoverStringFrame( CInterface* pObject );
	virtual				~CCoverStringFrame();

	virtual	void				RegisterFunc();	
	virtual int					Check_m_CStringitemField( string CString , int type);
	virtual	void				Add_m_CStringitemField(  string CString , int type);
	virtual	int					Remove_m_CStringitemField(string CString , int type);
	virtual	void				MatchOverlap();
	virtual	bool				MatchCoverStringFromOverlap( BYTE* text , int index );
	virtual	void				Save();	
	virtual	void				Load();

//protected:
	vector< CoverStringstruct >   m_CStringitemField_Cover;     //玩家自己設定要遮蔽的字串
	vector< CoverStringstruct >   m_CStringitemField_Show;		//玩家自己設定要顯示的字串
	vector< CoverStringstruct >	  m_CStringitemField_Overlap; 	//玩家設定的字串與內建字串相同時 先存在這
	CoverStringstruct			  m_Overlap;
};