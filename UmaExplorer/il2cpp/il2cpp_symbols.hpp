#pragma once
#include <cstdint>
#include <cstddef>
#include <filesystem>
#include <locale>
#include <string>
#include <thread>
#include <iostream>
#include <sstream>
#include <windows.h>
#include <MinHook.h>
#include <nlohmann/json.hpp>

// UnityEngine.Color
struct Color_t
{
public:
	// System.Single UnityEngine.Color::r
	float r;
	// System.Single UnityEngine.Color::g
	float g;
	// System.Single UnityEngine.Color::b
	float b;
	// System.Single UnityEngine.Color::a
	float a;
};

// UnityEngine.Vector2
struct Vector2_t
{
public:
	// System.Single UnityEngine.Vector2::x
	float x;
	// System.Single UnityEngine.Vector2::y
	float y;
};

// UnityEngine.Vector3
struct Vector3_t
{
public:
	// System.Single UnityEngine.Vector3::x
	float x;
	// System.Single UnityEngine.Vector3::y
	float y;
	// System.Single UnityEngine.Vector3::z
	float z;
};

struct Resolution_t
{
public:
	int width;
	int height;
	int herz;
};

// UnityEngine.TextGenerationSettings
struct TextGenerationSettings_t
{
public:
	// UnityEngine.Font UnityEngine.TextGenerationSettings::font
	void* font;
	// UnityEngine.Color UnityEngine.TextGenerationSettings::color
	Color_t color;
	// System.Int32 UnityEngine.TextGenerationSettings::fontSize
	int32_t fontSize;
	// System.Single UnityEngine.TextGenerationSettings::lineSpacing
	float lineSpacing;
	// System.Boolean UnityEngine.TextGenerationSettings::richText
	bool richText;
	// System.Single UnityEngine.TextGenerationSettings::scaleFactor
	float scaleFactor;
	// UnityEngine.FontStyle UnityEngine.TextGenerationSettings::fontStyle
	int32_t fontStyle;
	// UnityEngine.TextAnchor UnityEngine.TextGenerationSettings::textAnchor
	int32_t textAnchor;
	// System.Boolean UnityEngine.TextGenerationSettings::alignByGeometry
	bool alignByGeometry;
	// System.Boolean UnityEngine.TextGenerationSettings::resizeTextForBestFit
	bool resizeTextForBestFit;
	// System.Int32 UnityEngine.TextGenerationSettings::resizeTextMinSize
	int32_t resizeTextMinSize;
	// System.Int32 UnityEngine.TextGenerationSettings::resizeTextMaxSize
	int32_t resizeTextMaxSize;
	// System.Boolean UnityEngine.TextGenerationSettings::updateBounds
	bool updateBounds;
	// UnityEngine.VerticalWrapMode UnityEngine.TextGenerationSettings::verticalOverflow
	int32_t verticalOverflow;
	// UnityEngine.HorizontalWrapMode UnityEngine.TextGenerationSettings::horizontalOverflow
	int32_t horizontalOverflow;
	// UnityEngine.Vector2 UnityEngine.TextGenerationSettings::generationExtents
	Vector2_t  generationExtents;
	// UnityEngine.Vector2 UnityEngine.TextGenerationSettings::pivot
	Vector2_t  pivot;
	// System.Boolean UnityEngine.TextGenerationSettings::generateOutOfBounds
	bool generateOutOfBounds;
};

enum Il2CppTypeEnum
{
	IL2CPP_TYPE_END = 0x00,       /* End of List */
	IL2CPP_TYPE_VOID = 0x01,
	IL2CPP_TYPE_BOOLEAN = 0x02,
	IL2CPP_TYPE_CHAR = 0x03,
	IL2CPP_TYPE_I1 = 0x04,
	IL2CPP_TYPE_U1 = 0x05,
	IL2CPP_TYPE_I2 = 0x06,
	IL2CPP_TYPE_U2 = 0x07,
	IL2CPP_TYPE_I4 = 0x08,
	IL2CPP_TYPE_U4 = 0x09,
	IL2CPP_TYPE_I8 = 0x0a,
	IL2CPP_TYPE_U8 = 0x0b,
	IL2CPP_TYPE_R4 = 0x0c,
	IL2CPP_TYPE_R8 = 0x0d,
	IL2CPP_TYPE_STRING = 0x0e,
	IL2CPP_TYPE_PTR = 0x0f,
	IL2CPP_TYPE_BYREF = 0x10,
	IL2CPP_TYPE_VALUETYPE = 0x11,
	IL2CPP_TYPE_CLASS = 0x12,
	IL2CPP_TYPE_VAR = 0x13,
	IL2CPP_TYPE_ARRAY = 0x14,
	IL2CPP_TYPE_GENERICINST = 0x15,
	IL2CPP_TYPE_TYPEDBYREF = 0x16,
	IL2CPP_TYPE_I = 0x18,
	IL2CPP_TYPE_U = 0x19,
	IL2CPP_TYPE_FNPTR = 0x1b,
	IL2CPP_TYPE_OBJECT = 0x1c,
	IL2CPP_TYPE_SZARRAY = 0x1d,
	IL2CPP_TYPE_MVAR = 0x1e,
	IL2CPP_TYPE_CMOD_REQD = 0x1f,
	IL2CPP_TYPE_CMOD_OPT = 0x20,
	IL2CPP_TYPE_INTERNAL = 0x21,

	IL2CPP_TYPE_MODIFIER = 0x40,
	IL2CPP_TYPE_SENTINEL = 0x41,
	IL2CPP_TYPE_PINNED = 0x45,

	IL2CPP_TYPE_ENUM = 0x55
};

typedef struct Il2CppType
{
	void* dummy;
	unsigned int attrs : 16;
	Il2CppTypeEnum type : 8;
	unsigned int num_mods : 6;
	unsigned int byref : 1;
	unsigned int pinned : 1;
} Il2CppType;

struct ParameterInfo
{
	const char* name;
	int32_t position;
	uint32_t token;
	const Il2CppType* parameter_type;
};

struct MethodInfo
{
	uintptr_t methodPointer;
	uintptr_t invoker_method;
	const char* name;
	uintptr_t klass;
	uintptr_t return_type;
	const ParameterInfo* parameters;
	uintptr_t methodDefinition;
	uintptr_t genericContainer;
	uint32_t token;
	uint16_t flags;
	uint16_t iflags;
	uint16_t slot;
	uint8_t parameters_count;
	uint8_t is_generic : 1;
	uint8_t is_inflated : 1;
	uint8_t wrapper_type : 1;
	uint8_t is_marshaled_from_native : 1;
};

struct Il2CppObject
{
	union
	{
		void* klass;
		void* vtable;
	};
	void* monitor;
};

struct Il2CppString {
	Il2CppObject obj;
	int32_t length;
	wchar_t chars[1];
};

typedef struct Il2CppArraySize
{
	Il2CppObject obj;
	void* bounds;
	uintptr_t max_length;
	alignas(8)
		void* vector[1];
} Il2CppArraySize;

static const size_t kIl2CppSizeOfArray = (offsetof(Il2CppArraySize, vector));

// function types
typedef Il2CppString* (*il2cpp_string_new_utf16_t)(const wchar_t* str, unsigned int len);
typedef Il2CppString* (*il2cpp_string_new_t)(const char* str);
typedef void* (*il2cpp_domain_get_t)();
typedef void* (*il2cpp_domain_assembly_open_t)(void* domain, const char* name);
typedef void* (*il2cpp_assembly_get_image_t)(void* assembly);
typedef void* (*il2cpp_class_from_name_t)(void* image, const char* namespaze, const char* name);
typedef void* (*il2cpp_class_from_system_type_t)(void* type);
typedef void* (*il2cpp_object_get_class_t)(void* obj);

typedef void* (*il2cpp_class_get_fields_t)(void* klass, void** iter);
typedef const char* (*il2cpp_field_get_name_t)(void* field);
typedef void* (*il2cpp_field_get_type_t)(void* field);
typedef char* (*il2cpp_type_get_name_t)(void* type);
typedef void* (*il2cpp_class_get_parent_t)(void* klass);
typedef void* (*il2cpp_class_get_type_t)(void* klass);
typedef void (*il2cpp_field_get_value_t)(void* obj, void* field, void* value);
typedef void* (*il2cpp_field_get_value_object_t)(void* field, void* obj);
typedef void* (*il2cpp_type_get_object_t)(void* type);

typedef void* (*il2cpp_class_get_properties_t)(void* klass, void** iter);
typedef const char* (*il2cpp_property_get_name_t)(void* prop);
typedef MethodInfo* (*il2cpp_property_get_get_method_t)(void* prop);
typedef MethodInfo* (*il2cpp_property_get_set_method_t)(void* prop);
typedef void* (*il2cpp_method_get_return_type_t)(void* method);
typedef void* (*il2cpp_method_get_declaring_type_t)(void* method);

typedef bool (*il2cpp_class_is_enum_t)(void* klass);
typedef void* (*il2cpp_type_get_class_or_element_class_t)(void* klass);
typedef void* (*il2cpp_class_from_type_t)(void* type);

typedef MethodInfo* (*il2cpp_class_get_methods_t)(void* klass, void** iter);
typedef MethodInfo* (*il2cpp_class_get_method_from_name_t)(void* klass, const char* name, int argsCount);
typedef void* (*il2cpp_method_get_param_t)(const MethodInfo* method, uint32_t index);
typedef void* (*il2cpp_object_new_t)(void* klass);
typedef void* (*il2cpp_resolve_icall_t)(const char* name);
typedef void* (*il2cpp_array_new_t)(void* klass, uintptr_t count);
typedef void* (*il2cpp_thread_attach_t)(void* domain);
typedef void (*il2cpp_thread_detach_t)(void* thread);
typedef size_t (*il2cpp_image_get_class_count_t)(void* image);
typedef void* (*il2cpp_image_get_class_t)(void* image, size_t index);
typedef char* (*il2cpp_class_get_name_t)(void* klass);
typedef void* (*il2cpp_class_get_field_from_name_t)(void* klass, const char* name);
typedef size_t (*il2cpp_field_get_offset_t)(void* field);
typedef uint32_t (*il2cpp_array_length_t)(void* _array);

typedef char* (*il2cpp_method_get_name_t)(const MethodInfo* method);
typedef bool (*il2cpp_class_is_valuetype_t)(void* klass);
typedef void* (*il2cpp_class_get_element_class_t)(void* klass);

typedef int (*il2cpp_method_get_param_count_t)(const MethodInfo* method);
typedef void* (*il2cpp_runtime_invoke_t)(void* method, void* obj, void** params, void** exc);

// function defines
extern il2cpp_string_new_utf16_t il2cpp_string_new_utf16;
extern il2cpp_string_new_t il2cpp_string_new;
extern il2cpp_domain_get_t il2cpp_domain_get;
extern il2cpp_domain_assembly_open_t il2cpp_domain_assembly_open;
extern il2cpp_assembly_get_image_t il2cpp_assembly_get_image;
extern il2cpp_class_from_name_t il2cpp_class_from_name;
extern il2cpp_class_from_system_type_t il2cpp_class_from_system_type;
extern il2cpp_object_get_class_t il2cpp_object_get_class;
//Field
extern il2cpp_class_get_fields_t il2cpp_class_get_fields;
extern il2cpp_field_get_name_t il2cpp_field_get_name;
extern il2cpp_field_get_type_t il2cpp_field_get_type;
extern il2cpp_type_get_name_t il2cpp_type_get_name;
extern il2cpp_class_get_parent_t il2cpp_class_get_parent;
extern il2cpp_class_get_type_t il2cpp_class_get_type;
extern il2cpp_field_get_value_t il2cpp_field_get_value;
extern il2cpp_field_get_value_object_t il2cpp_field_get_value_object;
extern il2cpp_type_get_object_t il2cpp_type_get_object;
//

extern il2cpp_class_get_properties_t il2cpp_class_get_properties;
extern il2cpp_property_get_name_t il2cpp_property_get_name;
extern il2cpp_property_get_get_method_t il2cpp_property_get_get_method;
extern il2cpp_property_get_set_method_t il2cpp_property_get_set_method;
extern il2cpp_method_get_return_type_t il2cpp_method_get_return_type;
extern il2cpp_method_get_declaring_type_t il2cpp_method_get_declaring_type;

extern il2cpp_class_is_enum_t il2cpp_class_is_enum;
extern il2cpp_type_get_class_or_element_class_t il2cpp_type_get_class_or_element_class;
extern il2cpp_class_from_type_t il2cpp_class_from_type;

extern il2cpp_class_get_methods_t il2cpp_class_get_methods;
extern il2cpp_class_get_method_from_name_t il2cpp_class_get_method_from_name;
extern il2cpp_method_get_param_t il2cpp_method_get_param;
extern il2cpp_object_new_t il2cpp_object_new;
extern il2cpp_resolve_icall_t il2cpp_resolve_icall;
extern il2cpp_array_new_t il2cpp_array_new;
extern il2cpp_thread_attach_t il2cpp_thread_attach;
extern il2cpp_thread_detach_t il2cpp_thread_detach;
extern il2cpp_image_get_class_count_t il2cpp_image_get_class_count;
extern il2cpp_image_get_class_t il2cpp_image_get_class;
extern il2cpp_class_get_name_t il2cpp_class_get_name;
extern il2cpp_class_get_field_from_name_t il2cpp_class_get_field_from_name;
extern il2cpp_field_get_offset_t il2cpp_field_get_offset;
extern il2cpp_array_length_t il2cpp_array_length;
extern il2cpp_method_get_name_t il2cpp_method_get_name;
extern il2cpp_class_is_valuetype_t il2cpp_class_is_valuetype;
extern il2cpp_class_get_element_class_t il2cpp_class_get_element_class;
extern il2cpp_method_get_param_count_t il2cpp_method_get_param_count;
extern il2cpp_runtime_invoke_t il2cpp_runtime_invoke;

char* il2cpp_array_addr_with_size(void* arr, int32_t size, uintptr_t idx);

// array macro
#define il2cpp_array_addr(array, type, index) ((type*)(void*) il2cpp_array_addr_with_size (array, sizeof (type), index))

#define il2cpp_array_setref(array, index, value)  \
    do {    \
        void* *__p = (void* *) il2cpp_array_addr ((array), void*, (index)); \
         *__p = (value);    \
    } while (0)



namespace il2cpp_symbols
{
	void init(HMODULE game_module);
	uintptr_t get_method_pointer(const char* assemblyName, const char* namespaze,
		const char* klassName, const char* name, int argsCount, int index = -1);

	void* get_class(const char* assemblyName, const char* namespaze, const char* klassName);

	MethodInfo* get_method(const char* assemblyName, const char* namespaze,
		const char* klassName, const char* name, int argsCount);

	uintptr_t find_method(const char* assemblyName, const char* namespaze,
		const char* klassName, std::function<bool(const MethodInfo*)> predict);

	uintptr_t get_method_pointer_base(const char* assemblyName, const char* namespaze,
		const char* klassName, const char* name, int argsCount, size_t offset);

	size_t get_field_offset(const char* assemblyName, const char* namespaze,
		const char* klassName, const char* name);

	void* get_il2cpp_string(const char* str);

	uint32_t get_array_length(void* array);

	void* new_object(void* klass);

	void* new_array(void* elementTypeInfo, size_t length);

	uintptr_t get_method_pointer_re(const char* assemblyName, const char* namespaze,
		const char* klassName, const char* name, int argsCount, int index);

	void* get_type_class(void* type);

	uintptr_t class_get_method(void* klass, const char* name, int argsCount);

	void* object_get_class(void* obj);

	std::wstring il2cpp_get_wstring(Il2CppString* str);

	std::string il2cpp_get_string(void* str);

	void class_print_all_method(void* klass);
}
