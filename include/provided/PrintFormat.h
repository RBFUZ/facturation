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

#ifndef FACTURATION_PROVIDED_PRINTFORMAT_H
#define FACTURATION_PROVIDED_PRINTFORMAT_H

/** @defgroup ProvidedPrintFormat Provided functions
 * @ingroup PrintFormat
 * @{
 */

/** Initialize a print format
 * @param format a print format
 * @warning initialized print format should be finalized with PrintFormat_finalize()
 */
void provided_PrintFormat_init(PrintFormat * format);

/** Finalize a print format
 * @param format the print format
 */
void provided_PrintFormat_finalize(PrintFormat * format);

/** Load a print format from a file
 * @param format an initialized print format to fill
 * @param filename the file name
 */
void provided_PrintFormat_loadFromFile(PrintFormat * format, const char * filename);

char * provided_PrintFormat_format(PrintFormat * printFormat, Document * document);

/** @} */

#endif
