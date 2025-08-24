#include "UnityEngine_Material.hpp"

void* material_get_shader_orig = nullptr;

void* material_get_shader_hook(void* _this)
{

	void* ret = reinterpret_cast<decltype(material_get_shader_hook)*>(material_get_shader_orig)(_this);

	return ret;

}

void setup_UnityEngine_Material_get_shader()
{
	//从Material获取Shader
	auto material_get_shader_addr = il2cpp_symbols::get_method_pointer(
		"UnityEngine.CoreModule.dll", "UnityEngine",
		"Material", "get_shader", 0
	);

	printf("material_get_shader_addr at %p\n", material_get_shader_addr);
	MH_CreateHook((LPVOID)material_get_shader_addr, material_get_shader_hook, &material_get_shader_orig);
	MH_EnableHook((LPVOID)material_get_shader_addr);
}

void* material_GetColor_orig = nullptr;

void material_GetColor_hook(void* _this, int nameid, Color* ret)
{
	reinterpret_cast<decltype(material_GetColor_hook)*>(material_GetColor_orig)(_this, nameid, ret);
}

void setup_UnityEngine_Material_GetColor()
{
	//从Material获取Color
	auto material_GetColor_addr = il2cpp_resolve_icall("UnityEngine.Material::GetColorImpl_Injected(System.Int32, UnityEngine.Color)");

	printf("material_GetColor_addr at %p\n", material_GetColor_addr);
	MH_CreateHook((LPVOID)material_GetColor_addr, material_GetColor_hook, &material_GetColor_orig);
	MH_EnableHook((LPVOID)material_GetColor_addr);
}

void* material_GetVector_orig = nullptr;

Vector4 material_GetVector_hook(void* _this, int nameid)
{
	Vector4 ret = reinterpret_cast<decltype(material_GetVector_hook)*>(material_GetVector_orig)(_this, nameid);
	return ret;
}

void setup_UnityEngine_Material_GetVector()
{
	//从Material获取Vector
	auto material_GetVector_addr = il2cpp_symbols::get_method_pointer_re(
		"UnityEngine.CoreModule.dll", "UnityEngine",
		"Material", "GetVector", 1, 2
	);

	printf("material_GetVector_addr at %p\n", material_GetVector_addr);
	MH_CreateHook((LPVOID)material_GetVector_addr, material_GetVector_hook, &material_GetVector_orig);
	MH_EnableHook((LPVOID)material_GetVector_addr);
}

void* material_GetFloat_orig = nullptr;

float material_GetFloat_hook(void* _this, int nameid)
{
	float ret = reinterpret_cast<decltype(material_GetFloat_hook)*>(material_GetFloat_orig)(_this, nameid);
	return ret;
}

void setup_UnityEngine_Material_GetFloat()
{
	//从Material获取Float
	auto material_GetFloat_addr = il2cpp_symbols::get_method_pointer_re(
		"UnityEngine.CoreModule.dll", "UnityEngine",
		"Material", "GetFloat", 1, 2
	);

	printf("material_GetFloat_addr at %p\n", material_GetFloat_addr);
	MH_CreateHook((LPVOID)material_GetFloat_addr, material_GetFloat_hook, &material_GetFloat_orig);
	MH_EnableHook((LPVOID)material_GetFloat_addr);
}