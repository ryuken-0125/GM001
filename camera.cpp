#include "main.h"
#include "renderer.h"
#include "camera.h"

void Camera::Init()
{
    m_Position = { 0.0f, 2.0f, -5.0f };
    m_Target = { 0.0f, 0.0f, 0.0f };
}

void Camera::Update() {}

void Camera::Draw()
{
    XMVECTOR eye = XMLoadFloat3((XMFLOAT3*)&m_Position);
    XMVECTOR target = XMLoadFloat3((XMFLOAT3*)&m_Target);
    XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    XMMATRIX view = XMMatrixLookAtLH(eye, target, up);
    XMMATRIX proj = XMMatrixPerspectiveFovLH(XM_PIDIV4, 800.0f / 600.0f, 0.1f, 1000.0f);

    Renderer::SetViewMatrix(view);
    Renderer::SetProjectionMatrix(proj);
}

void Camera::Uninit() {}