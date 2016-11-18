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

#ifndef FACTURATION_PROVIDED_OPERATORTABLE_H
#define FACTURATION_PROVIDED_OPERATORTABLE_H

/** @defgroup ProvidedOperatorTable Provided functions
 * @ingroup OperatorTable
 * @{
 */
/**
 * Create an empty table of operators.
 * @return the new table
 */
OperatorTable * provided_OperatorTable_create(void);

/** Free a table of operators.
 * @param table a pointer to the table to free
 */
void provided_OperatorTable_destroy(OperatorTable * table);
/** Load a table of operators from a file.
 * @param filename the file name
 * @return the new table
 */
OperatorTable * provided_OperatorTable_loadFromFile(const char * filename);

/** Save a table of operators to a file.
 * @param table the table of operators
 * @param filename the file name
 */
void provided_OperatorTable_saveToFile(OperatorTable * table, const char * filename);

/** Get the number of records of a table of operators
 * @param table the table of operators
 * @return the number of records
 */
int provided_OperatorTable_getRecordCount(OperatorTable * table);

/** Get the name of a record of a table of operators.
 * @param table the table of operators
 * @param recordIndex the record number
 * @return the name of the operator
 */
const char * provided_OperatorTable_getName(OperatorTable * table, int recordIndex);

/** Get the password of a record of a table of operators.
 * @param table the table of operators
 * @param recordIndex the record index
 * @return the paswword of the operator
 */
const char * provided_OperatorTable_getPassword(OperatorTable * table, int recordIndex);

/** Find the record index associated with an operator name.
 * @param table the table of operators
 * @param name the name of the operator
 * @return the index of the operator or -1 if the operator is not in the table
 */
int provided_OperatorTable_findOperator(OperatorTable * table, const char * name);

/** Define or change the password of an operator
 * @param table the table of operators
 * @param name the name of the operator
 * @param password the password of the operator
 * @return the index of the operator in the table
 */
int provided_OperatorTable_setOperator(OperatorTable * table, const char * name, const char * password);

/** Remove an operator from the table.
 * @param table the table of operators
 * @param recordIndex the index of the record to remove
 */
void provided_OperatorTable_removeRecord(OperatorTable * table, int recordIndex);

/** @} */

#endif
