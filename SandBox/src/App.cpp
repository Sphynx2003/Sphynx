#include <Sphynx.h>
#include <Sphynx/Events/ApplicationEvents.h>
#include <iostream>
#include <string>
#include <Sphynx/Core/Graphics/Platform/GLWindow.h>
#include <Sphynx/Core/Transform.h>
#include <Sphynx/Camera.h>

using namespace Sphynx;

class SandBox : public Sphynx::Application
{
private:
	GameObject* Cam = nullptr;
	Transform* TransCam = nullptr;
	Camera* CamComp = nullptr;
public:
	SandBox() {
		Sphynx::Core::Bounds b = { 1024, 768 };
		//TODO: Remove The code that allows this.
		CreateMainWindow(std::make_unique<Core::GLWindow>(this, b, "SandBox", false));
	}
	void Start() {
		Cam = Camera::GetPrimaryCamera();
		TransCam = Cam->GetTransform();
		CamComp = Cam->GetComponent<Camera>();
	}
	void Update() {
		if (Input::IsKeyPressed(Keys::Up)) {
			//TransCam.Translate(glm::vec3(0.0f, 10.0f, 0.0f));
		}
	}
	~SandBox() {

	}
};

//The Main Entry Point for clients
Sphynx::Application* Sphynx::CreateApplication() {
	auto sandbox = new SandBox();
	return sandbox;
}