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
 * $Id: GtkCatalogModel.h 247 2010-09-10 10:23:07Z sebtic $
 */

#ifndef FACTURATION_GTKCATALOGMODEL_H
#define FACTURATION_GTKCATALOGMODEL_H

#include <Config.h>
#include <CatalogDB.h>

/** @defgroup GtkCatalogModel A specialized treeview model backed by our data structure instead of GTK+ ones
 * GTK+ related stuff. It has no interest for the teaching.
 * @ingroup Catalog
 * @{
 */

/* Some boilerplate GObject defines. 'klass' is used
 * instead of 'class', because 'class' is a C++ keyword */

#define GTKCATALOGMODEL_TYPE_LIST               (GtkCatalogModel_get_type ())
#define GTKCATALOGMODEL(obj)                    (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTKCATALOGMODEL_TYPE_LIST, GtkCatalogModel))
#define GTKCATALOGMODEL_CLASS(klass)            (G_TYPE_CHECK_CLASS_CAST ((klass),  GTKCATALOGMODEL_TYPE_LIST, GtkCatalogModelClass))
#define GTKCATALOGMODEL_IS_LIST(obj)            (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTKCATALOGMODEL_TYPE_LIST))
#define GTKCATALOGMODEL_IS_LIST_CLASS(klass)    (G_TYPE_CHECK_CLASS_TYPE ((klass),  GTKCATALOGMODEL_TYPE_LIST))
#define GTKCATALOGMODEL_LIST_GET_CLASS(obj)     (G_TYPE_INSTANCE_GET_CLASS ((obj),  GTKCATALOGMODEL_TYPE_LIST, GtkCatalogModelClass))

typedef struct _GtkCatalogModel GtkCatalogModel;
typedef struct _GtkCatalogModelClass GtkCatalogModelClass;

/* GtkCatalogModel: this structure contains everything we need for our
 *             model implementation. You can add extra fields to
 *             this structure, e.g. hashtables to quickly lookup
 *             rows or whatever else you might need, but it is
 *             crucial that 'parent' is the first member of the
 *             structure.                                          */

struct _GtkCatalogModel {
  GObject parent; /* this MUST be the first member */
  CatalogDB * catalogDB;
  int shouldCloseDB;
  gint stamp; /* Random integer to check whether an iter belongs to our model */
};

/* GtkCatalogModelClass: more boilerplate GObject stuff */
struct _GtkCatalogModelClass {
  GObjectClass parent_class;
};

GType GtkCatalogModel_get_type(void);

GtkCatalogModel *GtkCatalogModel_new(CatalogDB * catalogDB, int shouldCloseDB);

void GtkCatalogModel_remove_record(GtkCatalogModel * custom_list, gint recordIndex);
void GtkCatalogModel_change_record(GtkCatalogModel * custom_list, gint recordIndex);
void GtkCatalogModel_insert_record(GtkCatalogModel * custom_list, gint recordIndex);
void GtkCatalogModel_append_record(GtkCatalogModel * custom_list);

/** @} */

#endif
