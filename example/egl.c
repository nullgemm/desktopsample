#include "globox.h"
#include "globox_private_getters.h"

#include "willis.h"
#include "cursoryx.h"
#include "dpishit.h"

#include <stddef.h>
#include <stdio.h>

#if defined(GLOBOX_PLATFORM_WINDOWS)
#include <GL/gl.h>
#define GL_GLES_PROTOTYPES 0
#endif

#include <GLES2/gl2.h>

#define VERTEX_ATTR_POSITION 0

#if defined(GLOBOX_PLATFORM_WINDOWS)
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLCREATESHADERPROC glCreateShader;
PFNGLDELETESHADERPROC glDeleteShader;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;

// opengl32.dll only supports OpenGL 1
// so we have to load these functions
void wgl_load()
{
	glAttachShader =
		(PFNGLATTACHSHADERPROC)
			wglGetProcAddress("glAttachShader");
	glCompileShader =
		(PFNGLCOMPILESHADERPROC)
			wglGetProcAddress("glCompileShader");
	glCreateProgram =
		(PFNGLCREATEPROGRAMPROC)
			wglGetProcAddress("glCreateProgram");
	glCreateShader =
		(PFNGLCREATESHADERPROC)
			wglGetProcAddress("glCreateShader");
	glDeleteShader =
		(PFNGLDELETESHADERPROC)
			wglGetProcAddress("glDeleteShader");
	glEnableVertexAttribArray =
		(PFNGLENABLEVERTEXATTRIBARRAYPROC)
			wglGetProcAddress("glEnableVertexAttribArray");
	glLinkProgram =
		(PFNGLLINKPROGRAMPROC)
			wglGetProcAddress("glLinkProgram");
	glShaderSource =
		(PFNGLSHADERSOURCEPROC)
			wglGetProcAddress("glShaderSource");
	glUseProgram =
		(PFNGLUSEPROGRAMPROC)
			wglGetProcAddress("glUseProgram");
	glVertexAttribPointer =
		(PFNGLVERTEXATTRIBPOINTERPROC)
			wglGetProcAddress("glVertexAttribPointer");
}
#endif

void render(struct globox* globox)
{
	globox_platform_events_handle(
		globox);

	if (globox_error_catch(globox))
	{
		return;
	}

	if (globox_get_redraw(globox) == true)
	{
		int32_t width = globox_get_width(globox);
		int32_t height = globox_get_height(globox);
		GLint viewport_rect[4];

		// we can make OpenGL 1 calls without any loader
		glGetIntegerv(GL_VIEWPORT, viewport_rect);

		if ((viewport_rect[2] != width) || (viewport_rect[3] != height))
		{
			glViewport(0, 0, width, height);
		}

		glClearColor(0.2f, 0.4f, 0.9f, (0x22 / 255.0f));
		glClear(GL_COLOR_BUFFER_BIT);

		GLfloat vertices[] =
		{
			-100.0f / width, +100.0f / height, 1.0f,
			-100.0f / width, -100.0f / height, 1.0f,
			+100.0f / width, -100.0f / height, 1.0f,
			+100.0f / width, +100.0f / height, 1.0f,
		};

		glEnableVertexAttribArray(VERTEX_ATTR_POSITION);

		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			vertices);

		glDrawArrays(
			GL_TRIANGLE_FAN,
			0,
			4);

		globox_context_egl_copy(
			globox,
			0,
			0,
			width,
			height);
	}
}

void event(
	struct willis* willis,
	enum willis_event_code event_code,
	enum willis_event_state event_state,
	void* data)
{
	printf(
		"%s\n%s\n",
		willis_event_code_names[event_code],
		willis_event_state_names[event_state]);

	if ((event_code == WILLIS_KEY_Q)
	&& (event_state == WILLIS_STATE_PRESS))
	{
		willis_mouse_grab(willis);
	}

	if ((event_code == WILLIS_KEY_W)
	&& (event_state == WILLIS_STATE_PRESS))
	{
		willis_mouse_ungrab(willis);
	}

	char* utf8_string = willis_get_utf8_string(willis);

	if (utf8_string != NULL)
	{
		printf(
			"%s\n",
			utf8_string);
	}

	if (event_code == WILLIS_MOUSE_MOTION)
	{
		if (willis_get_mouse_grab(willis) == true)
		{
			printf(
				"diff: %x %x\n",
				(uint32_t) (willis_get_diff_x(willis) >> 32),
				(uint32_t) (willis_get_diff_y(willis) >> 32));
		}
		else
		{
			printf(
				"pos: %i %i\n",
				willis_get_mouse_x(willis),
				willis_get_mouse_y(willis));
		}
	}

	printf("\n");
}

int main(void)
{
	struct globox globox = {0};
	struct willis willis = {0};
	struct cursoryx cursoryx = {0};
	struct dpishit dpishit = {0};
	void* willis_backend_link = NULL;
	void* cursoryx_backend_link = NULL;

	// start initializing globox
	globox_open(
		&globox,
		0,
		0,
		500,
		500,
		"globox",
		GLOBOX_STATE_REGULAR,
		willis_handle_events,
		&willis);

	if (globox_error_catch(&globox))
	{
		return 1;
	}

	globox_platform_init(&globox, true, false, true);

	if (globox_error_catch(&globox))
	{
		globox_close(&globox);
		return 1;
	}

	// use OpenGL 2 or glES 2
	globox_context_egl_init(&globox, 2, 0);

	if (globox_error_catch(&globox))
	{
		globox_platform_free(&globox);
		globox_close(&globox);
		return 1;
	}

	globox_platform_create_window(&globox);

	if (globox_error_catch(&globox))
	{
		globox_platform_free(&globox);
		globox_close(&globox);
		return 1;
	}

	// initialize dpishit
#if defined(DPISHIT_X11)
	struct dpishit_data_x11 dpishit_data =
	{
		.conn = globox_get_x11_conn(&globox),
		.win = globox_get_x11_win(&globox),
	};
#elif defined(DPISHIT_WAYLAND)
	struct dpishit_data_wayland dpishit_data =
	{
		.listener = globox_get_wayland_output_listener(&globox),
		.output_data = globox_get_wayland_output_data(&globox),
	};
#elif defined(DPISHIT_WINDOWS)
	struct dpishit_data_windows dpishit_data =
	{
		.hdc = GetDC(globox_platform_get_event_handle(&globox)),
		.hwnd = globox_platform_get_event_handle(&globox),
	};
#elif defined(DPISHIT_MACOS)
	struct dpishit_data_macos dpishit_data =
	{
		.window_obj = globox_get_macos_obj_window(&globox),
	};
#endif

	dpishit_init(
		&dpishit,
		&dpishit_data);

	// finish initializing globox
	globox_context_egl_create(&globox);

	if (globox_error_catch(&globox))
	{
		globox_context_egl_free(&globox);
		globox_platform_free(&globox);
		globox_close(&globox);
		return 1;
	}

	globox_platform_hooks(&globox);

	if (globox_error_catch(&globox))
	{
		globox_context_egl_free(&globox);
		globox_platform_free(&globox);
		globox_close(&globox);
		return 1;
	}

#if defined(GLOBOX_PLATFORM_WINDOWS)
	// load OpenGL functions
	wgl_load();
#endif

	// prepare OpenGL or glES
	const char* vertex_shader_src =
		"#version 130\n"
		"attribute vec4 vPosition;"
		"void main()"
		"{"
		"\tgl_Position = vPosition;"
		"}";

	const char* fragment_shader_src =
		"#version 130\n"
		"precision mediump float;"
		"void main()"
		"{"
		"\tgl_FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);"
		"}";

	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_src, 0);
	glCompileShader(vertex_shader);

	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_src, 0);
	glCompileShader(fragment_shader);

	GLuint shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	glLinkProgram(shader_program);
	glUseProgram(shader_program);

	// continue initializing globox
	globox_platform_commit(&globox);

	render(&globox);

	// willis
#if defined(WILLIS_X11)
	struct willis_data_x11 willis_data =
	{
		.x11_conn =
			globox_get_x11_conn(&globox),
		.x11_root =
			globox_get_x11_root_win(&globox),
		.x11_window =
			globox_get_x11_win(&globox),
	};

	willis_backend_link = &willis_data;
#elif defined(WILLIS_WAYLAND)
	struct willis_data_wayland willis_data =
	{
		.wl_surface =
			globox_get_wayland_surface(&globox),
		.wl_relative_pointer =
			globox_get_wayland_pointer_manager(&globox),
		.wl_pointer_constraints =
			globox_get_wayland_pointer_constraints(&globox),
	};

	willis_backend_link = &willis_data;
#elif defined(WILLIS_WINDOWS)
	willis_backend_link = NULL;
#elif defined(WILLIS_MACOS)
	willis_backend_link = NULL;
#endif

	willis_init(
		&willis,
		willis_backend_link,
		true,
		event,
		NULL);

	// cursoryx
#if defined(CURSORYX_X11)
	struct cursoryx_data_x11 cursoryx_data =
	{
		.conn = globox_get_x11_conn(&globox),
		.window = globox_get_x11_win(&globox),
		.screen = globox_get_x11_screen_obj(&globox),
	};

	cursoryx_backend_link = &cursoryx_data;
#elif defined(CURSORYX_WAYLAND)
	struct cursoryx_data_wayland cursoryx_data =
	{
		.compositor = globox_get_wayland_compositor(&globox),
		.pointer = willis.willis_wayland.wl_pointer, // TODO getters
		.shm = globox_get_wayland_shm(&globox),
	};

	cursoryx_backend_link = &cursoryx_data;
#elif defined(CURSORYX_WINDOWS)
	struct cursoryx_data_windows cursoryx_data =
	{
		.handle = globox_platform_get_event_handle(&globox),
	};

	cursoryx_backend_link = &cursoryx_data;
#elif defined(CURSORYX_MACOS)
	cursoryx_backend_link = NULL;
#endif

	cursoryx_start(
		&cursoryx,
		cursoryx_backend_link);

	// main loop
	struct dpishit_display_info* display_info;
	bool dpishit_real;
	bool dpishit_logic;
	bool dpishit_scale;

	globox_platform_prepoll(&globox);

	if (globox_error_catch(&globox))
	{
		globox_context_egl_free(&globox);
		globox_platform_free(&globox);
		globox_close(&globox);
		return 1;
	}

	while (globox_get_closed(&globox) == false)
	{
		cursoryx_set(
			&cursoryx,
			CURSORYX_HAND);

		globox_platform_events_poll(&globox);

		if (globox_error_catch(&globox))
		{
			globox_context_egl_free(&globox);
			globox_platform_free(&globox);
			globox_close(&globox);
			return 1;
		}

		render(&globox);

		if (globox_error_catch(&globox))
		{
			globox_context_egl_free(&globox);
			globox_platform_free(&globox);
			globox_close(&globox);
			return 1;
		}

		dpishit_real = dpishit_refresh_real_density(&dpishit);
		dpishit_logic = dpishit_refresh_logic_density(&dpishit);
		dpishit_scale = dpishit_refresh_scale(&dpishit);
		display_info = dpishit_get_display_info(&dpishit);

		printf(
			"%u %u %u %u %lf %lf %d %d %d\n",
			display_info->px_width,
			display_info->px_height,
			display_info->mm_width,
			display_info->mm_height,
			display_info->dpi_logic,
			display_info->scale,
			dpishit_real,
			dpishit_logic,
			dpishit_scale);
	}

	cursoryx_stop(&cursoryx);
	willis_free(&willis);
	globox_context_egl_free(&globox);
	globox_platform_free(&globox);
	globox_close(&globox);

	return 0;
}
