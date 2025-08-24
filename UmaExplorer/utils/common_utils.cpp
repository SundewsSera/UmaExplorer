#include "common_utils.h"

// ���� keyword_arraydata
std::vector<KeywordEntry> ParseKeywords(const DynamicArrayData& arr)
{
	std::vector<KeywordEntry> result;

	uint8_t* base = reinterpret_cast<uint8_t*>(arr.data);

	for (int i = 0; i < arr.m_size; i++)
	{
		uint8_t* entry = base + i * 0x30;

		// ��ȡ���
		uint32_t flag = *reinterpret_cast<uint32_t*>(entry + 0x20);

		std::string keyword;

		if (flag == 1)  // ��Ƕ�ַ���
		{
			char buf[0x19] = { 0 }; // 0x18 �ռ� + ��β��
			memcpy(buf, entry, 0x18);

			// �ֶ��ضϣ�ȥ��β���� 00��
			buf[0x18] = '\0';
			keyword = std::string(buf);
		}
		else if (flag == 0) // ָ�뷽ʽ
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

		// ��ȡ���
		int id = *reinterpret_cast<int*>(entry + 0x28);

		result.push_back({ keyword, id });
	}

	return result;
}