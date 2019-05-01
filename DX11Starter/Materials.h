#pragma once
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")
#include "Mesh.h"
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include "Game.h"
#include "Lights.h"
class DefaultMaterials 
{
	SimpleVertexShader* vShader;
	SimplePixelShader* pShader;
	ID3D11ShaderResourceView* sRVPtrColor;
	ID3D11ShaderResourceView* sRVPtrNormal;
	ID3D11SamplerState* samplerState;
public:
	DefaultMaterials(SimpleVertexShader* , SimplePixelShader* , ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11SamplerState*);
	~DefaultMaterials();

	inline void SetVrtxptr(SimpleVertexShader* vsh) { vShader = vsh; }
	inline void SetPxlptr(SimplePixelShader* psh) { pShader = psh; }
	inline void SetSRV(ID3D11ShaderResourceView* srv) { sRVPtrColor = srv; }
	inline void SetSamplerState(ID3D11SamplerState* state) { samplerState = state; }

	inline SimpleVertexShader* GetVrtxptr() {return vShader; }
	inline SimplePixelShader* GetPxlptr() {return pShader; }
	inline ID3D11ShaderResourceView* GetSRVColor() {return sRVPtrColor;}
	inline ID3D11ShaderResourceView* GetSRVNormal() {return sRVPtrNormal;}
	inline ID3D11SamplerState* GetSamplerState() { return samplerState; }
};