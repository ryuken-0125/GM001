#include "main.h"
#include "renderer.h"
#include "camera.h"
#include <cmath> // sinf, cosf用

void Camera::Init()
{
    m_Position = { 0.0f, 2.0f, -5.0f };
    m_Target = { 0.0f, 0.0f, 0.0f };
    GetCursorPos(&m_PrevMousePos); // マウスの初期位置を取得
}

void Camera::Update()
{
    // --- 1. 矢印キーでのカメラ回転 ---
    if (GetAsyncKeyState(VK_LEFT) & 0x8000)  m_Yaw -= 0.05f;
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000) m_Yaw += 0.05f;
    if (GetAsyncKeyState(VK_UP) & 0x8000)    m_Pitch -= 0.05f;
    if (GetAsyncKeyState(VK_DOWN) & 0x8000)  m_Pitch += 0.05f;

    // --- 2. マウスでのカメラ回転（右クリックしながらドラッグ） ---
    POINT currentMousePos;
    GetCursorPos(&currentMousePos);

    if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
        float deltaX = (float)(currentMousePos.x - m_PrevMousePos.x);
        float deltaY = (float)(currentMousePos.y - m_PrevMousePos.y);
        m_Yaw += deltaX * 0.005f;
        m_Pitch += deltaY * 0.005f;
    }
    m_PrevMousePos = currentMousePos;

    // ピッチ角（上下）の制限（真上や真下を向いた時にカメラが反転・バグるのを防ぐ）
    if (m_Pitch > 1.5f) m_Pitch = 1.5f;
    if (m_Pitch < -1.5f) m_Pitch = -1.5f;

    // --- 3. カメラの座標をTargetを中心に計算 ---
    m_Position.x = m_Target.x + sinf(m_Yaw) * cosf(m_Pitch) * m_Distance;
    m_Position.y = m_Target.y + sinf(m_Pitch) * m_Distance;
    m_Position.z = m_Target.z - cosf(m_Yaw) * cosf(m_Pitch) * m_Distance;
}

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