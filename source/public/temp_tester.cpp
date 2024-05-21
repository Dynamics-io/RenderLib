#include "temp_tester.h"

#include "../private/triangle_renderer.h"
#include "../private/window.h"
#include "../private/window_glfw.h"

#include "input_events.h"

#include <thread>

using namespace render_vk;
using namespace render_vk::input;

void run_main_loop(Tester* inst);

Tester::Tester()
{
}

Tester::~Tester()
{
}

void Tester::Run()
{
	RendererBuildInfo info{};

	m_traingle_renderer = Renderer_p::Create<Triangle_Renderer_p>(info);

	Window_GLFW_p* window = dynamic_cast<Window_GLFW_p*>(m_traingle_renderer->Get_Window());

	window->SetOnKeyInput(OnKeyDown_s, this);
	

	m_traingle_renderer->Build();


	m_thread = std::thread(&run_main_loop, this);
	m_thread.join();

	m_traingle_renderer->Dispose();
}

void run_main_loop(Tester* inst) {
	inst->main_loop();
}

void Tester::main_loop()
{
	m_run = true;

	while (m_run) {

		_sleep(60);

		m_traingle_renderer->Update(1/60.0);

	}
}

void Tester::OnKeyDown(KeyInputEvent key_event)
{
	if (key_event.get_code() == KeyCode::Escape &&
		key_event.get_action() == KeyAction::Up) {
		m_run = false;
	}

}
