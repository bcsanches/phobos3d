#include "Editor/PH_RequestFactory.h"

Phobos::Editor::RequestFactory_c &Phobos::Editor::RequestFactory_c::GetInstance()
{
	static RequestFactory_c clInstance_gl;

	return clInstance_gl;
}
