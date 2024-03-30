#pragma once

#include "Math/Math.h"

namespace AnimationEngine
{
	class IWindow;
	class BufferTexture;
	class IShader;
	class IIndexBuffer;
	class IVertexArray;
	class IVertexBuffer;
}
namespace AnimationEngine
{
	class ScreenQuad
	{
	public:
		ScreenQuad();

		void Initialize();

		void Update();

		void Draw() const;

		void SetVertices(std::vector<Math::Vec2F> vertexData) noexcept;

		void SetTextureCoordinates(std::vector<Math::Vec2F> texCoords) noexcept;

		void AddTexture(std::weak_ptr<BufferTexture> texture) noexcept;

		void SetShader(std::weak_ptr<IShader> shader) noexcept;

		void SetWindowsWindow(std::weak_ptr<IWindow> windowsWindow) noexcept;

	private:
		std::vector<Math::Vec2F> vertices;

		std::vector<Math::Vec2F> textureCoordinates;

		std::shared_ptr<IVertexArray> vertexArrayObject;

		std::shared_ptr<IVertexBuffer> vertexBuffer;

		std::weak_ptr<IShader> shader;

		std::vector<std::weak_ptr<BufferTexture>> textures;

		//-- Lights Data --//
		std::vector<Math::Vec3F> lightPositions;

		std::vector<Math::Vec3F> lightColors;

		std::vector<float> lightRadius;
		//-- !Lights Data --//

		std::weak_ptr<IWindow> window;

		bool dirtyFlag;

		void SetupMesh() const;

		void CreateShader();

		void UploadShaderData() const;
	};
}
