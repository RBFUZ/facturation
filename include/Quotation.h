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
 * $Id: Quotation.h 247 2010-09-10 10:23:07Z sebtic $
 */

#ifndef FACTURATION_QUOTATION_H
#define FACTURATION_QUOTATION_H

#include <Config.h>
#include <Document.h>

/** @defgroup Quotation Functions for quotation management
 * @ingroup Documents
 * @{
 */

/** Create a new quotation
 * @param widget the menu item
 * @param parent the parent window
 */
void Quotation_new(GtkWidget * widget, GtkWindow * parent);

/** Open an existing quotation
 * @param widget the menu item
 * @param parent the parent window
 */
void Quotation_open(GtkWidget * widget, GtkWindow * parent);

/** Display an existing quotation
 * @param widget the menu item
 * @param parent the parent window
 */
void Quotation_display(GtkWidget * widget, GtkWindow * parent);

/** Create a bill from a quotation
 * @param widget the menu item
 * @param parent the parent window
 */
void Quotation_convertToBill(GtkWidget * widget, GtkWindow * parent);

/** Save a document as a quotation
 * @param document the document
 */
void Quotation_save(Document * document);

/** Load a document as a quotation
 * @param docNumber the document  number
 * @param document the document to fill
 */
void Quotation_load(const char * docNumber, Document * document);

/** @} */

#endif
