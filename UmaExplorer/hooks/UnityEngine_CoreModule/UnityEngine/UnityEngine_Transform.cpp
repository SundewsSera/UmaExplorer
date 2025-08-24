#include "UnityEngine_Transform.hpp"

void* childcount_orig = nullptr;

int childcount_hook(void* _this)
{

	int ret = reinterpret_cast<decltype(childcount_hook)*>(childcount_orig)(_this);

	return ret;

}

void* child_orig = nullptr;

void* child_hook(void* _this, int index)
{

	void* ret = reinterpret_cast<decltype(child_hook)*>(child_orig)(_this, index);

	return ret;

}

void* parent_orig = nullptr;

void* parent_hook(void* _this)
{

	void* ret = reinterpret_cast<decltype(parent_hook)*>(parent_orig)(_this);

	return ret;

}

void* position_orig = nullptr;

Vector3 position_hook(void* _this)
{

	Vector3 ret = reinterpret_cast<decltype(position_hook)*>(position_orig)(_this);

	return ret;

}

void* rotation_orig = nullptr;

Vector3 rotation_hook(void* _this)
{

	Vector3 ret = reinterpret_cast<decltype(rotation_hook)*>(rotation_orig)(_this);

	return ret;

}

void setup_UnityEngine_Transform_get_childCount()
{
	//获取Transform的子节点个数

	auto childcount_addr = il2cpp_symbols::get_method_pointer(
		"UnityEngine.CoreModule.dll", "UnityEngine",
		"Transform", "get_childCount", 0
	);

	printf("childcount_addr at %p\n", childcount_addr);

	MH_CreateHook((LPVOID)childcount_addr, childcount_hook, &childcount_orig);
	MH_EnableHook((LPVOID)childcount_addr);
}

void setup_UnityEngine_Transform_GetChild()
{
	//根据index获取child的子节点
	auto child_addr = il2cpp_symbols::get_method_pointer(
		"UnityEngine.CoreModule.dll", "UnityEngine",
		"Transform", "GetChild", 1
	);

	printf("child_addr at %p\n", child_addr);

	MH_CreateHook((LPVOID)child_addr, child_hook, &child_orig);
	MH_EnableHook((LPVOID)child_addr);
}

void setup_UnityEngine_Transform_GetParent()
{
	//获取父节点

	auto parent_addr = il2cpp_symbols::get_method_pointer(
		"UnityEngine.CoreModule.dll", "UnityEngine",
		"Transform", "GetParent", 0
	);

	printf("parent_addr at %p\n", parent_addr);

	MH_CreateHook((LPVOID)parent_addr, parent_hook, &parent_orig);
	MH_EnableHook((LPVOID)parent_addr);
}

void setup_UnityEngine_Transform_get_localPosition()
{
	//从Transform获取本地Position
	auto position_addr = il2cpp_symbols::get_method_pointer(
		"UnityEngine.CoreModule.dll", "UnityEngine",
		"Transform", "get_localPosition", 0
	);

	printf("position_addr at %p\n", position_addr);

	MH_CreateHook((LPVOID)position_addr, position_hook, &position_orig);
	MH_EnableHook((LPVOID)position_addr);
}

void setup_UnityEngine_Transform_get_localEulerAngles()
{
	//从Transform获取本地欧拉旋转
	auto rotation_addr = il2cpp_symbols::get_method_pointer(
		"UnityEngine.CoreModule.dll", "UnityEngine",
		"Transform", "get_localEulerAngles", 0
	);

	printf("rotation_addr at %p\n", rotation_addr);

	MH_CreateHook((LPVOID)rotation_addr, rotation_hook, &rotation_orig);
	MH_EnableHook((LPVOID)rotation_addr);
}

void* scale_orig = nullptr;

Vector3 scale_hook(void* _this)
{

	Vector3 ret = reinterpret_cast<decltype(scale_hook)*>(scale_orig)(_this);

	return ret;

}

void setup_UnityEngine_Transform_get_localScale()
{
	//从Transform获取本地缩放
	auto scale_addr = il2cpp_symbols::get_method_pointer(
		"UnityEngine.CoreModule.dll", "UnityEngine",
		"Transform", "get_localScale", 0
	);

	printf("scale_addr at %p\n", scale_addr);

	MH_CreateHook((LPVOID)scale_addr, scale_hook, &scale_orig);
	MH_EnableHook((LPVOID)scale_addr);
}