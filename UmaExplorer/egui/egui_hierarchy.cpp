#include "egui_hierarchy.h"
#include "egui_inspector_main.h"

static flat_hash_map<void*, ObjectTree> ObjDic = {};
bool show_hierarchy_window = false;
void* rootIndex = 0;
bool need_refresh = false;

void GetCompnents(ObjectTree* object) 
{
	void* Component_type = type_hook(il2cpp_symbols::get_il2cpp_string("UnityEngine.Component, UnityEngine"));

	void* component = components_hook(object->gameObj, Component_type);
	int ComLength = il2cpp_symbols::get_array_length(component);

	vector<ObjectInfo> components;

	for (int j = 0; j < ComLength; j++) {
		components.push_back({ "", "", arrayindex_hook(component, j)});
	}

	object->components = components;
}

void RefreshHierarchy()
{
	ObjDic = {};

	ObjectTree root;

	void* Transform_type = type_hook(il2cpp_symbols::get_il2cpp_string("UnityEngine.Transform, UnityEngine"));
	void* Object_list = object_hook(Transform_type, 1);
	int ObjLength = il2cpp_symbols::get_array_length(Object_list);


	vector<int> AliveCheckList;
	for (int i = 0; i < ObjLength; i++) {
		void* Obj = arrayindex_hook(Object_list, i);
		int Obj_Inst = obj_inst_hook(Obj);
		AliveCheckList.push_back(Obj_Inst);
	}

	for (int i = 0; i < ObjLength; i++) {
		void* Obj = arrayindex_hook(Object_list, i);
		if (inst_obj_hook(AliveCheckList[i]) != Obj) {
			continue; 
		}
		void* gameObj = gameobject_hook(Obj);

		vector<void*> children;
		int childCount = childcount_hook(Obj);
		for (int j = 0; j < childCount; j++) {
			children.push_back(child_hook(Obj, j));
		}


		ObjectTree node;
		node.name = il2cpp_symbols::il2cpp_get_string(objectname_hook(gameObj));
		node.gameObj = gameObj;
		node.transform = Obj;
		node.instanceID = obj_inst_hook(gameObj);
		node.children = children;
		node.activeSelf = activeSelf_hook(gameObj);
		node.activeInHierarchy = hierarchyActive_hook(gameObj);


		ObjDic[Obj] = node;

		//获取根Obj
		void* ObjParent = parent_hook(Obj);

		if (!ObjParent) {
			root.children.push_back(Obj);
		}
	}

	ObjDic[rootIndex] = root; // 添加根节点

}

void HierarchyRecursion(void* currentObj, ImGuiTreeNodeFlags base_flags) {

	ImGuiTreeNodeFlags node_flags = base_flags;
	ObjectTree* currentNode = &ObjDic[currentObj];

	if (currentObj) 
	{
		bool activeSelf = currentNode->activeSelf;
		bool activeInHierarchy = currentNode->activeInHierarchy;
		void* gameObj = currentNode->gameObj;

		ImGui::BeginDisabled();
		ImGui::PushID(gameObj);
		ImGui::Checkbox("##", &activeSelf);
		ImGui::PopID();
		ImGui::SameLine();
		ImGui::EndDisabled();

		if (currentNode->children.size() != 0) {
			if (!activeInHierarchy) ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
			bool node_open = ImGui::TreeNodeEx(currentObj, node_flags, currentNode->name.c_str());
			if (!activeInHierarchy) ImGui::PopStyleColor();
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
				if (inst_obj_hook(currentNode->instanceID) == gameObj) 
				{
					GetCompnents(currentNode);
					AddInspectorMainList(currentNode);
				}
				else 
				{
					need_refresh = true;
				}
			}
			if (node_open)
			{
				for (int index = 0; index < currentNode->children.size(); index++) {
					HierarchyRecursion(currentNode->children[index], base_flags);
				}
				ImGui::TreePop();
			}
		}
		else {
			node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
			if (!activeInHierarchy) ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
			ImGui::TreeNodeEx(currentObj, node_flags, currentNode->name.c_str());
			if (!activeInHierarchy) ImGui::PopStyleColor();
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
				if (inst_obj_hook(currentNode->instanceID) == gameObj)
				{
					GetCompnents(currentNode);
					AddInspectorMainList(currentNode);
				}
				else
				{
					need_refresh = true;
				}
			}
		}
	}
	else 
	{
		//如果currentObj为空，则表示当前是根节点
		if (currentNode && currentNode->children.size() != 0)
		{
			for (int index = 0; index < currentNode->children.size(); index++) {
				HierarchyRecursion(currentNode->children[index], base_flags);
			}

			//在递归末尾刷新
			if (need_refresh) 
			{
				RefreshHierarchy();
				need_refresh = false;
			}
		}
	}
};

void HierarchyWindow() 
{
	if (show_hierarchy_window)
	{
		ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
		ImGui::Begin("Hierarchy Window", &show_hierarchy_window);

		//建立树状结构
		static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

		HierarchyRecursion(rootIndex, base_flags);

		ImGui::End();
	}
}