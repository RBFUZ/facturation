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

#ifndef FACTURATION_BASE_CATALOGRECORDEDITOR_H
#define FACTURATION_BASE_CATALOGRECORDEDITOR_H

#include <Config.h>
#include <CatalogRecord.h>

/**
 * @defgroup CatalogRecordEditor Catalog records editing
 * @ingroup Catalog
 * @{
 */

/** A pointer to a function which test if the provided value can be accepted for the field (GTK+ related).
 *
 * value is the value to test. The function must return true if the value is valid, false otherwise
 */
typedef int (*CatalogRecord_Function_isValueValid)(const char * value);

/** A pointer to a function which set the value of the field from a string.
 *
 * record is a pointer to the record and value is the value of the field.
 * @note The string is already valid according to the isValueValid test
 */
typedef void (*CatalogRecord_Function_setValue)(CatalogRecord * record, const char * value);

/** A pointer to a function which get the value of the field as a new string allocated on the heap
 *
 * record is a pointer to the record. The function returns the new string allocated on the heap.
 * @note The string must be valid according to the isValueValid() test
 * @note The string is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
typedef char * (*CatalogRecord_Function_getValue)(CatalogRecord * record);

/** An enumeration defining access to the fields CustomerRecordecord using numerical indices */
typedef enum
{
  CATALOGRECORD_CODE_FIELD /**< The code id */,
  CATALOGRECORD_DESIGNATION_FIELD /**< The designation id */,
  CATALOGRECORD_UNITY_FIELD /**< The unity id */,
  CATALOGRECORD_BASEPRICE_FIELD /**< The base price id */,
  CATALOGRECORD_SELLINGPRICE_FIELD /**< The selling price id */,
  CATALOGRECORD_RATEOFVAT_FIELD /**< The rate of VAT id */,
  CATALOGRECORD_FIELDCOUNT
/**< The number of field in a CatalogRecord */
} CatalogRecord_FieldIdentifier;

/** A structure holding the properties of a field of a CatalogRecord */
typedef struct
{
  /** The size in bytes of the field */
  int size;
  /** The minimal display width in pixel to consider for the field (GTK+ related) */
  int minDisplayWidth;
  /** The name to display for the field (GTK+ related) */
  const char * name;
  /** The alignment for the field (GTK+ related) */
  float alignment;
  /** The maximal length in characters to allow for the field (GTK+ related) */
  int maxlength;
  /* A pointer t a pointer to o a function which test if the provided value can be accepted for the field (GTK+ related).*/
  CatalogRecord_Function_isValueValid * isValueValid;
  /* A pointer to a pointer to  a function which set the value of the field from a string. */
  CatalogRecord_Function_setValue * setValue;
  /* A pointer to a pointer to  a function which get the value of the field as a new string allocated on the heap */
  CatalogRecord_Function_getValue * getValue;

} CatalogRecord_FieldProperties;

/** Get a copy of the properties of a field
 * @param field the identifier of the field
 * @return the properties
 * @see CatalogRecord_FieldIdentifier
 * @relates CatalogRecord
 */
CatalogRecord_FieldProperties CatalogRecord_getFieldProperties(int field);

/** Edit a record
 * @param record a pointer to the record to edit
 * @return a non null value if the changes were validated, 0 otherwise
 * @relates CatalogRecord
 */
int CatalogRecord_edit(CatalogRecord * record);

/** @} */

#endif
