#include "common_utils.h"

// 解析 keyword_arraydata
std::vector<KeywordEntry> ParseKeywords(const DynamicArrayData& arr)
{
	std::vector<KeywordEntry> result;

	uint8_t* base = reinterpret_cast<uint8_t*>(arr.data);

	for (int i = 0; i < arr.m_size; i++)
	{
		uint8_t* entry = base + i * 0x30;

		// 读取标记
		uint32_t flag = *reinterpret_cast<uint32_t*>(entry + 0x20);

		std::string keyword;

		if (flag == 1)  // 内嵌字符串
		{
			char buf[0x19] = { 0 }; // 0x18 空间 + 结尾符
			memcpy(buf, entry, 0x18);

			// 手动截断（去掉尾部的 00）
			buf[0x18] = '\0';
			keyword = std::string(buf);
		}
		else if (flag == 0) // 指针方式
		{
			const char* strPtr = *reinterpret_cast<const char**>(entry);
			int len = *reinterpret_cast<int*>(entry + 0x10);

			if (strPtr && len > 0)
				keyword = std::string(strPtr, len);
			else
				keyword = "";
		}
		else
		{
			keyword = "[UnknownFlag]";
		}

		// 读取编号
		int id = *reinterpret_cast<int*>(entry + 0x28);

		result.push_back({ keyword, id });
	}

	return result;
}