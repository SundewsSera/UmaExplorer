#pragma once
#include <string>
#include <vector>

enum ShaderPropertyType
{
	ShaderPropertyType_Color = 0,
	ShaderPropertyType_Vector = 1,
	ShaderPropertyType_Float = 2,
	ShaderPropertyType_Range = 3,
	ShaderPropertyType_Texture = 4,
	ShaderPropertyType_Int = 5
};

struct Color
{
	float r; // 0x0
	float g; // 0x4
	float b; // 0x8
	float a; // 0xC
};

struct Vector4 // TypeDefIndex: 2063
{
	float x; // 0x0
	float y; // 0x4
	float z; // 0x8
	float w;
};

struct Vector3 // TypeDefIndex: 2063
{
	float x; // 0x0
	float y; // 0x4
	float z; // 0x8
};

struct Vector2 // TypeDefIndex: 2063
{
	float x; // 0x0
	float y; // 0x4
};

struct DynamicArrayData
{
	void* data = nullptr;
	int allocator = 0;
	size_t m_size = 0;
	size_t m_capacity = 0;
};

struct KeywordEntry
{
	std::string name;
	int id;
};

struct LocalKeyword
{
	void* unknown1;
	void* unknown2;
	void* m_SpaceInfo;
	void* m_Name;
	int m_Index;
};

struct ShaderProperty
{
	std::string name;
	int id;
	ShaderPropertyType type;
};

struct Shader
{
	bool state;
	std::string name;
	std::vector<ShaderProperty> shaderProperties;
};

std::vector<KeywordEntry> ParseKeywords(const DynamicArrayData& arr);