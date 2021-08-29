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

#define PADDING_X 12
#define PADDING_Y 12

void layout_demo_frame(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	widget->x = 0;

	widget->y = 0;

	widget->width =
		rzb->argb_width;

	widget->height =
		rzb->argb_height;
}

void layout_demo_handles(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	static bool first = true;
	struct rzb_widget_handles* data = widget->data_widget;
	struct rzb_default_widgets_context* context = data->context;

	if (widget->parent->height <= context->sizes_current->tab_default_height)
	{
		rzb_widget_set_hide(widget, true);
		return;
	}

	rzb_widget_set_hide(widget, false);

	int x =
		widget->parent->x;
	int y =
		widget->parent->y
		+ context->sizes_current->tab_default_height;
	int width =
		widget->parent->width;
	int height =
		widget->parent->height
		- context->sizes_current->tab_default_height;

	if (first == true)
	{
		data->section_lengths[0] =
			height / 3;
		data->section_lengths[1] =
			height / 3;
		data->section_lengths[2] =
			height / 3;
		first = false;
	}
	else if (height != widget->height)
	{
		data->section_lengths[0] =
			height / 3;
		data->section_lengths[1] =
			height / 3;
		data->section_lengths[2] =
			height / 3;
	}

	rzb_widget_layout_update(widget, x, y, width, height);
}

void layout_demo_pager(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
}

void layout_demo_tabs(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	struct rzb_widget_tabs* data = widget->data_widget;
	struct rzb_default_widgets_context* context = data->context;

	if (widget->parent != NULL)
	{
		widget->x =
			context->sizes_current->frame_border_size;

		widget->y =
			widget->parent->y
			+ context->sizes_current->frame_default_height;

		widget->width =
			rzb->argb_width
			- 2 * context->sizes_current->frame_border_size;

		widget->height =
			rzb->argb_height
			- context->sizes_current->frame_default_height
			- context->sizes_current->frame_border_size;
	}
	else
	{
		widget->x = 0;

		widget->y = 0;

		widget->width =
			rzb->argb_width;

		widget->height =
			rzb->argb_height;
	}
}

void layout_demo_popup(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	struct rzb_widget_handles* data_parent = widget->parent->data_widget;

	if (((widget->parent->width - (3 * PADDING_X) - 133) < 33)
		|| ((data_parent->section_lengths[1] - (2 * PADDING_Y)) < 105))
	{
		rzb_widget_set_hide(widget, true);
		return;
	}

	rzb_widget_set_hide(widget, false);

	int x =
		widget->parent->x
		+ 2 * PADDING_X
		+ 133;
	int y =
		widget->parent->y
		+ data_parent->section_lengths[0]
		+ PADDING_Y;
	int width =
		widget->parent->width
		- (3 * PADDING_X)
		- 133;
	int height =
		105;

	rzb_widget_layout_update(widget, x, y, width, height);
}

void layout_demo_dropmenu(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
}

void layout_demo_separator(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
}

#if 0
void layout_demo_text(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
}

void layout_demo_image(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
}
#endif

void layout_demo_button(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	struct rzb_widget_handles* data_parent = widget->parent->data_widget;

	if (((widget->parent->width - (2 * PADDING_X)) < 100)
		|| ((data_parent->section_lengths[0] - (2 * PADDING_Y)) < 100))
	{
		rzb_widget_set_hide(widget, true);
		return;
	}

	rzb_widget_set_hide(widget, false);

	int x =
		widget->parent->x
		+ PADDING_X;
	int y =
		widget->parent->y
		+ PADDING_Y;
	int width =
		100;
	int height =
		100;

	rzb_widget_layout_update(widget, x, y, width, height);
}

void layout_demo_button_b(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	struct rzb_widget_handles* data_parent = widget->parent->data_widget;

	if (((widget->parent->width - (2 * PADDING_X)) < 133)
		|| ((data_parent->section_lengths[1] - (2 * PADDING_Y)) < 33))
	{
		rzb_widget_set_hide(widget, true);
		return;
	}

	rzb_widget_set_hide(widget, false);

	int x =
		widget->parent->x
		+ PADDING_X;
	int y =
		widget->parent->y
		+ PADDING_Y
		+ data_parent->section_lengths[0];
	int width =
		133;
	int height =
		33;

	rzb_widget_layout_update(widget, x, y, width, height);
}

void layout_demo_button_c(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	struct rzb_widget_handles* data_parent = widget->parent->data_widget;

	if (((widget->parent->width - (2 * PADDING_X)) < 133)
		|| ((data_parent->section_lengths[1] - (3 * PADDING_Y)) / 2 < 33))
	{
		rzb_widget_set_hide(widget, true);
		return;
	}

	rzb_widget_set_hide(widget, false);

	int x =
		widget->parent->x
		+ PADDING_X;
	int y =
		widget->parent->y
		+ (2 * PADDING_Y)
		+ data_parent->section_lengths[0]
		+ 33;
	int width =
		133;
	int height =
		33;

	rzb_widget_layout_update(widget, x, y, width, height);
}

void layout_demo_numberbox(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	struct rzb_widget_handles* data_parent = widget->parent->data_widget;

	if (((widget->parent->width - (3 * PADDING_X) - 100) < 33)
		|| ((data_parent->section_lengths[0] - (2 * PADDING_Y)) < 33))
	{
		rzb_widget_set_hide(widget, true);
		return;
	}

	rzb_widget_set_hide(widget, false);

	int x =
		widget->parent->x
		+ 2 * PADDING_X
		+ 100;
	int y =
		widget->parent->y
		+ PADDING_Y;
	int height =
		33;
	int width;

	if ((widget->parent->width - (4 * PADDING_X) - 100) / 2 < 33)
	{
		width =
			(widget->parent->width
			- (3 * PADDING_X)
			- 100);
	}
	else
	{
		width =
			(widget->parent->width
			- (4 * PADDING_X)
			- 100) / 2;
	}

	rzb_widget_layout_update(widget, x, y, width, height);
}

void layout_demo_textbox(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	struct rzb_widget_handles* data_parent = widget->parent->data_widget;

	if (((widget->parent->width - (3 * PADDING_X) - 100) < 33)
		|| ((data_parent->section_lengths[0] - (3 * PADDING_Y)) / 2 < 33))
	{
		rzb_widget_set_hide(widget, true);
		return;
	}

	rzb_widget_set_hide(widget, false);

	int x =
		widget->parent->x
		+ 2 * PADDING_X
		+ 100;
	int y =
		widget->parent->y
		+ 2 * PADDING_Y
		+ 33;
	int height =
		33;
	int width;

	if ((widget->parent->width - (4 * PADDING_X) - 100) / 2 < 33)
	{
		width =
			(widget->parent->width
			- (3 * PADDING_X)
			- 100);
	}
	else
	{
		width =
			(widget->parent->width
			- (4 * PADDING_X)
			- 100) / 2;
	}

	rzb_widget_layout_update(widget, x, y, width, height);
}

void layout_demo_textarea(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	struct rzb_widget_handles* data_parent = widget->parent->data_widget;

	if (((widget->parent->width - (4 * PADDING_X) - 100) / 2 < 33)
		|| ((data_parent->section_lengths[0] - (2 * PADDING_Y)) < 33))
	{
		rzb_widget_set_hide(widget, true);
		return;
	}

	rzb_widget_set_hide(widget, false);

	int x =
		widget->parent->x
		+ 3 * PADDING_X
		+ 100
		+ (widget->parent->width
		- (4 * PADDING_X)
		- 100) / 2;
	int y =
		widget->parent->y
		+ PADDING_Y;
	int width =
		(widget->parent->width
		- (4 * PADDING_X)
		- 100) / 2;
	int height =
		data_parent->section_lengths[0]
		- (2 * PADDING_Y);

	rzb_widget_layout_update(widget, x, y, width, height);
}

void layout_demo_radiobutton(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	struct rzb_widget_handles* data_parent = widget->parent->data_widget;

	if (((widget->parent->width - (3 * PADDING_X) - 133) < 18)
		|| ((data_parent->section_lengths[1] - (3 * PADDING_Y) - 105) < 18))
	{
		rzb_widget_set_hide(widget, true);
		return;
	}

	rzb_widget_set_hide(widget, false);

	int x =
		widget->parent->x
		+ 2 * PADDING_X
		+ 133;
	int y =
		widget->parent->y
		+ data_parent->section_lengths[0]
		+ 2 * PADDING_Y
		+ 105;
	int width =
		18;
	int height =
		18;

	rzb_widget_layout_update(widget, x, y, width, height);
}

void layout_demo_checkbox(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	struct rzb_widget_handles* data_parent = widget->parent->data_widget;

	if (((widget->parent->width - (3 * PADDING_X) - 133) < 18)
		|| ((data_parent->section_lengths[1] - (4 * PADDING_Y) - 105 - 18) < 18))
	{
		rzb_widget_set_hide(widget, true);
		return;
	}

	rzb_widget_set_hide(widget, false);

	int x =
		widget->parent->x
		+ 2 * PADDING_X
		+ 133;
	int y =
		widget->parent->y
		+ data_parent->section_lengths[0]
		+ 3 * PADDING_Y
		+ 105
		+ 18;
	int width =
		18;
	int height =
		18;

	rzb_widget_layout_update(widget, x, y, width, height);
}

void layout_demo_scrollbar(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
}

void layout_demo_slider(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	struct rzb_widget_handles* data_parent = widget->parent->data_widget;

	if (((widget->parent->width - (4 * PADDING_X) - 133 - 18) < 18)
		|| ((data_parent->section_lengths[1] - (3 * PADDING_Y) - 105) < 18))
	{
		rzb_widget_set_hide(widget, true);
		return;
	}

	rzb_widget_set_hide(widget, false);

	int x =
		widget->parent->x
		+ 3 * PADDING_X
		+ 133
		+ 18;
	int y =
		widget->parent->y
		+ data_parent->section_lengths[0]
		+ 2 * PADDING_Y
		+ 105;
	int width =
		widget->parent->width - (4 * PADDING_X) - 133 - 18;
	int height =
		18;

	rzb_widget_layout_update(widget, x, y, width, height);
}

void layout_demo_progressbar(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	struct rzb_widget_handles* data_parent = widget->parent->data_widget;

	if (((widget->parent->width - (4 * PADDING_X) - 133 - 18) < 18)
		|| ((data_parent->section_lengths[1] - (4 * PADDING_Y) - 105 - 18) < 18))
	{
		rzb_widget_set_hide(widget, true);
		return;
	}

	rzb_widget_set_hide(widget, false);

	int x =
		widget->parent->x
		+ 3 * PADDING_X
		+ 133
		+ 18;
	int y =
		widget->parent->y
		+ data_parent->section_lengths[0]
		+ 3 * PADDING_Y
		+ 105
		+ 18;
	int width =
		widget->parent->width - (4 * PADDING_X) - 133 - 18;
	int height =
		18;

	rzb_widget_layout_update(widget, x, y, width, height);
}
