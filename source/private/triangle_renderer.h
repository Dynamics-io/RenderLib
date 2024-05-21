#pragma once

#include "renderer.h"

namespace render_vk {

	class Triangle_Renderer_p : public Renderer_p {
	public:

		Triangle_Renderer_p(){}
		~Triangle_Renderer_p(){}

	protected:

		bool Setup();

		bool Step(double dt);

		bool Cleanup();

	private:


	};

}