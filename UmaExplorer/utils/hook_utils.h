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

//1. ���ļ��д���Hook�ļ���Ȼ���Ͻ�VS
//2. д��hook
//3. ��hook_utils.h�����hook��ͷ�ļ�
//4. ��hook_utils.cpp�����Ŀ�꺯������