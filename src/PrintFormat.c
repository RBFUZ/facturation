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
static char * readMarked(FILE * file, const char * mark);

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
    copyStringWithLength(lineOfFile1, lineOfFile1 + 6, stringLength(lineOfFile1) - 6);
    format->name = lineOfFile1;

    lineOfFile1 = readLine(file);
    free(lineOfFile1);

    format->header = readMarked(file, ".ROW");
    format->row = readMarked(file, ".FOOTER");
    format->footer = readMarked(file, ".END");

    fclose(file);
}

/** Function read one line in a file
 * @param fichier a file
 * @return the new string
 */
static char * readLine(FILE * fichier)
{
    char * line = duplicateString("");
    char buffer[512];
    char * tmp = NULL;

    while (fgets(buffer, sizeof(buffer), fichier) != NULL)
    {
        tmp = line;
        line = concatenateString(line, buffer);
        free(tmp);

        if (buffer[stringLength(buffer) - 1] == '\n')
            break;
    }
    return line;
}

/** Function read a part of model
 * @param file a file
 * @param mark the name to the part of model
 * @return the new string
 */
static char * readMarked(FILE * file, const char * mark)
{
    char * lineOfFile1 = readLine(file);
    char * lineOfFile2 = readLine(file);
    char * lineOfFile3 = NULL;

    while (icaseStartWith(mark, lineOfFile2) != 1)
    {
        lineOfFile3 = concatenateString(lineOfFile1, lineOfFile2);
        free(lineOfFile1);
        free(lineOfFile2);
        lineOfFile2 = readLine(file);
        lineOfFile1 = duplicateString(lineOfFile3);
        free(lineOfFile3);
    }
    free(lineOfFile2);
    copyStringWithLength(lineOfFile1, lineOfFile1, stringLength(lineOfFile1));
    return lineOfFile1;
}
