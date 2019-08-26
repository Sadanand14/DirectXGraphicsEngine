#include "Game.h"
#include "Vertex.h"
#include "d3d11.h"
#include "Mesh.h"
#include <vector>
#include "Camera.h"
#include <iostream>
#include "Lights.h"
#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"
// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore(
		hInstance,		// The application's handle
		"DirectX Game",	   	// Text for the window's title bar
		1280,			// Width of the window's client area
		720,			// Height of the window's client area
		true)			// Show extra stats (fps) in title bar?
{
	// Initialize fields
	camera = new Camera((float)width, (float)height);
	vertexBuffer = 0;
	indexBuffer = 0;
	vertexShader = 0;
	pixelShader = 0;
	
#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.\n");
#endif

}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	entityList.clear();
	// Release any (and all!) DirectX objects
	// we've made in the Game class
	if (vertexBuffer) { vertexBuffer->Release(); }
	if (indexBuffer) { indexBuffer->Release(); }
	if(srv1)srv1->Release();
	if(srv2)srv2->Release();
	if (skySRV)skySRV->Release();
	//if (SkyDepthStencil)SkyDepthStencil->Release();
	//if (skyRaster)skyRaster->Release();
	if (skyboxPS) delete skyboxPS;
	if (skyboxVS) delete skyboxVS;

	shaderSampler->Release();
	
	if (mesh1 != nullptr)
		delete mesh1;
	if (mesh2 != nullptr)
		delete mesh2;
	if (mesh3 != nullptr)
		delete mesh3;
	if (mesh4 != nullptr)
		delete mesh4;
	if (material1 != nullptr)
		delete material1;
	if (material2 != nullptr)
		delete material2;
	if (material3 != nullptr)
		delete material3;
	// Delete our simple shader objects, which
	// will clean up their own internal DirectX stuff
	delete vertexShader;
	delete pixelShader;
	delete camera;
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{

	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	LoadShaders();
	CreateMatrices();
	Setmodels();
	SetLights();
	CreateBasicGeometry();
	CreateDDSTextureFromFile(device, L"Textures/Test.dds",0,&skySRV);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files using
// my SimpleShader wrapper for DirectX shader manipulation.
// - SimpleShader provides helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void Game::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, context);
	vertexShader->LoadShaderFile(L"VertexShader.cso");

	pixelShader = new SimplePixelShader(device, context);
	pixelShader->LoadShaderFile(L"PixelShader.cso");

	skyboxVS = new SimpleVertexShader(device, context);
	skyboxVS->LoadShaderFile(L"SkyBoxVS.cso");

	skyboxPS = new SimplePixelShader(device, context);
	skyboxPS->LoadShaderFile(L"SkyBoxPS.cso");
}



// --------------------------------------------------------
// Initializes the matrices necessary to represent our geometry's 
// transformations and our 3D camera
// --------------------------------------------------------
void Game::CreateMatrices()
{

	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W)); // Transpose for HLSL!


	XMVECTOR pos = XMVectorSet(0, 0, -5, 0);
	XMVECTOR dir = XMVectorSet(0, 0, 1, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	XMMATRIX V = XMMatrixLookToLH(
		pos,     // The position of the "camera"
		dir,     // Direction the camera is looking
		up);     // "Up" direction in 3D space (prevents roll)
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!


	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		(float)width / height,		// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}



void Game::SetLights() 
{
	light1.AmbientColor = XMFLOAT4(0.0f, 0.2f, 0.2f, 1.0f);
	light1.DiffuseColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	light1.Direction = XMFLOAT3(0.0f, 0.0f, 1.0f);

	light2.Position = XMFLOAT4(0,5,0, 0);
	light2.ambientColor = XMFLOAT4(0.0f, 0.2f, 0.2f,0);
	light2.sourceColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 0);
	
}

void Game::Setmodels() 
{
	

	//Generating a texture resource view from the loaded texture
	samplerStruct = {};

	samplerStruct.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerStruct.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerStruct.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	samplerStruct.Filter = D3D11_FILTER_ANISOTROPIC;

	samplerStruct.MaxLOD = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

	device->CreateSamplerState(&samplerStruct, &shaderSampler);
 	
	CreateWICTextureFromFile(device, context, L"Textures/Test.dds", 0, &srv1);// colour map
	CreateWICTextureFromFile(device, context, L"Textures/Hex_N.jpg", 0, &srv2);// normal map

	material1 = new DefaultMaterials(vertexShader, pixelShader, srv1, srv2, shaderSampler);
}

// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------

void Game::CreateBasicGeometry()
{
	XMMATRIX trans = XMMatrixTranslation(-2.0f, 0.0f, 0.0f);
	XMMATRIX rot = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	XMMATRIX scale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	
	/*mesh4 = new Mesh("Models/helix.obj", device);
	entityList.push_back(Entity(trans, rot, scale, mesh4, material1));

	trans = XMMatrixTranslation(2.0f, 0.0f, 0.0f);
	rot = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	scale = XMMatrixScaling(0.5f, 0.5f, 0.5f);*/

	mesh3 = new Mesh("Models/torus.obj", device);
	entityList.push_back(Entity(trans, rot, scale, mesh3, material1));

	rot = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	scale = XMMatrixScaling(1.5f, 1.5f, 1.5f);
	trans = XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	mesh2 = new Mesh("Models/sphere.obj", device);
	entityList.push_back(Entity(trans, rot, scale, mesh2, material1));

	mesh3 = new Mesh("Models/cube.obj", device);
	entityList.push_back(Entity(trans, rot, scale, mesh3, material1));
}


// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	// Update our projection matrix since the window size changed
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,	// Field of View Angle
		(float)width / height,	// Aspect ratio
		0.1f,				  	// Near clip plane distance
		100.0f);			  	// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	XMMATRIX rot = XMMatrixRotationRollPitchYaw(0.0f,totalTime, totalTime);
	entityList[0].SetRot(rot);

	rot = XMMatrixRotationRollPitchYaw(totalTime, 0.0f, totalTime);
	entityList[1].SetRot(rot);
	
	camera->Update(deltaTime);
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();
	
	if (GetAsyncKeyState('W')) 
	{
		camera->MoveForward();
	}
	if (GetAsyncKeyState('S'))
	{
		camera->MoveBackward();
	}
	if (GetAsyncKeyState('A')) 
	{
		camera->MoveLeft();
	}
	if (GetAsyncKeyState('D'))
	{
		camera->MoveRight();
	}
	if (GetAsyncKeyState(VK_SPACE))
	{
		camera->MoveUpward();
	}
	if (GetAsyncKeyState('X'))
	{
		camera->MoveDownward();
	}
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);
	


	//Looped all the sequences for loading the worldmatrix as well as loading the index and vertex buffers to the 
	//GPU using a vector of entities.

	for (int i = 0; i < 2; i++) {
		XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(entityList[i].GetWM()));

		//Passing in all the VertexShader Data
		SimpleVertexShader* vPointer= entityList[i].GetMaterial()->GetVrtxptr();
		vPointer->SetMatrix4x4("world", worldMatrix);
		vPointer->SetMatrix4x4("view", camera->GetView());
		vPointer->SetMatrix4x4("projection", camera->GetProjection());
		vPointer->CopyAllBufferData();
		vPointer->SetShader();

		//Passing data into the pixel Shader
		SimplePixelShader* pPointer = entityList[i].GetMaterial()->GetPxlptr();
		ID3D11SamplerState* sampler = entityList[i].GetMaterial()->GetSamplerState();
		ID3D11ShaderResourceView* srvColor = entityList[i].GetMaterial()->GetSRVColor();
		ID3D11ShaderResourceView* srvNormal = entityList[i].GetMaterial()->GetSRVNormal();
		
		pPointer->SetData("directionalLight", &light1, sizeof(DirectionalLight));
		pPointer->SetData("pointLight", &light2, sizeof(PointLight));
		pPointer->SetFloat4("cameraPosition", camera->GetPos());
		pPointer->SetSamplerState("Sampler", sampler);
		pPointer->SetShaderResourceView("Texture1", srvColor);
		pPointer->SetShaderResourceView("NormalMap1", srvNormal);
		pPointer->CopyAllBufferData();
		pPointer->SetShader();

		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		ID3D11Buffer *v1Buffer = entityList[i].GetMesh()->GetVertexBuffer();
		ID3D11Buffer *i1Buffer = entityList[i].GetMesh()->GetIndexBuffer();
		context->IASetVertexBuffers(0, 1, &v1Buffer, &stride, &offset);
		context->IASetIndexBuffer(i1Buffer, DXGI_FORMAT_R32_UINT, 0);
		int indicesCount1 = entityList[i].GetMesh()->GetIndexCount();
		context->DrawIndexed(indicesCount1, 0, 0);
	}

	DrawSky();

	swapChain->Present(0, 0);
}

void Game::DrawSky() 
{
	skyboxVS->SetShader();
	skyboxVS->SetMatrix4x4("view", camera->GetView());
	skyboxVS->SetMatrix4x4("projection", camera->GetProjection());
	skyboxVS->CopyAllBufferData();

	skyboxPS->SetShader();
	skyboxPS->SetShaderResourceView("tex", material1->GetSRVColor());
	skyboxPS->SetSamplerState("Sampler", shaderSampler);
	skyboxPS->CopyAllBufferData();

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	ID3D11Buffer *v1Buffer = entityList[2].GetMesh()->GetVertexBuffer();
	ID3D11Buffer *i1Buffer = entityList[2].GetMesh()->GetIndexBuffer();
	context->IASetVertexBuffers(0, 1, &v1Buffer, &stride, &offset);
	context->IASetIndexBuffer(i1Buffer, DXGI_FORMAT_R32_UINT, 0);
	int indicesCount1 = entityList[2].GetMesh()->GetIndexCount();
	context->DrawIndexed(indicesCount1, 0, 0);
}

#pragma region Mouse Input


void Game::OnMouseDown(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...
	
	
	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

	SetCapture(hWnd);
}


void Game::OnMouseUp(WPARAM buttonState, int x, int y)
{

	ReleaseCapture();
}


void Game::OnMouseMove(WPARAM buttonState, int x, int y)
{

	if (buttonState & 0x0001)
	{
		camera->MouseLook((x- prevMousePos.x),(y- prevMousePos.y));
	}

	prevMousePos.x = x;
	prevMousePos.y = y;
}

void Game::OnMouseWheel(float wheelDelta, int x, int y)
{
	// Add any custom code here...
}
#pragma endregion