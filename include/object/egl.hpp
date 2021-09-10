#pragma once

#include <raii/raii.hpp>
#include <EGL/egl.h>
#include <EGL/eglext.h>

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

class ImageKHR : public raii::AutoDeletable<EGLImageKHR> {
public:
  using raii::AutoDeletable<EGLImageKHR>::AutoDeletable;
  ImageKHR(Display, EGLImageKHR image);
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

auto createPlatformWindowSurfaceExt_(Display const &dpy, EGLConfig config,
				     void *win, const EGLint *attrib_list)
    -> Surface;

auto createImageKHR(Display const &dpy, EGLContext ctx, EGLenum target,
		    EGLClientBuffer buffer, const EGLint *attrib_list)
    -> ImageKHR;

auto getPlatformDisplay(EGLenum platform, void *native_display,
			const EGLint *attrib_list) -> Display;
} // namespace egl

