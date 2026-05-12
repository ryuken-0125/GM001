#include "main.h"
#include "renderer.h"
#include "polygon2D.h"

//static float g_Rotation2D = 0.0f;

void Polygon2D::Init()
{
    VERTEX_3D vertex[4];

    vertex[0].Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
    vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);
    vertex[1].Position = XMFLOAT3(200.0f, 0.0f, 0.0f);
    vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);
    vertex[2].Position = XMFLOAT3(0.0f, 200.0f, 0.0f);
    vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);
    vertex[3].Position = XMFLOAT3(200.0f, 200.0f, 0.0f);
    vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

    for (int i = 0; i < 4; i++) {
        vertex[i].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
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

    //テクスチャ読み込み
    TexMetadata metadata;
    ScratchImage image;
    LoadFromWICFile(L"asset\\texture\\apple.png", WIC_FLAGS_NONE, &metadata, image);//テクスチャは変更可
    CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(),
        image.GetImageCount(), metadata, &m_Texture);
    assert(m_Texture);//読み込み失敗時にダイアログを表示


}

void Polygon2D::Uninit()
{
    if (m_VertexBuffer) m_VertexBuffer->Release();
    if (m_VertexLayout) m_VertexLayout->Release();
    if (m_VertexShader) m_VertexShader->Release();
    if (m_PixelShader)  m_PixelShader->Release();

	if (m_Texture) m_Texture->Release();
}

void Polygon2D::Update()
{
    //g_Rotation2D += 0.02f; 
}

void Polygon2D::Draw()
{
    Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);
    Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
    Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

    Renderer::SetWorldViewProjection2D();

    XMMATRIX world = XMMatrixScaling(1.0f, 1.0f, 1.0f)* XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f) * XMMatrixTranslation(0.0f, 0.0f, 0.0f) ;
    Renderer::SetWorldMatrix(world);

    MATERIAL material{};
    material.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
    material.TextureEnable = true;
    Renderer::SetMaterial(material);

    //テクスチャ設定
    Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

    UINT stride = sizeof(VERTEX_3D), offset = 0;
    Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
    Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    Renderer::GetDeviceContext()->Draw(4, 0);



}