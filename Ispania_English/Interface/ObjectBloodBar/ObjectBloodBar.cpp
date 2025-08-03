#include "ObjectBloodBar.h"
#include "..\mainproc\GameMain.h"
#include "..\sprite\sprite.h"
#include "..\..\ui\core\Luatable.h"
#include "..\WorldFrames\RaidTargetFrame.h"
//#include "..\RaidTargetFrame.h"
//#include "../tools/CIni/Ini.h"

ObjectBloodBar*		g_pObjectBloodBar;
#define BLOODBAR_DISTANCE 180
#define BLOODBAR_MAX 40
#define BLOODBAR_HEIGHT 44
#define BLOODBAR_WIDTH 140
//#define GAME_SETTINGS_FILENAME	"GameSettings.ini"
//#define GC_MAINSCTION			"Game" 
//#define GC_BAGSCTION			"Bag"



int LuaFunc_OBB_ChangeTraget(lua_State* L);

static void _GetNameString(char *temp,int visableFlag,CRoleSprite*obj)
{
	LPCSTR name=obj->GetName();

	temp[0]=0;

	strcat(temp,name);

//	int visableFlag=GetVisable();
	if (visableFlag&OBB_TITLE_FLAG)
	{
		LPCSTR title=NULL;
		if (obj->GetTitleVisible()&&obj->GetTitleText())
			 title=obj->GetTitleText();
		LPCSTR guild=NULL;
		if (obj->GetGuildVisible()&&obj->GetGuildText())
			guild=obj->GetGuildText();
	
		if ((guild&&strlen(guild)>0)||(title&& strlen(title)>0))
			strcat(temp,"\n");
		if (guild&&strlen(guild)>0)
		{						
			//strcat(temp,"|cffffffff[|r");
			strcat(temp,"[");
			strcat(temp,guild);
			strcat(temp,"]");
			//strcat(temp,"|cffffffff]|r");
		}
		if (title&&strlen(title)>0)		
		{
			if (guild&&strlen(guild)>0)
				strcat(temp,"-");
			strcat(temp,title);
			//strcat(temp,"|r");
		}
	}

	//if (strlen(temp)>0)
		//strcat(temp,"\n");

	//strcat(temp,name);
}

struct ScreenStrip
{

	int m_index;
	int m_sort;
	bool m_used;
	CUiPoint m_screen;
	int m_posX;
	int m_posY;	
	float m_scale;
	float m_allScale;
	bool m_fixed;
	

	//int m_ipren;
	//int m_inext;
	//ScreenStrip* m_pren;
	//ScreenStrip* m_next;
	ScreenStrip():m_scale(1),m_allScale(1),m_used(false),m_fixed(false)//m_ipren(-1),m_inext(-1),
	{

	}
};





struct SNpcInfo
{
//	ScreenStrip* m_ScreenBar;
	//CRoleSprite *m_Sprite;
	int m_SpriteId;
	int m_type;
	int m_flag;
	int m_index;
	float m_z;
	int m_dis;
	SNpcInfo():m_index(-1){}


};
bool CanSeeYou(CRoleSprite* sprite ,float& z)
{
	if ( sprite == NULL )
		return false;


	int visible=false;

	int hpmax=sprite->GetMaxHP();
	int hp=sprite->GetHP();
	bool life=true;
	if (hpmax<=0 ||hp<=0)
		life=false;

	if (sprite->IsVisible()==false)
		return false;

	if (sprite->IsBloodBarHidden())
		return false;

	if (sprite->GetTargetState()&&sprite->GetCursorState()&&life&&!sprite->GetSneakHiddenState())
		visible=true;

	if (sprite==g_pGameMain->GetPlayer())

		visible=true;
	

	CRuVector3 position = sprite->GetPos();
	CRuCamera* cam=g_pGameMain->GetCamera();
	cam->GetCameraTransform().TransformPoint(position);
	if (position.m_z > 0 && visible)
	{
		z=position.m_z ;
		return true;
	}



	return false;
}
/*
float CalSeeYou(CRoleSprite* sprite)
{
CRuVector3 V1=player->GetPos();
CRuVector3 V2=obj->GetPos();
CRuVector3 V3=player->GetDirection();

float ptheta = acos( V3.Dot( CRuVector3(1.0f, 0.0f, 0.0f) ) );
if ( V3.m_z > 0.0f )
ptheta = PI + PI - ptheta;

// 轉換為角度
ptheta = ptheta * 180.0f / PI;

CRuVector3 dir		= V1 - V2;

float theta = acos( dir.Dot( CRuVector3(1.0f, 0.0f, 0.0f) ) );
if ( dir.m_z > 0.0f )
theta = PI + PI - theta;

// 轉換為角度
theta = theta * 180.0f / PI;

return theta-ptheta;
}
*/

class StripManager
{
public:
	StripManager():m_debugCount(0)//,m_ifirst(-1),m_itail(-1)
	{	
	}
	~StripManager()
	{
		//m_first=NULL;
		//m_tail=NULL;
	//	m_ifirst=-1;
	//	m_itail=-1;


		//m_RectPool.clear();
	}
//	void AddBar(ScreenStrip* bar);
	void AddBar(int index);
	void Remove(int index);

	void CalBarPos(void);
	void calBarPos(int index);
//	void Remove(ScreenStrip* bar);
	void Clear();

	ScreenStrip* getBar(int index)
	{
		ScreenStrip* ret=NULL;

		if (index>=0&&index<BLOODBAR_MAX)
		{
			ret=&m_List[index];

		}
		return ret;
	}


	void SortList(void);
private:

	//int m_ifirst;
	//int m_itail;


	//int m_sortList[BLOODBAR_MAX];

	ScreenStrip m_List[BLOODBAR_MAX];
	ScreenStrip* m_sortList[BLOODBAR_MAX];
//	ScreenStrip* m_tail;
	int m_width;
	int m_height;
	int m_debugCount;
	//map<int ,ScreenStrip*> m_RectPool;

};
bool	CompYDistance( ScreenStrip* a, ScreenStrip* b )
{

	
	return   a->m_posY > b->m_posY ;

}
void StripManager::SortList(void)
{
 	int i;
	int count=0;

 	for (i=0;i<BLOODBAR_MAX;i++)
 	{		
		ScreenStrip* pObj=&m_List[i];
		if (pObj->m_used)
		{
			pObj->m_posX=(int )(pObj->m_posX/pObj->m_allScale);
			pObj->m_posY=(int )(pObj->m_posY/pObj->m_allScale);
			m_sortList[count]=pObj;
			count++;
		}

 	}


	//根據Y排序.
	std::sort( &m_sortList[0] , &m_sortList[count], CompYDistance );

}

void StripManager::AddBar(int index)
{
	if (index>=0&&index<BLOODBAR_MAX)
	{		
		ScreenStrip&pobj= m_List[index];
		pobj.m_used=true;
		m_debugCount++;
	}

}
void StripManager::Remove(int index)
{
	if (index>=0&&index<BLOODBAR_MAX)
	{

		ScreenStrip&pobj= m_List[index];
		pobj.m_used=false;

		m_debugCount--;

		
	}

}
/*

void StripManager::Remove(ScreenStrip* bar)
{
	ScreenStrip* pren=bar->m_pren;
	ScreenStrip* next=bar->m_next;
	if (pren)
		pren->m_next=next;
	else
		m_first=next;

	if (next)
		next->m_pren=pren;
	else
		m_tail=pren;


	m_debugCount--;
	bar->m_pren=NULL;
	bar->m_next=NULL;
}*/

void StripManager::calBarPos(	int This)
{
	

	ScreenStrip* pThis=m_sortList[This];
	


	


	float allscale=pThis->m_allScale;

	int width=(int )(m_width/allscale);
	int height=(int )(m_height/allscale);

	int x=pThis->m_posX;//allscale;;
	float t64=64*allscale;
	float t16=16*allscale;
		int y=pThis->m_posY;//allscale;;
		if (x<t64)
			x=(int )t64;
		if (x>(width-t64))
			x=(int )(width-t64);



		if (y<t16)
			y=(int )t16;
		if (y>(height-t16))
			y=(int )(height-t16);
	
	if (!pThis->m_fixed)
	for (int i=0;i<m_debugCount;i++) 
	{
		
		if (i==This)
			break;		

		ScreenStrip* pObj=m_sortList[i];



		//allscale=pObj.m_scale;

		int disX=(int )(pObj->m_screen.m_x-x);
		int disY=(int )(pObj->m_screen.m_y-y);
		int type=0;
		float sHeight=BLOODBAR_HEIGHT*1.0f;
		float sWidth=BLOODBAR_WIDTH*1.0f;

		
		if (disY<0)
		{

			if (disX<0)
			{
				if (disY>-sHeight&& disX>-sWidth)
						y-=(int)(sHeight+disY);

			}
			else if (disX>0)
			{

				if (disY>-sHeight&& disX<sWidth)
						y-=(int)(sHeight+disY);
			}
			else
			{
				if (disY>-sHeight)
					y-=(int)(sHeight+disY);

			}

			

		}		
		else if (disY>0)
		{
			if (disX<0)
			{
				if (disY<sHeight&& disX>-sWidth)
						y-=(int)(sHeight-disY);

			}
			else if (disX>0)
			{
				if (disY<sHeight&& disX<sWidth)
					y-=(int)(sHeight-disY);

			}
			else 
			{
				if (disY<sHeight)
					y-=(int)(sHeight+disY);

			}
			
			
		
				
		}
		else
		{

			if (disX<0)
			{
				if (disX>-sWidth)
					x+=(int)(sWidth+disX);

			}
			else if (disX>0)
			{
				if (disX<sWidth)
					x-=(int)(sWidth-disX);

			}
			else 
			{
				
					y-=(int)(sHeight+disY);

			}
		}
		

		
	}
	pThis->m_screen.m_x=(float )x;
	pThis->m_screen.m_y=(float )y;
}
void StripManager::CalBarPos(void)
{
	m_width =g_pGameMain->GetScreenWidth();
	m_height =g_pGameMain->GetScreenHeight();

	int count=0;
	
	
	//if  (m_debugCount) 
	//{
//		ScreenStrip* pObj=m_sortList[0];
	
		
//		pObj->m_screen.m_x=pObj->m_posX;
//		pObj->m_screen.m_y=pObj->m_posY;
		
	//	count++;
	//}

	

	for (int i=0;i<m_debugCount;i++) 
	{
		
		

		count++;

		
		calBarPos(i);
		
	}
	if (count!=m_debugCount)

		count=0;

}
void  StripManager::Clear()
{
	m_debugCount=0;
//	m_ifirst=m_itail=-1;
}
/*
void StripManager::AddBar(ScreenStrip* bar)
{

	if (bar->m_pren ||bar->m_next)
		return ;


	m_debugCount++;
	bar->m_screen.m_x=bar->m_posX;
	bar->m_screen.m_y=bar->m_posY;


	ScreenStrip* pobj,*next,*pren=NULL;
	next=m_first;
	while(next)
	{
		pobj=next;
		next=next->m_next;
		if (bar->m_posY>pobj->m_posY)
		{
			ScreenStrip* pren=pobj->m_pren;
			bar->m_next=pobj;
			pobj->m_pren=bar;

			bar->m_pren=pren;
			if (pren)
				pren->m_next=bar;
			else
				m_first=bar;
			return;
		}
		pren=pobj;
	}


	//bar->m_next=NULL;

	bar->m_pren=m_tail;



	if (!m_first)
		m_first=bar;

	if (m_tail)
		m_tail->m_next=bar;


	m_tail=bar;





}*/
struct _BloodBar
{
	bool m_bLoaded;
	CUiFrame * m_Frame ;
	CUiStatusBar * m_Bar ;
	CUiFontString * m_Title;
	CUiFontString * m_Level;
	CUiFontString * m_Value;
	CUiTexture * m_TargetU;
	CUiTexture * m_UTarget;

	_BloodBar():m_bLoaded(false),m_Frame(NULL),m_Bar(NULL),m_Title(NULL),m_Level(NULL),m_Value(NULL)
	{

	}
	void SetColor(CRoleSprite *sptrite);
	void UnloadUI(void)
	{
		m_bLoaded=false;
		m_Frame=0;
		m_Bar=0;
		m_Title=0;
		m_Level=0;
		m_Value=0;
		m_TargetU=0;
		m_UTarget=0;

	}
};

void _BloodBar::SetColor(CRoleSprite *obj)
{
	CUiColor color(1,1,1,1);

	if (obj)
	{

		int icolor= obj->GetNameColor();


		color.m_r=(((icolor)>>16)&0xff)/255.0f; 
		color.m_g=(((icolor)>>8)&0xff)/255.0f; 
		color.m_b=(((icolor))&0xff)/255.0f; 

		if (m_Title)
			m_Title->SetColor(color.m_r,color.m_g,color.m_b);
		if (m_Bar)
			m_Bar->SetBarColor(color);
	}
}

struct SNpcInfoLink
{

	SNpcInfoLink* m_pren;
	SNpcInfoLink* m_next;
	int m_id;
	SNpcInfo m_info;
	SNpcInfoLink(int id):m_pren(NULL),m_next(NULL),m_id(id)
	{
	}

	bool  unLink()
	{
		bool ret=m_id!=-1;
		if (ret)
		{

			SNpcInfoLink * pren=m_pren;
			SNpcInfoLink * next=m_next;
			if (next)
				next->LinkTo(pren);
			else
			{ 
				if (pren)
					pren->m_next=NULL;
			}
			m_id=-1;
			m_pren=NULL;
			m_next=NULL;
		}
		return ret;
	}
	void LinkTo(SNpcInfoLink * pren)
	{
		m_pren=pren;

		if (pren)
			pren->m_next=this;

		

	}
};
struct SpriteID
{
	int m_id;

	SNpcInfoLink * m_pTrack;
	SpriteID(int id):m_pTrack(NULL),m_id(id)
	{
	}
	void Clear()
	{
		if (m_pTrack)
			delete m_pTrack;
		m_pTrack=NULL;
	}
};

class ObjectBloodBar::imp
{
public:
	imp();
	~imp();


	//加入新SpriteID
	void AddObj(int Spriteid);
	void ChangeGameID(int oldID,int newID);

	//移走SpriteID
	void DelObj(int Spriteid);
	//設定血調顯示的種類
	void SetVisable(int visableFlag){m_visableFlag=visableFlag;}
	//取出血條顯示的種類
	int  GetVisable(void){return m_visableFlag;}

	//由SptiteID 取出
	SNpcInfo *getInfo(int objID);
	//加入一個新血條
	bool AddBar(SNpcInfo *info);
	// 空閑的血條數量
	int GetFree(void);
	//隱藏
	void SetBarHide(SNpcInfo* info);
	//計算碰撞位置
	void CalBarPos(void);
	//顯示血條開關
	void SetBloodBarEnable(bool visible);
	//取出血條顯示狀態
	bool GetBloodBarEnable(void  ){return m_Eable;}
	//跟lua取出ui物件
	void BuildFrames(CInterface*	iface);

	//void DestroyFrames(void);
	//更新所有狀態
	//void Update(int deltaTime);
	//繪出
	void Render();

	//清除
	void Clear();

	//設定感應的距離
	void SetDistance(int distance){ m_distance=distance;}
	//取出感應的距離
	int GetDistance(){return m_distance;}

	//找出空閑的血條
	int  GetFreeCount() {return m_freeindex;} 

	// 更新血條的數值
	void SetBarValue(int id,int value);
	void SetBarTitle(int id,LPCSTR name);
	void SetNewScale(SNpcInfo* info);

	// 更新血條的位置
	void SetBarPos(SNpcInfo* info,CUiFrame *icon);
	//關閉所有血條
	void CloseAllBloodBar();
	void ShowBloodBar(SNpcInfo* info,CRoleSprite* obj);
	void TargetChange(int objID,INT64 oid);
	void ChangeTargetYou(SNpcInfo* info,bool on);
	void ChangeYouTarget(SNpcInfo* info,bool on);
	BOOL IsTheSpriteHaveBloodBar(int objID);
	void ChangeState(int objID);
	void UnloadUI(void);
	//取出 資料表
	SNpcInfoLink * GetInfoList() { return m_SNpcInfoLink;}
private:

	//認可可以顯示血條的物品
	void PushTrackNpcInfo(SpriteID& Sd,int type ,int flag);
	void PopTrackNpcInfo(SpriteID& Sd);

	//內部處理sprite
	void Pross(void);

	int m_visableFlag;
	CUiFrame*           m_pUiFrameView;
	int m_count;
	int m_freeindex;
	int m_free[BLOODBAR_MAX];
	int m_distance;

	bool m_Eable;
	StripManager m_StripManager;



	_BloodBar m_bloodbar[BLOODBAR_MAX];
	vector<SpriteID> m_unPross;
	SNpcInfoLink *m_SNpcInfoLink;
	int m_LinkCount;
	//vector<SNpcInfo*> m_TrackNpcInfoTable;
};
void ObjectBloodBar::imp::SetBarTitle(int id,LPCSTR name)
{
	char temp[2048];
	_BloodBar *pBarFrame=&m_bloodbar[id];

	if( !pBarFrame->m_bLoaded )
		return;

	sprintf(temp,"%s",name);
	pBarFrame->m_Title->SetTextA(temp);
	//pBarFrame->m_Bar->SetValue(value);

}
void ObjectBloodBar::DestroyFrames(void)
{
	m_imp->UnloadUI();
}

void ObjectBloodBar::imp::ChangeTargetYou(SNpcInfo* info,bool on)
{
	if (info->m_index>=0&&info->m_index<BLOODBAR_MAX)
	{
		_BloodBar* pBarFrame=&m_bloodbar[info->m_index];
		if (pBarFrame->m_bLoaded)
		{

		CRoleSprite* sprite=g_pGameMain->FindSpriteByID(info->m_SpriteId);

		if (sprite)
		pBarFrame->SetColor(sprite);

		pBarFrame->m_TargetU->SetVisible(on);
		}
	}
}
void  ObjectBloodBar::imp::ChangeYouTarget(SNpcInfo* info,bool on)
{

	if (info->m_index>=0&&info->m_index<BLOODBAR_MAX)
	{
		_BloodBar* pBarFrame=&m_bloodbar[info->m_index];
		if (pBarFrame->m_bLoaded)
		{

		
		pBarFrame->m_UTarget->SetVisible(on);
		CRoleSprite* sprite=g_pGameMain->FindSpriteByID(info->m_SpriteId);
		if (sprite)
			pBarFrame->SetColor(sprite);
		}
	}
}
void ObjectBloodBar::imp::UnloadUI(void)
{
	int count=	BLOODBAR_MAX;
	for(int i=0;i<count;i++)
	{
		_BloodBar* pBarFrame=&m_bloodbar[i];
		pBarFrame->UnloadUI();

	}
}
//關閉所有血條
void ObjectBloodBar::imp::CloseAllBloodBar()
{

	SNpcInfoLink * pObj=m_SNpcInfoLink;


	while(pObj)
	{

		SNpcInfo* x=&pObj->m_info;
		if (x->m_index>=0&&x->m_index<BLOODBAR_MAX)
		{
			CRoleSprite* sprite=g_pGameMain->FindSpriteByID(x->m_SpriteId);
			if (sprite)
			{
				g_pGameMain->ResetSpriteNameVisible(sprite);
			}
			SetBarHide(x);
		}

		pObj=pObj->m_next;
	}

	
}
void ObjectBloodBar::imp::SetBarPos(SNpcInfo* info,CUiFrame* icon)
{
	_BloodBar* pBarFrame=&m_bloodbar[info->m_index];

	if (!pBarFrame->m_bLoaded)
		return ;



	

	
	ScreenStrip* ret=m_StripManager.getBar(info->m_index);

	if (ret)
	{

	
	float scale=ret->m_scale;
	
	

	pBarFrame->m_Frame->ClearAllAnchors();
	CUiAnchor anchor;
	
	anchor.m_relativeTo = m_pUiFrameView;
	anchor.m_offset.m_x = ret->m_screen.m_x;	
	anchor.m_offset.m_y = ret->m_screen.m_y;	
	anchor.m_point = CUiAnchor::eAnchor_Center;
	anchor.m_relativePoint = CUiAnchor::eAnchor_TopLeft;

	pBarFrame->m_Frame->SetAnchor(anchor);
	float allScale=ret->m_allScale;;

	//pBarFrame->m_Frame->SetPos(info->m_ScreenBar.m_screen);

	if (icon)
	{

		CUiAnchor anchor;
		anchor.m_relativeTo = pBarFrame->m_Frame;
		anchor.m_offset.m_x = -16/ allScale;//-10.0f; //info->m_ScreenBar.m_screen.m_x/ scale;	
		anchor.m_offset.m_y = 4/ allScale;//-5.0f; //info->m_ScreenBar.m_screen.m_y/ scale;	
		anchor.m_point = CUiAnchor::eAnchor_Center;
		anchor.m_relativePoint = CUiAnchor::eAnchor_Left;

		icon->ClearAllAnchors();
		icon->SetAnchor(anchor);
		icon->SetVisible( true );
		CUiSize size(32,32);
		icon->SetSize( size);
		icon->SetScale( scale);

	}
	}

}

void ObjectBloodBar::imp::Clear()
{
	SNpcInfoLink * pObj=m_SNpcInfoLink;


	while(pObj)
	{

		SNpcInfo* info=&pObj->m_info;
		int index=info->m_index;

		if (index>=0&&index<BLOODBAR_MAX)		
		{
			CRoleSprite* sprite=g_pGameMain->FindSpriteByID(info->m_SpriteId);

			if (sprite)
			{
				g_pGameMain->ResetSpriteNameVisible(sprite);
			}
			SetBarHide(info);		
		}

		pObj=pObj->m_next;
	}

	m_SNpcInfoLink=NULL;

	m_LinkCount=0;

	m_StripManager.Clear();
	int count=(int)m_unPross.size();

	for (int i=0;i<count;i++)
	{

		m_unPross[i].Clear();

	}
	//m_TrackNpcInfoTable.clear();
	m_unPross.clear();

}
SNpcInfo *ObjectBloodBar::imp::getInfo(int objID)
{
	
	SNpcInfoLink * pObj=m_SNpcInfoLink;

	SNpcInfo* ret=NULL;
	int count=0;

	while(pObj&&count<m_LinkCount)
	{

		SNpcInfo* x=&pObj->m_info;
		if ( x->m_SpriteId==objID)
		{
			ret=x;

			break;
		}
		count++;
		pObj=pObj->m_next;

	}

	
	return ret;

}
bool ObjectBloodBar::imp::AddBar(SNpcInfo* info)
{
	int width =g_pGameMain->GetScreenWidth();
	int height =g_pGameMain->GetScreenHeight();
	bool ret=true;

	


	CRoleSprite *Sprite= g_pGameMain->FindSpriteByID(info->m_SpriteId);

	
	CRuVector3 vecPos(0,0,0) ;//= Sprite->GetPos();
	//eAttach_Name_Title
	if (Sprite)
	Sprite->GetAttachPos(eAttach_Name_Title, vecPos);

	g_pGameMain->WorldToViewport(vecPos);	
	int x=(int )vecPos.x;
	int y=(int )vecPos.y;

	if (x<-64 ||x>(width+64) ||y<-64||y>(height+64 ))
	{

		ret=false;

	}
	else
	{

		int index=GetFree();


		info->m_index=index;


		if(Sprite)
		{
			Sprite->SetNameVisibleForce(0);
		}
		ScreenStrip*ret= m_StripManager.getBar(info->m_index);
		if (ret)
		{
			ret->m_index=info->m_index;
			ret->m_fixed=Sprite==g_pGameMain->GetPlayer();

		}
		//m_StripManager.AddBar(&info->m_ScreenBar);
	

		
		m_StripManager.AddBar(info->m_index);
		//_BloodBar* pBarFrame=&m_bloodbar[info->m_index];
		//if(pBarFrame->m_bLoaded)
		{
   			//float allScale=pBarFrame->m_Frame->GetRealScale();

			//ret->m_scale=allScale;
			ret->m_posX=x;//allScale;
			ret->m_posY=y;///allScale;
		}
	}
	  
	return ret;

}


int ObjectBloodBar::imp::GetFree(void)
{
	int index=-1;
	if (m_freeindex)
	{
		m_freeindex--;
		index=m_free[m_freeindex];
	}
	return index;
}

void ObjectBloodBar::imp::BuildFrames(CInterface*	iface)
{
	
	CUi *ui=iface->GetUiWorld();

	m_pUiFrameView= dynamic_cast<CUiFrame*>( ui->FindObject( "UIParent" ) );
	char Buf[200];
	for (int i=0;i<BLOODBAR_MAX;i++)
	{
		m_bloodbar[i]. m_bLoaded=false;
		CUiStatusBar * bar;
		CUiFrame * frame;
		sprintf(Buf,"OBloodBar%d",i+1);
		
		frame=static_cast<CUiFrame *>(ui->FindObject(Buf));
		m_bloodbar[i].m_Frame=frame;
		if ( frame== NULL )
			continue;

		sprintf(Buf,"%sBar",m_bloodbar[i].m_Frame->GetName());

		bar=static_cast<CUiStatusBar *>(ui->FindObject(Buf));

		bar->SetMaxValue(100);
		bar->SetMinValue(0);
		bar->SetValue(7);
		m_bloodbar[i].m_Bar=bar;
		sprintf(Buf,"%sTitle",frame->GetName());
		m_bloodbar[i].m_Title=static_cast<CUiFontString *>(ui->FindObject(Buf));
		sprintf(Buf,"%sLevel",bar->GetName());
		m_bloodbar[i].m_Level=static_cast<CUiFontString *>(ui->FindObject(Buf));
		sprintf(Buf,"%sValue",bar->GetName());
		m_bloodbar[i].m_Value=static_cast<CUiFontString *>(ui->FindObject(Buf));


		sprintf(Buf,"%sTargetU",bar->GetName());
		 m_bloodbar[i].m_TargetU=static_cast<CUiTexture *>(ui->FindObject(Buf));
		 sprintf(Buf,"%sUTarget",bar->GetName());
		m_bloodbar[i].m_UTarget=static_cast<CUiTexture *>(ui->FindObject(Buf));


		m_bloodbar[i]. m_bLoaded=true;
	}
}
void ObjectBloodBar::imp::SetBarHide(SNpcInfo* info)
{

	if (info->m_index>=0&&info->m_index<BLOODBAR_MAX)
	{
		_BloodBar* pBarFrame=&m_bloodbar[info->m_index];
		if (pBarFrame->m_bLoaded)		
			pBarFrame->m_Frame->SetVisible(false);
		m_free[m_freeindex]=info->m_index;
		m_StripManager.Remove(info->m_index);
		info->m_index=-1;
		m_freeindex++;
		
		//m_StripManager.Remove(&info->m_ScreenBar);
	}

}

ObjectBloodBar::imp::imp():m_visableFlag(5),m_count(0),m_freeindex(0),m_distance(BLOODBAR_DISTANCE),m_Eable(false),m_SNpcInfoLink(NULL)
{
	int i;
	for(i=0;i<BLOODBAR_MAX;i++)
	{
		m_free[i]=i;

	}
	m_freeindex=BLOODBAR_MAX;
	m_LinkCount=0;


}
ObjectBloodBar::imp::~imp()
{
	m_SNpcInfoLink=NULL;
	UnloadUI();
	Clear();
}

void ObjectBloodBar::imp::SetBloodBarEnable(bool Enable)
{
	if (!Enable)
		CloseAllBloodBar();
	m_Eable=Enable;
}

void ObjectBloodBar::imp::CalBarPos(void)
{

	m_StripManager.SortList();
	
	m_StripManager.CalBarPos();

}
void ObjectBloodBar::imp::Pross(void)
{



	vector<SpriteID> ::iterator it=m_unPross.begin();

	for(;it!=m_unPross.end();++it)
	{

		SpriteID &sID=*it;
		int  objID=sID.m_id;
		CRoleSprite* obj=g_pGameMain->FindSpriteByID(objID);
		if (obj)
		{
			int hpmax=obj->GetMaxHP();
			int hp=obj->GetHP();
			bool life=true;
			if (hpmax<=0 ||hp<=0)
				life=false;

		

			if (obj&&obj->GetTargetState()&&obj->GetCursorState()&&life||obj==g_pGameMain->GetPlayer())
			{
				int type=obj->GetObjectType()	;

				int flag=0;
				switch (type)
				{
				case eSpriteObject_Player:
				case eSpriteObject_NPC:
				case eSpriteObject_QuestNPC:

					if (obj->GetCanAttackState())
						flag|=1;
					if (obj->GetEnemyState())
						flag|=2;
					PushTrackNpcInfo(sID,type,flag);
					break;
				}
			}
			else
			{
				PopTrackNpcInfo(sID);
			}
		}


	}

//	m_unPross.clear();
}


void ObjectBloodBar::imp::AddObj(int  objID)
{
	//CRoleSprite* obj=g_pGameMain->FindSpriteByDBID(objID);
	//if (obj->GetTargetState())

	int size=(int)m_unPross.size();
	for(int i=0;i<size;i++)
	{
		if (m_unPross[i].m_id==objID)
			return ;

	}

	SpriteID newSprite(objID);
	//newSprite.m_id=objID;
	m_unPross.push_back(newSprite);

}
void  ObjectBloodBar::imp::ChangeGameID(int oldID,int newID)
{
	{	
		vector<SpriteID>::iterator nextx=m_unPross.begin();
		vector<SpriteID>::iterator itx;
		for(;nextx!=m_unPross.end();nextx++)
		{
			SpriteID &sid=*nextx;
			itx=nextx;
				
			if (sid.m_id==oldID)			
			{
				sid.m_id=newID;
				SNpcInfoLink *pTrack=sid.m_pTrack;
				if (pTrack)
				{
					SNpcInfo*x=&pTrack->m_info;					
					x->m_SpriteId=newID;					
				}				
			}
		}
	}
}
void ObjectBloodBar::ChangeGameID(int oldID,int newID)
{

		m_imp->ChangeGameID(oldID,newID);

}
void  ObjectBloodBar::imp::DelObj(int objID)
{
		
	vector<SpriteID>::iterator nextx=m_unPross.begin();
	vector<SpriteID>::iterator itx;
	for(;nextx!=m_unPross.end();)
	{
		
		itx=nextx;
		SpriteID & SID=*itx;
		nextx++;
		if (SID.m_id==objID)
		{

			SNpcInfo* x=NULL;
			SNpcInfoLink *pTrack=SID.m_pTrack;
			if (pTrack)
			{
				x=&pTrack->m_info;

				if (pTrack==m_SNpcInfoLink)
				{
					m_SNpcInfoLink=pTrack->m_next;
					
				}
				if (pTrack->unLink())
					m_LinkCount--;
			}
			if (x)
			{
				int index=x->m_index;
				if (index>=0&&index<BLOODBAR_MAX)			
				{
					CRoleSprite* sprite=g_pGameMain->FindSpriteByID(x->m_SpriteId);
					g_pGameMain->ResetSpriteNameVisible(sprite);
					SetBarHide(x);
				}
			}
			SID.Clear();
			m_unPross.erase(itx);	
			nextx=m_unPross.begin();
		}
	}	
}


void ObjectBloodBar::imp::PopTrackNpcInfo(SpriteID& spriteID)
{
	SNpcInfoLink*  pTrack=spriteID.m_pTrack;
	if (pTrack)
	{
		SNpcInfo* xSNpcInfo=&pTrack->m_info;
		if (xSNpcInfo)
		{
			int index=xSNpcInfo->m_index;
			if (index>=0&&index<BLOODBAR_MAX)			
			{
				CRoleSprite* sprite=g_pGameMain->FindSpriteByID(xSNpcInfo->m_SpriteId);
				g_pGameMain->ResetSpriteNameVisible(sprite);
				SetBarHide(xSNpcInfo);
			}


		}


		if (pTrack==m_SNpcInfoLink)
			m_SNpcInfoLink=pTrack->m_next;
		if (pTrack->unLink())
			m_LinkCount--;


	}


}

void ObjectBloodBar::imp::PushTrackNpcInfo(SpriteID& spriteID,int type ,int flag)
{

	SNpcInfoLink*  pTrack=spriteID.m_pTrack;
	bool addToLink=false;
		
	if (!pTrack)
	{
		pTrack=new SNpcInfoLink(m_LinkCount);

		spriteID.m_pTrack=pTrack;
		

		addToLink=true;

	}
	if (pTrack->m_id==-1)
	{

		pTrack->m_id=m_LinkCount;
		
		pTrack->m_info.m_index=-1;
		addToLink=true;
		
	}
	if (addToLink)
	{
		SNpcInfoLink* pren=m_SNpcInfoLink;
		m_SNpcInfoLink=pTrack;
		if (pren)
		{
			pren->LinkTo(pTrack);
		}
		m_LinkCount++;

	}
	

	


	SNpcInfo* TrackNpcInfo =&pTrack->m_info;
	TrackNpcInfo->m_SpriteId = spriteID.m_id;
	TrackNpcInfo->m_flag=flag;
	TrackNpcInfo->m_type=type;
	




	
}
bool	CompObjectDistance( SNpcInfo* info2 , SNpcInfo* info1 )
{
	return  info1->m_dis > info2->m_dis ;

}
BOOL ObjectBloodBar::imp::IsTheSpriteHaveBloodBar(int objID)
{

	SNpcInfo *si=getInfo(objID);

	if (si&&si->m_index>=0&&si->m_index<BLOODBAR_MAX)
		return TRUE;
	return FALSE;


}
BOOL ObjectBloodBar::IsTheSpriteHaveBloodBar(int objID)
{
	return 	m_imp->IsTheSpriteHaveBloodBar(objID);
}
/*

void ObjectBloodBar::imp::Update(int deltaTime)
{
}
*/
void ObjectBloodBar::imp::SetNewScale( SNpcInfo *info )
{
	_BloodBar* pBarFrame=&m_bloodbar[info->m_index];

	float scale=1.0;

	if (!pBarFrame->m_bLoaded)
		return ;

	

	if (info->m_z>80)
	{
		int t=(int)(info->m_z-80.0f);
		if (t>120)
			t=120;
		scale=scale-(t)/600.0f;
	}
	pBarFrame->m_Frame->SetScale(scale);

	ScreenStrip *ret=	m_StripManager.getBar(info->m_index);

	if (ret)
	{

	
		ret->m_allScale=pBarFrame->m_Frame->GetRealScale();
		ret->m_scale=scale;
	}


}

void ObjectBloodBar::imp::Render()
{


	Pross();

	//血條
	bool eable=GetBloodBarEnable();
	if (!eable)
		return ;

	int visableFlag=GetVisable();
	int width =g_pGameMain->GetScreenWidth();
	int height =g_pGameMain->GetScreenHeight();
	int distance=GetDistance();

	CRoleSprite* player = g_pGameMain->GetPlayer();

	
	vector<SNpcInfo*> table;


	if ( player )
	{
		SNpcInfoLink* it=GetInfoList();
		vector<int> delList;

		while(it)
		{
		
			SNpcInfo*info=&it->m_info;
			CRoleSprite* sprite=g_pGameMain->FindSpriteByID(info->m_SpriteId);

			
			if (sprite)
			{
				table.push_back(info);
				CRuVector3 dis=player->GetPos() - sprite->GetPos();
				info->m_dis =(int) dis.Magnitude();
				if (info->m_index>=0&&info->m_index<BLOODBAR_MAX)
				{
					bool bRemove=false;

					//要剔除的.
					switch (info->m_type)
					{
					case eSpriteObject_NPC:
						if (sprite->GetCanAttackState())
						{
							if (!(visableFlag&OBB_EMENY_FLAG))
								bRemove=true;	
						}
						else
						{
							if (!(visableFlag&OBB_NPC_FLAG))
								bRemove=true;	
						}

						break;
					case eSpriteObject_QuestNPC:
						if (!(visableFlag&OBB_NPC_FLAG))
							bRemove=true;	
						break;


					case eSpriteObject_Player:

						if (sprite==player)
						{
							if (!(visableFlag&OBB_SELF_FLAG))
								bRemove=true;			
						}
						else if (!(visableFlag&OBB_PLAYER_FLAG))
							bRemove=true;						
						break;
					}
					//我看的到你嗎.
					ScreenStrip *ret=m_StripManager.getBar(info->m_index);
					if (ret)
					{
						bool see=CanSeeYou(sprite,info->m_z);

						if (info->m_dis>=distance||!see ||bRemove)
						{
							g_pGameMain->ResetSpriteNameVisible(sprite);
							SetBarHide(info);
						}
						else
						{
							CRuVector3 vecPos ;					
							sprite->GetAttachPos(eAttach_Name_Title, vecPos);
							g_pGameMain->WorldToViewport(vecPos);
							int x=(int )vecPos.x;
							int y=(int )vecPos.y;
							if (x<-100 ||x>(width+100) ||y<-100||y>(height+100 ))
							{
								//sprite->SetNameVisible(1);
								//sprite->SetTitleForceState(g);
								g_pGameMain->ResetSpriteNameVisible(sprite);
								SetBarHide(info);
							}
							else
							{
							
								
								//_BloodBar* pBarFrame=&m_bloodbar[info->m_index];
							//	if(pBarFrame->m_bLoaded)
								{
								 	//float allScale=pBarFrame->m_Frame->GetRealScale();
									//ret->m_scale=allScale;
									ret->m_posX=x;///allScale;
									ret->m_posY=y;///allScale;
								}
							}
						}
					}
				}
			}
			else
			{

				delList.push_back(info->m_SpriteId);

			}	
			it=it->m_next;

		}


		//移除失效的
		if (delList.size())
		{
			int count=(int)delList.size();
			for (int i=0;i<count;i++)
				DelObj(delList[i]);
		}
		//根據距離排序.
		std::sort( table.begin() , table.end() , CompObjectDistance );
	}

	int count=(int)table.size();
	for(int i=0;i<count;i++)
	{
		SNpcInfo* info=table[i];
		ScreenStrip *ret=m_StripManager.getBar(info->m_index);
		if (ret)
		{	
			ret->m_sort=i;			
		}
        CRoleSprite* sprite=g_pGameMain->FindSpriteByID(info->m_SpriteId);
		if (sprite&&info->m_index!=-1)
		{
			//m_StripManager.Remove(&info->m_ScreenBar);
			//	m_StripManager.AddBar(&info->m_ScreenBar);

			SetBarHide(info);
			//m_StripManager.Remove(info->m_index);
			if (AddBar(info))											
				ShowBloodBar(info,sprite);		

			//m_StripManager.AddBar(info->m_index);

		}
		else if (info->m_dis<distance)
		{
			
			if (sprite)
			{

				//ScreenStrip*ret= m_StripManager.getBar(info->m_index);
				//if (ret)
				{
					bool see=CanSeeYou(sprite,info->m_z);
					if (GetFreeCount()&&see)
					{
						switch(info->m_type)
						{
						case eSpriteObject_Player: 

							if (sprite==player)
							{
								if ((visableFlag&OBB_SELF_FLAG))
								{
									if (AddBar(info))											
										ShowBloodBar(info,sprite);				
								}
							}
							else if (visableFlag&OBB_PLAYER_FLAG)
							{
								

								if (AddBar(info))											
									ShowBloodBar(info,sprite);						
							}
							break;
						case eSpriteObject_NPC:
							if (info->m_flag&2) //emeny
							{
								if (visableFlag&OBB_EMENY_FLAG)
								{
									if (AddBar(info))														
										ShowBloodBar(info,sprite);							
								}
							}
							else if (info->m_flag&1) //can attack
							{
								if (visableFlag&OBB_EMENY_FLAG)
								{
									if (AddBar(info))												
										ShowBloodBar(info,sprite);
								}
							}
							else
							{
								if (visableFlag&OBB_NPC_FLAG)
								{
									if (AddBar(info))		
										ShowBloodBar(info,sprite);
								}
							}
							break;
						case eSpriteObject_QuestNPC:
							if (visableFlag&OBB_NPC_FLAG)
							{					
								if (AddBar(info))											
									ShowBloodBar(info,sprite);						
							}
							break;
						}
					}
				}
			}
		}
		
	}
	count=(int)table.size();
	for(int i=0;i<count;i++)
	{
		SNpcInfo* info=table[i];	
		if (info->m_index>=0&&info->m_index<BLOODBAR_MAX)
			SetNewScale(info);
	}


	//計算新位置
	CalBarPos();
	//新的位置
	count=(int)table.size();
	for(int i=0;i<count;i++)
	{
		SNpcInfo* info=table[i];	
		if (info->m_index>=0&&info->m_index<BLOODBAR_MAX)
		{

			CUiFrame *icon=	g_pRaidTargetFrame->GetIconFrame(info->m_SpriteId);
			//if ()

			SetBarPos(info,icon);	
		}
	}
}
ObjectBloodBar::ObjectBloodBar(CInterface* object ): CInterfaceBase( object )
{
	g_pObjectBloodBar=this;
	m_imp=new imp();
}
ObjectBloodBar::~ObjectBloodBar()
{
	g_pObjectBloodBar=NULL;
	delete m_imp;
}
void ObjectBloodBar::imp::ChangeState(int objID)
{
	CRoleSprite* sprite=g_pGameMain->FindSpriteByID(objID);
	CRoleSprite* player=g_pGameMain->GetPlayer();
	if (sprite&&player)
	{
		SNpcInfo* sc=getInfo(sprite->GetIdentify());	
		if (sc&&(sc->m_index>=0)&&(sc->m_index<BLOODBAR_MAX))
		{
			_BloodBar *pBarFrame=&m_bloodbar[sc->m_index];

			if (!pBarFrame->m_bLoaded)
				return;
			pBarFrame->	SetColor(sprite);
		}
	}
}
void  ObjectBloodBar::imp::TargetChange(int objID,INT64 oid)
{
	CRoleSprite* sprite=g_pGameMain->FindSpriteByID(objID);
	CRoleSprite* player=g_pGameMain->GetPlayer();
	if (sprite&&player)
	{		
		if (sprite == player)
		{
			CRoleSprite* starget = sprite->GetTargetSprite();
			if (starget)
			{		
				SNpcInfo* sc=getInfo(starget->GetIdentify());	
				if (sc&&(sc->m_index>=0)&&(sc->m_index<BLOODBAR_MAX))
					ChangeYouTarget(sc,1);
			}

			CRoleSprite* old=(CRoleSprite*)oid;
			if (old&&(starget!=old))
			{
				SNpcInfo* oldsc=getInfo(old->GetIdentify());	
				if (oldsc&&(oldsc->m_index>=0)&&(oldsc->m_index<BLOODBAR_MAX))
					ChangeYouTarget(oldsc,0);
			}
		}
		else
		{
			SNpcInfo* sc=getInfo(objID);	
			if (sc&&(sc->m_index>=0)&&(sc->m_index<BLOODBAR_MAX))
			{
				CRoleSprite* starget = sprite->GetTargetSprite();
				if (starget&& (starget==player))
					ChangeTargetYou(sc,1);
				else
					ChangeTargetYou(sc,0);			

			}
					
			
		}

	}
	
	


}
void  ObjectBloodBar:: TargetChange(int objID,INT64 oid)
{

	m_imp->TargetChange(objID,oid);



}
void ObjectBloodBar:: AddObj(int  obj)
{
	m_imp->AddObj(obj);

}

void ObjectBloodBar::LoadClientData(int type, int size, char* data)
{
	if ( type == eSaveClientData_MSNInfo )
	{
		lua_State* L = GetLuaState();

		CLuaTable table(L,"MSN_INFO");

		
		
	}
}


void ObjectBloodBar::ChangeName(int objID)
{
	SNpcInfo* x=	m_imp->getInfo(objID);
	if (x)
	{
		int index=x->m_index;
		if (index>=0&&index<BLOODBAR_MAX)
		{
			CRoleSprite* obj=g_pGameMain->FindSpriteByID(objID);


			char temp[512];
			

					
			
			m_imp->SetBarTitle(index,temp);
		}

	}


}

void ObjectBloodBar::ChangeHp(int objID)
{
	SNpcInfo* x=	m_imp->getInfo(objID);
	if (x)
	{
		int index=x->m_index;
		if (index>=0&&index<BLOODBAR_MAX)
		{
			CRoleSprite* obj=g_pGameMain->FindSpriteByID(objID);
			int hpmax=obj->GetMaxHP();
			if (hpmax<=0)
				hpmax=1;			
			double par =(double)obj->GetHP() / (double)hpmax * 100.0;
			m_imp->SetBarValue(index,(int)par);
		}

	}


}
void ObjectBloodBar::ChangeState(int objID)
{

	m_imp->ChangeState(objID);
}

void ObjectBloodBar::DelObj(int obj)
{
	m_imp->DelObj(obj);
}
void ObjectBloodBar::imp::SetBarValue(int id,int value)
{
	char temp[2048];
	_BloodBar *pBarFrame=&m_bloodbar[id];

	if( !pBarFrame->m_bLoaded )
		return;

	sprintf(temp,"%d",value);
	pBarFrame->m_Value->SetTextA(temp);
	pBarFrame->m_Bar->SetValue((float)value);
}
/*
void ObjectBloodBar::SetBarValue(int id,int value)
{

lua_State* L = GetLuaState();
lua_pushnumber( L, id+1 );
lua_setglobal( L, "arg1" );

lua_pushnumber( L, value );
lua_setglobal( L, "arg2" );


SendWorldEvent("OBJECTBLOODBAR_CHANGEVALUE");
}
*/
/*
void ObjectBloodBar::SetBarHide(int id)
{
//m_imp->SetBarHide();
lua_State* L = GetLuaState();
lua_pushnumber( L, id+1 );
lua_setglobal( L, "arg1" );
SendWorldEvent("OBJECTBLOODBAR_HIDE");
}
*/
/*
void ObjectBloodBar::SetBarPos(int id,int x,int y)
{

lua_State* L = GetLuaState();
lua_pushnumber( L, id+1 );
lua_setglobal( L, "arg1" );

lua_pushnumber( L, x );
lua_setglobal( L, "PosX" );
lua_pushnumber( L, y );
lua_setglobal( L, "PosY" );


SendWorldEvent("OBJECTBLOODBAR_CHANGEPOS");
}*/
void  ObjectBloodBar::imp::ShowBloodBar(SNpcInfo* info,CRoleSprite* obj)
{

	CRoleSprite* player=g_pGameMain->GetPlayer();

	if (!player|| !obj)
		return;

	if (obj && obj->GetNameVisibleForce() )
	{
		obj->SetNameVisibleForce(0);
	}


	int disLev=player->GetLevel();

	
	char temp[2048];
	_BloodBar *pBarFrame=&m_bloodbar[info->m_index];

	if (!pBarFrame->m_bLoaded)

		return;


	pBarFrame->	SetColor(obj);
	int hpmax=obj->GetMaxHP();
	if (hpmax<=0)
		hpmax=1;

	int visableFlag=GetVisable();
	_GetNameString(temp,visableFlag,obj);

	


	pBarFrame->m_Title->SetTextA(temp);

	

	CRoleSprite *ObjT=	obj->GetTargetSprite();
	CRoleSprite *PalyerT=	player->GetTargetSprite();
	if (ObjT==player)
		pBarFrame->m_TargetU->SetVisible(1);	
	else
		pBarFrame->m_TargetU->SetVisible(0);

	if (PalyerT==obj)
		pBarFrame->m_UTarget->SetVisible(1);
	else
		pBarFrame->m_UTarget->SetVisible(0);


	



	pBarFrame->m_Frame->SetID(info->m_SpriteId);



	int par = (int)((double)obj->GetHP() / (double)hpmax * 100.0);
	sprintf(temp,"%d",par);
	pBarFrame->m_Value->SetTextA(temp);
	pBarFrame->m_Bar->SetValue((float)par);


	int lev=obj->GetLevel();

	disLev=lev-disLev;
	sprintf(temp,"%d",lev);
	pBarFrame->m_Level->SetTextA(temp);

	if (disLev>=6)
	{
		pBarFrame->m_Level->SetColor(1.0f,0.2f,0.2f);
	}

	else if (disLev>=3)
	{
		pBarFrame->m_Level->SetColor(1,1,0);
	}

	else if (disLev==0)
	{
		pBarFrame->m_Level->SetColor(0.0f,0.0f,1.0f);
	}

	else if (disLev>=-3)
	{
		pBarFrame->m_Level->SetColor(1.0f,0.0f,0.0f);
	}
	else //if (disLev>=-6)
	{
		pBarFrame->m_Level->SetColor(0.8f,0.75f,0.75f);
	}
	float scale=1;
	//ScreenStrip*ret=m_StripManager.getBar(info->m_index);
	if (info->m_z>80)
	{
		int t=(int)(info->m_z-80.0f);
		if (t>120)
			t=120;
		scale-=(t)/600.0f;
	}
	pBarFrame->m_Frame->SetScale(scale);
	pBarFrame->m_Frame->SetVisible(true);
}
void ObjectBloodBar::RegisterFunc()
{
	lua_State* L = GetLuaState();
	lua_register(L, "OBB_ChangeTraget",		LuaFunc_OBB_ChangeTraget);

}

void ObjectBloodBar::Render()
{
	m_imp->Render();
}

void ObjectBloodBar::SetVisable(int visableFlag)
{
	m_imp->SetVisable(visableFlag);
}

void ObjectBloodBar::Clear(void)
{
	m_imp->Clear();
}
void  ObjectBloodBar::SetBloodBar(bool visible)
{
	m_imp->SetBloodBarEnable(visible);
}
bool ObjectBloodBar::GetBloodBar(void  )
{
	return m_imp->GetBloodBarEnable();
}
void ObjectBloodBar::Load(void)
{
	m_imp->BuildFrames(m_interface);
}
int  ObjectBloodBar::GetVisable(void)
{
	return m_imp->GetVisable();
}
void ObjectBloodBar::BuildFrames(void)
{
	m_imp->BuildFrames(m_interface);
}

void ObjectBloodBar::SetDistance(int distance)
{
	m_imp->SetDistance(distance);
}
int  ObjectBloodBar::GetDistance(void)
{
	return m_imp->GetDistance();
}


void ObjectBloodBar::OnBarClick(int objID)
{
	SNpcInfo * x=m_imp->getInfo(objID);
	if (x)
	{
		//CRoleSprite *player=g_pGameMain->GetPlayer();
		CRoleSprite *sprite=g_pGameMain->FindSpriteByID(objID);
		if (sprite&&g_pGameMain)
			g_pGameMain->SetPlayerTarget(sprite);
			//player->SetTargetSprite(sprite);
	}
}
int LuaFunc_OBB_ChangeTraget(lua_State* L)
{
	int spriteID=(int )lua_tonumber(L, 1);
	if (g_pObjectBloodBar)
		g_pObjectBloodBar->OnBarClick(spriteID);	
	return 0;
}