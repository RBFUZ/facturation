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
 * $Id: GtkCustomerModel.h 247 2010-09-10 10:23:07Z sebtic $
 */

#ifndef FACTURATION_GTKCUSTOMERMODEL_H
#define FACTURATION_GTKCUSTOMERMODEL_H

#include <Config.h>
#include <CustomerDB.h>

/** @defgroup GtkCustomerModel A specialized treeview model backed by our data structure instead of GTK+ ones
 * GTK+ related stuff. It has no interest for the teaching.
 * @ingroup Customer
 * @{
 */

/* Some boilerplate GObject defines. 'klass' is used
 *   instead of 'class', because 'class' is a C++ keyword */

#define GTKCUSTOMERMODEL_TYPE_LIST            (GtkCustomerModel_get_type ())
#define GTKCUSTOMERMODEL(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTKCUSTOMERMODEL_TYPE_LIST, GtkCustomerModel))
#define GTKCUSTOMERMODEL_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  GTKCUSTOMERMODEL_TYPE_LIST, GtkCustomerModelClass))
#define GTKCUSTOMERMODEL_IS_LIST(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTKCUSTOMERMODEL_TYPE_LIST))
#define GTKCUSTOMERMODEL_IS_LIST_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  GTKCUSTOMERMODEL_TYPE_LIST))
#define GTKCUSTOMERMODEL_LIST_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  GTKCUSTOMERMODEL_TYPE_LIST, GtkCustomerModelClass))

typedef struct _GtkCustomerModel GtkCustomerModel;
typedef struct _GtkCustomerModelClass GtkCustomerModelClass;

/* GtkCustomerModel: this structure contains everything we need for our
 *             model implementation. You can add extra fields to
 *             this structure, e.g. hashtables to quickly lookup
 *             rows or whatever else you might need, but it is
 *             crucial that 'parent' is the first member of the
 *             structure.                                          */

struct _GtkCustomerModel {
        GObject parent; /* this MUST be the first member */
        CustomerDB * clientDB;
        int shouldCloseDB;
        gint stamp; /* Random integer to check whether an iter belongs to our model */
};

/* GtkCustomerModelClass: more boilerplate GObject stuff */
struct _GtkCustomerModelClass {
        GObjectClass parent_class;
};

GType GtkCustomerModel_get_type(void);

GtkCustomerModel *GtkCustomerModel_new(CustomerDB * clientDB, int shouldCloseDB);

void GtkCustomerModel_remove_record(GtkCustomerModel * custom_list, gint recordIndex);
void GtkCustomerModel_change_record(GtkCustomerModel * custom_list, gint recordIndex);
void GtkCustomerModel_insert_record(GtkCustomerModel * custom_list, gint recordIndex);
void GtkCustomerModel_append_record(GtkCustomerModel * custom_list);

/** @} */

#endif
