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
 * $Id: Operator.h 247 2010-09-10 10:23:07Z sebtic $
 */

#ifndef FACTURATION_OPERATOR_H
#define FACTURATION_OPERATOR_H

#include <Config.h>

/** @defgroup Operator Management of the operators
 * @ingroup Operators
 * @{
 */

/** Manage operators
 * @param widget the menu item
 * @param parent the parent window
 */
void Operator_manage(GtkWidget * widget, GtkWindow * parent);

/** Identify an operator with a password
 * @param parent the parent GTK+ window
 * @return a new string created on the heap containing the name of the operator if the identification is valid,
 * or NULL if the user canceled the operator
 */
char * Operator_identify(GtkWindow * parent);

/** @} */

#endif
