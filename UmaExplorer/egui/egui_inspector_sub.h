#pragma once
#include <string>
#include <vector>
#include "../imgui/imgui.h"
#include "parallel_hashmap/phmap.h"

#include "il2cpp_utils.h"
#include "hook_utils.h"
#include "egui_utils.h"
#include "common_utils.h"

#include "egui_hierarchy.h"

struct InspectorSubInfo
{
	ObjectInfo obj;
	bool state;
	bool update = false;
};

void AddInspectorSubList(ObjectInfo obj);
void CreateInspectorSubWindows();