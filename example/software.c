#include "globox.h"
#include "globox_private_getters.h"

#include "willis.h"
#include "cursoryx.h"
#include "dpishit.h"
#include "helpers.h"
#include "razorbeard.h"
#include "razorbeard_default_widgets.h"

#include <stddef.h>
#include <stdio.h>

struct event_data
{
	struct rzb* rzb;
	bool* event_post;
};

void render(
	struct globox* globox,
	struct rzb* rzb,
	struct rzb_display_info* display_info,
	bool* event_post)
{
	globox_platform_events_handle(
		globox);

	if (globox_error_catch(globox))
	{
		return;
	}

	if ((globox_get_redraw(globox) == true) || (*event_post == true))
	{
		uint32_t width = globox_get_width(globox);
		uint32_t height = globox_get_height(globox);
		uint32_t* argb = globox_platform_get_argb(globox);

		*event_post = false;
		rzb->argb = argb;
		rzb->argb_width = width;
		rzb->argb_height = height;

		for (uint32_t i = 0; i < height * width; ++i)
		{
			argb[i] = 0xff191919;
		}

		rzb_render_all(rzb);

		globox_context_software_copy(
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

	if ((event_code == WILLIS_KEY_UP)
		|| (event_code == WILLIS_KEY_DOWN)
		|| (event_code == WILLIS_KEY_LEFT)
		|| (event_code == WILLIS_KEY_RIGHT))
	{
		struct event_data* event_data = data;

		rzb_send_event(event_data->rzb, event_code, event_state);
		*(event_data->event_post) = true;
	}

	printf("\n");
}

int main(void)
{
	struct globox globox = {0};
	struct willis willis = {0};
	struct cursoryx cursoryx = {0};
	struct dpishit dpishit = {0};
	struct rzb rzb = {0};
	struct rzb_default_widgets_context kit = {0};
	struct rzb_display_info rzb_display_info = {0};
	void* willis_backend_link = NULL;
	void* cursoryx_backend_link = NULL;
	bool event_post = false;

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

	globox_context_software_init(&globox, 0, 0);

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
	globox_context_software_create(&globox);

	if (globox_error_catch(&globox))
	{
		globox_context_software_free(&globox);
		globox_platform_free(&globox);
		globox_close(&globox);
		return 1;
	}

	globox_platform_hooks(&globox);

	if (globox_error_catch(&globox))
	{
		globox_context_software_free(&globox);
		globox_platform_free(&globox);
		globox_close(&globox);
		return 1;
	}

	globox_platform_commit(&globox);

	// widgets
	struct rzb_widget* widget_handles;
	//struct rzb_widget* widget_pager;
	struct rzb_widget* widget_tabs;
	struct rzb_widget* widget_popup;
	//struct rzb_widget* widget_dropmenu;
	//struct rzb_widget* widget_separator;
	//struct rzb_widget* widget_text;
	//struct rzb_widget* widget_image;
	struct rzb_widget* widget_button;
	struct rzb_widget* widget_button_b;
	struct rzb_widget* widget_button_c;
	struct rzb_widget* widget_numberbox;
	struct rzb_widget* widget_textbox;
	struct rzb_widget* widget_textarea;
	struct rzb_widget* widget_radiobutton;
	struct rzb_widget* widget_checkbox;
	//struct rzb_widget* widget_scrollbar;
	struct rzb_widget* widget_slider;
	struct rzb_widget* widget_progressbar;

	rzb_init(&rzb, &rzb_display_info);

	rzb_default_widgets_init(&kit, NULL);

	char* tabs[6] = {"first", "second", "third", "fourth", "fifth", "sixth"};

	widget_handles =
		rzb_alloc_widget_handles(
			&rzb, layout_demo_handles, &kit, false, 2);

#if 0
	widget_pager =
		rzb_alloc_widget_pager(
			&rzb, NULL, &kit, false);
#endif

	widget_tabs =
		rzb_alloc_widget_tabs(
			&rzb, layout_demo_tabs, &kit, NULL, tabs, 6, 2);

	widget_popup =
		rzb_alloc_widget_popup(
			&rzb, layout_demo_popup, &kit);

#if 0
	widget_dropmenu =
		rzb_alloc_widget_dropmenu(
			&rzb, NULL, &kit);

	widget_separator =
		rzb_alloc_widget_separator(
			&rzb, NULL, &kit);
#endif

	//widget_text =
	//	rzb_alloc_widget_text(
	//		&rzb, NULL, &kit, );

	//widget_image =
	//	rzb_alloc_widget_image(
	//		&rzb, NULL, &kit, false, );

	widget_button =
		rzb_alloc_widget_button(
			&rzb, layout_demo_button, &kit, NULL, NULL, NULL, false, "OK");

	widget_button_b =
		rzb_alloc_widget_button(
			&rzb, layout_demo_button_b, &kit, NULL, NULL, NULL, false, "OK");

	widget_button_c =
		rzb_alloc_widget_button(
			&rzb, layout_demo_button_c, &kit, NULL, NULL, NULL, false, "OK");

	widget_numberbox =
		rzb_alloc_widget_numberbox(
			&rzb, layout_demo_numberbox, &kit,  NULL, "42");

	widget_textbox =
		rzb_alloc_widget_textbox(
			&rzb, layout_demo_textbox, &kit, NULL, "");

	widget_textarea =
		rzb_alloc_widget_textarea(
			&rzb, layout_demo_textarea, &kit, NULL, true, "");

	widget_radiobutton =
		rzb_alloc_widget_radiobutton(
			&rzb, layout_demo_radiobutton, &kit, NULL, false);

	widget_checkbox =
		rzb_alloc_widget_checkbox(
			&rzb, layout_demo_checkbox, &kit, NULL, false);

#if 0
	widget_scrollbar =
		rzb_alloc_widget_scrollbar(
			&rzb, NULL, &kit, NULL);
#endif

	widget_slider =
		rzb_alloc_widget_slider(
			&rzb, layout_demo_slider, &kit, NULL, false, 33);

	widget_progressbar =
		rzb_alloc_widget_progressbar(
			&rzb, layout_demo_progressbar, &kit, false, 66);

	rzb_update_root_widget(&rzb, widget_tabs);

	rzb_make_child(widget_handles, widget_tabs);
	rzb_make_child(widget_button, widget_handles);
	rzb_make_child(widget_button_b, widget_handles);
	rzb_make_child(widget_button_c, widget_handles);
	rzb_make_child(widget_numberbox, widget_handles);
	rzb_make_child(widget_textbox, widget_handles);
	rzb_make_child(widget_textarea, widget_handles);
	rzb_make_child(widget_popup, widget_handles);
	rzb_make_child(widget_radiobutton, widget_handles);
	rzb_make_child(widget_checkbox, widget_handles);
	rzb_make_child(widget_slider, widget_handles);
	rzb_make_child(widget_progressbar, widget_handles);

	//rzb_select_widget(&rzb, widget_button_b);
	rzb_select_widget(&rzb, widget_slider);

	render(&globox, &rzb, &rzb_display_info, &event_post);

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

	struct event_data event_data =
	{
		.rzb = &rzb,
		.event_post = &event_post,
	};

	willis_init(
		&willis,
		willis_backend_link,
		true,
		event,
		&event_data);

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

	globox_platform_prepoll(&globox);

	if (globox_error_catch(&globox))
	{
		globox_context_software_free(&globox);
		globox_platform_free(&globox);
		globox_close(&globox);
		return 1;
	}

	while (globox_get_closed(&globox) == false)
	{
		// update cursor
		cursoryx_set(
			&cursoryx,
			CURSORYX_HAND);

		// handle display events
		globox_platform_events_poll(&globox);

		if (globox_error_catch(&globox))
		{
			globox_context_software_free(&globox);
			globox_platform_free(&globox);
			globox_close(&globox);
			return 1;
		}

		// update density info
		display_info = dpishit_get_display_info(&dpishit);
		rzb_display_info.px_width = display_info->px_width;
		rzb_display_info.px_height = display_info->px_height;
		rzb_display_info.mm_width = display_info->mm_width;
		rzb_display_info.mm_height = display_info->mm_height;
		rzb_display_info.dpi_logic = display_info->dpi_logic;
		rzb_display_info.scale = display_info->scale;

		// render
		render(&globox, &rzb, &rzb_display_info, &event_post);

		if (globox_error_catch(&globox))
		{
			globox_context_software_free(&globox);
			globox_platform_free(&globox);
			globox_close(&globox);
			return 1;
		}
	}

	cursoryx_stop(&cursoryx);
	willis_free(&willis);
	globox_context_software_free(&globox);
	globox_platform_free(&globox);
	globox_close(&globox);

	return 0;
}
