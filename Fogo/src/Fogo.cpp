#include "Fogo.h"

void Fogo::Initialize(const Properties & properties) {
	Window::Create(properties.window);
	Graphics::Create();
//	Game::System::Create(properties.first_key, properties.scenes);
	System::Create(properties.first_key, properties.createScenes);
	Window::Run();
}

void Fogo::Finalize() {
	Window::Destroy();
	Graphics::Destroy();
}
