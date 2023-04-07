/* cosmicagenda-window.c
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

#include "cosmicagenda-window.h"
#include "cJSON.h"
#include "cJSON-file-handler.h"

typedef struct _GtkListBoxRow_LList
{
  GtkListBoxRow *head;
} GtkListBoxRow_LList;


struct _CosmicagendaWindow
{
  GtkApplicationWindow  parent_instance;

  /* Template widgets */
  GtkHeaderBar        *header_bar;
  GtkButton           *add_button;
  GtkButton           *remove_button;
  GtkEntry            *todo_entry;

  GtkListBox          *listbox;
  GtkBox              *box;
  int                 item_count;

  FILE                *file;
  char                *fbuffer;
  cJSON               *json_array;
};

G_DEFINE_FINAL_TYPE (CosmicagendaWindow, cosmicagenda_window, GTK_TYPE_APPLICATION_WINDOW)

GtkListBoxRow *row_init (void)
{
  GtkListBoxRow *row = GTK_LIST_BOX_ROW(gtk_list_box_row_new());
  GtkCheckButton *checkbutton = GTK_CHECK_BUTTON(gtk_check_button_new());
  gtk_list_box_row_set_child(row, GTK_WIDGET(checkbutton));

  return row;
}

static void
on_window_destroy_cb (CosmicagendaWindow *self)
{
  // TODO: Save the file with cjson_file_write
  cjson_file_handler_write(&self->file, self->json_array);
  cjson_file_handler_close(&self->file);
  cJSON_Delete(self->json_array);
  return;
}

static int
list_box_has_checked_items (CosmicagendaWindow *self)
{
  for (int i = 0; i < self->item_count; i++)
  {
    GtkListBoxRow *row = gtk_list_box_get_row_at_index(self->listbox, i);
    GtkCheckButton *checkbutton = GTK_CHECK_BUTTON(gtk_list_box_row_get_child(row));
    if (gtk_check_button_get_active(checkbutton))
    {
      return 1;
    }
  }
  return 0;
}

static void
list_box_remove_checked_item (GtkButton *button,
    CosmicagendaWindow *self)
{
  for (int i = 0; i < self->item_count; i++)
  {
    GtkListBoxRow *row = gtk_list_box_get_row_at_index(self->listbox, i);
    GtkCheckButton *checkbutton = GTK_CHECK_BUTTON(gtk_list_box_row_get_child(row));
    if (gtk_check_button_get_active(checkbutton))
    {
      gtk_list_box_remove(self->listbox, GTK_WIDGET(row));
      cJSON *json_item = cJSON_GetArrayItem(self->json_array, i);
      cJSON_DeleteItemFromArray(self->json_array, i);
      self->item_count--;
    }
  }
}

static void
list_box_remove_checked_items_cb (GtkButton *button,
    CosmicagendaWindow *self)
{
  while (list_box_has_checked_items(self))
    list_box_remove_checked_item(button, self);
}

static void
list_box_print_rows (CosmicagendaWindow *self) {
  for (int i = 0; i < self->item_count; i++)
  {
    GtkListBoxRow *row = gtk_list_box_get_row_at_index(self->listbox, i);
    GtkCheckButton *checkbutton = GTK_CHECK_BUTTON(gtk_list_box_row_get_child(row));
    const gchar *label_text = gtk_check_button_get_label(checkbutton);
    g_print("label_text: %s", label_text);
  }
}

static void
list_box_add_item_cb (GtkButton *button,
    CosmicagendaWindow *self)
{
  GtkEntryBuffer *entry_buffer;
  const gchar *entry_text;

  entry_buffer = gtk_entry_get_buffer(self->todo_entry);
  entry_text = gtk_entry_buffer_get_text(entry_buffer);

  GtkListBoxRow *row= row_init();

  GtkCheckButton *checkbutton = GTK_CHECK_BUTTON(gtk_list_box_row_get_child(row));
  gtk_check_button_set_label(checkbutton, entry_text);

  gtk_list_box_insert(self->listbox, GTK_WIDGET(row), self->item_count++);

  cJSON *json_item = cJSON_CreateObject();
  cJSON_AddStringToObject(json_item, "item", entry_text);
  cJSON_AddItemToArray(self->json_array, json_item);
}

static void
list_box_restore_state(CosmicagendaWindow *self)
{
  GtkEntryBuffer *entry_buffer;
  const gchar *entry_text;

  for (int i = 0; i < cJSON_GetArraySize(self->json_array); i++)
  {
    cJSON *json_item = cJSON_GetArrayItem(self->json_array, i);
    cJSON *json_item_text = cJSON_GetObjectItemCaseSensitive(json_item, "item");
    entry_text = json_item_text->valuestring;
    GtkListBoxRow *row= row_init();
    GtkCheckButton *checkbutton = GTK_CHECK_BUTTON(gtk_list_box_row_get_child(row));
    gtk_check_button_set_label(checkbutton, entry_text);
    gtk_list_box_insert(self->listbox, GTK_WIDGET(row), self->item_count++);
  }
}

static void
cosmicagenda_window_class_init (CosmicagendaWindowClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  gtk_widget_class_set_template_from_resource (widget_class, "/org/gnome/Cosmicagenda/cosmicagenda-window.ui");
  gtk_widget_class_bind_template_child (widget_class, CosmicagendaWindow, header_bar);
  gtk_widget_class_bind_template_child (widget_class, CosmicagendaWindow, add_button);
  gtk_widget_class_bind_template_child (widget_class, CosmicagendaWindow, remove_button);
  gtk_widget_class_bind_template_child (widget_class, CosmicagendaWindow, todo_entry);
}

static void
cosmicagenda_window_init (CosmicagendaWindow *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));

  // Create the JSON object
  self->json_array = cJSON_CreateArray();  

  // Prepares a file for writing
  cjson_file_handler_init(&self->file, &self->fbuffer, &self->json_array);

   // Create the listbox
  self->listbox = GTK_LIST_BOX(gtk_list_box_new());
  gtk_window_set_child (GTK_WINDOW (self), GTK_WIDGET(self->listbox));

  // Load the previous state from the json_array
  list_box_restore_state(self);

  // Connect the add button
  g_signal_connect (self->add_button, "clicked", G_CALLBACK (list_box_add_item_cb), self);

  // Connect the remove button
  g_signal_connect (self->remove_button, "clicked", G_CALLBACK (list_box_remove_checked_items_cb), self);

  // Free the rowlist
  g_signal_connect (self, "destroy", G_CALLBACK (on_window_destroy_cb), NULL);
}
