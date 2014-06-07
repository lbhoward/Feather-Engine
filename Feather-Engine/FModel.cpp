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
	// Setup a Vertex Buffer for rendering the data
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC; // Allows write access by CPU and GPU
	bd.ByteWidth = sizeof(FVertex) * 3;//numOfVertices; // Calculate needed size of VBuffer
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // Allow the CPU to write to this buffer

	device->CreateBuffer(&bd, NULL, &vertexBuffer);// Finally create the Vertex Buffer

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

	// And now set these shaders to be our Active Shaders
	// Lawrence Note: So shaders are set before draw is called to take effect?
	//g_pImmediateContext->VSSetShader(VS, 0, 0);
	//g_pImmediateContext->PSSetShader(PS, 0, 0);

	return true;
}

void FModel::Render(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	// Set the Device to render using this models shader
	deviceContext->VSSetShader(vertexShader, 0, 0);
	deviceContext->PSSetShader(pixelShader, 0, 0);

	float x = 0, y = 0, z = 0, scale = 0.2f;
	// So first we should define the vertices based on input
	FVertex newTriangle[] =
	{
		{x, y+scale, z, FColour(0.0f, 0.0f, 0.0f, 1.0f)}, // Top
		{x+scale, y-scale, z, FColour(0.0f, 0.0f, 0.0f, 1.0f)},		// Right
		{x-scale, y-scale, z, FColour(0.0f, 0.0f, 0.0f, 1.0f)}		// Left
	};

	// Now we need to map the VBuffer, copy the VData to the VBuffer, and unmap the VBuffer
	D3D11_MAPPED_SUBRESOURCE ms;
	deviceContext->Map(vertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, newTriangle, sizeof(newTriangle));
	deviceContext->Unmap(vertexBuffer, NULL);

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
	// And the Primitive Topology
	deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// And Finally - draw!
	deviceContext->Draw(3, 0);
}