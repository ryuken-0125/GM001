#pragma once

#include "vector3.h"

class Camera
{
private:

    Vector3 m_Position{ 0.0f, 0.0f, 0.0f };
    Vector3 m_Target{ 0.0f, 0.0f, 0.0f };


public:
    void Init();
    void Uninit();
    void Update();
    void Draw();

};