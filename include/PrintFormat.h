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

#ifndef FACTURATION_BASE_PRINTFORMAT_H
#define FACTURATION_BASE_PRINTFORMAT_H

#include <Config.h>
#include <Document.h>

/** @defgroup PrintFormat Print format model management
 * @ingroup Format
 * @{
 */

/** Structure holding the three format strings defining a model */
typedef struct _PrintFormat
{
  /** The name of the model */
  char * name;
  /** The header format */
  char * header;
  /** The row format */
  char * row;
  /** The footer format */
  char * footer;
} PrintFormat;

/** Initialize a print format
 * @param format a print format
 * @warning initialized print format should be finalized with PrintFormat_finalize()
 */
OVERRIDABLE_PREFIX void OVERRIDABLE(PrintFormat_init)(PrintFormat * format);

/** Finalize a print format
 * @param format the print format
 */
OVERRIDABLE_PREFIX void OVERRIDABLE(PrintFormat_finalize)(PrintFormat * format);

/** Load a print format from a file
 * @param format an initialized print format to fill
 * @param filename the file name
 */
OVERRIDABLE_PREFIX void OVERRIDABLE(PrintFormat_loadFromFile)(PrintFormat * format, const char * filename);

/** @} */

#include <provided/PrintFormat.h>
#include <user/PrintFormat.h>

#endif
