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
 * $Id: Catalog.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <Catalog.h>
#include <CatalogDB.h>
#include <GtkCatalogModel.h>
#include <CatalogRecord.h>
#include <CatalogRecordEditor.h>

/**
 * Handler to add a product
 * @param button the source button
 * @param treeview the GtkTreeView pointer
 */
static void Catalog_add(GtkWidget * button, GtkTreeView * treeview);

/**
 * Handler to delete a product
 * @param button the source button
 * @param treeview the GtkTreeView pointer
 */
static void Catalog_delete(GtkWidget * button, GtkTreeView * treeview);

/**
 * Handler to modify a product
 * @param button the source button
 * @param treeview the GtkTreeView pointer
 */
static void Catalog_modify(GtkWidget * button, GtkTreeView * treeview);


void Catalog_add(GtkWidget * UNUSED(button), GtkTreeView * treeview) {
    GtkTreeModel * model = gtk_tree_view_get_model(treeview);
    GtkCatalogModel_append_record(
            GTKCATALOGMODEL(gtk_tree_model_sort_get_model (GTK_TREE_MODEL_SORT (model))));
}

void Catalog_delete(GtkWidget * UNUSED(button), GtkTreeView * treeview) {
    GtkTreeModel * model = gtk_tree_view_get_model(treeview);
    GtkTreeSelection * selection = gtk_tree_view_get_selection(treeview);
    GtkTreeIter iter;

    if (gtk_tree_selection_get_selected(selection, NULL, &iter)) {
        int i;
        GtkTreePath *path;

        path = gtk_tree_model_get_path(model, &iter);
        i = gtk_tree_path_get_indices(path)[0];
        GtkCatalogModel_remove_record(
                GTKCATALOGMODEL(gtk_tree_model_sort_get_model (GTK_TREE_MODEL_SORT (model))), i);
        gtk_tree_path_free(path);
    }
}

void Catalog_modify(GtkWidget * UNUSED(button), GtkTreeView * treeview) {
    GtkTreeModel * model = gtk_tree_view_get_model(treeview);
    GtkTreeSelection * selection = gtk_tree_view_get_selection(treeview);
    GtkTreeIter iter;

    if (gtk_tree_selection_get_selected(selection, NULL, &iter)) {
        gint i;
        GtkTreePath *path;

        path = gtk_tree_model_get_path(model, &iter);
        i = gtk_tree_path_get_indices(path)[0];
        GtkCatalogModel_change_record(
                GTKCATALOGMODEL(gtk_tree_model_sort_get_model (GTK_TREE_MODEL_SORT (model))), i);
        gtk_tree_path_free(path);
    }
}

void Catalog_manage(GtkWidget * UNUSED(widget), GtkWindow * parent) {
    CatalogDB * catalogDB = CatalogDB_openOrCreate(CATALOGDB_FILENAME);
    if (catalogDB == NULL) {
        GtkWidget *dialog;

        dialog = gtk_message_dialog_new(parent, GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE,
                "Erreur d'ouverture de la base de catalogue \"%s\"", CATALOGDB_FILENAME);

        /* Close dialog on user response */
        g_signal_connect (dialog,
                "response",
                G_CALLBACK (gtk_widget_destroy),
                NULL);

        gtk_widget_show(dialog);
    } else {
        GtkWidget * dialog;
        GtkWidget *hbox;
        GtkWidget * bbox;
        GtkWidget * sw;
        GtkTreeModel * model, *unsortedmodel;
        GtkWidget *treeview;
        GtkWidget * button;
        GtkCellRenderer * renderer;
        GtkTreeViewColumn *column;
        int columnNum;
        int treeviewWidth = 0;

        /** Creation de la fenêtre principale */
        dialog = gtk_dialog_new_with_buttons("Gestion du catalogue des produits", parent,
                GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, GTK_STOCK_CLOSE,
                GTK_RESPONSE_CLOSE, NULL);

        gtk_container_set_border_width(GTK_CONTAINER (dialog), 5);

        hbox = gtk_hbox_new(FALSE, 5);
        gtk_box_pack_start(GTK_BOX (GTK_DIALOG (dialog)->vbox), hbox, TRUE, TRUE, 0);

        sw = gtk_scrolled_window_new(NULL, NULL);
        gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW (sw), GTK_SHADOW_ETCHED_IN);
        gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW (sw), GTK_POLICY_AUTOMATIC,
                GTK_POLICY_AUTOMATIC);
        gtk_box_pack_start(GTK_BOX (hbox), sw, TRUE, TRUE, 0);

        unsortedmodel = GTK_TREE_MODEL(GtkCatalogModel_new(catalogDB, TRUE));
        model = gtk_tree_model_sort_new_with_model(unsortedmodel);

        /* create tree view */
        treeview = gtk_tree_view_new_with_model(model);
        gtk_tree_view_set_rules_hint(GTK_TREE_VIEW (treeview), TRUE);
        gtk_tree_view_set_search_column(GTK_TREE_VIEW (treeview), 0);

        for (columnNum = 0; columnNum < CATALOGRECORD_FIELDCOUNT; ++columnNum) {
            CatalogRecord_FieldProperties properties;
            renderer = gtk_cell_renderer_text_new();

            column = gtk_tree_view_column_new();
            properties = CatalogRecord_getFieldProperties(columnNum);
            gtk_tree_view_column_set_title(column, properties.name);
            gtk_tree_view_column_pack_start(column, renderer, TRUE);
            gtk_tree_view_column_add_attribute(column, renderer, "text", columnNum);
            gtk_tree_view_column_set_resizable(column, TRUE);
            gtk_tree_view_column_set_min_width(column, properties.minDisplayWidth);
            treeviewWidth += properties.minDisplayWidth;
            gtk_tree_view_column_set_sort_column_id(column, columnNum);
            gtk_tree_view_column_set_alignment(column, 0.5);
            gtk_cell_renderer_set_alignment(renderer, properties.alignment, 0.5);
            gtk_tree_view_append_column(GTK_TREE_VIEW (treeview), column);
        }
        gtk_widget_set_size_request(sw, treeviewWidth + 50, 400);
        gtk_widget_show(treeview);

        g_object_unref(model);
        g_object_unref(unsortedmodel);

        gtk_container_add(GTK_CONTAINER (sw), treeview);

        bbox = gtk_vbutton_box_new();
        gtk_button_box_set_layout(GTK_BUTTON_BOX (bbox), GTK_BUTTONBOX_START);
        gtk_box_set_spacing(GTK_BOX (bbox), 5);
        gtk_box_pack_start(GTK_BOX (hbox), bbox, TRUE, TRUE, 0);
        gtk_widget_show(bbox);

        button = gtk_button_new_with_label("Ajouter");
        g_signal_connect (button, "clicked", G_CALLBACK (Catalog_add), treeview);
        gtk_container_add(GTK_CONTAINER (bbox), button);
        gtk_widget_show(button);

        button = gtk_button_new_with_label("Supprimer");
        g_signal_connect (button, "clicked", G_CALLBACK (Catalog_delete), treeview);
        gtk_container_add(GTK_CONTAINER (bbox), button);
        gtk_widget_show(button);

        button = gtk_button_new_with_label("Modifier");
        g_signal_connect (button, "clicked", G_CALLBACK (Catalog_modify), treeview);
        gtk_container_add(GTK_CONTAINER (bbox), button);

        gtk_widget_show_all(hbox);

        gtk_dialog_run(GTK_DIALOG (dialog));
        gtk_widget_destroy(dialog);
    }
}

int Catalog_select(GtkWindow * parent) {
    CatalogDB * catalogDB = CatalogDB_openOrCreate(CATALOGDB_FILENAME);
    if (catalogDB == NULL) {
        GtkWidget *dialog = gtk_message_dialog_new(parent, GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE,
                "Erreur d'ouverture de la base de catalogue \"%s\"", CATALOGDB_FILENAME);
        g_signal_connect (dialog, "response", G_CALLBACK (gtk_widget_destroy), NULL);
        gtk_widget_show(dialog);
        return -1;
    } else {
        GtkWidget * dialog;
        GtkWidget *hbox;
        GtkWidget * bbox;
        GtkWidget * sw;
        GtkTreeModel * model, *unsortedmodel;
        GtkWidget *treeview;
        GtkWidget * button;
        GtkCellRenderer * renderer;
        GtkTreeViewColumn *column;
        int columnNum;
        int treeviewWidth = 0;
        gint response;
        int recordNum = -1;

        /** Creation de la fenêtre principale */
        dialog = gtk_dialog_new_with_buttons("Gestion du catalogue des produits", parent,
                GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, GTK_STOCK_APPLY,
                GTK_RESPONSE_APPLY, NULL);

        gtk_container_set_border_width(GTK_CONTAINER (dialog), 5);

        hbox = gtk_hbox_new(FALSE, 5);
        gtk_box_pack_start(GTK_BOX (GTK_DIALOG (dialog)->vbox), hbox, TRUE, TRUE, 0);

        sw = gtk_scrolled_window_new(NULL, NULL);
        gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW (sw), GTK_SHADOW_ETCHED_IN);
        gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW (sw), GTK_POLICY_AUTOMATIC,
                GTK_POLICY_AUTOMATIC);
        gtk_box_pack_start(GTK_BOX (hbox), sw, TRUE, TRUE, 0);

        unsortedmodel = GTK_TREE_MODEL(GtkCatalogModel_new(catalogDB, TRUE));
        model = gtk_tree_model_sort_new_with_model(unsortedmodel);

        /* create tree view */
        treeview = gtk_tree_view_new_with_model(model);
        gtk_tree_view_set_rules_hint(GTK_TREE_VIEW (treeview), TRUE);
        gtk_tree_view_set_search_column(GTK_TREE_VIEW (treeview), 0);

        for (columnNum = 0; columnNum < CATALOGRECORD_FIELDCOUNT; ++columnNum) {
            CatalogRecord_FieldProperties properties;
            renderer = gtk_cell_renderer_text_new();

            column = gtk_tree_view_column_new();
            properties = CatalogRecord_getFieldProperties(columnNum);
            gtk_tree_view_column_set_title(column, properties.name);
            gtk_tree_view_column_pack_start(column, renderer, TRUE);
            gtk_tree_view_column_add_attribute(column, renderer, "text", columnNum);
            gtk_tree_view_column_set_resizable(column, TRUE);
            gtk_tree_view_column_set_min_width(column, properties.minDisplayWidth);
            treeviewWidth += properties.minDisplayWidth;
            gtk_tree_view_column_set_sort_column_id(column, columnNum);
            gtk_tree_view_column_set_alignment(column, 0.5);
            gtk_cell_renderer_set_alignment(renderer, properties.alignment, 0.5);
            gtk_tree_view_append_column(GTK_TREE_VIEW (treeview), column);
        }
        gtk_widget_set_size_request(sw, treeviewWidth + 50, 400);
        gtk_widget_show(treeview);

        g_object_unref(model);
        g_object_unref(unsortedmodel);

        gtk_container_add(GTK_CONTAINER (sw), treeview);

        bbox = gtk_vbutton_box_new();
        gtk_button_box_set_layout(GTK_BUTTON_BOX (bbox), GTK_BUTTONBOX_START);
        gtk_box_set_spacing(GTK_BOX (bbox), 5);
        gtk_box_pack_start(GTK_BOX (hbox), bbox, TRUE, TRUE, 0);
        gtk_widget_show(bbox);

        button = gtk_button_new_with_label("Ajouter");
        g_signal_connect (button, "clicked", G_CALLBACK (Catalog_add), treeview);
        gtk_container_add(GTK_CONTAINER (bbox), button);
        gtk_widget_show(button);

        button = gtk_button_new_with_label("Supprimer");
        g_signal_connect (button, "clicked", G_CALLBACK (Catalog_delete), treeview);
        gtk_container_add(GTK_CONTAINER (bbox), button);
        gtk_widget_show(button);

        button = gtk_button_new_with_label("Modifier");
        g_signal_connect (button, "clicked", G_CALLBACK (Catalog_modify), treeview);
        gtk_container_add(GTK_CONTAINER (bbox), button);

        gtk_widget_show_all(hbox);

        do {
            GtkTreeSelection * selection;
            GtkTreeIter iter;
            response = gtk_dialog_run(GTK_DIALOG (dialog));
            selection = gtk_tree_view_get_selection(GTK_TREE_VIEW (treeview));

            if (gtk_tree_selection_get_selected(selection, NULL, &iter)) {
                GtkTreePath *path;

                path = gtk_tree_model_get_path(model, &iter);
                recordNum = gtk_tree_path_get_indices(path)[0];
                gtk_tree_path_free(path);
            } else
                recordNum = -1;
        } while (response == GTK_RESPONSE_APPLY && recordNum == -1);

        gtk_widget_destroy(dialog);
        return recordNum;
    }
}
