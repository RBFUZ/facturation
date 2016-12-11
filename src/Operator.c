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
 * $Id: Operator.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <Operator.h>
#include <OperatorTable.h>

static void Operator_add(GtkWidget * UNUSED(button), GtkTreeView * treeview) {
    GtkWidget *dialog;
    GtkWidget *hbox;
    GtkWidget *table;
    GtkWidget *name_entry;
    GtkWidget *password_entry;
    GtkWidget *label;
    gint response;

    GtkTreeModel * model = gtk_tree_view_get_model(treeview);

    dialog = gtk_dialog_new_with_buttons("Nouvel opérateur", NULL, GTK_DIALOG_MODAL
            | GTK_DIALOG_DESTROY_WITH_PARENT, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL,
            GTK_RESPONSE_CANCEL, NULL);

    hbox = gtk_hbox_new(FALSE, 8);
    gtk_container_set_border_width(GTK_CONTAINER (hbox), 8);
    gtk_box_pack_start(GTK_BOX (GTK_DIALOG (dialog)->vbox), hbox, FALSE, FALSE, 0);

    table = gtk_table_new(2, 2, FALSE);
    gtk_table_set_row_spacings(GTK_TABLE (table), 4);
    gtk_table_set_col_spacings(GTK_TABLE (table), 4);
    gtk_box_pack_start(GTK_BOX (hbox), table, TRUE, TRUE, 0);
    label = gtk_label_new("Nom :");
    gtk_table_attach_defaults(GTK_TABLE (table), label, 0, 1, 0, 1);
    name_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY (name_entry), "");
    gtk_table_attach_defaults(GTK_TABLE (table), name_entry, 1, 2, 0, 1);
    gtk_label_set_mnemonic_widget(GTK_LABEL (label), name_entry);
    gtk_entry_set_max_length(GTK_ENTRY (name_entry), OPERATORTABLE_MAXNAMESIZE - 1);

    label = gtk_label_new("Mot de passe");
    gtk_table_attach_defaults(GTK_TABLE (table), label, 0, 1, 1, 2);

    password_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY (password_entry), "");
    gtk_table_attach_defaults(GTK_TABLE (table), password_entry, 1, 2, 1, 2);
    gtk_label_set_mnemonic_widget(GTK_LABEL (label), password_entry);
    gtk_entry_set_max_length(GTK_ENTRY (password_entry), OPERATORTABLE_MAXPASSWORDSIZE - 1);

    gtk_widget_show_all(hbox);
    response = gtk_dialog_run(GTK_DIALOG (dialog));

    if (response == GTK_RESPONSE_OK) {
        OperatorTable * optable = OperatorTable_loadFromFile(OPERATORDB_FILENAME);
        if (optable != NULL) {
            int i;
            GtkTreeIter iter;
            GtkListStore * store =
                    GTK_LIST_STORE(gtk_tree_model_sort_get_model(GTK_TREE_MODEL_SORT (model)));
            OperatorTable_setOperator(optable, gtk_entry_get_text(GTK_ENTRY (name_entry)),
                    gtk_entry_get_text(GTK_ENTRY (password_entry)));
            OperatorTable_saveToFile(optable, OPERATORDB_FILENAME);

            gtk_list_store_clear(store);
            for (i = 0; i < OperatorTable_getRecordCount(optable); ++i) {
                gtk_list_store_append(store, &iter);
                gtk_list_store_set(store, &iter, 0, OperatorTable_getName(optable, i), -1);
            }
            OperatorTable_destroy(optable);
        }
    }
    gtk_widget_destroy(dialog);
}

static void Operator_delete(GtkWidget * UNUSED(button), GtkTreeView * treeview) {
    GtkTreeModel * model = gtk_tree_view_get_model(treeview);
    GtkTreeSelection * selection = gtk_tree_view_get_selection(treeview);
    GtkTreeIter iter;

    if (gtk_tree_selection_get_selected(selection, NULL, &iter)) {
        int i;
        GtkTreePath *path;
        OperatorTable * optable;

        path = gtk_tree_model_get_path(model, &iter);
        i = gtk_tree_path_get_indices(path)[0];

        optable = OperatorTable_loadFromFile(OPERATORDB_FILENAME);
        if (optable != NULL) {
            OperatorTable_removeRecord(optable, i);
            OperatorTable_saveToFile(optable, OPERATORDB_FILENAME);
            OperatorTable_destroy(optable);
            if (gtk_tree_model_get_iter(gtk_tree_model_sort_get_model(GTK_TREE_MODEL_SORT (model)),
                    &iter, path)) {
                gtk_list_store_remove(
                        GTK_LIST_STORE(gtk_tree_model_sort_get_model(GTK_TREE_MODEL_SORT (model))),
                        &iter);
            }
        }
        gtk_tree_path_free(path);
    }
}

static void Operator_modify(GtkWidget * UNUSED(button), GtkTreeView * treeview) {
    GtkTreeModel * model = gtk_tree_view_get_model(treeview);
    GtkTreeSelection * selection = gtk_tree_view_get_selection(treeview);
    GtkTreeIter iter;

    if (gtk_tree_selection_get_selected(selection, NULL, &iter)) {
        gint i;
        GtkTreePath *path;
        OperatorTable * optable;

        path = gtk_tree_model_get_path(model, &iter);
        i = gtk_tree_path_get_indices(path)[0];

        optable = OperatorTable_loadFromFile(OPERATORDB_FILENAME);
        if (optable != NULL) {
            GtkWidget *dialog;
            GtkWidget *hbox;
            GtkWidget *table;
            GtkWidget *name_entry;
            GtkWidget *password_entry;
            GtkWidget *label;
            gint response;

            dialog = gtk_dialog_new_with_buttons("Edition d'un opérateur", NULL, GTK_DIALOG_MODAL
                    | GTK_DIALOG_DESTROY_WITH_PARENT, GTK_STOCK_OK, GTK_RESPONSE_OK,
                    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);

            hbox = gtk_hbox_new(FALSE, 8);
            gtk_container_set_border_width(GTK_CONTAINER (hbox), 8);
            gtk_box_pack_start(GTK_BOX (GTK_DIALOG (dialog)->vbox), hbox, FALSE, FALSE, 0);

            table = gtk_table_new(2, 2, FALSE);
            gtk_table_set_row_spacings(GTK_TABLE (table), 4);
            gtk_table_set_col_spacings(GTK_TABLE (table), 4);
            gtk_box_pack_start(GTK_BOX (hbox), table, TRUE, TRUE, 0);
            label = gtk_label_new("Nom :");
            gtk_table_attach_defaults(GTK_TABLE (table), label, 0, 1, 0, 1);
            name_entry = gtk_entry_new();
            gtk_entry_set_text(GTK_ENTRY (name_entry), OperatorTable_getName(optable, i));
            gtk_entry_set_editable(GTK_ENTRY (name_entry), FALSE);
            gtk_table_attach_defaults(GTK_TABLE (table), name_entry, 1, 2, 0, 1);
            gtk_label_set_mnemonic_widget(GTK_LABEL (label), name_entry);

            label = gtk_label_new("Mot de passe");
            gtk_table_attach_defaults(GTK_TABLE (table), label, 0, 1, 1, 2);

            password_entry = gtk_entry_new();
            gtk_entry_set_text(GTK_ENTRY (password_entry), OperatorTable_getPassword(optable, i));
            gtk_table_attach_defaults(GTK_TABLE (table), password_entry, 1, 2, 1, 2);
            gtk_label_set_mnemonic_widget(GTK_LABEL (label), password_entry);

            gtk_widget_show_all(hbox);
            response = gtk_dialog_run(GTK_DIALOG (dialog));

            if (response == GTK_RESPONSE_OK) {
                GtkListStore * store =
                        GTK_LIST_STORE(gtk_tree_model_sort_get_model(GTK_TREE_MODEL_SORT (model)));
                OperatorTable_setOperator(optable, gtk_entry_get_text(GTK_ENTRY (name_entry)),
                        gtk_entry_get_text(GTK_ENTRY (password_entry)));
                OperatorTable_saveToFile(optable, OPERATORDB_FILENAME);

                gtk_list_store_clear(store);
                for (i = 0; i < OperatorTable_getRecordCount(optable); ++i) {
                    gtk_list_store_append(store, &iter);
                    gtk_list_store_set(store, &iter, 0, OperatorTable_getName(optable, i), -1);
                }
            }
            gtk_widget_destroy(dialog);

            OperatorTable_destroy(optable);
        }
        gtk_tree_path_free(path);
    }
}

static GtkTreeModel * Operator_loadModel(void) {
    GtkListStore * store;
    GtkTreeIter iter;
    OperatorTable * table;
    int i;

    store = gtk_list_store_new(1, G_TYPE_STRING);

    table = OperatorTable_loadFromFile(OPERATORDB_FILENAME);
    if (table != NULL) {
        for (i = 0; i < OperatorTable_getRecordCount(table); ++i) {
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter, 0, OperatorTable_getName(table, i), -1);
        }
        OperatorTable_destroy(table);
    }
    return GTK_TREE_MODEL(store);
}

void Operator_manage(GtkWidget * UNUSED(widget), GtkWindow * parent) {
    GtkWidget * dialog;
    GtkWidget *hbox;
    GtkWidget * bbox;
    GtkWidget * sw;
    GtkTreeModel * model;
    GtkWidget *treeview;
    GtkWidget * button;
    GtkCellRenderer * renderer;
    GtkTreeViewColumn *column;

    /** Creation de la fenêtre principale */
    dialog = gtk_dialog_new_with_buttons("Gestion des opérateurs", parent, GTK_DIALOG_MODAL
            | GTK_DIALOG_DESTROY_WITH_PARENT, GTK_STOCK_CLOSE, GTK_RESPONSE_CLOSE, NULL);

    gtk_container_set_border_width(GTK_CONTAINER (dialog), 5);

    hbox = gtk_hbox_new(FALSE, 5);
    gtk_box_pack_start(GTK_BOX (GTK_DIALOG (dialog)->vbox), hbox, TRUE, TRUE, 0);

    sw = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW (sw), GTK_SHADOW_ETCHED_IN);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW (sw), GTK_POLICY_AUTOMATIC,
            GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX (hbox), sw, TRUE, TRUE, 0);

    model = gtk_tree_model_sort_new_with_model(Operator_loadModel());

    /* create tree view */
    treeview = gtk_tree_view_new_with_model(model);
    gtk_tree_view_set_rules_hint(GTK_TREE_VIEW (treeview), TRUE);
    gtk_tree_view_set_search_column(GTK_TREE_VIEW (treeview), 0);

    renderer = gtk_cell_renderer_text_new();

    column = gtk_tree_view_column_new();
    gtk_tree_view_column_set_title(column, "Opérateur");
    gtk_tree_view_column_pack_start(column, renderer, TRUE);
    gtk_tree_view_column_add_attribute(column, renderer, "text", 0);
    gtk_tree_view_column_set_resizable(column, TRUE);
    gtk_tree_view_column_set_min_width(column, 300);
    gtk_tree_view_column_set_sort_column_id(column, 0);
    gtk_tree_view_column_set_alignment(column, 0.5);
    gtk_cell_renderer_set_alignment(renderer, 0, 0.5);
    gtk_tree_view_append_column(GTK_TREE_VIEW (treeview), column);

    gtk_widget_set_size_request(sw, 350, 400);
    gtk_widget_show(treeview);

    g_object_unref(model);

    gtk_container_add(GTK_CONTAINER (sw), treeview);

    bbox = gtk_vbutton_box_new();
    gtk_button_box_set_layout(GTK_BUTTON_BOX (bbox), GTK_BUTTONBOX_START);
    gtk_box_set_spacing(GTK_BOX (bbox), 5);
    gtk_box_pack_start(GTK_BOX (hbox), bbox, TRUE, TRUE, 0);
    gtk_widget_show(bbox);

    button = gtk_button_new_with_label("Ajouter");
    g_signal_connect (button, "clicked", G_CALLBACK (Operator_add), treeview);
    gtk_container_add(GTK_CONTAINER (bbox), button);
    gtk_widget_show(button);

    button = gtk_button_new_with_label("Supprimer");
    g_signal_connect (button, "clicked", G_CALLBACK (Operator_delete), treeview);
    gtk_container_add(GTK_CONTAINER (bbox), button);
    gtk_widget_show(button);

    button = gtk_button_new_with_label("Modifier");
    g_signal_connect (button, "clicked", G_CALLBACK (Operator_modify), treeview);
    gtk_container_add(GTK_CONTAINER (bbox), button);

    gtk_widget_show_all(hbox);

    gtk_dialog_run(GTK_DIALOG (dialog));
    gtk_widget_destroy(dialog);

}

char * Operator_identify(GtkWindow * parent) {
    GtkWidget *dialog;
    GtkWidget *table;
    GtkWidget *name_entry;
    GtkWidget *password_entry;
    GtkWidget *label;
    gint response;
    char * result = NULL;

    OperatorTable * optable = OperatorTable_loadFromFile(OPERATORDB_FILENAME);

    if (optable == NULL) {
        GtkWidget * errordialog = gtk_message_dialog_new(parent, GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "Aucun opérateur n'a été défini");
        g_signal_connect (errordialog, "response", G_CALLBACK (gtk_widget_destroy), NULL);
        gtk_widget_show(errordialog);
    } else {

        dialog = gtk_dialog_new_with_buttons("Identification de l'opérateur", parent,
                GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, GTK_STOCK_OK, GTK_RESPONSE_OK,
                GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);

        gtk_container_set_border_width(GTK_CONTAINER (GTK_DIALOG (dialog)->vbox), 8);

        table = gtk_table_new(2, 2, FALSE);
        gtk_table_set_row_spacings(GTK_TABLE (table), 4);
        gtk_table_set_col_spacings(GTK_TABLE (table), 4);
        gtk_box_pack_start(GTK_BOX (GTK_DIALOG (dialog)->vbox), table, TRUE, TRUE, 0);
        label = gtk_label_new("Nom :");
        gtk_table_attach_defaults(GTK_TABLE (table), label, 0, 1, 0, 1);
        name_entry = gtk_entry_new();
        gtk_entry_set_text(GTK_ENTRY (name_entry), "");
        gtk_table_attach_defaults(GTK_TABLE (table), name_entry, 1, 2, 0, 1);
        gtk_label_set_mnemonic_widget(GTK_LABEL (label), name_entry);
        gtk_entry_set_max_length(GTK_ENTRY (name_entry), OPERATORTABLE_MAXNAMESIZE - 1);

        label = gtk_label_new("Mot de passe");
        gtk_table_attach_defaults(GTK_TABLE (table), label, 0, 1, 1, 2);

        password_entry = gtk_entry_new();
        gtk_entry_set_text(GTK_ENTRY (password_entry), "");
        gtk_table_attach_defaults(GTK_TABLE (table), password_entry, 1, 2, 1, 2);
        gtk_label_set_mnemonic_widget(GTK_LABEL (label), password_entry);
        gtk_entry_set_max_length(GTK_ENTRY (password_entry), OPERATORTABLE_MAXPASSWORDSIZE - 1);
        gtk_entry_set_invisible_char(GTK_ENTRY (password_entry), '*');

        gtk_widget_show_all(GTK_DIALOG (dialog)->vbox);
        response = gtk_dialog_run(GTK_DIALOG (dialog));

        if (response == GTK_RESPONSE_OK) {
            int operator = OperatorTable_findOperator(optable, gtk_entry_get_text(
                    GTK_ENTRY (name_entry)));

            if (operator != -1 && strcmp(OperatorTable_getPassword(optable, operator),
                    gtk_entry_get_text(GTK_ENTRY (password_entry))) == 0) {
                result = duplicateString(gtk_entry_get_text(GTK_ENTRY (name_entry)));
            } else {
                GtkWidget * errordialog = gtk_message_dialog_new(parent,
                        GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE,
                        "Opérateur ou mot de passe invalide");
                g_signal_connect (errordialog, "response", G_CALLBACK (gtk_widget_destroy), NULL);
                gtk_widget_show(errordialog);
            }
        }
        gtk_widget_destroy(dialog);
        OperatorTable_destroy(optable);
    }
    return result;
}
