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

#ifndef FACTURATION_BASE_CATALOGRECORD_H
#define FACTURATION_BASE_CATALOGRECORD_H

#include <Config.h>

/**
 * @defgroup CatalogRecord Catalog records
 * @ingroup Catalog
 * @{
 */

/** A catalog record
 */
typedef struct
{
  /** The code of the product */
  char * code;
  /** The designation of the product */
  char * designation;
  /** The unity of the product */
  char * unity;
  /** The base price of the product (the product should not be sold at a lower price) */
  double basePrice;
  /** The selling price of the product */
  double sellingPrice;
  /** The rate of the VAT of the product */
  double rateOfVAT;
} CatalogRecord;



/** The size in bytes of the code field of a CatalogRecord */
#define CATALOGRECORD_CODE_SIZE 16UL
/** The size in bytes of the designation field of a CatalogRecord */
#define CATALOGRECORD_DESIGNATION_SIZE 127UL
/** The size in bytes of the unity field of a CatalogRecord */
#define CATALOGRECORD_UNITY_SIZE 20UL
/** The size in bytes of the basePrice field of a CatalogRecord */
#define CATALOGRECORD_BASEPRICE_SIZE ((unsigned long)sizeof(double))
/** The size in bytes of the sellingPrice field of a CatalogRecord */
#define CATALOGRECORD_SELLINGPRICE_SIZE ((unsigned long)sizeof(double))
/** The size in bytes of the rateOfVAT field of a CatalogRecord */
#define CATALOGRECORD_RATEOFVAT_SIZE ((unsigned long)sizeof(double))

/** The size in bytes of all the packed fields of a CatalogRecord */
#define CATALOGRECORD_SIZE (CATALOGRECORD_CODE_SIZE + \
                            CATALOGRECORD_DESIGNATION_SIZE + \
                            CATALOGRECORD_UNITY_SIZE + \
                            CATALOGRECORD_BASEPRICE_SIZE + \
                            CATALOGRECORD_SELLINGPRICE_SIZE + \
                            CATALOGRECORD_RATEOFVAT_SIZE)

/** The maximal length in characters of the string fields of a CatalogRecord */
#define CATALOGRECORD_MAXSTRING_SIZE (MAXVALUE(CATALOGRECORD_CODE_SIZE,MAXVALUE(CATALOGRECORD_DESIGNATION_SIZE,CATALOGRECORD_UNITY_SIZE)))

/** Static function which test if a code only contains numbers and letters
 * @param  value the value to test
 * @return true if the code is valid, false otherwise
 */
OVERRIDABLE_PREFIX int OVERRIDABLE(CatalogRecord_isValueValid_code)(const char * value);

/** Static function which test if the value is a positive number
 * @param  value the value to test
 * @return true if the valie is valid, false otherwise
 */
OVERRIDABLE_PREFIX int OVERRIDABLE(CatalogRecord_isValueValid_positiveNumber)(const char * value);

/** Static function to set the code field from a string
 * @param record a pointer to the record
 * @param value the value
 */
OVERRIDABLE_PREFIX void OVERRIDABLE(CatalogRecord_setValue_code)(CatalogRecord * record, const char * value);

/** Static function to set the designation field from a string
 * @param record a pointer to the record
 * @param value the value
 */
OVERRIDABLE_PREFIX void OVERRIDABLE(CatalogRecord_setValue_designation)(CatalogRecord * record, const char * value);

/** Static function to set the unity field from a string
 * @param record a pointer to the record
 * @param value the value
 */
OVERRIDABLE_PREFIX void OVERRIDABLE(CatalogRecord_setValue_unity)(CatalogRecord * record, const char * value);

/** Static function to set the basePrice field from a string
 * @param record a pointer to the record
 * @param value the value
 */
OVERRIDABLE_PREFIX void OVERRIDABLE(CatalogRecord_setValue_basePrice)(CatalogRecord * record, const char * value);

/** Static function to set the sellingPrice field from a string
 * @param record a pointer to the record
 * @param value the value
 */
OVERRIDABLE_PREFIX void OVERRIDABLE(CatalogRecord_setValue_sellingPrice)(CatalogRecord * record, const char * value);

/** Static function to set the rateOfVAT field from a string
 * @param record a pointer to the record
 * @param value the value
 */
OVERRIDABLE_PREFIX void OVERRIDABLE(CatalogRecord_setValue_rateOfVAT)(CatalogRecord * record, const char * value);

/** Static function which create a copy string on the head of the code field value
 * @param record a pointer to the record
 * @return the new string
 * @note The copy is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
OVERRIDABLE_PREFIX char * OVERRIDABLE(CatalogRecord_getValue_code)(CatalogRecord * record);

/** Static function which create a copy string on the head of the designation field value
 * @param record a pointer to the record
 * @return the new string
 * @note The copy is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
OVERRIDABLE_PREFIX char * OVERRIDABLE(CatalogRecord_getValue_designation)(CatalogRecord * record);

/** Static function which create a copy string on the head of the unity field value
 * @param record a pointer to the record
 * @return the new string
 * @note The copy is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
OVERRIDABLE_PREFIX char * OVERRIDABLE(CatalogRecord_getValue_unity)(CatalogRecord * record);

/** Static function which create a copy string on the head of the basePrice field value
 * @param record a pointer to the record
 * @return the new string
 * @note The copy is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
OVERRIDABLE_PREFIX char * OVERRIDABLE(CatalogRecord_getValue_basePrice)(CatalogRecord * record);

/** Static function which create a copy string on the head of the sellingPrice field value
 * @param record a pointer to the record
 * @return the new string
 * @note The copy is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
OVERRIDABLE_PREFIX char * OVERRIDABLE(CatalogRecord_getValue_sellingPrice)(CatalogRecord * record);

/** Static function which create a copy string on the head of the rateOfVAT field value
 * @param record a pointer to the record
 * @return the new string
 * @note The copy is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
OVERRIDABLE_PREFIX char * OVERRIDABLE(CatalogRecord_getValue_rateOfVAT)(CatalogRecord * record);

/** Initialize a record
 * @param[in] record a pointer to a record to initialize
 * @warning every initialized record must be finalized with CatalogRecord_finalize()
 * @relates CatalogRecord
 */
OVERRIDABLE_PREFIX void OVERRIDABLE(CatalogRecord_init)(/*@in@*/CatalogRecord * record);

/** Finalize a record.
 * @param[in] record a pointer to a record to finalize
 * @relates CatalogRecord
 */
OVERRIDABLE_PREFIX void OVERRIDABLE(CatalogRecord_finalize)(CatalogRecord * record);

/** Read a record from a file
 * @param record a pointer to an initialized record on which to store data
 * @param file the file from which the data are read
 * @relates CatalogRecord
 */
OVERRIDABLE_PREFIX void OVERRIDABLE(CatalogRecord_read)(CatalogRecord * record, FILE * file);

/** Write a record to a file
 * @param record a pointer to a record
 * @param file the file to which the data are written
 * @relates CatalogRecord
 */
OVERRIDABLE_PREFIX void OVERRIDABLE(CatalogRecord_write)(CatalogRecord * record, FILE * file);

/** @} */

#include <provided/CatalogRecord.h>
#include <user/CatalogRecord.h>

#endif
