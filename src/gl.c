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
#define GL_COLOR_BUFFER_BIT                       0x00004000
#define GL_ARRAY_BUFFER                           0x8892
#define GL_TEXTURE_2D                             0x0DE1
#define GL_FRAMEBUFFER_COMPLETE                   0x8CD5
#define GL_FRAMEBUFFER                            0x8D40
#define GL_STATIC_DRAW                            0x88E4
#define GL_COLOR_ATTACHMENT0                      0x8CE0
#define GL_FRAGMENT_SHADER                        0x8B30
#define GL_VERTEX_SHADER                          0x8B31
#define GL_LINK_STATUS                            0x8B82
#define GL_TEXTURE_MAG_FILTER                     0x2800
#define GL_TEXTURE_MIN_FILTER                     0x2801
#define GL_TRIANGLES                              0x0004
#define GL_COMPILE_STATUS                         0x8B81
#define GL_NEAREST                                0x2600
#define GL_LINEAR                                 0x2601
#define GL_FLOAT                                  0x1406
#define GL_RGB                                    0x1907
#define GL_RGBA8                                  0x8058
#define GL_BGRA                                   0x80E1
#define GL_UNSIGNED_BYTE                          0x1401
#define GL_UNSIGNED_INT_8_8_8_8_REV               0x8367

#define OPENGL_API_DECL_LIST \
    _X(void, glClear,                   u32 mask) \
    _X(void, glClearColor,              f32 r, f32 g, f32 b, f32 a) \
    _X(void, glViewport,                i32 x, i32 y, i32 w, i32 h) \
    _X(void, glGenBuffers,              i32 count, u32 *vbos) \
    _X(void, glBindBuffer,              u32 target, u32 vbo) \
    _X(void, glBufferData,              u32 target, i32 size, ptr buf, u32 usage) \
    _X(void, glDeleteBuffers,           i32 count, u32 *vbos) \
    _X(void, glDeleteVertexArrays,      i32 count, u32 *vaos) \
    _X(u32,  glCreateShader,            u32 type) \
    _X(void, glShaderSource,            u32 shader, i32 count, cstr *srcs, i32 *src_lengths) \
    _X(void, glCompileShader,           u32 shader) \
    _X(void, glGetShaderiv,             u32 shader, u32 pname, i32 *params) \
    _X(void, glGetShaderInfoLog,        u32 shader, i32 maxlen, i32 *len, c8 *log) \
    _X(void, glDeleteShader,            u32 shader) \
    _X(u32,  glCreateProgram,           void) \
    _X(void, glAttachShader,            u32 sp, u32 shader) \
    _X(void, glLinkProgram,             u32 sp) \
    _X(void, glGetProgramiv,            u32 sp, u32 pname, i32 *params) \
    _X(void, glGetProgramInfoLog,       u32 sp, i32 maxlen, i32 *len, c8 *log) \
    _X(void, glUseProgram,              u32 sp) \
    _X(void, glDeleteProgram,           u32 sp) \
    _X(void, glGenFramebuffers,         i32 count, u32 *fbos) \
    _X(void, glDeleteTextures,          i32 count, u32 *textures) \
    _X(void, glDeleteFramebuffers,      i32 count, u32 *fbos) \
    _X(void, glBindFramebuffer,         u32 target, u32 fbo) \
    _X(void, glFramebufferTexture2D,    u32 target, u32 attachment, u32 textarget, u32 tex, i32 level) \
    _X(u32,  glCheckFramebufferStatus,  u32 target) \
    _X(void, glGenTextures,             i32 count, u32 *textures) \
    _X(void, glBindTexture,             u32 target, u32 tex) \
    _X(void, glTexParameteri,           u32 target, u32 id, i32 value) \
    _X(void, glTexImage2D,              u32 target, i32 level, i32 ifmt, i32 w, i32 h, i32 border, i32 fmt, u32 type, const ptr data) \
    _X(void, glDrawArrays,              u32 mode, i32 first, i32 count) \
    _X(void, glGenVertexArrays,         i32 size, u32 *vaos) \
    _X(void, glBindVertexArray,         u32 array) \
    _X(void, glVertexAttribPointer,     u32 idx, i32 size, u32 type, u8 normalized, i32 stride, const ptr p) \
    _X(void, glEnableVertexAttribArray, u32 idx)

static struct {
    state_t state;
    ptr dev;
    ptr mod;
    ptr ctx;
    u32 window_width;
    u32 window_height;
    u32 render_width;
    u32 render_height;
    u32 max_render_width;
    u32 max_render_height;
    struct {
        #define _X(_r, _n, _a1, ...) _r (*_n)(_a1, ##__VA_ARGS__);
        OPENGL_API_DECL_LIST
        #undef _X
    } api;
    u32 vbo;
    u32 vao;
    u32 sp;
    u32 tex;
    u32 fbo;
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

static u32 compile_shader(u32 type, cstr src);
static void update_frame_vertices(void);

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

    struct { ptr *func; cstr name; } load_list[] = {
        #define _X(_r, _n, _a1, ...) { (ptr*)&g_gl.api._n, #_n },
        OPENGL_API_DECL_LIST
        #undef _X
    };
    for (u32 i = 0; i < countof(load_list); i++)
    {
        *load_list[i].func = wglGetProcAddress(load_list[i].name);
        if (!*load_list[i].func)
        {
            assert_winapi_retval_report(
                *load_list[i].func,
                *load_list[i].func,
                GetProcAddress,
                "%p, \"%s\"",
                g_gl.mod,
                load_list[i].name
            );
        }
    }

    print("using OpenGL %s on %s", glGetString(GL_VERSION), glGetString(GL_RENDERER));

    g_gl.state = STATE_INITIALIZED;
}

void configure_gl(u32 max_width, u32 max_height)
{
    assert_report(g_gl.state >= STATE_INITIALIZED);

    g_gl.api.glDeleteBuffers(1, &g_gl.vbo);
    g_gl.api.glDeleteVertexArrays(1, &g_gl.vao);
    g_gl.api.glDeleteProgram(g_gl.sp);
    g_gl.api.glDeleteFramebuffers(1, &g_gl.fbo);
    g_gl.api.glDeleteTextures(1, &g_gl.tex);
    g_gl.vbo = g_gl.vao = g_gl.sp = g_gl.fbo = g_gl.tex = 0;

    g_gl.max_render_width = max_width;
    g_gl.max_render_height = max_height;

    assert_glapi(g_gl.api.glGenVertexArrays, "%d, %p", 1, &g_gl.vao);
    assert_glapi(g_gl.api.glBindVertexArray, "%u", g_gl.vao);

    assert_glapi(g_gl.api.glGenBuffers, "%d, %p", 1, &g_gl.vbo);

    set_gl_window_size(g_gl.max_render_width, g_gl.max_render_height);
    set_gl_render_resolution(g_gl.max_render_width, g_gl.max_render_height);
    update_frame_vertices();

    assert_glapi(g_gl.api.glVertexAttribPointer, "%u, %d, %u, %hhu, %d, %p", 0, 2, GL_FLOAT, 0, sizeof(f32) * 4, 0);
    assert_glapi(g_gl.api.glEnableVertexAttribArray, "%u", 0);
    assert_glapi(g_gl.api.glVertexAttribPointer, "%u, %d, %u, %hhu, %d, %p", 1, 2, GL_FLOAT, 0, sizeof(f32) * 4, (ptr)(sizeof(f32) * 2));
    assert_glapi(g_gl.api.glEnableVertexAttribArray, "%u", 1);

    assert_glapi(g_gl.api.glBindVertexArray, "%u", 0);
    assert_glapi(g_gl.api.glBindBuffer, "%u, %u", GL_ARRAY_BUFFER, 0);

    u32 vs = compile_shader(
        GL_VERTEX_SHADER,
        "#version 330 core\n"
        "layout (location = 0) in vec2 xy;\n"
        "layout (location = 1) in vec2 uv;\n"
        "out vec2 uv_out;\n"
        "void main() {\n"
        "    uv_out = uv;\n"
        "    gl_Position = vec4(xy.x, xy.y, 0.0, 1.0); \n"
        "}"
    );

    u32 fs = compile_shader(
        GL_FRAGMENT_SHADER,
        "#version 330 core\n"
        "in vec2 uv_out;\n"
        "out vec4 FragColor;\n"
        "uniform sampler2D tex;\n"
        "void main() {\n"
        "    FragColor = texture(tex, uv_out);\n"
        "} "
    );

    assert_glapi(g_gl.sp = g_gl.api.glCreateProgram, "");
    assert_glapi(g_gl.api.glAttachShader, "%u, %u", g_gl.sp, vs);
    assert_glapi(g_gl.api.glAttachShader, "%u, %u", g_gl.sp, fs);
    assert_glapi(g_gl.api.glLinkProgram, "%u", g_gl.sp);
    i32 linked = false;
    assert_glapi(g_gl.api.glGetProgramiv, "%u, %u, %p", g_gl.sp, GL_LINK_STATUS, &linked);
    if (!linked)
    {
        c8 log[512];
        assert_glapi(g_gl.api.glGetProgramInfoLog, "%u, %d, %p, %p", g_gl.sp, sizeof(log), 0, log);
        print_error(__FILE__, __LINE__, "failed to link shader program:\n%s", log);
        crash();
    }
    assert_glapi(g_gl.api.glDeleteShader, "%u", vs);
    assert_glapi(g_gl.api.glDeleteShader, "%u", fs);

    assert_glapi(g_gl.api.glGenTextures, "%d, %p", 1, &g_gl.tex);
    assert_glapi(g_gl.api.glBindTexture, "%u, %u", GL_TEXTURE_2D, g_gl.tex);
    assert_glapi(g_gl.api.glTexParameteri, "%u, %u, %d", GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    assert_glapi(g_gl.api.glTexParameteri, "%u, %u, %d", GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    assert_glapi(
        g_gl.api.glTexImage2D, "%u, %d, %d, %d, %d, %d, %d, %u, %p",
        GL_TEXTURE_2D, 0, GL_RGB, max_width, max_height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0
    );
    assert_glapi(g_gl.api.glBindTexture, "%u, %u", GL_TEXTURE_2D, 0);

    assert_glapi(g_gl.api.glGenFramebuffers, "%d, %p", 1, &g_gl.fbo);
    assert_glapi(g_gl.api.glBindFramebuffer, "%u, %u", GL_FRAMEBUFFER, g_gl.fbo);
    assert_glapi(g_gl.api.glFramebufferTexture2D, "%u, %u, %u, %u, %d", GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, g_gl.tex, 0);
    assert_print_error(g_gl.api.glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "framebuffer readiness check failed (%d, %x)", glGetError(), g_gl.api.glCheckFramebufferStatus(GL_FRAMEBUFFER));
    assert_glapi(g_gl.api.glBindFramebuffer, "%u, %u", GL_FRAMEBUFFER, 0);

    print("GL configured for %ux%u mode", max_width, max_height);
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
    return g_gl.fbo;
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
    check_return(g_gl.state == STATE_ACTIVE);

    assert_glapi(g_gl.api.glBindFramebuffer, "%u, %u", GL_FRAMEBUFFER, 0);
    assert_glapi(g_gl.api.glViewport, "%d, %d, %d, %d", 0, 0, g_gl.window_width, g_gl.window_height);
    assert_glapi(g_gl.api.glClearColor, "%f, %f, %f, %f", 0, 0, 0, 0);
    assert_glapi(g_gl.api.glClear, "%u", GL_COLOR_BUFFER_BIT);

    assert_glapi(g_gl.api.glBindBuffer, "%u, %u", GL_ARRAY_BUFFER, g_gl.vbo);
    assert_glapi(g_gl.api.glBindVertexArray, "%u", g_gl.vao);
    assert_glapi(g_gl.api.glBindTexture, "%u, %u", GL_TEXTURE_2D, g_gl.tex);
    assert_glapi(g_gl.api.glUseProgram, "%u", g_gl.sp);
    assert_glapi(g_gl.api.glDrawArrays, "%u, %d, %d", GL_TRIANGLES, 0, 6);

    /* FIXME: works, but says "invalid handle value" */
    // assert_winapi_report(SwapBuffers, "%p", g_gl.dev);
    
    SwapBuffers(g_gl.dev);
}

void set_gl_window_size(u32 w, u32 h)
{
    assert_report(g_gl.state >= STATE_INITIALIZED);
    if (g_gl.window_width != w || g_gl.window_height != h)
    {
        g_gl.window_width = w;
        g_gl.window_height = h;
        update_frame_vertices();
    }
}

void set_gl_render_resolution(u32 w, u32 h)
{
    assert_report(g_gl.state >= STATE_INITIALIZED);
    if (g_gl.render_width != w || g_gl.render_height != h)
    {
        g_gl.render_width = w;
        g_gl.render_height = h;
        update_frame_vertices();
    }
}

u32 compile_shader(u32 type, cstr src)
{
    u32 shader;
    assert_glapi(shader = g_gl.api.glCreateShader, "%u", type);
    assert_glapi(g_gl.api.glShaderSource, "%u, %i, %p, %p", shader, 1, &src, 0);
    assert_glapi(g_gl.api.glCompileShader, "%u", shader);
    i32 compiled = false;
    assert_glapi(g_gl.api.glGetShaderiv, "%u, %u, %p", shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
    {
        c8 log[512];
        assert_glapi(g_gl.api.glGetShaderInfoLog, "%u, %d, %p, %p", shader, sizeof(log), 0, log);
        print_error(__FILE__, __LINE__, "failed to compile shader type %x:\n%s\n%s", type, log, src);
        crash();
    }
    return shader;
}

void update_frame_vertices(void)
{
    // TODO: letterboxing
    f32 u = g_gl.render_width / (f32)g_gl.max_render_width;
    f32 v = g_gl.render_height / (f32)g_gl.max_render_height;
    f32 verts[] = {
        -1,  1, 0, v,
         1,  1, u, v, 
         1, -1, u, 0, 
        -1,  1, 0, v,
         1, -1, u, 0, 
        -1, -1, 0, 0, 
    };
    assert_glapi(g_gl.api.glBindBuffer, "%u, %u", GL_ARRAY_BUFFER, g_gl.vbo);
    assert_glapi(g_gl.api.glBufferData, "%u, %d, %p, %u", GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
}
