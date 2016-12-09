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

/** Function to create a new CustomerDb on the heap
 * @param filename the filename of database
 * @return the new CustomerDB
 */
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

/** Function to open an exist database of CustomerDb
 * @param filename the filename of database
 * @return the new CustomerDB
 */
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

/** Function to choose between open of create a CustomerDB
 * @param filename the filename of database
 * @return the new CustomerDB
 */
CustomerDB * IMPLEMENT(CustomerDB_openOrCreate)(const char * filename)
{
    FILE * file = fopen(filename, "rb+");
    if (file == NULL)
        return CustomerDB_create(filename);

        return CustomerDB_open(filename);
}

/** Function to close a CustomerDB (freeing all resources)
 * @param filename the filename of database
 * @return the new CustomerDB
 */
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

/** Function to get the record count of CustomerDB
 * @param customerBD a pointer to the CustomerDB
 * @return the number of record count
 */
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

/** Function to set a new record at the end of file
 * @param customerBD a pointer to the CustomerDB
 * @param record a pointer to the CustomerRecord
 */
void IMPLEMENT(CustomerDB_appendRecord)(CustomerDB * customerDB, CustomerRecord *record)
{
    rewind(customerDB->file);
    fseek(customerDB->file, (int)sizeof(int) + (int)CUSTOMERRECORD_SIZE * customerDB->recordCount, SEEK_CUR);
    customerDB->recordCount += 1;
    CustomerRecord_write(record, customerDB->file);
    rewind(customerDB->file);
}

/** Function to insert a new record located by record index position
 * @param customerBD a pointer to the CustomerDB
 * @param recordIndex a integer contain the position of the new record
 * @param record a pointer to the CustomerRecord
 */
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

/** Function to remove a record located by record index position
 * @param customerBD a pointer to the CustomerDB
 * @param recordIndex a integer contain the position of the new record
 */
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

/** Function to read a record located in a file
 * @param customerBD a pointer to the CustomerDB
 * @param recordIndex a integer contain the position of the record to read
 * @param record a pointer to the CustomerRecord
 */
void IMPLEMENT(CustomerDB_readRecord)(CustomerDB * customerDB, int recordIndex, CustomerRecord * record)
{
    rewind(customerDB->file);
    fseek(customerDB->file, (int)sizeof(int) + (int)CUSTOMERRECORD_SIZE * recordIndex, SEEK_CUR);

    CustomerRecord_read(record, customerDB->file);
}

/** Function to write a record located in a file
 * @param customerBD a pointer to the CustomerDB
 * @param recordIndex a integer contain the position of the record to read
 * @param record a pointer to the CustomerRecord
 */
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
