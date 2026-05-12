#include "main.h"
#include "renderer.h"
#include "field.h"

void Field::Init()
{
    m_Position = { 0.0f, 0.0f, 0.0f };
    m_Rotation = { 0.0f, 0.0f, 0.0f };
    m_Scale = { 1.0f, 1.0f, 1.0f };

    VERTEX_3D vertex[4];
    vertex[0].Position = XMFLOAT3(-1.0f, 1.0f, 0.0f);  vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);
    vertex[1].Position = XMFLOAT3(1.0f, 1.0f, 0.0f);  vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);
    vertex[2].Position = XMFLOAT3(-1.0f, -1.0f, 0.0f);  vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);
    vertex[3].Position = XMFLOAT3(1.0f, -1.0f, 0.0f);  vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

    for (int i = 0; i < 4; i++) {
        vertex[i].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
        vertex[i].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    }

    D3D11_BUFFER_DESC bd{};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(VERTEX_3D) * 4;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA sd{};
    sd.pSysMem = vertex;
    Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

    Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
    Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");
}

void Field::Update()
{
    m_Rotation.y += 0.01f; // 垂直軸回転（立体的）
}

void Field::Draw()
{
    // ※3D描画用のView/Projection設定関数を呼んでください
    Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);
    Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
    Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

    XMMATRIX world = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z) *
        XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z) *
        XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
    Renderer::SetWorldMatrix(world);

    MATERIAL material{};
    material.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
    material.TextureEnable = true;
    Renderer::SetMaterial(material);

    UINT stride = sizeof(VERTEX_3D), offset = 0;
    Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
    Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    Renderer::GetDeviceContext()->Draw(4, 0);
}

void Field::Uninit() { /* 解放処理 */ }