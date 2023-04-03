/* pettracker-window.c
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

#include "pettracker-window.h"
#include "pettracker-create.h"
#include <stdio.h>

struct _PettrackerWindow
{
  GtkApplicationWindow  parent_instance;

  /* Template widgets */
  GtkHeaderBar        *header_bar;
  GtkButton           *add_button;
};

G_DEFINE_FINAL_TYPE (PettrackerWindow, pettracker_window, GTK_TYPE_APPLICATION_WINDOW)

/******************************************************************************
* Function: show_action_dialog    
* 
* Description: Used for testing. Is a helper for action_cb.
*****************************************************************************/
static void
show_action_dialog (gpointer user_data)
{
  const char *name = user_data;
  GtkWidget *dialog;

  dialog = gtk_message_dialog_new (NULL,
      GTK_DIALOG_DESTROY_WITH_PARENT,
      GTK_MESSAGE_INFO,
      GTK_BUTTONS_CLOSE,
      "You activated action: \"%s\"",
      name);

  g_signal_connect(dialog, "response",
      G_CALLBACK (gtk_window_destroy), NULL);

  gtk_widget_show(dialog);
}

/******************************************************************************
* Function: action_cb
* 
* Description: Used for testing.
*****************************************************************************/
static void
action_cb(GtkWidget *widget, gpointer user_data)
{
  char *button_name = (char *) user_data;
  show_action_dialog(button_name);
}

static void
add_button_clicked(GtkWidget *widget, gpointer user_data)
{
  PettrackerCreate *create = pettracker_create_new(NULL);
  gtk_window_present(GTK_WINDOW(create));
}

static void
pettracker_window_class_init (PettrackerWindowClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  gtk_widget_class_set_template_from_resource (widget_class, "/org/gnome/Pettracker/pettracker-window.ui");
  gtk_widget_class_bind_template_child (widget_class, PettrackerWindow, header_bar);
  gtk_widget_class_bind_template_child (widget_class, PettrackerWindow, add_button);
}

static void
pettracker_window_init (PettrackerWindow *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));
  // Connect the clicked signal of the add_button to the action_cb function
  g_signal_connect (self->add_button, "clicked", G_CALLBACK (add_button_clicked), NULL);
}
