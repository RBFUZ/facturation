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
 * $Id: GtkCatalogModel.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <GtkCatalogModel.h>
#include <CatalogDB.h>
#include <CatalogRecord.h>
#include <CatalogRecordEditor.h>

/** @defgroup GtkCatalogModel A specialized treeview model backed by our data structure instead of GTK+ ones
 * It has no interest for the teaching.
 * @{
 */

/* boring declarations of local functions */

static void GtkCatalogModel_init(GtkCatalogModel *pkg_tree);
static void GtkCatalogModel_class_init(GtkCatalogModelClass *klass);
static void GtkCatalogModel_tree_model_init(GtkTreeModelIface *iface);
static void GtkCatalogModel_finalize(GObject *object);
static GtkTreeModelFlags GtkCatalogModel_get_flags(GtkTreeModel *tree_model);
static gint GtkCatalogModel_get_n_columns(GtkTreeModel *tree_model);
static GType GtkCatalogModel_get_column_type(GtkTreeModel *tree_model, gint recordIndex);
static gboolean GtkCatalogModel_get_iter(GtkTreeModel *tree_model, GtkTreeIter *iter,
        GtkTreePath *path);
static GtkTreePath *GtkCatalogModel_get_path(GtkTreeModel *tree_model, GtkTreeIter *iter);
static void GtkCatalogModel_get_value(GtkTreeModel *tree_model, GtkTreeIter *iter, gint column,
        GValue *value);
static gboolean GtkCatalogModel_iter_next(GtkTreeModel *tree_model, GtkTreeIter *iter);
static gboolean GtkCatalogModel_iter_children(GtkTreeModel *tree_model, GtkTreeIter *iter,
        GtkTreeIter *parent);
static gboolean GtkCatalogModel_iter_has_child(GtkTreeModel *tree_model, GtkTreeIter *iter);
static gint GtkCatalogModel_iter_n_children(GtkTreeModel *tree_model, GtkTreeIter *iter);
static gboolean GtkCatalogModel_iter_nth_child(GtkTreeModel *tree_model, GtkTreeIter *iter,
        GtkTreeIter *parent, gint n);
static gboolean GtkCatalogModel_iter_parent(GtkTreeModel *tree_model, GtkTreeIter *iter,
        GtkTreeIter *child);
static GObjectClass *parent_class = NULL; /* GObject stuff - nothing to worry about */

/* GtkCatalogModel_get_type: here we register our new type and its interfaces
 *                        with the type system. If you want to implement
 *                        additional interfaces like GtkTreeSortable, you
 *                        will need to do it here.
 *
 */
GType GtkCatalogModel_get_type(void) {
    static GType GtkCatalogModel_type = 0;

    /* Some boilerplate type registration stuff */
    if (GtkCatalogModel_type == 0) {
        static const GTypeInfo GtkCatalogModel_info = { sizeof(GtkCatalogModelClass), NULL, /* base_init */
        NULL, /* base_finalize */
        (GClassInitFunc) GtkCatalogModel_class_init, NULL, /* class finalize */
        NULL, /* class_data */
        sizeof(GtkCatalogModel), 0, /* n_preallocs */
        (GInstanceInitFunc) GtkCatalogModel_init, NULL };
        static const GInterfaceInfo tree_model_info = {
                (GInterfaceInitFunc) GtkCatalogModel_tree_model_init, NULL, NULL };

        /* First register the new derived type with the GObject type system */
        GtkCatalogModel_type = g_type_register_static(G_TYPE_OBJECT, "GtkCatalogModel",
                &GtkCatalogModel_info, (GTypeFlags) 0);

        /* Now register our GtkTreeModel interface with the type system */
        g_type_add_interface_static(GtkCatalogModel_type, GTK_TYPE_TREE_MODEL, &tree_model_info);
    }
    return GtkCatalogModel_type;
}

/*****************************************************************************
 *
 *  GtkCatalogModel_class_init: more boilerplate GObject/GType stuff.
 *                          Init callback for the type system,
 *                          called once when our new class is created.
 *
 *****************************************************************************/

static void GtkCatalogModel_class_init(GtkCatalogModelClass *klass) {
    GObjectClass *object_class;

    parent_class = (GObjectClass*) g_type_class_peek_parent(klass);
    object_class = (GObjectClass*) klass;

    object_class->finalize = GtkCatalogModel_finalize;
}

/*****************************************************************************
 *
 *  GtkCatalogModel_tree_model_init: init callback for the interface registration
 *                               in custom_list_get_type. Here we override
 *                               the GtkTreeModel interface functions that
 *                               we implement.
 *
 *****************************************************************************/

static void GtkCatalogModel_tree_model_init(GtkTreeModelIface *iface) {
    iface->get_flags = GtkCatalogModel_get_flags;
    iface->get_n_columns = GtkCatalogModel_get_n_columns;
    iface->get_column_type = GtkCatalogModel_get_column_type;
    iface->get_iter = GtkCatalogModel_get_iter;
    iface->get_path = GtkCatalogModel_get_path;
    iface->get_value = GtkCatalogModel_get_value;
    iface->iter_next = GtkCatalogModel_iter_next;
    iface->iter_children = GtkCatalogModel_iter_children;
    iface->iter_has_child = GtkCatalogModel_iter_has_child;
    iface->iter_n_children = GtkCatalogModel_iter_n_children;
    iface->iter_nth_child = GtkCatalogModel_iter_nth_child;
    iface->iter_parent = GtkCatalogModel_iter_parent;
}

/*****************************************************************************
 *
 *  GtkCatalogModel_init: this is called everytime a new custom list object
 *                    instance is created (we do that in custom_list_new).
 *                    Initialise the list structure's fields here.
 *
 *****************************************************************************/

static void GtkCatalogModel_init(GtkCatalogModel *custom_list) {
    custom_list->catalogDB = NULL;
    custom_list->shouldCloseDB = FALSE;
    custom_list->stamp = (gint) g_random_int(); /* Random int to check whether an iter belongs to our model */

}

/*****************************************************************************
 *
 *  GtkCatalogModel_finalize: this is called just before a custom list is
 *                        destroyed. Free dynamically allocated memory here.
 *
 *****************************************************************************/

static void GtkCatalogModel_finalize(GObject *object) {
    GtkCatalogModel *custom_list = GTKCATALOGMODEL(object);
    if (custom_list->shouldCloseDB)
        CatalogDB_close(custom_list->catalogDB);

    custom_list->catalogDB = NULL;

    /* must chain up - finalize parent */
    (*parent_class->finalize)(object);
}

/*****************************************************************************
 *
 *  GtkCatalogModel_get_flags: tells the rest of the world whether our tree model
 *                         has any special characteristics. In our case,
 *                         we have a list model (instead of a tree), and each
 *                         tree iter is valid as long as the row in question
 *                         exists, as it only contains a pointer to the CatalogDB.
 *
 *****************************************************************************/

static GtkTreeModelFlags GtkCatalogModel_get_flags(GtkTreeModel *tree_model) {
    g_return_val_if_fail (GTKCATALOGMODEL_IS_LIST(tree_model), (GtkTreeModelFlags)0);

    return (GTK_TREE_MODEL_LIST_ONLY | GTK_TREE_MODEL_ITERS_PERSIST);
}

/*****************************************************************************
 *
 *  GtkCatalogModel_get_n_columns: tells the rest of the world how many data
 *                             columns we export via the tree model interface
 *
 *****************************************************************************/

static gint GtkCatalogModel_get_n_columns(GtkTreeModel *tree_model) {
    g_return_val_if_fail (GTKCATALOGMODEL_IS_LIST(tree_model), 0);

    return CATALOGRECORD_FIELDCOUNT;
}

/*****************************************************************************
 *
 *  GtkCatalogModel_get_column_type: tells the rest of the world which type of
 *                               data an exported model column contains
 *
 *****************************************************************************/

static GType GtkCatalogModel_get_column_type(GtkTreeModel *tree_model, gint UNUSED( recordIndex)) {
    g_return_val_if_fail (GTKCATALOGMODEL_IS_LIST(tree_model), G_TYPE_INVALID);

    return G_TYPE_STRING;
}

/*****************************************************************************
 *
 *  GtkCatalogModel_get_iter: converts a tree path (physical position) into a
 *                        tree iter structure (the content of the iter
 *                        fields will only be used internally by our model).
 *                        We simply store a pointer to our CustomRecord
 *                        structure that represents that row in the tree iter.
 *
 *****************************************************************************/

static gboolean GtkCatalogModel_get_iter(GtkTreeModel *tree_model, GtkTreeIter *iter,
        GtkTreePath *path) {
    GtkCatalogModel *custom_list;
    gint *indices, n, depth;

    g_assert(GTKCATALOGMODEL_IS_LIST(tree_model));
    g_assert(path!=NULL);

    custom_list = GTKCATALOGMODEL(tree_model);

    indices = gtk_tree_path_get_indices(path);
    depth = gtk_tree_path_get_depth(path);

    /* we do not allow children */
    g_assert(depth == 1); /* depth 1 = top level; a list only has top level nodes and no children */

    n = indices[0]; /* the n-th top level row */

    if (n >= CatalogDB_getRecordCount(custom_list->catalogDB) || n < 0)
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
GtkCatalogModel_get_path(GtkTreeModel *tree_model, GtkTreeIter *iter) {
    GtkTreePath *path;
    gint recordNum;
    /*GtkCatalogModel *custom_list;*/

    g_return_val_if_fail (GTKCATALOGMODEL_IS_LIST(tree_model), NULL);
    g_return_val_if_fail (iter != NULL, NULL);

    /*custom_list = GTKCATALOGMODEL(tree_model);*/

    recordNum = GPOINTER_TO_INT(iter->user_data);

    path = gtk_tree_path_new();
    gtk_tree_path_append_index(path, recordNum);

    return path;
}

/*****************************************************************************
 *
 *  GtkCatalogModel_get_value: Returns a row's exported data columns
 *                         (_get_value is what gtk_tree_model_get uses)
 *
 *****************************************************************************/

static void GtkCatalogModel_get_value(GtkTreeModel *tree_model, GtkTreeIter *iter, gint column,
        GValue *value) {
    GtkCatalogModel *custom_list;
    gint recordNum;
    char * content;

    g_return_if_fail (GTKCATALOGMODEL_IS_LIST (tree_model));
    custom_list = GTKCATALOGMODEL(tree_model);
    g_return_if_fail(custom_list->catalogDB != NULL);
    g_return_if_fail (iter != NULL);
    g_return_if_fail (column < CATALOGRECORD_FIELDCOUNT);

    g_value_init(value, G_TYPE_STRING);

    recordNum = GPOINTER_TO_INT(iter->user_data);

    if (recordNum >= CatalogDB_getRecordCount(custom_list->catalogDB))
        g_return_if_reached();

    content = CatalogDB_getFieldValueAsString(custom_list->catalogDB, recordNum, column);
    g_value_set_string(value, content);
    free(content);
}

/*****************************************************************************
 *
 *  GtkCatalogModel_iter_next: Takes an iter structure and sets it to point
 *                         to the next row.
 *
 *****************************************************************************/

static gboolean GtkCatalogModel_iter_next(GtkTreeModel *tree_model, GtkTreeIter *iter) {
    gint recordNum;
    GtkCatalogModel *custom_list;

    g_return_val_if_fail (GTKCATALOGMODEL_IS_LIST (tree_model), FALSE);

    if (iter == NULL)
        return FALSE;

    custom_list = GTKCATALOGMODEL(tree_model);

    recordNum = GPOINTER_TO_INT(iter->user_data);
    recordNum++;

    if (recordNum >= CatalogDB_getRecordCount(custom_list->catalogDB))
        return FALSE;

    iter->stamp = custom_list->stamp;
    iter->user_data = GINT_TO_POINTER(recordNum);

    return TRUE;
}

/*****************************************************************************
 *
 *  GtkCatalogModel_iter_children: Returns TRUE or FALSE depending on whether
 *                             the row specified by 'parent' has any children.
 *                             If it has children, then 'iter' is set to
 *                             point to the first child. Special case: if
 *                             'parent' is NULL, then the first top-level
 *                             row should be returned if it exists.
 *
 *****************************************************************************/

static gboolean GtkCatalogModel_iter_children(GtkTreeModel *tree_model, GtkTreeIter *iter,
        GtkTreeIter *parent) {
    GtkCatalogModel *custom_list;

    g_return_val_if_fail (parent == NULL || parent->user_data != NULL, FALSE);

    /* this is a list, nodes have no children */
    if (parent)
        return FALSE;

    /* parent == NULL is a special case; we need to return the first top-level row */

    g_return_val_if_fail (GTKCATALOGMODEL_IS_LIST (tree_model), FALSE);

    custom_list = GTKCATALOGMODEL(tree_model);

    /* No rows => no first row */
    if (CatalogDB_getRecordCount(custom_list->catalogDB) == 0)
        return FALSE;

    /* Set iter to first item in list */
    iter->stamp = custom_list->stamp;
    iter->user_data = (gpointer) 0;

    return TRUE;
}

/*****************************************************************************
 *
 *  GtkCatalogModel_iter_has_child: Returns TRUE or FALSE depending on whether
 *                              the row specified by 'iter' has any children.
 *                              We only have a list and thus no children.
 *
 *****************************************************************************/

static gboolean GtkCatalogModel_iter_has_child(GtkTreeModel * UNUSED( tree_model),
        GtkTreeIter * UNUSED( iter)) {
    return FALSE;
}

/*****************************************************************************
 *
 *  GtkCatalogModel_iter_n_children: Returns the number of children the row
 *                               specified by 'iter' has. This is usually 0,
 *                               as we only have a list and thus do not have
 *                               any children to any rows. A special case is
 *                               when 'iter' is NULL, in which case we need
 *                               to return the number of top-level nodes,
 *                               ie. the number of rows in our list.
 *
 *****************************************************************************/

static gint GtkCatalogModel_iter_n_children(GtkTreeModel *tree_model, GtkTreeIter *iter) {
    GtkCatalogModel *custom_list;

    g_return_val_if_fail (GTKCATALOGMODEL_IS_LIST (tree_model), -1);
    g_return_val_if_fail (iter == NULL, FALSE);

    custom_list = GTKCATALOGMODEL(tree_model);

    /* special case: if iter == NULL, return number of top-level rows */
    if (!iter)
        return CatalogDB_getRecordCount(custom_list->catalogDB);

    return 0; /* otherwise, this is easy again for a list */
}

/*****************************************************************************
 *
 *  GtkCatalogModel_iter_nth_child: If the row specified by 'parent' has any
 *                              children, set 'iter' to the n-th child and
 *                              return TRUE if it exists, otherwise FALSE.
 *                              A special case is when 'parent' is NULL, in
 *                              which case we need to set 'iter' to the n-th
 *                              row if it exists.
 *
 *****************************************************************************/

static gboolean GtkCatalogModel_iter_nth_child(GtkTreeModel *tree_model, GtkTreeIter *iter,
        GtkTreeIter *parent, gint n) {
    GtkCatalogModel *custom_list;

    g_return_val_if_fail (GTKCATALOGMODEL_IS_LIST (tree_model), FALSE);

    custom_list = GTKCATALOGMODEL(tree_model);

    /* a list has only top-level rows */
    if (parent)
        return FALSE;

    /* special case: if parent == NULL, set iter to n-th top-level row */

    if (n >= CatalogDB_getRecordCount(custom_list->catalogDB))
        return FALSE;

    iter->stamp = custom_list->stamp;
    iter->user_data = GINT_TO_POINTER(n);

    return TRUE;
}

/*****************************************************************************
 *
 *  CatalogDB_GetNbRecords(custom_list->catalogDB)_iter_parent: Point 'iter' to the parent node of 'child'. As
 *                           we have a list and thus no children and no
 *                           parents of children, we can just return FALSE.
 *
 *****************************************************************************/

static gboolean GtkCatalogModel_iter_parent(GtkTreeModel * UNUSED( tree_model),
        GtkTreeIter * UNUSED( iter), GtkTreeIter * UNUSED( child)) {
    return FALSE;
}

/*****************************************************************************
 *
 *  GtkCatalogModel_new:  This is what you use in your own code to create a
 *                    new custom list tree model for you to use.
 *
 *****************************************************************************/

GtkCatalogModel * GtkCatalogModel_new(CatalogDB * catalogDB, int shouldCloseDB) {
    GtkCatalogModel *newcustomlist;

    newcustomlist = (GtkCatalogModel*) g_object_new(GTKCATALOGMODEL_TYPE_LIST, NULL);

    g_assert( newcustomlist != NULL );
    newcustomlist->catalogDB = catalogDB;
    newcustomlist->shouldCloseDB = shouldCloseDB;

    return newcustomlist;
}

void GtkCatalogModel_append_record(GtkCatalogModel * custom_list) {
    GtkTreeIter iter;
    GtkTreePath *path;
    CatalogRecord record;

    g_return_if_fail (GTKCATALOGMODEL_IS_LIST(custom_list));

    CatalogRecord_init(&record);
    if (CatalogRecord_edit(&record)) {
        CatalogDB_appendRecord(custom_list->catalogDB, &record);

        /* inform the tree view and other interested objects
         * (e.g. tree row references) that we have inserted
         * a new row, and where it was inserted */

        path = gtk_tree_path_new();
        gtk_tree_path_append_index(path, CatalogDB_getRecordCount(custom_list->catalogDB) - 1);

        GtkCatalogModel_get_iter(GTK_TREE_MODEL(custom_list), &iter, path);

        gtk_tree_model_row_inserted(GTK_TREE_MODEL(custom_list), path, &iter);

        gtk_tree_path_free(path);
    }
    CatalogRecord_finalize(&record);
}

void GtkCatalogModel_remove_record(GtkCatalogModel * custom_list, gint recordIndex) {
    if (CatalogDB_getRecordCount(custom_list->catalogDB) > 0) {
        GtkTreePath *path;

        g_return_if_fail (GTKCATALOGMODEL_IS_LIST(custom_list));
        g_return_if_fail (recordIndex < CatalogDB_getRecordCount(custom_list->catalogDB));

        path = gtk_tree_path_new();
        gtk_tree_path_append_index(path, CatalogDB_getRecordCount(custom_list->catalogDB)-1);

        gtk_tree_model_row_deleted(GTK_TREE_MODEL(custom_list), path);

        gtk_tree_path_free(path);

        CatalogDB_removeRecord(custom_list->catalogDB, recordIndex);
    }
}

void GtkCatalogModel_change_record(GtkCatalogModel * custom_list, gint recordIndex) {
    GtkTreeIter iter;
    GtkTreePath *path;
    CatalogRecord record;

    g_return_if_fail (GTKCATALOGMODEL_IS_LIST(custom_list));
    g_return_if_fail (recordIndex < CatalogDB_getRecordCount(custom_list->catalogDB));

    CatalogRecord_init(&record);
    CatalogDB_readRecord(custom_list->catalogDB, recordIndex, &record);
    if (CatalogRecord_edit(&record)) {
        CatalogDB_writeRecord(custom_list->catalogDB, recordIndex, &record);

        /* inform the tree view and other interested objects
         * (e.g. tree row references) that we have inserted
         * a new row, and where it was inserted */

        path = gtk_tree_path_new();
        gtk_tree_path_append_index(path, recordIndex);

        GtkCatalogModel_get_iter(GTK_TREE_MODEL(custom_list), &iter, path);

        gtk_tree_model_row_changed(GTK_TREE_MODEL(custom_list), path, &iter);

        gtk_tree_path_free(path);
    }
    CatalogRecord_finalize(&record);
}

/** @} */
