#pragma once
#include "Transform.h"
#include "Input.h"
#include <DirectXMath.h>
class Camera
{
public:
	Camera(float x,float y, float z, float aspectRatio, float fov);
	~Camera();

	void Update(float dt);
	void UpdateViewMatrix();
	void UpdateProjectionMatrix(float fov, float aspectRatio, float nearPlane, float farPlane);

	Transform* GetTransform();
	DirectX::XMFLOAT4X4 GetViewMatrix();
	DirectX::XMFLOAT4X4 GetProjectionMatrix();

private:
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;

	Transform transform;
	float fov;
};

