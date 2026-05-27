#include "model.h"
#include "renderer.h"

Model::Model() : m_VertexBuffer(nullptr), m_Texture(nullptr), m_VertexCount(0) {}

Model::~Model() { Uninit(); }

bool Model::Load(const char* objFilename, const wchar_t* textureFilename)
{
    // ----------------------------------------------------
    // 1. テクスチャの読み込み (DirectXTexを使用)
    // ----------------------------------------------------
    DirectX::TexMetadata metadata;
    DirectX::ScratchImage image;

    // 相対パスからpngなどの画像を読み込む
    HRESULT hr = DirectX::LoadFromWICFile(textureFilename, DirectX::WIC_FLAGS_NONE, &metadata, image);
    if (SUCCEEDED(hr)) {
        DirectX::CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_Texture);
    }

    // ----------------------------------------------------
    // 2. OBJファイルの読み込み
    // ----------------------------------------------------
    FILE* file = fopen(objFilename, "r");
    if (!file) return false;

    std::vector<DirectX::XMFLOAT3> positions;
    std::vector<DirectX::XMFLOAT2> texcoords;
    std::vector<DirectX::XMFLOAT3> normals;
    std::vector<VERTEX_3D> vertices;

    char header[128];
    while (fscanf(file, "%s", header) != EOF)
    {
        if (strcmp(header, "v") == 0) // 頂点座標
        {
            DirectX::XMFLOAT3 pos;
            fscanf(file, "%f %f %f\n", &pos.x, &pos.y, &pos.z);
            positions.push_back(pos);
        }
        else if (strcmp(header, "vt") == 0) // テクスチャ座標(UV)
        {
            DirectX::XMFLOAT2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            uv.y = 1.0f - uv.y; // DirectXはV座標が反転するため修正
            texcoords.push_back(uv);
        }
        else if (strcmp(header, "vn") == 0) // 法線ベクトル
        {
            DirectX::XMFLOAT3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            normals.push_back(normal);
        }
        else if (strcmp(header, "f") == 0) // 面（ポリゴン）情報
        {
            // v/vt/vn のインデックスを取得
            unsigned int vIndex[3], tIndex[3], nIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
                &vIndex[0], &tIndex[0], &nIndex[0],
                &vIndex[1], &tIndex[1], &nIndex[1],
                &vIndex[2], &tIndex[2], &nIndex[2]);

            if (matches == 9)
            {
                // 三角形の3つの頂点を作成
                for (int i = 0; i < 3; i++)
                {
                    VERTEX_3D vertex;
                    vertex.Position = positions[vIndex[i] - 1]; // OBJ形式はインデックスが1から始まるため -1 する
                    vertex.TexCoord = texcoords[tIndex[i] - 1];
                    vertex.Normal = normals[nIndex[i] - 1];
                    vertices.push_back(vertex);
                }
            }
        }
    }
    fclose(file);
    m_VertexCount = vertices.size();

    // ----------------------------------------------------
    // 3. 頂点バッファの作成
    // ----------------------------------------------------
    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(VERTEX_3D) * m_VertexCount;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem = vertices.data();

    Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

    return true;
}

void Model::Uninit()
{
    if (m_VertexBuffer) { m_VertexBuffer->Release(); m_VertexBuffer = nullptr; }
    if (m_Texture) { m_Texture->Release(); m_Texture = nullptr; }
}

void Model::Draw()
{
    if (!m_VertexBuffer) return;

    ID3D11DeviceContext* context = Renderer::GetDeviceContext();

    UINT stride = sizeof(VERTEX_3D);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    if (m_Texture) {
        context->PSSetShaderResources(0, 1, &m_Texture);
    }

    context->Draw(m_VertexCount, 0);
}