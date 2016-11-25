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
 * $Id: EncryptDecrypt.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <EncryptDecrypt.h>

static char encode(char key1, char decoded);
static char decode(char key, char encoded);

void IMPLEMENT(encrypt)(const char * key, char * str) {
    size_t i = 0, j = 0;

    makeLowerCaseString(str);

    while(str[i] != '\0')
    {
        if(key[j] == '\0')
        {
            j = 0;
        }

        if(str[i] >= 'a' && str[i] <= 'z')
        {
            str[i] = encode(key[j], str[i]);
        }

        else
        {
            j--;
        }

        i++;
        j++;
    }
}
void IMPLEMENT(decrypt)(const char * key, char * str) {
    size_t i = 0, j = 0;

    makeLowerCaseString(str);

    while(str[i] != '\0')
    {
        if(key[j] == '\0')
        {
            j = 0;
        }

        if(str[i] >= 'a' && str[i] <= 'z')
        {
            str[i] = decode(key[j], str[i]);
        }

        else
        {
            j--;
        }

        i++;
        j++;
    }
}

static char encode(char key, char decoded) {
    int resultat = 0, total = 0;
    char encoded;

    decoded = toLowerChar(decoded);
    key = toLowerChar(key);

    if(decoded >= 'a' && decoded <= 'z')
    {
        resultat = decoded + key - 2 * 'a';

        if(resultat > 'z' - 'a')
        {
            resultat -= 'z' - 'a' - 1;
        }

        total = 'a' + resultat;
        encoded = (char)total;
    }

    else
    {
        encoded = decoded;
    }

    return encoded;
}

static char decode(char key, char encoded) {
    char decoded;
    int convertInt;

    encoded = toLowerChar(encoded);
    key = toLowerChar(key);

    if(encoded >= 'a' && encoded <= 'z')
    {
        convertInt = encoded - key;
        decoded = (char)convertInt;

        if(decoded < 0)
        {
            convertInt = decoded - 1;
            convertInt = convertInt + 'z' - 'a';
            decoded = (char)convertInt;
        }

        convertInt = convertInt + 'a';
        decoded = (char)convertInt;
    }

    else
    {
        decoded = (char)encoded;
    }

    return decoded;
}
