#include <iostream>
#include <fstream>
#include <optional>
#include <windows.h>
#include <vector>
#include <string>
#include <unordered_map>

bool read_dos_header(std::ifstream& file, IMAGE_DOS_HEADER& dos) {
    file.seekg(0);
    file.read(reinterpret_cast<char*>(&dos), sizeof(dos));
    return dos.e_magic == IMAGE_DOS_SIGNATURE;
}

// 判断 PE 是 32 位还是 64 位
enum class PEArch { PE32, PE64 };

std::optional<PEArch> get_pe_arch(std::ifstream& file, const IMAGE_DOS_HEADER& dos) {
    file.seekg(dos.e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER)); // PE Signature + FileHeader
    WORD magic = 0;
    file.read(reinterpret_cast<char*>(&magic), sizeof(magic));
    if (magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC) return PEArch::PE32;
    if (magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC) return PEArch::PE64;
    return std::nullopt;
}

// 通用 Section Header 结构（只取必要字段）
struct Section {
    DWORD virtual_address;
    DWORD virtual_size;
    DWORD raw_pointer;
};

// 读取所有 Section Headers
std::vector<Section> read_sections(std::ifstream& file, const IMAGE_DOS_HEADER& dos, bool is64) {
    std::vector<Section> result;

    DWORD nt_header_offset = dos.e_lfanew;
    file.seekg(nt_header_offset + 4); // Skip PE Signature

    IMAGE_FILE_HEADER file_header;
    file.read(reinterpret_cast<char*>(&file_header), sizeof(file_header));

    WORD number_of_sections = file_header.NumberOfSections;
    WORD size_of_optional_header = file_header.SizeOfOptionalHeader;

    // 跳过 Optional Header
    file.seekg(size_of_optional_header, std::ios::cur);

    for (int i = 0; i < number_of_sections; ++i) {
        IMAGE_SECTION_HEADER section = {};
        file.read(reinterpret_cast<char*>(&section), sizeof(section));

        result.push_back({
            section.VirtualAddress,
            section.Misc.VirtualSize,
            section.PointerToRawData
            });
    }

    return result;
}

// 通用 RVA -> File Offset 函数
std::optional<uint32_t> rva_to_file_offset(uint32_t rva, const std::vector<Section>& sections) {
    for (const auto& sec : sections) {
        if (rva >= sec.virtual_address && rva < sec.virtual_address + sec.virtual_size) {
            return rva - sec.virtual_address + sec.raw_pointer;
        }
    }
    return std::nullopt;
}

// 从文件偏移读取 null 结尾字符串
std::string read_c_string(std::ifstream& file, uint32_t file_offset) {
    file.seekg(file_offset);
    std::string result;
    char ch;
    while (file.get(ch) && ch != '\0') {
        result += ch;
    }
    return result;
}

std::unordered_map<std::string, std::string> resolve_il2cpp()
{
    std::ifstream file(".\\UnityPlayer.dll", std::ios::binary);
    if (!file) {
        std::cerr << "File open failed.\n";
        return {};
    }

    IMAGE_DOS_HEADER dos = {};
    if (!read_dos_header(file, dos)) {
        std::cerr << "Invalid DOS header.\n";
        return {};
    }

    auto arch_opt = get_pe_arch(file, dos);
    if (!arch_opt) {
        std::cerr << "Cannot determine PE architecture\n";
        return {};
    }

    bool is64 = *arch_opt == PEArch::PE64;
    std::cout << "Architecture: " << (is64 ? "64-bit" : "32-bit") << "\n";

    auto sections = read_sections(file, dos, is64);

    //关于如何知道RVA具体的位置
    //反编译UnityPlayer.dll知道其Imagebase   : 180000000
    //找到其LoadIl2cpp函数，参考https://katyscode.wordpress.com/2021/02/23/il2cpp-finding-obfuscated-global-metadata/
    //比如说目前在Image   : 180783480
    //LoadIl2cpp会按顺序加载il2cpp符号，由此可获得混淆后的il2cpp函数名
    //如何知道加载顺序？需要一个没有被混淆il2cpp符号的UntiyPlayer.dll参考一下

    const uint32_t START_RVA = 0x7834a2;

    std::vector<std::string> SYMBOL_LIST = {
        "il2cpp_init",
        "il2cpp_init_utf16",
        "il2cpp_shutdown",
        "il2cpp_set_config_dir",
        "il2cpp_set_data_dir",
        "il2cpp_set_temp_dir",
        "il2cpp_set_commandline_arguments",
        "il2cpp_set_commandline_arguments_utf16",
        "il2cpp_set_config_utf16",
        "il2cpp_set_config",
        "il2cpp_set_memory_callbacks",
        "il2cpp_get_corlib",
        "il2cpp_add_internal_call",
        "il2cpp_resolve_icall",
        "il2cpp_alloc",
        "il2cpp_free",
        "il2cpp_array_class_get",
        "il2cpp_array_length",
        "il2cpp_array_get_byte_length",
        "il2cpp_array_new",
        "il2cpp_array_new_specific",
        "il2cpp_array_new_full",
        "il2cpp_bounded_array_class_get",
        "il2cpp_array_element_size",
        "il2cpp_assembly_get_image",
        "il2cpp_class_for_each",
        "il2cpp_class_enum_basetype",
        "il2cpp_class_is_inited",
        "il2cpp_class_is_generic",
        "il2cpp_class_is_inflated",
        "il2cpp_class_is_assignable_from",
        "il2cpp_class_is_subclass_of",
        "il2cpp_class_has_parent",
        "il2cpp_class_from_il2cpp_type",
        "il2cpp_class_from_name",
        "il2cpp_class_from_system_type",
        "il2cpp_class_get_element_class",
        "il2cpp_class_get_events",
        "il2cpp_class_get_fields",
        "il2cpp_class_get_nested_types",
        "il2cpp_class_get_interfaces",
        "il2cpp_class_get_properties",
        "il2cpp_class_get_property_from_name",
        "il2cpp_class_get_field_from_name",
        "il2cpp_class_get_methods",
        "il2cpp_class_get_method_from_name",
        "il2cpp_class_get_name",
        "il2cpp_type_get_name_chunked",
        "il2cpp_class_get_namespace",
        "il2cpp_class_get_parent",
        "il2cpp_class_get_declaring_type",
        "il2cpp_class_instance_size",
        "il2cpp_class_num_fields",
        "il2cpp_class_is_valuetype",
        "il2cpp_class_value_size",
        "il2cpp_class_is_blittable",
        "il2cpp_class_get_flags",
        "il2cpp_class_is_abstract",
        "il2cpp_class_is_interface",
        "il2cpp_class_array_element_size",
        "il2cpp_class_from_type",
        "il2cpp_class_get_type",
        "il2cpp_class_get_type_token",
        "il2cpp_class_has_attribute",
        "il2cpp_class_has_references",
        "il2cpp_class_is_enum",
        "il2cpp_class_get_image",
        "il2cpp_class_get_assemblyname",
        "il2cpp_class_get_rank",
        "il2cpp_class_get_data_size",
        "il2cpp_class_get_static_field_data",
        "il2cpp_class_get_bitmap_size",
        "il2cpp_class_get_bitmap",
        "il2cpp_stats_dump_to_file",
        "il2cpp_stats_get_value",
        "il2cpp_domain_get",
        "il2cpp_domain_assembly_open",
        "il2cpp_domain_get_assemblies",
        "il2cpp_raise_exception",
        "il2cpp_exception_from_name_msg",
        "il2cpp_get_exception_argument_null",
        "il2cpp_format_exception",
        "il2cpp_format_stack_trace",
        "il2cpp_unhandled_exception",
        "il2cpp_native_stack_trace",
        "il2cpp_field_get_flags",
        "il2cpp_field_get_name",
        "il2cpp_field_get_parent",
        "il2cpp_field_get_offset",
        "il2cpp_field_get_type",
        "il2cpp_field_get_value",
        "il2cpp_field_get_value_object",
        "il2cpp_field_has_attribute",
        "il2cpp_field_set_value",
        "il2cpp_field_static_get_value",
        "il2cpp_field_static_set_value",
        "il2cpp_field_set_value_object",
        "il2cpp_field_is_literal",
        "il2cpp_gc_collect",
        "il2cpp_gc_collect_a_little",
        "il2cpp_gc_start_incremental_collection",
        "il2cpp_gc_disable",
        "il2cpp_gc_enable",
        "il2cpp_gc_is_disabled",
        "il2cpp_gc_set_mode",
        "il2cpp_gc_get_max_time_slice_ns",
        "il2cpp_gc_set_max_time_slice_ns",
        "il2cpp_gc_is_incremental",
        "il2cpp_gc_get_used_size",
        "il2cpp_gc_get_heap_size",
        "il2cpp_gc_wbarrier_set_field",
        "il2cpp_gc_has_strict_wbarriers",
        "il2cpp_gc_set_external_allocation_tracker",
        "il2cpp_gc_set_external_wbarrier_tracker",
        "il2cpp_gc_foreach_heap",
        "il2cpp_stop_gc_world",
        "il2cpp_start_gc_world",
        "il2cpp_gc_alloc_fixed",
        "il2cpp_gc_free_fixed",
        "il2cpp_gchandle_new",
        "il2cpp_gchandle_new_weakref",
        "il2cpp_gchandle_get_target",
        "il2cpp_gchandle_free",
        "il2cpp_gchandle_foreach_get_target",
        "il2cpp_object_header_size",
        "il2cpp_array_object_header_size",
        "il2cpp_offset_of_array_length_in_array_object_header",
        "il2cpp_offset_of_array_bounds_in_array_object_header",
        "il2cpp_allocation_granularity",
        "il2cpp_unity_liveness_allocate_struct",
        "il2cpp_unity_liveness_calculation_from_root",
        "il2cpp_unity_liveness_calculation_from_statics",
        "il2cpp_unity_liveness_finalize",
        "il2cpp_unity_liveness_free_struct",
        "il2cpp_method_get_return_type",
        "il2cpp_method_get_declaring_type",
        "il2cpp_method_get_name",
        "il2cpp_method_get_from_reflection",
        "il2cpp_method_get_object",
        "il2cpp_method_is_generic",
        "il2cpp_method_is_inflated",
        "il2cpp_method_is_instance",
        "il2cpp_method_get_param_count",
        "il2cpp_method_get_param",
        "il2cpp_method_get_class",
        "il2cpp_method_has_attribute",
        "il2cpp_method_get_flags",
        "il2cpp_method_get_token",
        "il2cpp_method_get_param_name",
        "il2cpp_property_get_flags",
        "il2cpp_property_get_get_method",
        "il2cpp_property_get_set_method",
        "il2cpp_property_get_name",
        "il2cpp_property_get_parent",
        "il2cpp_object_get_class",
        "il2cpp_object_get_size",
        "il2cpp_object_get_virtual_method",
        "il2cpp_object_new",
        "il2cpp_object_unbox",
        "il2cpp_value_box",
        "il2cpp_monitor_enter",
        "il2cpp_monitor_try_enter",
        "il2cpp_monitor_exit",
        "il2cpp_monitor_pulse",
        "il2cpp_monitor_pulse_all",
        "il2cpp_monitor_wait",
        "il2cpp_monitor_try_wait",
        "il2cpp_runtime_invoke",
        "il2cpp_runtime_invoke_convert_args",
        "il2cpp_runtime_class_init",
        "il2cpp_runtime_object_init",
        "il2cpp_runtime_object_init_exception",
        "il2cpp_runtime_unhandled_exception_policy_set",
        "il2cpp_string_length",
        "il2cpp_string_chars",
        "il2cpp_string_new",
        "il2cpp_string_new_len",
        "il2cpp_string_new_utf16",
        "il2cpp_string_new_wrapper",
        "il2cpp_string_intern",
        "il2cpp_string_is_interned",
        "il2cpp_thread_current",
        "il2cpp_thread_attach",
        "il2cpp_thread_detach",
        "il2cpp_thread_get_all_attached_threads",
        "il2cpp_is_vm_thread",
        "il2cpp_current_thread_walk_frame_stack",
        "il2cpp_thread_walk_frame_stack",
        "il2cpp_current_thread_get_top_frame",
        "il2cpp_thread_get_top_frame",
        "il2cpp_current_thread_get_frame_at",
        "il2cpp_thread_get_frame_at",
        "il2cpp_current_thread_get_stack_depth",
        "il2cpp_thread_get_stack_depth",
        "il2cpp_override_stack_backtrace",
        "il2cpp_type_get_object",
        "il2cpp_type_get_type",
        "il2cpp_type_get_class_or_element_class",
        "il2cpp_type_get_name",
        "il2cpp_type_is_byref",
        "il2cpp_type_get_attrs",
        "il2cpp_type_equals",
        "il2cpp_type_get_assembly_qualified_name",
        "il2cpp_type_get_reflection_name",
        "il2cpp_type_is_static",
        "il2cpp_type_is_pointer_type",
        "il2cpp_image_get_assembly",
        "il2cpp_image_get_name",
        "il2cpp_image_get_filename",
        "il2cpp_image_get_entry_point",
        "il2cpp_image_get_class_count",
        "il2cpp_image_get_class",
        "il2cpp_capture_memory_snapshot",
        "il2cpp_free_captured_memory_snapshot",
        "il2cpp_set_find_plugin_callback",
        "il2cpp_register_log_callback",
        "il2cpp_debugger_set_agent_options",
        "il2cpp_is_debugger_attached",
        "il2cpp_register_debugger_agent_transport",
        "il2cpp_debug_get_method_info",
        "il2cpp_unity_install_unitytls_interface",
        "il2cpp_custom_attrs_from_class",
        "il2cpp_custom_attrs_from_method",
        "il2cpp_custom_attrs_from_field",
        "il2cpp_custom_attrs_get_attr",
        "il2cpp_custom_attrs_has_attr",
        "il2cpp_custom_attrs_construct",
        "il2cpp_custom_attrs_free",
        "il2cpp_class_set_userdata",
        "il2cpp_class_get_userdata_offset",
        "il2cpp_set_default_thread_affinity",
        "il2cpp_unity_set_android_network_up_state_func"
    };

    std::unordered_map<std::string, std::string> symbol_map;

    uint32_t rva = START_RVA;
    for (size_t i = 0; i < SYMBOL_LIST.size(); ++i) {
        auto offset_opt = rva_to_file_offset(rva, sections);
        if (!offset_opt) {
            std::cerr << "rva 0x" << std::hex << rva << " not mapped\n";
            continue;
        }
        uint32_t offset = *offset_opt;

        // 从文件中读取4字节rip_offset
        file.seekg(offset);
        uint32_t rip_offset = 0;
        file.read(reinterpret_cast<char*>(&rip_offset), sizeof(rip_offset));

        // name_rva = rva + 4 + rip_offset
        uint32_t name_rva = rva + 4 + rip_offset;
        auto name_offset_opt = rva_to_file_offset(name_rva, sections);
        if (!name_offset_opt) {
            std::cerr << "name_rva 0x" << std::hex << name_rva << " not mapped\n";
            continue;
        }

        uint32_t name_offset = *name_offset_opt;
        std::string symbol_string = read_c_string(file, name_offset);
        std::cout << /*"String at RVA 0x" << std::hex << rva << ": " <<*/ SYMBOL_LIST[i] << ": " << symbol_string << "\n";
        symbol_map[SYMBOL_LIST[i]] = symbol_string;

        // 更新 rva（首项偏移为 0x28，之后是 0x26）
        rva += (i == 0) ? 0x28 : 0x26;
    }

    return symbol_map;

}