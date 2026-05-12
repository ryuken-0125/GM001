#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "polygon2D.h"

Polygon2D g_Polygon2D;


void Manager::Init()
{
	Renderer::Init();
	g_Polygon2D.Init();
}


void Manager::Uninit()
{
	Renderer::Uninit();
	g_Polygon2D.Uninit();
}

void Manager::Update()
{
	g_Polygon2D.Update();
}

void Manager::Draw()
{
	Renderer::Begin();

	g_Polygon2D.Draw();

	Renderer::End();
}
