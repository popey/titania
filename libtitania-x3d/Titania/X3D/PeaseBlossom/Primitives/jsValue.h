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

#ifndef __TITANIA_X3D_PEASE_BLOSSOM_PRIMITIVES_JS_VALUE_H__
#define __TITANIA_X3D_PEASE_BLOSSOM_PRIMITIVES_JS_VALUE_H__

#include "../Base/jsChildObject.h"
#include "../Base/jsOutputStreamObject.h"
#include "../Bits/Exception.h"
#include "../Bits/jsConstants.h"
#include "../Primitives/var.h"

#include <cmath>

namespace titania {
namespace pb {

/**
 *  Class to represent a JavaScript value. This is the base class for all JavaScript values.
 */
class jsValue :
	virtual public jsChildObject,
	virtual public jsOutputStreamObject
{
public:

	///  @name Common members

	///  Returns the type of this value.
	virtual
	ValueType
	getType () const = 0;

	///  Returns the a default of its input argument type.
	virtual
	var
	getDefaultValue () const = 0;

	///  @name Common operations

	///  Returns true if the input argument is a non-Object type otherwise false.
	virtual
	bool
	isPrimitive () const;

	///  Converts its input argument to a non-Object type.
	virtual
	var
	toPrimitive () const = 0;

	///  Converts its argument to a value of type Boolean.
	virtual
	bool
	toBoolean () const = 0;

	///  Converts its argument to an integral unsigned value of 16 bit.
	virtual
	uint16_t
	toUInt16 () const;

	///  Converts its argument to an integral signed value of 32 bit.
	virtual
	int32_t
	toInt32 () const;

	///  Converts its argument to an integral unsigned value of 32 bit.
	virtual
	uint32_t
	toUInt32 () const
	{ return toInt32Bit (); }

	///  Converts its argument to an integral numeric value.
	double
	toInteger () const;

	///  Converts its argument to a value of type Number.
	virtual
	double
	toNumber () const = 0;

	///  Converts its argument to a value of type Object.
	virtual
	var
	toObject () const
	throw (TypeError) = 0;

	///  Converts its input argument to either Primitive or Object type.
	virtual
	var
	toValue () const
	{ return var (const_cast <jsValue*> (this)); }

	///  @name Input/Output

	///  Inserts this object into the output stream @a ostream.
	virtual
	void
	toStream (std::ostream & ostream) const override
	{ toPrimitive () -> toStream (ostream); }


protected:

	///  @name Construction

	///  Constructs new jsValue.
	jsValue () :
		       jsChildObject (),
		jsOutputStreamObject ()
	{ }


private:

	double
	toInt32Bit () const;

};

} // pb
} // titania

#endif
