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
 * $Id: Document.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <Document.h>
#include <DocumentUtil.h>
#include <DocumentRowList.h>

/** Initialize a document
 * @param document a pointer to a document
 * @warning An initialized document must be finalized by Document_finalize() to free all resources
 */
void IMPLEMENT(Document_init)(Document * document) {
    provided_Document_init(document);
}

/** Finalize a document
 * @param document the document to finalize
 * @warning document must have been initialized
 */
void IMPLEMENT(Document_finalize)(Document * document) {
    provided_Document_finalize(document);
}

/** Save the content of a document to a file
 * @param document the document
 * @param filename the file name
 * @warning document must have been initialized
 */
void IMPLEMENT(Document_saveToFile)(Document * document, const char * filename) {
    provided_Document_saveToFile(document,filename);
}

/** Load the content of a document from a file
 * @param document the document to fill
 * @param filename the file name
 * @warning document must have been initialized
 */
void IMPLEMENT(Document_loadFromFile)(Document * document, const char * filename) {
    provided_Document_loadFromFile(document,filename);
}

