#include <windows.h>
#include "uiinput.h"

// --------------------------------------------------------------------------------------
CUiInput::CUiInput()
{
	memset( m_state, 0, sizeof(m_state) );
	memset( m_button, 0, sizeof(m_button) );
	memset( m_keyboardPushedTime, 0, sizeof(m_keyboardPushedTime) );
	memset( m_lastKeyboardPushedTime, 0, sizeof(m_lastKeyboardPushedTime) );
	memset( m_buttonPushedTime, 0, sizeof(m_buttonPushedTime) );
	memset( m_lastButtonPushedTime, 0, sizeof(m_lastButtonPushedTime) );

	m_xPos		= 0;
	m_yPos		= 0;
	m_xDelta	= 0.0f;
	m_yDelta	= 0.0f;
	m_zDelta	= 0.0f;
}

// --------------------------------------------------------------------------------------
CUiInput::~CUiInput()
{
}

// --------------------------------------------------------------------------------------
void CUiInput::Clear()
{
	memset( m_state, 0, sizeof(m_state) );
	memset( m_button, 0, sizeof(m_button) );
	memset( m_keyboardPushedTime, 0, sizeof(m_keyboardPushedTime) );
	memset( m_lastKeyboardPushedTime, 0, sizeof(m_lastKeyboardPushedTime) );
	memset( m_buttonPushedTime, 0, sizeof(m_buttonPushedTime) );
	memset( m_lastButtonPushedTime, 0, sizeof(m_lastButtonPushedTime) );

	m_xPos		= 0;
	m_yPos		= 0;
	m_xDelta	= 0.0f;
	m_yDelta	= 0.0f;
	m_zDelta	= 0.0f;
}

// --------------------------------------------------------------------------------------
void CUiInput::UpdateState(InputState &state, float elapsedTime)
{
	for ( int i = 0; i < 256; i++ )
	{
		m_state[i] = GetKeyFlag( m_state[i], state.key[i] );
		if (m_state[i] & eKey_Press && !(m_state[i] & eKey_Down))
			m_keyboardPushedTime[i] += elapsedTime;
		else
		{
			if ( m_keyboardPushedTime[i] != 0.0f )
				m_lastKeyboardPushedTime[i] = m_keyboardPushedTime[i];
			m_keyboardPushedTime[i] = 0.0f;
		}
	}

	for ( int i = 0; i < 4; i++ )
	{
		m_button[i] = GetKeyFlag( m_button[i], state.button[i] );
		if (m_button[i] & eKey_Press && !(m_button[i] & eKey_Down))
			m_buttonPushedTime[i] += elapsedTime;
		else
		{
			if ( m_buttonPushedTime[i] != 0.0f )
				m_lastButtonPushedTime[i] = m_buttonPushedTime[i];
			m_buttonPushedTime[i] = 0.0f;
		}
	}

	m_xOldPos	= m_xPos;
	m_yOldPos	= m_yPos;
	m_xPos		= state.xPos;
	m_yPos		= state.yPos;		
	m_xDelta	= state.xDelta;
	m_yDelta	= state.yDelta;
	m_zDelta	= state.zDelta;
}	

// --------------------------------------------------------------------------------------
bool CUiInput::GetKeyDown( BYTE num )
{
	if ( m_state[num] & eKey_Down && !(m_state[num] & eKey_DownLock) )
		return true;
	return false;
}	

// --------------------------------------------------------------------------------------
bool CUiInput::GetKeyUp( BYTE num )
{
	if ( m_state[num] & eKey_Up && !(m_state[num] & eKey_UpLock) )
		return true;
	return false;
}

// --------------------------------------------------------------------------------------
void CUiInput::LockKeyDown( BYTE num )
{
	m_state[num] |= eKey_DownLock;
}

// --------------------------------------------------------------------------------------
void CUiInput::LockKeyUp( BYTE num )
{
	m_state[num] |= eKey_UpLock;
}	

// --------------------------------------------------------------------------------------
bool CUiInput::GetButtonDown( BYTE num )
{
	if ( m_button[num] & eKey_Down && !(m_button[num] & eKey_DownLock) )
		return true;
	return false;
}

// --------------------------------------------------------------------------------------
bool CUiInput::GetButtonUp( BYTE num )
{
	if ( m_button[num] & eKey_Up && !(m_button[num] & eKey_UpLock) )
		return true;
	return false;
}

// --------------------------------------------------------------------------------------
void CUiInput::LockButtonDown( BYTE num )
{
	m_button[num] |= eKey_DownLock;
}

// --------------------------------------------------------------------------------------
void CUiInput::LockButtonUp( BYTE num )
{
	m_button[num] |= eKey_UpLock;
}	

// --------------------------------------------------------------------------------------
BYTE CUiInput::GetKeyFlag( BYTE keyState, bool state )
{
	char tmp = 0;

	if ( state )
	{
		tmp |= eKey_Press;
		if ( !(keyState & eKey_Press) )
			tmp |= eKey_Down;
	}
	else if ( keyState & eKey_Press )
		tmp |= eKey_Up;

	return tmp;
}