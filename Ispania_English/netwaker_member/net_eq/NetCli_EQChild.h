#pragma once

#include "NetCli_EQ.h"

class NetCli_EQChild : public NetCli_EQ
{
public:
	static bool _Init();
	static bool _Release();

	static void	S_EQ2Body( ItemFieldStruct& Item , int ItemPos , int EqPos );

	virtual void R_SetEQOK();
	virtual void R_SetEQFailed();
	virtual void R_EQBroken( EQWearPosENUM Pos );
	virtual void R_SetShowEQ( int GItemID , ShowEQStruct ShowEQ );
	virtual void R_SwapBackupEQ( int BackupPosID , bool Result );

	//®M¸Ë¨t²Î
	virtual void R_LearnSuitSkill_Open( int TargetID );
	virtual void R_LearnSuitSkillResult( int LearnSkillID , LearnSuitSkillResultENUM Result );
	virtual void R_SetSuitSkillResult( SetSuitSkillResultENUM Result );
	virtual void R_UseSuitSkillResult( UseSuitSkillResultENUM Result );

};
