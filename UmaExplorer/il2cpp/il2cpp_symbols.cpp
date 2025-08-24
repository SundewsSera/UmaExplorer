#include <Windows.h>
#include <shlobj.h>

#include <cinttypes>

#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <locale>
#include <string>
#include <thread>
#include <unordered_map>

#include <MinHook.h>

#include "il2cpp_symbols.hpp"

#include "il2cpp_symbols_resolver.hpp"


extern bool g_dump_entries;
extern bool g_enable_logger;
extern bool g_enable_console;
extern int g_max_fps;
extern bool g_unlock_size;
extern float g_ui_scale;
extern float g_aspect_ratio;
extern bool g_replace_font;
extern bool g_auto_fullscreen;

il2cpp_string_new_utf16_t il2cpp_string_new_utf16;
il2cpp_string_new_t il2cpp_string_new;
il2cpp_domain_get_t il2cpp_domain_get;
il2cpp_domain_assembly_open_t il2cpp_domain_assembly_open;
il2cpp_assembly_get_image_t il2cpp_assembly_get_image;
il2cpp_class_from_name_t il2cpp_class_from_name;
il2cpp_class_from_system_type_t il2cpp_class_from_system_type;
il2cpp_object_get_class_t il2cpp_object_get_class;
//Field
il2cpp_class_get_fields_t il2cpp_class_get_fields;
il2cpp_field_get_name_t il2cpp_field_get_name;
il2cpp_field_get_type_t il2cpp_field_get_type;
il2cpp_type_get_name_t il2cpp_type_get_name;
il2cpp_class_get_parent_t il2cpp_class_get_parent;
il2cpp_class_get_type_t il2cpp_class_get_type;
il2cpp_field_get_value_t il2cpp_field_get_value;
il2cpp_field_get_value_object_t il2cpp_field_get_value_object;
il2cpp_type_get_object_t il2cpp_type_get_object;
//

il2cpp_class_get_properties_t il2cpp_class_get_properties;
il2cpp_property_get_name_t il2cpp_property_get_name;
il2cpp_property_get_get_method_t il2cpp_property_get_get_method;
il2cpp_property_get_set_method_t il2cpp_property_get_set_method;
il2cpp_method_get_return_type_t il2cpp_method_get_return_type;
il2cpp_method_get_declaring_type_t il2cpp_method_get_declaring_type;

il2cpp_class_is_enum_t il2cpp_class_is_enum;
il2cpp_type_get_class_or_element_class_t il2cpp_type_get_class_or_element_class;
il2cpp_class_from_type_t il2cpp_class_from_type;

il2cpp_class_get_methods_t il2cpp_class_get_methods;
il2cpp_class_get_method_from_name_t il2cpp_class_get_method_from_name;
il2cpp_method_get_param_t il2cpp_method_get_param;
il2cpp_object_new_t il2cpp_object_new;
il2cpp_resolve_icall_t il2cpp_resolve_icall;
il2cpp_array_new_t il2cpp_array_new;
il2cpp_thread_attach_t il2cpp_thread_attach;
il2cpp_thread_detach_t il2cpp_thread_detach;
il2cpp_image_get_class_count_t il2cpp_image_get_class_count;
il2cpp_image_get_class_t il2cpp_image_get_class;
il2cpp_class_get_name_t il2cpp_class_get_name;
il2cpp_class_get_field_from_name_t il2cpp_class_get_field_from_name;
il2cpp_field_get_offset_t il2cpp_field_get_offset;

il2cpp_method_get_name_t il2cpp_method_get_name;
il2cpp_class_is_valuetype_t il2cpp_class_is_valuetype;
il2cpp_class_get_element_class_t il2cpp_class_get_element_class;

il2cpp_method_get_param_count_t il2cpp_method_get_param_count;
il2cpp_runtime_invoke_t il2cpp_runtime_invoke;


il2cpp_array_length_t il2cpp_array_length;

char* il2cpp_array_addr_with_size(void* array, int32_t size, uintptr_t idx)
{
	return ((char*)array) + kIl2CppSizeOfArray + size * idx;
}

namespace il2cpp_symbols
{
#define RESOLVE_IMPORT(name) name = reinterpret_cast<name##_t>(GetProcAddress(game_module, symbol_map[#name].c_str()))

	void* il2cpp_domain = nullptr;

	void init(HMODULE game_module)
	{
		std::unordered_map<std::string, std::string> symbol_map = resolve_il2cpp();

		RESOLVE_IMPORT(il2cpp_string_new_utf16);
		RESOLVE_IMPORT(il2cpp_string_new);
		RESOLVE_IMPORT(il2cpp_domain_get);
		RESOLVE_IMPORT(il2cpp_domain_assembly_open);
		RESOLVE_IMPORT(il2cpp_assembly_get_image);
		RESOLVE_IMPORT(il2cpp_class_from_name);
		RESOLVE_IMPORT(il2cpp_class_from_system_type);
		RESOLVE_IMPORT(il2cpp_object_get_class);

		RESOLVE_IMPORT(il2cpp_class_get_fields);
		RESOLVE_IMPORT(il2cpp_field_get_name);
		RESOLVE_IMPORT(il2cpp_field_get_type);
		RESOLVE_IMPORT(il2cpp_type_get_name);
		RESOLVE_IMPORT(il2cpp_class_get_parent);
		RESOLVE_IMPORT(il2cpp_class_get_type);
		RESOLVE_IMPORT(il2cpp_field_get_value);
		RESOLVE_IMPORT(il2cpp_field_get_value_object);
		RESOLVE_IMPORT(il2cpp_type_get_object);

		RESOLVE_IMPORT(il2cpp_class_get_properties);
		RESOLVE_IMPORT(il2cpp_property_get_name);
		RESOLVE_IMPORT(il2cpp_property_get_get_method);
		RESOLVE_IMPORT(il2cpp_property_get_set_method);
		RESOLVE_IMPORT(il2cpp_method_get_return_type);
		RESOLVE_IMPORT(il2cpp_method_get_declaring_type);

		RESOLVE_IMPORT(il2cpp_class_is_enum);
		RESOLVE_IMPORT(il2cpp_type_get_class_or_element_class);
		RESOLVE_IMPORT(il2cpp_class_from_type);

		RESOLVE_IMPORT(il2cpp_class_get_methods);
		RESOLVE_IMPORT(il2cpp_class_get_method_from_name);
		RESOLVE_IMPORT(il2cpp_method_get_param);
		RESOLVE_IMPORT(il2cpp_object_new);
		RESOLVE_IMPORT(il2cpp_resolve_icall);
		RESOLVE_IMPORT(il2cpp_array_new);
		RESOLVE_IMPORT(il2cpp_thread_attach);
		RESOLVE_IMPORT(il2cpp_thread_detach);
		RESOLVE_IMPORT(il2cpp_image_get_class_count);
		RESOLVE_IMPORT(il2cpp_image_get_class);
		RESOLVE_IMPORT(il2cpp_class_get_name);
		RESOLVE_IMPORT(il2cpp_class_get_field_from_name);
		RESOLVE_IMPORT(il2cpp_field_get_offset);
		RESOLVE_IMPORT(il2cpp_array_length);
		RESOLVE_IMPORT(il2cpp_method_get_name);
		RESOLVE_IMPORT(il2cpp_class_is_valuetype);
		RESOLVE_IMPORT(il2cpp_class_get_element_class);

		RESOLVE_IMPORT(il2cpp_method_get_param_count);
		RESOLVE_IMPORT(il2cpp_runtime_invoke);

		il2cpp_domain = il2cpp_domain_get();
	}

	std::string utf8_encode(const std::wstring& in)
	{
		if (in.empty()) return std::string();
		int size = WideCharToMultiByte(CP_UTF8, 0, &in[0], in.size(), NULL, 0, NULL, NULL);
		std::string dst(size, 0);
		WideCharToMultiByte(CP_UTF8, 0, &in[0], in.size(), &dst[0], size, NULL, NULL);
		return dst;
	}

	std::wstring il2cpp_get_wstring(Il2CppString* str)
	{
		if (!str) return L"";  // ¿ÕÖ¸Õë·µ»Ø¿Õ×Ö·û´®
		return std::wstring(str->chars, str->length);
	}

	std::string il2cpp_get_string(void* str) {
		return utf8_encode(il2cpp_get_wstring(reinterpret_cast<Il2CppString*>(str)));
	}

	void* get_il2cpp_string(const char* str) {
		return il2cpp_string_new(str);
	}

	uint32_t get_array_length(void* array) {
		return il2cpp_array_length(array);
	}

	void class_print_all_method(void* klass)
	{

		void* iter = nullptr;
		while (const MethodInfo* method = il2cpp_class_get_methods(klass, &iter))
		{
			printf("method name is %s \n", il2cpp_method_get_name(method));
			//printf("method address is %p \n", method->methodPointer);
			//printf("method arg count is %p \n", method->parameters_count);
		}

		return;
	}

	size_t get_image_class_count(void* image)
	{
		size_t num = il2cpp_image_get_class_count(image);
		printf("length is %p \n", num);
		return num;
	}

	void* image_get_class(void* image, size_t index)
	{
		auto klass = il2cpp_image_get_class(image, index);
		printf("get class from image is %p \n", klass);
		return klass;
	}

	char* class_get_name(void* klass)
	{
		char* name = il2cpp_class_get_name(klass);
		printf("class get name is %s \n", name);
		return name;
	}

	void* get_class(const char* assemblyName, const char* namespaze, const char* klassName)
	{
		auto assembly = il2cpp_domain_assembly_open(il2cpp_domain, assemblyName);
		auto image = il2cpp_assembly_get_image(assembly);
		return il2cpp_class_from_name(image, namespaze, klassName);
	}

	void* new_object(void* klass) {
		return il2cpp_object_new(klass);
	}

	uintptr_t get_method_pointer(const char* assemblyName, const char* namespaze,
		const char* klassName, const char* name, int argsCount, int index)
	{
		auto assembly = il2cpp_domain_assembly_open(il2cpp_domain, assemblyName);
		//printf("get dll success %p\n", assembly);
		auto image = il2cpp_assembly_get_image(assembly);
		//printf("get image success %p\n", image);
		//auto new_klass = image_get_class(image, 3648);
		//class_get_name(new_klass);
		void* klass;
		if (index != -1) {
			klass = image_get_class(image, index);
		}
		else {
			klass = il2cpp_class_from_name(image, namespaze, klassName);
			//class_get_name(klass);
			//class_get_method(klass);
		}		
		//class_get_method(klass);
		//printf("get class success %p\n", klass);

		return il2cpp_class_get_method_from_name(klass, name, argsCount)->methodPointer;
	}

	MethodInfo* get_method(const char* assemblyName, const char* namespaze,
		const char* klassName, const char* name, int argsCount)
	{
		auto assembly = il2cpp_domain_assembly_open(il2cpp_domain, assemblyName);
		auto image = il2cpp_assembly_get_image(assembly);
		auto klass = il2cpp_class_from_name(image, namespaze, klassName);

		return il2cpp_class_get_method_from_name(klass, name, argsCount);
	}

	uintptr_t find_method(const char* assemblyName, const char* namespaze,
		const char* klassName, std::function<bool(const MethodInfo*)> predict)
	{
		auto assembly = il2cpp_domain_assembly_open(il2cpp_domain, assemblyName);
		auto image = il2cpp_assembly_get_image(assembly);
		auto klass = il2cpp_class_from_name(image, namespaze, klassName);

		void* iter = nullptr;
		while (const MethodInfo* method = il2cpp_class_get_methods(klass, &iter))
		{
			if (predict(method))
				return method->methodPointer;
		}

		return 0;
	}

	uintptr_t get_method_pointer_base(const char* assemblyName, const char* namespaze,
		const char* klassName, const char* name, int argsCount, size_t offset)
	{
		auto assembly = il2cpp_domain_assembly_open(il2cpp_domain, assemblyName);
		auto image = il2cpp_assembly_get_image(assembly);
		auto klass = il2cpp_class_from_name(image, namespaze, klassName);

		auto base = (uintptr_t)((size_t)(il2cpp_class_get_method_from_name(klass, name, argsCount)->methodPointer) - offset);

		printf("method pointer base is %p \n", base);

		return base;
	}

	size_t get_field_offset(const char* assemblyName, const char* namespaze,
		const char* klassName, const char* name)
	{
		auto assembly = il2cpp_domain_assembly_open(il2cpp_domain, assemblyName);
		auto image = il2cpp_assembly_get_image(assembly);
		auto klass = il2cpp_class_from_name(image, namespaze, klassName);
		auto field = il2cpp_class_get_field_from_name(klass, name);

		return il2cpp_field_get_offset(field);
	}

	void* new_array(void* elementTypeInfo, size_t length) {
		return il2cpp_array_new(elementTypeInfo, length);
	}

	uintptr_t class_get_method_re(void* klass, const char* name, int argsCount, int index)
	{

		void* iter = nullptr;
		int count = 0;
		while (const MethodInfo* method = il2cpp_class_get_methods(klass, &iter))
		{
			//printf("Method is:%s\n", method->name);
			if (strcmp(il2cpp_method_get_name(method), name) == 0 && il2cpp_method_get_param_count(method) == argsCount) {
				count += 1;
			}
			if (count == index) {
				return method->methodPointer;
			}
		}

		return 0;
	}

	uintptr_t get_method_pointer_re(const char* assemblyName, const char* namespaze,
		const char* klassName, const char* name, int argsCount, int index)
	{
		auto assembly = il2cpp_domain_assembly_open(il2cpp_domain, assemblyName);
		auto image = il2cpp_assembly_get_image(assembly);
		void* klass = il2cpp_class_from_name(image, namespaze, klassName);
		uintptr_t method = class_get_method_re(klass, name, argsCount, index);

		return method;
	}

	void* get_type_class(void* type) {
		return il2cpp_class_from_system_type(type);
	}

	uintptr_t class_get_method(void* klass, const char* name, int argsCount) {
		return il2cpp_class_get_method_from_name(klass, name, argsCount)->methodPointer;
	}

	void* object_get_class(void* obj) {
		return il2cpp_object_get_class(obj);
	}

}