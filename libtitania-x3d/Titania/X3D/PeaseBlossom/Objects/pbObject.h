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

#ifndef __TITANIA_X3D_PEASE_BLOSSOM_OBJECTS_PB_OBJECT_H__
#define __TITANIA_X3D_PEASE_BLOSSOM_OBJECTS_PB_OBJECT_H__

#include "../Bits/pbConstants.h"
#include "../Objects/pbBaseObject.h"
#include "../Primitives/ptr.h"

#include <functional>
#include <map>
#include <memory>

#include <unordered_map>

namespace titania {
namespace pb {

class pbObject;
class pbFunction;

///  @relates pbObject
///  @name Type definitions

///  Type to represent a property descriptor.
struct PropertyDescriptor
{
	var value;
	PropertyFlagsType flags;
	ptr <pbFunction> get;
	ptr <pbFunction> set;

	~PropertyDescriptor ();

};

///  Type to represent a property descriptor pointer.
using PropertyDescriptorPtr = std::shared_ptr <PropertyDescriptor>;

/**
 *  Class to represent a basic object.
 */
class pbObject :
	public pbBaseObject
{
public:

	///  @name Construction

	///  Creates a deep copy of this object.
	virtual
	ptr <pbBaseObject>
	copy (pbExecutionContext*) const = 0;

	///  @name Common members

	///  Returns the type name of this object.
	virtual
	const std::string &
	getTypeName () const override
	{ return typeName; }

	///  @name Member access

	///  Checks wehter this object has a propterty @a name.
	bool
	hasProperty (const std::string & name) const
	noexcept (true)
	{ return hasProperty (getId (name)); }

	///  Adds the named property described by the given descriptor to this object.
	void
	addProperty (const std::string & name,
	             const var & value,
	             const PropertyFlagsType flags = DEFAULT,
	             const ptr <pbFunction> & get = nullptr,
	             const ptr <pbFunction> & set = nullptr)
	throw (std::invalid_argument)
	{ addProperty (getId (name), value, flags, get, set); }

	///  Updates the named property described by the given descriptor to this object.
	void
	updateProperty (const std::string & name,
	                const var & value,
	                const PropertyFlagsType flags = DEFAULT,
	                const ptr <pbFunction> & get = nullptr,
	                const ptr <pbFunction> & set = nullptr)
	throw (std::invalid_argument)
	{ updateProperty (getId (name), value, flags, get, set); }

	///  Removes the property @a name from this object.
	void
	removeProperty (const std::string & name)
	noexcept (true)
	{ removeProperty (getId (name)); }

	///  Removes the property @a name from this object.
	const var &
	getProperty (const std::string & name) const
	throw (std::out_of_range)
	{ return getProperty (getId (name)); }

	///  @name Destruction

	///  Reclaims any resources consumed by this object, now or at any time in the future. If this object has already been
	///  disposed, further requests have no effect. Disposing an object does not remove the object itself.
	virtual
	void
	dispose () override;

	///  Destructs the pbObject.
	virtual
	~pbObject ();


protected:

	///  @name Friends

	friend class VariableDeclaration;
	friend class VariableExpression;
	friend class Function;

	///  @name Member types

	using PropertyDescriptorIndex = std::unordered_map <size_t, PropertyDescriptorPtr>;
	using PropertyDescriptorArray = std::vector <std::pair <size_t, PropertyDescriptorPtr>>;

	///  @name Construction

	///  Constructs new pbObject.
	pbObject ();

	///  Creates a deep copy of this object.
	ptr <pbObject>
	copy (pbExecutionContext* executionContext, const ptr <pbObject> & object) const;

	///  @name Member access

	///  Returns the property descriptor for a named property on this object.
	const PropertyDescriptorPtr &
	getPropertyDescriptor (const std::string & name) const
	throw (std::out_of_range)
	{ return getPropertyDescriptor (getId (name)); }

	///  Checks wehter this object has a propterty @a id.
	bool
	hasProperty (const size_t id) const
	noexcept (true)
	{ return propertyDescriptors .count (id); }

	void
	addProperty (const size_t id,
	             const var & value,
	             const PropertyFlagsType flags = DEFAULT,
	             const ptr <pbFunction> & get = nullptr,
	             const ptr <pbFunction> & set = nullptr)
	throw (std::invalid_argument);

	///  Updates the named property described by the given descriptor to this object.
	void
	updateProperty (const size_t id,
	                const var & value,
	                const PropertyFlagsType flags = DEFAULT,
	                const ptr <pbFunction> & get = nullptr,
	                const ptr <pbFunction> & set = nullptr)
	throw (std::invalid_argument);

	///  Removes the property @a id from this object.
	const var &
	getProperty (const size_t id) const
	throw (std::out_of_range)
	{ return getPropertyDescriptor (id) -> value; }

	///  Removes the property @a id from this object.
	void
	removeProperty (const size_t id)
	noexcept (true);

	///  Returns the property descriptor for a property id on this object.
	const PropertyDescriptorPtr &
	getPropertyDescriptor (const size_t id) const
	throw (std::out_of_range);
	
	const PropertyDescriptorIndex &
	getPropertyDescriptors () const
	noexcept (true)
	{ return propertyDescriptors; }

	virtual
	var
	getDefaultValue (const ValueType preferedType) const
	throw (std::exception) final override;


private:

	///  @name Cache opeartions

	void
	addCachedPropertyDescriptor (const size_t, const PropertyDescriptorPtr &)
	noexcept (true);

	void
	removeCachedPropertyDescriptors (const size_t)
	noexcept (true);

	const PropertyDescriptorPtr &
	getCachedPropertyDescriptor (const size_t) const
	throw (std::out_of_range);

	var
	call (const size_t id, const std::vector <var> & arguments = { }) const
	throw (std::exception);

	///  @name Static members

	static constexpr size_t CACHE_SIZE = 32;

	static const std::string typeName;

	///  @name Members
	
	PropertyDescriptorIndex propertyDescriptors;
	PropertyDescriptorArray cachedPropertyDescriptors;

};

} // pb
} // titania

#endif
