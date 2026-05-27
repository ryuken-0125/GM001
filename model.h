#pragma once
#include "main.h"
#include <vector>


struct VERTEX_3D
{
    DirectX::XMFLOAT3 Position; // 座標
    DirectX::XMFLOAT3 Normal;   // 法線（光の反射計算用）
    DirectX::XMFLOAT2 TexCoord; // UV（テクスチャ座標）
};

class Model
{
private:
    ID3D11Buffer* m_VertexBuffer; // 頂点バッファ
    ID3D11ShaderResourceView* m_Texture;      // テクスチャ
    int                         m_VertexCount;  // 頂点数

public:
    Model();
    ~Model();

    bool Load(const char* objFilename, const wchar_t* textureFilename);
    void Uninit();
    void Draw();
};