#include "hook_utils.h"

void setup_hook_utils()
{
	setup_System_Type_GetType();
	setup_UnityEngine_Object_FindObjectsOfType();
	setup_UnityEngine_Object_ToString();
	setup_System_Array_GetValue();
	setup_UnityEngine_Component_get_gameObject();
	setup_UnityEngine_GameObject_GetComponentsInternal();
	setup_UnityEngine_Transform_get_childCount();
	setup_UnityEngine_Transform_GetChild();
	setup_UnityEngine_Object_GetInstanceID();
	setup_UnityEngine_Transform_GetParent();
	setup_UnityEngine_GameObject_get_activeSelf();
	setup_UnityEngine_GameObject_get_activeInHierarchy();
	setup_UnityEngine_Object_FindObjectFromInstanceID();
	setup_System_Guid_ToString();
	setup_UnityEngine_Transform_get_localPosition();
	setup_UnityEngine_Transform_get_localEulerAngles();
	setup_UnityEngine_Transform_get_localScale();
	setup_UnityEngine_Object_GetType();
	setup_System_RuntimeType_GetProperties();
	setup_System_Reflection_PropertyInfo_GetValue();
	setup_System_Reflection_Assembly_Load();
	setup_System_Reflection_Assembly_GetType();
	setup_System_Type_GetProperty();
	setup_UnityEngine_Material_get_shader();
	setup_UnityEngine_Material_GetColor();
	setup_UnityEngine_Material_GetVector();
	setup_UnityEngine_Material_GetFloat();
	setup_UnityEngine_MaterialPropertyBlock_GetFloat();
	setup_UnityEngine_MaterialPropertyBlock_GetColor();
	setup_UnityEngine_MaterialPropertyBlock_GetVector();
}