#include "Fogo.h"

void Fogo::Initialize(const Properties & properties) {
	Utility::Window::Create(properties.window);
	Graphics::DX12::Graphics::Create();
	Game::GameController<const char *>::Create(properties.first_key, properties.scenes);
	Utility::Window::Run();
}

void Fogo::Finalize() {
	Utility::Window::Destroy();
	Graphics::DX12::Graphics::Destroy();
}
