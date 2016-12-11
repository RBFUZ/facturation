/**
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
 * $Id$
 */

#ifndef FACTURATION_BASE_DOCUMENT_H
#define FACTURATION_BASE_DOCUMENT_H

#include <Config.h>
#include <CustomerRecord.h>
#include <DocumentRowList.h>

/** @defgroup Documents Documents relates stuff */

/** @defgroup Document Documents
 * @ingroup Documents
 * @{
 */

/** Enumeration defining the type of a document */
typedef enum
{
  QUOTATION /**< It's a quotation */, BILL
/**< It's a bill */
} TypeDocument;

/** Structure representing a document */
typedef struct
{
  CustomerRecord customer /** The customer */;
  char * editDate /** The last edit data */;
  char * expiryDate /** The peremption date */;
  char * docNumber /** The document number */;
  char * object /** The object of the document */;
  char * operator /** The last operator */;
  DocumentRow * rows /** The rows */;
  TypeDocument typeDocument /** The type of document */;
} Document;

/** Initialize a document
 * @param document a pointer to a document
 * @warning An initialized document must be finalized by Document_finalize() to free all resources
 */
OVERRIDABLE_PREFIX void OVERRIDABLE(Document_init)(/*@out@*/ Document * document);

/** Finalize a document
 * @param document the document to finalize
 * @warning document must have been initialized
 */
OVERRIDABLE_PREFIX void OVERRIDABLE(Document_finalize)(Document * document);

/** Save the content of a document to a file
 * @param document the document
 * @param filename the file name
 * @warning document must have been initialized
 */
OVERRIDABLE_PREFIX void OVERRIDABLE(Document_saveToFile)(Document * document, const char * filename);

/** Load the content of a document from a file
 * @param document the document to fill
 * @param filename the file name
 * @warning document must have been initialized
 */
OVERRIDABLE_PREFIX void OVERRIDABLE(Document_loadFromFile)(Document * document, const char * filename);

/** @} */

#include <provided/Document.h>
#include <user/Document.h>

#endif
