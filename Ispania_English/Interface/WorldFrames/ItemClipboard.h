#pragma once

#include <string>
#include "../interface.h"

class CItemClipboard;
extern CItemClipboard*			g_pItemClipboard;

using namespace std;

enum ClipboardItemType {
	eClipboardItem_Type_None			= 0,
	eClipboardItem_Type_Bag,					// �I�]
	eClipboardItem_Type_SplitBagItem,			// �]�]����
	eClipboardItem_Type_Bank,					// �Ȧ�
	eClipboardItem_Type_Equipment,				// �˳�
	eClipboardItem_Type_Trade,					// ���
	eClipboardItem_Type_Action,					// �ֱ�
	eClipboardItem_Type_Skill,					// �ޯ�
	eClipboardItem_Type_Shop,					// �ӫ~
	eClipboardItem_Type_Macro,					// ����
	eClipboardItem_Type_Emote,					// �����ʧ@
	eClipboardItem_Type_Merchant,				// �I�ưӫ�
	eClipboardItem_Type_House,					// �ЫΨt�� �a��C����_�c
	eClipboardItem_Type_Teleport,				// �ǰe�� �ǰe�I
	eClipboardItem_Type_GuildBank,				// ���|�Ȧ�
	eClipboardItem_Type_GuildFurniture,			// ���|�ήa��
	eClipboardItem_Type_PetEQ,					// �d���˳�
	eClipboardItem_Type_SuitSkill,				// �M�˧ޯ�
	eClipboardItem_Type_SuitSkillequip,			// �ޯ���
	eClipboardItem_Type_SkillPlate,				// �ޯ�L
	eClipboardItem_Type_CoolClothBag,			// �Ÿ˥]
 	eClipboardItem_Type_CoolSuit,			    // �ŮM��
	eClipboardItem_Type_HappyPartner,           //�ּ֪��٦��?
};

typedef union 
{	 
	struct {
		int	itemID;							// ���~�s��
		int	pos;							// �I�]���޽s��
		int count;							// ���~�Ӽ�(-1)�N�����
	}bag;

	struct {
		int	itemID;							// ���~�s��
		int	pos;							// �I�]���޽s��
		int count;							// ���~�Ӽ�(-1)�N�����
	}splitBagItem;

	struct {
		int	itemID;
		int	pos;
	}bank;	

	struct {
		int	itemID;
		int	pos;
	}equipment;

	struct {
		int	type;
		int	id;
		int arg[2];						// ���~�ߤ@�s��(�إ߮ɶ�,�P�ǦC��)
	}action;

	struct {
		int	type;
		int pos;
		int id;
	}trade;

	struct {
		int	skillID;
	}skill;

	struct {
		int type;
		int pos;
	}shop;

	struct {
		int id;
	}macro;

	struct {
		int id;
	}emote;

	struct {
		int	itemID;
		int parentID;
		int	pos;
	}house;

	struct {	
		int	pos;
	}teleport;

	struct {
		int	itemID;
		int	pos;
	}guildbank;

	struct {
		int	itemID;
		int	pos;
	}guildfurniture;

	struct {
		int index;						// �d���s��
		int pos;						// �˳ƥN�X
	}petEQ;

	struct {
		int skillID;					// �ޯ�s�X
	}SuitSkill;

	struct {
		int itemID;					// �ޯ���
		int pos;
	}SuitSkillequip;

	struct {
		int skillID;				// �ޯ�L
		int pos;
	}SkillPlate;

	struct {						 // �Ÿ˥]
		int index;
	}CoolClothBag;

	struct {						 // �ŮM��
		int index;
	}CoolSuit;

	struct  //�٦�
	{	
		int mode;
		int	pos;
	}partner;


}ClipboardItemInfo;

class CItemClipboard : public CInterfaceBase
{
public:
	CItemClipboard(CInterface* object);
	virtual ~CItemClipboard();

	virtual	void					RegisterFunc		();
	virtual	void					Update				(float elapsedTime);
	virtual	void					Clear				();
	virtual void					Unlock				();

	virtual	void					SetName				(const char* name);
	virtual	void					SetFile				(const char* file);
	virtual void					SetFileFromImageID	(int id);
	virtual	void					PickupBag			(int itemID, int pos);
	virtual void					PickupSplitBagItem  (int itemID, int pos, int count);
	virtual	void					PickupBank			(int itemID, int pos);
	virtual	void					PickupEquipment		(int itemID, int pos);
	virtual	void					PickupAction		(int actionType, int id, int arg1=0, int arg2=0);
	virtual	void					PickupTrade			(int tradeType, int pos, int id);
	virtual void					PickupSkill			(int skillID );
	virtual	void					PickupShop			(int shopType, int pos);
	virtual	void					PickupMacro			(int id);
	virtual	void					PickupEmote			(int id);
 	virtual	void					PickupHouse			(int itemID, int parentID, int pos);
	void							PickupTeleport		(int index);
	void							PickupPartner		(int mode,int index);
	virtual	void					PickupGuildBank		(int itemID, int pos);
	virtual	void					PickupGuildFurniture(int itemID, int pos);
	virtual void					PickupPetEQ			(int index, int pos);
	virtual void					PickupSuitSkill		(int skillID );
	virtual void					PickupSuitSkillequip(int itemID, int pos);
	virtual void					PickupSkillPlate	(int skillID, int pos);
 	virtual void					PickupCoolClothBag	(int index);
	virtual void					PickupCoolSuit		(int index);


	virtual	ClipboardItemType		GetType				()						{ return m_type; }
	virtual ClipboardItemInfo		GetItemInfo			()						{ return m_itemInfo; }
	virtual	const char*				GetName				()						{ return m_name.c_str(); }
	virtual	const char*				GetFile				()						{ return m_file.c_str(); }
	virtual	bool					IsEmpty				()						{ return m_type == eClipboardItem_Type_None; }

	static const char*				GetItemTypeString	(ClipboardItemType itemType);

protected:
	virtual void					UpdateItemInfo();

protected:
	ClipboardItemType			m_type;							// �ӷ���������
	ClipboardItemInfo			m_itemInfo;						// ���~���
	string						m_file;							// Icon �ϧθ��
	string						m_name;							// �W��
};