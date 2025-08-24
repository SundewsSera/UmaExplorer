#pragma once
#include <string>
#include <vector>
#include "../imgui/imgui.h"
#include "parallel_hashmap/phmap.h"

#include "il2cpp_utils.h"
#include "hook_utils.h"
#include "egui_utils.h"

using phmap::flat_hash_map;
using namespace std;

struct ObjectTree
{
	string name;
	void* gameObj;
	void* transform;
	int instanceID;
	vector<ObjectInfo> components;
	vector<void*> children;
	bool activeSelf;
	bool activeInHierarchy;
};

void RefreshHierarchy();
void HierarchyWindow();

extern bool show_hierarchy_window;