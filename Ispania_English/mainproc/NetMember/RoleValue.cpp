#include "../GameMain.h"

void CGameMain::R_FixAllRoleValue   ( BaseAbilityStruct& Ability )
{

}
void CGameMain::R_FixRoleValue      ( RoleValueName_ENUM ValueName , float Value )
{
     RoleDataEx*    Role    = g_pGameMain->RoleData();
     int        OrgValue = Role->GetValue( ValueName );
     Role->SetValue( ValueName , (int)Value );

     int        DValue = (int)Value - OrgValue;

     if( DValue == 0 )
         return;


     if( ValueName >= EM_RoleValue_SPMagic && ValueName < EM_RoleValue_SPMagic + _MAX_SPSkill_COUNT_ )
     {
        int SkillPos = ValueName - EM_RoleValue_SPMagic;
        g_pError->AddMessage( 2, 0, "�Ƿ|�ޯ� SPSkillID(%d) SPSkillPos(%d) LV(%d)" , Role->BaseData.Skill.SP[ SkillPos ].SkillID , SkillPos , (int)Value );        
     }
     else if( ValueName >= EM_RoleValue_NormalMagic && ValueName < EM_RoleValue_NormalMagic + _MAX_NormalSkill_COUNT_ )
     {
         int MagicPos = ValueName - EM_RoleValue_NormalMagic;
         g_pError->AddMessage( 2, 0, "�Ƿ|�ޯ� MagicID(%d) MagicPos(%d) LV(%d)" , Role->BaseData.Skill.MainJob[ MagicPos ].SkillID , MagicPos , (int)Value );        
     }
     else switch( ValueName )
     {
     case EM_RoleValue_LV:
        if( DValue >= 1 )
        {  
            g_pError->AddMessage( 2, 0, "���ŤW�ɬ�%d" , (int)Value );
        }
        else
        {
            g_pError->AddMessage( 2, 0, "���ŤU����%d" , (int)Value );
        }
        break;
     case EM_RoleValue_EXP:
        if( DValue > 0  )
        {  
            g_pError->AddMessage( 2, 0, "�o��g��� %d" , (int)DValue );
        }
        else
        {
            g_pError->AddMessage( 2, 0, "���h�g��� %d" , (int)DValue*-1 );
        }
        break;
     case EM_RoleValue_Point:
        g_pError->AddMessage( 2, 0, "�ثe�i�t�I�� %d" , (int)Value );
        break;
     case EM_RoleValue_SkillPoint:
        g_pError->AddMessage( 2, 0, "�ثe�i�t �k�N �I�� %d" , (int)Value );
        break;
     }
     
}
void CGameMain::R_SetPoint_OK       ( )
{

}
void CGameMain::R_SetPoint_Failed   ( )
{

}
void CGameMain::R_NotifyLvUp        ( int ItemID , int Level )
{
    g_pError->AddMessage( 2, 0, "GItemID(%d)  ���ŤW�ɬ�%d" , ItemID , Level );
}
void CGameMain::R_SetTitleFlag      ( int TitleID , bool Flag )
{

}
void CGameMain::R_SetFlag           ( int FlagID , bool Flag )
{

}
void CGameMain::R_SetTitle_OK       ( int TitleID )
{

}
void CGameMain::R_SetTitle_Failed   ( )
{

}
void CGameMain::R_Title             ( int ItemID , int TitleID )
{

}
