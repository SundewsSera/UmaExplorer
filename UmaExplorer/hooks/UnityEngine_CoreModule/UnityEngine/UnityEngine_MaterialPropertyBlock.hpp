#pragma once
#include "il2cpp_utils.h"
#include "common_utils.h"

void* MaterialPropertyBlock_ctor();

float MaterialPropertyBlock_GetFloat_hook(void* _this, int nameid);
Color MaterialPropertyBlock_GetColor_hook(void* _this, int nameid);
Vector4 MaterialPropertyBlock_GetVector_hook(void* _this, int nameid);

void setup_UnityEngine_MaterialPropertyBlock_GetFloat();
void setup_UnityEngine_MaterialPropertyBlock_GetColor();
void setup_UnityEngine_MaterialPropertyBlock_GetVector();