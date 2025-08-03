#pragma once


#include <string>
using namespace std;

enum EMapIconType
{
	EM_MapIconType_None							= 0 ,
	EM_MapIconType_NewQuest						= 1 ,   //	�s����
	EM_MapIconType_UnfinishQuest				= 2 ,   //	����������
	EM_MapIconType_FinishedQuest				= 3 ,   //	��������
	EM_MapIconType_TrustQuest					= 4 ,   // �e�U����
	EM_MapIconType_Bank							= 5 ,   // �Ȧ�
	EM_MapIconType_MailBox						= 6 ,   // �l��
	EM_MapIconType_FlightPoint					= 7 ,   // �����I
	EM_MapIconType_AuctionHouse					= 8 ,   // ����
 	EM_MapIconType_Mine							= 9 ,   // �Ķ��I-�q
	EM_MapIconType_Wood							= 10,   // �Ķ��I-��
	EM_MapIconType_Herb							= 11,   // �Ķ��I-�į�
  	EM_MapIconType_Inn							= 12,   // �ȩ�
	EM_MapIconType_GuildMan						= 13,   // ���|�إߪ�
	EM_MapIconType_HouseMan						= 14,   // �Ыκ޲z��
	EM_MapIconType_Weapon						= 15,   // �Z����
	EM_MapIconType_Armor						= 16,   // �����
	EM_MapIconType_Potion						= 17,   // �Ĥ���
	EM_MapIconType_Food							= 18,   // ������
	EM_MapIconType_Magic						= 19,   // �]�k�ө�
	EM_MapIconType_Mat							= 20,   // ���Ʀ�
	EM_MapIconType_Shop							= 21,   // ���f��
	EM_MapIconType_Fish							= 22,   // ���㩱
	EM_MapIconType_Recipe						= 23,   // �t��
  	EM_MapIconType_Warrior						= 24,
	EM_MapIconType_Ranger						= 25,
	EM_MapIconType_Rogue						= 26,
	EM_MapIconType_Wizard						= 27,
	EM_MapIconType_Priest						= 28,
	EM_MapIconType_Knight						= 29,
	EM_MapIconType_RuneDancer					= 30,
	EM_MapIconType_Druid						= 31,
	EM_MapIconType_Harpsyn            			= 32,
	EM_MapIconType_Psyron           			= 33,
	EM_MapIconType_Duelist            			= 34,  // ¾�~�O�d
	EM_MapIconType_temp4            			= 35,  // ¾�~�O�d
	EM_MapIconType_temp5            			= 36,  // ¾�~�O�d
	EM_MapIconType_temp6            			= 37,  // ¾�~�O�d
	EM_MapIconType_temp7            			= 38,  // ¾�~�O�d
	EM_MapIconType_temp8            			= 39,  // ¾�~�O�d
	EM_MapIconType_temp9            			= 40,  // ¾�~�O�d
	EM_MapIconType_temp10           			= 41,  // ¾�~�O�d
  	EM_MapIconType_Flag							= 42,   
	EM_MapIconType_NPC							= 43,
	EM_MapIconType_Player						= 44, 
	EM_MapIconType_TrackArrow					= 45,
 	EM_MapIconType_LotteryMan					= 46,  // �m��
	EM_MapIconType_VocationMan					= 47,  // ¾�~�޲z��
 	EM_MapIconType_36							= 48,	// //���ϥ� ���w�U���X�����
	EM_MapIconType_QuestNPC						= 49,	// ���ȩһݭn�������ؼ�
	EM_MapIconType_QuestOBJ						= 50,	// ���ȩһݭn�I�����ؼ�
	EM_MapIconType_TrackNpcArrow				= 51,	// �l��NPC�b�Y
 	EM_MapIconType_TrackNpc						= 52,	// �l��NPC
 	EM_MapIconType_RepeatQuest					= 53,	// �i���Ы����ȥi��
	EM_MapIconType_RepeatQuestDone				= 54,	// �i���Ы����ȧ���
	EM_MapIconType_GrayQuest					= 55,	// �i�����|����F���ű��󪺥���
  	EM_MapIconType_Speak						= 56,	// ����
	EM_MapIconType_GrayTower					= 57,	// �� (�Ǧ�) 
	EM_MapIconType_RedTower						= 58,	// �� (����)
	EM_MapIconType_BlueTower					= 59,	// �� (�Ŧ�)
	EM_MapIconType_Tomb							= 60,	// �ӸO
	EM_MapIconType_BGTeam1						= 61,   // �Գ�����аO1
	EM_MapIconType_BGTeam2						= 62,   // �Գ�����аO2
	EM_MapIconType_BGTeam3						= 63,   // �Գ�����аO3
	EM_MapIconType_BGTeam4						= 64,   // �Գ�����аO4
	EM_MapIconType_BGTeam5						= 65,   // �Գ�����аO5
	EM_MapIconType_BGTeam6						= 66,   // �Գ�����аO6
	EM_MapIconType_BGTeam7						= 67,   // �Գ�����аO7
	EM_MapIconType_BGTeam8						= 68,   // �Գ�����аO8
	EM_MapIconType_Fighting						= 69,   // �԰���
	EM_MapIconType_PublicQuest					= 70,   // PE QUEST
	EM_MapIconType_PublicQuestDone				= 71,   // PE QUEST
	EM_MapIconType_END							= 72
};

//
extern const char* g_szIconTextureName;

//�y��
struct SWorldCoordinate
{
	string wdbFile;
	float PosX;
	float PosZ;
};

struct SMapCoordinate
{
	int MapID;
	float PosX; //�ʤ��� 0~1
	float PosZ; //�ʤ��� 0~1
};

struct SMapTrackInfo
{
	SMapTrackInfo()
	{
		show = false;
		size = 16;
		id = -1;
		trackArrow = false;
	}
	bool show;
	string strName;
	SWorldCoordinate coord;
	int icontype;
	float r;
	float g;
	float b;
	float size;
	int id;	//��id!=-1 �| gotoid
	bool trackArrow; //��W�X�p�a�Ͻd��ɡA���S���l�ܽb�Y
};

//
bool Func_GetMapIconTextureInfo( EMapIconType eID  , CRuRectangle& rect );

//EX: Wailing Mountains -> �z�s�s�S
const char* Func_GetLocalMapName( const char* pMapName );
const char* Func_GetLocalMapName( int MapID );

//���ѶǦ^false
bool Func_TransformCoordinate_MapToWorld( const SMapCoordinate& rMapCoord, SWorldCoordinate& rWorldCoord );

//�`�N rMapCoord.MapID �����n���w �n�ഫ���쪺�a��id
//��wdbFile���@�� ��MapID���� 
//�ήy�� ���b 0.0 ~ 1.05 ���� �Ǧ^false
bool Func_TransformCoordinate_WorldToMap( const SWorldCoordinate& rWorldCoord, SMapCoordinate& rMapCoord );


//�ήy�Ш��ozoneid
//�^��GUID �䤣��Ǧ^-1
//int Func_GetZoneIDByPosition( const char* pMapName , float fX , float fZ );
































