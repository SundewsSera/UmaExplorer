#include <locale>
#include <string>
#include <iostream>
#include <sstream>
#include <windows.h>
#include <MinHook.h>
#include <fstream>
#include <nlohmann/json.hpp>

#include <jsoncons/json.hpp>
#include <jsoncons_ext/msgpack/msgpack.hpp>

#include "il2cpp_utils.h"
#include "egui/egui.h"

#include "hook_utils.h"
#include "globals.h"

using namespace std;
using namespace std::literals;

#pragma region GlobalValueDefine
using njson = nlohmann::json;
using namespace jsoncons;

#pragma endregion

#pragma region StructDefine

#pragma endregion

#pragma region HookFunction
void create_debug_console()
{
	AllocConsole();

	FILE* _;
	// open stdout stream
	freopen_s(&_, "CONOUT$", "w", stdout);
	freopen_s(&_, "CONOUT$", "w", stderr);
	freopen_s(&_, "CONIN$", "r", stdin);

	SetConsoleTitle(L"UmaExplorer");

	// set this to avoid turn japanese texts into question mark
	SetConsoleOutputCP(CP_UTF8);
	std::locale::global(std::locale(""));
}

std::string current_time()
{
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch());
	return std::to_string(ms.count());
}

void write_file(std::string file_name, char* buffer, int len)
{
	FILE* fp;
	fopen_s(&fp, file_name.c_str(), "wb");
	if (fp != nullptr)
	{
		fwrite(buffer, 1, len, fp);
		fclose(fp);
	}
}

void* key_orig = nullptr;

void* key_hook(void* dst)
{
	void* ret = reinterpret_cast<decltype(key_hook)*>(key_orig)(dst);

	return ret;
}

void* localkey_orig = nullptr;

void* localkey_hook(void* _this)
{
	void* ret = reinterpret_cast<decltype(localkey_hook)*>(localkey_orig)(_this);

	return ret;
}

void* Shader_GetPropertyCount_orig = nullptr;

int Shader_GetPropertyCount_hook(void* _this)
{
	int ret = reinterpret_cast<decltype(Shader_GetPropertyCount_hook)*>(Shader_GetPropertyCount_orig)(_this);

	return ret;
}

void* Shader_GetPropertyName_orig = nullptr;

void* Shader_GetPropertyName_hook(void* _this, int index)
{
	void* ret = reinterpret_cast<decltype(Shader_GetPropertyName_hook)*>(Shader_GetPropertyName_orig)(_this, index);

	return ret;
}

void* Shader_GetPropertyType_orig = nullptr;

int Shader_GetPropertyType_hook(void* _this, int index)
{

	int ret = reinterpret_cast<decltype(Shader_GetPropertyType_hook)*>(Shader_GetPropertyType_orig)(_this, index);
	return ret;

}

void* Shader_GetPropertyNameId_orig = nullptr;

int Shader_GetPropertyNameId_hook(void* _this, int index)
{

	int ret = reinterpret_cast<decltype(Shader_GetPropertyNameId_hook)*>(Shader_GetPropertyNameId_orig)(_this, index);
	return ret;

}

void* Renderer_GetPropertyBlock_orig = nullptr;

void Renderer_GetPropertyBlock_hook(void* _this, void* dst)
{
	reinterpret_cast<decltype(Renderer_GetPropertyBlock_hook)*>(Renderer_GetPropertyBlock_orig)(_this, dst);
}

void* MaterialPropertyBlock_HasPropertyImpl_orig = nullptr;

bool MaterialPropertyBlock_HasPropertyImpl_hook(void* _this, int nameId)
{
	bool ret = reinterpret_cast<decltype(MaterialPropertyBlock_HasPropertyImpl_hook)*>(MaterialPropertyBlock_HasPropertyImpl_orig)(_this, nameId);
	return ret;
}

#pragma endregion

#pragma region EnableHook
void bootstrap_carrot_juicer()
{

	auto libnative_module = GetModuleHandle(L"libnative.dll");
	printf("libnative.dll at %p\n", libnative_module);
	if (libnative_module == nullptr)
	{
		return;
	}

	printf("Trying to patch GameAssembly.dll...\n");

	auto il2cpp_module = GetModuleHandle(L"GameAssembly.dll");

	il2cpp_symbols::init(il2cpp_module);

	auto unityplayer_module = GetModuleHandle(L"UnityPlayer_orig.dll");

	printf("UnityPlayer_orig.dll at %p\n", unityplayer_module);

	unity_base = (void*)unityplayer_module;

	setup_hook_utils();

	//auto key_addr = il2cpp_resolve_icall("UnityEngine.Rendering.ShaderKeyword::GetGlobalKeywordIndex(System.String)");

	auto key_addr = (void*)((uintptr_t)unityplayer_module + 0x655E00);

	printf("key_addr at %p\n", key_addr);

	MH_CreateHook((LPVOID)key_addr, key_hook, &key_orig);
	MH_EnableHook((LPVOID)key_addr);

	auto localkey_addr = (void*)((uintptr_t)unityplayer_module + 0xB2F00);

	printf("localkey_addr at %p\n", localkey_addr);

	MH_CreateHook((LPVOID)localkey_addr, localkey_hook, &localkey_orig);
	MH_EnableHook((LPVOID)localkey_addr);

	auto Shader_GetPropertyCount_addr = (void*)((uintptr_t)unityplayer_module + 0xAF2E0);

	printf("Shader_GetPropertyCount_addr at %p\n", Shader_GetPropertyCount_addr);

	MH_CreateHook((LPVOID)Shader_GetPropertyCount_addr, Shader_GetPropertyCount_hook, &Shader_GetPropertyCount_orig);
	MH_EnableHook((LPVOID)Shader_GetPropertyCount_addr);

	auto Shader_GetPropertyName_addr = (void*)((uintptr_t)unityplayer_module + 0xAE1A0);

	printf("Shader_GetPropertyName_addr at %p\n", Shader_GetPropertyName_addr);

	MH_CreateHook((LPVOID)Shader_GetPropertyName_addr, Shader_GetPropertyName_hook, &Shader_GetPropertyName_orig);
	MH_EnableHook((LPVOID)Shader_GetPropertyName_addr);

	auto Shader_GetPropertyType_addr = (void*)((uintptr_t)unityplayer_module + 0xAE440);

	printf("Shader_GetPropertyType_addr at %p\n", Shader_GetPropertyType_addr);

	MH_CreateHook((LPVOID)Shader_GetPropertyType_addr, Shader_GetPropertyType_hook, &Shader_GetPropertyType_orig);
	MH_EnableHook((LPVOID)Shader_GetPropertyType_addr);

	auto Shader_GetPropertyNameId_addr = (void*)((uintptr_t)unityplayer_module + 0xAE300);

	printf("Shader_GetPropertyNameId_addr at %p\n", Shader_GetPropertyNameId_addr);

	MH_CreateHook((LPVOID)Shader_GetPropertyNameId_addr, Shader_GetPropertyNameId_hook, &Shader_GetPropertyNameId_orig);
	MH_EnableHook((LPVOID)Shader_GetPropertyNameId_addr);

	auto Renderer_GetPropertyBlock_addr = (void*)((uintptr_t)unityplayer_module + 0xA1C00);

	printf("Renderer_GetPropertyBlock_addr at %p\n", Renderer_GetPropertyBlock_addr);

	MH_CreateHook((LPVOID)Renderer_GetPropertyBlock_addr, Renderer_GetPropertyBlock_hook, &Renderer_GetPropertyBlock_orig);
	MH_EnableHook((LPVOID)Renderer_GetPropertyBlock_addr);

	auto MaterialPropertyBlock_HasPropertyImpl_addr = (void*)((uintptr_t)unityplayer_module + 0x9C3F0);

	printf("MaterialPropertyBlock_HasPropertyImpl_addr at %p\n", MaterialPropertyBlock_HasPropertyImpl_addr);

	MH_CreateHook((LPVOID)MaterialPropertyBlock_HasPropertyImpl_addr, MaterialPropertyBlock_HasPropertyImpl_hook, &MaterialPropertyBlock_HasPropertyImpl_orig);

	//il2cpp_symbols::class_print_all_method(il2cpp_symbols::get_class("UnityEngine.CoreModule.dll", "UnityEngine", "Shader"));

	//Ö´ÐÐGUI³ÌÐò
	thread([]() {
		auto tr = il2cpp_thread_attach(il2cpp_domain_get());

		printf("I am Running In Here!!!\n");
		imguiwindow();

		il2cpp_thread_detach(tr);
		}).detach();
}
#pragma endregion


void* load_library_w_orig = nullptr;

HMODULE __stdcall load_library_w_hook(const wchar_t* path)
{
	printf("Saw %ls\n", path);

	// GameAssembly.dll code must be loaded and decrypted while loading criware library
	if (path == L"cri_ware_unity.dll"s)
	{
		bootstrap_carrot_juicer();

		MH_DisableHook(LoadLibraryW);
		MH_RemoveHook(LoadLibraryW);

		return LoadLibraryW(path);
	}

	return reinterpret_cast<decltype(LoadLibraryW)*>(load_library_w_orig)(path);
}


void attach()
{
	create_debug_console();

	if (MH_Initialize() != MH_OK)
	{
		printf("Failed to initialize MinHook.\n");
		return;
	}
	printf("MinHook initialized.\n");

	MH_CreateHook(LoadLibraryW, load_library_w_hook, &load_library_w_orig);
	MH_EnableHook(LoadLibraryW);

	//chara_story = mdb::get_story_all(selected_uma_id);

}

void detach()
{
	MH_DisableHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}
