#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "polygon2D.h"
#include "field.h"   
#include "camera.h"  
#include "player.h"

Polygon2D g_Polygon2D;
Field g_Field;     
Camera g_Camera;   
Player* g_Player;
Camera* g_Camera;

void Manager::Init()
{
	Renderer::Init();
	g_Camera.Init();   
	g_Field.Init();    
	g_Polygon2D.Init();
	g_Player = new Player();
	g_Player->Init();
	g_Camera = new Camera();
	g_Camera->Init();

}

void Manager::Uninit()
{
	Renderer::Uninit();
	g_Camera.Uninit();  
	g_Field.Uninit();   
	g_Polygon2D.Uninit();
}

void Manager::Update()
{
	g_Camera.Update(); 
	g_Field.Update();  
	g_Polygon2D.Update();
	g_Player->Update(); 
	g_Camera->SetTarget(g_Player->GetPosition());
	g_Camera->Update(); 

}

void Manager::Draw()
{
	Renderer::Begin();

	g_Camera.Draw();

	g_Field.Draw();

	g_Polygon2D.Draw();

	Renderer::End();
}