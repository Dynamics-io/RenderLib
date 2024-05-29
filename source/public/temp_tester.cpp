#include "temp_tester.h"

#include "../private/triangle_renderer.h"
#include "../private/window.h"
#include "../private/window_glfw.h"

#include "input_events.h"
#include "generic_logger.h"

#include <thread>

using namespace render_vk;
using namespace render_vk::input;

void run_main_loop(Tester* inst);

Tester::Tester(Generic_Logger* logger) :
	m_logger { logger }
{
}

Tester::~Tester()
{
}

void Tester::Run()
{
	m_thread = std::thread(&run_main_loop, this);
	m_thread.join();
}

void run_main_loop(Tester* inst) {
	inst->main_loop();
}

void Tester::main_loop()
{
	m_run = true;

	std::string shader_folder = "triangle";

	RendererBuildInfo info{};
	info.Window_Enabled = true;
	info.Window_Title = "Tester";
	info.Window_Width = 480;
	info.Window_Height = 480;

	info.Instance_Info.Enable_Validation_Layers = true;
	info.Instance_Info.Instance_Type = InstanceType::Create_VK_Instance;

	info.Shader_Directory = std::string(CMAKE_ROOT_DIR) + "\\shaders\\" + shader_folder + "\\bin";

	m_traingle_renderer = Renderer_p::Create<Triangle_Renderer_p>(info);

	if (m_traingle_renderer->Has_Window()) {
		m_Window = dynamic_cast<Window_GLFW_p*>(m_traingle_renderer->Get_Window());
		m_Window->SetOnKeyInput(OnKeyDown_s, this);
		m_logger->Log_Info("Set key press callback");
	}

	m_traingle_renderer->Build();

	while (m_run) {

		_sleep(60);

		m_traingle_renderer->Update(1/60.0);

		if (m_traingle_renderer->Is_Finalized()) {
			m_run = false;
			break;
		}
	}

	if (!m_traingle_renderer->Is_Finalized()) {
		m_traingle_renderer->Finalize();
	}

	//delete m_traingle_renderer;
}

void Tester::OnKeyDown(KeyInputEvent key_event)
{
	m_logger->Log_Info("OnKeyDown: " + std::to_string((int)key_event.get_code()) + ", Action: " + std::to_string((int)key_event.get_action()));
	if (key_event.get_code() == KeyCode::Escape &&
		key_event.get_action() == KeyAction::Up) {

		m_run = false;
	}

}
