#pragma once
#include "il2cpp_utils.h"

void* refload_hook(void* assemblyString);
void* reftype_hook(void* _this, void* searchType);

void setup_System_Reflection_Assembly_Load();
void setup_System_Reflection_Assembly_GetType();