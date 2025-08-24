#pragma once
#include "il2cpp_utils.h"

void* components_hook(void* _this, void* type, bool useSearchTypeAsArrayReturnType = 1, bool recursive = 0, bool includeInactive = 1, bool reverse = 0, void* resultList = NULL);
bool activeSelf_hook(void* _this);
bool hierarchyActive_hook(void* _this);


void setup_UnityEngine_GameObject_GetComponentsInternal();
void setup_UnityEngine_GameObject_get_activeSelf();
void setup_UnityEngine_GameObject_get_activeInHierarchy();