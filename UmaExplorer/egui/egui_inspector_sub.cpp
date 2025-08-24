#include "egui_inspector_sub.h"
#include "../hook.hpp"

static vector<InspectorSubInfo> InspectorSubList;
int InspectorSubListSize;

//添加指定元素到列表
void AddInspectorSubList(ObjectInfo obj)
{

	for (auto& item : InspectorSubList)
	{
		if (item.obj.obj == obj.obj)
		{
			// 已经存在，直接返回
			item.update = true;
			return;
		}
	}

	InspectorSubList.push_back({ obj, true});
}

//清理列表指定Index
void ClearInspectorSubList(int index)
{
	auto it = InspectorSubList.begin();
	InspectorSubList.erase(it + index);
	InspectorSubListSize -= 1;
}

void CreateInspectorSubWindows()
{
	InspectorSubListSize = InspectorSubList.size();
	for (int i = 0; i < InspectorSubListSize; )
	{
		InspectorSubInfo* InspectorSubWindow = &InspectorSubList[i];
		if (InspectorSubWindow->state)
		{
			if (inst_obj_hook(InspectorSubWindow->obj.alivecheck.instanceID) == InspectorSubWindow->obj.alivecheck.gameObj && *(uintptr_t*)InspectorSubWindow->obj.obj != 0)
			{
				ImGui::SetNextWindowSize(ImVec2(1200, 600), ImGuiCond_FirstUseEver);
				string windowName = "Inspector Sub Window : " + InspectorSubWindow->obj.name + "##" + std::to_string((uintptr_t)InspectorSubWindow->obj.obj);
				ImGui::Begin(windowName.c_str(), &InspectorSubWindow->state, ImGuiWindowFlags_NoSavedSettings);
				if (InspectorSubWindow->update)
				{
					ImGui::SetWindowFocus(windowName.c_str());
					InspectorSubWindow->update = false;
				}

				ImGui::Text(("Object Name: " + InspectorSubWindow->obj.name).c_str());
				ImGui::Text(("Object Type: " + InspectorSubWindow->obj.typeName).c_str());
				ImGui::Text("Object Address: 0X%p", InspectorSubWindow->obj.obj);

				void* _class = il2cpp_symbols::object_get_class(InspectorSubWindow->obj.obj);

				if (InspectorSubWindow->obj.typeName == "UnityEngine.Material")
				{
					if (InspectorSubWindow->obj.materialInfo.state == false)
					{
						//printf("Update\n");
						void* ret = localkey_hook(InspectorSubWindow->obj.obj);
						//printf("Get Local Success: %p\n", ret);
						for(int j = 0; j < il2cpp_symbols::get_array_length(ret); j++)
						{

							LocalKeyword* newdata = (LocalKeyword*)arrayindex_hook(ret, 0);
							InspectorSubWindow->obj.materialInfo.enabledLocalKeywordList.push_back({ il2cpp_symbols::il2cpp_get_string(newdata->m_Name), newdata->m_Index });

						}

						DynamicArrayData keyword_arraydata;
						key_hook(&keyword_arraydata);
						InspectorSubWindow->obj.materialInfo.enabledGlobalKeywordList = ParseKeywords(keyword_arraydata);

						void* shader = material_get_shader_hook(InspectorSubWindow->obj.obj);
						//printf("Shader is %p\n", shader);
						if (shader)
						{
							string shader_name = il2cpp_symbols::il2cpp_get_string(objectname_hook(shader));

							InspectorSubWindow->obj.materialInfo.shader.name = shader_name;
							//printf("Shader Name is %s\n", shader_name.c_str());

							int property_count = Shader_GetPropertyCount_hook(shader);

							//printf("Property count is %d\n", property_count);

							for (int j = 0; j < property_count; j++)
							{
								string property_name = il2cpp_symbols::il2cpp_get_string(Shader_GetPropertyName_hook(shader, j));
								//printf("Name is %s\n", property_name.c_str());
								ShaderPropertyType property_type = (ShaderPropertyType)Shader_GetPropertyType_hook(shader, j);
								//printf("Type is %d\n", property_type);
								int property_id = Shader_GetPropertyNameId_hook(shader, j);
								InspectorSubWindow->obj.materialInfo.shader.shaderProperties.push_back({ property_name, property_id, property_type });
							}

							InspectorSubWindow->obj.materialInfo.shader.state = true;
						}

						InspectorSubWindow->obj.materialInfo.state = true;
					}
					ImGui::SeparatorText("Global Keywords");
					for (int j = 0; j < InspectorSubWindow->obj.materialInfo.enabledGlobalKeywordList.size(); j++)
					{
						ImGui::Text(InspectorSubWindow->obj.materialInfo.enabledGlobalKeywordList[j].name.c_str());
					}
					ImGui::SeparatorText("Local Keywords");
					for (int j = 0; j < InspectorSubWindow->obj.materialInfo.enabledLocalKeywordList.size(); j++)
					{
						ImGui::Text(InspectorSubWindow->obj.materialInfo.enabledLocalKeywordList[j].name.c_str());
					}

					if (InspectorSubWindow->obj.important_obj && InspectorSubWindow->obj.materialInfo.shader.state)
					{
						
						//printf("Get PropertyBlock Success!\n");
						ImGui::SeparatorText("Material Property Block Info");
						CreateMaterialPropertyBlockTable(InspectorSubWindow->obj, InspectorSubWindow->obj.alivecheck);
						/*
						for (int j = 0; j < InspectorSubWindow->obj.materialInfo.shader.shaderProperties.size(); j++)
						{
							int id = InspectorSubWindow->obj.materialInfo.shader.shaderProperties[j].id;
							if (MaterialPropertyBlock_HasPropertyImpl_hook(mpb, id))
							{
								printf("True Property:%s\n", InspectorSubWindow->obj.materialInfo.shader.shaderProperties[j].name.c_str());
							}
						}
						*/
					}

					if (InspectorSubWindow->obj.materialInfo.shader.state)
					{
						ImGui::SeparatorText("Shader Info");
						ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), ("Shader Name: " + InspectorSubWindow->obj.materialInfo.shader.name).c_str());
						CreateShaderPropertyTable(InspectorSubWindow->obj, InspectorSubWindow->obj.alivecheck);
					}
				}


				if (InspectorSubWindow->obj.typeName.rfind("[]") == InspectorSubWindow->obj.typeName.size() - 2)
				{
					ImGui::SeparatorText("Array Info");

					CreateArrayTable(InspectorSubWindow->obj, removeTrailingBrackets(InspectorSubWindow->obj.typeName), _class, InspectorSubWindow->obj.alivecheck);
				}

				ImGui::SeparatorText("Object Info");

				
				CreateFieldTable(InspectorSubWindow->obj, _class, InspectorSubWindow->obj.alivecheck);
				CreatePropertyTable(InspectorSubWindow->obj, _class, InspectorSubWindow->obj.alivecheck);

				ImGui::End();
				i++;
			}
			else
			{
				InspectorSubWindow->state = false;
				ClearInspectorSubList(i);
			}
		}
		else
		{
			ClearInspectorSubList(i);
		}
	}
}