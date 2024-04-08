#pragma once

namespace SculptorGL
{
	class IWindow;
}

namespace SculptorGL
{
	class IApplication
	{
	public:
		virtual ~IApplication() = default;

		virtual void Initialize() = 0;

		virtual void PreUpdate() = 0;

		virtual void Update() = 0;

		virtual void PostUpdate() = 0;

		virtual void Shutdown() = 0;

		std::weak_ptr<IWindow> GetWindowsWindow() const { return windowsWindow; }

		void SetWindowsWindow(std::weak_ptr<IWindow> window) noexcept { windowsWindow = std::move(window); }

	private:
		std::weak_ptr<IWindow> windowsWindow;
	};

	std::shared_ptr<IApplication> CreateApplication();
}