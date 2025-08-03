#include <windows.h>
#include "uimessageframe.h"

// ----------------------------------------------------------------------------------
CUiMessageFrame::CUiMessageFrame(const char* name, CUiLayout* pParent, CUi* pUi)
	: CUiFrame(name, pParent, pUi)
{
	m_uiType			= eUiType_MessageFrame;

	m_text				= NULL;
	m_fontString		= NULL;
	m_maxLines			= 0;
	m_sizeLines			= 0;
	m_lastLine			= 0;
	m_fadeDuration		= 10.0f;
	m_displayDuration	= 120.0f;
	m_lineBytes			= 511;

	m_enableHyperlink	= false;
	m_hyperlinkEnter	= false;
	memset(m_hyperlink, 0, sizeof(m_hyperlink));
	m_hyperlinkHit.clear();
}

// ----------------------------------------------------------------------------------
CUiMessageFrame::~CUiMessageFrame()
{
	UI_SAFE_DELETE(m_fontString);
	ReleaseText();
}

// ----------------------------------------------------------------------------------
void CUiMessageFrame::SetFontString(CUiFontString* fontString)
{
	UI_SAFE_DELETE(m_fontString);
	m_fontString = fontString;
	ResetFontString();
}

// ----------------------------------------------------------------------------------
void CUiMessageFrame::ResetFontString()
{
	ClearTextItemFont();
	ResetTextItemTime();
}

// ----------------------------------------------------------------------------------
CUiFrame* CUiMessageFrame::GetFrameAtPoint(CUiPoint pt, CUiFrame* pExclude)
{
	if (m_isVisible /*&& m_isEnable*/)
	{
		for (list<CUiFrame*>::reverse_iterator iter = m_child.rbegin(); iter != m_child.rend(); iter++ )
		{
			CUiFrame *pFocus;
			if ( (pFocus = (*iter)->GetFrameAtPoint(pt , pExclude)) != NULL )
				return pFocus;
		}

		if ( this != pExclude )
		{			
			for ( vector<MessageHyperlinkHit>::iterator iter = m_hyperlinkHit.begin(); iter != m_hyperlinkHit.end(); iter++ )
			{
				if ( pt.m_y - iter->y < 0 || iter->font == NULL )
					continue;

				HyperlinkData* hyperData = iter->font->GetHyperlinkText(pt.m_x - iter->x, pt.m_y - iter->y);
				if ( hyperData )
				{
					memcpy(m_hyperlink, hyperData->begin, hyperData->count * sizeof(WCHAR));
					m_hyperlink[hyperData->count] = 0;
					return this;
				}
			}

			if ( m_state.isMouseEnable && ContainsPoint(pt) )
				return this;
		}
	}	
	return NULL;
}

// ----------------------------------------------------------------------------------
void CUiMessageFrame::ClearText()
{
	for ( int i = 0; i < m_sizeLines; i++ )
	{
		UI_SAFE_DELETE(m_text[i].font);
		UI_SAFE_DELETE_ARRAY(m_text[i].str);
	}
	m_sizeLines = 0;
	m_lastLine = 0;

	ReleaseHyperlink();
}

// ----------------------------------------------------------------------------------
void CUiMessageFrame::ReleaseText()
{
	ClearText();
	UI_SAFE_DELETE_ARRAY(m_text);
	m_maxLines = 0;
}

// ----------------------------------------------------------------------------------
bool CUiMessageFrame::AddMessage(const WCHAR* text, DWORD color)
{
	if (text && m_fontString)
	{
		// pop front
		if ( m_sizeLines >= m_maxLines )
			PopItem();
		
		// push back
		PushItem(text, color);		
		return true;
	}
	return false;	
}

// ----------------------------------------------------------------------------------
bool CUiMessageFrame::AddMessage(const char* text, DWORD color)
{
	//WCHAR* tmp = MultiByteToWChar(text);
	WCHAR* tmp = Utf8ToWChar(text);
	bool re = AddMessage(tmp, color);
	UI_SAFE_DELETE_ARRAY(tmp);
	return re;
}

// ----------------------------------------------------------------------------------
void CUiMessageFrame::SetMaxLines(int maxLines)
{
	if (m_maxLines != maxLines)
	{
		ReleaseText();

		m_maxLines = maxLines;
		if (m_maxLines > 0)
		{
			m_text = new TextItem[m_maxLines];
			memset(m_text, 0, sizeof(TextItem)*m_maxLines);
		}
	}
}

// ----------------------------------------------------------------------------------
void CUiMessageFrame::SetLastLine(int line)
{	
	if ( line > 0 && line <= m_sizeLines && m_lastLine != line )
	{
		m_lastLine = line;
		OnValueChanged((float)m_lastLine);
	}
}

// ----------------------------------------------------------------------------------
int CUiMessageFrame::GetPageLines()
{
	int pageLines = 0;
	float scale = GetRealScale();
	if (m_fontString)
	{
		CUiSize size(m_size.m_x - m_textInset.m_right - m_textInset.m_left, m_size.m_y - m_textInset.m_bottom - m_textInset.m_top);
		CUiPoint screenPt = GetPaintPos();
		float y = screenPt.m_y + (size.m_y * scale);

		for ( int i = m_lastLine; i > 0; i-- )
		{
			TextItem* item = &m_text[i-1];
			if ( item->font )
			{
				CUiSize fontSize = item->font->GetRealSize();
				if (y - fontSize.m_y < screenPt.m_y)
				{
					//pageLines += item->font->GetStringLines(screenPt.m_y - y);
					break;
				}

				y -= fontSize.m_y;
				pageLines++;
				pageLines += item->font->GetPaintLines();
			}
		}
	}

	return pageLines;
}

// ----------------------------------------------------------------------------------
int CUiMessageFrame::GetMemSize()
{
	int size = CUiFrame::GetMemSize() + sizeof(CUiMessageFrame) - sizeof(CUiFrame);

	for ( int i = 0; i < m_sizeLines; i++ )
	{
		size += sizeof(TextItem);
		if ( m_text[i].font )
		{
			size += m_text[i].font->GetMemSize();
		}

		if ( m_text[i].str )
		{
			size += (int)(wcslen(m_text[i].str) + 1) * sizeof(WCHAR);
		}
	}

	if ( m_fontString )
	{
		size += m_fontString->GetMemSize();
	}

	for ( vector<MessageHyperlinkHit>::iterator iter = m_hyperlinkHit.begin(); iter != m_hyperlinkHit.end(); iter++ )
	{
		size += sizeof(MessageHyperlinkHit);

		if ( iter->font )
			size += iter->font->GetMemSize();
	}

	return size;
}

// ----------------------------------------------------------------------------------
void CUiMessageFrame::ScrollDown()
{
	ResetTime();
	SetLastLine(m_lastLine + 1);
}

// ----------------------------------------------------------------------------------
void CUiMessageFrame::ScrollUp()
{
	ResetTime();
	SetLastLine(m_lastLine - 1);
}

// ----------------------------------------------------------------------------------
void CUiMessageFrame::ScrollBottom()
{
	ResetTime();
	SetLastLine(m_sizeLines);
}

// ----------------------------------------------------------------------------------
void CUiMessageFrame::ScrollTop()
{
	ResetTime();
	SetLastLine(1);
}

// ----------------------------------------------------------------------------------
void CUiMessageFrame::EnableHyperlink(bool enable)
{
	if ( m_enableHyperlink != enable )
	{
		m_enableHyperlink = enable;
		ReleaseHyperlink();
	}
}

// ----------------------------------------------------------------------------------
void CUiMessageFrame::PopItem()
{
	if (m_sizeLines > 0)
	{
		m_sizeLines--;

		vector<MessageHyperlinkHit>::iterator iter = m_hyperlinkHit.begin();
		while ( iter != m_hyperlinkHit.end() )
		{
			if ( iter->font == m_text[0].font )
				iter = m_hyperlinkHit.erase(iter);
			else
				iter++;
		}

		UI_SAFE_DELETE(m_text[0].font);
		UI_SAFE_DELETE_ARRAY(m_text[0].str);
		memmove(m_text, m_text+1, sizeof(TextItem)*m_sizeLines);
		memset(m_text+m_sizeLines, 0, sizeof(TextItem) );		
	}
}

// ----------------------------------------------------------------------------------
void CUiMessageFrame::PushItem(const WCHAR* text, DWORD color)
{
	if ( m_sizeLines < m_maxLines )
	{
		TextItem* item	= &m_text[m_sizeLines++];
		item->font		= NULL;
		item->color		= color;
		item->time		= 0.0f;
		item->alpha		= 1.0f;
		item->str		= new WCHAR[wcslen(text) + 1];
		wcscpy(item->str, text);
		
		if ( m_sizeLines == m_lastLine+1 )
			SetLastLine(m_sizeLines);
	}
}

// ----------------------------------------------------------------------------------
void CUiMessageFrame::ClearTextItemFont()
{
	ReleaseHyperlink();
	for ( int i = 0; i < m_sizeLines; i++ )
	{
		UI_SAFE_DELETE(m_text[i].font);
	}	
}

// ----------------------------------------------------------------------------------
void CUiMessageFrame::ResetTextItemTime()
{
	for ( int i = 0; i < m_sizeLines; i++ )
	{
		m_text[i].time = 0.0f;
		m_text[i].alpha = 1.0f;
	}
}

// ----------------------------------------------------------------------------------
void CUiMessageFrame::ReleaseHyperlink()
{
	memset(m_hyperlink, 0, sizeof(m_hyperlink));
	m_hyperlinkHit.clear();
}

// ----------------------------------------------------------------------------------
void CUiMessageFrame::OnResize()
{
	ResetTextItemTime();
	ClearTextItemFont();
	CUiFrame::OnResize();
}

// ----------------------------------------------------------------------------------
void CUiMessageFrame::OnUpdate(float elapsedTime)
{
	if (m_displayDuration > 0.0f)
	{
		for ( int i = 0; i < m_sizeLines; i++ )
		{
			m_text[i].time += elapsedTime;
			if (m_text[i].time > m_displayDuration)
				m_text[i].alpha -= elapsedTime / m_fadeDuration;

			if (m_text[i].alpha < 0.0f)
				m_text[i].alpha = 0.0f;
		}
	}

	CUiFrame::OnUpdate(elapsedTime);
}

// ----------------------------------------------------------------------------------
void CUiMessageFrame::OnPaint(float alpha)
{
	// CUiFrame::OnPaint();

	m_hyperlinkHit.clear();

	float scale = GetRealScale();
	if (m_fontString && alpha > 0.0f)
	{
		CUiSize size(m_size.m_x - m_textInset.m_right - m_textInset.m_left, m_size.m_y - m_textInset.m_bottom - m_textInset.m_top);
		CUiPoint screenPt = GetPaintPos();
		float y = screenPt.m_y + (size.m_y * scale);
		
		for ( int i = m_lastLine; i > 0; i-- )
		{
			TextItem* item = &m_text[i-1];
			if (item->alpha <= 0.0f)
				break;

			if (item->font == NULL)
			{
				CUiColor color(item->color);
				item->font = new CUiFontString(NULL, this, m_pUi);
				item->font->SetBytes(m_lineBytes);
				item->font->SetFontType(m_fontString->GetFontType());
				item->font->SetFontShadow(m_fontString->GetFontShadow());
				item->font->SetJustifyHType(m_fontString->GetJustifyHType());
				item->font->SetJustifyVType(m_fontString->GetJustifyVType());
				item->font->SetSpacing(m_fontString->GetSpacing());
				item->font->SetRTL(m_fontString->IsRTL());
				item->font->SetAnchorSize(CUiSize(size.m_x, 0.0f));
				item->font->SetColor(color.m_r, color.m_g, color.m_b);
				item->font->SetText(item->str);				
			}
			
			CUiSize fontSize = item->font->GetRealSize();
			if (y - fontSize.m_y < screenPt.m_y)
				break;

			// Hyperlink Rectangle
			if ( m_enableHyperlink )
			{
				//CUiPoint point = item->font->GetPaintPos();
				MessageHyperlinkHit hyperlinkHit;
				switch ( m_fontString->GetJustifyHType() )
				{
				case CUiFontString::eJustifyH_Left:
				case CUiFontString::eJustifyH_Center:
					hyperlinkHit.x = screenPt.m_x + (m_textInset.m_left * scale);
					break;

				case CUiFontString::eJustifyH_Right:
					hyperlinkHit.x = screenPt.m_x + (m_textInset.m_left * scale);
					break;
				}
				
				hyperlinkHit.y = y - fontSize.m_y;
				hyperlinkHit.font = item->font;
				m_hyperlinkHit.push_back(hyperlinkHit);
				memset(m_hyperlink, 0, sizeof(m_hyperlink));
			}
			
			item->font->Render(alpha * item->alpha, screenPt.m_x + (m_textInset.m_left * scale), y - fontSize.m_y);
			y -= fontSize.m_y;
		}
	}
}

// ----------------------------------------------------------------------------------
void CUiMessageFrame::OnMouseLeave(CUiPoint pt)
{
	if ( m_hyperlinkEnter ) {
		OnHyperlinkLeave(m_hyperlink);
		m_hyperlinkEnter = false;	
	}
	else
		CUiFrame::OnMouseLeave(pt);
}

// ----------------------------------------------------------------------------------
void CUiMessageFrame::OnMouseEnter(CUiPoint pt)
{
	if ( m_hyperlink[0] > 0 ) {
		m_hyperlinkEnter = true;
		OnHyperlinkEnter(m_hyperlink);
	}
	else
		CUiFrame::OnMouseEnter(pt);
}

// ----------------------------------------------------------------------------------
void CUiMessageFrame::OnClick(CUiPoint pt, int key)
{
	if ( m_hyperlink[0] > 0 )
		OnHyperlinkClick(m_hyperlink, key);
	else
		CUiFrame::OnClick(pt, key);
}