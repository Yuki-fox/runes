#include "../GameMain.h"
// #include "../interfacemember/BackPackage.h"

void CGameMain::R_GetItemOK                        ( )
{
    CBackPackage::ClsOnItemCmdProc();
}
void CGameMain::R_GetGroundItemFailed              ( )
{
    CBackPackage::ClsOnItemCmdProc();
}
void CGameMain::R_GetBufferItemFailed              ( )
{
    m_RoleData.TempData.ItemTemp.Clear();
    g_pError->AddMessage( 2, 0, "在Buffer取資料失敗 ，清除 Buffer" );
    CBackPackage::ClsOnItemCmdProc();    
}

void CGameMain::R_ClsItemInBuffer                  ( )   
{
    m_RoleData.TempData.ItemTemp.Clear();
}
void CGameMain::R_NewItemInBuffer                  ( ItemFieldStruct& Item )   
{
    m_RoleData.AddItemtoBuf( Item );
    g_pError->AddMessage( 2, 0, "收到物品 %d", Item.OrgObjID );
}

void CGameMain::R_DeleteItemOK_Body                ( )   
{
    CBackPackage::ClsOnItemCmdProc();
}
void CGameMain::R_DeleteItemOK_Bank                ( )  
{
    CBackPackage::ClsOnItemCmdProc();
}
void CGameMain::R_DeleteItemOK_EQ                  ( )   
{
    CBackPackage::ClsOnItemCmdProc();
}
void CGameMain::R_DeleteItemFailed                 ( )   
{
    CBackPackage::ClsOnItemCmdProc();
}

void CGameMain::R_ExchangeFieldOK_Body             ( )   
{
    CBackPackage::ClsOnItemCmdProc();
}
void CGameMain::R_ExchangeFieldOK_Bank             ( )   
{
    CBackPackage::ClsOnItemCmdProc();
}
void CGameMain::R_ExchangeFieldOK_BodyBank         ( )   
{
    CBackPackage::ClsOnItemCmdProc();
}
void CGameMain::R_ExchangeFieldFailed_Body         ( )   
{
    CBackPackage::ClsOnItemCmdProc();
}
void CGameMain::R_ExchangeFieldFailed_Bank         ( )   
{
    CBackPackage::ClsOnItemCmdProc();
}
void CGameMain::R_ExchangeFieldFailed_BodyBank     ( )   
{
    CBackPackage::ClsOnItemCmdProc();
}

void CGameMain::R_BodyBankMoneyExchangeOK          ( )   
{
    
}
void CGameMain::R_BodyBankMoneyExchangeFalse       ( )   
{

}
void CGameMain::R_FixItemInfo_Body                 ( ItemFieldStruct& Item , int Pos )    
{   
    m_RoleData.SetItemInfo_Body( Item , Pos );
}
void CGameMain::R_FixItemInfo_Bank                 ( ItemFieldStruct& Item , int Pos )    
{
    m_RoleData.SetItemInfo_Bank( Item , Pos );
}
void CGameMain::R_FixItemInfo_EQ                   ( ItemFieldStruct& Item , int Pos )    
{
    m_RoleData.SetItemInfo_EQ( Item , Pos );
}

void CGameMain::R_FixAllItemInfo                   ( ItemStruct& Body , ItemStruct& Bank , EQStruct& EQ )   
{
    m_RoleData.BaseData.Body = Body;
    m_RoleData.BaseData.Bank = Bank;
    m_RoleData.BaseData.EQ   = EQ;
    m_RoleData.TempData.UpdateInfo.Bank = true;
    m_RoleData.TempData.UpdateInfo.Body = true;
    m_RoleData.TempData.UpdateInfo.Eq  = true;
}
void CGameMain::R_FixMoney                         ( int BodyMoney , int BankMoney )    
{
    m_RoleData.BaseData.Body.Money = BodyMoney;
    m_RoleData.BaseData.Bank.Money = BodyMoney;
    m_RoleData.TempData.UpdateInfo.Money = true;
}
void CGameMain::R_MaxItemCount                     ( int MaxBodyItem , int MaxBankItem )   
{
    m_RoleData.BaseData.Body.Count = MaxBodyItem;
    m_RoleData.BaseData.Bank.Count = MaxBankItem;
}
void CGameMain::R_OpenBank                         ( )    
{

}
void CGameMain::R_UseItemOK                        ( int Pos )
{
    CBackPackage::ClsOnItemCmdProc();   
}
void CGameMain::R_UseItemFailed                     ( int Pos )
{
    CBackPackage::ClsOnItemCmdProc();
    ItemFieldStruct* Item = m_RoleData.GetBodyItem( Pos );
    if( Item == NULL )
        return;

    if(     Item->Pos == EM_USE_ITEM 
        ||  Item->Pos == EM_ItemState_CliDisabled    )
    {
        Item->Pos = EM_ItemState_NONE;
        m_RoleData.TempData.UpdateInfo.Body = true;
    }
    g_pError->AddMessage( 0, 0, "物品使用失敗");
}