#define _WIN32_WINNT 0x0500
#define _WIN32_WINDOWS 0x0400

#include <windows.h>
#include "../../tools/getfontnamefromfile/getfontnamefromfile.h"
#include "../mainproc/GameMain.h"
#include "ScrollCombatTextFrame.h"
#include "../../ErrorList/errorlist.h"

CScrollCombatTextFrame*			g_pScrollCombatTextFrame = NULL;

// ----------------------------------------------------------------------------
static void ScrollCombatTextFrame_OnPaint(CUiFrame* frame, INT64 lParam, INT64 rParam)
{
	g_pScrollCombatTextFrame->OnPaint(frame, (int)lParam, (int)rParam);
}

// ----------------------------------------------------------------------------
CScrollCombatTextFrame::CScrollCombatTextFrame(CInterface* object)
	: CInterfaceBase(object)
{
	g_pScrollCombatTextFrame = this;

	m_display.clear();
	m_fontType		= NULL;	
	m_shadow.color	= 0xFF000000;
	m_shadow.x		= 2;
	m_shadow.y		= 2;
}

// ----------------------------------------------------------------------------
CScrollCombatTextFrame::~CScrollCombatTextFrame()
{
	DestroyFrames();
}

// ----------------------------------------------------------------------------
void CScrollCombatTextFrame::RegisterFunc()
{
}

// ----------------------------------------------------------------------------
void CScrollCombatTextFrame::BuildFrames()
{
	if ( m_interface && m_interface->GetUiWorld() )
	{
		string resource = m_interface->GetResourcePath();
		string fullname = resource + g_pGameMain->GetDefaultFontPath();
		AddFontResourceEx( fullname.c_str(), FR_PRIVATE, 0 );

		string fontname = GetFontNameFromFile( fullname.c_str() );

		if ( !fontname.empty() ) {
			m_fontType = CRuFontType::GetFontType(fontname.c_str(), 32, ruFONTWEIGHT_NORMAL, ruFONTOUTLINE_NORMAL);
		}

		m_interface->GetUiWorld()->GetRoot()->m_pfOnPaint = ScrollCombatTextFrame_OnPaint;
	}
}

// ----------------------------------------------------------------------------
void CScrollCombatTextFrame::DestroyFrames()
{
	for (list<SCTDisplay>::iterator iter = m_display.begin(); iter != m_display.end(); iter++)
		UI_SAFE_DELETE(iter->text);
	m_display.clear();
}

// ----------------------------------------------------------------------------
void CScrollCombatTextFrame::OnPaint(CUiFrame* frame, int lParam, int rParam)
{
	for (list<SCTDisplay>::iterator iter = m_display.begin(); iter != m_display.end(); iter++)
	{
		(*iter).text->Render(lParam / 100.0f);
	}
}

// ----------------------------------------------------------------------------
void CScrollCombatTextFrame::Update(float elapsedTime)
{
	// SCT Info
	UpdateQueue(elapsedTime);

	// SCT Display
	UpdateDisplay(elapsedTime);
}

// ----------------------------------------------------------------------------
void CScrollCombatTextFrame::PushSCTInfo(SCTInfo& info)
{
	// 來源或者目標為玩家時顯示
	CRoleSprite* player = g_pGameMain->GetPlayer();
	CRoleSprite* sprite = NULL;

	// 改筆資料是否有效
	bool isInsert = g_pGameMain->GetDisplayAllSCT();	
	if ( !isInsert && player ) {
		int id = player->GetIdentify();
		if ( id == info.source || id == info.target )
			isInsert = true;
		else
		{
			sprite = g_pGameMain->FindSpriteByID(info.source);
			if ( sprite && (sprite->GetMasterID() == id || sprite == player->GetImplementSprite()) )
				isInsert = true;
			else
			{
				sprite = g_pGameMain->FindSpriteByID(info.target);
				if ( sprite && sprite->GetMasterID() == id )
					isInsert = true;
			}
		}
	}

	if ( !isInsert )
		return;

	if ( sprite && (!sprite->IsVisible() || sprite->IsScriptHide()) )
		return;

	// g_pError->AddMessage(0, 0, "Push SCTInfo Begin");

	SctQueue::iterator iter = m_queue.find(info.target);
	if ( iter != m_queue.end() ) {
		SctItem& item = iter->second;
		item.push_back(info);
	} else {
		SctItem item;
		item.push_back(info);
		m_queue.insert(make_pair(info.target, item));
	}

	// g_pError->AddMessage(0, 0, "Push SCTInfo End");
}

// ----------------------------------------------------------------------------
void CScrollCombatTextFrame::UpdateQueue(float elapsedTime)
{
	static float updateTime = 0.0f;
	
	updateTime += elapsedTime;
	if ( updateTime > 0.2f ) {
		updateTime = 0.0f;
		return;
	}

	SctQueue::iterator iter = m_queue.begin();
	while (iter != m_queue.end()) {
		SctItem& item = iter->second;
		UpdateItem(item, elapsedTime);

		if ( item.empty() ) {
			iter = m_queue.erase(iter);
		} else {
			iter++;
		}
	}

	//g_pError->AddMessage(0, 0, "Update SCTInfo End");
}

// ----------------------------------------------------------------------------
void CScrollCombatTextFrame::UpdateItem(SctItem& item, float elapsedTime)
{
	static float offset[7] = { 0.0f, 12.0f, -12.0f, 24.0f, -24.0f, 36.0f, -36.0f };

	int count = 0;
	int size = (int)item.size();
	SctItem::iterator iter = item.begin();
	while ( iter != item.end() ) {
		iter->delayTime -= elapsedTime;
		if ( iter->delayTime > 0.0f )  {
			iter++;
		} else if ( count > 0 && size < 8 ) {
			iter->delayTime = 0.8f / item.size();
			iter++;
		} else {
			CRoleSprite* target = g_pGameMain->FindSpriteByID(iter->target);
			CRoleSprite* player = g_pGameMain->GetPlayer();

			if (target && player)
			{	
				char temp[256];
				CRuVector3 point;
				CRuVector3 oriPt = target->GetPos();
				SCTDisplay display;

				target->GetAttachPos(eAttach_Name_Title, point);
				display.x = oriPt.m_x;
				display.y = oriPt.m_y + (point.m_y - oriPt.m_y) * 0.85f;
				display.z = oriPt.m_z;

				display.xOffset = offset[count % 7];
				if ( iter->target == player->GetIdentify() ) {
					display.mode		= eSCT_Display_Vertical;
					display.yVector		= 160.0f;
					display.xVector		= 0.0f;
				} else if ( iter->source == player->GetIdentify() )	{
					display.transform	= true;
					display.mode		= eSCT_Display_Throb;
					display.yVector		= 0.0f;
					display.xVector		= 70.0f;
				} else {
					display.transform	= true;
					display.depth		= true;
					display.mode		= eSCT_Display_Vertical;
					display.yVector		= 150.0f;
					display.xVector		= 0.0f;
				}

				switch (iter->type)
				{
				case eSCT_Show_Text:
					AddText(display, iter->text.c_str(), 0.94f, 0.4f, 0.5f);
					break;

				case eSCT_Combat_Enter:
					//display.xVector = 0.0f;
					AddText(display, g_ObjectData->GetString("SYS_ENTER_FIGHT"), 0.94f, 0.4f, 0.133f);
					break;

				case eSCT_Combat_Leave:
					//display.xVector = 0.0f;
					AddText(display, g_ObjectData->GetString("SYS_LEAVE_FIGHT"), 0.94f, 0.4f, 0.133f);
					break;

				case eSCT_Increase_EXP:
					//display.xVector = 0.0f;
					sprintf(temp, g_ObjectData->GetString("SYS_INCREASE_EXP"), iter->value);
					AddText(display, temp, 0.0f, 0.63f, 0.23f);
					break;

				case eSCT_Decrease_EXP:
					//display.xVector = 0.0f;
					sprintf(temp, g_ObjectData->GetString("SYS_DECREASE_EXP"), iter->value);
					AddText(display, temp, 0.0f, 0.63f, 0.23f);
					break;

				case eSCT_Increase_TP:
					//display.xVector = 0.0f;
					sprintf(temp, g_ObjectData->GetString("SYS_INCREASE_TP"), iter->value);
					AddText(display, temp, 0.94f, 0.62f, 0.0f);
					break;

				case eSCT_Decrease_TP:
					sprintf(temp, g_ObjectData->GetString("SYS_DECREASE_TP"), iter->value);
					AddText(display, temp, 0.94f, 0.62f, 0.0f);
					break;

					// 平常傷害
				case eSCT_Normal_Miss:
					AddText(display, g_ObjectData->GetString("SYS_MISS"), 1.0f, 0.9f, 0.45f);
					break;

				case eSCT_Normal_Dodge:
					AddText(display, g_ObjectData->GetString("SYS_DODGE"), 1.0f, 0.9f, 0.45f);
					break;

				// 獲得榮譽點數
				case eSCT_Increase_Honor:
					sprintf(temp, g_ObjectData->GetString("SYS_INCREASE_HONOR"), iter->value);
					AddText(display, temp, 0.0f, 0.63f, 0.23f);
					break;

				// 失去榮譽點數
				case eSCT_Decrease_Honor:
					sprintf(temp, g_ObjectData->GetString("SYS_DECREASE_HONOR"), iter->value);
					AddText(display, temp, 0.0f, 0.63f, 0.23f);
					break;

				case eSCT_Normal_Parry:
					sprintf(temp, g_ObjectData->GetString("SYS_PARRY"), -iter->hp);
					if ( iter->target == player->GetIdentify() )
						AddText(display, temp, 1.0f, 0.1f, 0.1f);
					else if ( iter->source == player->GetIdentify() )
						AddText(display, temp, 1.0f, 1.0f, 1.0f);
					else
						AddText(display, temp, 0.7f, 0.6f, 0.6f);
					break;

				case eSCT_Normal_Damage:
				case eSCT_Normal_Critical:
					if (iter->type == eSCT_Normal_Critical) {
						display.magnify = true;
						display.yOffset = -12.0f;
					}

					sprintf(temp, "%d", -iter->hp);
					if ( iter->target == player->GetIdentify() )
						AddText(display, temp, 1.0f, 0.1f, 0.1f);
					else if ( iter->source == player->GetIdentify() )
						AddText(display, temp, 1.0f, 1.0f, 1.0f);
					else
						AddText(display, temp, 0.7f, 0.6f, 0.6f);
					break;

				case eSCT_Normal_Absorb:
					if ( target->GetDeadState() == false )
						AddText(display, g_ObjectData->GetString("SYS_ABSORB"), 1.0f, 0.9f, 0.45f);
					break;

				case eSCT_Normal_Block:
					if ( target->GetDeadState() == false )
						AddText(display, g_ObjectData->GetString("SYS_BLOCK"), 1.0f, 0.9f, 0.45f);
					break;

					// 法術效果
				case eSCT_Magic_Miss:
					if ( target->GetDeadState() == false )
						AddText(display, g_ObjectData->GetString("SYS_MISS"), 1.0f, 0.9f, 0.45f);
					break;

				case eSCT_Magic_Damage:
				case eSCT_Magic_Critical:
					if (iter->type == eSCT_Magic_Critical) {
						display.magnify = true;
						display.yOffset = -12.0f;
					}

					// 生命
					if (iter->hp != SCT_DEFAULT_VALUE)
					{
						if (iter->hp > 0)
						{
							sprintf(temp, "+%d", iter->hp);
							AddText(display, temp, 0.67f, 0.86f, 0.45f);
						}
						else
						{
							if ( iter->source == player->GetIdentify() )
							{
								sprintf(temp, "%d", -(iter->hp));
								AddText(display, temp, 0.925f, 0.61f, 0.14f);
							}
							else if ( iter->target == player->GetIdentify() )
							{
								sprintf(temp, "%d", iter->hp);
								AddText(display, temp, 0.925f, 0.11f, 0.14f);
							}
							else
							{
								sprintf(temp, "%d", iter->hp);
								AddText(display, temp, 0.925f, 0.11f, 0.14f);
							}
						}
					}

					// 魔力
					if (iter->mp != SCT_DEFAULT_VALUE)
					{
						if (iter->mp > 0)
						{
							sprintf(temp, "+%d", iter->mp);
							AddText(display, temp, 0.0f, 0.676f, 0.933f);
						}
						else
						{
							sprintf(temp, "%d", iter->mp);
							AddText(display, temp, 0.568f, 0.0f, 0.482f);
						}
					}

					// 其他
					if (iter->sp != SCT_DEFAULT_VALUE)
					{
						if (iter->sp > 0)
						{
							sprintf(temp, "+%d", iter->sp);
							AddText(display, temp, 0.656f, 0.39f, 0.656f);
						}
						else
						{
							sprintf(temp, "%d", iter->sp);
							AddText(display, temp, 0.96f, 0.586f, 0.473f);
						}
					}						
					break;
				case eSCT_Magic_NoEffect:
					if ( target->GetDeadState() == false ) {
						if ( iter->target == player->GetIdentify() )
							AddText(display, g_ObjectData->GetString("SYS_IMMUNE"), 1.0f, 0.9f, 0.45f);
						else
							AddText(display, g_ObjectData->GetString("SYS_IMMUNE"), 0.92f, 0.0f, 0.523f);
					}
					break;

				case eSCT_Magic_Absorb:
					if ( target->GetDeadState() == false ) {
						if ( iter->target == player->GetIdentify() )
							AddText(display, g_ObjectData->GetString("SYS_ABSORB"), 1.0f, 0.9f, 0.45f);
						else
							AddText(display, g_ObjectData->GetString("SYS_ABSORB"), 0.92f, 0.0f, 0.523f);
					}
					break; 	

				case eSCT_Magic_Resist:
					if ( target->GetDeadState() == false ) {
						if ( iter->target == player->GetIdentify() )
							AddText(display, g_ObjectData->GetString("SYS_RESIST"), 1.0f, 0.9f, 0.45f);
						else
							AddText(display, g_ObjectData->GetString("SYS_RESIST"), 0.92f, 0.0f, 0.523f);
					}
					break;

				case eSCT_Magic_Dodge:
					if ( target->GetDeadState() == false ) {
						if ( iter->target == player->GetIdentify() )
							AddText(display, g_ObjectData->GetString("SYS_DODGE"), 1.0f, 0.9f, 0.45f);
						else
							AddText(display, g_ObjectData->GetString("SYS_DODGE"), 0.92f, 0.0f, 0.523f);
					}
					break;
				}
			}
			iter = item.erase(iter);
			count++;
		}
	}
}

// ----------------------------------------------------------------------------
void CScrollCombatTextFrame::UpdateDisplay(float elapsedTime)
{
	static float a1 = 380.0f;
	static float a2 = 160.0f;
	static float a3 = 80.0f;
	static float g = 960.0f;

	list<SCTDisplay>::iterator iter = m_display.begin();
	while ( iter != m_display.end() ) {
		SCTDisplay& sct = *iter;
		if ( sct.time > 1.75f )	{
			UI_SAFE_DELETE(sct.text);
			iter = m_display.erase(iter);
		} else {
			iter++;
			sct.time += elapsedTime;

			CRuVector3 point(sct.x, sct.y, sct.z);
			float x = 0.0f;
			float y = 0.0f;
			float scale = 1.0f;
			CUiPoint nowPos;
			
			if ( sct.transform )
			{
				CRuCamera* camera = g_pGameMain->GetCamera();
				CRuVector3 testPoint = point;

				// 測試景深
				camera->GetCameraTransform().TransformPoint(testPoint);
				if ( testPoint.m_z > 1.0f )	{
					sct.text->SetVisible(true);

					// 轉換為2D座標
					g_pGameMain->WorldToViewport(point);

					if ( sct.depth )
					{
						scale = 1.2f - testPoint.m_z * 0.003f;
						if ( scale < 0.25f )
							sct.text->SetVisible(false);
					}
				} else {
					sct.text->SetVisible(false);
				}
			}

			switch(sct.mode)
			{
			case eSCT_Display_Vertical:				
				// x = sct.xVector;
				y = -sct.yVector * sct.time + 0.5f * sct.time * sct.yVector;
				break;

			case eSCT_Display_Throb:
				if (sct.time < a1/g)
				{
					float time = sct.time;
					y = -a1 * time + 0.5f * g * time * time;
				}
				else if (sct.time < 2*a1/g)
				{
					float time = sct.time - a1 / g;
					float h = a1 * a1 * 0.5f / g;
					y = 0.5f * g * time * time - h;
				}
				else if (sct.time < 2*a1/g+a2/g)
				{
					float time = sct.time - 2*a1/g;
					y = -a2 * time + 0.5f * g * time * time;
				}				
				else if (sct.time < 2*a1/g+2*a2/g)
				{
					float time = sct.time - (2*a1/g + a2/g);
					float h = a2 * a2 * 0.5f / g;
					y = 0.5f * g * time * time - h;
				}				
				else if (sct.time < 2*a1/g+2*a2/g+a3/g)
				{
					float time = sct.time - (2*a1/g+2*a2/g);
					y = -a3 * time + 0.5f * g * time * time;
				}
				else if (sct.time < 2*a1/g+2*a2/g+2*a3/g)
				{
					float time = sct.time - (2*a1/g+2*a2/g+a3/g);
					float h = a3 * a3 * 0.5f / g;
					y = 0.5f * g * time * time - h;
				}
				else
					y = 0.0f;

				x = sct.xVector * sct.time;
				break;
			}

			// 淡出
			if (sct.time > 0.75f)
				sct.text->SetAlpha(1.75f - sct.time);

			// 放大(特殊顯眼表現)
			if (sct.magnify)
			{
				if (sct.time < 0.2f)
					scale *= 0.6f + sct.time * 8.0f;
				else if (sct.time < 0.4f)
					scale *= 2.2f - (sct.time - 0.2f) * 6.0f;
				else
					scale *= 1.0f;
			}
			sct.text->SetScale(scale);
			nowPos.m_x = point.m_x - (sct.text->GetWidth() * sct.text->GetScale() / 2.0f) + (x + sct.xOffset);
			nowPos.m_y = point.m_y + (y + sct.yOffset);
			sct.text->SetPos(nowPos);
		}		
	}
}

// ----------------------------------------------------------------------------
void CScrollCombatTextFrame::AddText(SCTDisplay display, const char* text, float r, float g, float b)
{
	if (text)
	{
		if ( !display.transform )
		{
			CRuCamera* camera = g_pGameMain->GetCamera();			
			CRuVector3 testVec, transVec;

			testVec.m_x = display.x;
			testVec.m_y = display.y;
			testVec.m_z = display.z;

			// 測試景深
			camera->GetCameraTransform().TransformPoint(testVec);
			if ( testVec.m_z < 1.0f )
				return;

			// 轉換為2D座標
			transVec.m_x = display.x;
			transVec.m_y = display.y;
			transVec.m_z = display.z;
			g_pGameMain->WorldToViewport(transVec);

			display.x = transVec.m_x;
			display.y = transVec.m_y;
			display.z = transVec.m_z;
		}

		display.time = 0.0f;
		display.text = new CUiFontString(NULL, NULL, NULL);
		display.text->SetFontType(m_fontType);
		display.text->SetFontShadow(&m_shadow);		
		display.text->SetSpacing(0.0f);
		display.text->SetAnchorSize(CUiSize(0.0f, 0.0f));
		display.text->SetColor(r, g, b);
		display.text->SetTextA(text);
		display.text->SetAlpha(1.0f);
		display.text->SetScale(0.75f);		
		m_display.push_back(display);
	}
}