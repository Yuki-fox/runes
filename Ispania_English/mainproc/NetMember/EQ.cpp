#include "../GameMain.h"
#include "../interfacemember/BackPackage.h"

void CGameMain::R_SetEQOK( )
{
    CBackPackage::ClsOnItemCmdProc();
}
void CGameMain::R_SetEQFailed( )
{
    CBackPackage::ClsOnItemCmdProc();
}