#pragma once
#include <d3d11.h>
#include "Vertex.h"

//creating mesh class
class Mesh
{
	ID3D11Buffer *vertexPointer = nullptr, *indexPointer = nullptr;
	int indexCount=NULL;
	void CalculateTangents(Vertex* , int , unsigned int* , int);
public: 

	Mesh(Vertex* vertextArray, unsigned int* intArray, int totalVertices, int totalIndices, ID3D11Device* device);
	Mesh(char* objFile, ID3D11Device* device);
	~Mesh();
	
	inline ID3D11Buffer* GetVertexBuffer() { return vertexPointer; }
	inline ID3D11Buffer* GetIndexBuffer() { return indexPointer; }
	inline int GetIndexCount() { return indexCount; }
	void Mesh::CreatingBuffer(Vertex* vertextArray, unsigned int* intArray, int totalVertices, int totalIndices, ID3D11Device* device);
};