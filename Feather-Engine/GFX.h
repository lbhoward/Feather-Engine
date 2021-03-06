#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3d11_1.h>

#include "ShaderLoader.h"

struct FColour
{
	FColour(float R, float G, float B, float A)
	{
		Colour[0] = R;
		Colour[1] = G;
		Colour[2] = B;
		Colour[3] = A;
	}

	float Colour[4];
};

struct FVertex
{
	float X,Y,Z;
};

class GFX
{
public:
	GFX(HWND hwnd);
	~GFX();

	ID3D11DeviceContext* Device();

	// Drawing Functions
	void ClearScene(FColour RGBA);
	void Draw();

	void DrawTriangle();

private:
	HRESULT InitialiseDevice(HWND hwnd);
	void InitialisePipeline();

	//DirectX
	D3D_DRIVER_TYPE         g_driverType; 
	D3D_FEATURE_LEVEL       g_featureLevel; 
	ID3D11Device*           g_pd3dDevice; 
	ID3D11DeviceContext*    g_pImmediateContext; 
	IDXGISwapChain*         g_pSwapChain; 
	ID3D11RenderTargetView* g_pRenderTargetView;

	//DX11 Buffers
	ID3D11Buffer* g_vertexBuffer;

	//DX11 Shaders
	ID3D11VertexShader* g_vertexShader;
	ID3D11PixelShader* g_pixelShader;
	ID3D11InputLayout* g_inputLayout;
};