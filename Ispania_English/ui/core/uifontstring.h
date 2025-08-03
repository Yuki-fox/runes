#pragma once
#include <vector>
#include "uilayout.h"
#include "Rune Engine/Rune/UI/RuUI_Font.h"
using namespace std;
struct HyperlinkData
{
	const WCHAR* begin;				// 開始字元位置
	int count;						// 總長度
};
struct CharInfo
{
	CharInfo(CRuFontString* ruFontString, DWORD fontColor, int hyperIndex = -1, bool Outline = false)
	{
		fontString = ruFontString;
		color = fontColor;
		hyperlinkIndex = hyperIndex;
		bOutline = Outline;
	}
	CRuFontString* fontString;
	DWORD			color;
	int				hyperlinkIndex;
	bool			bOutline;
};
struct StringInfo
{
	int			begin;
	int			end;
	float		width;
};
// --------------------------------------------------------------------------------------
class CUiLuaFontString
{
public:
	static	void			OpenLib(lua_State* L);
	static	int				SetMetaTable(lua_State* L);
};
// --------------------------------------------------------------------------------------
class CUiFontString : public CUiLayout
{
public:
	CUiFontString(const char* name, CUiLayout* pParent, CUi* pUi);
	virtual ~CUiFontString();
	// 對齊方式
	enum {
		eJustifyV_Top = 0,
		eJustifyV_Middle = 1,
		eJustifyV_Bottom = 2
	};
	enum {
		eJustifyH_Left = 0,
		eJustifyH_Center = 1,
		eJustifyH_Right = 2
	};
	// static
	static	bool			HyperlinkText(const WCHAR* inStr, int inSize, wstring& outStr);
	static	bool			HyperlinkTextA(const char* inStr, int inSize, string& outStr);
	//virtual	void			Update(float elapsedTime)					{}
	virtual	void			Render(float alpha);
	virtual	void			Render(float alpha, float x, float y);
	virtual	void			Render(float alpha, float x, float y, int begin, int end);
	virtual	void			SetAnchorSize(CUiSize size);
	virtual void			SetSize(CUiSize size, bool isReAnchor = true);
	virtual	void			SetFontType(CRuFontType* pFontType);
	virtual bool			CreateFontType(const char* font, int size, RuFontWeight weight, RuFontOutline outline);
	virtual	void			SetBytes(int bytes);
	virtual	void			SetText(const WCHAR* text, bool isParse = true);
	virtual	void			SetTextA(const char* text, bool isParse = true);
	virtual	const WCHAR* GetText() { return m_text; }
	virtual	void			SetFontShadow(RuFontShadow* shadow);
	virtual	void			ClearFontShadow();
	virtual RuFontShadow* GetFontShadow() { return m_pShadow; }
	virtual	void			SetSpacing(float spacing) { m_spacing = spacing; }
	virtual	float			GetSpacing() { return m_spacing; }
	virtual	void			SetJustifyVType(int justifyType) { m_justifyVType = justifyType; }
	virtual	int				GetJustifyVType() { return m_justifyVType; }
	virtual	void			SetJustifyHType(int justifyType) { m_justifyHType = justifyType; }
	virtual	int				GetJustifyHType() { return m_justifyHType; }
	virtual	CRuFontType* GetFontType() { return m_pFontType; }
	virtual	void			SetLuaMetaTable(lua_State* l) { CUiLuaFontString::SetMetaTable(l); }
	virtual	void			SetColor(float r, float g, float b);
	virtual	void			SetAlpha(float a);
	virtual HyperlinkData* GetHyperlinkText(float x, float y);
	virtual int				GetStringLines(float height);
	virtual int				TextToCaret(int position);
	virtual int				CaretToIndex(int caret);
	virtual int				GetCharSize(int index, int* beginIndex = NULL);
	virtual int				GetPaintLines() { return (int)m_strData.size(); }
	virtual void			HideLastDot(bool hide) { m_hideLastDot = hide; }
	virtual int				GetMemSize();
	virtual vector<StringInfo>& GetStringData() { return m_strData; }
	virtual float			GetDisplayWidth(int mode = 0);
	virtual bool			IsDrawDot() { return m_isDrawDot; }
	virtual void			SetRTL(bool isRTL) { m_isRTL = isRTL; }
	virtual bool			IsRTL() { return m_isRTL; }
protected:
	friend class CUiEditBox;
	virtual void			ResetSize() {}
	virtual	void			PaintChar(float x, float y, int begin, int end, float alpha, float scale);
	virtual	float			Separate(int index, int& end, float limitLength = 0.0f);
	virtual	float			CaretToPos(int begin, int caret);
	virtual	int				PosToCaret(int begin, float width, bool& trail);
	virtual	void			GetLineInfo();
	virtual void			ModifySize();
	virtual	int				HexStrToInt(const WCHAR* valueStr);
	virtual void			ResetCharData();
	virtual void			SetCharData(bool parse, const WCHAR* ws, int size = -1);
protected:
	// 設定值
	vector<CharInfo>		m_charData;
	vector<StringInfo>		m_strData;
	vector<HyperlinkData>	m_hyperlinkData;
	vector<int>				m_caretPosition;
	WCHAR* m_text;
	int						m_bytes;
	CRuFontType* m_pFontType;
	CRuFontString* m_pDotChar;
	RuFontShadow* m_pShadow;
	float					m_spacing;						// 行列間距
	int						m_justifyVType;
	int						m_justifyHType;
	CUiRGBA					m_blockColor;					// 選取區域顏色
	bool					m_internalChangedSize;
	bool					m_hideLastDot;					// 隱藏"..."
	bool					m_isDrawDot;					// 是否有顯示"..."
	bool					m_isRTL;						// 右到左
	static int				s_charColor;					// 文字顏色
	static int				s_linkIndex;					// 索引值
	static const WCHAR* s_textBegin;					// 文字開始位置
};