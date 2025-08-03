#pragma once

#include "../interface.h"

class CBillboardFrame;
extern CBillboardFrame*			g_pBillboardFrame;

struct BillboardBrowseFilterStruct
{
	int		ID;
	string	Name;

	vector< BillboardBrowseFilterStruct > Childs;
};

class CBillboardFrame : public CInterfaceBase
{
	void	_ReleaseFiler( );
	void	_InitFiler( );

public:
	CBillboardFrame(CInterface* object);
	virtual ~CBillboardFrame();
	
	virtual	void				RegisterFunc();
//	virtual	bool				IsVisible() { return (_UI_Win !=NULL && _UI_Win->IsVisible() ); } ;
	virtual	void				OnLoad();

	virtual	void				Open( int NpcID );
	virtual	void				Close();
	virtual void				Update(float elapsedTime);
	virtual void				UpdateData( );

public:
	int	m_NpcID;
	vector< BillboardBrowseFilterStruct >	m_BrowseFilter;
	


};