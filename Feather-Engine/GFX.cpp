#include "GFX.h"

FVertex Triangle_Verts[] =
	{
		{ 0.0f, 0.5f, 0.0f, FColour(1.0f, 0.0f, 0.0f, 1.0f)},
        { 0.45f, -0.5f, 0.0f, FColour(0.0f, 1.0f, 0.0f, 1.0f)},
        { -0.45f, -0.5f, 0.0f, FColour(0.0f, 0.0f, 1.0f, 1.0f)}
    };

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
	InitialiseShaders();
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
	//DrawTriangle();
	DrawTriangle(0.5, 0, 0, 1);
	DrawTriangle(0, 0.5, 0, 1);
	DrawTriangle(0, -0.5, 0, 1);
	DrawTriangle(-0.5, 0, 0, 1);

	g_pSwapChain->Present(0,0);
}

// Adds vertices to VBuffer and draws a triangle
void GFX::DrawTriangle(float x, float y, float z, float scale)
{
	// So first we should define the vertices based on input
	FVertex newTriangle[] =
	{
		{x, y+0.1f, z, FColour(0.0f, 0.0f, 0.0f, 1.0f)}, // Top
		{x+0.1f, y-0.1f, z, FColour(0.0f, 0.0f, 0.0f, 1.0f)},		// Right
		{x-0.1f, y-0.1f, z, FColour(0.0f, 0.0f, 0.0f, 1.0f)}		// Left
	};

	// Now we need to map the VBuffer, copy the VData to the VBuffer, and unmap the VBuffer
	D3D11_MAPPED_SUBRESOURCE ms;
	g_pImmediateContext->Map(g_vertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, newTriangle, sizeof(newTriangle));
	g_pImmediateContext->Unmap(g_vertexBuffer, NULL);

	// Now we make sure the GPU knows exactly how we defined the Layout of our Vertex information
	D3D11_INPUT_ELEMENT_DESC ied[] = 
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	g_pd3dDevice->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &g_inputLayout);
	g_pImmediateContext->IASetInputLayout(g_inputLayout); // Finally load this layout into the device context.

	// Set the Vertex Buffer
	UINT stride = sizeof(FVertex);
	UINT offset = 0;
	g_pImmediateContext->IASetVertexBuffers(0, 1, &g_vertexBuffer, &stride, &offset);
	// And the Primitive Topology
	g_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// And Finally - draw!
	g_pImmediateContext->Draw(3, 0);
}

// Draw a Triangle!
void GFX::DrawTriangle()
{	
	// Set the Vertex Buffer
	UINT stride = sizeof(FVertex);
	UINT offset = 0;
	g_pImmediateContext->IASetVertexBuffers(0, 1, &g_vertexBuffer, &stride, &offset);
	// And the Primitive Topology
	g_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// And Finally - draw!
	g_pImmediateContext->Draw(3, 0);
}

//	Sets the basic VShader and PShader and assigns to ImmContext
void GFX::InitialiseShaders()
{
	//D3DX11CompileFromFile(); // Deprecated
	D3DCompileFromFile(L"shaders.shader", 0, 0, "VShader", "vs_4_0", 0, 0, &VS, 0);
	D3DCompileFromFile(L"shaders.shader", 0, 0, "PShader", "ps_4_0", 0, 0, &PS, 0);

	// Go ahead and create the Shader Objects from the blob objects
	g_pd3dDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &g_VS); // These are both stored in the classes pre-defined pointers
	g_pd3dDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &g_PS);

	// And now set these shaders to be our Active Shaders
	g_pImmediateContext->VSSetShader(g_VS, 0, 0);
	g_pImmediateContext->PSSetShader(g_PS, 0, 0);
}

// Set up the basic Rendering Pipeline (Shaders etc)
void GFX::InitialisePipeline()
{
	// Setup a Vertex Buffer for rendering the data
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC; // Allows write access by CPU and GPU
	bd.ByteWidth = sizeof(FVertex) * ARRAYSIZE(Triangle_Verts);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // Allow the CPU to write to this buffer

	g_pd3dDevice->CreateBuffer(&bd, NULL, &g_vertexBuffer);// Finally create the Vertex Buffer

	//// Now we need to map the VBuffer, copy the VData to the VBuffer, and unmap the VBuffer
	//D3D11_MAPPED_SUBRESOURCE ms;
	//g_pImmediateContext->Map(g_vertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	//memcpy(ms.pData, Triangle_Verts, sizeof(Triangle_Verts));
	//g_pImmediateContext->Unmap(g_vertexBuffer, NULL);

	//// Now we make sure the GPU knows exactly how we defined the Layout of our Vertex information
	//D3D11_INPUT_ELEMENT_DESC ied[] = 
	//{
	//	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	//	{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	//};
	//g_pd3dDevice->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &g_inputLayout);
	//g_pImmediateContext->IASetInputLayout(g_inputLayout); // Finally load this layout into the device context.
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

// Release all used COM objects
void GFX::CleanD3D()
{
	g_pSwapChain->SetFullscreenState(FALSE, NULL);

	g_VS->Release();
	g_PS->Release();

	g_pSwapChain->Release();
	//backbuffer->Release();
	g_pd3dDevice->Release();
	g_pImmediateContext->Release();
}