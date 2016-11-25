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
 * $Id: PrintFormat.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <PrintFormat.h>
#include <Dictionary.h>

static char * readLine(FILE * fichier);
char * readMarked(FILE * file, const char * mark);

/** Initialize a print format
 * @param format a print format
 * @warning initialized print format should be finalized with PrintFormat_finalize()
 */
void IMPLEMENT(PrintFormat_init)(PrintFormat * format)
{
    format->name = duplicateString("");
    format->header = duplicateString("");
    format->row = duplicateString("");
    format->footer = duplicateString("");
}

/** Finalize a print format
 * @param format the print format
 */
void IMPLEMENT(PrintFormat_finalize)(PrintFormat * format)
{
    free(format->name);
    free(format->header);
    free(format->row);
    free(format->footer);
}

/** Load a print format from a file
 * @param format an initialized print format to fill
 * @param filename the file name
 */
void IMPLEMENT(PrintFormat_loadFromFile)(PrintFormat * format, const char * filename)
{
    char * lineOfFile1 = NULL;

    FILE * file = fopen(filename, "r");

    if (file == NULL)
        fatalError("Opening gile with fopen failed");

    PrintFormat_finalize(format);

    lineOfFile1 = readLine(file);
    /* FUITE ICI */
    lineOfFile1 = subString(lineOfFile1+6, lineOfFile1 + stringLength(lineOfFile1) - 1);
    format->name = duplicateString(lineOfFile1);
    free(lineOfFile1);

    lineOfFile1 = readLine(file);
    free(lineOfFile1);

    format->header = readMarked(file, ".ROW\n");
    format->row = readMarked(file, ".FOOTER\n");
    format->footer = readMarked(file, ".END");
}


static char * readLine(FILE * fichier)
{
    char * lineOfFile = (char*)malloc(sizeof(char) * 512);

    if (lineOfFile == NULL)
        fatalError("malloc error : Allocation of lineOfFile failed.");

    memset(lineOfFile, '\0', 512);
    lineOfFile = fgets(lineOfFile, 512, fichier);

    if (lineOfFile == NULL)
        fatalError("fgets error : read failed.");

    lineOfFile = realloc(lineOfFile, sizeof(char) * stringLength(lineOfFile) +1);

    if (lineOfFile == NULL)
        fatalError("realloc error : Allocation of lineOfFile failed.");

    return lineOfFile;
}


char * readMarked(FILE * file, const char * mark)
{
    char * lineOfFile1 = readLine(file);
    char * lineOfFile2 = readLine(file);
    char * stringReturn = NULL;

    if (compareString(lineOfFile2, mark) != 0 )
    {
        do
        {
            lineOfFile1 = concatenateString(lineOfFile1, lineOfFile2);
            free(lineOfFile2);
            lineOfFile2 = readLine(file);
        }while (compareString(lineOfFile2, mark) != 0);
    }
    stringReturn = subString(lineOfFile1, lineOfFile1 + stringLength(lineOfFile1) - 1);
    free(lineOfFile1);
    free(lineOfFile2);
    return stringReturn;
}
