#pragma once

#include <streambuf>

class MemoryBuffer : public std::streambuf
{
public:
	MemoryBuffer(unsigned char* data, size_t size)
	{
		setg(
			reinterpret_cast<char*>(data),
			reinterpret_cast<char*>(data),
			reinterpret_cast<char*>(data + size)
		);
	}
};