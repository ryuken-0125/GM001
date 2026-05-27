#pragma once
#include "vector3.h"
#include <windows.h> // POINT構造体用

class Camera
{
private:
    Vector3 m_Position{ 0.0f, 2.0f, -5.0f };
    Vector3 m_Target{ 0.0f, 0.0f, 0.0f };

    float m_Yaw = 0.0f;    
    float m_Pitch = 0.2f;     
    float m_Distance = 5.0f;  
    POINT m_PrevMousePos = { 0, 0 }; 

public:
    void Init();
    void Uninit();
    void Update();
    void Draw();


    void SetTarget(Vector3 targetPos) { m_Target = targetPos; }
};