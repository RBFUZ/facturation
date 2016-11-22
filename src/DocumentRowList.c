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
 * $Id: DocumentRowList.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <DocumentRowList.h>
#include <DocumentUtil.h>

/** Initialize a row
 * @param row the row
 * @warning an initialized row must be finalized by DocumentRow_finalize() to free all resources
 */
void IMPLEMENT(DocumentRow_init)(DocumentRow * row)
{
    row->code = duplicateString("");

    if (row->code == NULL)
        fatalError("malloc error : Allocation of row->code failed.");

    row->designation = duplicateString("");

    if (row->designation == NULL)
        fatalError("malloc error : Allocation of row->designation failed.");

    row->quantity = 0;

    row->unity = duplicateString("");

    if (row->unity == NULL)
        fatalError("malloc error : Allocation of row->unity failed.");

    row->basePrice = 0;
    row->sellingPrice = 0;
    row->discount = 0;
    row->rateOfVAT = 0;

    memset(row->code, '\0', 1);
    memset(row->designation, '\0', 1);
    memset(row->unity, '\0', 1);

    row->next = NULL;
}

/** Finalize a row
 * @param row the row
 * @warning document must have been initialized
 */
void IMPLEMENT(DocumentRow_finalize)(DocumentRow * row)
{
    free(row->code);
    free(row->designation);
    free(row->unity);
    row->next = NULL;
}

/** Create a new row on the heap and initialize it
 * @return the new row
 */
DocumentRow * IMPLEMENT(DocumentRow_create)(void)
{
    DocumentRow * row = malloc(sizeof(DocumentRow));

    if (row == NULL)
        fatalError("malloc error : Allocation of DocumentRow * row failed.");

    DocumentRow_init(row);
    return row;
}

/** Finalize and destroy a row previously created on the heap
 * @param row the row
 */
void IMPLEMENT(DocumentRow_destroy)(DocumentRow * row)
{
    DocumentRow_finalize(row);
    free(row);
}

/** Initialize a list of rows
 * @param list the address of the pointer on the first cell of the list
 */
void IMPLEMENT(DocumentRowList_init)(DocumentRow ** list)
{
    *list = NULL;
}

/** Finalize a list of rows
 * @param list the address of the pointer on the first cell of the list
 * @note Each row of the list are destroyer using DocumentRow_destroy()
 */
void IMPLEMENT(DocumentRowList_finalize)(DocumentRow ** list)
{
    if (list != NULL)
    {
        DocumentRow * rowEnd;

        while (*list != NULL)
        {
            DocumentRow * rowBeforeEnd;

            rowEnd = *list;
            rowBeforeEnd = *list;

            while (rowEnd->next != NULL)
            {
                rowBeforeEnd = rowEnd;
                rowEnd = rowEnd->next;
            }

            if (rowEnd == rowBeforeEnd)
            {
                DocumentRow_destroy(rowEnd);
                *list = NULL;
            }
            else
            {
                rowBeforeEnd->next = NULL;
                DocumentRow_destroy(rowEnd);
            }
        }
    }
}

/** Get a pointer on the rowIndex-th row of the list
 * @param list the pointer on the first cell of the list
 * @param rowIndex the index of the requested row
 * @return a pointer on the rowIndex-th row of the list or NULL if the list contains less rows than the requested one
 */
DocumentRow * IMPLEMENT(DocumentRowList_get)(DocumentRow * list, int rowIndex)
{
    DocumentRow * rowIndexTh = list;
    int countRow = 1;

    if (list != NULL && rowIndex >= 0)
    {
        while (rowIndexTh->next != NULL && countRow-1 != rowIndex)
        {
            rowIndexTh = rowIndexTh->next;
            countRow++;
        }
        if (countRow-1 != rowIndex)
            return NULL;
    }
    else
        return NULL;

    return rowIndexTh;
}

/**
 * Get the number of rows in the list
 * @param list the pointer on the first cell of the list
 */
int IMPLEMENT(DocumentRowList_getRowCount)(DocumentRow * list)
{
    DocumentRow * rowEnd = list;
    int count = 0;

    if (list != NULL)
    {
        count = 1;

        while (rowEnd->next != NULL)
        {
            rowEnd = rowEnd->next;
            count++;
        }
    }
    return count;
}

/** Add a row at the end of the list
 * @param list the address of the pointer on the first cell of the list
 * @param row the row to add
 */
void IMPLEMENT(DocumentRowList_pushBack)(DocumentRow ** list, DocumentRow * row)
{
    DocumentRow * rowCount = *list;

    if (rowCount == NULL)
        *list = row;
    else
    {
        while (rowCount->next != NULL)
        {
            rowCount = rowCount->next;
        }
        rowCount->next = row;
    }
    row->next = NULL;
}

/** Insert a row before a given row
 * @param list the address of the pointer on the first cell of the list
 * @param position a pointer on the positioning row
 * @param row the row to insert
 */
void IMPLEMENT(DocumentRowList_insertBefore)(DocumentRow ** list, DocumentRow * position, DocumentRow * row)
{
    DocumentRow * rowTemp = *list;

    if (*list == position)
        *list = row;
    else
    {
        while (rowTemp-> next != position)
        {
            rowTemp = rowTemp->next;
        }
        rowTemp->next = row;
    }
    row->next = position;

}

/** Insert a row after a given row
 * @param list the address of the pointer on the first cell of the list
 * @param position a pointer on the positioning row
 * @param row the row to insert
 */
void IMPLEMENT(DocumentRowList_insertAfter)(DocumentRow ** list, DocumentRow * position, DocumentRow * row)
{
    DocumentRow * rowTemp = *list;

    if (*list == NULL)
    {
        *list = row;
        row->next = NULL;
    }
    else if (position->next == NULL)
        DocumentRowList_pushBack(list, row);
    else
    {
        rowTemp = position;
        rowTemp = rowTemp->next;
        row->next = rowTemp;
        position->next = row;
    }
}

/** Remove a row from the list
 * @param list the address of the pointer on the first cell of the list
 * @param position the row to remove
 */
void IMPLEMENT(DocumentRowList_removeRow)(DocumentRow ** list, DocumentRow * position)
{
    DocumentRow * rowTemp = *list;

    if (*list == NULL)
        fatalError("Error : List not contains rows.");
    else if (rowTemp->next == NULL && rowTemp == position)
        DocumentRowList_init(list);
    else
    {
        while (rowTemp->next != NULL && rowTemp->next != position)
        {
            rowTemp = rowTemp->next;
        }
        if (rowTemp->next == NULL && rowTemp != position)
            fatalError("Error : List not contains rows position.");
        else
            DocumentRow_destroy(position);
    }
}

/** Write a row in a binary file
 * @param row the row
 * @param file the opened file
 */
void IMPLEMENT(DocumentRow_writeRow)(DocumentRow * row, FILE * file)
{
    char buffer[127UL] = "";

    writeString(row->code, file);
    writeString(row->designation, file);

    memset(buffer, '\0', 127UL);
    if (row->quantity <= (int)row->quantity)
        sprintf (buffer, "%.f", row->quantity);
    else
        sprintf (buffer, "%.2f", row->quantity);
    writeString(buffer, file);

    writeString(row->unity, file);

    memset(buffer, '\0', 127UL);
    if (row->basePrice <= (int)row->basePrice)
        sprintf (buffer, "%.f", row->basePrice);
    else
        sprintf (buffer, "%.2f", row->basePrice);
    writeString(buffer, file);

    memset(buffer, '\0', 127UL);
    if (row->sellingPrice <= (int)row->sellingPrice)
        sprintf (buffer, "%.f", row->sellingPrice);
    else
        sprintf (buffer, "%.2f", row->sellingPrice);
    writeString(buffer, file);

    memset(buffer, '\0', 127UL);
    if (row->discount <= (int)row->discount)
        sprintf (buffer, "%.f", row->discount);
    else
        sprintf (buffer, "%.2f", row->discount);
    writeString(buffer, file);

    memset(buffer, '\0', 127UL);
    if (row->rateOfVAT <= (int)row->rateOfVAT)
        sprintf (buffer, "%.f", row->rateOfVAT);
    else
        sprintf (buffer, "%.2f", row->rateOfVAT);
    writeString(buffer, file);
}

/** Read a row from a file
 * @param file the opened file
 * @return a new row created on the heap filled with the data
 */
DocumentRow * IMPLEMENT(DocumentRow_readRow)(FILE * file)
{
    DocumentRow * row = DocumentRow_create();
    char * buffer = NULL;
    DocumentRow_finalize(row);


    row->code = readString(file);
    row->designation = readString(file);

    buffer = readString(file);
    sscanf (buffer, "%lf", &row->quantity);
    free(buffer);

    row->unity = readString(file);

    buffer = readString(file);
    sscanf (buffer, "%lf", &row->basePrice);
    free(buffer);

    buffer = readString(file);
    sscanf (buffer, "%lf", &row->sellingPrice);
    free(buffer);

    buffer = readString(file);
    sscanf (buffer, "%lf", &row->discount);
    free(buffer);

    buffer = readString(file);
    sscanf (buffer, "%lf", &row->rateOfVAT);
    free(buffer);

    return row;
}
