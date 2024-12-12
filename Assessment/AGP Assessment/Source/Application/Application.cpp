#include "Application.h"

Application::Application()
{
}

Application::~Application()
{
	if (m_window)
	{
		delete m_window;
		m_window = nullptr;
	}
	if (m_renderer)
	{
		delete m_renderer;
		m_renderer = nullptr;
	}
}

bool Application::Initialize(HINSTANCE hInstance, int nCmdShow)
{
	if (m_window == nullptr)
	m_window = new Window();
	else
	{
		return false;
	}

	if (FAILED(m_window->InitWindow(hInstance, nCmdShow)))
	{
		return false;
	}

	if (m_renderer == nullptr)
		m_renderer = new Renderer();
	else
	{
		return false;
	}

	if (FAILED(m_renderer->InitRenderer(m_window->GetHWND(), SCREEN_WIDTH, SCREEN_HEIGHT)))
	{
		return false;
	}

	m_window->OpenConsole();	

	return true;
}

void Application::Run()
{
	MSG msg = { 0 };

	while (WM_QUIT != msg.message)
	{

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			///Run game code here
			m_renderer->Clear();

			//render current scene in here, as well as determine whether we are in "edit" mode or "play" mode, and render IMGUI accordingly, and update behaviours accordingly.


			m_renderer->Present();
		}

	}
}

void Application::SetMode(Mode mode)
{
	m_mode = mode;
	//if mode is play, hide IMGUI, if mode is edit, show IMGUI.
	//send objects back to their original positions if mode is edit, and update behaviours if mode is play. 
}
