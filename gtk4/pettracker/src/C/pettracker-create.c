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

#include "pettracker-create.h"
#include <cJSON.h>

#define JSON_FILE "data.json"

struct _PettrackerCreate
{
  GtkApplicationWindow  parent_instance;

  /* Template widgets */
  GtkLabel *name_label;
  GtkEntry *name_entry;

  GtkLabel *age_label;
  GtkEntry *age_entry;

  GtkButton *save_button;
};

G_DEFINE_FINAL_TYPE (PettrackerCreate, pettracker_create, GTK_TYPE_APPLICATION_WINDOW)

static int
write_to_file (const gchar *name, const gchar *age)
{
  int iAge = atoi(age);

  // Read in the JSON file
  FILE *file = fopen(JSON_FILE, "r");
  if (!file) {
      printf("%s does not exist. Creating it.\n", JSON_FILE);
      file = fopen(JSON_FILE, "w");
      if (!file) {
          printf("Failed to create %s.\n", JSON_FILE);
          return 1;
      }
      // New file
      // TODO: Write the JSON file

  }
  // Existing file
  // TODO: Read the JSON file
  // TODO: Add the new data
  return 0;
}


static void
save_button_clicked (GtkButton *button,
                     gpointer   user_data)
{

  struct _PettrackerCreate *self = PETTRACKER_CREATE (user_data);

  GtkEntryBuffer *buffer;
  const gchar* name_text;
  const gchar* age_text;

  buffer = gtk_entry_get_buffer (self->name_entry);
  name_text = gtk_entry_buffer_get_text (buffer);

  buffer = gtk_entry_get_buffer (self->age_entry);
  age_text = gtk_entry_buffer_get_text (buffer);

  write_to_file(name_text, age_text);

  //g_print("Name: %s\n", name_text);
  //g_print("Age: %s\n", age_text);
}

static void
pettracker_create_class_init (PettrackerCreateClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  gtk_widget_class_set_template_from_resource (widget_class, "/org/gnome/Pettracker/pettracker-create.ui");

  gtk_widget_class_bind_template_child (widget_class, PettrackerCreate, name_label);
  gtk_widget_class_bind_template_child (widget_class, PettrackerCreate, name_entry);

  gtk_widget_class_bind_template_child (widget_class, PettrackerCreate, age_label);
  gtk_widget_class_bind_template_child (widget_class, PettrackerCreate, age_entry);

  gtk_widget_class_bind_template_child (widget_class, PettrackerCreate, save_button);
}

static void
pettracker_create_init (PettrackerCreate *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));
  g_signal_connect(self->save_button, "clicked", G_CALLBACK(save_button_clicked), (gpointer*) self);

  /*
  g_signal_connect (self->save_button, "clicked", G_CALLBACK (save_button_clicked), self->name_entry);
  g_signal_connect (self->save_button, "clicked", G_CALLBACK (save_button_clicked), self->age_entry);
  */
}

PettrackerCreate * pettracker_create_new (PettrackerCreate *create)
{
  return g_object_new (PETTRACKER_TYPE_CREATE,
                       "application", create,
                       NULL);
}
