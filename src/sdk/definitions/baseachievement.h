#pragma once

#include "cgameeventlistener.h"
#include "iachievementmgr.h"

class CBaseEntity;

class CBaseAchievement : public CGameEventListener, public IAchievement
{
	CBaseAchievement();
	virtual ~CBaseAchievement();
	virtual void Init() {}
	virtual void ListenForEvents() {};
	virtual void Event_EntityKilled( CBaseEntity *pVictim, CBaseEntity *pAttacker, CBaseEntity *pInflictor, IGameEvent *event );

	int GetAchievementID() { return m_iAchievementID; }
	void SetAchievementID( int iAchievementID ) { m_iAchievementID = iAchievementID; }
	void SetName( const char *pszName ) { m_pszName = pszName; }
	const char *GetName() { return m_pszName; }
	const char *GetStat() { return m_pszStat?m_pszStat:GetName(); }
	int GetFlags() { return m_iFlags; }
	void SetGoal( int iGoal ) { m_iGoal = iGoal; }
	int GetGoal() { return m_iGoal; }
	void SetGameDirFilter( const char *pGameDir );
	bool HasComponents() { return ( m_iFlags & ACH_HAS_COMPONENTS ) > 0; }
	void SetPointValue( int iPointValue ) { m_iPointValue = iPointValue; }
	int	GetPointValue() { return m_iPointValue; }
	bool ShouldHideUntilAchieved() { return m_bHideUntilAchieved; }
	void SetHideUntilAchieved( bool bHide ) { m_bHideUntilAchieved = bHide; }
	void SetStoreProgressInSteam( bool bStoreProgressInSteam ) { m_bStoreProgressInSteam = bStoreProgressInSteam; }
	bool StoreProgressInSteam() { return m_bStoreProgressInSteam; }

	virtual bool ShouldShowProgressNotification() { return true; }
	virtual void OnPlayerStatsUpdate() {}

	virtual bool ShouldSaveWithGame();
	virtual void PreRestoreSavedGame();
	virtual void PostRestoreSavedGame();

	void SetCount( int iCount ) { m_iCount = iCount; }
	int GetCount() { return m_iCount; }
	void SetProgressShown( int iProgressShown ) { m_iProgressShown = iProgressShown; }
	int GetProgressShown() { return m_iProgressShown; }

	virtual bool IsAchieved() { return m_bAchieved; }
	virtual bool IsActive();
	virtual bool LocalPlayerCanEarn( void ) { return true; }
	void SetAchieved( bool bAchieved ) { m_bAchieved = bAchieved; }
	virtual bool IsMetaAchievement() { return false; }
	virtual bool AlwaysListen() { return false; }
	virtual bool AlwaysEnabled() { return false; }
	virtual void OnAchieved() {}
	uint32 GetUnlockTime() const { return m_uUnlockTime; }
	void SetUnlockTime( uint32 unlockTime ) { m_uUnlockTime = unlockTime; }
	virtual void OnMapEvent( const char *pEventName );
	virtual void PrintAdditionalStatus() {}
	virtual void OnSteamUserStatsStored() {}
	virtual void UpdateAchievement( int nData ) {}
	virtual bool ShouldShowOnHUD() { return m_bShowOnHUD; }
	virtual void SetShowOnHUD( bool bShow );
	virtual void GetSettings( KeyValues* pNodeOut );				// serialize
	virtual void ApplySettings( /* const */ KeyValues* pNodeIn );	// unserialize
	virtual void Think( void ) { return; }
	const char *GetMapNameFilter( void ){ return m_pMapNameFilter; }
	IAchievementMgr *GetAchievementMgr( void ){ return m_pAchievementMgr; }

protected:
	virtual void FireGameEvent( IGameEvent *event );
	virtual void FireGameEvent_Internal( IGameEvent *event ) {};
	virtual void CalcProgressMsgIncrement();

	const char *m_pszName;								// name of this achievement
	const char *m_pszStat;								// stat this achievement uses
	int m_iAchievementID;								// ID of this achievement
	int	m_iFlags;										// ACH_* flags for this achievement
	int	m_iGoal;										// goal # of steps to award this achievement
	int m_iProgressMsgIncrement;						// after how many steps show we show a progress notification
	int m_iProgressMsgMinimum;							// the minimum progress needed before showing progress notification
	int m_iPointValue;									// # of points this achievement is worth (currently only used for XBox Live)
	bool m_bHideUntilAchieved;							// should this achievement be hidden until achieved?
	bool m_bStoreProgressInSteam;						// should incremental progress be stored in Steam.  A counter with same name as achievement must be set up in Steam.
	const char *m_pInflictorClassNameFilter;			// if non-NULL, inflictor class name to filter with
	const char *m_pInflictorEntityNameFilter;			// if non-NULL, inflictor entity name to filter with
	const char *m_pVictimClassNameFilter;				// if non-NULL, victim class name to filter with
	const char *m_pAttackerClassNameFilter;				// if non-NULL, attacker class name to filter with
	const char *m_pMapNameFilter;						// if non-NULL, map name to filter with
	const char *m_pGameDirFilter;						// if non-NULL, game dir name to filter with

	const char **m_pszComponentNames;
	int			m_iNumComponents;
	const char *m_pszComponentPrefix;
	int			m_iComponentPrefixLen;
	bool		m_bAchieved;							// is this achievement achieved
	uint32		m_uUnlockTime;							// time_t that this achievement was unlocked (0 if before Steamworks unlock time support)
	int			m_iCount;								// # of steps satisfied toward this achievement (only valid if not achieved)
	int			m_iProgressShown;						// # of progress msgs we've shown
	uint64		m_iComponentBits;						// bitfield of components achieved
	IAchievementMgr *m_pAchievementMgr;					// our achievement manager
	bool		m_bShowOnHUD;							// if set, the player wants this achievement pinned to the HUD

	friend class IAchievementMgr;
};