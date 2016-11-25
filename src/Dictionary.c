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
    return provided_Dictionary_format(dictionary, format);
}
