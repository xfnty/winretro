#include "gl.h"

#include "error.h"
#include "windows.h"

#define GL_RENDERER                               0x1F01
#define GL_VERSION                                0x1F02
#define WGL_CONTEXT_MAJOR_VERSION_ARB             0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB             0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB              0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB          0x00000001
#define WGL_CONTEXT_FLAGS_ARB                     0x2094
#define WGL_CONTEXT_DEBUG_BIT_ARB                 0x00000001

static struct {
    state_t state;
    ptr dev;
    ptr mod;
    ptr ctx;
} g_gl;

u32 SwapBuffers(ptr dev);
ptr WINAPI wglCreateContext(ptr dev);
u32 WINAPI wglDeleteContext(ptr ctx);
ptr WINAPI wglGetProcAddress(cstr name);
u32 WINAPI wglMakeCurrent(ptr dev, ptr ctx);
u32 WINAPI glGetError(void);
c8* WINAPI glGetString(u32 id);

#define assert_glapi(_func, _args_fmt, ...) do { \
        _func(__VA_ARGS__); \
        i32 _e = glGetError(); \
        if (_e) { \
            print_error(__FILE__, __LINE__, #_func "(" _args_fmt ") failed (%x)", ##__VA_ARGS__, _e); \
            crash(); \
        } \
    } while(0)

void init_gl(ptr dev)
{
    assert_report(dev);
    free_gl();

    g_gl.dev = dev;
    assert_winapi_retval_report(g_gl.mod, g_gl.mod, LoadLibraryA, "\"%s\"", "opengl32.dll");

    u32 e = 0;
    PIXELFORMATDESCRIPTOR pfd = {0};
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.iLayerType = PFD_MAIN_PLANE;
    int pf = 0;
    assert_winapi_retval_report(pf, pf, ChoosePixelFormat, "%p, %p", g_gl.dev, &pfd);
    assert_winapi_retval_report(e, e, DescribePixelFormat, "%p, %d, %llu, %p", g_gl.dev, pf, sizeof(pfd), &pfd);
    assert_winapi_retval_report(e, e, SetPixelFormat, "%p, %d, %p", g_gl.dev, pf, &pfd);

    ptr tmp_ctx = 0;
    assert_winapi_retval_report(tmp_ctx, tmp_ctx, wglCreateContext, "%p", g_gl.dev);
    assert_winapi_retval_report(e, e, wglMakeCurrent, "%p, %p", g_gl.dev, tmp_ctx);

    ptr (WINAPI *wglCreateContextAttribsARB)(ptr hdc, ptr share, i32 *attrs);
    assert_winapi_retval_report(wglCreateContextAttribsARB, wglCreateContextAttribsARB, wglGetProcAddress, "\"%s\"", "wglCreateContextAttribsARB");

    assert_winapi_retval_report(e, e, wglMakeCurrent, "%p, %p", 0, 0);
    assert_winapi_retval_report(e, e, wglDeleteContext, "%p", tmp_ctx);

    g_gl.ctx = wglCreateContextAttribsARB(g_gl.dev, 0, (i32[]){
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    });
    assert_print_error(g_gl.ctx, "wglCreateContextAttribsARB() failed (%u)", glGetError());
    assert_winapi_retval_report(e, e, wglMakeCurrent, "%p, %p", g_gl.dev, g_gl.ctx);

    print("using OpenGL %s on %s", glGetString(GL_VERSION), glGetString(GL_RENDERER));

    g_gl.state = STATE_INITIALIZED;
}

void configure_gl(u32 max_width, u32 max_height)
{
    (void)max_width; (void)max_height;
    g_gl.state = STATE_ACTIVE;
}

void free_gl(void)
{
    wglDeleteContext(g_gl.ctx);
    RtlZeroMemory(&g_gl, sizeof(g_gl));
    SetLastError(0);
}

u64 get_gl_framebuffer(void)
{
    assert_print_error(g_gl.state == STATE_ACTIVE, "called get_gl_framebuffer() when GL was not configured");
    return 0;
}

ptr get_gl_proc_address(cstr name)
{
    assert_print_error(g_gl.state >= STATE_INITIALIZED, "called get_gl_proc_address() when GL was not initialized");
    ptr f = wglGetProcAddress(name);
    if (!f) check_winapi_retval_report(f, f, GetProcAddress, "%p, \"%s\"", g_gl.mod, name);
    return f;
}

void present_frame(void)
{
    /* FIXME: works, but says "invalid handle value" */
    // assert_winapi_report(SwapBuffers, "%p", g_gl.dev);
    SwapBuffers(g_gl.dev);
}
