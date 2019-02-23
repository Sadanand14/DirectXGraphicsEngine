#include "Game.h"
#include "Vertex.h"
#include "d3d11.h"
#include "Mesh.h"
#include <vector>
#include "Camera.h"
#include <iostream>
#include "Lights.h"
#include "WICTextureLoader.h"

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
	srv1->Release();
	srv2->Release();
	shaderSampler->Release();
	//delete mesh1, mesh2, mesh3, mesh4,entity1, entity2, entity3, entity4, entity5, &entityList;
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
	CreateBasicGeometry();

	//intitalizing the directional light structure defined in game.h
	light1.AmbientColor.x = 0.1f;
	light1.AmbientColor.y = 0.1f;
	light1.AmbientColor.z = 0.1f;
	light1.AmbientColor.w = 1.0f;

	light1.DiffuseColor.x = 1.0f;
	light1.DiffuseColor.y = 1.0f;
	light1.DiffuseColor.z = 1.0f;
	light1.DiffuseColor.w = 1.0f;

	light1.Direction.x = 1.0f;
	light1.Direction.y = -1.0f;
	light1.Direction.z = 0.0f;
	
	light2.AmbientColor.x = 0.1f;
	light2.AmbientColor.y = 0.1f;
	light2.AmbientColor.z = 0.1f;
	light2.AmbientColor.w = 1.0f;

	light2.DiffuseColor.x = 0.0f;
	light2.DiffuseColor.y = 0.0f;
	light2.DiffuseColor.z = 1.0f;
	light2.DiffuseColor.w = 1.0f;

	light2.Direction.x = -1.0f;
	light2.Direction.y = 1.0f;
	light2.Direction.z = 0.0f;

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
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
}



// --------------------------------------------------------
// Initializes the matrices necessary to represent our geometry's 
// transformations and our 3D camera
// --------------------------------------------------------
void Game::CreateMatrices()
{
	// Set up world matrix
	// - In an actual game, each object will need one of these and they should
	//    update when/if the object moves (every frame)
	// - You'll notice a "transpose" happening below, which is redundant for
	//    an identity matrix.  This is just to show that HLSL expects a different
	//    matrix (column major vs row major) than the DirectX Math library
	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W)); // Transpose for HLSL!

	// Create the View matrix
	// - In an actual game, recreate this matrix every time the camera 
	//    moves (potentially every frame)
	// - We're using the LOOK TO function, which takes the position of the
	//    camera and the direction vector along which to look (as well as "up")
	// - Another option is the LOOK AT function, to look towards a specific
	//    point in 3D space
	XMVECTOR pos = XMVectorSet(0, 0, -5, 0);
	XMVECTOR dir = XMVectorSet(0, 0, 1, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	XMMATRIX V = XMMatrixLookToLH(
		pos,     // The position of the "camera"
		dir,     // Direction the camera is looking
		up);     // "Up" direction in 3D space (prevents roll)
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!

	// Create the Projection matrix
	// - This should match the window's aspect ratio, and also update anytime
	//    the window resizes (which is already happening in OnResize() below)
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		(float)width / height,		// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}


// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------

//Doing linear interpolation between

void Game::CreateBasicGeometry()
{
	//Generating a texture resource view from the loaded texture
	CreateWICTextureFromFile(device,context,L"Textures/Image1.JPG",0,&srv1);
	CreateWICTextureFromFile(device,context,L"Textures/Image2.JPG",0,&srv2);
	samplerStruct = {};
	
	samplerStruct.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerStruct.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerStruct.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	samplerStruct.Filter= D3D11_FILTER_ANISOTROPIC;

	samplerStruct.MaxLOD = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	
	device->CreateSamplerState(&samplerStruct, &shaderSampler);
	
	material1 = new Materials(vertexShader, pixelShader, srv1, shaderSampler);
	material2 = new Materials(vertexShader, pixelShader, srv2, shaderSampler);

	XMMATRIX trans = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	XMMATRIX rot = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	XMMATRIX scale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	
	mesh4 = new Mesh("Models/helix.obj", device);
	entityList.push_back(Entity(trans, rot, scale, mesh4, material1,light1));

	trans = XMMatrixTranslation(2.0f, 0.0f, 0.0f);
	rot = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	scale = XMMatrixScaling(0.5f, 0.5f, 0.5f);

	mesh3 = new Mesh("Models/torus.obj", device);
	entityList.push_back(Entity(trans, rot, scale, mesh3, material2,light2));

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
	if (GetAsyncKeyState('V')) 
	{
		camera->RotateUp();
	}
	if (GetAsyncKeyState('C')) 
	{
		camera->RotateDown();
	}
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	//Passing in all the  lighting data to the pixelShader  
	
	//pixelShader->SetData("Light", &light1, sizeof(DirectionalLight));
	
	//pixelShader->SetSamplerState("Sampler", shaderSampler);
	//pixelShader->SetShaderResourceView("Texture", srv1);
	//pixelShader->CopyAllBufferData();
	//pixelShader->SetShader();

	//Looped all the sequences for loading the worldmatrix as well as loading the index and vertex buffers to the 
	//GPU using a vector of entities.

	for (int i = 0; i < entityList.size(); i++) {
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
		DirectionalLight* tempLight = entityList[i].GetLight();
		ID3D11SamplerState* sampler = entityList[i].GetMaterial()->GetSamplerState();
		ID3D11ShaderResourceView* srv = entityList[i].GetMaterial()->GetSRV();
		pPointer->SetData("Light",tempLight, sizeof(DirectionalLight));
		pPointer->SetSamplerState("Sampler", sampler);
		pPointer->SetShaderResourceView("Texture", srv);
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

	swapChain->Present(0, 0);
}


#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void Game::OnMouseDown(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...
	if (buttonState & 0x0001) 
	{
		camera->RotateLeft();
	}
	
	if (buttonState & 0x0002) 
	{
		camera->RotateRight();
	}
	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void Game::OnMouseUp(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void Game::OnMouseMove(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;
}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.  
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void Game::OnMouseWheel(float wheelDelta, int x, int y)
{
	// Add any custom code here...
}
#pragma endregion