#pragma once

#include <cstdint>
#include "ibaseinterface.h"

class SurfacePlat;
class IClientPanel;
class Panel;
class KeyValues;

//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Basic header for using vgui
//
// $NoKeywords: $
//=============================================================================//

typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned int   uint;

typedef uintptr_t VPANEL;

// handles to vgui objects
// NULL values signify an invalid value
typedef unsigned long HScheme;
// Both -1 and 0 are used for invalid textures. Be careful.
typedef unsigned long HTexture;
typedef unsigned long HCursor;
typedef unsigned long HPanel;
const HPanel INVALID_PANEL = 0xffffffff;
typedef unsigned long HFont;
const HFont INVALID_FONT = 0; // the value of an invalid font handle

enum EInterfaceID
{
	ICLIENTPANEL_STANDARD_INTERFACE = 0,
};

class IForceVirtualInheritancePanel
{
	// We need Panel to use virtual inheritance so that
	// pointers to its members are max size.
	// This is due to a limitation in C++ with ahead
	// declarations of points to members as used in MessageMap.
};

class IClientPanel
{
public:
	virtual VPANEL GetVPanel() = 0;

	// straight interface to Panel functions
	virtual void Think() = 0;
	virtual void PerformApplySchemeSettings() = 0;
	virtual void PaintTraverse(bool forceRepaint, bool allowForce) = 0;
	virtual void Repaint() = 0;
	virtual VPANEL IsWithinTraverse(int x, int y, bool traversePopups) = 0;
	virtual void GetInset(int &top, int &left, int &right, int &bottom) = 0;
	virtual void GetClipRect(int &x0, int &y0, int &x1, int &y1) = 0;
	virtual void OnChildAdded(VPANEL child) = 0;
	virtual void OnSizeChanged(int newWide, int newTall) = 0;

	virtual void InternalFocusChanged(bool lost) = 0;
	virtual bool RequestInfo(KeyValues *outputData) = 0;
	virtual void RequestFocus(int direction) = 0;
	virtual bool RequestFocusPrev(VPANEL existingPanel) = 0;
	virtual bool RequestFocusNext(VPANEL existingPanel) = 0;
	virtual void OnMessage(const KeyValues *params, VPANEL ifromPanel) = 0;
	virtual VPANEL GetCurrentKeyFocus() = 0;
	virtual int GetTabPosition() = 0;

	// for debugging purposes
	virtual const char *GetName() = 0;
	virtual const char *GetClassName() = 0;

	// get scheme handles from panels
	virtual HScheme GetScheme() = 0;
	// gets whether or not this panel should scale with screen resolution
	virtual bool IsProportional() = 0;
	// auto-deletion
	virtual bool IsAutoDeleteSet() = 0;
	// deletes this
	virtual void DeletePanel() = 0;

	// interfaces
	virtual void *QueryInterface(EInterfaceID id) = 0;

	// returns a pointer to the vgui controls baseclass Panel *
	virtual Panel *GetPanel() = 0;

	// returns the name of the module this panel is part of
	virtual const char *GetModuleName() = 0;

	virtual void OnTick() = 0;
};

class IPanel : public IBaseInterface
{
public:
	virtual void Init(VPANEL vguiPanel, IClientPanel *panel) = 0;

	// methods
	virtual void SetPos(VPANEL vguiPanel, int x, int y) = 0;
	virtual void GetPos(VPANEL vguiPanel, int &x, int &y) = 0;
	virtual void SetSize(VPANEL vguiPanel, int wide,int tall) = 0;
	virtual void GetSize(VPANEL vguiPanel, int &wide, int &tall) = 0;
	virtual void SetMinimumSize(VPANEL vguiPanel, int wide, int tall) = 0;
	virtual void GetMinimumSize(VPANEL vguiPanel, int &wide, int &tall) = 0;
	virtual void SetZPos(VPANEL vguiPanel, int z) = 0;
	virtual int  GetZPos(VPANEL vguiPanel) = 0;

	virtual void GetAbsPos(VPANEL vguiPanel, int &x, int &y) = 0;
	virtual void GetClipRect(VPANEL vguiPanel, int &x0, int &y0, int &x1, int &y1) = 0;
	virtual void SetInset(VPANEL vguiPanel, int left, int top, int right, int bottom) = 0;
	virtual void GetInset(VPANEL vguiPanel, int &left, int &top, int &right, int &bottom) = 0;

	virtual void SetVisible(VPANEL vguiPanel, bool state) = 0;
	virtual bool IsVisible(VPANEL vguiPanel) = 0;
	virtual void SetParent(VPANEL vguiPanel, VPANEL newParent) = 0;
	virtual int GetChildCount(VPANEL vguiPanel) = 0;
	virtual VPANEL GetChild(VPANEL vguiPanel, int index) = 0;
	virtual void* &GetChildren( VPANEL vguiPanel ) = 0;
	virtual VPANEL GetParent(VPANEL vguiPanel) = 0;
	virtual void MoveToFront(VPANEL vguiPanel) = 0;
	virtual void MoveToBack(VPANEL vguiPanel) = 0;
	virtual bool HasParent(VPANEL vguiPanel, VPANEL potentialParent) = 0;
	virtual bool IsPopup(VPANEL vguiPanel) = 0;
	virtual void SetPopup(VPANEL vguiPanel, bool state) = 0;
	virtual bool IsFullyVisible( VPANEL vguiPanel ) = 0;

	// gets the scheme this panel uses
	virtual HScheme GetScheme(VPANEL vguiPanel) = 0;
	// gets whether or not this panel should scale with screen resolution
	virtual bool IsProportional(VPANEL vguiPanel) = 0;
	// returns true if auto-deletion flag is set
	virtual bool IsAutoDeleteSet(VPANEL vguiPanel) = 0;
	// deletes the Panel * associated with the vpanel
	virtual void DeletePanel(VPANEL vguiPanel) = 0;

	// input interest
	virtual void SetKeyBoardInputEnabled(VPANEL vguiPanel, bool state) = 0;
	virtual void SetMouseInputEnabled(VPANEL vguiPanel, bool state) = 0;
	virtual bool IsKeyBoardInputEnabled(VPANEL vguiPanel) = 0;
	virtual bool IsMouseInputEnabled(VPANEL vguiPanel) = 0;

	// calculates the panels current position within the hierarchy
	virtual void Solve(VPANEL vguiPanel) = 0;

	// gets names of the object (for debugging purposes)
	virtual const char *GetName(VPANEL vguiPanel) = 0;
	virtual const char *GetClassName(VPANEL vguiPanel) = 0;

	// delivers a message to the panel
	virtual void SendMessage(VPANEL vguiPanel, KeyValues *params, VPANEL ifromPanel) = 0;

	// these pass through to the IClientPanel
	virtual void Think(VPANEL vguiPanel) = 0;
	virtual void PerformApplySchemeSettings(VPANEL vguiPanel) = 0;
	virtual void PaintTraverse(VPANEL vguiPanel, bool forceRepaint, bool allowForce = true) = 0;
	virtual void Repaint(VPANEL vguiPanel) = 0;
	virtual VPANEL IsWithinTraverse(VPANEL vguiPanel, int x, int y, bool traversePopups) = 0;
	virtual void OnChildAdded(VPANEL vguiPanel, VPANEL child) = 0;
	virtual void OnSizeChanged(VPANEL vguiPanel, int newWide, int newTall) = 0;

	virtual void InternalFocusChanged(VPANEL vguiPanel, bool lost) = 0;
	virtual bool RequestInfo(VPANEL vguiPanel, KeyValues *outputData) = 0;
	virtual void RequestFocus(VPANEL vguiPanel, int direction = 0) = 0;
	virtual bool RequestFocusPrev(VPANEL vguiPanel, VPANEL existingPanel) = 0;
	virtual bool RequestFocusNext(VPANEL vguiPanel, VPANEL existingPanel) = 0;
	virtual VPANEL GetCurrentKeyFocus(VPANEL vguiPanel) = 0;
	virtual int GetTabPosition(VPANEL vguiPanel) = 0;

	// used by ISurface to store platform-specific data
	virtual SurfacePlat *Plat(VPANEL vguiPanel) = 0;
	virtual void SetPlat(VPANEL vguiPanel, SurfacePlat *Plat) = 0;

	// returns a pointer to the vgui controls baseclass Panel *
	// destinationModule needs to be passed in to verify that the returned Panel * is from the same module
	// it must be from the same module since Panel * vtbl may be different in each module
	virtual Panel *GetPanel(VPANEL vguiPanel, const char *destinationModule) = 0;

	virtual bool IsEnabled(VPANEL vguiPanel) = 0;
	virtual void SetEnabled(VPANEL vguiPanel, bool state) = 0;

	// Used by the drag/drop manager to always draw on top
	virtual bool IsTopmostPopup( VPANEL vguiPanel) = 0;
	virtual void SetTopmostPopup( VPANEL vguiPanel, bool state ) = 0;

	// sibling pins
	virtual void SetSiblingPin(VPANEL vguiPanel, VPANEL newSibling, uint8_t iMyCornerToPin = 0, uint8_t iSiblingCornerToPinTo = 0 ) = 0;

};

class Panel: public IClientPanel, virtual public IForceVirtualInheritancePanel
{

};