#include "Game.h"
#include "Vertex.h"
#include "d3d11.h"
#include "Mesh.h"
#include <vector>
#include "Materials.h"

DefaultMaterials::DefaultMaterials(SimpleVertexShader* a, SimplePixelShader* b, ID3D11ShaderResourceView* c, ID3D11ShaderResourceView* d, ID3D11SamplerState* e)
{
	vShader = a;
	pShader = b;
	sRVPtrColor = c;
	sRVPtrNormal = d;
	samplerState = e;
}

DefaultMaterials::~DefaultMaterials() 
{

}