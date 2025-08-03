

#include "StdAfx.h"
#include <UI\UISliderBar.h>
CSliderBarUI::CSliderBarUI(SquirrelObject&so):CControlUI(so), m_LowRange(20),m_HighRange(10000),m_Value(50),m_uButtonState(0)
{
m_cxyFixed.cx=120;
m_cxyFixed.cy=40;

}

void  CSliderBarUI::SetWidth(int cxWidth)
{
	m_cxyFixed.cx=cxWidth;

}
void  CSliderBarUI::SetHeight(int cyHeight)
{
	m_cxyFixed.cy=cyHeight;

}

void CSliderBarUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcscmp(pstrName, _T("width")) == 0 ) 
		SetWidth(_ttoi(pstrValue));
	else if( _tcscmp(pstrName, _T("height")) == 0 )
	
		SetHeight(_ttoi(pstrValue));
	
	else 
		CControlUI::SetAttribute(pstrName, pstrValue);
}
int CSliderBarUI::GetValue(void)
{

	return m_Value;

}
void CSliderBarUI::SetValue(int Value)
{

	m_Value=Value;

	if (m_Value<m_LowRange)
		m_Value=	m_LowRange;
	if (m_Value>m_HighRange)
		m_Value=	m_HighRange;

	if( m_pManager != NULL ) 
		m_pManager->SendNotify(this, _T("changed"));

	Invalidate();
}
void CSliderBarUI::SetRange(int lowRange,int highRange)
{
	 m_LowRange=lowRange;
	 m_HighRange=highRange;

}

void CSliderBarUI::DoPaint(HDC hDC, const RECT& rcPaint)
{

	int half_h=(m_rcItem.bottom -m_rcItem.top)/2;
	int half_w=( m_rcItem.right-m_rcItem.left)/2;
	int witdh=(m_rcItem.right-m_rcItem.left)-20;


	UITYPE_COLOR clrBorder1 = UICOLOR_CONTROL_TEXT_NORMAL;
	UITYPE_COLOR clrBorder2 = UICOLOR_CONTROL_BORDER_NORMAL;
	if( (m_uButtonState & UISTATE_PUSHED)  ) 
	{
		clrBorder1 = UICOLOR_CONTROL_TEXT_NORMAL;
		clrBorder2 = UICOLOR_CONTROL_BORDER_SELECTED;
	}



	
	RECT rc ={
		m_rcItem.left,
		m_rcItem.top, 
		m_rcItem.right,
		m_rcItem.bottom
	};

	if( IsFocused() ) 
	{
		CBlueRenderEngineUI::DoPaintFrame(hDC, m_pManager, rc, UICOLOR_BUTTON_BORDER_LIGHT, UICOLOR_BUTTON_BORDER_DARK, UICOLOR__INVALID, UIFRAME_FOCUS);
	}
	
	rc.left	=	m_rcItem.left +2;
	rc.top	=	m_rcItem.top+half_h-8; 
	rc.right	=	m_rcItem.right -2;
	rc.bottom =	m_rcItem.top+half_h+8;

	
	CBlueRenderEngineUI::DoPaintFrame(hDC, m_pManager, rc, UICOLOR_BUTTON_BORDER_LIGHT, UICOLOR_BUTTON_BORDER_DARK, UICOLOR__INVALID, UIFRAME_ROUND);

	
	 m_rcChannel.left	=	m_rcItem.left +9;
	 m_rcChannel.top	=	m_rcItem.top+half_h-5; 
	 m_rcChannel.right	=	m_rcItem.right -9;
	 m_rcChannel.bottom =	m_rcItem.top+half_h+5;

	CBlueRenderEngineUI::DoPaintFrame(hDC, m_pManager, m_rcChannel,UICOLOR_STANDARD_RED, UICOLOR_STANDARD_WHITE);

	



	//::InflateRect(&rc, -1, -1);
	// CBlueRenderEngineUI::DoPaintGradient(hDC, m_pManager, rc,RGB(0,10,20), RGB(255,0,240), false, 128);

	//::InflateRect(&rc, -2, -2);
	

	// Draw THUMB Button

	//THUMB_WIDTH
	int v=m_HighRange-m_LowRange;

	float a=((m_Value-m_LowRange)*1.f)/v;

	int v1=m_rcChannel.right-m_rcChannel.left;


	

	int x=m_rcChannel.left;
	int y=m_rcItem.top+half_h;
	int pos=v1*a;

	 m_rcThumb.left =	x+pos-THUMB_WIDTH/2;
	 m_rcThumb.top	=		y-THUMB_HEIGHT/2;
	 m_rcThumb.right	=	x+pos+THUMB_WIDTH/2, 
	 m_rcThumb.bottom	=	y+THUMB_HEIGHT/2, 


	
	CBlueRenderEngineUI::DoPaintRectangle(hDC, m_pManager, m_rcThumb, clrBorder1,clrBorder2);
	//CBlueRenderEngineUI::DoPaintGradient(hDC, m_pManager, rc1,RGB(0,10,20), RGB(255,0,240), true, 16);
	
	//

}

UINT CSliderBarUI::GetControlFlags() const
{
	return UIFLAG_TABSTOP;
}
void CSliderBarUI::CalMouse(POINT &MousePos,bool step)
{

	int v1=m_rcChannel.right-m_rcChannel.left;
	float a1=((MousePos.x-m_rcChannel.left)*1.f)/v1;

	int v2=m_HighRange-m_LowRange;
	float a2=((m_Value-m_LowRange)*1.f)/v2;
	if (a2<0)
		a2=0;
	else if(a2>1)
		a2=1;

	int  value=v2/10;

	if (value<0)
	value=1;

	float d=a2-a1;
	int dValue=GetValue();
	
	if (step)
	{
		if (d>0.1)
		{
			dValue=dValue-value;
		
		}
		else if	(d<(-0.1))
		{
			dValue=dValue+value;
			
		}
		else
		{
			float x=v2*a1;
			if (x<0.5)
				x=1;
			//else if (x>0.5)
			//	x=1;
			dValue=x+m_LowRange;
		}

	}
	else
	{

		float x=v2*a1;
		//if (x<0.5)
		//	x=1;
		dValue=x+m_LowRange;

	}

		

	

	SetValue(dValue);

}
void CSliderBarUI::Step(bool up,float scale)
{

	int v2=m_HighRange-m_LowRange;
	
	int dValue=GetValue();
	
	float a=v2*scale;
	if (a<1)
		a=1;
	if (up)
		dValue+=a;
	else
		dValue-=a;

	

	SetValue(dValue);

	
}

bool CSliderBarUI::Activate()
{
	if( !CControlUI::Activate() ) 
		return false;



	
	return true;
}
void CSliderBarUI::Event(TEventUI& event)
{
	if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK )
	{
		if(  IsEnabled())
		{

		
			if(	::PtInRect(&m_rcChannel, event.ptMouse) ) 
			{
				m_uButtonState |=  UISTATE_CAPTURED;

			}
			if( ::PtInRect(&m_rcThumb, event.ptMouse) ) 
			{
				m_uButtonState |= UISTATE_PUSHED|UISTATE_CAPTURED;
	
				Invalidate();
			}

		}
	}
	if( event.Type == UIEVENT_KEYDOWN )
	{
		if (IsFocused())
		{		
			switch( event.chKey )
			{
			case VK_LEFT:
				Step(false,0.01f);

				return;
			case VK_UP:
				Step(true,0.1f);
				return;
			case VK_DOWN:
				Step(false,0.1f);

				return;
			case VK_RIGHT:
				Step(true,0.01f);
			return;
			}
		}
	}
	if( event.Type == UIEVENT_MOUSEMOVE )
	{
		if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) 
		{
			if( ::PtInRect(&m_rcItem, event.ptMouse) ) 
			{
				//	m_uButtonState |= UISTATE_PUSHED;
				//else 
				//	m_uButtonState &= ~UISTATE_PUSHED;

				if (m_uButtonState &	UISTATE_PUSHED)
				{
					CalMouse(event.ptMouse,false);

				}
				else

					CalMouse(event.ptMouse,false);

				//Invalidate();
			}
		}
	}
	if( event.Type == UIEVENT_BUTTONUP )
	{
		if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) 
		{
			if( ::PtInRect(&m_rcItem, event.ptMouse) )
			{
				//event.ptMouse
				if (m_uButtonState&UISTATE_PUSHED)
					CalMouse(event.ptMouse,false);

				else
					CalMouse(event.ptMouse,false);
				Activate();
			}
			m_uButtonState &= ~(UISTATE_PUSHED | UISTATE_CAPTURED);
			Invalidate();
		}
	}
	if( event.Type == UIEVENT_MOUSEENTER)
	{
		m_uButtonState |= UISTATE_HOT;
		Invalidate();
	}
	if( event.Type == UIEVENT_MOUSELEAVE)
	{
		m_uButtonState &= ~UISTATE_HOT;
		Invalidate();
	}
	if( event.Type == UIEVENT_SCROLLWHEEL )
	{
		bool bDownward = LOWORD(event.wParam) == SB_LINEDOWN;
		Step(bDownward,0.01f);
		return;
	}
	CControlUI::Event(event);
}

LPCTSTR CSliderBarUI::GetClass() const 
{

	return _T("SliderBarUI");
}
SIZE CSliderBarUI::EstimateSize(SIZE szAvailable)
{
	return m_cxyFixed;

}