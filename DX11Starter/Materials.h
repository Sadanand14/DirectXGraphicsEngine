#pragma once
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")
#include "Mesh.h"
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include "Game.h"
#include "Lights.h"
class Materials 
{
	SimpleVertexShader* vShader = nullptr;
	SimplePixelShader* pShader = nullptr;
	ID3D11ShaderResourceView* sRVpointer = nullptr;
	ID3D11SamplerState* samplerState = nullptr;
public:
	Materials(SimpleVertexShader* , SimplePixelShader* , ID3D11ShaderResourceView*, ID3D11SamplerState*);
	virtual ~Materials();

	void SetVrtxptr(SimpleVertexShader* vsh) { vShader = vsh; }
	void SetPxlptr(SimplePixelShader* psh) { pShader = psh; }
	void SetSRV(ID3D11ShaderResourceView* srv) { sRVpointer = srv; }
	void SetSamplerState(ID3D11SamplerState* state) { samplerState = state; }

	SimpleVertexShader* GetVrtxptr() { return vShader; }
	SimplePixelShader* GetPxlptr() { return pShader; }
	ID3D11ShaderResourceView* GetSRV() { return sRVpointer; }
	ID3D11SamplerState* GetSamplerState() { return samplerState; }
};