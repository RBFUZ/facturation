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
 * $Id: GtkCustomerModel.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <GtkCustomerModel.h>
#include <CustomerDB.h>
#include <CustomerRecord.h>
#include <CustomerRecordEditor.h>

/** @defgroup GtkCustomerModel A specialized treeview model backed by our data structure instead of GTK+ ones
 * GTK+ related stuff. It has no interest for the teaching.
 * @{
 */

/* boring declarations of local functions */

static void GtkCustomerModel_init(GtkCustomerModel *pkg_tree);
static void GtkCustomerModel_class_init(GtkCustomerModelClass *klass);
static void GtkCustomerModel_tree_model_init(GtkTreeModelIface *iface);
static void GtkCustomerModel_finalize(GObject *object);
static GtkTreeModelFlags GtkCustomerModel_get_flags(GtkTreeModel *tree_model);
static gint GtkCustomerModel_get_n_columns(GtkTreeModel *tree_model);
static GType GtkCustomerModel_get_column_type(GtkTreeModel *tree_model,
        gint recordIndex);
static gboolean GtkCustomerModel_get_iter(GtkTreeModel *tree_model,
        GtkTreeIter *iter, GtkTreePath *path);
static GtkTreePath *GtkCustomerModel_get_path(GtkTreeModel *tree_model,
        GtkTreeIter *iter);
static void GtkCustomerModel_get_value(GtkTreeModel *tree_model,
        GtkTreeIter *iter, gint column, GValue *value);
static gboolean GtkCustomerModel_iter_next(GtkTreeModel *tree_model,
        GtkTreeIter *iter);
static gboolean GtkCustomerModel_iter_children(GtkTreeModel *tree_model,
        GtkTreeIter *iter, GtkTreeIter *parent);
static gboolean GtkCustomerModel_iter_has_child(GtkTreeModel *tree_model,
        GtkTreeIter *iter);
static gint GtkCustomerModel_iter_n_children(GtkTreeModel *tree_model,
        GtkTreeIter *iter);
static gboolean GtkCustomerModel_iter_nth_child(GtkTreeModel *tree_model,
        GtkTreeIter *iter, GtkTreeIter *parent, gint n);
static gboolean GtkCustomerModel_iter_parent(GtkTreeModel *tree_model,
        GtkTreeIter *iter, GtkTreeIter *child);
static GObjectClass *parent_class = NULL; /* GObject stuff - nothing to worry about */

/*****************************************************************************
 *
 *  GtkCustomerModel_get_type: here we register our new type and its interfaces
 *                        with the type system. If you want to implement
 *                        additional interfaces like GtkTreeSortable, you
 *                        will need to do it here.
 *
 *****************************************************************************/

GType GtkCustomerModel_get_type(void) {
    static GType GtkCustomerModel_type = 0;

    /* Some boilerplate type registration stuff */
    if (GtkCustomerModel_type == 0) {
        static const GTypeInfo GtkCustomerModel_info = {
                sizeof(GtkCustomerModelClass), NULL, /* base_init */
                NULL, /* base_finalize */
                (GClassInitFunc) GtkCustomerModel_class_init, NULL, /* class finalize */
                NULL, /* class_data */
                sizeof(GtkCustomerModel), 0, /* n_preallocs */
                (GInstanceInitFunc) GtkCustomerModel_init, NULL };
        static const GInterfaceInfo tree_model_info = {
                (GInterfaceInitFunc) GtkCustomerModel_tree_model_init, NULL,
                NULL };

        /* First register the new derived type with the GObject type system */
        GtkCustomerModel_type = g_type_register_static(G_TYPE_OBJECT,
                "GtkCustomerModel", &GtkCustomerModel_info, (GTypeFlags) 0);

        /* Now register our GtkTreeModel interface with the type system */
        g_type_add_interface_static(GtkCustomerModel_type, GTK_TYPE_TREE_MODEL,
                &tree_model_info);
    }

    return GtkCustomerModel_type;
}

/*****************************************************************************
 *
 *  GtkCustomerModel_class_init: more boilerplate GObject/GType stuff.
 *                          Init callback for the type system,
 *                          called once when our new class is created.
 *
 *****************************************************************************/

static void GtkCustomerModel_class_init(GtkCustomerModelClass *klass) {
    GObjectClass *object_class;

    parent_class = (GObjectClass*) g_type_class_peek_parent(klass);
    object_class = (GObjectClass*) klass;

    object_class->finalize = GtkCustomerModel_finalize;
}

/*****************************************************************************
 *
 *  GtkCustomerModel_tree_model_init: init callback for the interface registration
 *                               in custom_list_get_type. Here we override
 *                               the GtkTreeModel interface functions that
 *                               we implement.
 *
 *****************************************************************************/

static void GtkCustomerModel_tree_model_init(GtkTreeModelIface *iface) {
    iface->get_flags = GtkCustomerModel_get_flags;
    iface->get_n_columns = GtkCustomerModel_get_n_columns;
    iface->get_column_type = GtkCustomerModel_get_column_type;
    iface->get_iter = GtkCustomerModel_get_iter;
    iface->get_path = GtkCustomerModel_get_path;
    iface->get_value = GtkCustomerModel_get_value;
    iface->iter_next = GtkCustomerModel_iter_next;
    iface->iter_children = GtkCustomerModel_iter_children;
    iface->iter_has_child = GtkCustomerModel_iter_has_child;
    iface->iter_n_children = GtkCustomerModel_iter_n_children;
    iface->iter_nth_child = GtkCustomerModel_iter_nth_child;
    iface->iter_parent = GtkCustomerModel_iter_parent;
}

/*****************************************************************************
 *
 *  GtkCustomerModel_init: this is called everytime a new custom list object
 *                    instance is created (we do that in custom_list_new).
 *                    Initialise the list structure's fields here.
 *
 *****************************************************************************/

static void GtkCustomerModel_init(GtkCustomerModel *custom_list) {
    custom_list->clientDB = NULL;
    custom_list->shouldCloseDB = FALSE;
    custom_list->stamp = (gint) g_random_int(); /* Random int to check whether an iter belongs to our model */

}

/*****************************************************************************
 *
 *  GtkCustomerModel_finalize: this is called just before a custom list is
 *                        destroyed. Free dynamically allocated memory here.
 *
 *****************************************************************************/

static void GtkCustomerModel_finalize(GObject *object) {
    GtkCustomerModel *custom_list = GTKCUSTOMERMODEL(object);
    if (custom_list->shouldCloseDB)
        CustomerDB_close(custom_list->clientDB);

    custom_list->clientDB = NULL;

    /* must chain up - finalize parent */
    (*parent_class->finalize)(object);
}

/*****************************************************************************
 *
 *  GtkCustomerModel_get_flags: tells the rest of the world whether our tree model
 *                         has any special characteristics. In our case,
 *                         we have a list model (instead of a tree), and each
 *                         tree iter is valid as long as the row in question
 *                         exists, as it only contains a pointer to the CatalogDB.
 *
 *****************************************************************************/

static GtkTreeModelFlags GtkCustomerModel_get_flags(GtkTreeModel *tree_model) {
    g_return_val_if_fail (GTKCUSTOMERMODEL_IS_LIST(tree_model), (GtkTreeModelFlags)0);

    return (GTK_TREE_MODEL_LIST_ONLY | GTK_TREE_MODEL_ITERS_PERSIST);
}

/*****************************************************************************
 *
 *  GtkCustomerModel_get_n_columns: tells the rest of the world how many data
 *                             columns we export via the tree model interface
 *
 *****************************************************************************/

static gint GtkCustomerModel_get_n_columns(GtkTreeModel *tree_model) {
    g_return_val_if_fail (GTKCUSTOMERMODEL_IS_LIST(tree_model), 0);

    return CUSTOMERRECORD_FIELDCOUNT;
}

/*****************************************************************************
 *
 *  GtkCustomerModel_get_column_type: tells the rest of the world which type of
 *                               data an exported model column contains
 *
 *****************************************************************************/

static GType GtkCustomerModel_get_column_type(GtkTreeModel *tree_model,
        gint UNUSED(recordIndex)) {
    g_return_val_if_fail (GTKCUSTOMERMODEL_IS_LIST(tree_model), G_TYPE_INVALID);

    return G_TYPE_STRING;
}

/*****************************************************************************
 *
 *  GtkCustomerModel_get_iter: converts a tree path (physical position) into a
 *                        tree iter structure (the content of the iter
 *                        fields will only be used internally by our model).
 *                        We simply store a pointer to our CustomRecord
 *                        structure that represents that row in the tree iter.
 *
 *****************************************************************************/

static gboolean GtkCustomerModel_get_iter(GtkTreeModel *tree_model,
        GtkTreeIter *iter, GtkTreePath *path) {
    GtkCustomerModel *custom_list;
    gint *indices, n, depth;

    g_assert(GTKCUSTOMERMODEL_IS_LIST(tree_model));
    g_assert(path!=NULL);

    custom_list = GTKCUSTOMERMODEL(tree_model);

    indices = gtk_tree_path_get_indices(path);
    depth = gtk_tree_path_get_depth(path);

    /* we do not allow children */
    g_assert(depth == 1); /* depth 1 = top level; a list only has top level nodes and no children */

    n = indices[0]; /* the n-th top level row */

    if (n >= CustomerDB_getRecordCount(custom_list->clientDB) || n < 0)
        return FALSE;

    /* We simply store a pointer to our custom record in the iter */
    iter->stamp = custom_list->stamp;
    iter->user_data = GINT_TO_POINTER(n);
    iter->user_data2 = NULL; /* unused */
    iter->user_data3 = NULL; /* unused */

    return TRUE;
}

/*****************************************************************************
 *
 *  custom_list_get_path: converts a tree iter into a tree path (ie. the
 *                        physical position of that row in the list).
 *
 *****************************************************************************/

static GtkTreePath *
GtkCustomerModel_get_path(GtkTreeModel *tree_model, GtkTreeIter *iter) {
    GtkTreePath *path;
    gint recordNum;
    /*GtkCustomerModel *custom_list;*/

    g_return_val_if_fail (GTKCUSTOMERMODEL_IS_LIST(tree_model), NULL);
    g_return_val_if_fail (iter != NULL, NULL);

    /*custom_list = GTKCUSTOMERMODEL(tree_model);*/

    recordNum = GPOINTER_TO_INT(iter->user_data);

    path = gtk_tree_path_new();
    gtk_tree_path_append_index(path, recordNum);

    return path;
}

/*****************************************************************************
 *
 *  GtkCustomerModel_get_value: Returns a row's exported data columns
 *                         (_get_value is what gtk_tree_model_get uses)
 *
 *****************************************************************************/

static void GtkCustomerModel_get_value(GtkTreeModel *tree_model,
        GtkTreeIter *iter, gint column, GValue *value) {
    GtkCustomerModel *custom_list;
    gint recordNum;
    char * content;

    g_return_if_fail (GTKCUSTOMERMODEL_IS_LIST (tree_model));
    custom_list = GTKCUSTOMERMODEL(tree_model);
    g_return_if_fail(custom_list->clientDB != NULL);
    g_return_if_fail(iter != NULL);
    g_return_if_fail(column < CUSTOMERRECORD_FIELDCOUNT);

    g_value_init(value, G_TYPE_STRING);

    recordNum = GPOINTER_TO_INT(iter->user_data);

    if (recordNum >= CustomerDB_getRecordCount(custom_list->clientDB))
        g_return_if_reached();

    content = CustomerDB_getFieldValueAsString(custom_list->clientDB, recordNum, column);
    g_value_set_string(value, content);
    free(content);
}

/*****************************************************************************
 *
 *  GtkCustomerModel_iter_next: Takes an iter structure and sets it to point
 *                         to the next row.
 *
 *****************************************************************************/

static gboolean GtkCustomerModel_iter_next(GtkTreeModel *tree_model,
        GtkTreeIter *iter) {
    gint recordNum;
    GtkCustomerModel *custom_list;

    g_return_val_if_fail (GTKCUSTOMERMODEL_IS_LIST (tree_model), FALSE);

    if (iter == NULL)
        return FALSE;

    custom_list = GTKCUSTOMERMODEL(tree_model);

    recordNum = GPOINTER_TO_INT(iter->user_data);
    recordNum++;

    if (recordNum >= CustomerDB_getRecordCount(custom_list->clientDB))
        return FALSE;

    iter->stamp = custom_list->stamp;
    iter->user_data = GINT_TO_POINTER(recordNum);

    return TRUE;
}

/*****************************************************************************
 *
 *  GtkCustomerModel_iter_children: Returns TRUE or FALSE depending on whether
 *                             the row specified by 'parent' has any children.
 *                             If it has children, then 'iter' is set to
 *                             point to the first child. Special case: if
 *                             'parent' is NULL, then the first top-level
 *                             row should be returned if it exists.
 *
 *****************************************************************************/

static gboolean GtkCustomerModel_iter_children(GtkTreeModel *tree_model,
        GtkTreeIter *iter, GtkTreeIter *parent) {
    GtkCustomerModel *custom_list;

    g_return_val_if_fail (parent == NULL || parent->user_data != NULL, FALSE);

    /* this is a list, nodes have no children */
    if (parent)
        return FALSE;

    /* parent == NULL is a special case; we need to return the first top-level row */

    g_return_val_if_fail (GTKCUSTOMERMODEL_IS_LIST (tree_model), FALSE);

    custom_list = GTKCUSTOMERMODEL(tree_model);

    /* No rows => no first row */
    if (CustomerDB_getRecordCount(custom_list->clientDB) == 0)
        return FALSE;

    /* Set iter to first item in list */
    iter->stamp = custom_list->stamp;
    iter->user_data = (gpointer) 0;

    return TRUE;
}

/*****************************************************************************
 *
 *  GtkCustomerModel_iter_has_child: Returns TRUE or FALSE depending on whether
 *                              the row specified by 'iter' has any children.
 *                              We only have a list and thus no children.
 *
 *****************************************************************************/

static gboolean GtkCustomerModel_iter_has_child(GtkTreeModel * UNUSED(tree_model),
        GtkTreeIter * UNUSED(iter)) {
    return FALSE;
}

/*****************************************************************************
 *
 *  GtkCustomerModel_iter_n_children: Returns the number of children the row
 *                               specified by 'iter' has. This is usually 0,
 *                               as we only have a list and thus do not have
 *                               any children to any rows. A special case is
 *                               when 'iter' is NULL, in which case we need
 *                               to return the number of top-level nodes,
 *                               ie. the number of rows in our list.
 *
 *****************************************************************************/

static gint GtkCustomerModel_iter_n_children(GtkTreeModel *tree_model,
        GtkTreeIter *iter) {
    GtkCustomerModel *custom_list;

    g_return_val_if_fail (GTKCUSTOMERMODEL_IS_LIST (tree_model), -1);
    g_return_val_if_fail (iter == NULL, FALSE);

    custom_list = GTKCUSTOMERMODEL(tree_model);

    /* special case: if iter == NULL, return number of top-level rows */
    if (!iter)
        return CustomerDB_getRecordCount(custom_list->clientDB);

    return 0; /* otherwise, this is easy again for a list */
}

/*****************************************************************************
 *
 *  GtkCustomerModel_iter_nth_child: If the row specified by 'parent' has any
 *                              children, set 'iter' to the n-th child and
 *                              return TRUE if it exists, otherwise FALSE.
 *                              A special case is when 'parent' is NULL, in
 *                              which case we need to set 'iter' to the n-th
 *                              row if it exists.
 *
 *****************************************************************************/

static gboolean GtkCustomerModel_iter_nth_child(GtkTreeModel *tree_model,
        GtkTreeIter *iter, GtkTreeIter *parent, gint n) {
    GtkCustomerModel *custom_list;

    g_return_val_if_fail (GTKCUSTOMERMODEL_IS_LIST (tree_model), FALSE);

    custom_list = GTKCUSTOMERMODEL(tree_model);

    /* a list has only top-level rows */
    if (parent)
        return FALSE;

    /* special case: if parent == NULL, set iter to n-th top-level row */

    if (n >= CustomerDB_getRecordCount(custom_list->clientDB))
        return FALSE;

    iter->stamp = custom_list->stamp;
    iter->user_data = GINT_TO_POINTER(n);

    return TRUE;
}

/*****************************************************************************
 *
 *  CustomerDB_getRecordCount(custom_list->clientDB)_iter_parent: Point 'iter' to the parent node of 'child'. As
 *                           we have a list and thus no children and no
 *                           parents of children, we can just return FALSE.
 *
 *****************************************************************************/

static gboolean GtkCustomerModel_iter_parent(GtkTreeModel * UNUSED(tree_model),
        GtkTreeIter * UNUSED(iter), GtkTreeIter * UNUSED(child)) {
    return FALSE;
}

/*****************************************************************************
 *
 *  GtkCustomerModel_new:  This is what you use in your own code to create a
 *                    new custom list tree model for you to use.
 *
 *****************************************************************************/

GtkCustomerModel * GtkCustomerModel_new(CustomerDB * clientDB,
        int shouldCloseDB) {
    GtkCustomerModel *newcustomlist;

    newcustomlist = (GtkCustomerModel*) g_object_new(GTKCUSTOMERMODEL_TYPE_LIST,
            NULL);

    g_assert( newcustomlist != NULL );
    newcustomlist->clientDB = clientDB;
    newcustomlist->shouldCloseDB = shouldCloseDB;

    return newcustomlist;
}

void GtkCustomerModel_append_record(GtkCustomerModel * custom_list) {
    GtkTreeIter iter;
    GtkTreePath *path;
    CustomerRecord record;

    g_return_if_fail (GTKCUSTOMERMODEL_IS_LIST(custom_list));

    CustomerRecord_init(&record);
    if (CustomerRecord_edit(&record)) {
        CustomerDB_appendRecord(custom_list->clientDB, &record);

        /* inform the tree view and other interested objects
         * (e.g. tree row references) that we have inserted
         * a new row, and where it was inserted */

        path = gtk_tree_path_new();
        gtk_tree_path_append_index(path, CustomerDB_getRecordCount(custom_list->clientDB) - 1);

        GtkCustomerModel_get_iter(GTK_TREE_MODEL(custom_list), &iter, path);

        gtk_tree_model_row_inserted(GTK_TREE_MODEL(custom_list), path, &iter);

        gtk_tree_path_free(path);
    }
    CustomerRecord_finalize(&record);
}

void GtkCustomerModel_remove_record(GtkCustomerModel * custom_list, gint recordIndex) {
    if (CustomerDB_getRecordCount(custom_list->clientDB) > 0) {
        GtkTreePath *path;

        g_return_if_fail (GTKCUSTOMERMODEL_IS_LIST(custom_list));
        g_return_if_fail (recordIndex < CustomerDB_getRecordCount(custom_list->clientDB));

        path = gtk_tree_path_new();
        gtk_tree_path_append_index(path, CustomerDB_getRecordCount(custom_list->clientDB)-1);

        gtk_tree_model_row_deleted(GTK_TREE_MODEL(custom_list), path);

        gtk_tree_path_free(path);

        CustomerDB_removeRecord(custom_list->clientDB, recordIndex);
    }
}

void GtkCustomerModel_change_record(GtkCustomerModel * custom_list,
        gint recordIndex) {
    GtkTreeIter iter;
    GtkTreePath *path;
    CustomerRecord record;

    g_return_if_fail (GTKCUSTOMERMODEL_IS_LIST(custom_list));
    g_return_if_fail (recordIndex < CustomerDB_getRecordCount(custom_list->clientDB));

    CustomerRecord_init(&record);
    CustomerDB_readRecord(custom_list->clientDB, recordIndex, &record);
    if (CustomerRecord_edit(&record)) {
        CustomerDB_writeRecord(custom_list->clientDB, recordIndex, &record);

        /* inform the tree view and other interested objects
         * (e.g. tree row references) that we have inserted
         * a new row, and where it was inserted */

        path = gtk_tree_path_new();
        gtk_tree_path_append_index(path, recordIndex);

        GtkCustomerModel_get_iter(GTK_TREE_MODEL(custom_list), &iter, path);

        gtk_tree_model_row_changed(GTK_TREE_MODEL(custom_list), path, &iter);

        gtk_tree_path_free(path);
    }
    CustomerRecord_finalize(&record);
}

/** @} */

