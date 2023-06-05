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
#include <stdio.h>
//#include <webkit2/webkit2.h>

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

  // Read the file contents into a string
  gchar *path_md = "./src/input/ipsum.md";
  GFile *file = g_file_new_for_path (path_md);
  gchar *contents;
  g_file_load_contents (file, NULL, &contents, NULL, NULL, NULL);

  // Get the text buffer
  GtkTextBuffer *buffer = gtk_text_view_get_buffer (self->text_view);
  // Set the text view to be uneditable
  gtk_text_view_set_editable (self->text_view, FALSE);
  // Set the text view to be cursorless
  gtk_text_view_set_cursor_visible (self->text_view, FALSE);
  // Set the text buffer's text
  gtk_text_buffer_set_text (buffer, contents, -1);
}
