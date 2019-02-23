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
	SimpleVertexShader* vShader;
	SimplePixelShader* pShader;
	ID3D11ShaderResourceView* sRVpointer;
	ID3D11SamplerState* samplerState;
public:
	Materials(SimpleVertexShader* , SimplePixelShader* , ID3D11ShaderResourceView*, ID3D11SamplerState*);
	~Materials();

	inline void SetVrtxptr(SimpleVertexShader* vsh) { vShader = vsh; }
	inline void SetPxlptr(SimplePixelShader* psh) { pShader = psh; }
	inline void SetSRV(ID3D11ShaderResourceView* srv) { sRVpointer = srv; }
	inline void SetSamplerState(ID3D11SamplerState* state) { samplerState = state; }

	inline SimpleVertexShader* GetVrtxptr() {return vShader; }
	inline SimplePixelShader* GetPxlptr() {return pShader; }
	inline ID3D11ShaderResourceView* GetSRV() {return sRVpointer;}
	inline ID3D11SamplerState* GetSamplerState() { return samplerState; }
};