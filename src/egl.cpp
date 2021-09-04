
#include "object/egl.hpp"
#include <EGL/eglext.h>

namespace egl {

Display::Display(EGLDisplay display)
    : raii::AutoDeletable<EGLDisplay>(display, [](auto display) {
	release<Display>(display);
      }) {}

Surface::Surface(Display display, EGLSurface surface)
    : raii::AutoDeletable<EGLSurface>(surface, [display](auto surface) {
	release<Surface>(display, surface);
      }) {}

Context::Context(Display display, EGLContext context)
    : raii::AutoDeletable<EGLContext>(context, [display](auto context) {
	release<Context>(display, context);
      }) {}

Context::Context(EGLContext context)
    : raii::AutoDeletable<EGLContext>(context, [](auto) {}) {
  //TODO: Change EGL_NO_CONTEXT as default value
  if (context != EGL_NO_CONTEXT) {
    throw std::runtime_error("ERROR: egl::Context allows only EGL_NO_CONTEXT");
  }
}

// Generated
auto getPlatformDisplay(EGLenum platform, void *native_display,
			const EGLint *attrib_list) -> Display {
  static auto eglGetPlatformDisplayEXT =
      reinterpret_cast<PFNEGLGETPLATFORMDISPLAYEXTPROC>(
	  eglGetProcAddress("eglGetPlatformDisplayEXT"));
  if (!eglGetPlatformDisplayEXT) {
    throw std::runtime_error("EGL doesn't support eglGetPlatformDisplayEXT");
  }
  auto display =
      eglGetPlatformDisplayEXT(platform, native_display, attrib_list);
  return Display(display);
}

auto createWindowSurface_(Display const &dpy, EGLConfig config,
			  EGLNativeWindowType win, const EGLint *attrib_list)
    -> Surface {
  auto surface = eglCreateWindowSurface(dpy, config, win, attrib_list);
  return Surface(dpy, surface);
}

// CREATE
auto createContext(Display dpy, EGLConfig config, Context share_context,
		   const EGLint *attrib_list) -> Context {
  auto context = eglCreateContext(dpy, config, share_context, attrib_list);
  return Context(dpy, context);
}

} // namespace egl

