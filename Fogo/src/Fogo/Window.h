#pragma once

#include <windows.h>
#include <functional>

namespace Fogo {

	class Window
	{
	public:
		using Procedure = std::function<HRESULT CALLBACK(HWND, UINT, WPARAM, LPARAM)>;
		struct create_window_class_exeption : public std::exception { create_window_class_exeption() : std::exception("create_window_class_exeption") {} };

		static constexpr LPCWSTR DEFAULT_TITLE = L"DEFAULT_TITLE";
		static constexpr LPCWSTR DEFAULT_CLASS_NAME = L"DEFAULT_CLASS_NAME";

	private:
		HINSTANCE __instance;
		HWND __window_handle;
		WNDCLASSEX __window_class;
		struct { int width, height; } __window_size;

		auto createWindowClass(const WNDPROC & procedure, const LPCWSTR & className) const -> WNDCLASSEX;
		auto createWindow(const LPCWSTR & title) const -> HWND;
		auto moveWindowCenter() const -> void;

	public:
		Window(
			const int width,
			const int height,
			const WNDPROC & procedure = [](HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) -> LRESULT {
				if (message == WM_DESTROY) PostQuitMessage(0);
				return DefWindowProc(hwnd, message, wParam, lParam);
			},
			const LPCWSTR & title = DEFAULT_TITLE,
			const LPCWSTR & className = DEFAULT_CLASS_NAME
		);
		auto run() const -> int;
		auto getHandle() const -> HWND;
		static auto HideConsole() -> void;

	};

} // namespace Fogo
