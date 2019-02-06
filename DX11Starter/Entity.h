#pragma once
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")
#include "Mesh.h"
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include "Game.h"

using namespace DirectX;
//creating an enitity class

class Entity 
{
	XMFLOAT4X4 translation, rotation, scaling; // Guess this is pretty obvious
	Mesh *mesh;
	static float dT,tT; //static variables representing delta time and totalt time, respectively, for the all entity objects to use.
public:
	Entity(XMMATRIX a, XMMATRIX b, XMMATRIX c, Mesh* d)
	{
		XMStoreFloat4x4(&translation, a);
		XMStoreFloat4x4(&rotation, b);
		XMStoreFloat4x4(&scaling, c);
		mesh = d;
	}
	~Entity(){ delete &translation,&rotation,&scaling,&mesh;}
	static void SetTime(float a, float b)
	{
		/*dT = a;
		tT = b;*/
	}
	void SetPos(XMMATRIX X) { XMStoreFloat4x4(&translation,X); }
	void SetRot(XMMATRIX Y) { XMStoreFloat4x4(&rotation,Y); }
	void SetScale(XMMATRIX Z) { XMStoreFloat4x4(&scaling,Z); }
	XMFLOAT4X4 GetPos(){ return translation; }
	XMFLOAT4X4 GetRot(){ return rotation; }
	XMFLOAT4X4 GetScale(){ return scaling; }
	Mesh* GetMesh() { return mesh; }
	XMMATRIX GetWM() // returns a world matrix for storing in the worldMatrix variable
	{
		XMMATRIX trans, rot, scale, result;
		trans = XMLoadFloat4x4(&translation);
		rot = XMLoadFloat4x4(&rotation);
		scale = XMLoadFloat4x4(&scaling);
		result = XMMatrixMultiply(XMMatrixMultiply(scale, rot),trans);
		return result;
	}
	/*void AddTranslation(XMMATRIX X) 
	{
		XMMATRIX currentTrans = XMLoadFloat4x4(&translation);
		currentTrans += X;
		XMStoreFloat4x4(&translation,current)
	}*/
	
	void AddRotaion(XMMATRIX Y) 
	{
		XMMATRIX currentRot = XMLoadFloat4x4(&rotation); 
	}
	
	void AddScaling(XMMATRIX Z) 
	{
		XMMATRIX currentScale = XMLoadFloat4x4(&scaling); 
	}
};