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
 * $Id: CustomerDB.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <CustomerDB.h>
#include <CustomerRecord.h>
#include <CustomerRecordEditor.h>

const char * CUSTOMERDB_FILENAME = BASEPATH "/data/Customer.db";

CustomerDB * IMPLEMENT(CustomerDB_create)(const char * filename)
{
    CustomerDB * customerDB = malloc(sizeof(CustomerDB));

    if (customerDB == NULL)
        fatalError("malloc error : Allocation of CustomerDB * customerDB failed.");

    FILE * file = fopen(filename, "wb+");

    if (file == NULL)
    {
        free(customerDB);
        return NULL;
    }

    customerDB->recordCount = 0;

    if (fwrite(&customerDB->recordCount, sizeof(int), 1, file) < 1)
    {
        fclose(file);
        free(customerDB);
        fatalError("fwrite error : return value is < 1.");
    }
    customerDB->file = file;

    return customerDB;
}

CustomerDB * IMPLEMENT(CustomerDB_open)(const char * filename)
{
    CustomerDB * customerDB = malloc(sizeof(CustomerDB));

    if (customerDB == NULL)
        fatalError("malloc error : Allocation of CustomerDB * customerDB failed");

    FILE * file = fopen(filename, "rb+");

    if (file == NULL)
    {
        free(customerDB);
        return NULL;
    }

    if (fread(&customerDB->recordCount, sizeof(int), 1, file) < 1)
    {
        fclose(file);
        free(customerDB);
        fatalError("fread error : return value is < 1");
    }

    customerDB->file = file;

    return customerDB;
}

CustomerDB * IMPLEMENT(CustomerDB_openOrCreate)(const char * filename)
{
    FILE * file = fopen(filename, "rb+");
    if (file == NULL)
        return CustomerDB_create(filename);

        return CustomerDB_open(filename);
}

void IMPLEMENT(CustomerDB_close)(CustomerDB * customerDB)
{
    rewind(customerDB->file);
    if (fwrite(&customerDB->recordCount, sizeof(int), 1, customerDB->file) < 1)
    {
        fclose(customerDB->file);
        free(customerDB);
        fatalError("fwrite error : return value is < 1");
    }
    fclose(customerDB->file);
    free(customerDB);
}

int IMPLEMENT(CustomerDB_getRecordCount)(CustomerDB * customerDB)
{
    return customerDB->recordCount;
}

char * CustomerDB_getFieldValueAsString(CustomerDB * customerDB, int recordIndex, int field) {
    char * content = NULL;
    if (customerDB != NULL) {
        CustomerRecord_FieldProperties properties = CustomerRecord_getFieldProperties(field);
        CustomerRecord record;
        CustomerRecord_init(&record);
        CustomerDB_readRecord(customerDB, recordIndex, &record);
        content = (*properties.getValue)(&record);
        CustomerRecord_finalize(&record);
    }
    return content;
}

void IMPLEMENT(CustomerDB_appendRecord)(CustomerDB * customerDB, CustomerRecord *record)
{
    customerDB->recordCount += 1;
    fseek(customerDB->file, 0, SEEK_END);
    CustomerRecord_write(record, customerDB->file);
    rewind(customerDB->file);
}

void IMPLEMENT(CustomerDB_insertRecord)(CustomerDB * customerDB, int recordIndex, CustomerRecord * record)
{
    int recordCount = CustomerDB_getRecordCount(customerDB);
    CustomerRecord * recordCopie = malloc(sizeof(CustomerRecord));

    if (recordCopie == NULL)
        fatalError("malloc error : Allocation of CustomerRecord * recordCopie failed");

    CustomerRecord_init(recordCopie);

    while (recordCount != recordIndex)
    {
        CustomerDB_readRecord(customerDB, recordCount-1, recordCopie);
        CustomerDB_writeRecord(customerDB, recordCount, recordCopie);
        recordCount--;
    }

    free(recordCopie);
    CustomerDB_writeRecord(customerDB, recordIndex, record);
}


void IMPLEMENT(CustomerDB_removeRecord)(CustomerDB * customerDB, int recordIndex)
{
    int recordCount = CustomerDB_getRecordCount(customerDB);
    CustomerRecord * recordCopie = malloc(sizeof(CustomerRecord));

    if (recordCopie == NULL)
        fatalError("malloc error : Allocation of CustomerRecord * recordCopie failed");

    CustomerRecord_init(recordCopie);

    if (recordIndex >= recordCount || recordIndex < 0 )
        fatalError("Error : recordIndex is too higher or too smaller than recordCount");

    rewind(customerDB->file);

    while (recordIndex != recordCount - 1)
    {
        CustomerDB_readRecord(customerDB, recordIndex+1, recordCopie);
        CustomerDB_writeRecord(customerDB, recordIndex, recordCopie);
        recordIndex++;
    }
    customerDB->recordCount -= 1;
    free(recordCopie);
}

void IMPLEMENT(CustomerDB_readRecord)(CustomerDB * customerDB, int recordIndex, CustomerRecord * record)
{
    rewind(customerDB->file);
    fseek(customerDB->file, (int)sizeof(int) + (int)CUSTOMERRECORD_SIZE * recordIndex, SEEK_CUR);

    CustomerRecord_read(record, customerDB->file);
}

void IMPLEMENT(CustomerDB_writeRecord)(CustomerDB * customerDB, int recordIndex, CustomerRecord * record)
{
    if (recordIndex < CustomerDB_getRecordCount(customerDB))
    {
        rewind(customerDB->file);
        fseek(customerDB->file, (int)sizeof(int) + (int)CUSTOMERRECORD_SIZE * recordIndex, SEEK_CUR);

        CustomerRecord_write(record, customerDB->file);
    }
    else
        CustomerDB_appendRecord(customerDB, record);
}
