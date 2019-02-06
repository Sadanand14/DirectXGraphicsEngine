#pragma once
#include <d3d11.h>
#include "Vertex.h"

//creating mesh class
class Mesh
{
	ID3D11Buffer *vertexPointer, *indexPointer;
	int indexCount;
public: 

	Mesh(Vertex* vertextArray, int* intArray, int totalVertices, int totalIndices, ID3D11Device* device)
	{
		indexCount = totalIndices;
		//creating buffer for the vertices
		D3D11_BUFFER_DESC vbd;
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(Vertex) * 7;// I modified this so that I wouldn't need to define 4 separate meshes to create 4 objects      
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER; 
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA initialVertexData;
		initialVertexData.pSysMem = vertextArray;
		device->CreateBuffer(&vbd, &initialVertexData, &vertexPointer);

		//creating buffer for the indices
		D3D11_BUFFER_DESC ibd;
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = sizeof(int) * 12;// I modified this so that I wouldn't need to define 4 separate meshes to create 4 objects         
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER; 
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;
		ibd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA initialIndexData;
		initialIndexData.pSysMem = intArray;
		device->CreateBuffer(&ibd, &initialIndexData, &indexPointer);
	}
	ID3D11Buffer* GetVertexBuffer() { return vertexPointer; }
	ID3D11Buffer* GetIndexBuffer() { return indexPointer; }
	int GetIndexCount() { return indexCount; }
	~Mesh() 
	{ 
		if (vertexPointer) { vertexPointer->Release(); }
		if (indexPointer) { indexPointer->Release(); }
	}
};