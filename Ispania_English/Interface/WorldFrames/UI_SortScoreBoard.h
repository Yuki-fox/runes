

#pragma once 
#include "../interface.h"

struct NewBillBoardRoleInfoStruct;
class CUISortScoreBoard  : public CInterfaceBase
{
public:
	CUISortScoreBoard(CInterface* object);
	virtual ~CUISortScoreBoard();

	void RegisterFunc(void);
	void GetScoreItemBoard(int sortType);
	void SetMyNote(int sortType,const char *name);
		void				BuildFrames()	;//						{}				// UI ROOT 已建立,但未載入XML之前
	 void				DestroyFrames()		;//					{}				// UI ROOT 將被刪除

	 void SearchRankResult	( int SortType , int TotalCount , int ID , NewBillBoardRoleInfoStruct& Info ) ;
	 void SearchMyRankResult	( int SortType , int SortValue , int MyRank , int MyLastRank , const char* Note ) ;

};

extern CUISortScoreBoard * g_pSortScoreBoard;

