#include "System_Type.hpp"

void* type_orig = nullptr;

void* type_hook(void* _typename)
{

	void* ret = reinterpret_cast<decltype(type_hook)*>(type_orig)(_typename);

	return ret;

}

void setup_System_Type_GetType()
{
	// 获取Type.GetType方法的指针
	auto type_addr = il2cpp_symbols::get_method_pointer(
		"mscorlib.dll", "System",
		"Type", "GetType", 1
	);

	printf("type_addr at %p\n", type_addr);

	MH_CreateHook((LPVOID)type_addr, type_hook, &type_orig);
	MH_EnableHook((LPVOID)type_addr);
}

void* typePro_orig = nullptr;

void* typePro_hook(void* _this, void* name)
{

	void* ret = reinterpret_cast<decltype(typePro_hook)*>(typePro_orig)(_this, name);

	return ret;

}

void setup_System_Type_GetProperty()
{
	//Type获取property

	auto typePro_addr = il2cpp_symbols::get_method_pointer(
		"mscorlib.dll", "System",
		"Type", "GetProperty", 1
	);

	printf("typePro_addr at %p\n", typePro_addr);

	MH_CreateHook((LPVOID)typePro_addr, typePro_hook, &typePro_orig);
	MH_EnableHook((LPVOID)typePro_addr);
}