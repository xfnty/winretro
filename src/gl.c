#include "gl.h"

g_gl_t g_gl;

#define OPENGL_EXT_API_DECL_LIST \
    _X(ptr,  wglCreateContextAttribsARB, ptr hdc, ptr share, i32 *attrs) \
    _X(u32,  wglSwapIntervalEXT,         i32 interval) \
    _X(u32,  glGenFramebuffers,          i64 n, u32 *array)

#define _X(_ret, _name, _arg1, ...)  _ret (WINAPI *_name)(_arg1, ##__VA_ARGS__);
OPENGL_EXT_API_DECL_LIST
#undef _X

u8 init_gl(ptr hwnd, gl_params_t params)
{
    free_gl();

    g_gl.params = params;
    g_gl.window = hwnd;
    g_gl.device_context = GetDC(g_gl.window);
    checkp_goto(g_gl.device_context, Failure, "GetDC() failed (%d)", GetLastError());
    PIXELFORMATDESCRIPTOR pfd = {0};
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.iLayerType = PFD_MAIN_PLANE;
    int pixel_format = ChoosePixelFormat(g_gl.device_context, &pfd);
    checkp_goto(DescribePixelFormat(g_gl.device_context, pixel_format, sizeof(pfd), &pfd), Failure, "DescribePixelFormat() failed (%d)", GetLastError());
    checkp_goto(SetPixelFormat(g_gl.device_context, pixel_format, &pfd), Failure, "SetPixelFormat() failed (%d)", GetLastError());
    ptr tmp_gl = wglCreateContext(g_gl.device_context);
    checkp_goto(tmp_gl, Failure, "wglCreateContext() failed (%d)", GetLastError());
    checkp_goto(wglMakeCurrent(g_gl.device_context, tmp_gl), Failure, "wglMakeCurrent() failed (%d)", GetLastError());

    #define _X(_ret, _name, _arg1, ...) checkp_goto(_name = wglGetProcAddress(#_name), Failure, "wglGetProcAddress(\"%s\") failed (%d)", #_name, GetLastError());
    OPENGL_EXT_API_DECL_LIST
    #undef _X

    checkp_goto(wglMakeCurrent(0, 0), Failure, "wglMakeCurrent() failed (%d)", GetLastError());
    checkp_goto(wglDeleteContext(tmp_gl), Failure, "wglDeleteContext() failed (%d)", GetLastError());
    g_gl.opengl_context = wglCreateContextAttribsARB(g_gl.device_context, 0, (i32[]){
        WGL_CONTEXT_MAJOR_VERSION_ARB, params.version_major,
        WGL_CONTEXT_MINOR_VERSION_ARB, params.version_minor,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        (params.debug) ? (WGL_CONTEXT_FLAGS_ARB) : (0), WGL_CONTEXT_DEBUG_BIT_ARB,
        0
    });
    checkp_goto(g_gl.opengl_context, Failure, "wglCreateContextAttribsARB() failed (%d)", glGetError());
    checkp_goto(wglMakeCurrent(g_gl.device_context, g_gl.opengl_context), Failure, "wglMakeCurrent() failed (%d)", GetLastError());
    checkp_goto(wglSwapIntervalEXT(1), Failure, "wglSwapIntervalEXT() failed (%d)", GetLastError());

    print("using OpenGL %s on %s", glGetString(GL_VERSION), glGetString(GL_RENDERER));
    return true;

    Failure:
    free_gl();
    print("init_gl() failed");
    return false;
}

void free_gl(void)
{
    wglMakeCurrent(0, 0);
    wglDeleteContext(g_gl.opengl_context);
    ReleaseDC(g_gl.window, g_gl.device_context);
    RtlZeroMemory(&g_gl, sizeof(g_gl));
}
