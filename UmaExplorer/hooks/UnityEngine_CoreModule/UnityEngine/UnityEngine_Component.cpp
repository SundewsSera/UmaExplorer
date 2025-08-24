#include "UnityEngine_Component.hpp"

void* gameobject_orig = nullptr;

void* gameobject_hook(void* _this)
{

	void* ret = reinterpret_cast<decltype(gameobject_hook)*>(gameobject_orig)(_this);

	return ret;

}

void setup_UnityEngine_Component_get_gameObject()
{
	//¥”TransformªÒ»°GameObject

	auto gameobject_addr = il2cpp_symbols::get_method_pointer(
		"UnityEngine.CoreModule.dll", "UnityEngine",
		"Component", "get_gameObject", 0
	);

	printf("gameobject_addr at %p\n", gameobject_addr);

	MH_CreateHook((LPVOID)gameobject_addr, gameobject_hook, &gameobject_orig);
	MH_EnableHook((LPVOID)gameobject_addr);
}