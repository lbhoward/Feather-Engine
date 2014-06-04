#include "ShaderLoader.h"

using namespace System;

CSORead LoadShaderFile(std::string File)
{
    CSORead FileData;

    std::ifstream ifs(File, std::ifstream::in | std::ifstream::binary);  
	ifs.seekg( 0, std::ios::end );  
	size_t size = ifs.tellg();  
	char* buf = new char[size];
	ifs.seekg(0, std::ios::beg);  
	ifs.read( &buf[0], size);  
	ifs.close();

	FileData.Data = buf;
	FileData.Length = size;

    return FileData;
}