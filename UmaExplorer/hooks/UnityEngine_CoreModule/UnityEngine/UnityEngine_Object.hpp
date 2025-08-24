#pragma once
#include "il2cpp_utils.h"

void* object_hook(void* type, bool includeInactive);
void* objectname_hook(void* _this);
int obj_inst_hook(void* _this);
void* inst_obj_hook(int instanceID);
void* objType_hook(void* _this);

void setup_UnityEngine_Object_FindObjectsOfType();
void setup_UnityEngine_Object_ToString();
void setup_UnityEngine_Object_GetInstanceID();
void setup_UnityEngine_Object_FindObjectFromInstanceID();
void setup_UnityEngine_Object_GetType();