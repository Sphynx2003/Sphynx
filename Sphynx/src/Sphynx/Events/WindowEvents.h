#pragma once
#include "Event.h"
#define ImplDefaultWinEvent(e) e(Sphynx::Core::IWindow* win) : WindowEvent(win){}
namespace Sphynx {
	namespace Core {
		class IWindow;
	}
}

namespace Sphynx::Events {
	struct WindowEvent : public Event {
	protected:
		Sphynx::Core::IWindow* window;
		WindowEvent() = delete;
		WindowEvent(Sphynx::Core::IWindow* win) : window(win) {};
	public:
		Sphynx::Core::IWindow* GetWindow() { return window; };
	};
	struct OnWindowOpen : public WindowEvent {
	public:
		OnWindowOpen(Sphynx::Core::IWindow* win) : WindowEvent(win) {};
	};
	struct OnWindowResize :public WindowEvent {
		int Width, Height;
		OnWindowResize(Sphynx::Core::IWindow* win, int width, int height) :WindowEvent(win), Width(width), Height(height) {};
	};
	struct OnWindowRestore : public WindowEvent{
		ImplDefaultWinEvent(OnWindowRestore);
	};
	struct OnWindowMaximize : public WindowEvent {
		ImplDefaultWinEvent(OnWindowMaximize);
	};
	struct OnWindowMinimize : public WindowEvent {
		ImplDefaultWinEvent(OnWindowMinimize);
	};
	struct OnWindowFocus : public WindowEvent {
	private:
		int Focus = 0;
	public:
		int IsFocused() { return Focus; }
		OnWindowFocus(Sphynx::Core::IWindow* win, int focus) : WindowEvent(win), Focus(focus) {};
	};
	struct OnWindowUpdate : public WindowEvent {
	public:
		ImplDefaultWinEvent(OnWindowUpdate);
	};
	struct OnWindowClose : public WindowEvent {
		OnWindowClose(Sphynx::Core::IWindow* win) : WindowEvent(win) {};
	};
}
