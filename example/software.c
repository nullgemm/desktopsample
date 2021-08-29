#include "globox.h"
#include "globox_private_getters.h"

#include "willis.h"
#include "cursoryx.h"
#include "dpishit.h"
#include "helpers.h"
#include "razorbeard.h"
#include "razorbeard_default_widgets.h"
#include "button/razorbeard_widget_button.h"
#include "checkbox/razorbeard_widget_checkbox.h"
#include "dropmenu/razorbeard_widget_dropmenu.h"
#include "frame/razorbeard_widget_frame.h"
#include "handles/razorbeard_widget_handles.h"
#include "image/razorbeard_widget_image.h"
#include "numberbox/razorbeard_widget_numberbox.h"
#include "pager/razorbeard_widget_pager.h"
#include "popup/razorbeard_widget_popup.h"
#include "progressbar/razorbeard_widget_progressbar.h"
#include "radiobutton/razorbeard_widget_radiobutton.h"
#include "scrollbar/razorbeard_widget_scrollbar.h"
#include "separator/razorbeard_widget_separator.h"
#include "slider/razorbeard_widget_slider.h"
#include "tabs/razorbeard_widget_tabs.h"
#include "text/razorbeard_widget_text.h"
#include "textarea/razorbeard_widget_textarea.h"
#include "textbox/razorbeard_widget_textbox.h"

#include <stddef.h>
#include <stdio.h>

struct event_data
{
	struct rzb* rzb;
	struct globox* globox;
	bool* event_post;
};

enum rzb_default_widgets_event_state state_table[] =
{
	[WILLIS_STATE_NONE] = RZB_STATE_NONE,
	[WILLIS_STATE_PRESS] = RZB_STATE_PRESS,
	[WILLIS_STATE_RELEASE] = RZB_STATE_RELEASE,
};

enum rzb_default_widgets_events event_table[] =
{
	[WILLIS_NONE] = RZB_NONE,

	[WILLIS_MOUSE_CLICK_LEFT] = RZB_MOUSE_CLICK_LEFT,
	[WILLIS_MOUSE_CLICK_RIGHT] = RZB_MOUSE_CLICK_RIGHT,
	[WILLIS_MOUSE_CLICK_MIDDLE] = RZB_MOUSE_CLICK_MIDDLE,
	[WILLIS_MOUSE_WHEEL_UP] = RZB_MOUSE_WHEEL_UP,
	[WILLIS_MOUSE_WHEEL_DOWN] = RZB_MOUSE_WHEEL_DOWN,
	[WILLIS_MOUSE_MOTION] = RZB_MOUSE_MOTION,

	[WILLIS_KEY_ESCAPE] = RZB_KEY_ESCAPE,
	[WILLIS_KEY_BACKSPACE] = RZB_KEY_BACKSPACE,
	[WILLIS_KEY_TAB] = RZB_KEY_TAB,
	[WILLIS_KEY_ENTER] = RZB_KEY_ENTER,
	[WILLIS_KEY_SHIFT_LEFT] = RZB_KEY_SHIFT_LEFT,
	[WILLIS_KEY_SHIFT_RIGHT] = RZB_KEY_SHIFT_RIGHT,
	[WILLIS_KEY_CTRL_LEFT] = RZB_KEY_CTRL_LEFT,
	[WILLIS_KEY_CTRL_RIGHT] = RZB_KEY_CTRL_RIGHT,
	[WILLIS_KEY_DELETE] = RZB_KEY_DELETE,
	[WILLIS_KEY_HOME] = RZB_KEY_HOME,
	[WILLIS_KEY_END] = RZB_KEY_END,
	[WILLIS_KEY_PAGE_UP] = RZB_KEY_PAGE_UP,
	[WILLIS_KEY_PAGE_DOWN] = RZB_KEY_PAGE_DOWN,
	[WILLIS_KEY_UP] = RZB_KEY_UP,
	[WILLIS_KEY_DOWN] = RZB_KEY_DOWN,
	[WILLIS_KEY_LEFT] = RZB_KEY_LEFT,
	[WILLIS_KEY_RIGHT] = RZB_KEY_RIGHT,
};

struct frame_data
{
	struct globox* globox;
	struct cursoryx* cursoryx;
};

void frame_on_area(struct rzb* rzb, struct rzb_widget* widget)
{
	struct rzb_widget_frame* frame = widget->data_widget;
	enum rzb_widget_frame_status status = frame->status;
	struct frame_data* data = frame->button_data;
	struct cursoryx* cursoryx = data->cursoryx;

	switch (status)
	{
		case RZB_WIDGET_FRAME_MINIMIZE:
		case RZB_WIDGET_FRAME_MAXIMIZE:
		case RZB_WIDGET_FRAME_CLOSE:
		case RZB_WIDGET_FRAME_MOVE:
		{
			cursoryx_set(
				cursoryx,
				CURSORYX_HAND);
			break;
		}
		case RZB_WIDGET_FRAME_SIZE_E:
		{
			cursoryx_set(
				cursoryx,
				CURSORYX_SIZE_W_E);
			break;
		}
		case RZB_WIDGET_FRAME_SIZE_NE:
		{
			cursoryx_set(
				cursoryx,
				CURSORYX_SIZE_NE_SW);
			break;
		}
		case RZB_WIDGET_FRAME_SIZE_N:
		{
			cursoryx_set(
				cursoryx,
				CURSORYX_SIZE_N_S);
			break;
		}
		case RZB_WIDGET_FRAME_SIZE_NW:
		{
			cursoryx_set(
				cursoryx,
				CURSORYX_SIZE_NW_SE);
			break;
		}
		case RZB_WIDGET_FRAME_SIZE_W:
		{
			cursoryx_set(
				cursoryx,
				CURSORYX_SIZE_W_E);
			break;
		}
		case RZB_WIDGET_FRAME_SIZE_SW:
		{
			cursoryx_set(
				cursoryx,
				CURSORYX_SIZE_NE_SW);
			break;
		}
		case RZB_WIDGET_FRAME_SIZE_S:
		{
			cursoryx_set(
				cursoryx,
				CURSORYX_SIZE_N_S);
			break;
		}
		case RZB_WIDGET_FRAME_SIZE_SE:
		{
			cursoryx_set(
				cursoryx,
				CURSORYX_SIZE_NW_SE);
			break;
		}
		default:
		{
			break;
		}
	}
}

void frame_off_area(struct rzb* rzb, struct rzb_widget* widget)
{
	struct rzb_widget_frame* frame = widget->data_widget;
	struct frame_data* data = frame->button_data;
	struct cursoryx* cursoryx = data->cursoryx;

	cursoryx_set(
		cursoryx,
		CURSORYX_ARROW);
}

void frame_pressed(struct rzb* rzb, struct rzb_widget* widget)
{
	struct rzb_widget_frame* frame = widget->data_widget;
	enum rzb_widget_frame_status status = frame->status;
	struct frame_data* data = frame->button_data;
	struct globox* globox = data->globox;

	switch (status)
	{
		case RZB_WIDGET_FRAME_MOVE:
		{
			globox_platform_interactive_mode(globox, GLOBOX_INTERACTIVE_MOVE);
			break;
		}
		case RZB_WIDGET_FRAME_SIZE_E:
		{
			globox_platform_interactive_mode(globox, GLOBOX_INTERACTIVE_E);
			break;
		}
		case RZB_WIDGET_FRAME_SIZE_NE:
		{
			globox_platform_interactive_mode(globox, GLOBOX_INTERACTIVE_NE);
			break;
		}
		case RZB_WIDGET_FRAME_SIZE_N:
		{
			globox_platform_interactive_mode(globox, GLOBOX_INTERACTIVE_N);
			break;
		}
		case RZB_WIDGET_FRAME_SIZE_NW:
		{
			globox_platform_interactive_mode(globox, GLOBOX_INTERACTIVE_NW);
			break;
		}
		case RZB_WIDGET_FRAME_SIZE_W:
		{
			globox_platform_interactive_mode(globox, GLOBOX_INTERACTIVE_W);
			break;
		}
		case RZB_WIDGET_FRAME_SIZE_SW:
		{
			globox_platform_interactive_mode(globox, GLOBOX_INTERACTIVE_SW);
			break;
		}
		case RZB_WIDGET_FRAME_SIZE_S:
		{
			globox_platform_interactive_mode(globox, GLOBOX_INTERACTIVE_S);
			break;
		}
		case RZB_WIDGET_FRAME_SIZE_SE:
		{
			globox_platform_interactive_mode(globox, GLOBOX_INTERACTIVE_SE);
			break;
		}
		default:
		{
			break;
		}
	}
}

void frame_released(struct rzb* rzb, struct rzb_widget* widget)
{
	struct rzb_widget_frame* frame = widget->data_widget;
	enum rzb_widget_frame_status status = frame->status;
	struct frame_data* data = frame->button_data;
	struct cursoryx* cursoryx = data->cursoryx;
	struct globox* globox = data->globox;

	switch (status)
	{
		case RZB_WIDGET_FRAME_MINIMIZE:
		{
			globox_platform_set_state(
				globox,
				GLOBOX_STATE_MINIMIZED); 

			break;
		}
		case RZB_WIDGET_FRAME_MAXIMIZE:
		{
			globox_platform_set_state(
				globox,
				GLOBOX_STATE_MAXIMIZED); 

			break;
		}
		case RZB_WIDGET_FRAME_CLOSE:
		{
			globox_set_closed(
				globox,
				true);

			break;
		}
		default:
		{
			globox_platform_interactive_mode(
				globox,
				GLOBOX_INTERACTIVE_STOP);

			break;
		}
	}

	cursoryx_set(
		cursoryx,
		CURSORYX_ARROW);
}

void frame_dragged(struct rzb* rzb, struct rzb_widget* widget)
{

}

struct handles_data
{
	struct globox* globox;
	struct cursoryx* cursoryx;
};

void handles_on_area(struct rzb* rzb, struct rzb_widget* widget)
{
	struct rzb_widget_handles* handles = widget->data_widget;
	struct handles_data* data = handles->handles_data;
	struct cursoryx* cursoryx = data->cursoryx;

	if (handles->horizontal == true)
	{
		cursoryx_set(
			cursoryx,
			CURSORYX_SIZE_W_E);
	}
	else
	{
		cursoryx_set(
			cursoryx,
			CURSORYX_SIZE_N_S);
	}
}

void handles_off_area(struct rzb* rzb, struct rzb_widget* widget)
{
	struct rzb_widget_handles* handles = widget->data_widget;
	struct handles_data* data = handles->handles_data;
	struct cursoryx* cursoryx = data->cursoryx;

	cursoryx_set(
		cursoryx,
		CURSORYX_ARROW);
}

void handles_pressed(struct rzb* rzb, struct rzb_widget* widget)
{
	struct rzb_widget_handles* handles = widget->data_widget;
	struct handles_data* data = handles->handles_data;
	struct cursoryx* cursoryx = data->cursoryx;

	if (handles->horizontal == true)
	{
		cursoryx_set(
			cursoryx,
			CURSORYX_SIZE_W_E);
	}
	else
	{
		cursoryx_set(
			cursoryx,
			CURSORYX_SIZE_N_S);
	}
}

void handles_released(struct rzb* rzb, struct rzb_widget* widget)
{
	struct rzb_widget_handles* handles = widget->data_widget;
	struct handles_data* data = handles->handles_data;
	struct cursoryx* cursoryx = data->cursoryx;

	cursoryx_set(
		cursoryx,
		CURSORYX_ARROW);
}

void handles_dragged(struct rzb* rzb, struct rzb_widget* widget)
{
	struct rzb_widget_handles* handles = widget->data_widget;
	struct handles_data* data = handles->handles_data;
	struct cursoryx* cursoryx = data->cursoryx;

	if (handles->horizontal == true)
	{
		cursoryx_set(
			cursoryx,
			CURSORYX_SIZE_W_E);
	}
	else
	{
		cursoryx_set(
			cursoryx,
			CURSORYX_SIZE_N_S);
	}
}

#if defined(WILLIS_WAYLAND)
void callback_serial(
	void* data,
	uint32_t serial)
{
	struct globox* globox = data;
	globox_wayland_save_serial(globox, serial);
}
#endif

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
		globox_platform_prepare_buffer(globox);

		uint32_t width = globox_get_width(globox);
		uint32_t height = globox_get_height(globox);
		uint32_t* argb = globox_platform_get_argb(globox);

		*event_post = false;
		rzb->argb = argb;
		rzb->argb_width = width;
		rzb->argb_height = height;

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
	struct rzb_default_widgets_events_data rzb_events_data = {0};
	char* utf8_string = willis_get_utf8_string(willis);
	struct event_data* event_data = data;

#if defined(WILLIS_WAYLAND)
	// wayland dumbness workaround
	if ((event_code == WILLIS_MOUSE_MOTION)
		&& (globox_get_interactive_mode(event_data->globox) != GLOBOX_INTERACTIVE_STOP))
	{
		event_code = WILLIS_MOUSE_CLICK_LEFT;
		event_state = WILLIS_STATE_RELEASE;
	}
#endif

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

	if (utf8_string != NULL)
	{
		printf(
			"%s\n",
			utf8_string);

		rzb_events_data.typed_string = utf8_string;
		//rzb_send_events_data(event_data->rzb, &rzb_events_data);
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

			rzb_events_data.mouse_pos_x = willis_get_mouse_x(willis);
			rzb_events_data.mouse_pos_y = willis_get_mouse_y(willis);
			rzb_send_events_data(event_data->rzb, &rzb_events_data);
		}
	}

	if ((event_code == WILLIS_KEY_UP)
		|| (event_code == WILLIS_KEY_DOWN)
		|| (event_code == WILLIS_KEY_LEFT)
		|| (event_code == WILLIS_KEY_RIGHT)
		|| (event_code == WILLIS_MOUSE_MOTION)
		|| (event_code == WILLIS_MOUSE_CLICK_LEFT))
	{
		rzb_send_event(event_data->rzb, event_table[event_code], state_table[event_state]);
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
	struct rzb_widget* widget_frame;
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

	rzb_default_widgets_init(&rzb, &kit);

	char* tabs[6] = {"first", "second", "third", "fourth", "fifth", "sixth"};

	struct frame_data frame_data =
	{
		.globox = &globox,
		.cursoryx = &cursoryx,
	};

	widget_frame =
		rzb_alloc_widget_frame(
			&rzb,
			layout_demo_frame,
			&kit,
			"desktop sample",
			frame_on_area,
			frame_off_area,
			frame_pressed,
			frame_released,
			frame_dragged,
			&frame_data);

	struct handles_data handles_data =
	{
		.globox = &globox,
		.cursoryx = &cursoryx,
	};

	widget_handles =
		rzb_alloc_widget_handles(
			&rzb,
			layout_demo_handles,
			&kit,
			handles_on_area,
			handles_off_area,
			handles_pressed,
			handles_released,
			handles_dragged,
			&handles_data,
			false,
			3);

#if 0
	widget_pager =
		rzb_alloc_widget_pager(
			&rzb, NULL, &kit, false);
#endif

	widget_tabs =
		rzb_alloc_widget_tabs(
			&rzb, layout_demo_tabs, &kit, NULL, NULL, NULL, NULL, NULL, tabs, 6, 2);

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
			&rzb, layout_demo_button, &kit,  NULL, NULL, NULL, NULL, NULL, false, "OK");

	widget_button_b =
		rzb_alloc_widget_button(
			&rzb, layout_demo_button_b, &kit, NULL, NULL, NULL, NULL, NULL, false, "OK");

	widget_button_c =
		rzb_alloc_widget_button(
			&rzb, layout_demo_button_c, &kit, NULL, NULL, NULL, NULL, NULL, false, "OK");

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

	if (true || globox_get_frameless(&globox))
	{
		rzb_update_root_widget(&rzb, widget_frame);
		rzb_make_child(widget_tabs, widget_frame);
	}
	else
	{
		rzb_update_root_widget(&rzb, widget_tabs);
	}

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
		.callback_serial =
			callback_serial,
		.callback_serial_data =
			&globox,
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
		.globox = &globox,
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
