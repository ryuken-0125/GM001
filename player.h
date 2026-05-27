#pragma once
#include "vector3.h"

class Player
{
private:
    Vector3 m_Position; // プレイヤーの3D座標
    Vector3 m_Rotation; // 回転角度
    Vector3 m_Scale;    // 大きさ

    Model* m_Model;

public:
    void Init();
    void Uninit();
    void Update();
    void Draw();

    Vector3 GetPosition() const { return m_Position; }
};