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
	vector< CoverStringstruct >   m_CStringitemField_Cover;     //���a�ۤv�]�w�n�B�����r��
	vector< CoverStringstruct >   m_CStringitemField_Show;		//���a�ۤv�]�w�n��ܪ��r��
	vector< CoverStringstruct >	  m_CStringitemField_Overlap; 	//���a�]�w���r��P���ئr��ۦP�� ���s�b�o
	CoverStringstruct			  m_Overlap;
};