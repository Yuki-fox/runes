#pragma once

#include <string>
#include <vector>
#include <list>
#include <map>
#include "../interface.h"

#define NUM_MAX_MACROS				56
#define NUM_NAME_MACRO				64
#define NUM_CONTENT_MACRO			256
#define MACRO_MAX_NAME				256
#define MACRO_MAX_CONTENT			1024

class CMacroFrame;
extern CMacroFrame*				g_pMacroFrame;

using namespace std;

struct MacroItem2
{	
	MacroItem2()
	{
		id = 0;
		icon = -1;
		memset(name, 0, sizeof(name));
		memset(text, 0, sizeof(text));
	}

	bool IsEmpty()
	{
		return icon == -1;
	}

	int		id;							// 唯一編號
	int		icon;						// 巨集圖示(-1空白)
	char	name[NUM_NAME_MACRO];		// 巨集名稱
	char	text[NUM_CONTENT_MACRO];	// 巨集內容	
};

struct MacroItem
{	
	MacroItem()
	{
		id = 0;
		icon = -1;
		memset(name, 0, sizeof(name));
		memset(text, 0, sizeof(text));
	}

	bool IsEmpty()
	{
		return icon == -1;
	}

	int			id;							// 唯一編號
	int			icon;						// 巨集圖示(-1空白)
	char		name[MACRO_MAX_NAME];		// 巨集名稱	
	char		text[MACRO_MAX_CONTENT];	// 巨集名稱
};

struct ExecuteMacroLine
{
	float				sleep;			// 停止時間
	list<string>		lines;			// 每一行內容資料
};

class CMacroFrame : public CInterfaceBase
{
public:
	CMacroFrame(CInterface* object);
	virtual ~CMacroFrame();

	void				Update(float elapsedTime);
	void				SaveClientData();
	void				LoadClientData(int type, int size, char* data);
	void				RegisterFunc();
	void				DestroyFrames();
	int					GetMacroIndexById(int id);
	bool				CreateMacro(const char* name, int iconId, const char* text);
	void				EditMacro(int index, const char* name, int iconId, const char* text);
	void				ClearMacro(int index);
	MacroItem*			GetMacroItem(int index);
	MacroItem*			GetMacroItemById(int id)		{ return GetMacroItem(GetMacroIndexById(id)); }
	const char*			GetMacroIcon(int index);
	int					GetNumMacroIcons();
	void				PickupMacroItem(int serial);
	void				UseMacro(int serial);
	void				Save();
	void				Load();
	void				SearchIcons();	
	void				RemoveExecuteLine(int id);

protected:
	bool						m_isUpdate;
	MacroItem					m_macro[NUM_MAX_MACROS];
	vector<string>				m_icons;
	map<int, ExecuteMacroLine>	m_executeLines;
};