#include <Windows.h>
#include "../source/public/temp_tester.h" 

#include "logger.h"

using namespace render_vk;

//int main()
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{

	Logger log = Logger();

	log.Log_Info("starting");

	Sleep(10000);

	return 0;
	Tester* tester = new Tester();

	tester->Run();

	return 0;
}