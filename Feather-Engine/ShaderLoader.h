#include <fstream>
#include <memory> // for std::unique_ptr
#include <stdint.h> // for uint8_t

using namespace System;

struct CSORead
{
	char* Data;
	size_t Length;
};

CSORead LoadShaderFile(std::string File);