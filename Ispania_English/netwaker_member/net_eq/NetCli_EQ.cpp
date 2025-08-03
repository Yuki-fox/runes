#include "NetCli_EQ.h"
#include "../../MainProc/NetGlobal.h"
//-------------------------------------------------------------------
NetCli_EQ*    NetCli_EQ::This         = NULL;

//-------------------------------------------------------------------
bool NetCli_EQ::_Init()
{
    //_Net.RegOnSrvPacketFunction	( EM_PG_EQ_LtoC_SetEQResult  , _PG_EQ_LtoC_SetEQResult );
    Cli_NetReg( PG_EQ_LtoC_SetEQResult	);
	Cli_NetReg( PG_EQ_LtoC_EQBroken		);
	Cli_NetReg( PG_EQ_LtoC_SetShowEQ	);
	Cli_NetReg( PG_EQ_LtoC_SwapBackupEQ	);

	Cli_NetReg( PG_EQ_LtoC_LearnSuitSkill_Open	);
	Cli_NetReg( PG_EQ_LtoC_LearnSuitSkillResult	);
	Cli_NetReg( PG_EQ_LtoC_SetSuitSkillResult	);
	Cli_NetReg( PG_EQ_LtoC_UseSuitSkillResult	);
    return true;
}
//-------------------------------------------------------------------
bool NetCli_EQ::_Release()
{
    return true;
}
//-------------------------------------------------------------------
void NetCli_EQ::_PG_EQ_LtoC_LearnSuitSkill_Open	( int Sockid , int Size , void* Data )
{
	PG_EQ_LtoC_LearnSuitSkill_Open*  PG = (PG_EQ_LtoC_LearnSuitSkill_Open*)Data;
	This->R_LearnSuitSkill_Open( PG->TargetID );
}
void NetCli_EQ::_PG_EQ_LtoC_LearnSuitSkillResult( int Sockid , int Size , void* Data )
{
	PG_EQ_LtoC_LearnSuitSkillResult*  PG = (PG_EQ_LtoC_LearnSuitSkillResult*)Data;
	This->R_LearnSuitSkillResult( PG->LearnSkillID , PG->Result );
}
void NetCli_EQ::_PG_EQ_LtoC_SetSuitSkillResult	( int Sockid , int Size , void* Data )
{
	PG_EQ_LtoC_SetSuitSkillResult*  PG = (PG_EQ_LtoC_SetSuitSkillResult*)Data;
	This->R_SetSuitSkillResult( PG->Result );
}
void NetCli_EQ::_PG_EQ_LtoC_UseSuitSkillResult	( int Sockid , int Size , void* Data )
{
	PG_EQ_LtoC_UseSuitSkillResult*  PG = (PG_EQ_LtoC_UseSuitSkillResult*)Data;
	This->R_UseSuitSkillResult( PG->Result );
}

void NetCli_EQ::_PG_EQ_LtoC_SwapBackupEQ( int Sockid , int Size , void* Data )
{
	PG_EQ_LtoC_SwapBackupEQ*  PG = (PG_EQ_LtoC_SwapBackupEQ*)Data;
	This->R_SwapBackupEQ( PG->BackupPosID , PG->Result );
}
void NetCli_EQ::_PG_EQ_LtoC_SetShowEQ	( int Sockid , int Size , void* Data )
{
	PG_EQ_LtoC_SetShowEQ*  PG = (PG_EQ_LtoC_SetShowEQ*)Data;
	This->R_SetShowEQ( PG->GItemID , PG->ShowEQ );
	
}
void NetCli_EQ::_PG_EQ_LtoC_EQBroken	( int Sockid , int Size , void* Data )
{
	PG_EQ_LtoC_EQBroken*  PG = (PG_EQ_LtoC_EQBroken*)Data;
	This->R_EQBroken( PG->Pos );
}
void NetCli_EQ::_PG_EQ_LtoC_SetEQResult ( int Sockid , int Size , void* Data )
{
    PG_EQ_LtoC_SetEQResult*  PG = (PG_EQ_LtoC_SetEQResult*)Data;
    switch( PG->Result )
    {
    case ENMU_EQ_SetEqOK:
        This->R_SetEQOK(  );
        break;
    case ENMU_EQ_SetEqFailed:
        This->R_SetEQFailed( );
        break;
    }    
}
//-------------------------------------------------------------------
void NetCli_EQ::S_SetEQ_Body( ItemFieldStruct& Item , int ItemPos , int EqPos , bool IsFromEq )
{
    PG_EQ_CtoL_SetEQ Send;
    Send.Item = Item;
    Send.ItemPos = ItemPos;
    Send.EqPos = (EQWearPosENUM)EqPos;
    Send.PosType = Def_Item_Pos_Body;
	Send.IsFromEq = IsFromEq;

    SendToLocal( sizeof(Send) , &Send );
}

void NetCli_EQ::S_SetEQ_Bank( ItemFieldStruct& Item , int ItemPos , int EqPos , bool IsFromEq )
{
    PG_EQ_CtoL_SetEQ Send;
    Send.Item = Item;
    Send.ItemPos = ItemPos;
    Send.EqPos = (EQWearPosENUM)EqPos;
    Send.PosType = Def_Item_Pos_Bank;
	Send.IsFromEq = IsFromEq;

    SendToLocal( sizeof(Send) , &Send );
}
//-------------------------------------------------------------------
void NetCli_EQ::S_SetEQ_EQ( ItemFieldStruct& Item , int ItemPos , int EqPos )
{
    PG_EQ_CtoL_SetEQ Send;
    Send.Item = Item;
    Send.ItemPos = ItemPos;
    Send.EqPos = (EQWearPosENUM)EqPos;
    Send.PosType = Def_Item_Pos_EQ;

    SendToLocal( sizeof(Send) , &Send );
}
//-------------------------------------------------------------------
void NetCli_EQ::S_SetShowEQ( ShowEQStruct ShowEQ )
{
	PG_EQ_CtoL_SetShowEQ Send;
	Send.ShowEQ = ShowEQ;
	SendToLocal( sizeof(Send) , &Send );
}

void NetCli_EQ::S_SwapBackupEQ( int BackupPosID )
{
	PG_EQ_CtoL_SwapBackupEQ Send;
	Send.BackupPosID = BackupPosID;
	SendToLocal( sizeof(Send) , &Send );
}

void NetCli_EQ::S_LearnSuitSkill_Close	()
{
	PG_EQ_CtoL_LearnSuitSkill_Close Send;
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_EQ::S_LearnSuitSkillRequest	( vector<int> ItemPosList , int LearnSkillID )
{
	if( ItemPosList.size() <=0 || ItemPosList.size() > 10 )
		return ;

	PG_EQ_CtoL_LearnSuitSkillRequest Send;
	
	Send.ItemCount = (int)ItemPosList.size() ;		//套裝數量		
	for( int i = 0 ; i < ItemPosList.size() ; i++ )
		Send.ItemPos[i] = ItemPosList[i];

	Send.LearnSkillID = LearnSkillID;			//學習的技能
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_EQ::S_SetSuitSkillRequest	( int SkillID , int Pos )
{
	PG_EQ_CtoL_SetSuitSkillRequest Send;
	Send.SkillID = SkillID;
	Send.Pos = Pos;
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_EQ::S_UseSuitSkillRequest	( int SkillID , int TargetID , float PosX , float PosY , float PosZ )
{
	PG_EQ_CtoL_UseSuitSkillRequest Send;
	Send.SkillID = SkillID;
	Send.TargetID = TargetID;
	Send.PosX = PosX; 
	Send.PosY = PosY;
	Send.PosZ = PosZ;


	SendToLocal( sizeof(Send) , &Send );

}