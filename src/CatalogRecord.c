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
 * $Id: CatalogRecord.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <CatalogRecord.h>

void testError(size_t nbrOpSuccess, CatalogRecord * record, FILE * file);

/** Static function which test if a code only contains numbers and letters
 * @param  value the value to test
 * @return true if the code is valid, false otherwise
 */
int IMPLEMENT(CatalogRecord_isValueValid_code)(const char * value)
{
    int i = 0;

    while (value[i] != '\0')
    {
        if ((value[i] >= 'a' && value[i] <= 'z') || (value[i] >= 'A' && value[i] <= 'Z') || (value[i] >= '0' && value[i] <= '9'))
            i++;
        else
            return 0;
    }
    return 1;
}

/** Static function which test if the value is a positive number
 * @param  value the value to test
 * @return true if the valie is valid, false otherwise
 */
int IMPLEMENT(CatalogRecord_isValueValid_positiveNumber)(const char * value)
{
    char * valueChar;
    double valueInt = strtod (value, &valueChar);

    if (valueChar[0] != '\0' || valueInt < 0)
        return 0;
    else
        return 1;
}

/** Static function to set the code field from a string
 * @param record a pointer to the record
 * @param value the value
 */
void IMPLEMENT(CatalogRecord_setValue_code)(CatalogRecord * record, const char * value)
{
    free(record->code);

    if (CatalogRecord_isValueValid_code(value) == 1)
        record->code = duplicateString(value);
    else
        fatalError("Error : value is not a valid code.");
}

/** Static function to set the designation field from a string
 * @param record a pointer to the record
 * @param value the value
 */
void IMPLEMENT(CatalogRecord_setValue_designation)(CatalogRecord * record, const char * value)
{
    free(record->designation);
    record->designation = duplicateString(value);
}

/** Static function to set the unity field from a string
 * @param record a pointer to the record
 * @param value the value
 */
void IMPLEMENT(CatalogRecord_setValue_unity)(CatalogRecord * record, const char * value)
{
    free(record->unity);
    record->unity = duplicateString(value);
}

/** Static function to set the basePrice field from a string
 * @param record a pointer to the record
 * @param value the value
 */
void IMPLEMENT(CatalogRecord_setValue_basePrice)(CatalogRecord * record, const char * value)
{
    if (CatalogRecord_isValueValid_positiveNumber(value) == 1)
    {
        double valueFloat = 0;
        sscanf (value, "%lf", &valueFloat);
        record->basePrice = valueFloat;
    }
    else
        fatalError("Error : Price is not positive number");
}

/** Static function to set the sellingPrice field from a string
 * @param record a pointer to the record
 * @param value the value
 */
void IMPLEMENT(CatalogRecord_setValue_sellingPrice)(CatalogRecord * record, const char * value)
{
    if (CatalogRecord_isValueValid_positiveNumber(value) == 1)
    {
        double valueFloat = 0;
        sscanf (value, "%lf", &valueFloat);
        record->sellingPrice = valueFloat;
    }
    else
        fatalError("Error : SellingPrice is not positive number");
}

/** Static function to set the rateOfVAT field from a string
 * @param record a pointer to the record
 * @param value the value
 */
void IMPLEMENT(CatalogRecord_setValue_rateOfVAT)(CatalogRecord * record, const char * value)
{
    if (CatalogRecord_isValueValid_positiveNumber(value) == 1)
    {
        double valueFloat = 0;
        sscanf (value, "%lf", &valueFloat);
        record->rateOfVAT = valueFloat;
    }
    else
        fatalError("Error : RateOfVAT is not positive number");
}

/** Static function which create a copy string on the head of the code field value
 * @param record a pointer to the record
 * @return the new string
 * @note The copy is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
char * IMPLEMENT(CatalogRecord_getValue_code)(CatalogRecord * record)
{
    return duplicateString(record->code);
}

/** Static function which create a copy string on the head of the designation field value
 * @param record a pointer to the record
 * @return the new string
 * @note The copy is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
char * IMPLEMENT(CatalogRecord_getValue_designation)(CatalogRecord * record)
{
    return duplicateString(record->designation);
}

/** Static function which create a copy string on the head of the unity field value
 * @param record a pointer to the record
 * @return the new string
 * @note The copy is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
char * IMPLEMENT(CatalogRecord_getValue_unity)(CatalogRecord * record)
{
    return duplicateString(record->unity);
}

/** Static function which create a copy string on the head of the basePrice field value
 * @param record a pointer to the record
 * @return the new string
 * @note The copy is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
char * IMPLEMENT(CatalogRecord_getValue_basePrice)(CatalogRecord * record)
{
    char copyBasePrice[CATALOGRECORD_CODE_SIZE] = "";

    if (record->basePrice <= (int)record->basePrice)
        sprintf (copyBasePrice, "%.f", record->basePrice);
    else
        sprintf (copyBasePrice, "%.2f", record->basePrice);

    return duplicateString(copyBasePrice);
}

/** Static function which create a copy string on the head of the sellingPrice field value
 * @param record a pointer to the record
 * @return the new string
 * @note The copy is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
char * IMPLEMENT(CatalogRecord_getValue_sellingPrice)(CatalogRecord * record)
{
    char copySellingPrice[CATALOGRECORD_SELLINGPRICE_SIZE] = "";

    if (record->sellingPrice <= (int)record->sellingPrice)
        sprintf (copySellingPrice, "%.f", record->sellingPrice);
    else
        sprintf (copySellingPrice, "%.2f", record->sellingPrice);

    return duplicateString(copySellingPrice);
}

/** Static function which create a copy string on the head of the rateOfVAT field value
 * @param record a pointer to the record
 * @return the new string
 * @note The copy is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
char * IMPLEMENT(CatalogRecord_getValue_rateOfVAT)(CatalogRecord * record)
{
    char copyRateOfVAT[CATALOGRECORD_RATEOFVAT_SIZE] = "";

    if (record->rateOfVAT <= (int)record->rateOfVAT)
        sprintf (copyRateOfVAT, "%.f", record->rateOfVAT);
    else
        sprintf (copyRateOfVAT, "%.2f", record->rateOfVAT);

    return duplicateString(copyRateOfVAT);
}

/** Initialize a record
 * @param[in] record a pointer to a record to initialize
 * @warning every initialized record must be finalized with CatalogRecord_finalize()
 */
void IMPLEMENT(CatalogRecord_init)(CatalogRecord * record)
{
    record->code = duplicateString("");
    record->designation = duplicateString("");
    record->unity = duplicateString("");
    record->basePrice = 0;
    record->sellingPrice = 0;
    record->rateOfVAT = 0;
}

/** Finalize a record.
 * @param[in] record a pointer to a record to finalize
 */
void IMPLEMENT(CatalogRecord_finalize)(CatalogRecord * record)
{
    free(record->code);
    free(record->designation);
    free(record->unity);
}

/** Read a record from a file
 * @param record a pointer to an initialized record on which to store data
 * @param file the file from which the data are read
 */
void IMPLEMENT(CatalogRecord_read)(CatalogRecord * record, FILE * file)
{
    char buffer[CATALOGRECORD_SIZE];

    memset(buffer, '\0', sizeof(char) * CATALOGRECORD_CODE_SIZE);

    if (fread(buffer, sizeof(char) * CATALOGRECORD_CODE_SIZE, 1, file) < 1)
        fatalError("fread error : return value is not valid.");

    CatalogRecord_setValue_code(record, buffer);


    memset(buffer, '\0', sizeof(char) * CATALOGRECORD_DESIGNATION_SIZE);

    if (fread(buffer, sizeof(char) * CATALOGRECORD_DESIGNATION_SIZE, 1, file) < 1)
        fatalError("fread error : return value is not valid.");

    CatalogRecord_setValue_designation(record, buffer);


    memset(buffer, '\0', sizeof(char) * CATALOGRECORD_UNITY_SIZE);

    if (fread(buffer, sizeof(char) * CATALOGRECORD_UNITY_SIZE, 1, file) < 1)
        fatalError("fread error : return value is not valid.");

    CatalogRecord_setValue_unity(record, buffer);


    if(fread(&record->basePrice, CATALOGRECORD_BASEPRICE_SIZE, 1, file) < 1)
        fatalError("fread error : return value is not valid.");


    if(fread(&record->sellingPrice, CATALOGRECORD_SELLINGPRICE_SIZE, 1, file) < 1)
        fatalError("fread error : return value is not valid.");


    if(fread(&record->rateOfVAT, CATALOGRECORD_RATEOFVAT_SIZE, 1, file) < 1)
        fatalError("fread error : return value is not valid.");
}

/** Write a record to a file
 * @param record a pointer to a record
 * @param file the file to which the data are written
 */
void IMPLEMENT(CatalogRecord_write)(CatalogRecord * record, FILE * file)
{
    char buffer[CATALOGRECORD_SIZE];

    memset(buffer, '\0', sizeof(char) * CATALOGRECORD_CODE_SIZE);
    copyStringWithLength(buffer, record->code, CATALOGRECORD_CODE_SIZE);

    if(fwrite(buffer, sizeof(char) * CATALOGRECORD_CODE_SIZE, 1, file) < 1)
        fatalError("fwrite error : return value is not valid.");


    memset(buffer, '\0', sizeof(char) * CATALOGRECORD_DESIGNATION_SIZE);
    copyStringWithLength(buffer, record->designation, CATALOGRECORD_DESIGNATION_SIZE);

    if(fwrite(buffer, sizeof(char) * CATALOGRECORD_DESIGNATION_SIZE, 1, file) < 1)
        fatalError("fwrite error : return value is not valid.");


    memset(buffer, '\0', sizeof(char) * CATALOGRECORD_UNITY_SIZE);
    copyStringWithLength(buffer, record->unity, CATALOGRECORD_UNITY_SIZE);

    if(fwrite(buffer, sizeof(char) * CATALOGRECORD_UNITY_SIZE, 1, file) < 1)
        fatalError("fwrite error : return value is not valid.");


    if(fwrite(&record->basePrice, CATALOGRECORD_BASEPRICE_SIZE, 1, file) < 1)
        fatalError("fwrite error : return value is not valid.");

    if(fwrite(&record->sellingPrice, CATALOGRECORD_SELLINGPRICE_SIZE, 1, file) < 1)
        fatalError("fwrite error : return value is not valid.");

    if(fwrite(&record->rateOfVAT, CATALOGRECORD_RATEOFVAT_SIZE, 1, file) < 1)
        fatalError("fwrite error : return value is not valid.");
}
