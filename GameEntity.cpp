#include "GameEntity.h"

GameEntity::GameEntity(std::shared_ptr<Mesh> mesh)
{
	this->mesh = mesh;
}

std::shared_ptr<Mesh> GameEntity::GetMesh()
{
	return mesh;
}

Transform* GameEntity::GetTransform()
{
	return &transform;
}

void GameEntity::Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, Microsoft::WRL::ComPtr<ID3D11Buffer> constBuffer, 
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView, Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader, 
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader, Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout,
	std::shared_ptr<Camera> camera)
{
	// Set the vertex and pixel shaders to use for the next Draw() command
	//  - These don't technically need to be set every frame
	//  - Once you start applying different shaders to different objects,
	//    you'll need to swap the current shaders before each draw
	deviceContext->VSSetShader(vertexShader.Get(), 0, 0);
	deviceContext->PSSetShader(pixelShader.Get(), 0, 0);

	// Ensure the pipeline knows how to interpret the data (numbers)
	// from the vertex buffer.  
	// - If all of your 3D models use the exact same vertex layout,
	//    this could simply be done once in Init()
	// - However, this isn't always the case (but might be for this course)
	deviceContext->IASetInputLayout(inputLayout.Get());

	VertexShaderExternalData vsData;
	vsData.colorTint = DirectX::XMFLOAT4(1.0f, 0.5f, 0.5f, 1.0f);
	vsData.worldMatrix = transform.GetWorldMatrix();
	vsData.projection = camera->GetProjectionMatrix();
	vsData.view = camera->GetViewMatrix();

	D3D11_MAPPED_SUBRESOURCE mappedBuffer = {};
	deviceContext->Map(constBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer);
	memcpy(mappedBuffer.pData, &vsData, sizeof(vsData));
	deviceContext->Unmap(constBuffer.Get(), 0);

	deviceContext->VSSetConstantBuffers(0, // Which slot (register) to bind the buffer to?  
		1, // How many are we activating?  Can do multiple at once  
		constBuffer.GetAddressOf());  // Array of buffers (or the address of one)

	mesh->Draw();
}
