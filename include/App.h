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
 * $Id: App.h 247 2010-09-10 10:23:07Z sebtic $
 */

#ifndef FACTURATION_APP_H
#define FACTURATION_APP_H

#include <Config.h>

/** @defgroup App Application main functions
 * @{
 */


/** Initialize and run the application
 * @param argc Address of the argc parameter of the main() function. Changed if any arguments were handled.
 * @param argv Address of the argv parameter of main(). Any parameters understood by gtk_init()  are stripped before return.
 * @see main()
 */
void App_run(int * argc, char * *argv[]);

/** @} */

#endif
