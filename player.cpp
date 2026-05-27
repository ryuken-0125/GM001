#include "main.h"
#include "renderer.h"
#include "player.h"
#include "model.h"

void Player::Init()
{
    // 初期座標とスケールの設定
    m_Position = { 0.0f, 0.0f, 0.0f };
    m_Rotation = { 0.0f, 0.0f, 0.0f };
    m_Scale = { 1.0f, 1.0f, 1.0f };

    m_Model = new Model();
    m_Model->Load("asset/model/player.obj", L"asset/texture/apple.png");
}

void Player::Uninit()
{
    if (m_Model) {
        m_Model->Uninit();
        delete m_Model;
    }
}

void Player::Update()
{
    float speed = 0.1f; // プレイヤーの移動速度

    if (GetAsyncKeyState('W') & 0x8000) 
    { 
        m_Position.z += speed; 
    }
    if (GetAsyncKeyState('S') & 0x8000) 
    { 
        m_Position.z -= speed; 
    }
    if (GetAsyncKeyState('A') & 0x8000) 
    { 
        m_Position.x -= speed;
    }
    if (GetAsyncKeyState('D') & 0x8000) 
    {
        m_Position.x += speed;
    }

}

void Player::Draw()
{
    XMMATRIX scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
    XMMATRIX rotX = XMMatrixRotationX(m_Rotation.x);
    XMMATRIX rotY = XMMatrixRotationY(m_Rotation.y);
    XMMATRIX rotZ = XMMatrixRotationZ(m_Rotation.z);
    XMMATRIX trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

    XMMATRIX world = scale * rotX * rotY * rotZ * trans;

    Renderer::SetWorldMatrix(world);

    m_Model->Draw();

}