#ifndef _MAINMENU_H
#define _MAINMENU_H

#include <string>

// **********************************************************************************************
// Globals
class	CMainMenu;

extern	CMainMenu							*g_pMainMenu;

// **********************************************************************************************
enum DragDropTypeEnum
{
    EM_DragDropType_Null		= -1,
    EM_DragDropType_Body        = 0 ,
    EM_DragDropType_Bank            ,
    EM_DragDropType_EQ              ,
    EM_DragDropType_Shop            ,
    EM_DragDropType_Tran            ,
    EM_DragDropType_ShortCut        ,
};

#define _Def_DragDrop_Flag_Item_        0x001
#define _Def_DragDrop_Flag_ShortCut_    0x002


struct ItemClipboard
{
	std::string			    name;
	std::string			    file;	
	DragDropTypeEnum		type;
	int					    flag;
    int                     itemID;
    int                     version;        //物品板本，確定物品
    //物品類
	int                     pos;            //位置
    //Macro所需資訊
    std::string             macro;
    DragDropTypeEnum		orgType;        //元始來原

    void    SetItem_Body( std::string Name , std::string ImgFile , int ItemID , int Pos , int Version );
    void    SetItem_Bank( std::string Name , std::string ImgFile , int ItemID, int Pos , int Version );
    void    SetItem_EQ  ( std::string Name , std::string ImgFile , int ItemID, int Pos , int Version );
    void    SetShortCut ( std::string Name , std::string ImgFile  , std::string Macro , int ItemID, int Pos , int Version , DragDropTypeEnum OrgType );
    void    SetMacro    ( std::string Name , std::string ImgFile  , std::string Macro );
};

class CMainMenu
{
public:
	CMainMenu();
	virtual ~CMainMenu();

	static	void		Initial();
	static	void		Release();

	//void				Update( float elapsedTime );
	ItemClipboard*		PasteItemClipboard();
	void				CopyItemClipboard( ItemClipboard& itemData );
	void				ClearItemClipboard();
protected:
	ItemClipboard		m_itemBuf;
	bool				m_haveItemBuf;
};

#endif //_MAINMENU_H
