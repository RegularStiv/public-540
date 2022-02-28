#include "Transform.h"
using namespace DirectX;
Transform::Transform()
{
    this->worldMatrix = DirectX::XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    this->worldInverseTranspose = DirectX::XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    this->position = DirectX::XMFLOAT3(0, 0, 0);
    this->scale = DirectX::XMFLOAT3(1, 1, 1);
    XMStoreFloat4(&rotation, DirectX::XMQuaternionRotationRollPitchYaw(0, 0, 0));
    this->forward = DirectX::XMFLOAT3(0, 0, 1);
    this->up = DirectX::XMFLOAT3(0, 1, 0);
    this->right = DirectX::XMFLOAT3(1, 0, 0);
}

Transform::Transform(DirectX::XMFLOAT4X4 world, DirectX::XMFLOAT4X4 worldInverseTranspose, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 scale, DirectX::XMFLOAT4 rotaion)
{
    this->worldMatrix = world;
    this->worldInverseTranspose = worldInverseTranspose;
    this->position = position;
    this->scale = scale;
    this->rotation = rotaion;
    this->forward = DirectX::XMFLOAT3(0, 0, 1);
    this->up = DirectX::XMFLOAT3(0, 1, 0);
    this->right = DirectX::XMFLOAT3(1, 0, 0);
     XMVECTOR forwardRot = DirectX::XMVector3Rotate(XMLoadFloat3(&forward),
        DirectX::XMLoadFloat4(&rotation));
     XMVECTOR upRot = DirectX::XMVector3Rotate(XMLoadFloat3(&up),
         DirectX::XMLoadFloat4(&rotation));
     XMVECTOR rightRot = DirectX::XMVector3Rotate(XMLoadFloat3(&right),
         DirectX::XMLoadFloat4(&rotation));
    XMStoreFloat3(&forward, forwardRot);
    XMStoreFloat3(&up, upRot);
    XMStoreFloat3(&right, rightRot);
}

void Transform::SetPosition(float x, float y, float z)
{
    position = DirectX::XMFLOAT3(x, y, z);
}

void Transform::SetPosition(DirectX::XMFLOAT3 xyz)
{
    position = xyz;
}

void Transform::SetRotation(float pitch, float yaw, float roll)
{
    DirectX::XMVECTOR quat = DirectX::XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);
    XMStoreFloat4(&rotation, quat);
    XMVECTOR forwardRot = DirectX::XMVector3Rotate(XMLoadFloat3(&forward),
        DirectX::XMLoadFloat4(&rotation));
    XMVECTOR upRot = DirectX::XMVector3Rotate(XMLoadFloat3(&up),
        DirectX::XMLoadFloat4(&rotation));
    XMVECTOR rightRot = DirectX::XMVector3Rotate(XMLoadFloat3(&right),
        DirectX::XMLoadFloat4(&rotation));
    XMStoreFloat3(&forward, forwardRot);
    XMStoreFloat3(&up, upRot);
    XMStoreFloat3(&right, rightRot);
}

void Transform::SetRotation(DirectX::XMFLOAT4 quaternion)
{
    rotation = quaternion;
    XMVECTOR forwardRot = DirectX::XMVector3Rotate(XMLoadFloat3(&forward),
        DirectX::XMLoadFloat4(&rotation));
    XMVECTOR upRot = DirectX::XMVector3Rotate(XMLoadFloat3(&up),
        DirectX::XMLoadFloat4(&rotation));
    XMVECTOR rightRot = DirectX::XMVector3Rotate(XMLoadFloat3(&right),
        DirectX::XMLoadFloat4(&rotation));
    XMStoreFloat3(&forward, forwardRot);
    XMStoreFloat3(&up, upRot);
    XMStoreFloat3(&right, rightRot);
}

void Transform::SetScale(float x, float y, float z)
{
    scale = DirectX::XMFLOAT3(x, y, z);
}

void Transform::SetScale(DirectX::XMFLOAT3 xyz)
{
    scale = xyz;
}

DirectX::XMFLOAT3 Transform::GetPosition()
{
    return position;
}

DirectX::XMFLOAT4 Transform::GetQuaternion()
{   
    return rotation;

}

DirectX::XMFLOAT3 Transform::GetScale()
{
    return scale;
}

DirectX::XMFLOAT4X4 Transform::GetWorldMatrix()
{
    UpdateMatricies();
    return worldMatrix;
}

DirectX::XMFLOAT4X4 Transform::GetWorldInverseTransposeMatrix()
{
    UpdateMatricies();
    return worldInverseTranspose;
}

DirectX::XMFLOAT3 Transform::GetRight()
{
    return right;
}

DirectX::XMFLOAT3 Transform::GetUp()
{
    return up;
}

DirectX::XMFLOAT3 Transform::GetForward()
{
    return forward;
}

void Transform::MoveAbsolute(float x, float y, float z)
{
    position.x += x;
    position.y += y;
    position.z += z;
}

void Transform::MoveRelative(float x, float y, float z)
{
    DirectX::XMVECTOR moveVec = DirectX::XMVectorSet(x, y, z, 0);
    DirectX::XMVECTOR rotatedVec = DirectX::XMVector3Rotate(moveVec, 
        DirectX::XMLoadFloat4(&rotation));

    DirectX::XMVECTOR newPos = DirectX::XMLoadFloat3(&position) + rotatedVec;
    DirectX::XMStoreFloat3(&position, newPos);
}

void Transform::Rotate(DirectX::XMFLOAT4 quaternionRotation)
{
    DirectX::XMVECTOR quat = DirectX::XMLoadFloat4(&quaternionRotation);
    DirectX::XMVECTOR rotationVect = DirectX::XMLoadFloat4(&rotation);
    rotationVect = DirectX::XMQuaternionMultiply(rotationVect, quat);

    XMStoreFloat4(&rotation, rotationVect);

    XMVECTOR forwardRot = DirectX::XMVector3Rotate(XMLoadFloat3(&forward),
        DirectX::XMLoadFloat4(&rotation));
    XMVECTOR upRot = DirectX::XMVector3Rotate(XMLoadFloat3(&up),
        DirectX::XMLoadFloat4(&rotation));
    XMVECTOR rightRot = DirectX::XMVector3Rotate(XMLoadFloat3(&right),
        DirectX::XMLoadFloat4(&rotation));
    XMStoreFloat3(&forward, forwardRot);
    XMStoreFloat3(&up, upRot);
    XMStoreFloat3(&right, rightRot);
}

void Transform::Rotate(float pitch, float yaw, float roll)
{
    DirectX::XMVECTOR quat = DirectX::XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);
    DirectX::XMVECTOR rotationVect = DirectX::XMLoadFloat4(&rotation);
    rotationVect = DirectX::XMQuaternionMultiply(rotationVect, quat);

    XMStoreFloat4(&rotation, rotationVect);

    XMVECTOR forwardRot = DirectX::XMVector3Rotate(XMLoadFloat3(&forward),
        DirectX::XMLoadFloat4(&rotation));
    XMVECTOR upRot = DirectX::XMVector3Rotate(XMLoadFloat3(&up),
        DirectX::XMLoadFloat4(&rotation));
    XMVECTOR rightRot = DirectX::XMVector3Rotate(XMLoadFloat3(&right),
        DirectX::XMLoadFloat4(&rotation));
    XMStoreFloat3(&forward, forwardRot);
    XMStoreFloat3(&up, upRot);
    XMStoreFloat3(&right, rightRot);
}

void Transform::Scale(float x, float y, float z)
{
    scale.x *= x;
    scale.y *= y;
    scale.z *= z;
}

void Transform::UpdateMatricies()
{
    DirectX::XMMATRIX world = (Scaling() * RotationRollPitchYaw() * Translation());

    DirectX::XMStoreFloat4x4(&worldMatrix, world);
    DirectX::XMStoreFloat4x4(&worldInverseTranspose, DirectX::XMMatrixTranspose(world));
}

DirectX::XMMATRIX Transform::Translation()
{
    return DirectX::XMMatrixTranslation(position.x, position.y, position.z);
}

DirectX::XMMATRIX Transform::Scaling()
{
    return DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
}

DirectX::XMMATRIX Transform::RotationRollPitchYaw()
{
    DirectX::XMVECTOR quaternion = DirectX::XMVectorSet(rotation.x, rotation.y, rotation.z, rotation.w);
    return DirectX::XMMatrixRotationQuaternion(quaternion);;
}
