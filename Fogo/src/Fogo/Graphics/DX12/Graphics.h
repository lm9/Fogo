#pragma once

#include <Windows.h>
#include <d3d12.h>
#include <d3d12shader.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include <stdexcept>
#include <vector>
#include <functional>
#include <memory>
#include <array>
#include "RenderTargetView.h"
#include "DepthStencilView.h"

namespace Fogo {
	class Graphics
	{
	private:
		template <typename T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;

	public: // Types
		struct WindowSize { UINT width, height; };
		struct exception : public std::exception {
			exception(const char * message) : std::exception(message) {}
			exception() : exception("[DX12Graphics] error") {}
		};

	private: // private variables
		static constexpr unsigned char RENDER_TARGET_VIEW_NUMBER = 2;
		static constexpr auto MINIMUM_FEATURE_LEVEL = D3D_FEATURE_LEVEL_11_0;

		static Graphics * instance;

		static std::array<float, 4> clearColor;

		HWND windowHandle;
		WindowSize windowSize;

		UINT64 frames;
		UINT rtvIndex;

		ComPtr<IDXGIFactory4> factory;
		ComPtr<ID3D12Device> device;
		ComPtr<ID3D12CommandQueue> commandQueue;

		HANDLE fenceEvent;
		ComPtr<ID3D12Fence> queueFence;

		ComPtr<IDXGISwapChain3> swapChain;
		ComPtr<ID3D12GraphicsCommandList> commandList;
		ComPtr<ID3D12CommandAllocator> commandAllocator;

		std::unique_ptr<RenderTargetView> renderTargetView;

		std::unique_ptr<DepthStencilView> depthStencilView;

		D3D12_RECT scissorRect;
		D3D12_VIEWPORT viewport;

		std::vector<std::function<void(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>)>> renderers;
		ComPtr<ID3D12Debug> debugger;

	private: // private functions
		auto createFactory() -> void;
		auto createDevice() -> void;
		auto createCommandQueue() -> void;
		auto createSwapChain() -> void;
		auto createRenderTargetView() -> void;
		auto createDepthStencilBuffer() -> void;
		auto createCommandList() -> void;
		auto waitForPreviousFrame() -> void;
		auto setResourceBarrier(D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState) -> void;
		auto populateCommandList() -> void;
		auto render() -> void;
		Graphics();

	public: // functions
		Graphics(const Graphics &) = delete;
		auto operator=(const Graphics &) -> Graphics & = delete;
		static auto Create() -> Graphics &;
		static auto Destroy() -> void;
		static auto GetInstance() -> Graphics &;
		static auto Render(const std::function<void(ComPtr<ID3D12GraphicsCommandList>)> & renderer) -> void;
		static auto Render() -> void;
		static auto GetDevice() -> ComPtr<ID3D12Device>;
		static auto GetCommandList() -> ComPtr<ID3D12GraphicsCommandList>;
		static auto CompileVertexShader(LPCWSTR fileName, UINT compileFlag = 0, const char * entryFunc = "VSMain", const char * target = "vs_5_0") ->ComPtr<ID3DBlob>;
		static auto CompilePixelShader(LPCWSTR fileName, UINT compileFlag = 0, const char * entryFunc = "PSMain", const char * target = "ps_5_0") -> ComPtr<ID3DBlob>;
		static auto SetClearColor(const std::array<float, 4> & newClearColor) -> void;
	};
}