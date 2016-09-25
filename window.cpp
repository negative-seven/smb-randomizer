#include "window.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread]
int window(array<String^>^ args) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew SuperMeatBoyrandomizer::window);

	return 0;
}