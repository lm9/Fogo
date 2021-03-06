#pragma once

#include <unordered_map>
#include <Windows.h>
#include <functional>

namespace Fogo {
#ifdef _DEBUG
	static constexpr auto DEBUG = true;
#else
	static constexpr auto DEBUG = false;
#endif

	void Log(const char * message);
	void Log(const std::string & message);
	std::string GenerateGUID();

	// Templates

	// switch-caseっぽいもの
	template <typename State>
	void Match(State state, std::unordered_map<State, std::function<void(void)>> matches) {
		matches[state]();
	}

	// 失敗したときに例外なげるやつ
	template <typename exception = std::exception>
	void ExecOrFail(const HRESULT & hr) {
		static_assert(std::is_base_of<std::exception, exception>());
		if (FAILED(hr)) throw exception();
	}
	template <typename exception = std::exception>
	void ExecOrFail(const HANDLE & handle) {
		static_assert(std::is_base_of<std::exception, exception>());
		if (handle == nullptr) throw exception();
	}
	template <typename exception = std::exception>
	void ExecOrFail(const bool & condition) {
		static_assert(std::is_base_of<std::exception, exception>());
		if (!condition) throw exception();
	}
}
