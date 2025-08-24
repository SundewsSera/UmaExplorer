#include "UnityEngine_GameObject.hpp"

void* components_orig = nullptr;

void* components_hook(void* _this, void* type, bool useSearchTypeAsArrayReturnType, bool recursive, bool includeInactive, bool reverse, void* resultList)
{

	void* ret = reinterpret_cast<decltype(components_hook)*>(components_orig)(_this, type, useSearchTypeAsArrayReturnType, recursive, includeInactive, reverse, resultList);

	return ret;

}

void* activeSelf_orig = nullptr;

bool activeSelf_hook(void* _this)
{

	bool ret = reinterpret_cast<decltype(activeSelf_hook)*>(activeSelf_orig)(_this);

	return ret;

}

void* hierarchyActive_orig = nullptr;

bool hierarchyActive_hook(void* _this)
{

	bool ret = reinterpret_cast<decltype(hierarchyActive_hook)*>(hierarchyActive_orig)(_this);

	return ret;

}

void setup_UnityEngine_GameObject_GetComponentsInternal()
{
	//尝试获取Components

	auto components_addr = il2cpp_symbols::get_method_pointer(
		"UnityEngine.CoreModule.dll", "UnityEngine",
		"GameObject", "GetComponentsInternal", 6
	);

	printf("components_addr at %p\n", components_addr);

	MH_CreateHook((LPVOID)components_addr, components_hook, &components_orig);
	//MH_EnableHook((LPVOID)components_addr);
}

void setup_UnityEngine_GameObject_get_activeSelf()
{
	//获取object激活状态（真实激活状态）
	auto activeSelf_addr = il2cpp_symbols::get_method_pointer(
		"UnityEngine.CoreModule.dll", "UnityEngine",
		"GameObject", "get_activeSelf", 0
	);

	printf("activeSelf_addr at %p\n", activeSelf_addr);

	MH_CreateHook((LPVOID)activeSelf_addr, activeSelf_hook, &activeSelf_orig);
	//MH_EnableHook((LPVOID)activeSelf_addr);
}

void setup_UnityEngine_GameObject_get_activeInHierarchy()
{
	//获取object激活状态（如果父级不激活，显示为未激活）
	auto hierarchyActive_addr = il2cpp_symbols::get_method_pointer(
		"UnityEngine.CoreModule.dll", "UnityEngine",
		"GameObject", "get_activeInHierarchy", 0
	);

	printf("hierarchyActive_addr at %p\n", hierarchyActive_addr);

	MH_CreateHook((LPVOID)hierarchyActive_addr, hierarchyActive_hook, &hierarchyActive_orig);
	//MH_EnableHook((LPVOID)hierarchyActive_addr);
}