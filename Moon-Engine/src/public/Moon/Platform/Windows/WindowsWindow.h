#pragma once

#include "Moon/Core/Window.h"
#include "Moon/Core/Renderer/GraphicsContext.h"

struct GLFWwindow;


namespace Moon {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowSpecification& spec);
		virtual ~WindowsWindow();

		virtual void Init() override;
		virtual void OnUpdate() override;

		virtual inline uint32_t GetWidth() const override { return m_Data.Width; };
		virtual inline uint32_t GetHeight() const override { return m_Data.Height; };

		virtual std::pair<uint32_t, uint32_t> GetSize() const override { return { m_Data.Width, m_Data.Height }; }
		virtual std::pair<float, float> GetWindowPos() const override;

		// Window attributes
		virtual inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync() const override;
		virtual void SetResizable(bool resizable) const override;

		virtual void Maximize() override;
		virtual void CenterWindow() override;

		virtual void EnableCursor() override;
		virtual void DisableCursor() override;

		virtual const std::string& GetTitle() const override { return m_Data.Title; }
		virtual void SetTitle(const std::string& title) override;

		virtual inline void* GetNativeWindow() const override { return m_Window; };

	private:
		virtual void Shutdown();

	private:
		GLFWwindow* m_Window;
		Scope<GraphicsContext> m_Context;
		WindowSpecification m_Specification;

		struct WindowData
		{
			std::string Title;
			uint32_t Width, Height;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}
