#pragma once

#include "DXCore.h"
#include "SimpleShader.h"
#include "Mesh.h"
#include "Entity.h"
#include "Camera.h"
#include "Lights.h"
#include "Textures.h"

#include "types.h"

class Game 
	: public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();

	Materials* material = nullptr;
	ID3D11Buffer* WaterVertexBuffer = nullptr;
	ID3D11Buffer* WaterIndexBuffer = nullptr;
	Camera* camera = nullptr;
	
	std::vector<Entity*> entityList;
	std::map<std::string, Mesh*> meshMap;
	std::map<std::string, Texture*> texMap;

	//creating Directional light
	DirectionalLight light1,light2;
	
	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

	// Overridden mouse input helper methods
	void OnMouseDown (WPARAM buttonState, int x, int y);
	void OnMouseUp	 (WPARAM buttonState, int x, int y);
	void OnMouseMove (WPARAM buttonState, int x, int y);
	void OnMouseWheel(float wheelDelta,   int x, int y);

private:
	// Initialization helper methods - feel free to customize, combine, etc.
	void LoadShaders(); 
	void CreateMatrices();
	void CreateBasicGeometry();
	void LoadModelDirectory();
	void LoadTextureDirectory();
	void AddLighting();
	void RenderSky();
	void CreateWaterMesh();
	void DrawWater(float);
	void CreateWaves();

	// Buffers to hold actual geometry data
	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;

	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* SkyVS = nullptr;
	SimplePixelShader* SkyPS = nullptr;
	SimpleVertexShader* vertexShader = nullptr;
	SimplePixelShader* pixelShader = nullptr;
	SimpleVertexShader* waterShaderVS = nullptr;
	SimplePixelShader* waterShaderPS = nullptr;

	// The matrices to go from model space to screen space
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;

	//RasterStates
	ID3D11RasterizerState* skyRS = nullptr ;

	//DepthStates
	ID3D11DepthStencilState* skyDS = nullptr;

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;

	XMFLOAT4X4 WaterMatrix;
	float WaterTime;
	Waves* waves;
};

