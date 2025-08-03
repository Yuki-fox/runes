#include "NetCli_Other.h"
#include "../net_channel/NetCli_Channel.h"

//-------------------------------------------------------------------
NetCli_Other*   		NetCli_Other::This         = NULL;
string					NetCli_Other::m_DialogContent;
vector<string>			NetCli_Other::m_DialogSelectStr;
LuaDialogType_ENUM		NetCli_Other::m_DialogType			= EM_LuaDialogType_None;
pfCliSpeakDialogfunc	NetCli_Other::m_pfuncCliSpeakDialog = NULL;
string					NetCli_Other::m_DialogTitle;
PVOID					NetCli_Other::m_pCliSpeakDialogContext = NULL;
map<int, int>			NetCli_Other::m_SessionID;
int						NetCli_Other::m_SessionCBID = 0;
map<int, OpenSessionResultFunc> NetCli_Other::m_OpenSessionResultFunc;
//-------------------------------------------------------------------
bool NetCli_Other::_Init()
{
	/*
	_Net.RegOnSrvPacketFunction	( EM_PG_Other_LtoC_OpenDialog					, _PG_Other_LtoC_OpenDialog				);
	_Net.RegOnSrvPacketFunction	( EM_PG_Other_LtoC_DialogSelectStr				, _PG_Other_LtoC_DialogSelectStr		);
	_Net.RegOnSrvPacketFunction	( EM_PG_Other_LtoC_CloseDialog					, _PG_Other_LtoC_CloseDialog			);
	_Net.RegOnSrvPacketFunction	( EM_PG_Other_LtoC_DialogSelectStrEnd			, _PG_Other_LtoC_DialogSelectStrEnd		);
	_Net.RegOnSrvPacketFunction	( EM_PG_Other_LtoC_DialogSetContent				, _PG_Other_LtoC_DialogSetContent		);
	_Net.RegOnSrvPacketFunction	( EM_PG_Other_LtoC_BeginCastingResult			, _PG_Other_LtoC_BeginCastingResult		);
	_Net.RegOnSrvPacketFunction	( EM_PG_Other_LtoC_InterruptCasting				, _PG_Other_LtoC_InterruptCasting		);
	_Net.RegOnSrvPacketFunction	( EM_PG_Other_LtoC_CastingOK					, _PG_Other_LtoC_CastingOK				);
	_Net.RegOnSrvPacketFunction	( EM_PG_Other_LtoC_RangeDataTransfer			, _PG_Other_LtoC_RangeDataTransfer		);
	_Net.RegOnSrvPacketFunction	( EM_PG_Other_LtoC_ColoringShopResult			, _PG_Other_LtoC_ColoringShopResult		);
	_Net.RegOnSrvPacketFunction	( EM_PG_Other_LtoC_PlayerInfoResult				, _PG_Other_LtoC_PlayerInfoResult		);
	_Net.RegOnSrvPacketFunction	( EM_PG_Other_LtoC_SysKeyFunctionResult			, _PG_Other_LtoC_SysKeyFunctionResult	);
	_Net.RegOnSrvPacketFunction	( EM_PG_Other_LtoC_ObjectPosInfo				, _PG_Other_LtoC_ObjectPosInfo			);
	_Net.RegOnSrvPacketFunction	( EM_PG_Other_LtoC_OpenClientMenu				, _PG_Other_LtoC_OpenClientMenu			);
	_Net.RegOnSrvPacketFunction	( EM_PG_Other_LtoC_DialogSetTitle				, _PG_Other_LtoC_DialogSetTitle			);
	_Net.RegOnSrvPacketFunction	( EM_PG_Other_LtoC_RunGlobalPlotResult			, _PG_Other_LtoC_RunGlobalPlotResult	);
	_Net.RegOnSrvPacketFunction	( EM_PG_Other_LtoC_HateListResult				, _PG_Other_LtoC_HateListResult			);
	_Net.RegOnSrvPacketFunction	( EM_PG_Other_LtoC_OpenGiveItemPlot				, _PG_Other_LtoC_OpenGiveItemPlot		);
	_Net.RegOnSrvPacketFunction	( EM_PG_Other_LtoC_CheckPasswordResult			, _PG_Other_LtoC_CheckPasswordResult	);
	_Net.RegOnSrvPacketFunction	( EM_PG_Other_DtoC_Find_DBID_RoleName			, _PG_Other_DtoC_Find_DBID_RoleName		);
	_Net.RegOnSrvPacketFunction	( EM_PG_Other_LtoC_CampID						, _PG_Other_LtoC_CampID					);
	_Net.RegOnSrvPacketFunction	( EM_PG_Other_LtoC_ExchangeMoneyTable			, _PG_Other_LtoC_ExchangeMoneyTable		);
	_Net.RegOnSrvPacketFunction	( EM_PG_Other_LtoC_DBStringTable				, _PG_Other_LtoC_DBStringTable			);
	_Net.RegOnSrvPacketFunction	( EM_PG_Other_LtoC_GoodEvilValue				, _PG_Other_LtoC_GoodEvilValue			);
	_Net.RegOnSrvPacketFunction	( EM_PG_Other_LtoC_ColoringHorseResult			, _PG_Other_LtoC_ColoringHorseResult	);
	_Net.RegOnSrvPacketFunction	( EM_PG_Other_LtoC_PKDeadDropItem				, _PG_Other_LtoC_PKDeadDropItem			);
	*/

	Cli_NetReg( PG_Other_LtoC_OpenDialog			);
	Cli_NetReg( PG_Other_LtoC_DialogSelectStr		);
	Cli_NetReg( PG_Other_LtoC_CloseDialog			);
	Cli_NetReg( PG_Other_LtoC_DialogSelectStrEnd	);
	Cli_NetReg( PG_Other_LtoC_DialogSetContent		);
	Cli_NetReg( PG_Other_LtoC_BeginCastingResult	);
	Cli_NetReg( PG_Other_LtoC_InterruptCasting		);
	Cli_NetReg( PG_Other_LtoC_CastingOK				);
	Cli_NetReg( PG_Other_LtoC_RangeDataTransfer		);
	Cli_NetReg( PG_Other_LtoC_ColoringShopResult	);
	Cli_NetReg( PG_Other_LtoC_PlayerInfoResult		);
	Cli_NetReg( PG_Other_LtoC_SysKeyFunctionResult	);
	Cli_NetReg( PG_Other_LtoC_ObjectPosInfo			);
	Cli_NetReg( PG_Other_LtoC_OpenClientMenu		);
	Cli_NetReg( PG_Other_LtoC_DialogSetTitle		);
	Cli_NetReg( PG_Other_LtoC_RunGlobalPlotResult	);
	Cli_NetReg( PG_Other_LtoC_HateListResult		);
	Cli_NetReg( PG_Other_LtoC_OpenGiveItemPlot		);
	Cli_NetReg( PG_Other_LtoC_CheckPasswordResult	);
	Cli_NetReg( PG_Other_DtoC_Find_DBID_RoleName	);
	Cli_NetReg( PG_Other_LtoC_CampID				);
//	Cli_NetReg( PG_Other_LtoC_ExchangeMoneyTable	);
	_Net.RegOnSrvPacketFunction	( EM_PG_Other_LtoC_ExchangeMoneyTable			, _PG_Other_LtoC_ExchangeMoneyTable		);
//	Cli_NetReg( PG_Other_LtoC_DBStringTable			);
	_Net.RegOnSrvPacketFunction	( EM_PG_Other_LtoC_DBStringTable				, _PG_Other_LtoC_DBStringTable			);
	Cli_NetReg( PG_Other_LtoC_GoodEvilValue			);
	Cli_NetReg( PG_Other_LtoC_ColoringHorseResult	);
	Cli_NetReg( PG_Other_LtoC_PKDeadDropItem		);
	Cli_NetReg( PG_Other_LtoC_PlayerEvent			);
	Cli_NetReg( PG_Other_LtoC_ClientClock			);
	Cli_NetReg( PG_Other_LtoC_AccountMoneyLockInfo	);
	Cli_NetReg( PG_Other_LtoC_AccountMoneyTranError	);
	Cli_NetReg( PG_Other_LtoC_DelJob				);

	Cli_NetReg( PG_Other_LtoC_SetSmallGameMenuType	);
	Cli_NetReg( PG_Other_LtoC_SetSmallGameMenuValue	);
	Cli_NetReg( PG_Other_LtoC_SetSmallGameMenuStr	);

	Cli_NetReg( PG_Other_LtoC_ResetInstanceResult	);
	Cli_NetReg( PG_Other_LtoC_ResetInstanceInfo		);

	Cli_NetReg( PG_Other_LtoC_PartyDice				);

	Cli_NetReg( PG_Other_LtoC_BodyShopResult		);

	Cli_NetReg( PG_Other_LtoC_MoneyVendorReport		);
	Cli_NetReg( PG_Other_LtoC_BotReport				);

	Cli_NetReg( PG_Other_LtoC_SortBankEnd			);

	Cli_NetReg( PG_Other_DtoC_LoadClientDataResult	);
	Cli_NetReg( PG_Other_LtoC_SaveClientDataResult	);

	Cli_NetReg( PG_Other_DtoC_LoadClientDataResult_Account	);
	Cli_NetReg( PG_Other_MtoC_SaveClientDataResult_Account	);

	Cli_NetReg( PG_Other_LtoC_SetMapMark			);
	Cli_NetReg( PG_Other_LtoC_ClsMapMark			);

	Cli_NetReg( PG_Other_LtoC_SetRoomValue			);
	Cli_NetReg( PG_Other_LtoC_ClsRoomValue			);

	Cli_NetReg( PG_Other_LtoC_AddExSkill			);
	Cli_NetReg( PG_Other_LtoC_DelExSkill			);
	Cli_NetReg( PG_Other_LtoC_SpellExSkillResult	);

	Cli_NetReg( PG_Other_LtoC_BaseObjValueChange	);
	Cli_NetReg( PG_Other_LtoC_TransferData_Range	);
	Cli_NetReg( PG_Other_LtoC_LuaClientEvent		);
	Cli_NetReg( PG_Other_LtoC_SetRoleValueResult	);
	Cli_NetReg( PG_Other_LtoC_SetNewTitleSysResult	);
	Cli_NetReg( PG_Other_LtoC_NewTitleSys_UseItemResult	);
	Cli_NetReg( PG_Other_LtoC_OpenSessionResult			);
	Cli_NetReg( PG_Other_LtoC_FixAccountFlag			);

	Cli_NetReg( PG_Other_LtoC_WeekInstances 				);
	Cli_NetReg( PG_Other_LtoC_WeekInstancesTeleportResult 	);
	Cli_NetReg( PG_Other_LtoC_WeekInstancesReset 			);
	Cli_NetReg( PG_Other_LtoC_WeekInstancesLiveTime 		);
	Cli_NetReg( PG_Other_LtoC_OpenWeekInstancesFrame 		);

	Cli_NetReg( PG_Other_LtoC_PhantomRankUpResult 	);
	Cli_NetReg( PG_Other_LtoC_PhantomLevelUpResult 	);
	Cli_NetReg( PG_Other_LtoC_PhantomEQResult		);
	Cli_NetReg( PG_Other_LtoC_FixPhantomEQ			);
	Cli_NetReg( PG_Other_LtoC_FixPhantom 			);
	Cli_NetReg( PG_Other_LtoC_OpenPhantomFrame 		);

	Cli_NetReg( PG_Other_LtoC_ItemCombinInfo 	);
	Cli_NetReg( PG_Other_LtoC_ItemCombinGroupID 	);

	_Net.RegOnSrvPacketFunction( EM_PG_Other_LtoC_ZoneOpen , _PG_Other_LtoC_ZoneOpen , 10000  );

	// Cli_NetReg( PG_Other_LtoC_ZoneOpen				);

	ClitoCli_NetReg( PG_CtoC_Emote );
	ClitoCli_NetReg( PG_CtoC_PlayerTarget );


    return true;
}

void NetCli_Other::_PG_Other_LtoC_ItemCombinInfo( int Sockid , int size , void* data )
{
	PG_Other_LtoC_ItemCombinInfo* pg = (PG_Other_LtoC_ItemCombinInfo*)data;
	This->RL_ItemCombinInfo( pg->GroupID , pg->ItemCombine );
}
void NetCli_Other::_PG_Other_LtoC_ItemCombinGroupID( int Sockid , int size , void* data )
{
	PG_Other_LtoC_ItemCombinGroupID* pg = (PG_Other_LtoC_ItemCombinGroupID*)data;
	This->RL_ItemCombinGroupID( pg->GroupID );
}

void NetCli_Other::_PG_Other_LtoC_PhantomRankUpResult( int Sockid , int size , void* data )
{
	PG_Other_LtoC_PhantomRankUpResult* pg = (PG_Other_LtoC_PhantomRankUpResult*)data;
	This->RL_PhantomRankUpResult( pg->Summon , pg->Result );
}
void NetCli_Other::_PG_Other_LtoC_PhantomLevelUpResult( int Sockid , int size , void* data )
{
	PG_Other_LtoC_PhantomLevelUpResult* pg = (PG_Other_LtoC_PhantomLevelUpResult*)data;
	This->RL_PhantomLevelUpResult( pg->Result );
}
void NetCli_Other::_PG_Other_LtoC_PhantomEQResult( int Sockid , int size , void* data )
{
	PG_Other_LtoC_PhantomEQResult* pg = (PG_Other_LtoC_PhantomEQResult*)data;
	This->RL_PhantomEQResult( pg->Result );
}
void NetCli_Other::_PG_Other_LtoC_FixPhantomEQ( int Sockid , int size , void* data )
{
	PG_Other_LtoC_FixPhantomEQ* pg = (PG_Other_LtoC_FixPhantomEQ*)data;
	This->RL_FixPhantomEQ( pg->EQ );
}
void NetCli_Other::_PG_Other_LtoC_FixPhantom( int Sockid , int size , void* data )
{
	PG_Other_LtoC_FixPhantom* pg = (PG_Other_LtoC_FixPhantom*)data;
	This->RL_FixPhantom( pg->PhantomID , pg->Info );
}
void NetCli_Other::_PG_Other_LtoC_OpenPhantomFrame( int Sockid , int size , void* data )
{
	This->RL_OpenPhantomFrame();
}
void NetCli_Other::_PG_Other_LtoC_WeekInstancesLiveTime( int Sockid , int size , void* data )
{
	PG_Other_LtoC_WeekInstancesLiveTime* pg = (PG_Other_LtoC_WeekInstancesLiveTime*)data;
	This->RL_WeekInstancesLiveTime( pg->Sec );
}
void NetCli_Other::_PG_Other_LtoC_OpenWeekInstancesFrame( int Sockid , int size , void* data )
{
	This->RL_OpenWeekInstancesFrame();
}
void NetCli_Other::_PG_Other_LtoC_WeekInstances( int Sockid , int size , void* data )
{
	PG_Other_LtoC_WeekInstances* pg = (PG_Other_LtoC_WeekInstances*)data;
	This->RL_WeekInstances( pg->Day , pg->Count , pg->Info );
}
void NetCli_Other::_PG_Other_LtoC_WeekInstancesTeleportResult( int Sockid , int size , void* data )
{
	PG_Other_LtoC_WeekInstancesTeleportResult* pg = (PG_Other_LtoC_WeekInstancesTeleportResult*)data;
	This->RL_WeekInstancesTeleportResult( pg->Result );
}
void NetCli_Other::_PG_Other_LtoC_WeekInstancesReset( int Sockid , int size , void* data )
{
	PG_Other_LtoC_WeekInstancesReset* pg = (PG_Other_LtoC_WeekInstancesReset*)data;
	This->RL_WeekInstancesReset( pg->Result );
}
void NetCli_Other::_PG_Other_LtoC_FixAccountFlag		( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_FixAccountFlag* PG = (PG_Other_LtoC_FixAccountFlag*)Data;
	This->RL_FixAccountFlag( PG->AccountFlag );
}

void NetCli_Other::_PG_Other_LtoC_NewTitleSys_UseItemResult( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_NewTitleSys_UseItemResult* PG = (PG_Other_LtoC_NewTitleSys_UseItemResult*)Data;
	This->RL_NewTitleSys_UseItemResult( PG->Result );
}

void NetCli_Other::_PG_Other_LtoC_OpenSessionResult( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_OpenSessionResult* PG = (PG_Other_LtoC_OpenSessionResult*)Data;
	This->RL_OpenSessionResult( PG->Type, PG->SessionID, PG->CBID );
}

void NetCli_Other::_PG_Other_LtoC_SetNewTitleSysResult	( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_SetNewTitleSysResult* PG = (PG_Other_LtoC_SetNewTitleSysResult*)Data;
	This->RL_SetNewTitleSysResult( PG->Result );
}

void NetCli_Other::_PG_Other_LtoC_ZoneOpen				( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_ZoneOpen* PG = (PG_Other_LtoC_ZoneOpen*)Data;
	This->RL_ZoneOpen( PG->ZoneID );
}
void NetCli_Other::_PG_Other_LtoC_SetRoleValueResult	( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_SetRoleValueResult* PG = (PG_Other_LtoC_SetRoleValueResult*)Data;
	This->RL_SetRoleValueResult( PG->Result );
}
void NetCli_Other::_PG_Other_LtoC_LuaClientEvent		( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_LuaClientEvent* PG = (PG_Other_LtoC_LuaClientEvent*)Data;
	This->RL_LuaClientEvent( PG->GItemID , PG->EventType , PG->Content.Begin() );
}

void NetCli_Other::_PG_Other_LtoC_TransferData_Range	( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_TransferData_Range* PG = (PG_Other_LtoC_TransferData_Range*)Data;
	This->RL_TransferData_Range( PG->GItemID , PG->Data );
}
void NetCli_Other::_PG_Other_LtoC_BaseObjValueChange	( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_BaseObjValueChange* PG = (PG_Other_LtoC_BaseObjValueChange*)Data;
	This->RL_BaseObjValueChange( PG->GItemID , PG->Type , PG->ValueStr.Begin() , PG->Value );
}

void NetCli_Other::_PG_Other_LtoC_AddExSkill			( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_AddExSkill* PG = (PG_Other_LtoC_AddExSkill*) Data;
	This->RL_AddExSkill( PG->Info );
}
void NetCli_Other::_PG_Other_LtoC_DelExSkill			( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_DelExSkill* PG = (PG_Other_LtoC_DelExSkill*) Data;
	This->RL_DelExSkill( PG->Info );
}
void NetCli_Other::_PG_Other_LtoC_SpellExSkillResult	( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_SpellExSkillResult* PG = (PG_Other_LtoC_SpellExSkillResult*) Data;
	This->RL_SpellExSkillResult( PG->Result , PG->Msg );
}

void NetCli_Other::_PG_Other_LtoC_SetRoomValue			( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_SetRoomValue* PG = (PG_Other_LtoC_SetRoomValue*) Data;
	This->RL_SetRoomValue( PG->KeyID , PG->Value );
}
void NetCli_Other::_PG_Other_LtoC_ClsRoomValue			( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_ClsRoomValue* PG = (PG_Other_LtoC_ClsRoomValue*) Data;
	This->RL_ClsRoomValue( PG->KeyID );
}

void NetCli_Other::_PG_Other_LtoC_SetMapMark			( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_SetMapMark* PG = (PG_Other_LtoC_SetMapMark*) Data;
	This->RL_SetMapMark( PG->Info );
}

void NetCli_Other::_PG_Other_LtoC_ClsMapMark			( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_ClsMapMark* PG = (PG_Other_LtoC_ClsMapMark*) Data;
	This->RL_ClsMapMark( PG->MarkID );
}

void NetCli_Other::_PG_Other_DtoC_LoadClientDataResult_Account ( int Sockid , int Size , void* Data )
{
	PG_Other_DtoC_LoadClientDataResult_Account* PG = (PG_Other_DtoC_LoadClientDataResult_Account*) Data;
	This->RD_LoadClientDataResult_Account( PG->KeyID , PG->Size , PG->Data  );
}
void NetCli_Other::_PG_Other_MtoC_SaveClientDataResult_Account ( int Sockid , int Size , void* Data )
{
	PG_Other_MtoC_SaveClientDataResult_Account* PG = (PG_Other_MtoC_SaveClientDataResult_Account*) Data;
	This->RM_SaveClientDataResult_Account( PG->KeyID , PG->Result  );
}

void NetCli_Other::_PG_Other_DtoC_LoadClientDataResult ( int Sockid , int Size , void* Data )
{
	PG_Other_DtoC_LoadClientDataResult* PG = (PG_Other_DtoC_LoadClientDataResult*) Data;
	This->RD_LoadClientDataResult( PG->KeyID , PG->Size , PG->Data  );
}
void NetCli_Other::_PG_Other_LtoC_SaveClientDataResult ( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_SaveClientDataResult* PG = (PG_Other_LtoC_SaveClientDataResult*) Data;
	This->RL_SaveClientDataResult( PG->KeyID , PG->Result  );
}

void NetCli_Other::_PG_Other_LtoC_SortBankEnd			( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_SortBankEnd* PG = (PG_Other_LtoC_SortBankEnd*) Data;
	This->RL_SortBankEnd( );
}

void NetCli_Other::_PG_Other_LtoC_UnlockBotReport	( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_UnlockBotReport* PG = (PG_Other_LtoC_UnlockBotReport*) Data;
	This->RL_UnlockBotReport( PG->Name.Begin() , PG->Type );
}

void NetCli_Other::_PG_Other_LtoC_MoneyVendorReport	( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_MoneyVendorReport* PG = (PG_Other_LtoC_MoneyVendorReport*) Data;
	This->RL_MoneyVendorReport( PG->Count );
}
void NetCli_Other::_PG_Other_LtoC_BotReport			( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_BotReport* PG = (PG_Other_LtoC_BotReport*) Data;
	This->RL_BotReport( PG->Count );
}

void NetCli_Other::_PG_Other_LtoC_BodyShopResult	( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_BodyShopResult* PG = (PG_Other_LtoC_BodyShopResult*) Data;
	This->RL_BodyShopResult( PG->Result );
}
void NetCli_Other::_PG_Other_LtoC_PartyDice			( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_PartyDice* PG = (PG_Other_LtoC_PartyDice*) Data;
	This->RL_PartyDice( PG->PlayerDBID , PG->Type , PG->Rand );
}

void NetCli_Other::_PG_Other_LtoC_ResetInstanceResult( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_ResetInstanceResult* PG = (PG_Other_LtoC_ResetInstanceResult*) Data;
	This->RL_ResetInstanceResult( PG->Result );
}
void NetCli_Other::_PG_Other_LtoC_ResetInstanceInfo( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_ResetInstanceInfo* PG = (PG_Other_LtoC_ResetInstanceInfo*) Data;
	This->RL_ResetInstanceInfo( PG->ZoneID , PG->IsReset );
}

void NetCli_Other::_PG_Other_LtoC_SetSmallGameMenuType	( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_SetSmallGameMenuType* PG = (PG_Other_LtoC_SetSmallGameMenuType*) Data;
	This->RL_SetSmallGameMenuType( PG->MenuID , PG->Type  );
}
void NetCli_Other::_PG_Other_LtoC_SetSmallGameMenuValue	( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_SetSmallGameMenuValue* PG = (PG_Other_LtoC_SetSmallGameMenuValue*) Data;
	This->RL_SetSmallGameMenuValue( PG->MenuID , PG->ID , PG->Value );
}
void NetCli_Other::_PG_Other_LtoC_SetSmallGameMenuStr	( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_SetSmallGameMenuStr* PG = (PG_Other_LtoC_SetSmallGameMenuStr*) Data;
	This->RL_SetSmallGameMenuStr( PG->MenuID , PG->ID , PG->Content.Begin() );
}

void NetCli_Other::_PG_Other_LtoC_DelJob			( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_DelJob* PG = (PG_Other_LtoC_DelJob*) Data;
	This->RL_DelJob( PG->Job );
}

void NetCli_Other::_PG_Other_LtoC_SetCycleMagic		( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_SetCycleMagic* PG = (PG_Other_LtoC_SetCycleMagic*) Data;
	This->RL_SetCycleMagic( PG->MagicID , PG->MagicPos );
}

void NetCli_Other::_PG_Other_LtoC_AccountMoneyTranError( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_AccountMoneyTranError* PG = (PG_Other_LtoC_AccountMoneyTranError*) Data;
	This->R_AccountMoneyTranError( );
}
void NetCli_Other::_PG_Other_LtoC_AccountMoneyLockInfo	( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_AccountMoneyLockInfo* PG = (PG_Other_LtoC_AccountMoneyLockInfo*) Data;
	This->R_AccountMoneyLockInfo( PG->LockAccountMoney , PG->LockAccount_Forever );
}
void NetCli_Other::_PG_Other_LtoC_ClientClock			( int Sockid , int Size , void* Data )
{
	if ( s_boFinishUI )	{
		PG_Other_LtoC_ClientClock* PG = (PG_Other_LtoC_ClientClock*) Data;
		This->R_ClientClock( PG->BeginTime , PG->NowTime , PG->EndTime , PG->Type );		
	} else {
		PushTempEvent(Sockid, Size, Data, _PG_Other_LtoC_ClientClock);
	}
}

void NetCli_Other::_PG_Other_LtoC_PlayerEvent			( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_PlayerEvent* PG = (PG_Other_LtoC_PlayerEvent*) Data;
	This->R_PlayerEvent( PG->Type );
}
void NetCli_Other::_PG_Other_LtoC_PKDeadDropItem		( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_PKDeadDropItem* PG = (PG_Other_LtoC_PKDeadDropItem*) Data;
	This->R_PKDeadDropItem( PG->Item );
}
void NetCli_Other::_PG_Other_LtoC_ColoringHorseResult	( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_ColoringHorseResult* PG = (PG_Other_LtoC_ColoringHorseResult*) Data;
	This->R_ColoringHorseResult( PG->Result );
}

void NetCli_Other::_PG_Other_LtoC_DBStringTable	( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_DBStringTable* PG = (PG_Other_LtoC_DBStringTable*) Data;
	This->R_DBStringTable( PG->Count , PG->DataSize , PG->Data );
}

void NetCli_Other::_PG_Other_LtoC_ExchangeMoneyTable	( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_ExchangeMoneyTable* PG = (PG_Other_LtoC_ExchangeMoneyTable*) Data;
	This->R_ExchangeMoneyTable( PG->Count , PG->DataSize , PG->Data );
}

void NetCli_Other::_PG_Other_LtoC_GoodEvilValue			( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_GoodEvilValue* PG = (PG_Other_LtoC_GoodEvilValue*) Data;
	This->R_GoodEvilValue( PG->PlayerGUID , PG->GoodEvilValue , PG->GoodEvilType );
}
void NetCli_Other::_PG_Other_LtoC_CampID				( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_CampID* PG = (PG_Other_LtoC_CampID*) Data;
	This->R_CampID( PG->GItemID , PG->CampID );
}
void NetCli_Other::_PG_Other_DtoC_Find_DBID_RoleName	( int Sockid , int Size , void* Data )
{
	PG_Other_DtoC_Find_DBID_RoleName* PG = (PG_Other_DtoC_Find_DBID_RoleName*) Data;
	This->R_Find_DBID_RoleName( PG->DBID , (char*)PG->RoleName.Begin() , PG->IsFind );
}
void NetCli_Other::_PG_Other_LtoC_CheckPasswordResult	( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_CheckPasswordResult* PG = (PG_Other_LtoC_CheckPasswordResult*) Data;
	This->R_CheckPasswordResult( (char*)PG->Password.Begin() , PG->Result );
}

void NetCli_Other::_PG_Other_LtoC_OpenGiveItemPlot		( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_OpenGiveItemPlot* PG = (PG_Other_LtoC_OpenGiveItemPlot*) Data;
	This->R_OpenGiveItemPlo( PG->IsSort , PG->Count , PG->TargetGUID );

}

void NetCli_Other::_PG_Other_LtoC_HateListResult		( int Sockid , int Size , void* Data )	
{
	PG_Other_LtoC_HateListResult* PG = (PG_Other_LtoC_HateListResult*) Data;
	This->R_HateListResult( PG->NpcGUID , PG->NPCHate );
}

void NetCli_Other::_PG_Other_LtoC_RunGlobalPlotResult	( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_RunGlobalPlotResult* PG = (PG_Other_LtoC_RunGlobalPlotResult*) Data;
	This->R_RunGlobalPlotResult( PG->Result );
}
//Cli to Cli 傳送情緒指令
void NetCli_Other::_PG_CtoC_Emote( void *Data , int Size )
{
	PG_CtoC_Emote* PG = (PG_CtoC_Emote*)Data;
	This->R_CtoC_Emote( PG );
}

//-----------------------------------------------------------------------------------------------------
void NetCli_Other::_PG_CtoC_PlayerTarget( void *Data , int Size )
{
	PG_CtoC_PlayerTarget* PG = (PG_CtoC_PlayerTarget*)Data;
	This->R_PlayerTarget(PG->player, PG->target);
}

//-------------------------------------------------------------------
bool NetCli_Other::_Release()
{
    return true;
}
//-------------------------------------------------------------------
void NetCli_Other::_PG_Other_LtoC_OpenClientMenu	( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_OpenClientMenu* PG = (PG_Other_LtoC_OpenClientMenu*) Data;
	This->R_OpenClientMenu( PG->MenuType );
}
void NetCli_Other::_PG_Other_LtoC_ObjectPosInfo		( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_ObjectPosInfo* PG = (PG_Other_LtoC_ObjectPosInfo*) Data;

	ObjectPosInfoStruct* Info = new ObjectPosInfoStruct[PG->Count];
	MyLzoClass myZip;
	int RetSize = myZip.Decode( PG->Data , PG->DataSize , PG->Count*sizeof(ObjectPosInfoStruct) , (char*)Info );

	This->R_ObjectPosInfo( PG->Count , Info );

	delete Info;
}

void NetCli_Other::_PG_Other_LtoC_SysKeyFunctionResult	( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_SysKeyFunctionResult* PG = (PG_Other_LtoC_SysKeyFunctionResult*) Data;
	This->R_SysKeyFunctionResult( PG->Result , PG->KeyID , PG->TargetGUID );
}

void NetCli_Other::_PG_Other_LtoC_PlayerInfoResult		( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_PlayerInfoResult* PG = (PG_Other_LtoC_PlayerInfoResult*) Data;
	if( PG->Result == true )
		This->R_PlayerInfoOK( PG->GItemID , PG->EQ , PG->Note.Begin() );
	else
		This->R_PlayerInfoFailed( PG->GItemID );

}

void NetCli_Other::_PG_Other_LtoC_ColoringShopResult	( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_ColoringShopResult* PG = (PG_Other_LtoC_ColoringShopResult*) Data;
	This->R_ColoringShopResult( PG->Result );
}
void NetCli_Other::_PG_Other_LtoC_RangeDataTransfer	( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_RangeDataTransfer* PG = (PG_Other_LtoC_RangeDataTransfer*) Data;
	This->R_RangeDataTransfer( PG->DataSize , PG->Data );
}
/*
void NetCli_Other::_PG_Other_LtoC_ItemExchangeResult	( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_ItemExchangeResult* PG = (PG_Other_LtoC_ItemExchangeResult*) Data;
	This->R_ItemExchangeResult( PG->ResultType , PG->Item , PG->Money , PG->Money_Account );
}
*/
void NetCli_Other::_PG_Other_LtoC_BeginCastingResult	( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_BeginCastingResult* PG = (PG_Other_LtoC_BeginCastingResult*) Data;
	This->R_BeginCastingResult( PG->Type , PG->ItemID , PG->CastingTime , PG->Result );
}
void NetCli_Other::_PG_Other_LtoC_InterruptCasting		( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_InterruptCasting* PG = (PG_Other_LtoC_InterruptCasting*) Data;
	This->R_InterruptCasting( PG->Type );
}
void NetCli_Other::_PG_Other_LtoC_CastingOK				( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_CastingOK* PG = (PG_Other_LtoC_CastingOK*) Data;
	This->R_CastingOK( PG->Type );
}
/*
void NetCli_Other::_PG_Other_XtoC_SystemMessage		( int Sockid , int Size , void* Data )
{
	PG_Other_XtoC_SystemMessage* PG = (PG_Other_XtoC_SystemMessage*) Data;
	This->R_SystemMessage( PG->Msg );
}
*/
/*
void NetCli_Other::_PG_Other_XtoC_SystemMessageData	( int Sockid , int Size , void* Data )
{
	PG_Other_XtoC_SystemMessageData* PG = (PG_Other_XtoC_SystemMessageData*) Data;
	This->R_SystemMessageData( PG->Msg , PG->DataSize , PG->Data );
}
*/

void NetCli_Other::_PG_Other_LtoC_OpenDialog			( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_OpenDialog* PG = (PG_Other_LtoC_OpenDialog*) Data;	
	This->R_CreateDialog( PG->Type, PG->Content.Begin() );
}
void NetCli_Other::_PG_Other_LtoC_DialogSelectStr		( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_DialogSelectStr* PG = (PG_Other_LtoC_DialogSelectStr*) Data;
	m_DialogSelectStr.push_back( PG->SelectStr.Begin() );
}
void NetCli_Other::_PG_Other_LtoC_CloseDialog			( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_CloseDialog* PG = (PG_Other_LtoC_CloseDialog*) Data;
	This->R_CloseDialog( );
}
void NetCli_Other::_PG_Other_LtoC_DialogSelectStrEnd	( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_DialogSelectStrEnd* PG = (PG_Other_LtoC_DialogSelectStrEnd*) Data;
	This->R_OpenDialog( m_DialogType , NULL, m_DialogContent.c_str(), m_DialogSelectStr, NULL, NULL );
}
void NetCli_Other::_PG_Other_LtoC_DialogSetContent		( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_DialogSetContent* PG = (PG_Other_LtoC_DialogSetContent*) Data;
	This->R_DialogSetContent( PG->Content.Begin() );
}

void NetCli_Other::_PG_Other_LtoC_DialogSetTitle( int Sockid , int Size , void* Data )
{
	PG_Other_LtoC_DialogSetTitle* PG = (PG_Other_LtoC_DialogSetTitle*) Data;
	This->R_DialogSetTitle( PG->Title.Begin() );
}





//要求開始跑Casting Bar
void NetCli_Other::S_BeginCastingRequest		( CastingTypeENUM Type , int ItemID , int TargetGUID )
{
	PG_Other_CtoL_BeginCastingRequest Send;
	Send.Type = Type;
	Send.ItemID = ItemID;
	Send.TargetGUID = TargetGUID;
	SendToLocal( sizeof(Send) , &Send );
}
//中斷Casting Bar
void NetCli_Other::S_InterruptCastingRequest	( )
{
	PG_Other_CtoL_InterruptCastingRequest Send;
	SendToLocal( sizeof(Send) , &Send );
}
//傳送效能資訊
void NetCli_Other::S_SendPerformance	(const char* cMac, float MaxCPUUsage, float MaxMemUsage, float MaxFPS, float AverageCPUUsage, float AverageMemUsage, float AverageFPS, float MinCPUUsage, float MinMemUsage, float MinFPS, float Ping, float PosX, float PosY, float PosZ )
{
	PG_Other_CtoL_Performance Send;
	strcpy(Send.cMac, cMac);
	Send.MaxCPUUsage = MaxCPUUsage;
	Send.MaxMemUsage = MaxMemUsage;
	Send.MaxFPS = MaxFPS;
	Send.AverageCPUUsage = AverageCPUUsage;
	Send.AverageMemUsage = AverageMemUsage;
	Send.AverageFPS = AverageFPS;
	Send.MinCPUUsage = MinCPUUsage;
	Send.MinMemUsage = MinMemUsage;
	Send.MinFPS = MinFPS;
	Send.Ping = Ping;
	Send.PosX = PosX;
	Send.PosY = PosY;
	Send.PosZ = PosZ;
	SendToLocal( sizeof(Send) , &Send );
}
//傳送Tracert資訊
void NetCli_Other::S_SendTracertList(int DBID, int ping, TracertList list)
{
	PG_Other_CtoL_TracertList Send;
	Send.DBID = DBID;
	Send.Ping = ping;
	Send.List = list;
	SendToLocal( sizeof(Send) , &Send );
}

//要求兌換物品
/*
void NetCli_Other::S_ItemExchangeRequest	( char* ItemKey , int ItemSerial )
{
	PG_Other_CtoL_ItemExchangeRequest Send;
	strncpy( Send.ItemKey , ItemKey , sizeof(Send.ItemKey) );
	Send.ItemSerial = ItemSerial;
	SendToLocal( sizeof(Send) , &Send );
}
*/
//要求轉送範圍資訊
void NetCli_Other::S_RangeDataTransferRequest( int Range , int Size , const char* Data )
{
	if( Size >_MAX_CLIENT_TRANSFER_DATA_SIZE )
		return;

	PG_Other_CtoL_RangeDataTransferRequest Send;
	Send.Range = Range;
	Send.DataSize = Size;
	memcpy( Send.Data , Data , Size );
	SendToLocal( sizeof(Send) , &Send );	
}
void NetCli_Other::S_SendEmote( int sourceID, const char* pTargetName, int emoteID )
{
	PG_CtoC_Emote emote;

	emote.sourceID = sourceID;
	
	if( pTargetName )
		strcpy( emote.szTargetName , pTargetName );
	else
		ZeroMemory( emote.szTargetName , sizeof(emote.szTargetName) );
	
	emote.emoteID = emoteID;
	S_RangeDataTransferRequest( 200 , sizeof(PG_CtoC_Emote) , (const char*)&emote);
}
//-----------------------------------------------------------------------------------------------------
void NetCli_Other::S_SendPlayerTarget(int sourceID, int targetID)
{
	PG_CtoC_PlayerTarget Data;

	Data.player = sourceID;
	Data.target = targetID;

	S_RangeDataTransferRequest( 600 , sizeof(PG_CtoC_PlayerTarget) , (const char*)&Data);
}
void NetCli_Other::S_ColoringShopRequest( ColoringStruct& ColorInfo , const char* Password )
{
	PG_Other_CtoL_ColoringShopRequest Send;
	Send.ColorInfo = ColorInfo;
	Send.Password = Password;
	SendToLocal( sizeof(Send) , &Send );	
}
//要求某玩家的裝備資料
void NetCli_Other::S_PlayerInfoRequest( int PlayerGItemID )
{
	PG_Other_CtoL_PlayerInfoRequest Send;
	Send.GItemID = PlayerGItemID;
	SendToLocal( sizeof(Send) , &Send );	
}
void NetCli_Other::S_SysKeyFunctionRequest( int KeyID , int TargetGUID )
{
	PG_Other_CtoL_SysKeyFunctionRequest Send;
	Send.KeyID = KeyID;
	Send.TargetGUID = TargetGUID;
	SendToLocal( sizeof(Send) , &Send );	
}

//要求區域內所有NPC的位置資料
void NetCli_Other::S_AllObjectPosRequest( )
{
	PG_Other_CtoL_AllObjectPosRequest Send;
	SendToLocal( sizeof(Send) , &Send );	
}

//系統全域的劇情
void NetCli_Other::S_RunGlobalPlotRequest( int PlotID , int Value )
{
	PG_Other_CtoL_RunGlobalPlotRequest Send;
	Send.PlotID = PlotID;
	Send.Value = Value;
	SendToLocal( sizeof(Send) , &Send );	

}
//要求仇恨列表	
void NetCli_Other::S_HateListRequest( int NpcGUID )
{
	PG_Other_CtoL_HateListRequest Send;
	Send.NpcGUID = NpcGUID;
	SendToLocal( sizeof(Send) , &Send );	
}
void NetCli_Other::S_ManagePasswordKey( )
{
	PG_Other_CtoL_ManagePasswordKey Send;
	SendToLocal( sizeof(Send) , &Send );	
}
void NetCli_Other::S_ManagePassword( const char* MD5 )
{
	PG_Other_CtoL_ManagePassword Send;
	Send.MD5Pwd = MD5;
	SendToLocal( sizeof(Send) , &Send );	
}

void NetCli_Other::S_GiveItemPlot( bool IsSort , int ItemPos[4] )
{
	PG_Other_CtoL_GiveItemPlot Send;
	Send.IsSort = IsSort;
	memcpy( Send.ItemPos , ItemPos , sizeof(Send.ItemPos) );
	SendToLocal( sizeof(Send) , &Send );	
}

void NetCli_Other::S_CheckPassword( const char* Password )
{
	PG_Other_CtoL_CheckPassword Send;
	Send.Password = Password;
	SendToLocal( sizeof(Send) , &Send );	
}

void NetCli_Other::SL_GmNotification( const char* Subject, const char* Content, const char* Classification )
{
	PG_Other_CtoL_GmNotification Send;
	Send.Subject = Subject;
	Send.Content = Content;
	Send.Classification = Classification;

	SendToLocal( sizeof(Send) , &Send );	
}

//dbid 與 名字查尋
void NetCli_Other::S_Find_DBID_RoleName	( int DBID , const char* RoleName )
{
	PG_Other_CtoL_Find_DBID_RoleName Send;
	Send.DBID = DBID;
	Send.RoleName = RoleName;

	SendToLocal_Base( sizeof(Send) , &Send );	
}

void NetCli_Other::S_DialogInput		( int SelectID, const char* pszText, int iCheckResult )
{
	PG_Other_CtoL_DialogInput	Packet;

	Packet.SelectID		= SelectID;
	Packet.pszString	= pszText;
	Packet.iCheckResult	= iCheckResult;
	SendToLocal( sizeof(Packet) , &Packet );
}

void NetCli_Other::S_DialogSelectID	( int SelectID )
{
	PG_Other_CtoL_DialogSelectID Send;
	Send.SelectID = SelectID;
	SendToLocal( sizeof(Send) , &Send );
}

void NetCli_Other::S_ColoringHorse( int Color[4], char* Password , int ItemPos )
{
	PG_Other_CtoL_ColoringHorse Send;
	Send.ItemPos = ItemPos;
	Send.Password = Password;
	memcpy( Send.Color , Color , sizeof(Send.Color) );
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Other::S_RoleNameDuplicateErr( int DBID1 , int DBID2 , const char* RoleName )
{
	PG_Other_CtoL_RoleNameDuplicateErr Send;
	Send.RoleName = RoleName; 
	Send.PlayerDBID[0] = DBID1;
	Send.PlayerDBID[1] = DBID2;

	SendToLocal( sizeof(Send) , &Send );
}

//通知Server看的目標
void NetCli_Other::SL_LookTarget( int TargetGUID )
{
	PG_Other_CtoL_LookTarget Send;
	Send.TargetGUID = TargetGUID; 

	SendToLocal( sizeof(Send) , &Send );
}

//設定玩家循環法術
void NetCli_Other::SL_SetCycleMagic( int MagicID , int MagicPos )
{
	PG_Other_CtoL_SetCycleMagic Send;
	Send.MagicID = MagicID; 
	Send.MagicPos = MagicPos;

	SendToLocal( sizeof(Send) , &Send );
}

//要求關閉Master 的登入
void NetCli_Other::SL_CloseMaster( const char* Password )
{
	PG_Other_CtoL_CloseMaster Send;
	strncpy( Send.Password , Password , sizeof(Send.Password) );
	SendToLocal( sizeof(Send) , &Send );
}

//封包衰減設定
void NetCli_Other::SL_SetSendPacketMode( SendPacketModeStruct& Mode )
{
	PG_Other_CtoL_SetSendPacketMode Send;
	Send.Mode = Mode;
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Other::SL_ResetInstanceRequest		( int ZoneID )
{
	PG_Other_CtoL_ResetInstanceRequest Send;
	Send.ZoneID = ZoneID;
	SendToLocal( sizeof(Send) , &Send );
}

bool NetCli_Other::SL_GmResetRole				( int RoleBaseSize , int PlayerRoleBaseSize , BaseRoleData&	BaseData , PlayerBaseRoleData& PlayerBaseData )
{
	if( RoleBaseSize != sizeof(BaseData) || PlayerRoleBaseSize != sizeof(PlayerBaseData) )
		return false;
	PG_Other_CtoL_GmResetRole Send;
	Send.RoleBaseSize = RoleBaseSize;
	Send.PlayerRoleBaseSize = PlayerRoleBaseSize;
	Send.BaseData = BaseData;
	Send.PlayerBaseData = PlayerBaseData;
	SendToLocal( sizeof(Send) , &Send );
	return true;
}

void NetCli_Other::SL_PartyDice				( int Type )
{
	PG_Other_CtoL_PartyDice Send;
	Send.Type = Type;
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Other::SL_BodyShop( char Bone[15]  )
{
	PG_Other_CtoL_BodyShop Send;
	memcpy( Send.BoneScale , Bone , sizeof(Send.BoneScale) );
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Other::SL_MoneyVendorReport		( const char* Name , const char* Content )
{
	PG_Other_CtoL_MoneyVendorReport Send;
	Send.Content = Content;
	Send.Name = Name;
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Other::SL_BotReport				( const char* Name , const char* Content )
{
	PG_Other_CtoL_BotReport Send;
	Send.Content = Content;
	Send.Name = Name;
	SendToLocal( sizeof(Send) , &Send );
}

void NetCli_Other::SL_UnlockBotReport			( )
{
	PG_Other_CtoL_UnlockBotReport Send;
	SendToLocal( sizeof(Send) , &Send );
}

void NetCli_Other::SL_SortBankRequest( )
{
	PG_Other_CtoL_SortBankRequest Send;
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Other::SM_LoadClientData_Account	( int KeyID )
{
	PG_Other_CtoM_LoadClientData_Account Send;
	Send.KeyID = KeyID;
	SendToMaster( sizeof(Send) , &Send );
}
void NetCli_Other::SM_SaveClientData_Account	( int KeyID , int Size , char* Data )
{
	if( Size > 0x10000 )
		return;

	PG_Other_CtoM_SaveClientData_Account Send;
	Send.KeyID = KeyID;
	char* DataSrc=Data;


	int size=Size;

	int DataSize=1024;
	for (int i=0;size>0;i++,DataSrc+=DataSize,size-=DataSize)
	{

		if (size<DataSize)
			DataSize=size;

		Send.PageID  =i;
		Send.Size = DataSize;
		memcpy( Send.Data , DataSrc , DataSize );
		SendToMaster( sizeof(Send) , &Send );

	}


	/*
		// 2k = 1k +1k + 0k 會多送一個 0 data size封包

	for( int i = 0 ; ; i++ )
	{
		Send.PageID = i;
		if( Size < 1024 )
		{
			memcpy( Send.Data , Data+i*1024 , Size );
			Send.Size = Size;
			SendToMaster( sizeof(Send) , &Send );
			break;
		}
		else
		{
			memcpy( Send.Data , Data+i*1024 , 1024 );
			SendToMaster( sizeof(Send) , &Send );
			Size -= 1024;
		}
	}
	*/
}

void NetCli_Other::SL_LoadClientData	( int KeyID )
{
	PG_Other_CtoL_LoadClientData Send;
	Send.KeyID = KeyID;
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Other::SL_SaveClientData	( int KeyID , int Size , char* Data )
{
	if( Size > 0x10000 )
		return;

	PG_Other_CtoL_SaveClientData Send;
	Send.KeyID = KeyID;	
	char* DataSrc=Data;
	

	int size=Size;
	
	int DataSize=1024;
	for (int i=0;size>0;i++,DataSrc+=DataSize,size-=DataSize)
	{
		
		if (size<DataSize)
			DataSize=size;

		Send.PageID  =i;
		Send.Size = DataSize;
		memcpy( Send.Data , DataSrc , DataSize );
		SendToLocal( sizeof(Send) , &Send );

	}


/*
// 2k = 1k +1k + 0k 會多送一個 0 data size封包
	for( int i = 0 ; ; i++ )
	{
		Send.PageID = i;
		if( Size < 1024 )
		{
			memcpy( Send.Data , Data+i*1024 , Size );
			Send.Size = Size;
			SendToLocal( sizeof(Send) , &Send );
			break;
		}
		else
		{
			memcpy( Send.Data , Data+i*1024 , 1024 );
			SendToLocal( sizeof(Send) , &Send );
			Size -= 1024;
		}
	}
	*/
}

//////////////////////////////////////////////////////////////////////////
//自殺封包
void NetCli_Other::SL_Suicide			( int Type )
{
	PG_Other_CtoL_Suicide Send;
	Send.Type = Type;
	SendToLocal( sizeof(Send) , &Send );
}

void NetCli_Other::SL_SpellExSkill( int TargetID , float TargetX , float TargetY , float TargetZ ,ExSkillBaseStruct& Info , int ShootAngle , int ExplodeTime )
{
	PG_Other_CtoL_SpellExSkill Send;
	Send.Info = Info;
	Send.TargetID = TargetID;
	Send.TargetX = TargetX;
	Send.TargetY = TargetY;
	Send.TargetZ = TargetZ;
	Send.ShootAngle = ShootAngle;
	Send.ExplodeTime = ExplodeTime;
	SendToLocal( sizeof(Send) , &Send );
}
//client 端事件需要記錄給營運商查詢的事件
void NetCli_Other::SL_ClientEventLog( int EventType )
{
	PG_Other_CtoL_ClientEventLog Send;
	Send.EventType = EventType;
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Other::SL_TransferData_Range( char Data[20] )
{
	PG_Other_CtoL_TransferData_Range Send;
	memcpy( Send.Data , Data , sizeof( Send.Data ) );
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Other::SL_SetRoleValue	( RoleValueName_ENUM Type , double Value )
{
	PG_Other_CtoL_SetRoleValue Send;
	Send.Type = Type;
	Send.Value = Value;
	SendToLocal( sizeof(Send) , &Send );
}

void NetCli_Other::SL_ClientLanguage( int iLanguage )
{
	PG_Other_CtoL_Client_Language Send;
	Send.iLanguage = iLanguage;
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Other::SL_SetNewTitleSys( NewTitleSysPosENUM Pos , int TitleID )
{
	PG_Other_CtoL_SetNewTitleSys Send;
	Send.Pos = Pos;
	Send.TitleID = TitleID;
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Other::SL_NewTitleSys_UseItem( int PosType , int Pos )
{
	PG_Other_CtoL_NewTitleSys_UseItem Send;
	Send.Pos = Pos;
	Send.PosType = PosType;
	SendToLocal( sizeof(Send) , &Send );
}

void NetCli_Other::SL_OpenSession(SessionTypeENUM Type, OpenSessionResultFunc func)
{
	m_SessionCBID++;

	PG_Other_CtoL_OpenSession Send;
	Send.Type = Type;
	Send.CBID = m_SessionCBID;

	if (func != NULL)
	{
		m_OpenSessionResultFunc[m_SessionCBID] = func;
	}

	SendToLocal( sizeof(Send) , &Send );
}

void NetCli_Other::SL_CloseSession(SessionTypeENUM Type, int SessionID)
{
	PG_Other_CtoL_CloseSession Send;
	Send.Type = Type;
	Send.SessionID = SessionID;
	SendToLocal( sizeof(Send) , &Send );
}

void NetCli_Other::SL_AutoReportPossibleCheater( AutoReportBaseInfoStruct& TargetInfo , AutoReportPossibleCheaterENUM Type )
{
	PG_Other_CtoL_AutoReportPossibleCheater Send;
	Send.Type = Type;
	Send.TargetInfo = TargetInfo;
	SendToLocal( sizeof(Send) , &Send );
}
//要求今天的複本
void NetCli_Other::SL_WeekInstancesRequest(  )
{
	PG_Other_CtoL_WeekInstancesRequest send;
	SendToLocal( sizeof(send) , &send );
}
//要求傳送到某副本
void NetCli_Other::SL_WeekInstancesTeleport( int zoneID )
{
	PG_Other_CtoL_WeekInstancesTeleport send;
	send.ZoneID = zoneID;
	SendToLocal( sizeof(send) , &send );
}
//要求重置某副本
void NetCli_Other::SL_WeekInstancesZoneReset( int zoneID )
{
	PG_Other_CtoL_WeekInstancesZoneReset send;
	send.ZoneID = zoneID;
	SendToLocal( sizeof(send) , &send );
}
//重置副本傳送次數
void NetCli_Other::SL_WeekInstancesReset(  )
{
	PG_Other_CtoL_WeekInstancesReset send;
	SendToLocal( sizeof(send) , &send );
}

//魔靈升階(或產生)
void NetCli_Other::SL_PhantomRankUp( int phantomID , bool summon )
{
	PG_Other_CtoL_PhantomRankUp send;
	send.PhantomID = phantomID;
	send.Summon = summon;
	SendToLocal( sizeof(send) , &send );
}
//魔靈升級
void NetCli_Other::SL_PhantomLevelUp( int phantomID )
{
	PG_Other_CtoL_PhantomLevelUp send;
	send.PhantomID = phantomID;
	SendToLocal( sizeof(send) , &send );
}
//魔靈裝備
void NetCli_Other::SL_PhantomEQ( int eq[2] )
{
	PG_Other_CtoL_PhantomEQ send;
	send.EQ[0] = eq[0];
	send.EQ[1] = eq[1];
	SendToLocal( sizeof(send) , &send );
}
