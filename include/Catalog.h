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
 * $Id: Catalog.h 247 2010-09-10 10:23:07Z sebtic $
 */

#ifndef FACTURATION_CATALOG_H
#define FACTURATION_CATALOG_H

#include <Config.h>

/**
 * @defgroup CatalogUI Management of the catalog of products
 * @ingroup Catalog
 * @{
 */

/** Manage the catalog of products
 * @param widget unused parameter
 * @param parent the parent GTK+ window
 */
void Catalog_manage(GtkWidget * widget, GtkWindow * parent);

/** Select a product in the catalog of products
 * @param parent the parent GTK+ window
 * @return -1 if no choice was done (canceling), the record position otherwise
 */
int Catalog_select(GtkWindow * parent);

/** @} */

#endif
