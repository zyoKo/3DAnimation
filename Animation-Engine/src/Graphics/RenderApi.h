#pragma once

#include <memory>

#include "Core/Logger/Log.h"
#include "Core/Window/IWindow.h"
#include "OpenGL/Buffers/IndexBuffer.h"
#include "OpenGL/Buffers/VertexArray.h"
#include "OpenGL/Buffers/VertexBuffer.h"
#include "OpenGL/OpenGLContext/OpenGLContext.h"
#include "OpenGL/OpenGLContext/Interfaces/IContext.h"
#include "OpenGL/Shader/Shader.h"
#include "Graphics/OpenGL/Texture2D.h"

namespace Animator
{
	class RenderApi
	{
	public:
		static void CreateContext(IWindow* window)
		{
			if (graphicsContext)
				return;

			graphicsContext = std::make_unique<OpenGLContext>(static_cast<GLFWwindow*>(window->GetNativeWindow()));
		}

		static const std::unique_ptr<IContext>& GetContext()
		{
			ANIM_ASSERT(graphicsContext != nullptr, "Trying to fetch Graphics Context before it was created!");

			return graphicsContext;
		}

		template <typename... Args>
		static std::shared_ptr<IIndexBuffer> CreateIndexBuffer(Args&&... args)
		{
			return std::make_shared<IndexBuffer>(std::forward<Args>(args)...);
		}

		template <typename... Args>
		static std::shared_ptr<IShader> CreateShader(Args&&... args)
		{
			return std::make_shared<Shader>(std::forward<Args>(args)...);
		}

		template <typename... Args>
		static std::shared_ptr<IVertexArray> CreateVertexArray(Args&&... args)
		{
			return std::make_shared<VertexArray>(std::forward<Args>(args)...);
		}

		template <typename... Args>
		static std::shared_ptr<IVertexBuffer> CreateVertexBuffer(Args&&... args)
		{
			return std::make_shared<VertexBuffer>(std::forward<Args>(args)...);
		}

		template <typename... Args>
		static std::shared_ptr<ITexture2D> CreateTexture2D(Args&&... args)
		{
			return std::make_shared<Texture2D>(std::forward<Args>(args)...);
		}

	private:
		inline static std::unique_ptr<IContext> graphicsContext{nullptr};
	};
	
}
