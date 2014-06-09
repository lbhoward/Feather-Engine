#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "FStructs.h"

using namespace std;

class FObjLoader
{
public:
	FObjLoader(char* filePath);
	FObjLoader();
	~FObjLoader();

	FVertex* GetVertices();
	int GetVerticesLength();

	unsigned long* GetIndices();
	int GetIndicesLength();

private:
	vector<FVertex> Vertices;
	vector<unsigned long> Indices;
};