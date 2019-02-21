#include "Game.h"
#include "Vertex.h"
#include "d3d11.h"
#include "Mesh.h"
#include <vector>
#include "Materials.h"

Materials::Materials(SimpleVertexShader* a, SimplePixelShader* b, ID3D11ShaderResourceView* c, ID3D11SamplerState*d)
{
	vShader = a;
	pShader = b;
	sRVpointer = c;
	samplerState = d;
}

Materials::~Materials() 
{
}