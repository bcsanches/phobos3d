#include "Phobos/Editor/Camera.h"

Phobos::Editor::Camera::Camera()
{
	m_clCamera.SetNearClipDistance(0.1f);
	m_clCamera.SetFarClipDistance(100);

	m_clCamera.EnableViewport();
}

Phobos::Editor::Camera::~Camera()
{
	//empty
}
