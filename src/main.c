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
 * $Id: main.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <App.h>

/** Last name of the student. */
const char * STUDENT_LASTNAME = "de La Motte-Picquet";
/** First name of the student. */
const char * STUDENT_FIRSTNAME = "Jean-Noel";
/** The current school years */
const char * STUDENT_YEARS = "2013-2014";
/** The group of the students */
const char * STUDENT_GROUP = "X";

/** The main function
 * @param argc the number of arguments of the program
 * @param argv the arguments of the program
 */
int main(int argc, char *argv[])
{
  App_run(&argc, &argv);

  return 0;
}
