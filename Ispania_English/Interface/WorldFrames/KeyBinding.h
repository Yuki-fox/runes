#pragma once

#include "../interface.h"
#include <vector>
#include <map>
#include <string>

class CKeyBinding;
extern CKeyBinding*			g_pKeyBinding;

using namespace std;

#define BINDINGS_KEY_TEXT_SHIFT				"SHIFT"
#define BINDINGS_KEY_TEXT_CTRL				"CTRL"
#define BINDINGS_KEY_TEXT_ALT				"ALT"

enum eKeyBindingFlag
{
	KEY_BINDING_FLAG_KEYBOARD				= 0,
	KEY_BINDING_FLAG_MOUSE					= 0x01000000,

	KEY_BINDING_FLAG_SHIFT					= 0x010000,
	KEY_BINDING_FLAG_CTRL					= 0x020000,
	KEY_BINDING_FLAG_ALT					= 0x040000,
};

struct BindingItem
{
	BindingItem()
	{
		name.clear();
		script.clear();
		header.clear();
		runOnUp = false;
		memset(hotkey, 0, sizeof(hotkey));
	}

	void ClearHotkey(int key)
	{
		if ( hotkey[0] == key )
			hotkey[0] = 0;
		if ( hotkey[1] == key )
			hotkey[1] = 0;
	}
	
	//char*		name;					// ����S���W��
	//char*		script;					// script ���e

	string			name;					// ����S���W��
	string			header;					// header
	string			script;					// script ���e
	int				hotkey[2];				// ������s
	bool			runOnUp;				// �O�_����key up, �Yfalse�h�u����key down�ƥ�
};

class CKeyBinding : public CInterfaceBase
{
public:
	CKeyBinding(CInterface* object);
	virtual ~CKeyBinding();

	virtual	void					RegisterFunc();	
	virtual void					SaveClientData();
	virtual void					LoadClientData(int type, int size, char* data);
	virtual	void					Save();
	virtual	void					Load();
	virtual	void					Clear();

	virtual	void					DefaultBindingKey();
	virtual	BindingItem*			AddBinding(const char* name, const char* script, bool runOnUp, const char* header=NULL);
	virtual BindingItem*			FindBinding(const char* name);
	virtual	BindingItem*			FindBinding(int key);
	virtual	BindingItem*			GetBindingByIndex(int index);

	virtual	void					SetBindingKey(const char* name, int key1, int key2=0);
	virtual	void					RunBinding(int key, bool keyUp);
	virtual	void					RunBinding(BindingItem* binding);
	virtual	int						GetNumBindings()							{ return (int)m_bindings.size(); }

	virtual	const char*				GetBindingKeyText(int key);
	virtual	int						ChangeBindingKey(const char* keyText);	

protected:
	virtual	int						BindingKeyNormalize(int key);
	virtual	void					ReadLine(const char* lineStr);

protected:
	map<int, BindingItem*>			m_bindingKey;		// �O������P��M���檺���e,��MAP��M�i�H��K
														// �ֳt��X�O�_���ۦP����.
	map<string, BindingItem*>		m_bindings;			// �O���Ҧ��ƥ�W�ٻP��M����Script

	vector<BindingItem*>			m_order;			// �[�J����
};
