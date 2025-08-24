#pragma once
#include <string>
#include <vector>
#include "../imgui/imgui.h"
#include "parallel_hashmap/phmap.h"

#include "il2cpp_utils.h"
#include "hook_utils.h"
#include "common_utils.h"

using phmap::flat_hash_map;
using namespace std;

struct AliveCheckInfo
{
	void* gameObj = nullptr;
	int instanceID = 0;
};

struct PropertyInfo
{
	bool state = false;
	void* type = nullptr;
	vector<void*> properties;
	int proLength = 0;
	vector<string> propertyNameList;
};

struct MaterialInfo
{
	bool state = false;
	vector<KeywordEntry> enabledLocalKeywordList;
	vector<KeywordEntry> enabledGlobalKeywordList;
	Shader shader;
};

struct ObjectInfo
{
	string name;
	string typeName;
	void* obj;
	AliveCheckInfo alivecheck;
	PropertyInfo propertyInfo;
	MaterialInfo materialInfo;
	void* important_obj = nullptr;
	int important_index = 0;
};

std::string removeTrailingBrackets(const std::string& s);

string GetTypeName(string name);
void CreateFieldTable(ObjectInfo& obj, void* _class, AliveCheckInfo alivecheck, int i = 0);
void CreatePropertyTable(ObjectInfo& obj, void* _class, AliveCheckInfo alivecheck, int i = 0);
void CreateArrayTable(ObjectInfo& obj, string type, void* _class, AliveCheckInfo alivecheck, int i = 0);
void CreateShaderPropertyTable(ObjectInfo& obj, AliveCheckInfo alivecheck, int i = 0);
void CreateMaterialPropertyBlockTable(ObjectInfo& obj, AliveCheckInfo alivecheck, int i = 0);

