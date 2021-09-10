
#include "object/egl.hpp"
#include <EGL/eglext.h>

namespace egl {

Display::Display(EGLDisplay display)
    : raii::AutoDeletable<EGLDisplay>(
	  display, [](auto display) { release<Display>(display); }) {}

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
  // TODO: Change EGL_NO_CONTEXT as default value
  if (context != EGL_NO_CONTEXT) {
    throw std::runtime_error("ERROR: egl::Context allows only EGL_NO_CONTEXT");
  }
}

ImageKHR::ImageKHR(Display display, EGLImageKHR image)
    : raii::AutoDeletable<EGLImageKHR>(image, [display](auto image) {
	static auto eglDestroyImageKHR =
	    (PFNEGLDESTROYIMAGEKHRPROC)eglGetProcAddress("eglDestroyImageKHR");
	if (!eglDestroyImageKHR) {
	  throw std::runtime_error("ERROR: EGLImageKHR Not supproted");
	}
	eglDestroyImageKHR(display, image);
      }) {}

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

auto createPlatformWindowSurfaceExt_(Display const &dpy, EGLConfig config,
				     void *win, const EGLint *attrib_list)
    -> Surface {

  static auto eglCreatePlatformWindowSurfaceEXT =
      (PFNEGLCREATEPLATFORMWINDOWSURFACEEXTPROC)eglGetProcAddress(
	  "eglCreatePlatformWindowSurfaceEXT");
  if (!eglCreatePlatformWindowSurfaceEXT) {
    throw std::runtime_error(
	"EGL doesn't support eglCreatePlatformWindowSurfaceEXT");
  }
  // auto surface =
  // eglCreatePlatformWindowSurfaceEXT(dpy, config, win, attrib_list);
  auto surface = eglCreateWindowSurface(dpy, config, (EGLNativeWindowType)win,
					attrib_list);
  return Surface(dpy, surface);
}

// CREATE
auto createContext(Display dpy, EGLConfig config, Context share_context,
		   const EGLint *attrib_list) -> Context {
  auto context = eglCreateContext(dpy, config, share_context, attrib_list);
  return Context(dpy, context);
}

auto createImageKHR(Display const &dpy, EGLContext ctx, EGLenum target,
		    EGLClientBuffer buffer, const EGLint *attrib_list)
    -> ImageKHR {
  static auto eglCreateImageKHR =
      (PFNEGLCREATEIMAGEKHRPROC)eglGetProcAddress("eglCreateImageKHR");
  if (!eglCreateImageKHR) {
    throw std::runtime_error("EGL doesn't support eglCreateImageKHR");
  }
  auto image = eglCreateImageKHR(dpy, ctx, target, buffer, attrib_list);
  return ImageKHR(dpy, image);
}

} // namespace egl

