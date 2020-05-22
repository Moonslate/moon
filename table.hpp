#pragma once
#include <string>

#include "file.hpp"
#include "string.hpp"
#include "bit_conv.hpp"

//Not inlining in debug mode
#ifdef _DEBUG
#define _prefix 
#else
#define _prefix __forceinline
#endif

namespace Moon
{
	namespace Hacking
	{
		class Table
		{
			public:
				Table(const std::string& path)
				{					
					Open(path);
				}
				Table() = default;
				void Open(const std::string& path)
				{
					std::string s = File::ReadAllText(path);

					auto lines = String::ViewLines(s);					

					NewTable();

					for (const auto& line : lines)
					{
						unsigned char left = BitConverter::FromHexString(line.data());

						if (left != line[3])
						{
							m_Change = true;
							m_Table[left] = line[3];
						}
					}
				}
				_prefix void Input(std::string& str) const
				{
					Input((unsigned char*)str.data(), str.size());
				}
				_prefix void Input(unsigned char* str, size_t lenght) const
				{
					if (!m_Change)
						return;

					for (size_t i = 0; i < lenght; ++i)
					{
						str[i] = m_Table[str[i]];
					}
				}
				_prefix void Output(std::string& str) const
				{
					if (!m_Change)
						return;

					for (size_t i = 0; i < str.size(); ++i)
					{
						size_t pos = m_Table.find(str[i]);

						if (pos != std::string::npos)
						{
							str[i] = pos;
						}
					}
				}
				inline char& operator[](const unsigned char& pos)
				{
					return m_Table[pos];
				}

				void NewTable()
				{
					m_Table.clear();
					m_Table.reserve(256);

					for (unsigned char i = 0; i < 0xff; ++i)
					{
						m_Table.push_back(i);
					}
				}
				_prefix unsigned char Find(const char& c, const size_t& start = 0) const
				{
					return m_Table.find(c, start);
				}
				_prefix void ReplaceAll(const char& c, const char& other)
				{
					if (c != other)
					{
						m_Change = true;

						unsigned char pos = Find(c);

						while (pos != 0xff)
						{
							m_Table[pos] = other;
							pos = Find(c, pos + 1);
						}
					}
				}

		private:
			std::string m_Table;
			bool m_Change = false;
		};
	};
};