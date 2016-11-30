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
 * $Id: Dictionary.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <Dictionary.h>

char * findCharInString(char * str, char c);
char * formatString(char * copyValueStringEntry, char * buffer, char * charaEqual);
char * formatNumber(char * copyValueEntryNumber, char * buffer, char * charaEqual);
char * valueAfterEqual(char * buffer, char * charaEqual);
char * parameterNotFound(Dictionary * dictionary, char * buffer, char * copyFormat);
char * setCursorEndOfModele(char * copyFormat);

/** Create an empty dictionary on the heap
 * @return a new dictionary
 * @warning the dictionary should be destroyed using Dictionary_destroy()
 */
Dictionary * IMPLEMENT(Dictionary_create)(void)
{
    Dictionary * dictionary = malloc(sizeof(Dictionary));

    if (dictionary == NULL)
        fatalError("malloc error : Attribution of Dictionary * dictionary on the heap failed");

    dictionary->count = 0;
    dictionary->entries = NULL;

    return dictionary;
}

/** Destroy a dictionary
 * @param dictionary the dictionary
 */
void IMPLEMENT(Dictionary_destroy)(Dictionary * dictionary)
{
    int i = 0;

    while (dictionary->count > i)
    {
        if (dictionary->entries[i].type == STRING_ENTRY)
            free(dictionary->entries[i].value.stringValue);

        free(dictionary->entries[i].name);
        i++;
    }
    free(dictionary->entries);
    free(dictionary);
}

/** Get a pointer on the entry associated with the given entry name
 * @param dictionary the dictionary
 * @param name the name of the entry
 * @return a pointer on the entry or NULL if the entry was not found
 */
DictionaryEntry * IMPLEMENT(Dictionary_getEntry)(Dictionary * dictionary, const char * name)
{
    int i = 0;

    if (dictionary->entries == NULL)
        return NULL;

    while (dictionary->count > i)
    {
        if (icaseCompareString(dictionary->entries[i].name, name) == 0)
            return &(dictionary->entries[i]);
        i++;
    }
    return NULL;
}

/** Define or change a dictionary entry as a string
 * @param dictionary the dictionary
 * @param name the entry name
 * @param value the value
 */
void IMPLEMENT(Dictionary_setStringEntry)(Dictionary * dictionary, const char * name, const char * value)
{
    DictionaryEntry * entryTemp = Dictionary_getEntry(dictionary, name);

    if (entryTemp == NULL)
    {
        dictionary->count += 1;

        if (dictionary->entries == NULL)
        {
            dictionary->entries = malloc(sizeof(DictionaryEntry));

            if (dictionary->entries == NULL)
                fatalError("error malloc : Attribution of dictionary->entries on the heap failed");
        }
        else
        {
            dictionary->entries = realloc(dictionary->entries, sizeof(DictionaryEntry) * (size_t)dictionary->count);

            if (dictionary->entries == NULL)
                fatalError("error realloc : Attribution of dictionary->entries on the heap failed");
        }

        entryTemp = dictionary->entries + (dictionary->count - 1);
        entryTemp->name = duplicateString(name);
        entryTemp->value.stringValue = duplicateString(value);
        entryTemp->type = STRING_ENTRY;
    }
    else
    {
        if (entryTemp->type == STRING_ENTRY)
            free(entryTemp->value.stringValue);

        entryTemp->value.stringValue = duplicateString(value);
        entryTemp->type = STRING_ENTRY;
    }
}

/** Define or change a dictionary entry as a number
 * @param dictionary the dictionary
 * @param name the entry name
 * @param value the value
 */
void IMPLEMENT(Dictionary_setNumberEntry)(Dictionary * dictionary, const char * name, double value)
{
    DictionaryEntry * entryTemp = Dictionary_getEntry(dictionary, name);

    if (entryTemp == NULL)
    {
        dictionary->count += 1;

        if (dictionary->entries == NULL)
        {
            dictionary->entries = malloc(sizeof(DictionaryEntry));

            if (dictionary->entries == NULL)
                fatalError("error malloc : Attribution of dictionary->entries on the heap failed");
        }
        else
        {
            dictionary->entries = realloc(dictionary->entries, sizeof(DictionaryEntry) * (size_t)dictionary->count);

            if (dictionary->entries == NULL)
                fatalError("error realloc : Attribution of dictionary->entries on the heap failed");
        }

        entryTemp = dictionary->entries + (dictionary->count - 1);
        entryTemp->name = duplicateString(name);
        entryTemp->value.numberValue = value;
        entryTemp->type = NUMBER_ENTRY;
    }
    else
    {
        if (entryTemp->type == STRING_ENTRY)
            free(entryTemp->value.stringValue);

        entryTemp->value.numberValue = value;
        entryTemp->type = NUMBER_ENTRY;
    }
}

/** Create a new string on the heap which is the result of the formatting of format according to the dictionary content
 * @param dictionary the dictionary
 * @param format the string to format
 * @return a new string created on the heap
 * @warning the user is responsible for freeing the returned string
 */
char * IMPLEMENT(Dictionary_format)(Dictionary * dictionary, const char * format)
{
    DictionaryEntry * entry;
    char buffer[30];
    char bufferForNumberEntry[30];
    char * copyFormat = duplicateString(format), * charaAccolade = NULL, * charaEqual = NULL, * stringFormatedFinal = duplicateString(""), * stringFormated = duplicateString("");

    copyStringWithLength(copyFormat, format+1, stringLength(format) +1);

    while (copyFormat[1] != '\0')
    {
        memset(buffer, '\0', sizeof(char) * 30);
        memset(bufferForNumberEntry, '\0', sizeof(char) * 30);

        charaAccolade = findCharInString(copyFormat, '{');

        if (charaAccolade == NULL)
        {
            stringFormatedFinal = concatenateString(stringFormated, parameterNotFound(dictionary, buffer, copyFormat));
            copyFormat = setCursorEndOfModele(copyFormat);
        }
        else if(copyFormat[0] == '%')
        {
            stringFormatedFinal = concatenateString(stringFormated, "%");
            copyFormat = setCursorEndOfModele(copyFormat);
        }
        else
        {
            copyStringWithLength(buffer, copyFormat, stringLength(copyFormat) - stringLength(charaAccolade) + 1);
            entry = Dictionary_getEntry(dictionary, buffer);
            memset(buffer, '\0', sizeof(char) * 30);

            if (entry == NULL)
            {
                printf("ERROR : NAME NO SEARCH");
                return NULL;
            }

            charaEqual = findCharInString(copyFormat, '=');
            copyStringWithLength(buffer, charaAccolade+1, stringLength(charaAccolade) - stringLength(charaEqual));

            if (entry->type == STRING_ENTRY)
            {
                copyStringWithLength(stringFormated, stringFormatedFinal, stringLength(stringFormatedFinal));
                free(stringFormatedFinal);
                stringFormatedFinal = concatenateString(stringFormated, formatString(duplicateString(entry->value.stringValue), buffer, charaEqual));
                copyFormat = setCursorEndOfModele(copyFormat);

                if (copyFormat[0] == ',')
                {
                    charaEqual = findCharInString(copyFormat, '=');
                    memset(buffer, '\0', sizeof(char) * 30);
                    copyStringWithLength(buffer, copyFormat+1, stringLength(copyFormat+1) - stringLength(charaEqual)+1);
                    copyStringWithLength(stringFormated, stringFormatedFinal, stringLength(stringFormatedFinal));
                    free(stringFormatedFinal);
                    stringFormatedFinal = formatString(duplicateString(stringFormated), buffer, charaEqual);
                    copyStringWithLength(copyFormat, charaEqual + stringLength(valueAfterEqual(buffer, charaEqual)) + 1, stringLength(charaEqual) - stringLength(valueAfterEqual(buffer, charaEqual)));
                }
            }
            else if (entry->type == NUMBER_ENTRY)
            {
                sprintf (bufferForNumberEntry, "%f", entry->value.numberValue);
                copyStringWithLength(stringFormated, stringFormatedFinal, stringLength(stringFormatedFinal) +1);
                free(stringFormatedFinal);
                stringFormatedFinal = concatenateString(stringFormated, formatNumber(duplicateString(bufferForNumberEntry), buffer, charaEqual));
                copyFormat = setCursorEndOfModele(copyFormat);

                if (copyFormat[0] == ',')
                {
                    charaEqual = findCharInString(copyFormat, '=');
                    memset(buffer, '\0', sizeof(char) * 30);
                    copyStringWithLength(buffer, copyFormat+1, stringLength(copyFormat+1) - stringLength(charaEqual) +1);
                    copyStringWithLength(stringFormated, stringFormatedFinal, stringLength(stringFormatedFinal) +1);
                    free(stringFormatedFinal);
                    stringFormatedFinal = formatNumber(duplicateString(stringFormated), buffer, charaEqual);
                    copyStringWithLength(copyFormat, charaEqual + stringLength(valueAfterEqual(buffer, charaEqual)) + 1, stringLength(charaEqual) - stringLength(valueAfterEqual(buffer, charaEqual)));
                }
            }
            else
            {
                printf("Error : Type value not defined");
                return stringFormated;
            }
        }

        if (copyFormat[1] != '\0')
        {
            copyFormat = copyFormat + 3;
            copyStringWithLength(stringFormated, stringFormatedFinal, stringLength(stringFormatedFinal) + 1);
            free(stringFormatedFinal);
            stringFormatedFinal = insertString(stringFormated, (int)stringLength(stringFormated), " ", 1);
        }

    }
    return stringFormatedFinal;
}



char * formatString(char * copyValueStringEntry, char * buffer, char * charaEqual)
{
    if (icaseCompareString(buffer, "case") == 0)
    {
        if (charaEqual[1] >= 'A' && charaEqual[1] <= 'Z')
            makeUpperCaseString(copyValueStringEntry);
        else if (charaEqual[1] >= 'a' && charaEqual[1] <= 'z')
            makeLowerCaseString(copyValueStringEntry);
    }
    else if (icaseCompareString(buffer, "min") == 0)
    {
        while (atoi(valueAfterEqual(buffer, charaEqual)) > (int)stringLength(copyValueStringEntry))
        {
            int a = (int)stringLength(copyValueStringEntry);
            copyValueStringEntry = insertString(copyValueStringEntry, a, " ", 1);
        }
    }
    else if (icaseCompareString(buffer, "max") == 0)
    {
        copyStringWithLength(copyValueStringEntry, copyValueStringEntry, (size_t)atoi(valueAfterEqual(buffer, charaEqual))+1);
    }
    return copyValueStringEntry;
}


char * formatNumber(char * result, char * buffer, char * charaEqual)
{
    double doubleOfCopy = 0.0;
    int valueAfterEqualInt = 0;
    char * copyOfResult = duplicateString("");

    if (icaseCompareString(buffer, "precision") == 0)
    {
        doubleOfCopy = atof(result);
        buffer = valueAfterEqual(buffer, charaEqual);
        sprintf (result, insertString("%.f", 2, buffer, (int)stringLength(buffer)), doubleOfCopy);
    }
    else if (icaseCompareString(buffer, "min") == 0)
    {
        valueAfterEqualInt = atoi(valueAfterEqual(buffer, charaEqual));

        while (valueAfterEqualInt > (int)stringLength(result))
        {
            copyStringWithLength(copyOfResult, result, stringLength(result) + 1);
            free(result);
            result = insertString(copyOfResult, 0, " ", 1);
        }
    }
    return duplicateString(result);
}



char * valueAfterEqual(char * buffer, char * charaEqual)
{
    int i = 1;
    memset(buffer, '\0', sizeof(char) * 30);

    while (charaEqual[i] >= '0' && charaEqual[i] <= '9')
    {
        buffer[i-1] = charaEqual[i];
        i++;
    }
    return buffer;
}



char * findCharInString(char * str, char c)
{
    int i = 0;
    char * resultat = NULL;

    while (str[i] != '\0' && resultat == NULL)
    {
        if (str[i] == c)
        {
            resultat = &str[i];
        }
        i++;
    }
    return resultat;
}


char * parameterNotFound(Dictionary * dictionary, char * buffer, char * copyFormat)
{
    DictionaryEntry * entry;
    char * result = NULL;

    copyStringWithLength(buffer, copyFormat, stringLength(copyFormat) - stringLength(findCharInString(copyFormat, '%')) + 1);
    entry = Dictionary_getEntry(dictionary, buffer);

    memset(buffer, '\0', sizeof(char) * 30);

    if (entry != NULL)
    {
        if (entry->type == STRING_ENTRY)
            result = duplicateString(entry->value.stringValue);
        else if (entry->type == NUMBER_ENTRY)
        {
            sprintf (buffer, "%.6f", entry->value.numberValue);
            result = duplicateString(buffer);
        }
        else
            result = duplicateString("");
    }
    else
    {
        printf("ERROR : %s NO SEARCH", buffer);
        result = duplicateString("");
    }
    return result;
}


char * setCursorEndOfModele(char * copyFormat)
{
    int i = 0;

    while (copyFormat[i] != ',' && copyFormat[i] != '%')
        i++;

    return &copyFormat[i];
}

