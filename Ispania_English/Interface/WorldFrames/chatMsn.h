#define NUM_MAX_CHATMSN_INFO 10
enum MSN_STATE
{
	MS_NOME,
	MS_POPUP_IN,
	MS_POPUPED,
	MS_POPUP_OUT,
	MS_OPEN,
	//	MS_SMALL,
	//	MS_SMALL_FLASH
};
enum MSN_TYPE
{
	MT_WHISPER,
	MT_YELL,
	MT_ZONE,
	MT_PARTY,
	MT_GUILD,
	MT_CHANNEL,
	MT_COUNT
};
struct MsgLine;
class CUiFrame;
class CUiFontString;
class CUiSlider;
struct ChatMsnInfo
{
	int  m_ChildSizeW;
	int  m_ChildSizeH;
	int m_fontSize;
	MsgLine* m_data;
	vector<string> m_members;
	bool m_used;
	int m_channel;
	MSN_TYPE m_MsnType;
	MSN_STATE m_state;
	string m_target;
	int m_id;
	string m_msg;
	string m_channelName;
	string m_channelPassword;
	int m_maxLines;
	int m_usedLines;
	int m_scroll_count;
	int m_bottomLine;
	int m_inputLines;
	int m_topLine;
	CUiFrame* m_frame;
	CUiFontString* m_font;
	CUiSlider* m_scrollBar;
	float m_alpha;
	float m_tempX;
	float m_tempY;
	float m_x;
	float m_y;
	//int m_mode;
	float m_time;
	ChatMsnInfo();
	int GetMemberCount(void);
	const char* MemberGetName(int index);
	void SetNameAndPassword(string& name, string& password);
	void GetNameAndPassword(string& name, string& password);
	void SetSize(int w, int h);
	void SetFontSize(int fsize);
	int GetFontSize(void);
	void SetType(MSN_TYPE type);
	void SetState(MSN_STATE state);
	void Initial(int maxLines, int id);
	void AddMember(const char* who);
	void DelMember(const char* who);
	void Finish(void);
	void SetMaxLines(int maxLines);
	void PushLine(LPCWSTR msg, int icolor);
	void PopLine(void);
	void SetBottomLine(int line);
	bool AutoLeaveChannel(void);
	void ClearAllLine();
	void SetPopup(int x, int y, const char* who, const char* msg/*,ChatMsnInfo* pren*/);
	void Update(float delate);
	void SetTellTarget(const char* playername);
	void SetChannel(int ChannelNo);
	MSN_TYPE GetType(void);
	void Close();
	void SetOpen(int x, int y);
	bool AddMessage(LPCSTR msg, int color);
	bool AddMessage(LPCWSTR msg, int color);
	void Update();
	const char* GetChannelOwner(void);
	const char* GetChannelName(void);
	const char* GetChannelPass(void);
	void ClearAllMsg();
};
class CInterface;
class CUi;
class ChatListManager;
class MsnManager
{
public:
	MsnManager(CInterface* object);
	~MsnManager();
	void Button_OnMouseUp(CUiFrame* uiFrame, int lParam, int rParam);
	void ItemLink_OnMouseUp(CUiFrame* uiFrame, int lParam, int rParam);
	void ItemLink_OnMouseEnter(CUiFrame* uiFrame, int lParam, int rParam);
	void ItemLink_OnMouseLeave(CUiFrame* uiFrame, int lParam, int rParam);
	void NameLink_OnMouseUp(CUiFrame* uiFrame, int lParam, int rParam);
	void CreateChannelOK(const char* name, int channelNo);
	void JoinChannelOK(const char* name, int channelNo, const char* pass);
	void ChangeChannelOwner(const char* name, int channelNo, const char* LeaderName);
	void DelChannelMember(const char* name, int channelNo, const char* Name);
	void AddChannelMember(const char* name, int channelNo, const char* Name);
	void ReflashList(int msnid);
	void CloseMsn(const char* name);
	int ListGetCount(void);
	int MemberGetCount(int id);
	const char* MemberGetName(int id, int index);
	void ListGetInfo(int index, int& _type, int& state, std::string& name, bool& add, int& mode);
	void ListSetToggle(int index);
	void InviteListOK(void);
	void SetFontSize(int index, int fsize);
	int GetFontSize(int index);
	void InviteChannel(int ChannelID, const char* Name);
	static void CopyToClipboard(const char* Text);
	void Initial(CUi* ui);
	void Finish(void);
	void Update(float elapsedTime);
	ChatMsnInfo* GetMsnInfo(int id);
	void SetMsnInfoOpen(int id);
	void SetMsnClose(int id);
	const char* GetChannelOwner(int index);
	const char* GetChannelName(int index);
	const char* GetChannelPass(int index);
	void SetAutoCheck(int type, bool cb);
	static void RegC2C(void);
	void SetSelfName(const char* name);
	string m_selfName;
	ChatMsnInfo* GetFreeInfo(int& index);
	void HaveNewWhisper(const char* who, const char* msg, int type);
	void HaveNewChannel(int msnType, int channelNo, const char* who, const char* msg, int type, const char* ChannleName = NULL, const char* passWord = NULL);
	ChatMsnInfo* FindMsn(const char* who, int& index);
	ChatMsnInfo* FindMsn(int msnType, int ChannelId, int& index);
	ChatMsnInfo* FindChannel(const char* name, int& index);
	void UpdateChatList(int index);
	ChatListManager* m_pListManager;
	CInterface* m_iface;
	ChatMsnInfo* m_pTop;
	bool m_autoOpen[MT_COUNT];
	ChatMsnInfo	m_msnfo[NUM_MAX_CHATMSN_INFO];
};