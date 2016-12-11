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
 * $Id: DocumentEditor.h 247 2010-09-10 10:23:07Z sebtic $
 */

#ifndef FACTURATION_DOCUMENTEDITOR_H
#define FACTURATION_DOCUMENTEDITOR_H

#include <Config.h>
#include <Document.h>

/** @defgroup DocumentEditor Functions for editing documents
 * @ingroup Documents
 * @{
 */

/** Type of operation to do on the document */
typedef enum _TypeAction {
    NEW_DOCUMENT /** It's a creation */, DISPLAY_DOCUMENT /** It's a viewing */, MODIFY_DOCUMENT
/** It's a modification */
} TypeAction;

/** Edit a document
 * @param parent the parent window
 * @param typeAction the type of the operation to do on the document
 * @param document the document
 */
int DocumentEditor_edit(GtkWindow * parent, TypeAction typeAction, Document * document);

/** @} */

#endif
