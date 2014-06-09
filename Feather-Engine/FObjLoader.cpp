#include "FObjLoader.h"

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

FObjLoader::FObjLoader(char* filePath)
{
	ifstream OBJFile("C:\\Users\\Lawrence\\workspace\\Feather-Engine\\Debug\\sphere.obj");

	while (!OBJFile.eof())
	{
		string line;
		getline(OBJFile,line);

		vector<string> currentLine = split(line,' ');
		if (currentLine.size() == 0)
			continue;

		// Check if this is a Vertex
		if (currentLine[0] == "v")
		{
			FVertex thisVertex = {atof(currentLine[1].c_str()), atof(currentLine[2].c_str()), atof(currentLine[3].c_str()), FColour(0.0f, 0.0f, 0.0f, 1.0f)};
			Vertices.push_back(thisVertex);
		}

		// Check if this is an Indice List (Face)
		if (currentLine[0] == "f")
		{
			currentLine[0].erase();

			for (int i = 1; i <= 3; i++)
			{
				Indices.push_back(atoi(currentLine[i].c_str()));
			}
		}
	}
};

FObjLoader::FObjLoader()
{
};

FObjLoader::~FObjLoader()
{
};

FVertex* FObjLoader::GetVertices()
{
	FVertex* result = (FVertex*)malloc(sizeof(FVertex)*Vertices.size());
	memcpy(result, &Vertices[0], Vertices.size() * sizeof(FVertex));
	return result;
};

unsigned long* FObjLoader::GetIndices()
{
	return &Indices[0];
};

int FObjLoader::GetVerticesLength()
{
	return Vertices.size();
};

int FObjLoader::GetIndicesLength()
{
	return Indices.size();
}