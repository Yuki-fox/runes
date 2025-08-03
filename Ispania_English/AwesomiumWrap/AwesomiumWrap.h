#pragma once



//---------------------------------------------------------------
#include <vector>
#include <string>
#include <map>
struct IWebView;

#define INTERFACE_IWeb(T)\
public:\
	virtual void update(void)##T\
	virtual IWebView* createWebView(int width, int height)##T\



#define BASE_IWeb INTERFACE_IWeb(=0;)
#define DERIVED_IWeb INTERFACE_IWeb(;)
	struct IWeb
	{
		BASE_IWeb;

	};



	enum Cursor {
		kCursor_Pointer,
		kCursor_Cross,
		kCursor_Hand,
		kCursor_IBeam,
		kCursor_Wait,
		kCursor_Help,
		kCursor_EastResize,
		kCursor_NorthResize,
		kCursor_NorthEastResize,
		kCursor_NorthWestResize,
		kCursor_SouthResize,
		kCursor_SouthEastResize,
		kCursor_SouthWestResize,
		kCursor_WestResize,
		kCursor_NorthSouthResize,
		kCursor_EastWestResize,
		kCursor_NorthEastSouthWestResize,
		kCursor_NorthWestSouthEastResize,
		kCursor_ColumnResize,
		kCursor_RowResize,
		kCursor_MiddlePanning,
		kCursor_EastPanning,
		kCursor_NorthPanning,
		kCursor_NorthEastPanning,
		kCursor_NorthWestPanning,
		kCursor_SouthPanning,
		kCursor_SouthEastPanning,
		kCursor_SouthWestPanning,
		kCursor_WestPanning,
		kCursor_Move,
		kCursor_VerticalText,
		kCursor_Cell,
		kCursor_ContextMenu,
		kCursor_Alias,
		kCursor_Progress,
		kCursor_NoDrop,
		kCursor_Copy,
		kCursor_None,
		kCursor_NotAllowed,
		kCursor_ZoomIn,
		kCursor_ZoomOut,
		kCursor_Grab,
		kCursor_Grabbing,
		kCursor_Custom
	};
//---------------------------------------------------------------
struct IWebViewListener;
#define INTERFACE_IWebView(T)\
	public:\
	virtual void loadURL(const char* surl)##T\
	virtual void injectKeyboardEvent(UINT uMsg, WPARAM wParam, LPARAM lParam)##T\
	virtual void injectMouseDown(int x,int y,int key)##T\
	virtual void injectMouseUp(int x,int y,int key)##T\
	virtual void injectMouseWheel(int delta)##T\
	virtual void injectMouseMove(int x,int y)##T\
	virtual void setListener(IWebViewListener* Listener)##T\
	virtual bool isDirty()##T\
	virtual void render(unsigned char* destination, int destRowSpan, int destDepth, RECT* renderedRect = 0)##T\
	virtual void focus()##T\
	virtual void unfocus()##T\
	virtual void destroy()##T\
	



#define BASE_IWebView INTERFACE_IWebView(=0;)
#define DERIVED_IWebView INTERFACE_IWebView(;)
	struct IWebView
	{
		BASE_IWebView;

	};



//---------------------------------------------------------------
typedef IWeb*(*GETIWEB)();


	struct IWebApi
	{
		GETIWEB getIWeb;
	};

typedef IWebApi*(WINAPI* GETIWEBINTERFACE)();
//---------------------------------------------------------------
//namespace ImplWarp { struct VariantValue; }
/*
struct JSValueWrap
{

	ImplWarp::VariantValue* varValue;
	typedef std::map<std::wstring, JSValueWrap> Object;
	typedef std::vector<JSValueWrap> Array;


};
*/
//typedef std::vector<JSValueWrap> JSArgumentsWarp;




#define INTERFACE_IWebViewListener(T)\
	public:\
	virtual void onChangeTitle(const wchar_t* Title)##T\
	virtual void onChangeAddressBar(const wchar_t* url)##T\
	virtual void onChangeTooltip(const wchar_t* Tooltip)##T\
	virtual void onChangeTargetURL(const wchar_t* url)##T\
	virtual void onChangeCursor(int cursor)##T\
	virtual void onChangeFocus(int type)##T

/*
	virtual void onBeginNavigation(const std::string& url, const std::wstring& frameName) ##T\
	virtual void onBeginLoading(const std::string& url, const std::wstring& frameName, int statusCode, const std::wstring& mimeType) ##T\
	virtual void onFinishLoading() ##T\
	virtual void onCallback(const std::wstring& objectName, const std::wstring& callbackName, const JSArgumentsWarp& args)##T\
	virtual void onReceiveTitle(const std::wstring& title, const std::wstring& frameName)##T\
	virtual void onChangeTooltip(const std::wstring& tooltip)##T\
	virtual void onChangeCursor(const HCURSOR& cursor) ##T\
	virtual void onChangeKeyboardFocus(bool isFocused)  ##T\
	virtual void onChangeTargetURL(const std::string& url)##T
	*/

#define BASE_IWebViewListener INTERFACE_IWebViewListener(=0;)
#define DERIVED_IWebViewListener INTERFACE_IWebViewListener(;)

	struct IWebViewListener
	{
		BASE_IWebViewListener;

	};
