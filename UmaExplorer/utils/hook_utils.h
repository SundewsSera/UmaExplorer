#pragma once

#include "../hooks/mscorlib/System/System_Type.hpp"
#include "../hooks/UnityEngine_CoreModule/UnityEngine/UnityEngine_Object.hpp"
#include "../hooks/mscorlib/System/System_Array.hpp"
#include "../hooks/UnityEngine_CoreModule/UnityEngine/UnityEngine_Component.hpp"
#include "../hooks/UnityEngine_CoreModule/UnityEngine/UnityEngine_GameObject.hpp"
#include "../hooks/UnityEngine_CoreModule/UnityEngine/UnityEngine_Transform.hpp"
#include "../hooks/mscorlib/System/System_Guid.hpp"
#include "../hooks/mscorlib/System/System_RuntimeType.hpp"
#include "../hooks/mscorlib/System_Reflection/System_Reflection_PropertyInfo.hpp"
#include "../hooks/mscorlib/System_Reflection/System_Reflection_Assembly.hpp"
#include "../hooks/UnityEngine_CoreModule/UnityEngine/UnityEngine_Material.hpp"
#include "../hooks/UnityEngine_CoreModule/UnityEngine/UnityEngine_MaterialPropertyBlock.hpp"

void setup_hook_utils();

//1. 在文件夹创建Hook文件，然后拖进VS
//2. 写好hook
//3. 在hook_utils.h中添加hook的头文件
//4. 在hook_utils.cpp中添加目标函数调用