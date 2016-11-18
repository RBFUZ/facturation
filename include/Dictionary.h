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

#ifndef FACTURATION_BASE_DICTIONARY_H
#define FACTURATION_BASE_DICTIONARY_H

#include <Config.h>

/** @defgroup Format Template based formatting of documents */

/** @defgroup Dictionary Dictionary based formatting functions
 * @ingroup Format
 * @{
 */

/** Enumeration defining the type of the doctionary entries */
typedef enum
{
  UNDEFINED_ENTRY /* It's undefined */, NUMBER_ENTRY /** It's a number */, STRING_ENTRY
/** It's a string */
} DictionaryEntryType;

/** Structure representing an entry in the dictionary */
typedef struct
{
  /** The type of entry */
  DictionaryEntryType type;
  /** The name of the entry */
  char * name;
  /** The union which store the value of the entry */
  union
  {
    /** The value of the entry when it's a string */
    char * stringValue;
    /** The value of the entry when it's a real number */
    double numberValue;
  } value;
} DictionaryEntry;

/** Structure representing a dictionary */
typedef struct _Dictionary
{
  /** The number of entries of the dictionary */
  int count;
  /** The table of entries */
  DictionaryEntry * entries;
} Dictionary;

/** Create an empty dictionary on the heap
 * @return a new dictionary
 * @warning the dictionary should be destroyed using Dictionary_destroy()
 */
OVERRIDABLE_PREFIX Dictionary * OVERRIDABLE(Dictionary_create)(void);

/** Destroy a dictionary
 * @param dictionary the dictionary
 */
OVERRIDABLE_PREFIX void OVERRIDABLE(Dictionary_destroy)(Dictionary * dictionary);
/** Get a pointer on the entry associated with the given entry name
 * @param dictionary the dictionary
 * @param name the name of theentry
 * @return a pointer on the entry or NULL if the entry was not found
 */
OVERRIDABLE_PREFIX DictionaryEntry * OVERRIDABLE(Dictionary_getEntry)(Dictionary * dictionary, const char * name);

/** Define or change a dictionary entry as a string
 * @param dictionary the dictionary
 * @param name the entry name
 * @param value the value
 */
OVERRIDABLE_PREFIX void OVERRIDABLE(Dictionary_setStringEntry)(Dictionary * dictionary, const char * name, const char * value);

/** Define or change a dictionary entry as a number
 * @param dictionary the dictionary
 * @param name the entry name
 * @param value the value
 */
OVERRIDABLE_PREFIX void OVERRIDABLE(Dictionary_setNumberEntry)(Dictionary * dictionary, const char * name, double value);

/** Create a new string on the heap which is the result of the formatting of format according to the dictionary content
 * @param dictionary the dictionary
 * @param format the string to format
 * @return a new string created on the heap
 * @warning the user is responsible for freeing the returned string
 */
OVERRIDABLE_PREFIX char * OVERRIDABLE(Dictionary_format)(Dictionary * dictionary, const char * format);

/** @} */

#include <provided/Dictionary.h>
#include <user/Dictionary.h>

#endif
