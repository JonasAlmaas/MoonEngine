#pragma once

#include "Moon/Core/Base.h"
#include "Moon/Core/Event.h"


namespace Moon {

	struct WindowSpecification
	{
		std::string Title = "Window Title";
		uint32_t Width = 1920;
		uint32_t Height = 1080;
		bool Decorated = true;
		bool Fullscreen = false;
		bool VSync = true;
	};

	// Interface representing a desktop system based window
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void Init() = 0;
		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual std::pair<uint32_t, uint32_t> GetSize() const = 0;
		virtual std::pair<float, float> GetWindowPos() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		virtual void SetResizable(bool resizable) const = 0;

		virtual void Maximize() = 0;
		virtual void CenterWindow() = 0;

		virtual void EnableCursor() = 0;
		virtual void DisableCursor() = 0;

		virtual const std::string& GetTitle() const = 0;
		virtual void SetTitle(const std::string& title) = 0;

		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowSpecification& spec = WindowSpecification());

	};

}
