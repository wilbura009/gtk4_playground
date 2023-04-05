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
  GtkListBoxRow_LList *rowlist;
  int                 item_count;

  FILE                *file;
  cJSON               *json_obj;
};

G_DEFINE_FINAL_TYPE (CosmicagendaWindow, cosmicagenda_window, GTK_TYPE_APPLICATION_WINDOW)

GtkListBoxRow *row_init (void)
{
  GtkListBoxRow *row = GTK_LIST_BOX_ROW(gtk_list_box_row_new());
  GtkCheckButton *checkbutton = GTK_CHECK_BUTTON(gtk_check_button_new());
  gtk_list_box_row_set_child(row, GTK_WIDGET(checkbutton));

  return row;
}

/*
GtkListBoxRow_LList *list_init (void)
{
  GtkListBoxRow_LList *list = g_new(GtkListBoxRow_LList, 1);
  list->head = NULL;

  return list;
}

static void list_free (GtkListBoxRow_LList *list)
{
  g_free(list);
}

static void
savefile (CosmicagendaWindow *self)
{
  const gchar *json_string = cJSON_Print(self->json_obj);
  fputs(json_string, self->file);
  //g_print("entry_text: %s\n, json_string: %s\n", entry_text, json_string);
  free((void*)json_string);
}
*/

static void
on_window_destroy_cb (CosmicagendaWindow *self)
{
  //savefile(self);
  //list_free(self->rowlist);
  //g_free(self->listbox);
  //free(self->json_obj);
  //fclose(self->file);
}

/*
static void
additem_cb (GtkButton *button,
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
}
*/

static void
removeitem_cb (GtkButton *button,
               CosmicagendaWindow *self)
{
  for (int i = 0; i < self->item_count; i++)
  {
    GtkListBoxRow *row = gtk_list_box_get_row_at_index(self->listbox, i);
    GtkCheckButton *checkbutton = GTK_CHECK_BUTTON(gtk_list_box_row_get_child(row));
    if (gtk_check_button_get_active(checkbutton))
    {
      gtk_list_box_remove(self->listbox, GTK_WIDGET(row));
      self->item_count--;
    }
  }
}

static FILE
*openfile(FILE *file)
{
  file = fopen("cosmicagenda.json", "w+");
  if (file == NULL)
  {
   g_printerr("openfile: Error opening file.\n");
   return NULL;
  }
  return file;
}

static void
additem_cb (GtkButton *button,
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

  self->listbox = GTK_LIST_BOX(gtk_list_box_new());
  gtk_window_set_child (GTK_WINDOW (self), GTK_WIDGET(self->listbox));

  // Connect the add button
  g_signal_connect (self->add_button, "clicked", G_CALLBACK (additem_cb), self);

  // Connect the remove button
  g_signal_connect (self->remove_button, "clicked", G_CALLBACK (removeitem_cb), self);

  // Free the rowlist
  g_signal_connect (self, "destroy", G_CALLBACK (on_window_destroy_cb), NULL);


  /*
  // Open the file
  self->file = openfile(self->file);

  // Create the JSON object
  self->json_obj = cJSON_CreateObject();  

  // Create the listbox
  self->listbox = GTK_LIST_BOX(gtk_list_box_new());
  gtk_window_set_child (GTK_WINDOW (self), GTK_WIDGET(self->listbox));

  // Create the rowlist
  self->rowlist = list_init();

  // Connect the add button
  g_signal_connect (self->add_button, "clicked", G_CALLBACK (additem_cb), self);

  // Connect the remove button
  g_signal_connect (self->remove_button, "clicked", G_CALLBACK (removeitem_cb), self);

  // Free the rowlist
  g_signal_connect (self, "destroy", G_CALLBACK (on_window_destroy_cb), NULL);
  */
}
