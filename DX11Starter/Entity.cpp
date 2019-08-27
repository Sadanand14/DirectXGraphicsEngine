#include "Mesh.h"
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include "Game.h"
#include "Materials.h"

std::vector<Mesh> Entity::m_meshList;

Entity::Entity(XMMATRIX a, XMMATRIX b, XMMATRIX c, unsigned int d, Materials* e)
{
	XMStoreFloat4x4(&m_translation, a);
	XMStoreFloat4x4(&m_rotation, b);
	XMStoreFloat4x4(&m_scaling, c);
	m_meshoffset = d;
	material = e;
}

Entity::~Entity() 
{
}

XMMATRIX Entity::GetWM()
{
	XMMATRIX trans, rot, scale, result;
	trans = XMLoadFloat4x4(&m_translation);
	rot = XMLoadFloat4x4(&m_rotation);
	scale = XMLoadFloat4x4(&m_scaling);
	result = XMMatrixMultiply(XMMatrixMultiply(scale, rot), trans);
	return result;
}