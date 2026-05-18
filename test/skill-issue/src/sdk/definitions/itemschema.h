#pragma once

#include "../../libsigscan.h"

void* GetItemSchema();
void* ItemSchema_GetItemDefinition(int iItemDefinitionIndex);
const char* ItemDefinition_GetBaseName(int iItemDefinitionIndex);