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

#ifndef FACTURATION_BASE_CATALOGDB_H
#define FACTURATION_BASE_CATALOGDB_H

#include <Config.h>
#include <CatalogRecord.h>

/**
 * @defgroup CatalogDB Catalog database
 * @ingroup Catalog
 * @{
 */

/** The catalog file name */
extern const char * CATALOGDB_FILENAME;

/** The structure which represents an opened catalog database */
typedef struct _CatalogDB
{
  FILE * file; /**< The FILE pointer for the associated file */
  int recordCount; /**< The number of record in the database */
} CatalogDB;

/** Create a database of products
 * @param filename the file name of the database
 * @return a pointer on a CatalogDB representing the opened database, NULL otherwise
 * @relates CatalogDB
 */
OVERRIDABLE_PREFIX CatalogDB * OVERRIDABLE(CatalogDB_create)(const char * filename);

/** Open an existing database of products
 * @param filename the file name of the database
 * @return a pointer on a CatalogDB representing the opened database, NULL otherwise
 * @relates CatalogDB
 */
OVERRIDABLE_PREFIX CatalogDB * OVERRIDABLE(CatalogDB_open)(const char * filename);
/** Open if exists or create otherwise a database of products
 * @param filename the file name of the database
 * @return a pointer on a CatalogDB representing the opened database, NULL otherwise
 * @relates CatalogDB
 */
OVERRIDABLE_PREFIX CatalogDB * OVERRIDABLE(CatalogDB_openOrCreate)(const char * filename);

/** Flush cached data, close a database and free the structure representing the opened database
 * @param catalogDB a pointer on a CatalogDB representing the opened database
 * @relates CatalogDB
 */
OVERRIDABLE_PREFIX void OVERRIDABLE(CatalogDB_close)(CatalogDB * catalogDB);

/** Get the number of records of the database
 * @param catalogDB the database
 * @return the number of records
 * @relates CatalogDB
 */
OVERRIDABLE_PREFIX int OVERRIDABLE(CatalogDB_getRecordCount)(CatalogDB * catalogDB);

/** Create a new string on the heap containing the value of the specified field for the specified record of a database
 * @param catalogDB the database
 * @param recordIndex the record index
 * @param field the field to query
 * @return a new string
 * @note The string is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 * @relates CatalogDB
 */
char * CatalogDB_getFieldValueAsString(CatalogDB * catalogDB, int recordIndex, int field);

/** Append the specified record at the end of the database
 * @param catalogDB the database
 * @param record the record
 * @relates CatalogDB
 */
OVERRIDABLE_PREFIX void OVERRIDABLE(CatalogDB_appendRecord)(CatalogDB * catalogDB, CatalogRecord *record);

/** Insert the specified record at the given position in the database
 * @param catalogDB the database
 * @param recordIndex the insertion position
 * @param record the record
 * @relates CatalogDB
 */
OVERRIDABLE_PREFIX void OVERRIDABLE(CatalogDB_insertRecord)(CatalogDB * catalogDB, int recordIndex, CatalogRecord * record);

/** Remove a record at a given position from the database
 * @param catalogDB the database
 * @param recordIndex the removal position
 * @relates CatalogDB
 */
OVERRIDABLE_PREFIX void OVERRIDABLE(CatalogDB_removeRecord)(CatalogDB * catalogDB, int recordIndex);

/** Read a record from the database
 * @param catalogDB the database
 * @param recordIndex the position of the record to read
 * @param record the record to fill with data
 * @relates CatalogDB
 */
OVERRIDABLE_PREFIX void OVERRIDABLE(CatalogDB_readRecord)(CatalogDB * catalogDB, int recordIndex, CatalogRecord * record);

/** Write a record from the database
 * @param catalogDB the database
 * @param recordIndex the position of the record to write
 * @param record the record containing the data
 * @relates CatalogDB
 */
OVERRIDABLE_PREFIX void OVERRIDABLE(CatalogDB_writeRecord)(CatalogDB * catalogDB, int recordIndex, CatalogRecord * record);

/** @} */

#include <provided/CatalogDB.h>
#include <user/CatalogDB.h>

#endif
