#include "System_Reflection_Assembly.hpp"

void* refload_orig = nullptr;

void* refload_hook(void* assemblyString)
{

	void* ret = reinterpret_cast<decltype(refload_hook)*>(refload_orig)(assemblyString);

	return ret;

}

void setup_System_Reflection_Assembly_Load()
{
	//���ظ��������ڵ�Dll(����Hook����)

	auto refload_addr = il2cpp_symbols::get_method_pointer(
		"mscorlib.dll", "System.Reflection",
		"Assembly", "Load", 1
	);

	printf("refload_addr at %p\n", refload_addr);

	MH_CreateHook((LPVOID)refload_addr, refload_hook, &refload_orig);
	MH_EnableHook((LPVOID)refload_addr);
}

void* reftype_orig = nullptr;

void* reftype_hook(void* _this, void* searchType)
{

	void* ret = reinterpret_cast<decltype(reftype_hook)*>(reftype_orig)(_this, searchType);

	return ret;

}

void setup_System_Reflection_Assembly_GetType()
{
	//ͨ�������ȡType(����Hook����)

	auto reftype_addr = il2cpp_symbols::get_method_pointer(
		"mscorlib.dll", "System.Reflection",
		"Assembly", "GetType", 1
	);

	printf("reftype_addr at %p\n", reftype_addr);

	MH_CreateHook((LPVOID)reftype_addr, reftype_hook, &reftype_orig);
	MH_EnableHook((LPVOID)reftype_addr);
}