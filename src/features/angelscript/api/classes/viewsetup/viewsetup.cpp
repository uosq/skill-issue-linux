#include "viewsetup.h"

#include "../../../../../sdk/definitions/cviewsetup.h"

#define VIEWSETUP_CLASSNAME "ViewSetup"

void ViewSetup_RegisterClass(asIScriptEngine *engine)
{
	engine->RegisterObjectType(VIEWSETUP_CLASSNAME, 0, asOBJ_REF | asOBJ_NOCOUNT);

	engine->RegisterObjectProperty(VIEWSETUP_CLASSNAME, "int x", asOFFSET(CViewSetup, x));
	engine->RegisterObjectProperty(VIEWSETUP_CLASSNAME, "int m_nUnscaledX", asOFFSET(CViewSetup, m_nUnscaledX));
	engine->RegisterObjectProperty(VIEWSETUP_CLASSNAME, "int y", asOFFSET(CViewSetup, y));
	engine->RegisterObjectProperty(VIEWSETUP_CLASSNAME, "int m_nUnscaledY", asOFFSET(CViewSetup, m_nUnscaledY));
	engine->RegisterObjectProperty(VIEWSETUP_CLASSNAME, "int width", asOFFSET(CViewSetup, width));
	engine->RegisterObjectProperty(VIEWSETUP_CLASSNAME, "int m_nUnscaledWidth",
				       asOFFSET(CViewSetup, m_nUnscaledWidth));
	engine->RegisterObjectProperty(VIEWSETUP_CLASSNAME, "int height", asOFFSET(CViewSetup, height));
	engine->RegisterObjectProperty(VIEWSETUP_CLASSNAME, "int m_nUnscaledHeight",
				       asOFFSET(CViewSetup, m_nUnscaledHeight));
	engine->RegisterObjectProperty(VIEWSETUP_CLASSNAME, "bool m_bOrtho", asOFFSET(CViewSetup, m_bOrtho));
	engine->RegisterObjectProperty(VIEWSETUP_CLASSNAME, "float m_OrthoLeft", asOFFSET(CViewSetup, m_OrthoLeft));
	engine->RegisterObjectProperty(VIEWSETUP_CLASSNAME, "float m_OrthoTop", asOFFSET(CViewSetup, m_OrthoTop));
	engine->RegisterObjectProperty(VIEWSETUP_CLASSNAME, "float m_OrthoRight", asOFFSET(CViewSetup, m_OrthoRight));
	engine->RegisterObjectProperty(VIEWSETUP_CLASSNAME, "float m_OrthoBottom", asOFFSET(CViewSetup, m_OrthoBottom));
	engine->RegisterObjectProperty(VIEWSETUP_CLASSNAME, "float fov", asOFFSET(CViewSetup, fov));
	engine->RegisterObjectProperty(VIEWSETUP_CLASSNAME, "float fovViewmodel", asOFFSET(CViewSetup, fovViewmodel));
	engine->RegisterObjectProperty(VIEWSETUP_CLASSNAME, "Vector3 origin", asOFFSET(CViewSetup, origin));
	engine->RegisterObjectProperty(VIEWSETUP_CLASSNAME, "Vector3 angles", asOFFSET(CViewSetup, angles));
	engine->RegisterObjectProperty(VIEWSETUP_CLASSNAME, "float zNear", asOFFSET(CViewSetup, zNear));
	engine->RegisterObjectProperty(VIEWSETUP_CLASSNAME, "float zFar", asOFFSET(CViewSetup, zFar));
	engine->RegisterObjectProperty(VIEWSETUP_CLASSNAME, "float zNearViewmodel",
				       asOFFSET(CViewSetup, zNearViewmodel));
	engine->RegisterObjectProperty(VIEWSETUP_CLASSNAME, "float zFarViewmodel", asOFFSET(CViewSetup, zFarViewmodel));
	engine->RegisterObjectProperty(VIEWSETUP_CLASSNAME, "bool m_bRenderToSubrectOfLargerScreen",
				       asOFFSET(CViewSetup, m_bRenderToSubrectOfLargerScreen));
	engine->RegisterObjectProperty(VIEWSETUP_CLASSNAME, "float m_flAspectRatio",
				       asOFFSET(CViewSetup, m_flAspectRatio));
	engine->RegisterObjectProperty(VIEWSETUP_CLASSNAME, "bool m_bOffCenter", asOFFSET(CViewSetup, m_bOffCenter));
	engine->RegisterObjectProperty(VIEWSETUP_CLASSNAME, "float m_flOffCenterTop",
				       asOFFSET(CViewSetup, m_flOffCenterTop));
	engine->RegisterObjectProperty(VIEWSETUP_CLASSNAME, "float m_flOffCenterBottom",
				       asOFFSET(CViewSetup, m_flOffCenterBottom));
	engine->RegisterObjectProperty(VIEWSETUP_CLASSNAME, "float m_flOffCenterLeft",
				       asOFFSET(CViewSetup, m_flOffCenterLeft));
	engine->RegisterObjectProperty(VIEWSETUP_CLASSNAME, "float m_flOffCenterRight",
				       asOFFSET(CViewSetup, m_flOffCenterRight));
	engine->RegisterObjectProperty(VIEWSETUP_CLASSNAME, "bool m_bDoBloomAndToneMapping",
				       asOFFSET(CViewSetup, m_bDoBloomAndToneMapping));
	engine->RegisterObjectProperty(VIEWSETUP_CLASSNAME, "bool m_bCacheFullSceneState",
				       asOFFSET(CViewSetup, m_bCacheFullSceneState));
	engine->RegisterObjectProperty(VIEWSETUP_CLASSNAME, "bool m_bViewToProjectionOverride",
				       asOFFSET(CViewSetup, m_bViewToProjectionOverride));
}