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

#ifndef FACTURATION_USER_CATALOGRECORD_H
#define FACTURATION_USER_CATALOGRECORD_H

/**
 * @defgroup UserCatalogRecord User functions
 * @ingroup CatalogRecord
 * @{
 */

/** Static function which test if a code only contains numbers and letters
 * @param  value the value to test
 * @return true if the code is valid, false otherwise
 */
int user_CatalogRecord_isValueValid_code(const char * value);

/** Static function which test if the value is a positive number
 * @param  value the value to test
 * @return true if the valie is valid, false otherwise
 */
int user_CatalogRecord_isValueValid_positiveNumber(const char * value);

/** Static function to set the code field from a string
 * @param record a pointer to the record
 * @param value the value
 */
void user_CatalogRecord_setValue_code(CatalogRecord * record, const char * value);

/** Static function to set the designation field from a string
 * @param record a pointer to the record
 * @param value the value
 */
void user_CatalogRecord_setValue_designation(CatalogRecord * record, const char * value);

/** Static function to set the unity field from a string
 * @param record a pointer to the record
 * @param value the value
 */
void user_CatalogRecord_setValue_unity(CatalogRecord * record, const char * value);

/** Static function to set the basePrice field from a string
 * @param record a pointer to the record
 * @param value the value
 */
void user_CatalogRecord_setValue_basePrice(CatalogRecord * record, const char * value);

/** Static function to set the sellingPrice field from a string
 * @param record a pointer to the record
 * @param value the value
 */
void user_CatalogRecord_setValue_sellingPrice(CatalogRecord * record, const char * value);

/** Static function to set the rateOfVAT field from a string
 * @param record a pointer to the record
 * @param value the value
 */
void user_CatalogRecord_setValue_rateOfVAT(CatalogRecord * record, const char * value);

/** Static function which create a copy string on the head of the code field value
 * @param record a pointer to the record
 * @return the new string
 * @note The copy is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
char * user_CatalogRecord_getValue_code(CatalogRecord * record);

/** Static function which create a copy string on the head of the designation field value
 * @param record a pointer to the record
 * @return the new string
 * @note The copy is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
char * user_CatalogRecord_getValue_designation(CatalogRecord * record);

/** Static function which create a copy string on the head of the unity field value
 * @param record a pointer to the record
 * @return the new string
 * @note The copy is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
char * user_CatalogRecord_getValue_unity(CatalogRecord * record);

/** Static function which create a copy string on the head of the basePrice field value
 * @param record a pointer to the record
 * @return the new string
 * @note The copy is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
char * user_CatalogRecord_getValue_basePrice(CatalogRecord * record);

/** Static function which create a copy string on the head of the sellingPrice field value
 * @param record a pointer to the record
 * @return the new string
 * @note The copy is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
char * user_CatalogRecord_getValue_sellingPrice(CatalogRecord * record);

/** Static function which create a copy string on the head of the rateOfVAT field value
 * @param record a pointer to the record
 * @return the new string
 * @note The copy is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
char * user_CatalogRecord_getValue_rateOfVAT(CatalogRecord * record);

/** Initialize a record
 * @param[in] record a pointer to a record to initialize
 * @warning every initialized record must be finalized with CatalogRecord_finalize()
 */
void user_CatalogRecord_init(CatalogRecord * record);

/** Finalize a record.
 * @param[in] record a pointer to a record to finalize
 */
void user_CatalogRecord_finalize(CatalogRecord * record);

/** Read a record from a file
 * @param record a pointer to an initialized record on which to store data
 * @param file the file from which the data are read
 */
void user_CatalogRecord_read(CatalogRecord * record, FILE * file);

/** Write a record to a file
 * @param record a pointer to a record
 * @param file the file to which the data are written
 */
void user_CatalogRecord_write(CatalogRecord * record, FILE * file);

/** @} */

#endif
