#pragma once
#include "il2cpp_utils.h"
#include "common_utils.h"

int childcount_hook(void* _this);
void* child_hook(void* _this, int index);
void* parent_hook(void* _this);
Vector3 position_hook(void* _this);
Vector3 rotation_hook(void* _this);
Vector3 scale_hook(void* _this);

void setup_UnityEngine_Transform_get_childCount();
void setup_UnityEngine_Transform_GetChild();
void setup_UnityEngine_Transform_GetParent();
void setup_UnityEngine_Transform_get_localPosition();
void setup_UnityEngine_Transform_get_localEulerAngles();
void setup_UnityEngine_Transform_get_localScale();