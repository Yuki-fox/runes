#pragma	   once

// **********************************************************************************************
// Globals
class	CUIOptionsFrame;

extern	CUIOptionsFrame				*g_pUIOptionsFrame;
// **********************************************************************************************

void	RegisterUIOptionsFrameFunc();

class CUIOptionsFrame
{
public:
	CUIOptionsFrame();
	~CUIOptionsFrame();

	static void				Initial();
	static void				Release();

public:
	bool					m_isHitMove;
	bool					m_isAutoFaceToFace;
	bool					m_isCameraFollow;
};