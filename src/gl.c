#include "gl.h"

#include "assert.h"
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

#define assert_gl(_f, _fmt_args, ...) do { \
        _f(__VA_ARGS__); \
        i32 _e = glGetError(); \
        if (_e) { print(#_f "(" _fmt_args ") failed (%x) [%s():%d]", ##__VA_ARGS__, _e, __func__, __LINE__); crash(); } \
    } while(0)

void init_gl(ptr dev)
{
    assert(dev);
    free_gl();

    g_gl.dev = dev;
    assert_winapi(g_gl.mod, g_gl.mod, LoadLibraryA, "\"%s\"", "opengl32.dll");

    u32 e = 0;
    PIXELFORMATDESCRIPTOR pfd = {0};
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.iLayerType = PFD_MAIN_PLANE;
    int pf = 0;
    assert_winapi(pf, pf, ChoosePixelFormat, "%p, %p", g_gl.dev, &pfd);
    assert_winapi(e, e, DescribePixelFormat, "%p, %d, %llu, %p", g_gl.dev, pf, sizeof(pfd), &pfd);
    assert_winapi(e, e, SetPixelFormat, "%p, %d, %p", g_gl.dev, pf, &pfd);

    ptr tmp_ctx = 0;
    assert_winapi(tmp_ctx, tmp_ctx, wglCreateContext, "%p", g_gl.dev);
    assert_winapi(e, e, wglMakeCurrent, "%p, %p", g_gl.dev, tmp_ctx);

    ptr (WINAPI *wglCreateContextAttribsARB)(ptr hdc, ptr share, i32 *attrs);
    assert_winapi(wglCreateContextAttribsARB, wglCreateContextAttribsARB, wglGetProcAddress, "\"%s\"", "wglCreateContextAttribsARB");

    assert_winapi(e, e, wglMakeCurrent, "%p, %p", 0, 0);
    assert_winapi(e, e, wglDeleteContext, "%p", tmp_ctx);

    g_gl.ctx = wglCreateContextAttribsARB(g_gl.dev, 0, (i32[]){
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    });
    assertp(g_gl.ctx, "wglCreateContextAttribsARB() failed (%u)", glGetError());
    assert_winapi(e, e, wglMakeCurrent, "%p, %p", g_gl.dev, g_gl.ctx);

    print("using OpenGL %s on %s", glGetString(GL_VERSION), glGetString(GL_RENDERER));

    g_gl.state = STATE_INITIALIZED;
}

void configure_gl(u32 max_width, u32 max_height)
{
    print("configure_gl(%u, %u)", max_width, max_height);
    g_gl.state = STATE_ACTIVE;
}

void free_gl(void)
{
    g_gl.state = STATE_UNINITIALIZED;
}

u64 get_gl_framebuffer(void)
{
    assertp(g_gl.state == STATE_ACTIVE, "called get_gl_framebuffer() when GL was not configured");
    return 0;
}

ptr get_gl_proc_address(cstr name)
{
    assertp(g_gl.state >= STATE_INITIALIZED, "called get_gl_proc_address() when GL was not initialized");
    ptr f = wglGetProcAddress(name);
    return (f) ? (f) : (GetProcAddress(g_gl.mod, name));
}

void present_frame(void)
{
    u32 e;
    assert_winapi(e, e, SwapBuffers, "%p", g_gl.dev);
}
