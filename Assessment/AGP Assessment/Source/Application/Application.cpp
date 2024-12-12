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
}

void Application::Run()
{
	if (m_window)
		m_window->Run(); 
}
