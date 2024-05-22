#include <Windows.h>
#include "../source/public/temp_tester.h" 

#include "logger.h"

using namespace render_vk;

int x[100];

//int main()
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{

	//printf("Hello!\n");
	//x[100] = 5; // Boom!
	//return 0;


	// Attempt to attach to the parent process console if it exists
	if (!AttachConsole(ATTACH_PARENT_PROCESS))
	{
		// No parent console, allocate a new one for this process
		if (!AllocConsole())
		{
			throw std::runtime_error{ "AllocConsole error" };
		}
	}

	Logger log = Logger();

	//log.Log_Info("starting");

	Tester* tester = new Tester(log.Get_Generic_Logger());

	tester->Run();

	delete tester;

	return 0;
}