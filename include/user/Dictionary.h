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

#ifndef FACTURATION_USER_DICTIONARY_H
#define FACTURATION_USER_DICTIONARY_H

/** @defgroup UserDictionary User functions
 * @ingroup Dictionary
 * @{
 */

/** Create an empty dictionary on the heap
 * @return a new dictionary
 * @warning the dictionary should be destroyed using Dictionary_destroy()
 */
Dictionary * user_Dictionary_create(void);

/** Destroy a dictionary
 * @param dictionary the dictionary
 */
void user_Dictionary_destroy(Dictionary * dictionary);

/** Get a pointer on the entry associated with the given entry name
 * @param dictionary the dictionary
 * @param name the name of theentry
 * @return a pointer on the entry or NULL if the entry was not found
 */
DictionaryEntry * user_Dictionary_getEntry(Dictionary * dictionary, const char * name);

/** Define or change a dictionary entry as a string
 * @param dictionary the dictionary
 * @param name the entry name
 * @param value the value
 */
void user_Dictionary_setStringEntry(Dictionary * dictionary, const char * name, const char * value);

/** Define or change a dictionary entry as a number
 * @param dictionary the dictionary
 * @param name the entry name
 * @param value the value
 */
void user_Dictionary_setNumberEntry(Dictionary * dictionary, const char * name, double value);

/** Create a new string on the heap which is the result of the formatting of format according to the dictionary content
 * @param dictionary the dictionary
 * @param format the string to format
 * @return a new string created on the heap
 * @warning the user is responsible for freeing the returned string
 */
char * user_Dictionary_format(Dictionary * dictionary, const char * format);

/** @} */

#endif
