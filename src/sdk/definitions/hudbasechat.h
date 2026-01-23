#pragma once

#include "chudlement.h"
#include "color.h"
#include "editablepanel.h"
#include "ipanel.h"

class CBaseHudChat : public CHudElement, public EditablePanel
{
public:
	enum
	{
		CHAT_INTERFACE_LINES = 6,
		MAX_CHARS_PER_LINE = 128
	};

	CBaseHudChat( const char *pElementName );

	virtual void	CreateChatInputLine( void );
	virtual void	CreateChatLines( void );
	
	virtual void	Init( void );

	void			LevelInit( const char *newmap );
	void			LevelShutdown( void );

	void			MsgFunc_TextMsg(const char *pszName, int iSize, void *pbuf);
	
	virtual void	Printf( int iFilter, const char *fmt, ... );
	virtual void	ChatPrintf( int iPlayerIndex, int iFilter, const char *fmt, ... );
	
	virtual void	StartMessageMode( int iMessageModeType );
	virtual void	StopMessageMode( void );
	void			Send( void );

	virtual void	ApplySchemeSettings(void* *pScheme);
	virtual void	Paint( void );
	virtual void	OnTick( void );
	virtual void	Reset();
	Panel		*GetInputPanel( void );

	static int		m_nLineCounter;

	virtual int		GetChatInputOffset( void );

	// IGameEventListener interface:
	virtual void FireGameEvent( IGameEvent *event);

	void			*GetChatHistory();

	void					FadeChatHistory();
	float					m_flHistoryFadeTime;
	float					m_flHistoryIdleTime;

	virtual void			MsgFunc_SayText( bf_read &msg );
	virtual void			MsgFunc_SayText2( bf_read &msg );
	virtual void			MsgFunc_TextMsg( bf_read &msg );
	virtual void			MsgFunc_VoiceSubtitle( bf_read &msg );

	
	void	*GetChatInput( void ) { return m_pChatInput; }
	void		*GetChatFilterPanel( void );

	virtual int				GetFilterFlags( void ) { return m_iFilterFlags; }
	void					SetFilterFlag( int iFilter );

	//-----------------------------------------------------------------------------
	virtual Color	GetDefaultTextColor( void );
	virtual Color	GetTextColorForClient( Color colorNum, int clientIndex );
	virtual Color	GetClientColor( int clientIndex );

	virtual int		GetFilterForString( const char *pString );

	virtual const char *GetDisplayedSubtitlePlayerName( int clientIndex );

	bool			IsVoiceSubtitle( void ) { return m_bEnteringVoice; }
	void			SetVoiceSubtitleState( bool bState ) { m_bEnteringVoice = bState; }
	int				GetMessageMode( void ) { return m_nMessageMode; }

	void			SetCustomColor( Color colNew ) { m_ColorCustom = colNew; }
	void			SetCustomColor( const char *pszColorName );

protected:
	void		*FindUnusedChatLine( void );

	void	*m_pChatInput;
	void		*m_ChatLine;
	int					m_iFontHeight;

	void			*m_pChatHistory;

	void	*m_pFiltersButton;
	void		*m_pFilterPanel;

	Color			m_ColorCustom;

private:	
	void			Clear( void );

	int				ComputeBreakChar( int width, const char *text, int textlen );

	int				m_nMessageMode;

	int				m_nVisibleHeight;

	HFont		m_hChatFont;

	int				m_iFilterFlags;
	bool			m_bEnteringVoice;

};