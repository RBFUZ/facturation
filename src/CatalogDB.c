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
 * $Id: CatalogDB.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <CatalogDB.h>
#include <CatalogRecord.h>
#include <CatalogRecordEditor.h>

/** The catalog file name */
const char * CATALOGDB_FILENAME = BASEPATH "/data/Catalog.db";

/** Create a database of products
 * @param filename the file name of the database
 * @return a pointer on a CatalogDB representing the opened database, NULL otherwise
 */
CatalogDB * IMPLEMENT(CatalogDB_create)(const char * filename)
{
    CatalogDB * catalogDB = malloc(sizeof(CatalogDB));

    if (catalogDB == NULL)
        fatalError("malloc error : Allocation of CatalogDB * catalogDB failed.");

    FILE * file = fopen(filename, "wb+");

    if (file == NULL)
    {
        free(catalogDB);
        return NULL;
    }

    catalogDB->recordCount = 0;

    if (fwrite(&catalogDB->recordCount, sizeof(int), 1, file) < 1)
    {
        fclose(file);
        free(catalogDB);
        fatalError("fwrite error : return value is < 1.");
    }
    catalogDB->file = file;

    return catalogDB;
}

/** Open an existing database of products
 * @param filename the file name of the database
 * @return a pointer on a CatalogDB representing the opened database, NULL otherwise
 */
CatalogDB * IMPLEMENT(CatalogDB_open)(const char * filename)
{
    CatalogDB * catalogDB = malloc(sizeof(CatalogDB));

    if (catalogDB == NULL)
        fatalError("malloc error : Allocation of CatalogDB * catalogDB failed");

    FILE * file = fopen(filename, "rb+");

    if (file == NULL)
    {
        free(catalogDB);
        return NULL;
    }

    if (fread(&catalogDB->recordCount, sizeof(int), 1, file) < 1)
    {
        fclose(file);
        free(catalogDB);
        fatalError("fread error : return value is < 1");
    }

    catalogDB->file = file;

    return catalogDB;
}

/** Open if exists or create otherwise a database of products
 * @param filename the file name of the database
 * @return a pointer on a CatalogDB representing the opened database, NULL otherwise
 */
CatalogDB * IMPLEMENT(CatalogDB_openOrCreate)(const char * filename)
{
    FILE * file = fopen(filename, "rb+");
    if (file == NULL)
        return CatalogDB_create(filename);

        return CatalogDB_open(filename);
}

/** Flush cached data, close a database and free the structure representing the opened database
 * @param catalogDB a pointer on a CatalogDB representing the opened database
 */
void IMPLEMENT(CatalogDB_close)(CatalogDB * catalogDB)
{
    rewind(catalogDB->file);
    if (fwrite(&catalogDB->recordCount, sizeof(int), 1, catalogDB->file) < 1)
    {
        fclose(catalogDB->file);
        free(catalogDB);
        fatalError("fwrite error : return value is < 1");
    }
    fclose(catalogDB->file);
    free(catalogDB);
}

/** Get the number of records of the database
 * @param catalogDB the database
 * @return the number of records
 */
int IMPLEMENT(CatalogDB_getRecordCount)(CatalogDB * catalogDB) {
    return catalogDB->recordCount;
}

/** Create a new string on the heap containing the value of the specified field for the specified record of a database
 * @param catalogDB the database
 * @param recordIndex the record index
 * @param field the field to query
 * @return a new string
 * @note The string is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
char * CatalogDB_getFieldValueAsString(CatalogDB * catalogDB, int recordIndex, int field) {
    char * content = NULL;
    if (catalogDB != NULL) {
        CatalogRecord_FieldProperties properties = CatalogRecord_getFieldProperties(field);
        CatalogRecord record;
        CatalogRecord_init(&record);
        CatalogDB_readRecord(catalogDB, recordIndex, &record);
        content = (*properties.getValue)(&record);
        CatalogRecord_finalize(&record);
    }
    return content;
}

/** Append the specified record at the end of the database
 * @param catalogDB the database
 * @param record the record
 */
void IMPLEMENT(CatalogDB_appendRecord)(CatalogDB * catalogDB, CatalogRecord *record)
{
    rewind(catalogDB->file);
    fseek(catalogDB->file, (int)sizeof(int) + (int)CATALOGRECORD_SIZE * catalogDB->recordCount, SEEK_CUR);
    catalogDB->recordCount += 1;
    CatalogRecord_write(record, catalogDB->file);
    rewind(catalogDB->file);
}

/** Insert the specified record at the given position in the database
 * @param catalogDB the database
 * @param recordIndex the insertion position
 * @param record the record
 */
void IMPLEMENT(CatalogDB_insertRecord)(CatalogDB * catalogDB, int recordIndex, CatalogRecord * record)
{
    int recordCount = CatalogDB_getRecordCount(catalogDB);
    CatalogRecord * recordCopie = malloc(sizeof(CatalogRecord));

    if (recordCopie == NULL)
        fatalError("malloc error : Allocation of CatalogRecord * recordCopie failed");

    CatalogRecord_init(recordCopie);

    while (recordCount != recordIndex)
    {
        CatalogDB_readRecord(catalogDB, recordCount-1, recordCopie);
        CatalogDB_writeRecord(catalogDB, recordCount, recordCopie);
        recordCount--;
    }

    CatalogRecord_finalize(recordCopie);
    free(recordCopie);
    CatalogDB_writeRecord(catalogDB, recordIndex, record);
}

/** Remove a record at a given position from the database
 * @param catalogDB the database
 * @param recordIndex the removal position
 */
void IMPLEMENT(CatalogDB_removeRecord)(CatalogDB * catalogDB, int recordIndex)
{
    int recordCount = CatalogDB_getRecordCount(catalogDB);
    CatalogRecord * recordCopie = malloc(sizeof(CatalogRecord));

    if (recordCopie == NULL)
        fatalError("malloc error : Allocation of CatalogRecord * recordCopie failed");

    CatalogRecord_init(recordCopie);

    if (recordIndex >= recordCount || recordIndex < 0 )
        fatalError("Error : recordIndex is too higher or too smaller than recordCount");

    rewind(catalogDB->file);

    while (recordIndex != recordCount - 1)
    {
        CatalogDB_readRecord(catalogDB, recordIndex+1, recordCopie);
        CatalogDB_writeRecord(catalogDB, recordIndex, recordCopie);
        recordIndex++;
    }
    catalogDB->recordCount -= 1;
    CatalogRecord_finalize(recordCopie);
    free(recordCopie);
}

/** Read a record from the database
 * @param catalogDB the database
 * @param recordIndex the position of the record to read
 * @param record the record to fill with data
 */
void IMPLEMENT(CatalogDB_readRecord)(CatalogDB * catalogDB, int recordIndex, CatalogRecord * record)
{
    fseek(catalogDB->file, (int)sizeof(int) + (int)CATALOGRECORD_SIZE * recordIndex, SEEK_SET);
    CatalogRecord_read(record, catalogDB->file);
}

/** Write a record from the database
 * @param catalogDB the database
 * @param recordIndex the position of the record to write
 * @param record the record containing the data
 */
void IMPLEMENT(CatalogDB_writeRecord)(CatalogDB * catalogDB, int recordIndex, CatalogRecord * record)
{
    if (recordIndex < CatalogDB_getRecordCount(catalogDB))
    {
        rewind(catalogDB->file);
        fseek(catalogDB->file, (int)sizeof(int) + (int)CATALOGRECORD_SIZE * recordIndex, SEEK_CUR);
        CatalogRecord_write(record, catalogDB->file);
    }
    else
        CatalogDB_appendRecord(catalogDB, record);
}

