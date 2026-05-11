#pragma once

#include "../feature.h"

class InfoPanel
{
public:
	void OnImGui(bool bMenuOpen);
};

DECLARE_FEATURE(InfoPanel, infopanel)