#include "helpers.h"

#include "razorbeard.h"
#include "razorbeard_default_widgets.h"

#define PADDING_X 12
#define PADDING_Y 12

void layout_demo_handles(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
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

	rzb_widget_layout_update(widget, x, y, width, height);

	data->section_lengths[0] =
		widget->height / 2;
	data->section_lengths[1] =
		widget->height / 2;
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
	widget->x = 2;
	widget->y = 2;
	widget->width = rzb->argb_width - 4;
	widget->height = rzb->argb_height - 4;
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
