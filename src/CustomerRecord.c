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
 * $Id: CustomerRecord.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <CustomerRecord.h>

static void testError(size_t nbrOpSuccess, char * pointRecord, CustomerRecord * record, FILE * file);


/** Static function to set the name field from a string
 * @param record a pointer to the record
 * @param value the value
 */
void IMPLEMENT(CustomerRecord_setValue_name)(CustomerRecord * record, const char * value)
{
    copyStringWithLength(record->name, value, stringLength(value)+1);
}

/** Static function to set the address field from a string
 * @param record a pointer to the record
 * @param value the value
 */
void IMPLEMENT(CustomerRecord_setValue_address)(CustomerRecord * record, const char * value)
{
    copyStringWithLength(record->address, value, stringLength(value)+1);
}

/** Static function to set the postal code field from a string
 * @param record a pointer to the record
 * @param value the value
 */
void IMPLEMENT(CustomerRecord_setValue_postalCode)(CustomerRecord * record, const char * value)
{
    copyStringWithLength(record->postalCode, value, stringLength(value)+1);
}

/** Static function to set the town field from a string
 * @param record a pointer to the record
 * @param value the value
 */
void IMPLEMENT(CustomerRecord_setValue_town)(CustomerRecord * record, const char * value)
{
    copyStringWithLength(record->town, value, stringLength(value)+1);
}

/** Function to get the name field from a record
 * @param record a pointer to the record
 * @return a new string on the heap of name
 */
char * IMPLEMENT(CustomerRecord_getValue_name)(CustomerRecord * record)
{
    return duplicateString(record->name);
}

/** Function to get the address field from a record
 * @param record a pointer to the record
 * @return a new string on the heap of address
 */
char * IMPLEMENT(CustomerRecord_getValue_address)(CustomerRecord * record)
{
    return duplicateString(record->address);
}

/** Function to get the postal code field from a record
 * @param record a pointer to the record
 * @return a new string on the heap of postal code
 */
char * IMPLEMENT(CustomerRecord_getValue_postalCode)(CustomerRecord * record)
{
    return duplicateString(record->postalCode);
}

/** Function to get the town field from a record
 * @param record a pointer to the record
 * @return a new string on the heap of town
 */
char * IMPLEMENT(CustomerRecord_getValue_town)(CustomerRecord * record)
{
    return duplicateString(record->town);
}

/** Static function to init all fields from a record
 * @param record a pointer to the record
 */
void IMPLEMENT(CustomerRecord_init)(CustomerRecord * record)
{
    memset(record->name, 0, CUSTOMERRECORD_NAME_SIZE);
    memset(record->address, 0, CUSTOMERRECORD_ADDRESS_SIZE);
    memset(record->postalCode, 0, CUSTOMERRECORD_POSTALCODE_SIZE);
    memset(record->town, 0, CUSTOMERRECORD_TOWN_SIZE);
}

/** Static function to freeing all fields from a record
 * @param record a pointer to the record
 */
void IMPLEMENT(CustomerRecord_finalize)(CustomerRecord * UNUSED(record)){
}

/** Static function to read all fields from a record to a file
 * @param record a pointer to the record
 * @param file a file
 */
void IMPLEMENT(CustomerRecord_read)(CustomerRecord * record, FILE * file)
{
    testError(fread(record->name, CUSTOMERRECORD_NAME_SIZE, 1, file), record->name, record, file);
    testError(fread(record->address, CUSTOMERRECORD_ADDRESS_SIZE, 1, file), record->address, record,  file);
    testError(fread(record->postalCode, CUSTOMERRECORD_POSTALCODE_SIZE, 1, file), record->postalCode, record, file);
    testError(fread(record->town, CUSTOMERRECORD_TOWN_SIZE, 1, file), record->town, record, file);
}

/** Static function to write all fields from a record to a file
 * @param record a pointer to the record
 * @param file a file
 */
void IMPLEMENT(CustomerRecord_write)(CustomerRecord * record, FILE * file)
{
    testError(fwrite(record->name, CUSTOMERRECORD_NAME_SIZE, 1, file), record->name, record, file);
    testError(fwrite(record->address, CUSTOMERRECORD_ADDRESS_SIZE, 1, file), record->address, record, file);
    testError(fwrite(record->postalCode, CUSTOMERRECORD_POSTALCODE_SIZE, 1, file), record->postalCode, record, file);
    testError(fwrite(record->town, CUSTOMERRECORD_TOWN_SIZE, 1, file), record->town, record, file);
}

/** Static function to test if fwrite and fread works correctly
 * @param nbrOpSuccess the return value of function
 * @param pointRecord a pointer to the record filed
 * @param record a pointer to the record
 * @param file a file
 */
static void testError(size_t nbrOpSuccess, char * pointRecord, CustomerRecord * record, FILE * file)
{
    if(nbrOpSuccess < 1 && stringLength(pointRecord) != 0)
    {
        fclose(file);
        free(record);
        fatalError("fread or fwrite error : return value is < 1");
    }
}
