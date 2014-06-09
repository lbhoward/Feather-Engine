#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>

#include "FStructs.h"
#include "FObjLoader.h"

class FModel
{
public:
	FModel();
	FModel(ID3D11Device* device);
	~FModel();

	void Render(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

private:
	bool InitVertexBuffer(ID3D11Device* device);
	bool InitShaders(ID3D11Device* device);

	int numOfVertices, numOfIndices;
	
	ID3D11Buffer *vertexBuffer, *indexBuffer; // Each model has it's own Vertex & Index Buffer
	ID3D11InputLayout* inputLayout; // And a define layout

	ID3D10Blob *VS, *PS;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
};