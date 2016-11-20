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
 * $Id: Document.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <Document.h>
#include <DocumentUtil.h>
#include <DocumentRowList.h>

void testError(size_t nbrOpSuccess, CustomerRecord * record, FILE * file);

/** Initialize a document
 * @param document a pointer to a document
 * @warning An initialized document must be finalized by Document_finalize() to free all resources
 */
void IMPLEMENT(Document_init)(Document * document)
{
    CustomerRecord_init(&document->customer);

    document->editDate = (char*) malloc(sizeof(char) * 15UL);
    if (document->editDate == NULL)
        fatalError("malloc error : Attribution of document->editDate on the heap failed");

    document->expiryDate = (char*) malloc(sizeof(char) * 15UL);
    if (document->expiryDate == NULL)
        fatalError("malloc error : Attribution of document->expiryDate on the heap failed");

    document->docNumber = (char*) malloc(sizeof(char) * 15UL);
    if (document->docNumber == NULL)
        fatalError("malloc error : Attribution of document->docNumber on the heap failed");

    document->object = (char*) malloc(sizeof(char) * 100UL);
    if (document->object == NULL)
        fatalError("malloc error : Attribution of document->object on the heap failed");

    document->operator = (char*) malloc(sizeof(char) * 30);
    if (document->operator == NULL)
        fatalError("malloc error : Attribution of document->operator on the heap failed");


    memset(document->editDate, '\0', 1);
    memset(document->expiryDate, '\0', 1);
    memset(document->docNumber, '\0', 1);
    memset(document->object, '\0', 1);
    memset(document->operator, '\0', 1);

    DocumentRowList_init(&document->rows);
    document->typeDocument = QUOTATION;
}

/** Finalize a document
 * @param document the document to finalize
 * @warning document must have been initialized
 */
void IMPLEMENT(Document_finalize)(Document * document)
{
    free(document->editDate);
    free(document->expiryDate);
    free(document->docNumber);
    free(document->object);
    free(document->operator);

    DocumentRowList_finalize(&document->rows);
}

/** Save the content of a document to a file
 * @param document the document
 * @param filename the file name
 * @warning document must have been initialized
 */
void IMPLEMENT(Document_saveToFile)(Document * document, const char * filename)
{
    DocumentRow * rowIndexTh = document->rows;
    int rowCount = DocumentRowList_getRowCount(document->rows), i = 0;
    FILE * file = fopen(filename, "wb+");

    if (file == NULL)
        fatalError("Error : File opening failed");

    CustomerRecord_write(&document->customer, file);

    writeString(document->editDate, file);
    writeString(document->expiryDate, file);
    writeString(document->docNumber, file);
    writeString(document->object, file);
    writeString(document->operator, file);


    while (rowCount > 0)
    {
        rowIndexTh = DocumentRowList_get(document->rows, i);
        DocumentRow_writeRow(rowIndexTh, file);
        rowCount--;
        i++;
    }
    fclose(file);
}

/** Load the content of a document from a file
 * @param document the document to fill
 * @param filename the file name
 * @warning document must have been initialized
 */
void IMPLEMENT(Document_loadFromFile)(Document * document, const char * filename)
{
    FILE * file = fopen(filename, "rb");

    if (file == NULL)
        fatalError("Error : File opening failed");

    fseek(file, 0, SEEK_END);
    long endOfFile = ftell(file);
    rewind(file);


    CustomerRecord_read(&document->customer, file);

    Document_finalize(document);

    document->editDate = readString(file);
    document->expiryDate = readString(file);
    document->docNumber = readString(file);
    document->object = readString(file);
    document->operator = readString(file);

    while (endOfFile != ftell(file))
    {
        DocumentRowList_pushBack(&document->rows, DocumentRow_readRow(file));
    }
    fclose(file);
}

