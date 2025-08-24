#include "UnityEngine_MaterialPropertyBlock.hpp"

void* MaterialPropertyBlock_ctor()
{
	void* klass = il2cpp_symbols::get_class("UnityEngine.CoreModule.dll", "UnityEngine", "MaterialPropertyBlock");
	//printf("klass is %p\n", klass);
	void* obj = il2cpp_object_new(klass);
	//printf("obj is %p\n", obj);

	void* ctor = il2cpp_class_get_method_from_name(klass, ".ctor", 0);
	//printf(".ctor is %p\n", obj);
	il2cpp_runtime_invoke(ctor, obj, nullptr, nullptr);
	return obj;
}

void* MaterialPropertyBlock_GetFloat_orig = nullptr;

float MaterialPropertyBlock_GetFloat_hook(void* _this, int nameid)
{
	float ret = reinterpret_cast<decltype(MaterialPropertyBlock_GetFloat_hook)*>(MaterialPropertyBlock_GetFloat_orig)(_this, nameid);
	return ret;
}

void setup_UnityEngine_MaterialPropertyBlock_GetFloat()
{
	//从Material获取Float
	auto MaterialPropertyBlock_GetFloat_addr = il2cpp_symbols::get_method_pointer_re(
		"UnityEngine.CoreModule.dll", "UnityEngine",
		"MaterialPropertyBlock", "GetFloat", 1, 2
	);

	printf("MaterialPropertyBlock_GetFloat_addr at %p\n", MaterialPropertyBlock_GetFloat_addr);
	MH_CreateHook((LPVOID)MaterialPropertyBlock_GetFloat_addr, MaterialPropertyBlock_GetFloat_hook, &MaterialPropertyBlock_GetFloat_orig);
	//MH_EnableHook((LPVOID)material_GetFloat_addr);
}

void* MaterialPropertyBlock_GetColor_orig = nullptr;

Color MaterialPropertyBlock_GetColor_hook(void* _this, int nameid)
{
	
	Color ret = reinterpret_cast<decltype(MaterialPropertyBlock_GetColor_hook)*>(MaterialPropertyBlock_GetColor_orig)(_this, nameid);
	return ret;
}

void setup_UnityEngine_MaterialPropertyBlock_GetColor()
{
	//从Material获取Color
	auto MaterialPropertyBlock_GetColor_addr = il2cpp_symbols::get_method_pointer_re(
		"UnityEngine.CoreModule.dll", "UnityEngine",
		"MaterialPropertyBlock", "GetColor", 1, 2
	);

	printf("MaterialPropertyBlock_GetColor_addr at %p\n", MaterialPropertyBlock_GetColor_addr);

	MH_CreateHook((LPVOID)MaterialPropertyBlock_GetColor_addr, MaterialPropertyBlock_GetColor_hook, &MaterialPropertyBlock_GetColor_orig);
	//MH_EnableHook((LPVOID)MaterialPropertyBlock_GetColor_addr);
}

void* MaterialPropertyBlock_GetVector_orig = nullptr;

Vector4 MaterialPropertyBlock_GetVector_hook(void* _this, int nameid)
{
	Vector4 ret = reinterpret_cast<decltype(MaterialPropertyBlock_GetVector_hook)*>(MaterialPropertyBlock_GetVector_orig)(_this, nameid);
	return ret;
}

void setup_UnityEngine_MaterialPropertyBlock_GetVector()
{
	//从Material获取Vector
	auto MaterialPropertyBlock_GetVector_addr = il2cpp_symbols::get_method_pointer_re(
		"UnityEngine.CoreModule.dll", "UnityEngine",
		"MaterialPropertyBlock", "GetVector", 1, 2
	);

	printf("MaterialPropertyBlock_GetVector_addr at %p\n", MaterialPropertyBlock_GetVector_addr);
	MH_CreateHook((LPVOID)MaterialPropertyBlock_GetVector_addr, MaterialPropertyBlock_GetVector_hook, &MaterialPropertyBlock_GetVector_orig);
	//MH_EnableHook((LPVOID)MaterialPropertyBlock_GetVector_addr);
}