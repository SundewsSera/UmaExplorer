#pragma once
#include "il2cpp_utils.h"
#include "common_utils.h"

void* material_get_shader_hook(void* _this);
void material_GetColor_hook(void* _this, int nameid, Color* ret);
Vector4 material_GetVector_hook(void* _this, int nameid);
float material_GetFloat_hook(void* _this, int nameid);

void setup_UnityEngine_Material_get_shader();
void setup_UnityEngine_Material_GetColor();
void setup_UnityEngine_Material_GetVector();
void setup_UnityEngine_Material_GetFloat();