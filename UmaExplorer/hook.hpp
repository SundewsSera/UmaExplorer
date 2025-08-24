#pragma once

void* key_hook(void* dst);
void* localkey_hook(void* _this);
int Shader_GetPropertyCount_hook(void* _this);
void* Shader_GetPropertyName_hook(void* _this, int index);
int Shader_GetPropertyType_hook(void* _this, int index);
int Shader_GetPropertyNameId_hook(void* _this, int index);
void Renderer_GetPropertyBlock_hook(void* _this, void* dst);
bool MaterialPropertyBlock_HasPropertyImpl_hook(void* _this, int nameId);

//void* gameobject_hook(void* _this);