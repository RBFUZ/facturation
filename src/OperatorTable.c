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
 * $Id: OperatorTable.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <OperatorTable.h>
#include <EncryptDecrypt.h>

/**
 * Create an empty table of operators.
 * @return the new table
 * @relates OperatorTable
 */
OperatorTable * IMPLEMENT(OperatorTable_create)(void)
{
    OperatorTable * emptyStruct =  malloc(sizeof(OperatorTable));

    if (emptyStruct == NULL)
        fatalError("Error : Attribution of OperatorTable * emptyStruct on the heap failed");

    emptyStruct->recordCount = 0;
    emptyStruct->records = NULL;
    return emptyStruct;
}

/** Free a table of operators.
 * @param table a pointer to the table to free
 * @relates OperatorTable
 */
void IMPLEMENT(OperatorTable_destroy)(OperatorTable * table)
{
    int recordCount = OperatorTable_getRecordCount(table);
    recordCount--;

    while (recordCount >= 0)
    {
        if (table->records[recordCount][0] != NULL)
            free(table->records[recordCount][0]);
        if (table->records[recordCount][1] != NULL)
            free(table->records[recordCount][1]);
        if (table->records[recordCount] != NULL)
            free(table->records[recordCount]);
        recordCount--;
    }
    free(table->records);
    free(table);
}

/** Load a table of operators from a file.
 * @param filename the file name
 * @return the new table
 * @relates OperatorTable
 */
OperatorTable * IMPLEMENT(OperatorTable_loadFromFile)(const char * filename)
{
    OperatorTable * newTable = OperatorTable_create();

    if (newTable == NULL)
        fatalError("malloc error : Allocation of OperatorTable * newTable failed.");

    FILE * file = fopen(filename, "r+");

    if (file == NULL)
        return NULL;

    fseek(file, 0, SEEK_END);
    long endOfFile = ftell(file);
    rewind(file);

    while (fgetc(file) != '\n')
    ;

    while (endOfFile != ftell(file))
    {
        char name[OPERATORTABLE_MAXNAMESIZE] = "", password[OPERATORTABLE_MAXPASSWORDSIZE] = "";

        decrypt(OperatorCryptKey, fgets(name, OPERATORTABLE_MAXNAMESIZE, file));
        name[stringLength(name) - 1] = '\0';

        if (name == '\0')
            break;

        decrypt(OperatorCryptKey, fgets(password, OPERATORTABLE_MAXPASSWORDSIZE, file));
        password[stringLength(password) - 1] = '\0';

        OperatorTable_setOperator(newTable, name, password);
    }
    fclose(file);
    return newTable;
}

/** Save a table of operators to a file.
 * @param table the table of operators
 * @param filename the file name
 * @relates OperatorTable
 */
void IMPLEMENT(OperatorTable_saveToFile)(OperatorTable * table, const char * filename)
{
    FILE * file = fopen(filename, "wt+");

    if (file == NULL)
        fatalError("Error : Opening file failed");

    int recordCount = table->recordCount, i = 0;
    char pointCharactere[2] = "";
    char * recordCountString = malloc(sizeof(char) * (size_t)sprintf(pointCharactere, "%d", table->recordCount) + 2);
    sprintf(recordCountString, "%d", table->recordCount);

    fputs(recordCountString, file);
    fputs("\n", file);

    free(recordCountString);

    while (i < recordCount && i != -1)
    {
        encrypt(OperatorCryptKey, table->records[i][0]);
        encrypt(OperatorCryptKey, table->records[i][1]);

        fputs(table->records[i][0], file);
        fputs("\n", file);
        fputs(table->records[i][1], file);
        fputs("\n", file);
        i++;
    }
    fclose(file);
}

/** Get the number of records of a table of operators
 * @param table the table of operators
 * @return the number of records
 * @relates OperatorTable
 */
int IMPLEMENT(OperatorTable_getRecordCount)(OperatorTable * table)
{
    return table->recordCount;
}

/** Get the name of a record of a table of operators.
 * @param table the table of operators
 * @param recordIndex the record number
 * @return the name of the operator
 * @relates OperatorTable
 */
const char * IMPLEMENT(OperatorTable_getName)(OperatorTable * table, int recordIndex)
{
    char * getNameOperator;

    if (recordIndex > OperatorTable_getRecordCount(table) || recordIndex < 0)
        fatalError("Error : Operator doesn't exist");

    getNameOperator = table->records[recordIndex][0];
    return getNameOperator;
}

/** Get the password of a record of a table of operators.
 * @param table the table of operators
 * @param recordIndex the record index
 * @return the paswword of the operator
 * @relates OperatorTable
 */
const char * IMPLEMENT(OperatorTable_getPassword)(OperatorTable * table, int recordIndex)
{
    char * getPasswordOperator;

    if (recordIndex > OperatorTable_getRecordCount(table) || recordIndex < 0)
        fatalError("Error : Operator doesn't exist");

    getPasswordOperator = table->records[recordIndex][1];
    return getPasswordOperator;
}

/** Get the record index associated with an operator name.
 * @param table the table of operators
 * @param name the name of the operator
 * @return the index of the operator or -1 if the operator is not in the table
 * @relates OperatorTable
 */
int IMPLEMENT(OperatorTable_findOperator)(OperatorTable * table, const char * name)
{
    int i = 0, recordIndex = -1;
    char * nameOperator;

    while (i < OperatorTable_getRecordCount(table) && recordIndex != 0)
    {
        nameOperator = table->records[i][0];
        recordIndex = icaseCompareString(nameOperator, name);
        i++;
    }
    if (recordIndex != 0)
    {
        i = 0;
    }
    return i - 1;
}

/** Define or change the password of an operator
 * @param table the table of operators
 * @param name the name of the operator
 * @param password the password of the operator
 * @return the index of the operator in the table
 * @relates OperatorTable
 */
int IMPLEMENT(OperatorTable_setOperator)(OperatorTable * table, const char * name, const char * password)
{
    int indexOperator = OperatorTable_findOperator(table, name);
    int recordCount = OperatorTable_getRecordCount(table);

    if (indexOperator != -1)
    {
        free(table->records[indexOperator][1]);
        table->records[indexOperator][1] = duplicateString(password);
        return indexOperator;
    }
    else
    {
        if (recordCount == 0)
        {
            table->records = (char***) malloc(sizeof(char **));

            if (table->records == NULL)
                fatalError("Error : Attribution of table->records on the heap failed");
        }
        else
    	{
            table->records = (char ***)realloc(table->records, sizeof(char **) * ((size_t)recordCount + 1));

            if (table->records == NULL)
                fatalError("Error : Realloc of table->records failed");
        }

	table->records[recordCount] = (char**) malloc(sizeof(char *) * 2);

	if (table->records[0] == NULL)
		fatalError("Error : Attribution of table->records on the heap failed");

	table->records[recordCount][0] = duplicateString(name);
	table->records[recordCount][1] = duplicateString(password);

    table->recordCount += 1;
    indexOperator = recordCount;
    }
    return indexOperator;
}

/** Remove an operator from the table.
 * @param table the table of operators
 * @param recordIndex the index of the record to remove
 * @relates OperatorTable
 */
void IMPLEMENT(OperatorTable_removeRecord)(OperatorTable * table, int recordIndex)
{
    int recordCount = OperatorTable_getRecordCount(table);

    if (recordIndex > recordCount || recordIndex < 0)
        fatalError("Error : Operator doesn't exist");

    table->recordCount -= 1;

    free(table->records[recordIndex][0]);
    free(table->records[recordIndex][1]);
    free(table->records[recordIndex]);

    if (recordIndex != recordCount - 1)
    {
        while (recordIndex < recordCount - 1)
        {
            table->records[recordIndex] = table->records[recordIndex+1];
            recordIndex++;
        }
    }
}
