#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "polygon2D.h"
#include "field.h"   
#include "camera.h"  

Polygon2D g_Polygon2D;
Field g_Field;     
Camera g_Camera;   

void Manager::Init()
{
	Renderer::Init();
	g_Camera.Init();   
	g_Field.Init();    
	g_Polygon2D.Init();
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
}

void Manager::Draw()
{
	Renderer::Begin();

	g_Camera.Draw();

	g_Field.Draw();

	g_Polygon2D.Draw();

	Renderer::End();
}