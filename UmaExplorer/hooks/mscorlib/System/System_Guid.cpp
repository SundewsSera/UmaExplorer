#include "System_Guid.hpp"

void* guid_orig = nullptr;

void* guid_hook(void* _this)
{

	void* ret = reinterpret_cast<decltype(guid_hook)*>(guid_orig)(_this);

	return ret;

}

void setup_System_Guid_ToString()
{
	//GUIDתstring

	auto guid_addr = il2cpp_symbols::get_method_pointer(
		"mscorlib.dll", "System",
		"Guid", "ToString", 0
	);

	printf("guid_addr at %p\n", guid_addr);

	MH_CreateHook((LPVOID)guid_addr, guid_hook, &guid_orig);
	MH_EnableHook((LPVOID)guid_addr);
}