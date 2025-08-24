#include "System_RuntimeType.hpp"

void* typePros_orig = nullptr;

void* typePros_hook(void* _this, int flag)
{

	void* ret = reinterpret_cast<decltype(typePros_hook)*>(typePros_orig)(_this, flag);

	return ret;

}

void setup_System_RuntimeType_GetProperties()
{
	//TypeªÒ»°properties

	auto typePros_addr = il2cpp_symbols::get_method_pointer(
		"mscorlib.dll", "System",
		"RuntimeType", "GetProperties", 1
	);

	printf("typePros_addr at %p\n", typePros_addr);

	MH_CreateHook((LPVOID)typePros_addr, typePros_hook, &typePros_orig);
	MH_EnableHook((LPVOID)typePros_addr);
}