#pragma once

#include "DXCore.h"
#include <DirectXMath.h>
#include <memory>
#include <vector>
#include "Mesh.h"
#include "BufferStructs.h"
#include "GameEntity.h"
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects
#include "Camera.h"

class Game 
	: public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

private:

	// Should we use vsync to limit the frame rate?
	bool vsync;

	// Initialization helper methods - feel free to customize, combine, etc.
	void LoadShaders(); 
	void CreateBasicGeometry();

	// Note the usage of ComPtr below
	//  - This is a smart pointer for objects that abide by the
	//    Component Object Model, which DirectX objects do
	//  - More info here: https://github.com/Microsoft/DirectXTK/wiki/ComPtr

	// Buffers to hold actual geometry data
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	
	// Shaders and shader-related constructs
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

	std::shared_ptr<Mesh> triangle;
	std::shared_ptr<Mesh> rect;
	std::shared_ptr<Mesh> pentagon;
	std::vector<std::shared_ptr<GameEntity>> entities;
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBufferVS;
	std::shared_ptr<Camera> camera;
	Transform transform;
};

