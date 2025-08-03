#include "..\GameMain.h"
#include "../InterfaceMember/ChatFrame.h"
#include "../interfacemember/UI_Trade.h"
#include "../interfacemember/BackPackage.h"

void CGameMain::R_RequestTrade          ( int TargetID )
{
    char Buf[256];
    sprintf( Buf , "%d要求交易" , TargetID );
}
void CGameMain::R_OpenTrade             ( char* Name , int TargetID )
{
    CUI_Trade::OpenTradeMenu( TargetID );
}
void CGameMain::R_TargetPutItem         ( int Pos , ItemFieldStruct& Item )
{
    CUI_Trade::SetTargetTradeItem( Pos , Item );

}
void CGameMain::R_TargetPutMoney        ( int Money )
{
    CUI_Trade::SetTargetTradeMoney( Money );
}
void CGameMain::R_OwnerPutMoney         ( int Money )
{
    CUI_Trade::SetMyTradeMoney( Money );
}
void CGameMain::R_TargetItemOK          ( )
{
    CUI_Trade::IsTargetTradeSelectOK( true );
}
void CGameMain::R_CancelItemOK          ( )
{
    CUI_Trade::IsTargetTradeSelectOK( false );
    CUI_Trade::IsMyTradeSelectOK( false );
}
void CGameMain::R_FinalOKEnable         ( )
{

}
void CGameMain::R_TargetFinalOK         ( )
{
//    CUI_Trade::IsTargetTradeOK( true );
}
void CGameMain::R_ClearTradeItem        ( )
{
    RoleDataEx* Role    = g_pGameMain->RoleData();
    CUI_Trade::CloseTradeMenu();
    Role->DelAllTradeItem();
    
}
void CGameMain::R_TradeItem_OK          ( )
{
    RoleDataEx* Role    = g_pGameMain->RoleData();
    CUI_Trade::CloseTradeMenu();
    Role->ClsAllTradeItem();
}
void CGameMain::R_TradeItem_Failed      ( )
{
    RoleDataEx* Role    = g_pGameMain->RoleData();
    CUI_Trade::CloseTradeMenu();
    Role->ClsAllTradeItem();
}
void CGameMain::R_PutItem_OK            ( )
{
    CBackPackage::ClsOnItemCmdProc();
}
void CGameMain::R_PutItem_Failed        ( )
{
    CBackPackage::ClsOnItemCmdProc();
}