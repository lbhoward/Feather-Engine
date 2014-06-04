#include "GFX.h"

// Constructor
GFX::GFX(HWND hwnd)
{
	// Safely initialise DirectX Variables
	g_driverType = D3D_DRIVER_TYPE_NULL; 
	g_featureLevel = D3D_FEATURE_LEVEL_11_1; 
	g_pd3dDevice = nullptr; 
	g_pImmediateContext = nullptr; 
	g_pSwapChain = nullptr; 
	g_pRenderTargetView = nullptr;

	// Safely initialise DX11 Buffer Variables
	g_vertexBuffer = nullptr;

	InitialiseDevice(hwnd);

	// Save Vertices for Basic Triangle
	FVertex Triangle_Verts[] =
    {
        { 0.0f, 0.5f, 0.0f },
        { 0.45f, -0.5f, 0.0f },
        { -0.45f, -0.5f, 0.0f }
    };

	// Setup a Vertex Buffer for rendering the data
	D3D11_BUFFER_DESC bd;
	bd.ByteWidth = sizeof(FVertex) * ARRAYSIZE(Triangle_Verts);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	D3D11_SUBRESOURCE_DATA srd = {Triangle_Verts, 0, 0};
	// And finally create the Vertex Buffer from the above description
	g_pd3dDevice->CreateBuffer(&bd, &srd, &g_vertexBuffer);

	InitialisePipeline();
}

// Deconstructor
GFX::~GFX()
{
}

// Device Accessor
ID3D11DeviceContext* GFX::Device()
{
	return g_pImmediateContext;
}

// Clear the Renderer
void GFX::ClearScene(FColour RGBA)
{
	g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, nullptr);
	g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, RGBA.Colour);

	return;
}

// Draw at V-Sync rate
void GFX::Draw()
{
	DrawTriangle();

	g_pSwapChain->Present(1,0);
}

// Draw a Triangle!
void GFX::DrawTriangle()
{	
	// Set the Vertex Buffer
	UINT stride = sizeof(FVertex);
	UINT offset = 0;
	g_pImmediateContext->IASetVertexBuffers(0, 1, &g_vertexBuffer, &stride, &offset);
	// And the Primitive Topology
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// And Finally - draw!
	g_pImmediateContext->Draw(3, 0);
}

// Set up the basic Rendering Pipeline (Shaders etc)
void GFX::InitialisePipeline()
{
	// Load the Shader Files
	CSORead VSFile = LoadShaderFile("BasicVertexShader.cso");
	CSORead PSFile = LoadShaderFile("BasicPixelShader.cso");

	// And Create the Shader Objects
	g_pd3dDevice->CreateVertexShader(VSFile.Data, VSFile.Length, nullptr, &g_vertexShader);
	g_pd3dDevice->CreatePixelShader(PSFile.Data, PSFile.Length, nullptr, &g_pixelShader);

	// Set these as our Active Shaders
	g_pImmediateContext->VSSetShader(g_vertexShader, nullptr, 0);
	g_pImmediateContext->PSSetShader(g_pixelShader, nullptr, 0);

	// Define the input layout for our Shaders
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0}//,
		//{"COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	// Create and set the Input Layout
	g_pd3dDevice->CreateInputLayout(ied, ARRAYSIZE(ied), VSFile.Data, VSFile.Length, &g_inputLayout);
	g_pImmediateContext->IASetInputLayout(g_inputLayout);
}

// Set up the D3D11 Device
HRESULT GFX::InitialiseDevice(HWND hwnd)
{
	HRESULT hr = S_OK; 
 
    RECT rc; 
    GetClientRect( hwnd, &rc ); 
    UINT width = rc.right - rc.left; 
    UINT height = rc.bottom - rc.top; 

	// Safely Request all possible Feature Levels
	const D3D_FEATURE_LEVEL lvl[] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0,
										D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0,
										D3D_FEATURE_LEVEL_9_3, D3D_FEATURE_LEVEL_9_2, D3D_FEATURE_LEVEL_9_1 };
 
    UINT createDeviceFlags = 0; 
#ifdef _DEBUG 
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG; 
#endif 
 
 
    DXGI_SWAP_CHAIN_DESC sd; 
    ZeroMemory( &sd, sizeof( sd ) ); 
    sd.BufferCount = 1; 
    sd.BufferDesc.Width = width; 
    sd.BufferDesc.Height = height; 
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; 
    sd.BufferDesc.RefreshRate.Numerator = 60; 
    sd.BufferDesc.RefreshRate.Denominator = 1; 
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; 
    sd.OutputWindow = hwnd; 
    sd.SampleDesc.Count = 1; 
    sd.SampleDesc.Quality = 0; 
    sd.Windowed = TRUE; 

	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, lvl, _countof(lvl), 
					       D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, NULL, &g_pImmediateContext);

	//hr = D3D11CreateDeviceAndSwapChain( nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, lvl, _countof(lvl),
										//D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext );
	if ( hr == E_INVALIDARG )
	{
		//hr = D3D11CreateDeviceAndSwapChain( nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, &lvl[1], _countof(lvl) - 1,
											//D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext );
	}

    if( FAILED( hr ) ) 
        return hr;
 
    // Create a render target view 
    ID3D11Texture2D* pBackBuffer = nullptr; 
    hr = g_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast<void**>( &pBackBuffer ) ); 
    if( FAILED( hr ) ) 
        return hr; 
 
    hr = g_pd3dDevice->CreateRenderTargetView( pBackBuffer, nullptr, &g_pRenderTargetView ); 
    pBackBuffer->Release(); 
    if( FAILED( hr ) ) 
        return hr; 
 
    g_pImmediateContext->OMSetRenderTargets( 1, &g_pRenderTargetView, nullptr ); 
 
    // Setup the viewport 
    D3D11_VIEWPORT vp; 
    vp.Width = (FLOAT)width; 
    vp.Height = (FLOAT)height; 
    vp.MinDepth = 0.0f; 
    vp.MaxDepth = 1.0f; 
    vp.TopLeftX = 0; 
    vp.TopLeftY = 0; 
    g_pImmediateContext->RSSetViewports( 1, &vp ); 
 
    return S_OK; 
}