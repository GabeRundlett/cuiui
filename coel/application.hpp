#pragma once
#include <coel/window.hpp>
#include <vector>
#include <memory>

namespace coel {
	class application {
		void init_window(const window &w);

	public:
		std::vector<std::unique_ptr<window>> windows;
		bool invalidated = false;

		template<typename derived_window_t, typename...ctor_args_t>
		void add_window(ctor_args_t...args) {
            windows.emplace_back(std::make_unique<derived_window_t>(args...));
			init_window(*windows.back());
            invalidated = true;
		}

		void update();
	};
}
