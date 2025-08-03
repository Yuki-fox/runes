#pragma once

#include "PG/PG_EQ.h"
#include "../../MainProc/NetGlobal.h"

class NetCli_EQ : public CNetGlobal
{
protected:
    //-------------------------------------------------------------------
    static NetCli_EQ* This;
    static bool _Init();
    static bool _Release();
    //-------------------------------------------------------------------
    static void _PG_EQ_LtoC_SetEQResult 		( int Sockid , int Size , void* Data );   
    static void _PG_EQ_LtoC_EQBroken			( int Sockid , int Size , void* Data );   
	static void _PG_EQ_LtoC_SetShowEQ			( int Sockid , int Size , void* Data );   
	static void _PG_EQ_LtoC_SwapBackupEQ		( int Sockid , int Size , void* Data );   

	static void _PG_EQ_LtoC_LearnSuitSkill_Open	( int Sockid , int Size , void* Data );
	static void _PG_EQ_LtoC_LearnSuitSkillResult( int Sockid , int Size , void* Data );
	static void _PG_EQ_LtoC_SetSuitSkillResult	( int Sockid , int Size , void* Data );
	static void _PG_EQ_LtoC_UseSuitSkillResult	( int Sockid , int Size , void* Data );
public:    
    NetCli_EQ() { This = this; }

    static void S_SetEQ_Body( ItemFieldStruct& Item , int ItemPos , int EqPos , bool IsFromEq );
    static void S_SetEQ_Bank( ItemFieldStruct& Item , int ItemPos , int EqPos , bool IsFromEq );
    static void S_SetEQ_EQ  ( ItemFieldStruct& Item , int ItemPos , int EqPos );
	static void	S_SetShowEQ( ShowEQStruct ShowEQ );
	static void	S_SwapBackupEQ( int BackupPosID );

    virtual void R_SetEQOK( ) = 0;
    virtual void R_SetEQFailed( ) = 0;
	virtual void R_EQBroken( EQWearPosENUM Pos ) = 0;
	virtual void R_SetShowEQ( int GItemID , ShowEQStruct ShowEQ ) = 0;
	virtual void R_SwapBackupEQ( int BackupPosID , bool Result ) = 0;

	//////////////////////////////////////////////////////////////////////////
	//®M¸Ë¨t²Î
	virtual void R_LearnSuitSkill_Open( int TargetID ) = 0;
	virtual void R_LearnSuitSkillResult( int LearnSkillID , LearnSuitSkillResultENUM Result ) = 0;
	virtual void R_SetSuitSkillResult( SetSuitSkillResultENUM Result ) = 0;
	virtual void R_UseSuitSkillResult( UseSuitSkillResultENUM Result ) = 0;


	static void S_LearnSuitSkill_Close	();
	static void S_LearnSuitSkillRequest	( vector<int> ItemPosList , int LearnSkillID );
	static void S_SetSuitSkillRequest	( int SkillID , int Pos );
	static void	S_UseSuitSkillRequest	( int SkillID , int TargetID , float PosX , float PosY , float PosZ );

};
