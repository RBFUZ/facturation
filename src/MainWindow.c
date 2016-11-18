/*
 * Copyright 2010 Sébastien Aupetit <sebastien.aupetit@univ-tours.fr>
 *
 * This source code is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * This source code is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this source code. If not, see <http://www.gnu.org/licenses/>.
 *
 * $Id: MainWindow.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <MainWindow.h>
#include <Catalog.h>
#include <Customer.h>
#include <Operator.h>
#include <Quotation.h>
#include <Bill.h>

/** Create the menu for the main window
 * @return a pointer on the menu
 */
static GtkWidget * MainWindow_createMenuBar(GtkWidget * window)
{
  GtkWidget * menubar, *submenu;
  GtkWidget * menuitem, *menuitem2;

  menubar = gtk_menu_bar_new();

  menuitem = gtk_menu_item_new_with_label("Devis");
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), menuitem);

  submenu = gtk_menu_new();
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem), submenu);

  menuitem2 = gtk_menu_item_new_with_label("Nouveau");
  gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menuitem2);
  gtk_signal_connect(GTK_OBJECT (menuitem2), "activate", GTK_SIGNAL_FUNC (Quotation_new), window);

  menuitem2 = gtk_menu_item_new_with_label("Visualiser");
  gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menuitem2);
  gtk_signal_connect(GTK_OBJECT (menuitem2), "activate", GTK_SIGNAL_FUNC (Quotation_display), window);

  menuitem2 = gtk_menu_item_new_with_label("Ouvrir");
  gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menuitem2);
  gtk_signal_connect(GTK_OBJECT (menuitem2), "activate", GTK_SIGNAL_FUNC (Quotation_open), window);

  menuitem2 = gtk_menu_item_new_with_label("Convertir en facture");
  gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menuitem2);
  gtk_signal_connect(GTK_OBJECT (menuitem2), "activate", GTK_SIGNAL_FUNC (Quotation_convertToBill), window);

  menuitem = gtk_menu_item_new_with_label("Facture");
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), menuitem);

  submenu = gtk_menu_new();
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem), submenu);

  menuitem2 = gtk_menu_item_new_with_label("Nouveau");
  gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menuitem2);
  gtk_signal_connect(GTK_OBJECT (menuitem2), "activate", GTK_SIGNAL_FUNC (Bill_new), window);

  menuitem2 = gtk_menu_item_new_with_label("Visualiser");
  gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menuitem2);
  gtk_signal_connect(GTK_OBJECT (menuitem2), "activate", GTK_SIGNAL_FUNC (Bill_display), window);

  menuitem2 = gtk_menu_item_new_with_label("Ouvrir");
  gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menuitem2);
  gtk_signal_connect(GTK_OBJECT (menuitem2), "activate", GTK_SIGNAL_FUNC (Bill_open), window);

  menuitem = gtk_menu_item_new_with_label("Paramétrage");
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), menuitem);

  submenu = gtk_menu_new();
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem), submenu);

  menuitem2 = gtk_menu_item_new_with_label("Catalogue des produits");
  gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menuitem2);
  gtk_signal_connect(GTK_OBJECT (menuitem2), "activate", GTK_SIGNAL_FUNC (Catalog_manage), window);

  menuitem2 = gtk_menu_item_new_with_label("Gestion des clients");
  gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menuitem2);
  gtk_signal_connect(GTK_OBJECT (menuitem2), "activate", GTK_SIGNAL_FUNC (Customer_manage), window);

  menuitem2 = gtk_menu_item_new_with_label("Gestion des opérateurs");
  gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menuitem2);
  gtk_signal_connect(GTK_OBJECT (menuitem2), "activate", GTK_SIGNAL_FUNC (Operator_manage), window);

  menuitem = gtk_menu_item_new_with_label("Quitter");
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), menuitem);
  gtk_signal_connect_object(GTK_OBJECT (menuitem), "activate", G_CALLBACK(gtk_main_quit), NULL);

  gtk_widget_show_all(menubar);

  return menubar;
}

/** Create the student panel for the main window
 * @return a pointer on the panel
 */
static GtkWidget * MainWindow_createStudentPanel(void)
{
  GtkWidget * panel;
  GtkWidget * table;
  GtkWidget * label;

  char name[1024];
  snprintf(name, 1024, "%s %s", STUDENT_FIRSTNAME, STUDENT_LASTNAME);

  panel = gtk_frame_new("Etudiants");
  gtk_frame_set_shadow_type(GTK_FRAME(panel), GTK_SHADOW_IN);
  gtk_container_set_border_width(GTK_CONTAINER(panel), 5);
  gtk_widget_show(panel);

  table = gtk_table_new(3, 2, FALSE);
  gtk_container_add(GTK_CONTAINER (panel), table);
  gtk_widget_show(table);

  label = gtk_label_new("Etudiants :");
  gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
  gtk_table_attach(GTK_TABLE(table), label, 0, 1, 0, 1, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
  gtk_widget_show(label);

  label = gtk_label_new(name);
  gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
  gtk_table_attach(GTK_TABLE(table), label, 1, 2, 0, 1, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
  gtk_widget_show(label);

  label = gtk_label_new("Groupe :");
  gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
  gtk_table_attach(GTK_TABLE(table), label, 0, 1, 1, 2, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
  gtk_widget_show(label);

  label = gtk_label_new(STUDENT_GROUP);
  gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
  gtk_table_attach(GTK_TABLE(table), label, 1, 2, 1, 2, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
  gtk_widget_show(label);

  label = gtk_label_new("Année :");
  gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
  gtk_table_attach(GTK_TABLE(table), label, 0, 1, 2, 3, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
  gtk_widget_show(label);

  label = gtk_label_new(STUDENT_YEARS);
  gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
  gtk_table_attach(GTK_TABLE(table), label, 1, 2, 2, 3, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
  gtk_widget_show(label);

  return panel;
}

/** Create the main window of the application
 * @return a pointer on the window
 */
GtkWidget * MainWindow_create()
{
  GtkWidget * window;
  GtkAccelGroup * accel_group;
  GtkWidget * vbox;
  GtkWidget * menubar;
  GtkWidget * studentpanel;

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Travaux pratiques de langage C");
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  gtk_container_set_border_width(GTK_CONTAINER (window), 5);

  accel_group = gtk_accel_group_new();
  gtk_window_add_accel_group(GTK_WINDOW (window), accel_group);

  vbox = gtk_vbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER (window), vbox);
  gtk_widget_show(vbox);

  menubar = MainWindow_createMenuBar(window);
  gtk_box_pack_start(GTK_BOX (vbox), menubar, FALSE, TRUE, 0);
  gtk_widget_show(menubar);

  studentpanel = MainWindow_createStudentPanel();
  gtk_box_pack_start(GTK_BOX (vbox), studentpanel, FALSE, TRUE, 0);
  gtk_widget_show(studentpanel);

  return window;
}
