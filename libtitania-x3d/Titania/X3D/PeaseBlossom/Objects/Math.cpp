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

#include "Math.h"

#include "../Bits/pbMath.h"
#include "../Objects/NativeFunction.h"

namespace titania {
namespace pb {

const std::string Math::typeName = "Math";

Math::Math () :
	pbObject ()
{
	addPropertyDescriptor ("E",       M_E,       NONE);
	addPropertyDescriptor ("LN10",    M_LN10,    NONE);
	addPropertyDescriptor ("LN2",     M_LN2,     NONE);
	addPropertyDescriptor ("LOG2E",   M_LOG2E,   NONE);
	addPropertyDescriptor ("LOG10E",  M_LOG10E,  NONE);
	addPropertyDescriptor ("PI",      M_PI,      NONE);
	addPropertyDescriptor ("SQRT1_2", M_SQRT1_2, NONE);
	addPropertyDescriptor ("SQRT2",   M_SQRT2,   NONE);

	addPropertyDescriptor ("abs",    new NativeFunction ("abs",    abs,    1), NONE);
	addPropertyDescriptor ("acos",   new NativeFunction ("acos",   acos,   1), NONE);
	addPropertyDescriptor ("asin",   new NativeFunction ("asin",   asin,   1), NONE);
	addPropertyDescriptor ("atan",   new NativeFunction ("atan",   atan,   1), NONE);
	addPropertyDescriptor ("atan2",  new NativeFunction ("atan2",  atan2,  2), NONE);
	addPropertyDescriptor ("ceil",   new NativeFunction ("ceil",   ceil,   1), NONE);
	addPropertyDescriptor ("cos",    new NativeFunction ("cos",    cos,    1), NONE);
	addPropertyDescriptor ("exp",    new NativeFunction ("exp",    exp,    1), NONE);
	addPropertyDescriptor ("floor",  new NativeFunction ("floor",  floor,  1), NONE);
	addPropertyDescriptor ("log",    new NativeFunction ("log",    log,    1), NONE);
	addPropertyDescriptor ("max",    new NativeFunction ("max",    max,    2), NONE);
	addPropertyDescriptor ("min",    new NativeFunction ("min",    min,    2), NONE);
	addPropertyDescriptor ("pow",    new NativeFunction ("pow",    pow,    2), NONE);
	addPropertyDescriptor ("random", new NativeFunction ("random", random, 0), NONE);
	addPropertyDescriptor ("round",  new NativeFunction ("round",  round,  1), NONE);
	addPropertyDescriptor ("sin",    new NativeFunction ("sin",    sin,    1), NONE);
	addPropertyDescriptor ("sqrt",   new NativeFunction ("sqrt",   sqrt,   1), NONE);
	addPropertyDescriptor ("tan",    new NativeFunction ("tan",    tan,    1), NONE);
}

ptr <pbObject>
Math::copy (pbExecutionContext* const executionContext) const
noexcept (true)
{ return pbObject::copy (executionContext, new Math ()); }

var
Math::abs (const ptr <pbObject> & object, const std::vector <var> & arguments)
{
	if (arguments .size () not_eq 1)
		throw Error ("wrong number of arguments");

	return std::abs (arguments [0] .toNumber ());
}

var
Math::acos (const ptr <pbObject> & object, const std::vector <var> & arguments)
{
	if (arguments .size () not_eq 1)
		throw Error ("wrong number of arguments");

	return std::acos (arguments [0] .toNumber ());
}

var
Math::asin (const ptr <pbObject> & object, const std::vector <var> & arguments)
{
	if (arguments .size () not_eq 1)
		throw Error ("wrong number of arguments");

	return std::asin (arguments [0] .toNumber ());
}

var
Math::atan (const ptr <pbObject> & object, const std::vector <var> & arguments)
{
	if (arguments .size () not_eq 1)
		throw Error ("wrong number of arguments");

	return std::atan (arguments [0] .toNumber ());
}

var
Math::atan2 (const ptr <pbObject> & object, const std::vector <var> & arguments)
{
	if (arguments .size () not_eq 2)
		throw Error ("wrong number of arguments");

	return std::atan2 (arguments [0] .toNumber (), arguments [1] .toNumber ());
}

var
Math::ceil (const ptr <pbObject> & object, const std::vector <var> & arguments)
{
	if (arguments .size () not_eq 1)
		throw Error ("wrong number of arguments");

	return std::ceil (arguments [0] .toNumber ());
}

var
Math::cos (const ptr <pbObject> & object, const std::vector <var> & arguments)
{
	if (arguments .size () not_eq 1)
		throw Error ("wrong number of arguments");

	return std::cos (arguments [0] .toNumber ());
}

var
Math::exp (const ptr <pbObject> & object, const std::vector <var> & arguments)
{
	if (arguments .size () not_eq 1)
		throw Error ("wrong number of arguments");

	return std::exp (arguments [0] .toNumber ());
}

var
Math::floor (const ptr <pbObject> & object, const std::vector <var> & arguments)
{
	if (arguments .size () not_eq 1)
		throw Error ("wrong number of arguments");

	return std::floor (arguments [0] .toNumber ());
}

var
Math::log (const ptr <pbObject> & object, const std::vector <var> & arguments)
{
	if (arguments .size () not_eq 1)
		throw Error ("wrong number of arguments");

	return std::log (arguments [0] .toNumber ());
}

var
Math::max (const ptr <pbObject> & object, const std::vector <var> & arguments)
{
	double result = NEGATIVE_INFINITY ();

	for (const auto & value : arguments)
	{
		const auto number = value .toNumber ();

		if (std::isnan (number))
			return NaN ();
		
		if (number > result)
			result = number;
	}

	return result;
}

var
Math::min (const ptr <pbObject> & object, const std::vector <var> & arguments)
{
	double result = POSITIVE_INFINITY ();

	for (const auto & value : arguments)
	{
		const auto number = value .toNumber ();

		if (std::isnan (number))
			return NaN ();

		if (number < result)
			result = number;
	}

	return result;
}

var
Math::pow (const ptr <pbObject> & object, const std::vector <var> & arguments)
{
	if (arguments .size () not_eq 2)
		throw Error ("wrong number of arguments");

	return std::pow (arguments [0] .toNumber (), arguments [1] .toNumber ());
}

var
Math::random (const ptr <pbObject> & object, const std::vector <var> & arguments)
{
	static std::uniform_real_distribution <double> uniform_real_distribution (0, 1);
	static std::default_random_engine random_engine (std::chrono::system_clock::now () .time_since_epoch () .count ());

	if (arguments .size () not_eq 0)
		throw Error ("wrong number of arguments");

	return uniform_real_distribution (random_engine);
}

var
Math::round (const ptr <pbObject> & object, const std::vector <var> & arguments)
{
	if (arguments .size () not_eq 1)
		throw Error ("wrong number of arguments");

	return std::round (arguments [0] .toNumber ());
}

var
Math::sin (const ptr <pbObject> & object, const std::vector <var> & arguments)
{
	if (arguments .size () not_eq 1)
		throw Error ("wrong number of arguments");

	return std::sin (arguments [0] .toNumber ());
}

var
Math::sqrt (const ptr <pbObject> & object, const std::vector <var> & arguments)
{
	if (arguments .size () not_eq 1)
		throw Error ("wrong number of arguments");

	return std::sqrt (arguments [0] .toNumber ());
}

var
Math::tan (const ptr <pbObject> & object, const std::vector <var> & arguments)
{
	if (arguments .size () not_eq 1)
		throw Error ("wrong number of arguments");

	return std::tan (arguments [0] .toNumber ());
}

} // pb
} // titania