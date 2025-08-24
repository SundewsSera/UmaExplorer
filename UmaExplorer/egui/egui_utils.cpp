#include "egui_utils.h"
#include "egui_inspector_sub.h"
#include "../hook.hpp"

void* namePro = nullptr;

void GetField(ObjectInfo& base, void* obj, void* _class, AliveCheckInfo alivecheck, string path, int color);

std::string removeTrailingBrackets(const std::string& s)
{
	if (s.size() >= 2 && s.compare(s.size() - 2, 2, "[]") == 0)
	{
		return s.substr(0, s.size() - 2);
	}
	return s; // 如果没有以 [] 结尾，就原样返回
}

// 定义循环颜色数组
ImVec4 levelColors[] = {
	ImVec4(0,0,0,0),               // 0 → 默认颜色（透明，不覆盖原 RowBg）
	ImVec4(0.5f, 0.8f, 0.5f, 0.3f), // 1 → 淡绿色
	ImVec4(0.8f, 0.5f, 0.5f, 0.3f), // 2 → 淡红色
	ImVec4(0.5f, 0.5f, 0.8f, 0.3f), // 3 → 淡蓝色
};

//从名称中获取类型名
string GetTypeName(string name) {
	int left = name.rfind('(') + 1;
	int right = name.rfind(')');

	return name.substr(left, right - left);
}

//找到泛型对应的字符串
string GetEnumName(void* _class, int id) {
	void* iter = nullptr;
	void* tempObj = il2cpp_object_new(_class);
	bool first = true;
	while (void* field = il2cpp_class_get_fields(_class, &iter)) {
		if (first) {
			first = false;
			continue;
		}
		string fieldName = il2cpp_field_get_name(field);
		void* fieldId = il2cpp_field_get_value_object(field, tempObj);
		if (fieldId) {
			int trueId = *((int*)fieldId + 4);
			if (id == trueId) {
				return fieldName;
			}
		}
	}
	return "";
}

void TextColoredWrapped(const ImVec4& col, const char* fmt, ...)
{
	ImGui::PushStyleColor(ImGuiCol_Text, col);
	ImGui::PushTextWrapPos(0.0f);

	va_list args;
	va_start(args, fmt);
	ImGui::TextV(fmt, args);
	va_end(args);

	ImGui::PopTextWrapPos();
	ImGui::PopStyleColor();
}

void GetArray(ObjectInfo& base, void* obj, string type, void* _class, AliveCheckInfo alivecheck, string path = "", int color = 0)
{
	int array_len = il2cpp_array_length(obj);

	int max_len = array_len < 20 ? array_len : 20;

	for (int i = 0; i < max_len; i++)
	{
		void* value = arrayindex_hook(obj, i);
		if (value)
		{
			ImGui::TableNextRow();

			// 取模选择颜色
			int colorIndex = color % (sizeof(levelColors) / sizeof(levelColors[0]));
			ImVec4 col = levelColors[colorIndex];

			if (colorIndex != 0) // 0 表示默认，不设置
				ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, ImGui::ColorConvertFloat4ToU32(col));
			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%d", i);
			ImGui::TableSetColumnIndex(2);
			ImGui::TextWrapped(type.c_str());
			ImGui::TableSetColumnIndex(3);
			if (il2cpp_class_is_enum(_class))
			{
				int trueEnumValue = *((int*)value + 4);
				string trueEnumName = GetEnumName(_class, trueEnumValue);
				ImGui::TextWrapped("%d (%s)", trueEnumValue, trueEnumName.c_str());
			}
			else if (type == "System.String")
			{
				string trueText = il2cpp_symbols::il2cpp_get_string(value);
				TextColoredWrapped(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), trueText.c_str());
			}
			else if (type == "System.Int32")
			{
				int trueInt32 = *((int*)value + 4);
				ImGui::TextWrapped(to_string(trueInt32).c_str());
			}
			else if (type == "System.UInt32")
			{
				unsigned int trueUInt32 = *((unsigned int*)value + 4);
				ImGui::TextWrapped(to_string(trueUInt32).c_str());
			}
			else if (type == "System.IntPtr")
			{
				size_t trueIntPtr = *(size_t*)((int*)value + 4);
				ImGui::TextWrapped("0X%p", trueIntPtr);
			}
			else if (type == "System.Single")
			{
				float trueSingle = *(float*)((int*)value + 4);
				ImGui::TextWrapped(to_string(trueSingle).c_str());
			}
			else if (type == "System.Boolean")
			{
				bool trueBoolean = *(bool*)((int*)value + 4);
				if (trueBoolean)
				{
					ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "True");
				}
				else
				{
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "False");
				}
			}
			else if (type == "System.Guid")
			{
				string trueGuid = il2cpp_symbols::il2cpp_get_string(guid_hook(value));
				ImGui::TextWrapped(trueGuid.c_str());
			}
			else if (type == "UnityEngine.Vector2")
			{
				float x = *((float*)value + 4);
				float y = *((float*)value + 5);
				TextColoredWrapped(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "X=%.3f", x); ImGui::SameLine();
				TextColoredWrapped(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Y=%.3f", y);
			}
			else if (type == "UnityEngine.Vector3")
			{
				float x = *((float*)value + 4);
				float y = *((float*)value + 5);
				float z = *((float*)value + 6);
				TextColoredWrapped(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "X=%.3f", x); ImGui::SameLine();
				TextColoredWrapped(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Y=%.3f", y); ImGui::SameLine();
				TextColoredWrapped(ImVec4(0.529f, 0.808f, 0.922f, 1.0f), "Z=%.3f", z);
			}
			else if (type == "UnityEngine.Vector4" or type == "UnityEngine.Quaternion")
			{
				float x = *((float*)value + 4);
				float y = *((float*)value + 5);
				float z = *((float*)value + 6);
				float w = *((float*)value + 7);
				TextColoredWrapped(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "X=%.3f", x); ImGui::SameLine();
				TextColoredWrapped(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Y=%.3f", y); ImGui::SameLine();
				TextColoredWrapped(ImVec4(0.529f, 0.808f, 0.922f, 1.0f), "Z=%.3f", z); ImGui::SameLine();
				TextColoredWrapped(ImVec4(1.0f, 0.6f, 0.2f, 1.0f), "W=%.3f", w);
			}
			else if (type == "UnityEngine.Color")
			{
				float r = *((float*)value + 4);
				float g = *((float*)value + 5);
				float b = *((float*)value + 6);
				float a = *((float*)value + 7);

				string colorblock = "ColorBlock##" + path + to_string(color) + "Index" + to_string((uintptr_t)i);
				ImGui::ColorButton(colorblock.c_str(), ImVec4(r, g, b, 1), ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop, ImVec2(20, 20));
				ImGui::SameLine();

				TextColoredWrapped(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "R=%.3f", r); ImGui::SameLine();
				TextColoredWrapped(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "G=%.3f", g); ImGui::SameLine();
				TextColoredWrapped(ImVec4(0.529f, 0.808f, 0.922f, 1.0f), "B=%.3f", b); ImGui::SameLine();
				TextColoredWrapped(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "A=%.3f", a);
			}
			else if (type == "System.Char")
			{
				wchar_t trueChar = *(wchar_t*)((int*)value + 4);
				char buf[8] = { 0 };
				int len = std::wctomb(buf, trueChar);  // 转宽字再转多字节
				if (len > 0) ImGui::Text("%s", buf);
			}
			else if (il2cpp_class_is_valuetype(_class))
			{
				//ImGui::Text("This is Struct: 0X%p", value);

				string deeppath = path + to_string(color) + "Index" + to_string((uintptr_t)i);
				string structHeader = "Show Struct##" + deeppath;

				if (ImGui::CollapsingHeader(structHeader.c_str()))
				{
					GetField(base, value, _class, alivecheck, deeppath, color + 1);
				}
			}
			else if (type.find("[]") != string::npos)
			{
				int array_len = il2cpp_array_length(value);

				string deeppath = path + to_string(color) + "Index" + to_string((uintptr_t)i);
				string structHeader = "Show Array : " + to_string(array_len) + "##" + deeppath;

				if (ImGui::CollapsingHeader(structHeader.c_str()))
				{
					GetArray(base, value, removeTrailingBrackets(type), il2cpp_class_get_element_class(_class), alivecheck, deeppath, color + 1);
				}
			}
			else
			{
				ImGui::Text("0X%p", value);
				ImGui::SameLine();
				string objButtonName = "View##" + path + to_string((int)i) + "_" + to_string((int)value);
				if (ImGui::Button(objButtonName.c_str()))
				{
					ObjectInfo objData = { to_string((int)i) , type , value, alivecheck };

					if (base.important_obj)
					{
						objData.important_obj = base.important_obj;
					}

					AddInspectorSubList(objData);
				}
			}
		}
	}
}

//递归遍历所有Field
void GetField(ObjectInfo& base, void* obj, void* _class, AliveCheckInfo alivecheck, string path = "" , int color = 0) {
	
	if (path == "")
	{
		path = to_string((uintptr_t)obj);
	}
	
	void* iter = nullptr;
	void* parent = il2cpp_class_get_parent(_class);

	bool first = true;
	while (void* field = il2cpp_class_get_fields(_class, &iter))
	{
		ImGui::TableNextRow();

		// 取模选择颜色
		int colorIndex = color % (sizeof(levelColors) / sizeof(levelColors[0]));
		ImVec4 col = levelColors[colorIndex];

		if (colorIndex != 0) // 0 表示默认，不设置
			ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, ImGui::ColorConvertFloat4ToU32(col));

		ImGui::TableSetColumnIndex(0);
		if (first) {
			string className = il2cpp_type_get_name(il2cpp_class_get_type(_class));
			ImGui::TextWrapped(className.c_str());
			first = false;
		}
		ImGui::TableSetColumnIndex(1);
		string fieldName = il2cpp_field_get_name(field);
		ImGui::TextWrapped(fieldName.c_str());
		ImGui::TableSetColumnIndex(2);
		void* fieldType = il2cpp_field_get_type(field);
		string fieldTypeName = il2cpp_type_get_name(fieldType);
		ImGui::TextWrapped(fieldTypeName.c_str());
		ImGui::TableSetColumnIndex(3);
		void* value = il2cpp_field_get_value_object(field, obj);
		if (value) {
			void* fieldClass = il2cpp_object_get_class(value);
			if (il2cpp_class_is_enum(fieldClass)) {
				int trueEnumValue = *((int*)value + 4);
				string trueEnumName = GetEnumName(fieldClass, trueEnumValue);
				ImGui::TextWrapped("%d (%s)", trueEnumValue, trueEnumName.c_str());
			}
			else if (fieldTypeName == "System.String") {
				string trueText = il2cpp_symbols::il2cpp_get_string(value);
				TextColoredWrapped(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), trueText.c_str());
			}
			else if (fieldTypeName == "System.Int32") {
				int trueInt32 = *((int*)value + 4);
				ImGui::TextWrapped(to_string(trueInt32).c_str());
			}
			else if (fieldTypeName == "System.UInt32") {
				unsigned int trueUInt32 = *((unsigned int*)value + 4);
				ImGui::TextWrapped(to_string(trueUInt32).c_str());
			}
			else if (fieldTypeName == "System.IntPtr") {
				size_t trueIntPtr = *(size_t*)((int*)value + 4);
				ImGui::TextWrapped("0X%p", trueIntPtr);
			}
			else if (fieldTypeName == "System.Single") {
				float trueSingle = *(float*)((int*)value + 4);
				ImGui::TextWrapped(to_string(trueSingle).c_str());
			}
			else if (fieldTypeName == "System.Boolean") {
				bool trueBoolean = *(bool*)((int*)value + 4);
				if (trueBoolean) {
					ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "True");
				}
				else {
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "False");
				}
			}
			else if (fieldTypeName == "System.Guid") {
				string trueGuid = il2cpp_symbols::il2cpp_get_string(guid_hook(value));
				ImGui::TextWrapped(trueGuid.c_str());
			}
			else if (fieldTypeName == "UnityEngine.Vector2") {
				float x = *((float*)value + 4);
				float y = *((float*)value + 5);
				TextColoredWrapped(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "X=%.3f", x); ImGui::SameLine();
				TextColoredWrapped(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Y=%.3f", y);
			}
			else if (fieldTypeName == "UnityEngine.Vector3") {
				float x = *((float*)value + 4);
				float y = *((float*)value + 5);
				float z = *((float*)value + 6);
				TextColoredWrapped(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "X=%.3f", x); ImGui::SameLine();
				TextColoredWrapped(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Y=%.3f", y); ImGui::SameLine();
				TextColoredWrapped(ImVec4(0.529f, 0.808f, 0.922f, 1.0f), "Z=%.3f", z);
			}
			else if (fieldTypeName == "UnityEngine.Vector4" or fieldTypeName == "UnityEngine.Quaternion") {
				float x = *((float*)value + 4);
				float y = *((float*)value + 5);
				float z = *((float*)value + 6);
				float w = *((float*)value + 7);
				TextColoredWrapped(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "X=%.3f", x); ImGui::SameLine();
				TextColoredWrapped(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Y=%.3f", y); ImGui::SameLine();
				TextColoredWrapped(ImVec4(0.529f, 0.808f, 0.922f, 1.0f), "Z=%.3f", z); ImGui::SameLine();
				TextColoredWrapped(ImVec4(1.0f, 0.6f, 0.2f, 1.0f), "W=%.3f", w);
			}
			else if (fieldTypeName == "UnityEngine.Color") 
			{
				float r = *((float*)value + 4);
				float g = *((float*)value + 5);
				float b = *((float*)value + 6);
				float a = *((float*)value + 7);

				string colorblock = "ColorBlock##" + path + to_string(color) + to_string((uintptr_t)field);
				ImGui::ColorButton(colorblock.c_str(), ImVec4(r, g, b, 1), ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop, ImVec2(20, 20));
				ImGui::SameLine();

				TextColoredWrapped(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "R=%.3f", r); ImGui::SameLine();
				TextColoredWrapped(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "G=%.3f", g); ImGui::SameLine();
				TextColoredWrapped(ImVec4(0.529f, 0.808f, 0.922f, 1.0f), "B=%.3f", b); ImGui::SameLine();
				TextColoredWrapped(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "A=%.3f", a);
			}
			else if (fieldTypeName == "System.Char")
			{
				wchar_t trueChar = *(wchar_t*)((int*)value + 4);
				char buf[8] = { 0 };
				int len = std::wctomb(buf, trueChar);  // 转宽字再转多字节
				if (len > 0) ImGui::Text("%s", buf);
			}
			else if (il2cpp_class_is_valuetype(fieldClass))
			{
				//ImGui::Text("This is Struct: 0X%p", value);

				string deeppath = path + to_string(color) + to_string((uintptr_t)field);
				string structHeader = "Show Struct##" + deeppath;

				if (ImGui::CollapsingHeader(structHeader.c_str()))
				{
					GetField(base, value, fieldClass, alivecheck, deeppath, color + 1);
				}
			}
			else if (fieldTypeName.rfind("[]") == fieldTypeName.size() - 2)
			{
				int array_len = il2cpp_array_length(value);

				string deeppath = path + to_string(color) + to_string((uintptr_t)field);
				string structHeader = "Show Array : " + to_string(array_len) + "##" + deeppath;

				if (ImGui::CollapsingHeader(structHeader.c_str()))
				{
					GetArray(base, value, removeTrailingBrackets(fieldTypeName), il2cpp_class_get_element_class(fieldClass), alivecheck, deeppath, color + 1);
				}
			}
			else 
			{
				ImGui::Text("0X%p", value);
				ImGui::SameLine();
				string objButtonName = "View##" + to_string((int)field) + "_" + to_string((int)value);
				if (ImGui::Button(objButtonName.c_str()))
				{
					ObjectInfo objData = { fieldName , fieldTypeName , value, alivecheck };
					AddInspectorSubList(objData);
				}
			}
		}
		else {
			ImGui::TextDisabled("Empty");
		}

	}
	if (parent) {
		GetField(base, obj, parent, alivecheck);
	}
}

//创建FieldWindow
void CreateFieldTable(ObjectInfo& obj, void* _class, AliveCheckInfo alivecheck, int i) {
	ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable;

	//开始Field处理

	string componentField = to_string(int(obj.obj)) + "_field_" + to_string(i);
	if (ImGui::TreeNode((componentField + "_tree").c_str(), "Field")) {
		if (ImGui::BeginTable((componentField + "_table").c_str(), 4, flags)) {
			ImGui::TableSetupColumn("Class Name");
			ImGui::TableSetupColumn("Field Name");
			ImGui::TableSetupColumn("Field Type");
			ImGui::TableSetupColumn("Field Value");
			ImGui::TableHeadersRow();

			GetField(obj, obj.obj, _class, alivecheck);

			ImGui::EndTable();
		}
		ImGui::TreePop();
	}
}

//递归遍历所有Property
void GetProperty(void* obj, void* _class, int* index, vector<void*> new_properties, vector<string> propertyList, int proLength, AliveCheckInfo alivecheck)
{
	void* iter = nullptr;
	void* parent = il2cpp_class_get_parent(_class);

	bool first = true;

	while (void* property = il2cpp_class_get_properties(_class, &iter))
	{

		if (*index == proLength)
		{
			break;
		}

		string propertyName = il2cpp_property_get_name(property);
		//printf("Property Name is %s\n", propertyName.c_str());
		//printf("Index is %d\n", *index);

		void* pro = new_properties[*index];
		string trueProName = propertyList[*index];
		//printf("New Name is %s\n", trueProName.c_str());



		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		string className = il2cpp_type_get_name(il2cpp_class_get_type(_class));
		if (first)
		{
			ImGui::TextWrapped(className.c_str());
			first = false;
		}
		ImGui::TableSetColumnIndex(1);
		ImGui::TextWrapped(propertyName.c_str());

		MethodInfo* getMethod;
		if (getMethod = il2cpp_property_get_get_method(property))
		{
			//printf("Start Here\n");
			ImGui::TableSetColumnIndex(2);
			void* propertyType = nullptr;
			void* propertyClass = nullptr;
			string propertyTypeName;
			propertyType = il2cpp_method_get_return_type(getMethod);
			//printf("Get Type OK\n");
			if (propertyType)
			{
				propertyTypeName = il2cpp_type_get_name(propertyType);
				propertyClass = il2cpp_class_from_type(propertyType);
				//printf("Get Name OK is %s\n", propertyTypeName.c_str());
				ImGui::TextWrapped(propertyTypeName.c_str());
			}
			//printf("Everything OK");
			//printf("Length is %d, Index is %d\n", proLength, *index);
			//printf("Pro address is %p\n", pro);

			if (propertyName != trueProName)
			{
				continue;
			}

			void* value = nullptr;

			bool close_button = true;
			string getName = "get##" + to_string((int)property) + "_" + to_string((int)index);
			ImGui::TableSetColumnIndex(3);
			try
			{
				if (il2cpp_class_is_enum(propertyClass))
				{
					value = property_hook(pro, obj);
					if (value)
					{
						int trueEnumValue = *((int*)value + 4);
						string trueEnumName = GetEnumName(propertyClass, trueEnumValue);
						ImGui::TextWrapped("%d (%s)", trueEnumValue, trueEnumName.c_str());
					}
				}
				else if (propertyTypeName == "System.String")
				{
					value = property_hook(pro, obj);
					if (value)
					{
						string trueText = il2cpp_symbols::il2cpp_get_string(value);
						TextColoredWrapped(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), trueText.c_str());
					}
				}
				else if (propertyTypeName == "System.Int32")
				{
					value = property_hook(pro, obj);
					if (value)
					{
						int trueInt32 = *((int*)value + 4);
						ImGui::TextWrapped(to_string(trueInt32).c_str());
					}
				}
				else if (propertyTypeName == "System.UInt32")
				{
					value = property_hook(pro, obj);
					if (value)
					{
						unsigned int trueUInt32 = *((unsigned int*)value + 4);
						ImGui::TextWrapped(to_string(trueUInt32).c_str());
					}
				}
				else if (propertyTypeName == "System.Int64")
				{
					value = property_hook(pro, obj);
					if (value)
					{
						size_t trueInt64 = *(size_t*)((int*)value + 4);
						ImGui::TextWrapped("%d", trueInt64);
					}
				}
				else if (propertyTypeName == "System.IntPtr")
				{
					value = property_hook(pro, obj);
					if (value)
					{
						size_t trueIntPtr = *(size_t*)((int*)value + 4);
						ImGui::TextWrapped("0X%p", trueIntPtr);
					}
				}
				else if (propertyTypeName == "System.Boolean")
				{
					value = property_hook(pro, obj);
					if (value)
					{
						bool trueBoolean = *(bool*)((int*)value + 4);
						if (trueBoolean)
						{
							ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "True");
						}
						else
						{
							ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "False");
						}
					}
				}
				else if (propertyTypeName == "System.Single")
				{
					value = property_hook(pro, obj);
					if (value)
					{
						float trueSingle = *(float*)((int*)value + 4);
						ImGui::TextWrapped(to_string(trueSingle).c_str());
					}
				}
				else if (propertyTypeName == "System.Guid")
				{
					value = property_hook(pro, obj);
					if (value)
					{
						string trueGuid = il2cpp_symbols::il2cpp_get_string(guid_hook(value));
						ImGui::TextWrapped(trueGuid.c_str());
					}
				}

				else if (propertyTypeName == "UnityEngine.Vector2")
				{
					value = property_hook(pro, obj);
					if (value)
					{
						float vector[] = { *((float*)value + 4),*((float*)value + 5) };
						string vectorName = "##Vector2_" + to_string((int)property);
						TextColoredWrapped(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "X=%.3f", vector[0]); ImGui::SameLine();
						TextColoredWrapped(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Y=%.3f", vector[1]);
					}
				}
				else if (propertyTypeName == "UnityEngine.Vector3")
				{
					value = property_hook(pro, obj);
					if (value)
					{
						float vector[] = { *((float*)value + 4),*((float*)value + 5), *((float*)value + 6) };
						string vectorName = "##Vector3_" + to_string((int)property);
						TextColoredWrapped(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "X=%.3f", vector[0]); ImGui::SameLine();
						TextColoredWrapped(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Y=%.3f", vector[1]); ImGui::SameLine();
						TextColoredWrapped(ImVec4(0.529f, 0.808f, 0.922f, 1.0f), "Z=%.3f", vector[2]);
					}
				}
				else if (propertyTypeName == "UnityEngine.Vector4" or propertyTypeName == "UnityEngine.Quaternion")
				{
					value = property_hook(pro, obj);
					if (value)
					{
						float vector[] = { *((float*)value + 4),*((float*)value + 5), *((float*)value + 6), *((float*)value + 7) };
						string vectorName = "##Vector4_" + to_string((int)property);
						TextColoredWrapped(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "X=%.3f", vector[0]); ImGui::SameLine();
						TextColoredWrapped(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Y=%.3f", vector[1]); ImGui::SameLine();
						TextColoredWrapped(ImVec4(0.529f, 0.808f, 0.922f, 1.0f), "Z=%.3f", vector[2]); ImGui::SameLine();
						TextColoredWrapped(ImVec4(1.0f, 0.6f, 0.2f, 1.0f), "W=%.3f", vector[3]);
					}
				}
				else if (il2cpp_class_is_valuetype(propertyClass))
				{
					TextColoredWrapped(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "This is a Struct! Very dangerous! I won't let you have it!");
				}
				else
				{
					//value = property_hook(pro, obj);
					//ImGui::Text("0X%p", value);
					close_button = false;
				}
				ImGui::TableSetColumnIndex(4);
				ImGui::BeginDisabled(close_button);
				if (ImGui::Button(getName.c_str()))
				{

					try
					{
						value = property_hook(pro, obj);
						if (value)
						{
							printf("Get!\n");
							if (propertyType)
							{
								ObjectInfo objData = { propertyName, propertyTypeName, value, alivecheck };
								if (className == "UnityEngine.Renderer")
								{
									if (propertyTypeName == "UnityEngine.Material" or propertyTypeName == "UnityEngine.Material[]")
									{
										objData.important_obj = obj;
									}
								}
								AddInspectorSubList(objData);
							}	
						}
						else
						{
							printf("Empty!\n");
						}
					}
					catch (...)
					{
						printf("Error!\n");
					}
				}
				ImGui::SameLine();
				ImGui::EndDisabled();
			}
			catch (...)
			{
				printf("Exit!\n");
				return;
			}
		}
		MethodInfo* setMethod;
		if (setMethod = il2cpp_property_get_set_method(property))
		{
			string setName = "set##" + to_string((int)property) + "_" + to_string((int)index);
			ImGui::TableSetColumnIndex(4);
			ImGui::BeginDisabled();
			if (ImGui::Button(setName.c_str()))
			{

			}
			ImGui::EndDisabled();
		}
		//printf("OK %s\n", propertyName.c_str());
		if (propertyName != trueProName)
		{
			continue;
		}
		*index += 1;
	}
	if (parent)
	{
		GetProperty(obj, parent, index, new_properties, propertyList, proLength, alivecheck);
	}
}

//创建PropertyWindow
void CreatePropertyTable(ObjectInfo& obj, void* _class, AliveCheckInfo alivecheck, int i)
{
	ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable;

	string componentProperty = to_string(int(obj.obj)) + "_property_" + to_string(i);

	if (ImGui::TreeNode((componentProperty + "_tree").c_str(), "Property"))
	{
		if (ImGui::BeginTable((componentProperty + "_table").c_str(), 5, flags))
		{
			if (!namePro)
			{
				void* assembly = refload_hook(il2cpp_symbols::get_il2cpp_string("mscorlib"));
				void* Global_ProInfo = reftype_hook(assembly, il2cpp_symbols::get_il2cpp_string("System.Reflection.PropertyInfo"));
				namePro = typePro_hook(Global_ProInfo, il2cpp_symbols::get_il2cpp_string("Name"));
				printf("Get namePro Success!\n");
			}

			if (!obj.propertyInfo.state)
			{
				PropertyInfo proInfo;
				proInfo.type = objType_hook(obj.obj);
				void* tempProperties = typePros_hook(proInfo.type, 4 | 8 | 16 | 32);
				proInfo.proLength = il2cpp_symbols::get_array_length(tempProperties);

				vector<string> proNameList;
				vector<void*> proList;
				for (int i = 0; i < proInfo.proLength; i++)
				{
					void* pro = arrayindex_hook(tempProperties, i);
					proList.push_back(pro);
					proNameList.push_back(il2cpp_symbols::il2cpp_get_string(property_hook(namePro, pro)));
				}

				proInfo.properties = proList;
				proInfo.propertyNameList = proNameList;

				obj.propertyInfo = proInfo;
				obj.propertyInfo.state = true;

				printf("Get Everything Success!\n");
			}

			ImGui::TableSetupColumn("Class Name", ImGuiTableColumnFlags_WidthStretch, 2.0f);
			ImGui::TableSetupColumn("Property Name", ImGuiTableColumnFlags_WidthStretch, 2.0f);
			ImGui::TableSetupColumn("Property Type", ImGuiTableColumnFlags_WidthStretch, 2.0f);
			ImGui::TableSetupColumn("Property Value", ImGuiTableColumnFlags_WidthStretch, 2.0f);
			ImGui::TableSetupColumn("Property Method", ImGuiTableColumnFlags_WidthStretch, 1.0f);
			ImGui::TableHeadersRow();

			
			try
			{
				int index = 0;
				GetProperty(obj.obj, _class, &index, obj.propertyInfo.properties, obj.propertyInfo.propertyNameList, obj.propertyInfo.proLength, alivecheck);
			}
			catch (...)
			{
				printf("Error happen during !\n");
			}
			

			ImGui::EndTable();
		}

		ImGui::TreePop();
	}
}

void CreateArrayTable(ObjectInfo& obj, string type, void* _class, AliveCheckInfo alivecheck, int i)
{
	ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable;

	string componentArray = to_string(int(obj.obj)) + "_array_" + to_string(i);

	if (ImGui::TreeNode((componentArray + "_tree").c_str(), "Array"))
	{
		if (ImGui::BeginTable((componentArray + "_table").c_str(), 4, flags))
		{

			ImGui::TableSetupColumn("Class Name", ImGuiTableColumnFlags_WidthStretch, 2.0f);
			ImGui::TableSetupColumn("Array Index", ImGuiTableColumnFlags_WidthStretch, 2.0f);
			ImGui::TableSetupColumn("Array Type", ImGuiTableColumnFlags_WidthStretch, 2.0f);
			ImGui::TableSetupColumn("Array Value", ImGuiTableColumnFlags_WidthStretch, 2.0f);
			ImGui::TableHeadersRow();


			GetArray(obj, obj.obj, type, il2cpp_class_get_element_class(_class), alivecheck);


			ImGui::EndTable();
		}

		ImGui::TreePop();
	}
}

void GetShaderProperty(ObjectInfo& obj, AliveCheckInfo alivecheck, string path = "", int color = 0)
{
	for (int i = 0; i < obj.materialInfo.shader.shaderProperties.size(); i++)
	{
		ImGui::TableNextRow();

		// 取模选择颜色
		int colorIndex = color % (sizeof(levelColors) / sizeof(levelColors[0]));
		ImVec4 col = levelColors[colorIndex];

		if (colorIndex != 0) // 0 表示默认，不设置
			ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, ImGui::ColorConvertFloat4ToU32(col));

		int id = obj.materialInfo.shader.shaderProperties[i].id;
		string name = obj.materialInfo.shader.shaderProperties[i].name;
		ShaderPropertyType type = obj.materialInfo.shader.shaderProperties[i].type;

		ImGui::TableSetColumnIndex(0);
		ImGui::TextWrapped(name.c_str());

		if (type == ShaderPropertyType::ShaderPropertyType_Color)
		{
			ImGui::TableSetColumnIndex(1);
			ImGui::TextWrapped("Color");
			ImGui::TableSetColumnIndex(2);
			Color tempColor;
			material_GetColor_hook(obj.obj, id, &tempColor);
			//printf("Get Success!\n");
			float r = tempColor.r;
			float g = tempColor.g;
			float b = tempColor.b;
			float a = tempColor.a;
			string colorblock = "ColorBlock##" + name + to_string(type) + to_string(color) + "Index" + to_string((uintptr_t)i);
			ImGui::ColorButton(colorblock.c_str(), ImVec4(r, g, b, 1), ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop, ImVec2(20, 20));
			ImGui::SameLine();
			TextColoredWrapped(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "R=%.3f", r); ImGui::SameLine();
			TextColoredWrapped(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "G=%.3f", g); ImGui::SameLine();
			TextColoredWrapped(ImVec4(0.529f, 0.808f, 0.922f, 1.0f), "B=%.3f", b); ImGui::SameLine();
			TextColoredWrapped(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "A=%.3f", a);
		}
		else if (type == ShaderPropertyType::ShaderPropertyType_Vector)
		{
			ImGui::TableSetColumnIndex(1);
			ImGui::TextWrapped("Vector");
			ImGui::TableSetColumnIndex(2);
			Vector4 value = material_GetVector_hook(obj.obj, id);
			//printf("Get Success!\n");
			float x = value.x;
			float y = value.y;
			float z = value.z;
			float w = value.w;
			TextColoredWrapped(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "X=%.3f", x); ImGui::SameLine();
			TextColoredWrapped(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Y=%.3f", y); ImGui::SameLine();
			TextColoredWrapped(ImVec4(0.529f, 0.808f, 0.922f, 1.0f), "Z=%.3f", z); ImGui::SameLine();
			TextColoredWrapped(ImVec4(1.0f, 0.6f, 0.2f, 1.0f), "W=%.3f", w);
		}
		else if (type == ShaderPropertyType::ShaderPropertyType_Float)
		{
			ImGui::TableSetColumnIndex(1);
			ImGui::TextWrapped("Float");
			ImGui::TableSetColumnIndex(2);
			float value = material_GetFloat_hook(obj.obj, id);
			ImGui::TextWrapped(to_string(value).c_str());
		}
		else if (type == ShaderPropertyType::ShaderPropertyType_Range)
		{
			ImGui::TableSetColumnIndex(1);
			ImGui::TextWrapped("Range");
			ImGui::TableSetColumnIndex(2);
			float value = material_GetFloat_hook(obj.obj, id);
			ImGui::TextWrapped(to_string(value).c_str());
		}
		else if (type == ShaderPropertyType::ShaderPropertyType_Texture)
		{
			ImGui::TableSetColumnIndex(1);
			ImGui::TextWrapped("Texture");
		}
		else if (type == ShaderPropertyType::ShaderPropertyType_Int)
		{
			ImGui::TableSetColumnIndex(1);
			ImGui::TextWrapped("Int");
		}
	}
}

void CreateShaderPropertyTable(ObjectInfo& obj, AliveCheckInfo alivecheck, int i)
{
	ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable;
	string componentShaderProperty = to_string(int(obj.obj)) + "_shader_property_" + to_string(i);
	if (ImGui::TreeNode((componentShaderProperty + "_tree").c_str(), "Shader Property"))
	{
		if (ImGui::BeginTable((componentShaderProperty + "_table").c_str(), 3, flags))
		{
			ImGui::TableSetupColumn("Property Name", ImGuiTableColumnFlags_WidthStretch, 2.0f);
			ImGui::TableSetupColumn("Property Type", ImGuiTableColumnFlags_WidthStretch, 2.0f);
			ImGui::TableSetupColumn("Property Value", ImGuiTableColumnFlags_WidthStretch, 2.0f);
			ImGui::TableHeadersRow();
			GetShaderProperty(obj, alivecheck);
			ImGui::EndTable();
		}
		ImGui::TreePop();
	}
}

void GetMaterialBlockProperty(ObjectInfo& obj, AliveCheckInfo alivecheck, string path = "", int color = 0)
{
	void* mpb = MaterialPropertyBlock_ctor();
	Renderer_GetPropertyBlock_hook(obj.important_obj, mpb);

	for (int i = 0; i < obj.materialInfo.shader.shaderProperties.size(); i++)
	{
		int id = obj.materialInfo.shader.shaderProperties[i].id;
		if (MaterialPropertyBlock_HasPropertyImpl_hook(mpb, id))
		{
			ImGui::TableNextRow();

			// 取模选择颜色
			int colorIndex = color % (sizeof(levelColors) / sizeof(levelColors[0]));
			ImVec4 col = levelColors[colorIndex];

			if (colorIndex != 0) // 0 表示默认，不设置
				ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, ImGui::ColorConvertFloat4ToU32(col));

			string name = obj.materialInfo.shader.shaderProperties[i].name;
			ShaderPropertyType type = obj.materialInfo.shader.shaderProperties[i].type;
			//printf("True Property:%s\n", obj.materialInfo.shader.shaderProperties[j].name.c_str());

			ImGui::TableSetColumnIndex(0);
			ImGui::TextWrapped(name.c_str());

			if (type == ShaderPropertyType::ShaderPropertyType_Color)
			{
				ImGui::TableSetColumnIndex(1);
				ImGui::TextWrapped("Color");
				ImGui::TableSetColumnIndex(2);
				
				Color tempColor = MaterialPropertyBlock_GetColor_hook(mpb, id);
				//printf("Get Success!\n");
				float r = tempColor.r;
				float g = tempColor.g;
				float b = tempColor.b;
				float a = tempColor.a;
				string colorblock = "ColorBlock##" + name + to_string(type) + to_string(color) + "Index" + to_string((uintptr_t)i);
				ImGui::ColorButton(colorblock.c_str(), ImVec4(r, g, b, 1), ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop, ImVec2(20, 20));
				ImGui::SameLine();
				TextColoredWrapped(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "R=%.3f", r); ImGui::SameLine();
				TextColoredWrapped(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "G=%.3f", g); ImGui::SameLine();
				TextColoredWrapped(ImVec4(0.529f, 0.808f, 0.922f, 1.0f), "B=%.3f", b); ImGui::SameLine();
				TextColoredWrapped(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "A=%.3f", a);
			}
			else if (type == ShaderPropertyType::ShaderPropertyType_Vector)
			{
				ImGui::TableSetColumnIndex(1);
				ImGui::TextWrapped("Vector");
				ImGui::TableSetColumnIndex(2);
				Vector4 value = MaterialPropertyBlock_GetVector_hook(mpb, id);
				//printf("Get Success!\n");
				float x = value.x;
				float y = value.y;
				float z = value.z;
				float w = value.w;
				TextColoredWrapped(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "X=%.3f", x); ImGui::SameLine();
				TextColoredWrapped(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Y=%.3f", y); ImGui::SameLine();
				TextColoredWrapped(ImVec4(0.529f, 0.808f, 0.922f, 1.0f), "Z=%.3f", z); ImGui::SameLine();
				TextColoredWrapped(ImVec4(1.0f, 0.6f, 0.2f, 1.0f), "W=%.3f", w);
			}
			else if (type == ShaderPropertyType::ShaderPropertyType_Float)
			{
				ImGui::TableSetColumnIndex(1);
				ImGui::TextWrapped("Float");
				ImGui::TableSetColumnIndex(2);
				float value = MaterialPropertyBlock_GetFloat_hook(mpb, id);
				ImGui::TextWrapped(to_string(value).c_str());
			}
			else if (type == ShaderPropertyType::ShaderPropertyType_Range)
			{
				ImGui::TableSetColumnIndex(1);
				ImGui::TextWrapped("Range");
				ImGui::TableSetColumnIndex(2);
				float value = MaterialPropertyBlock_GetFloat_hook(mpb, id);
				ImGui::TextWrapped(to_string(value).c_str());
			}
			else if (type == ShaderPropertyType::ShaderPropertyType_Texture)
			{
				ImGui::TableSetColumnIndex(1);
				ImGui::TextWrapped("Texture");
			}
			else if (type == ShaderPropertyType::ShaderPropertyType_Int)
			{
				ImGui::TableSetColumnIndex(1);
				ImGui::TextWrapped("Int");
			}
		}
	}
}

void CreateMaterialPropertyBlockTable(ObjectInfo& obj, AliveCheckInfo alivecheck, int i)
{
	ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable;
	string componentMaterialPropertyBlock = to_string(int(obj.obj)) + "_material_property_block_" + to_string(i);
	if (ImGui::TreeNode((componentMaterialPropertyBlock + "_tree").c_str(), "Material Property Block"))
	{
		if (ImGui::BeginTable((componentMaterialPropertyBlock + "_table").c_str(), 3, flags))
		{
			ImGui::TableSetupColumn("Property Name", ImGuiTableColumnFlags_WidthStretch, 2.0f);
			ImGui::TableSetupColumn("Property Type", ImGuiTableColumnFlags_WidthStretch, 2.0f);
			ImGui::TableSetupColumn("Property Value", ImGuiTableColumnFlags_WidthStretch, 2.0f);
			ImGui::TableHeadersRow();
			GetMaterialBlockProperty(obj, alivecheck);
			ImGui::EndTable();
		}
		ImGui::TreePop();
	}
}