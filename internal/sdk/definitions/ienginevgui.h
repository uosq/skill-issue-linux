#pragma once

#include <cstdint>
#include "isurface.h"

enum VGuiPanel_t
{
	PANEL_ROOT = 0,
	PANEL_GAMEUIDLL,
	PANEL_CLIENTDLL,
	PANEL_TOOLS,
	PANEL_INGAMESCREENS,
	PANEL_GAMEDLL,
	PANEL_CLIENTDLL_TOOLS,
	PANEL_SIZING,
};

// In-game panels are cropped to the current engine viewport size
enum PaintMode_t
{
	PAINT_UIPANELS		= (1<<0),
	PAINT_INGAMEPANELS  = (1<<1),
	PAINT_CURSOR		= (1<<2), // software cursor, if appropriate
};

class IEngineVGui
{
public:
	virtual		~IEngineVGui( void ) { }
	virtual VPANEL	GetPanel( VGuiPanel_t type ) = 0;
	virtual bool	IsGameUIVisible() = 0;
	virtual bool	GetWorkshopMap( uint32_t uIndex, void *pDesc ) = 0;
};

class IEngineVGuiInternal : public IEngineVGui
{
public:
	virtual void Init() = 0;
	virtual void Connect() = 0;
	virtual void Shutdown() = 0;
	virtual bool SetVGUIDirectories() = 0;
	virtual bool IsInitialized() const = 0;
	virtual CreateInterfaceFn GetGameUIFactory() = 0;
	virtual bool Key_Event( const void** event ) = 0;
	virtual void BackwardCompatibility_Paint() = 0;
	virtual void UpdateButtonState( const void** event ) = 0;
	virtual void PostInit() = 0;

    	virtual void Paint( PaintMode_t mode ) = 0;

	// handlers for game UI (main menu)
	virtual void ActivateGameUI() = 0;
	virtual bool HideGameUI() = 0;
	virtual bool IsGameUIVisible() = 0;

	// console
	virtual void ShowConsole() = 0;
	virtual void HideConsole() = 0;
	virtual bool IsConsoleVisible() = 0;
	virtual void ClearConsole() = 0;

	virtual void HideDebugSystem() = 0;

	// level loading
	virtual void OnLevelLoadingStarted() = 0;
	virtual void OnLevelLoadingFinished() = 0;
	virtual void NotifyOfServerConnect(const char *game, int IP, int connectionPort, int queryPort) = 0;
	virtual void NotifyOfServerDisconnect() = 0;
	virtual void EnabledProgressBarForNextLoad() = 0;
	virtual void UpdateProgressBar(int progress) = 0;
	virtual void UpdateCustomProgressBar( float progress, const wchar_t *desc ) = 0;
	virtual void StartCustomProgress() = 0;
	virtual void FinishCustomProgress() = 0;
	virtual void ShowErrorMessage() = 0;

	// Should pause?
	virtual bool ShouldPause() = 0;
	virtual void SetGameDLLPanelsVisible( bool show ) = 0;
	virtual void ShowNewGameDialog( int chapter ) = 0;

	virtual void Simulate() = 0;

	virtual void SetNotAllowedToHideGameUI( bool bNotAllowedToHide ) = 0;
	virtual void SetNotAllowedToShowGameUI( bool bNotAllowedToShow ) = 0;

	// Xbox 360
	virtual void SessionNotification( const int notification, const int param = 0 ) = 0;
	virtual void SystemNotification( const int notification ) = 0;
	virtual void ShowMessageDialog( const uint nType, Panel *pOwner = NULL ) = 0;
	virtual void UpdatePlayerInfo( uint64_t nPlayerId, const char *pName, int nTeam, byte cVoiceState, int nPlayersNeeded, bool bHost ) = 0;
	virtual void SessionSearchResult( int searchIdx, void *pHostData, void *pResult, int ping ) = 0;
	virtual void OnCreditsFinished( void ) = 0;

	// Storage device validation:
	//		returns true right away if storage device has been previously selected.
	//		otherwise returns false and will set the variable pointed by pStorageDeviceValidated to 1
	//				  once the storage device is selected by user.
	virtual bool ValidateStorageDevice( int *pStorageDeviceValidated ) = 0;

	virtual void ConfirmQuit( void ) = 0;
};