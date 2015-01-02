/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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
 * Copyright 1999, 2012 Holger Seelig <holger.seelig@yahoo.de>.
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

#ifndef __TITANIA_X3D_PEASE_BLOSSOM_BITS_VS_MATH_H__
#define __TITANIA_X3D_PEASE_BLOSSOM_BITS_VS_MATH_H__

#include <cmath>
#include <limits>
#include <sstream>

namespace titania {
namespace pb {

constexpr double M_2_16 = 65536.0;
constexpr double M_2_31 = 2147483648.0;
constexpr double M_2_32 = 4294967296.0;

///  The value of Number.MIN_VALUE is the smallest positive value of the Number type, which is approximately 5 � 10?324.
static
constexpr double
MIN_VALUE ()
{
	return std::numeric_limits <double>::min ();
}

///  The value of Number.MAX_VALUE is the largest positive finite value of the Number type, which is approximately 1.7976931348623157 � 10308.
static
constexpr double
MAX_VALUE ()
{
	return std::numeric_limits <double>::max ();
}

///  The value of Number.NaN is NaN.
static
constexpr double
NaN ()
{
	return std::numeric_limits <double>::quiet_NaN ();
}

///  The value of Number.NEGATIVE_INFINITY is -Infintiy.
static
constexpr double
NEGATIVE_INFINITY ()
{
	return -std::numeric_limits <double>::infinity ();
}

///  The value of Number.POSITIVE_INFINITY is Infinity.
static
constexpr double
POSITIVE_INFINITY ()
{
	return std::numeric_limits <double>::infinity ();
}

inline
bool
isNaN (const double value)
{
	return std::isnan (value);
}

inline
double
parseInt (const std::string & string)
{
	double number = NaN ();

	std::istringstream isstream (string);

	isstream >> number;

	return std::copysign (std::floor (std::abs (number)), number);
}

inline
double
parseFloat (const std::string & string)
{
	double number = NaN ();

	std::istringstream isstream (string);

	isstream >> number;

	return number;
}

} // pb
} // titania

#endif