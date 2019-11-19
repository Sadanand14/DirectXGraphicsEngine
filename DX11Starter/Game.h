#pragma once

#include "DXCore.h"
#include "SimpleShader.h"
#include "Mesh.h"
#include "Entity.h"
#include "Camera.h"
#include "Lights.h"
#include "Textures.h"
#include "Materials.h";
#include "types.h"
#include "Emitter.h"
class Game
	: public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();

	//Particle Stuff
	ID3D11DepthStencilState* particleDepth = nullptr;
	ID3D11BlendState* particleBlend = nullptr;
	ID3D11RasterizerState* debugRaster = nullptr;
	Emitter* emitter = nullptr;

	//Water Stuff
	Materials* material = nullptr;
	XMFLOAT4X4 WaterMatrix;
	float WaterTime;
	Waves* waves;
	ID3D11RenderTargetView* refractionRTV = nullptr;
	ID3D11RenderTargetView*reflectionRTV = nullptr;
	ID3D11SamplerState* refractSampler = nullptr;
	ID3D11ShaderResourceView* refractionSRV = nullptr;
	ID3D11ShaderResourceView* reflectionSRV = nullptr;
	ID3D11ShaderResourceView* depthSRV = nullptr;
	ID3D11DepthStencilView* depthView = nullptr;


	//TerrainStuff
	TerrainVertex* terrainVertices = nullptr;
	unsigned int * heightArray = nullptr, * terrainIndices = nullptr;
	XMFLOAT4X4 TerrainMatrix;
	unsigned int m_resolution;

	//General Stuff
	Camera * camera = nullptr;
	std::vector<Entity*> entityList;
	std::map<std::string, Mesh*> meshMap;
	std::map<std::string, Texture*> texMap;

	//creating Directional light
	DirectionalLight light1, light2;

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

	// Overridden mouse input helper methods
	void OnMouseDown(WPARAM buttonState, int x, int y);
	void OnMouseUp(WPARAM buttonState, int x, int y);
	void OnMouseMove(WPARAM buttonState, int x, int y);
	void OnMouseWheel(float wheelDelta, int x, int y);

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
	void LoadHeightMap(const char*, unsigned int );
	void DrawTerrain();
	void DrawQuad(ID3D11ShaderResourceView*);
	/*void CreateReflectionRTVSRV();
	void CreateRfractionRTVSRV();
	void CreateReflectionDSVRTV();*/

	// Buffers to hold actual geometry data
	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;

	// Wrappers for DirectX shaders to provide simplified functionality
	//sky shaders
	SimpleVertexShader* SkyVS = nullptr;
	SimplePixelShader* SkyPS = nullptr;
	//basic entity shaders
	SimpleVertexShader* vertexShader = nullptr;
	SimplePixelShader* pixelShader = nullptr;
	//terrain shaders
	SimpleVertexShader* terrainVS = nullptr;
	SimplePixelShader* terrainPS = nullptr;
	//water shaders
	SimpleVertexShader* QuadVS = nullptr, * waterShaderVS = nullptr, *SSReflVS = nullptr;
	SimplePixelShader* QuadPS = nullptr, * waterShaderPS = nullptr, * SSReflPS = nullptr;
	
	//Particle Shaders
	SimpleVertexShader* particleVS = nullptr, *hybridParticleVS = nullptr, *GPUparticleVS = nullptr;
	SimplePixelShader* particlePS = nullptr;


	// The matrices to go from model space to screen space
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;

	//RasterStates
	ID3D11RasterizerState* skyRS = nullptr;

	//DepthStates
	ID3D11DepthStencilState* skyDS = nullptr;

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;

	int randomNumber, drawcount = 0;
};

