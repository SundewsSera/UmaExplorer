#include "System_Array.hpp"

void* arrayindex_orig = nullptr;

void* arrayindex_hook(void* _this, long index)
{

	void* ret = reinterpret_cast<decltype(arrayindex_hook)*>(arrayindex_orig)(_this, index);

	return ret;

}

void setup_System_Array_GetValue()
{
	//获取获得Array内部数据的方法
	auto arrayindex_addr = il2cpp_symbols::get_method_pointer(
		"mscorlib.dll", "System",
		"Array", "GetValue", 1
	);
	printf("arrayindex_addr at %p\n", arrayindex_addr);
	MH_CreateHook((LPVOID)arrayindex_addr, arrayindex_hook, &arrayindex_orig);
	MH_EnableHook((LPVOID)arrayindex_addr);
}