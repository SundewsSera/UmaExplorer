#include "System_Reflection_PropertyInfo.hpp"

void* property_orig = nullptr;

void* property_hook(void* _this, void* obj)
{

	void* ret = reinterpret_cast<decltype(property_hook)*>(property_orig)(_this, obj);

	return ret;

}

void setup_System_Reflection_PropertyInfo_GetValue()
{
	//根据PropertyInfo获取值

	auto property_addr = il2cpp_symbols::get_method_pointer(
		"mscorlib.dll", "System.Reflection",
		"PropertyInfo", "GetValue", 1
	);

	printf("property_addr at %p\n", property_addr);

	MH_CreateHook((LPVOID)property_addr, property_hook, &property_orig);
	MH_EnableHook((LPVOID)property_addr);
}