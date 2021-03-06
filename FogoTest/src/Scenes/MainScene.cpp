#include "MainScene.h"
#include "../Store.h"
#include "../Components/Camera.h"
#include <iostream>
#include "../Components/Model.h"

using Microsoft::WRL::ComPtr;
using Fogo::Graphics;
using Fogo::Texture;
using Fogo::FBXModel;
using Fogo::Input;
using Fogo::KeyCode;
using Fogo::PubSub;
using Fogo::MappedStore;
using Fogo::System;
using Fogo::Store;
using Fogo::Component;
using Fogo::Updater;
using Fogo::Renderer;
using Fogo::Initializer;
using Fogo::Finalizer;
using Fogo::Meta;
using Fogo::Time;

MainScene::MainScene() {
	static auto once = true;
	if (once) {
		ResourceStore::Insert(VertexShader::BOX, Graphics::CompileVertexShader(L"./resources/shader/VertexShader.hlsl", 0, "main", "vs_5_1"));
		ResourceStore::Insert(PixelShader::BOX, Graphics::CompilePixelShader(L"./resources/shader/PixelShader.hlsl", 0, "main", "ps_5_1"));
		ResourceStore::Insert(TextureType::BOX, std::make_shared<Texture>(L"./resources/Textures/KUTIJE/M_1.jpg"));
		once = false;
	}
	
	camera = &create<Camera>().makeIndex("MainCamera");

	using namespace DirectX;

	camera->target = XMFLOAT3 { 0, 0, 0 };
	camera->position = XMFLOAT3{ 0, 0, -50 };
	camera->fov = 60;
	camera->nearZ = 1;
	camera->farZ = 1000;
}

void MainScene::update() {
	static constexpr auto CAMERA_SPEED = 100.0f;
	if (Input::GetPress(KeyCode::UpArrow)) {
		camera->position.y += CAMERA_SPEED * Time::GetElapsedTime();
	}
	if (Input::GetPress(KeyCode::DownArrow)) {
		camera->position.y -= CAMERA_SPEED * Time::GetElapsedTime();
	}
	if (Input::GetPress(KeyCode::LeftArrow)) {
		camera->position.x += CAMERA_SPEED * Time::GetElapsedTime();
	}
	if (Input::GetPress(KeyCode::RightArrow)) {
		camera->position.x -= CAMERA_SPEED * Time::GetElapsedTime();
	}
	const auto pos = get<Model>()->getPosition();

	camera->target = pos;
	
	Scene::update();
}
