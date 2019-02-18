#pragma once
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")
#include "Mesh.h"
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include "Game.h"
#include "Materials.h"
#include "Lights.h"

using namespace DirectX;
//creating an enitity class

class Entity 
{
	XMFLOAT4X4 translation, rotation, scaling; // Guess this is pretty obvious
	Mesh *mesh;
	Materials* material;
	//static float dT,tT; //static variables representing delta time and totalt time, respectively, for the all entity objects to use.
public:
	Entity(XMMATRIX a, XMMATRIX b, XMMATRIX c, Mesh* d, Materials* e);
	~Entity();
	
	void SetPos(XMMATRIX X) { XMStoreFloat4x4(&translation,X); }
	void SetRot(XMMATRIX Y) { XMStoreFloat4x4(&rotation,Y); }
	void SetScale(XMMATRIX Z) { XMStoreFloat4x4(&scaling,Z); }
	XMFLOAT4X4 GetPos(){ return translation; }
	XMFLOAT4X4 GetRot(){ return rotation; }
	XMFLOAT4X4 GetScale(){ return scaling; }
	Mesh* GetMesh() { return mesh; }

	XMMATRIX GetWM(); // returns a world matrix for storing in the worldMatrix variable

	//void AddTranslation(XMMATRIX X); //These 3 functions are for added features and optimisations that I am planning to implement
	//void AddRotaion(XMMATRIX Y);// But I am yet to decide how I wanna do this.
	//void AddScaling(XMMATRIX Z);
};