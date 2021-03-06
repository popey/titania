/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraße 31a, Leipzig, Germany 2011.
 *
 * All rights reserved. Holger Seelig <holger.seelig@yahoo.de>.
 *
 * THIS IS UNPUBLISHED SOURCE CODE OF create3000.
 *
 * The copyright notice above does not evidence any actual of intended
 * publication of such source code, and is an unpublished work by create3000.
 * This material contains CONFIDENTIAL INFORMATION that is the property of
 * create3000.
 *
 * No permission is granted to copy, distribute, or create derivative works from
 * the contents of this software, in whole or in part, without the prior written
 * permission of create3000.
 *
 * NON-MILITARY USE ONLY
 *
 * All create3000 software are effectively free software with a non-military use
 * restriction. It is free. Well commented source is provided. You may reuse the
 * source in any way you please with the exception anything that uses it must be
 * marked to indicate is contains 'non-military use only' components.
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright 1999, 2016 Holger Seelig <holger.seelig@yahoo.de>.
 *
 * This file is part of the Titania Project.
 *
 * Titania is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License version 3 only, as published by the
 * Free Software Foundation.
 *
 * Titania is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License version 3 for more
 * details (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version 3
 * along with Titania.  If not, see <http://www.gnu.org/licenses/gpl.html> for a
 * copy of the GPLv3 License.
 *
 * For Silvio, Joy and Adi.
 *
 ******************************************************************************/

#ifndef __TITANIA_X3D_PEASE_BLOSSOM_EXPRESSIONS_ASSIGNMENT_OPERATOR_TYPE_H__
#define __TITANIA_X3D_PEASE_BLOSSOM_EXPRESSIONS_ASSIGNMENT_OPERATOR_TYPE_H__

#include <sstream>

namespace titania {
namespace pb {

enum class AssignmentOperatorType :
	uint8_t
{
	ASSIGNMENT,
	MULTIPLICATION_ASSIGNMENT,
	DIVISION_ASSIGNMENT,
	REMAINDER_ASSIGNMENT,
	ADDITION_ASSIGNMENT,
	SUBTRACTION_ASSIGNMENT,
	LEFT_SHIFT_ASSIGNMENT,
	RIGHT_SHIFT_ASSIGNMENT,
	UNSIGNED_RIGHT_SHIFT_ASSIGNMENT,
	BITWISE_AND_ASSIGNMENT,
	BITWISE_XOR_ASSIGNMENT,
	BITWISE_OR_ASSIGNMENT

};

///  @relates AssignmentOperatorType
///  @name Input/Output operators.

///  Insertion operator for AssignmentOperatorType.
template <class CharT, class Traits>
inline
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const AssignmentOperatorType type)
{
	switch (type)
	{
		case AssignmentOperatorType::ASSIGNMENT:                      ostream << '=';    break;
		case AssignmentOperatorType::MULTIPLICATION_ASSIGNMENT:       ostream << "*=";   break;
		case AssignmentOperatorType::DIVISION_ASSIGNMENT:             ostream << "/=";   break;
		case AssignmentOperatorType::REMAINDER_ASSIGNMENT:            ostream << "%=";   break;
		case AssignmentOperatorType::ADDITION_ASSIGNMENT:             ostream << "+=";   break;
		case AssignmentOperatorType::SUBTRACTION_ASSIGNMENT:          ostream << "-=";   break;
		case AssignmentOperatorType::LEFT_SHIFT_ASSIGNMENT:           ostream << "<<=";  break;
		case AssignmentOperatorType::RIGHT_SHIFT_ASSIGNMENT:          ostream << ">>=";  break;
		case AssignmentOperatorType::UNSIGNED_RIGHT_SHIFT_ASSIGNMENT: ostream << ">>>="; break;
		case AssignmentOperatorType::BITWISE_AND_ASSIGNMENT:          ostream << "&=";   break;
		case AssignmentOperatorType::BITWISE_XOR_ASSIGNMENT:          ostream << "^=";   break;
		case AssignmentOperatorType::BITWISE_OR_ASSIGNMENT:           ostream << "|=";   break;
	}

	return ostream;
}

inline
std::string
to_string (const AssignmentOperatorType type)
{
	std::ostringstream osstream;

	osstream << type;

	return osstream .str ();
}

} // pb
} // titania

#endif
