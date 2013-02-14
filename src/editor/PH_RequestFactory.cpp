#include "Editor/PH_RequestFactory.h"

Phobos::Editor::RequestFactory &Phobos::Editor::RequestFactory::GetInstance()
{
	static RequestFactory clInstance_gl;

	return clInstance_gl;
}
