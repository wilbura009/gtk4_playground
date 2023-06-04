/* viewmd-window.c
 *
 * Copyright 2023 wb
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

//#include "config.h"

#include "viewmd-window.h"

struct _ViewmdWindow
{
	AdwApplicationWindow  parent_instance;

	/* Template widgets */
	GtkHeaderBar        *header_bar;
  GtkTextView         *text_view;
};

G_DEFINE_FINAL_TYPE (ViewmdWindow, viewmd_window, ADW_TYPE_APPLICATION_WINDOW)

static void
viewmd_window_class_init (ViewmdWindowClass *klass)
{
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

	gtk_widget_class_set_template_from_resource (widget_class, "/org/gnome/viewmd/viewmd-window.ui");
	gtk_widget_class_bind_template_child (widget_class, ViewmdWindow, header_bar);
  gtk_widget_class_bind_template_child (widget_class, ViewmdWindow, text_view);
}

static void
viewmd_window_init (ViewmdWindow *self)
{
	gtk_widget_init_template (GTK_WIDGET (self));

  // Get the text buffer
  GtkTextBuffer *buffer = gtk_text_view_get_buffer (self->text_view);
  gtk_text_buffer_set_text (buffer, "Hello, World!", -1);
}
