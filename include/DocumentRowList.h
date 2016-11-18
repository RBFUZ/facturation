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

#ifndef FACTURATION_BASE_DOCUMENTROWLIST_H
#define FACTURATION_BASE_DOCUMENTROWLIST_H

#include <Config.h>

/** @defgroup DocumentRowList List of rows of a document
 * @see Document
 * @ingroup Documents
 * @{
 */

/** Structure representing a row in a document (as a cell in a simple linked list */
typedef struct _DocumentRow
{
  char * code /** The code */;
  char * designation /** The designation */;
  double quantity /** The quantity */;
  char * unity /** The unity */;
  double basePrice /** The base price */;
  double sellingPrice /** The selling price */;
  double discount /** The discount */;
  double rateOfVAT /** The rate of VAT */;
  struct _DocumentRow * next /** The pointer to the next row */;
} DocumentRow;

/** Initialize a row
 * @param row the row
 * @warning an initialized row must be finalized by DocumentRow_finalize() to free all resources
 */
OVERRIDABLE_PREFIX void OVERRIDABLE(DocumentRow_init)(DocumentRow * row);

/** Finalize a row
 * @param row the row
 * @warning document must have been initialized
 */
OVERRIDABLE_PREFIX void OVERRIDABLE(DocumentRow_finalize)(DocumentRow * row);

/** Create a new row on the heap and initialize it
 * @return the new row
 */
OVERRIDABLE_PREFIX DocumentRow * OVERRIDABLE(DocumentRow_create)(void);

/** Finalize and destroy a row previously created on the heap
 * @param row the row
 */
OVERRIDABLE_PREFIX void OVERRIDABLE(DocumentRow_destroy)(DocumentRow * row);

/** Initialize a list of rows
 * @param list the address of the pointer on the first cell of the list
 */
OVERRIDABLE_PREFIX void OVERRIDABLE(DocumentRowList_init)(DocumentRow ** list);

/** Finalize a list of rows
 * @param list the address of the pointer on the first cell of the list
 * @note Each row of the list are destroyer using DocumentRow_destroy()
 */
OVERRIDABLE_PREFIX void OVERRIDABLE(DocumentRowList_finalize)(DocumentRow ** list);

/** Get a pointer on the rowIndex-th row of the list
 * @param list the pointer on the first cell of the list
 * @param rowIndex the index of the requested row
 * @return a pointer on the rowIndex-th row of the list or NULL if the list contains less rows than the requested one
 */
OVERRIDABLE_PREFIX DocumentRow * OVERRIDABLE(DocumentRowList_get)(DocumentRow * list, int rowIndex);

/**
 * Get the number of rows in the list
 * @param list the pointer on the first cell of the list
 */
OVERRIDABLE_PREFIX int OVERRIDABLE(DocumentRowList_getRowCount)(DocumentRow * list);

/** Add a row at the end of the list
 * @param list the address of the pointer on the first cell of the list
 * @param row the row to add
 */
OVERRIDABLE_PREFIX void OVERRIDABLE(DocumentRowList_pushBack)(DocumentRow ** list, DocumentRow * row);

/** Insert a row before a given row
 * @param list the address of the pointer on the first cell of the list
 * @param position a pointer on the positioning row
 * @param row the row to insert
 */
OVERRIDABLE_PREFIX void OVERRIDABLE(DocumentRowList_insertBefore)(DocumentRow ** list, DocumentRow * position, DocumentRow * row);

/** Insert a row after a given row
 * @param list the address of the pointer on the first cell of the list
 * @param position a pointer on the positioning row
 * @param row the row to insert
 */
OVERRIDABLE_PREFIX void OVERRIDABLE(DocumentRowList_insertAfter)(DocumentRow ** list, DocumentRow * position, DocumentRow * row);

/** Remove a row from the list
 * @param list the address of the pointer on the first cell of the list
 * @param position the row to remove
 */
OVERRIDABLE_PREFIX void OVERRIDABLE(DocumentRowList_removeRow)(DocumentRow ** list, DocumentRow * position);

/** Write a row in a binary file
 * @param row the row
 * @param file the opened file
 */
OVERRIDABLE_PREFIX void OVERRIDABLE(DocumentRow_writeRow)(DocumentRow * row, FILE * file);

/** Read a row from a file
 * @param file the opened file
 * @return a new row created on the heap filled with the data
 */
OVERRIDABLE_PREFIX DocumentRow * OVERRIDABLE(DocumentRow_readRow)(FILE * file);

/** @} */

#include <provided/DocumentRowList.h>
#include <user/DocumentRowList.h>

#endif
