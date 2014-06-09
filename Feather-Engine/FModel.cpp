#include "FModel.h"

FModel::FModel()
{
}

FModel::FModel(ID3D11Device* device)
{
	InitVertexBuffer(device);
	InitShaders(device);
}

FModel::~FModel()
{
}

// Prepares the Vertex Buffer for this Model and binds to the Immediate Context
bool FModel::InitVertexBuffer(ID3D11Device* device)
{
	FVertex* vertices;
	unsigned long* indices;

	FObjLoader thisModelFile("hardcode for now");
	vertices = thisModelFile.GetVertices();
	numOfVertices = thisModelFile.GetVerticesLength();

	indices = thisModelFile.GetIndices();
	numOfIndices = thisModelFile.GetIndicesLength();

	// Setup a Vertex Buffer for rendering the data
	D3D11_BUFFER_DESC vertexBD, indexBD;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	ZeroMemory(&vertexBD, sizeof(vertexBD));
	ZeroMemory(&indexBD, sizeof(indexBD));

	vertexBD.Usage = D3D11_USAGE_DYNAMIC; // Allows write access by CPU and GPU
	vertexBD.ByteWidth = sizeof(FVertex) * numOfVertices; // Calculate needed size of VBuffer
	vertexBD.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBD.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // Allow the CPU to write to this buffer

	//FVertex vertices[] =
	//{
	//	{0, 0.5, 0, FColour(0.0f, 0.0f, 0.0f, 1.0f)}, // Top
	//	{0.5, -0.5, 0, FColour(0.0f, 0.0f, 0.0f, 1.0f)},		// Right
	//	{-0.5, -0.5, 0, FColour(0.0f, 0.0f, 0.0f, 1.0f)}		// Left
	//};

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	device->CreateBuffer(&vertexBD, &vertexData, &vertexBuffer);// Finally create the Vertex Buffer

	indexBD.Usage = D3D11_USAGE_DYNAMIC; // Allows write access by CPU and GPU
	indexBD.ByteWidth = sizeof(unsigned long) * numOfIndices; // Calculate needed size of IBuffer
	indexBD.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	indexBD.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // Allow the CPU to write to this buffer

	//unsigned long indices[] = {0, 1, 2};

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	device->CreateBuffer(&indexBD, &indexData, &indexBuffer);// Finally create the Index Buffer

	return true;
}

bool FModel::InitShaders(ID3D11Device* device)
{
	//D3DX11CompileFromFile(); // Deprecated
	D3DCompileFromFile(L"shaders.shader", 0, 0, "VShader", "vs_4_0", 0, 0, &VS, 0);
	D3DCompileFromFile(L"shaders.shader", 0, 0, "PShader", "ps_4_0", 0, 0, &PS, 0);

	// Go ahead and create the Shader Objects from the blob objects
	device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &vertexShader); // These are both stored in the classes pre-defined pointers
	device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pixelShader);

	return true;
}

void FModel::Render(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	// Set the Device to render using this models shader
	deviceContext->VSSetShader(vertexShader, 0, 0);
	deviceContext->PSSetShader(pixelShader, 0, 0);

	// Now we make sure the GPU knows exactly how we defined the Layout of our Vertex information
	D3D11_INPUT_ELEMENT_DESC ied[] = 
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	device->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &inputLayout);
	deviceContext->IASetInputLayout(inputLayout); // Finally load this layout into the device context.

	// Set the Vertex Buffer
	UINT stride = sizeof(FVertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// And Finally - draw!
	deviceContext->DrawIndexed(numOfIndices, 0, 0);
}