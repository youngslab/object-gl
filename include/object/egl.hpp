#pragma once

#include <raii/raii.hpp>
#include <EGL/egl.h>

namespace egl {

class Display : public raii::AutoDeletable<EGLDisplay> {
public:
  using raii::AutoDeletable<EGLContext>::AutoDeletable;
  Display(EGLDisplay display);
};

class Surface : public raii::AutoDeletable<EGLSurface> {
public:
  using raii::AutoDeletable<EGLContext>::AutoDeletable;
  Surface(Display, EGLSurface);
};

class Context : public raii::AutoDeletable<EGLContext> {
public:
  using raii::AutoDeletable<EGLContext>::AutoDeletable;
  Context(Display, EGLContext);

  Context(EGLContext);
};

template <typename T> auto release(void *) -> void;
template <> inline auto release<Display>(EGLDisplay display) -> void {
  eglTerminate(display);
}

template <typename T> auto release(void *, void *) -> void;
template <>
inline auto release<Surface>(EGLDisplay display, EGLSurface surface) -> void {
  eglDestroySurface(display, surface);
}

template <>
inline auto release<Context>(EGLDisplay display, EGLContext context) -> void {
  eglDestroyContext(display, context);
}

auto createContext(Display dpy, EGLConfig config, Context share_context,
		   const EGLint *attrib_list) -> Context;

auto createWindowSurface_(Display const &dpy, EGLConfig config,
			  EGLNativeWindowType win, const EGLint *attrib_list)
    -> Surface;

auto getPlatformDisplay(EGLenum platform, void *native_display,
			const EGLint *attrib_list) -> Display;
} // namespace egl

