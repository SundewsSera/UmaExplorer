#include "UnityEngine_Object.hpp"

void* object_orig = nullptr;

void* object_hook(void* type, bool includeInactive)
{

	void* ret = reinterpret_cast<decltype(object_hook)*>(object_orig)(type, includeInactive);

	return ret;

}

void* objectname_orig = nullptr;

void* objectname_hook(void* _this)
{

	void* ret = reinterpret_cast<decltype(objectname_hook)*>(objectname_orig)(_this);

	return ret;

}

void* obj_inst_orig = nullptr;

int obj_inst_hook(void* _this)
{

	int ret = reinterpret_cast<decltype(obj_inst_hook)*>(obj_inst_orig)(_this);

	return ret;

}

void* inst_obj_orig = nullptr;

void* inst_obj_hook(int instanceID)
{

	void* ret = reinterpret_cast<decltype(inst_obj_hook)*>(inst_obj_orig)(instanceID);

	return ret;

}

void setup_UnityEngine_Object_FindObjectsOfType() 
{
	//开始尝试获取Object

	auto object_addr = il2cpp_symbols::get_method_pointer(
		"UnityEngine.CoreModule.dll", "UnityEngine",
		"Object", "FindObjectsOfType", 2
	);

	printf("object_addr at %p\n", object_addr);

	MH_CreateHook((LPVOID)object_addr, object_hook, &object_orig);
	MH_EnableHook((LPVOID)object_addr);
}

void setup_UnityEngine_Object_ToString()
{
	//获取Object名称

	auto objectname_addr = il2cpp_symbols::get_method_pointer(
		"UnityEngine.CoreModule.dll", "UnityEngine",
		"Object", "ToString", 0
	);
	printf("objectname_addr at %p\n", objectname_addr);
	MH_CreateHook((LPVOID)objectname_addr, objectname_hook, &objectname_orig);
	MH_EnableHook((LPVOID)objectname_addr);
}

void setup_UnityEngine_Object_GetInstanceID()
{
	//从Object获取Instance
	auto obj_inst_addr = il2cpp_symbols::get_method_pointer(
		"UnityEngine.CoreModule.dll", "UnityEngine",
		"Object", "GetInstanceID", 0
	);

	printf("obj_inst_addr at %p\n", obj_inst_addr);

	MH_CreateHook((LPVOID)obj_inst_addr, obj_inst_hook, &obj_inst_orig);
	MH_EnableHook((LPVOID)obj_inst_addr);
}

void setup_UnityEngine_Object_FindObjectFromInstanceID()
{
	//从Instance获取Object
	auto inst_obj_addr = il2cpp_symbols::get_method_pointer(
		"UnityEngine.CoreModule.dll", "UnityEngine",
		"Object", "FindObjectFromInstanceID", 1
	);

	printf("inst_obj_addr at %p\n", inst_obj_addr);

	MH_CreateHook((LPVOID)inst_obj_addr, inst_obj_hook, &inst_obj_orig);
	MH_EnableHook((LPVOID)inst_obj_addr);
}

void* objType_orig = nullptr;

void* objType_hook(void* _this)
{

	void* ret = reinterpret_cast<decltype(objType_hook)*>(objType_orig)(_this);

	return ret;

}

void setup_UnityEngine_Object_GetType()
{
	//Object获取Type

	auto objType_addr = il2cpp_symbols::get_method_pointer(
		"mscorlib.dll", "System",
		"Object", "GetType", 0
	);

	printf("objType_addr at %p\n", objType_addr);

	MH_CreateHook((LPVOID)objType_addr, objType_hook, &objType_orig);
	MH_EnableHook((LPVOID)objType_addr);
}