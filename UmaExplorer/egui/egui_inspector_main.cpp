#include "egui_inspector_main.h"

static vector<InspectorMainInfo> InspectorMainList;
int InspectorMainListSize;

//添加指定元素到列表
void AddInspectorMainList(ObjectTree* obj) {

	for (auto& item : InspectorMainList) {
		if (item.obj == obj) {
			// 已经存在，直接返回
			item.update = true;
			return;
		}
	}

	InspectorMainList.push_back({ obj, true });
}

//清理列表指定Index
void ClearInspectorMainList(int index) {
	auto it = InspectorMainList.begin();
	InspectorMainList.erase(it + index);
	InspectorMainListSize -= 1;
}

//显示组件

void ShowComponents(ObjectTree* currentNode, AliveCheckInfo alivecheck) {
	auto& components = currentNode->components;
	for (int i = 0; i < components.size(); i++) {
		string component_name = GetTypeName(il2cpp_symbols::il2cpp_get_string(objectname_hook(components[i].obj)));
		if (ImGui::TreeNode(components[i].obj, component_name.c_str())) {

			void* _class = il2cpp_symbols::object_get_class(components[i].obj);

			CreateFieldTable(components[i], _class, alivecheck, i);
			CreatePropertyTable(components[i], _class, alivecheck, i);

			ImGui::TreePop();
		};
	}
}


void CreateInspectorMainWindows() 
{
	InspectorMainListSize = InspectorMainList.size();
	for (int i = 0; i < InspectorMainListSize; ) 
	{
		InspectorMainInfo* InspectorMainWindow = &InspectorMainList[i];
		if (InspectorMainWindow->state)
		{
			if (inst_obj_hook(InspectorMainWindow->obj->instanceID) == InspectorMainWindow->obj->gameObj)
			{
				ImGui::SetNextWindowSize(ImVec2(1200, 600), ImGuiCond_FirstUseEver);

				string windowName = "Inspector Main Window : " + InspectorMainWindow->obj->name + "##" + std::to_string((uintptr_t)InspectorMainWindow->obj);

				ImGui::Begin(windowName.c_str(), &InspectorMainWindow->state, ImGuiWindowFlags_NoSavedSettings);

				if (InspectorMainWindow->update)
				{
					ImGui::SetWindowFocus(windowName.c_str());
					InspectorMainWindow->update = false;
				}

				ImGui::Text(("GameObject Name: " + InspectorMainWindow->obj->name).c_str());

				ImGui::SeparatorText("Transform");
				Vector3 V_pos = position_hook(InspectorMainWindow->obj->transform);
				Vector3 V_rot = rotation_hook(InspectorMainWindow->obj->transform);
				Vector3 V_scale = scale_hook(InspectorMainWindow->obj->transform);

				float position[] = { V_pos.x, V_pos.y, V_pos.z };
				float rotation[] = { V_rot.x, V_rot.y, V_rot.z };
				float scale[] = { V_scale.x, V_scale.y, V_scale.z };

				ImGui::InputFloat3(("Position##" + to_string((uintptr_t)InspectorMainWindow->obj)).c_str(), position, "%.5f", ImGuiInputTextFlags_ReadOnly);
				ImGui::InputFloat3(("Rotation##" + to_string((uintptr_t)InspectorMainWindow->obj)).c_str(), rotation, "%.5f", ImGuiInputTextFlags_ReadOnly);
				ImGui::InputFloat3(("Scale##" + to_string((uintptr_t)InspectorMainWindow->obj)).c_str(), scale, "%.5f", ImGuiInputTextFlags_ReadOnly);


				ImGui::SeparatorText("Components");
				ShowComponents(InspectorMainWindow->obj, { InspectorMainWindow->obj->gameObj , InspectorMainWindow->obj->instanceID });
				ImGui::End();
				i++;
			}
			else 
			{
				//printf("Prepare to Destory!\n");
				InspectorMainWindow->state = false;
				ClearInspectorMainList(i);
				//printf("Destory OK!\n");
			}
		}
		else 
		{
			ClearInspectorMainList(i);
		}
	}
}