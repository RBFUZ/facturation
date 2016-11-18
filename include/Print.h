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
 * $Id: Print.h 247 2010-09-10 10:23:07Z sebtic $
 */

#ifndef FACTURATION_PRINT_H
#define FACTURATION_PRINT_H

#include <Config.h>
#include <Document.h>
#include <PrintFormat.h>

/**
 * @defgroup Print Print preview of a document
 * @ingroup Format
 * @{
 */

/** Display a dialog showing a print preview of the document
 * @param document the document
 */
void Print_preview(Document * document);

/** Format a document according to a print format
 * @param printFormat the print format
 * @param document the document
 * @return a new string created on the heap containing the formatted document
 * @warning The user is responsible for freeing the memory
 */
char * PrintFormat_format(PrintFormat * printFormat, Document * document);

/** @} */

#endif
