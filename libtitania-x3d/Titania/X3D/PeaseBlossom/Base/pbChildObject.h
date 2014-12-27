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

#ifndef __TITANIA_X3D_PEASE_BLOSSOM_BASE_VS_CHILD_OBJECT_H__
#define __TITANIA_X3D_PEASE_BLOSSOM_BASE_VS_CHILD_OBJECT_H__

#include "../Base/pbBase.h"
#include "../Base/pbGarbageCollector.h"

#include <Titania/Utility/Pass.h>
#include <cassert>
#include <cstddef>
#include <set>
#include <vector>

namespace titania {
namespace pb {

class pbChildObject;

using ChildObjectSet = std::set <pbChildObject*>;

class pbChildObject :
	virtual public pbBase,
	public pbGarbageCollector
{
public:

	///  @name Parent handling

	///  Add a parent to this object.
	void
	addParent (pbChildObject* const parent);

	///  Fast replaces @a parentToRemove with @a parentToAdd.
	void
	replaceParent (pbChildObject* const parentToRemove, pbChildObject* const parentToAdd);

	///  Remove a parent from this object.
	void
	removeParent (pbChildObject* const parent);

	///  Add a parent to this object.
	void
	addWeakParent (pbChildObject* const weakParent);

	///  Remove a parent from this object.
	void
	removeWeakParent (pbChildObject* const weakParent);

	///  Get all parents of this object.
	const ChildObjectSet &
	getParents () const
	{ return parents; }

	///  Returns true if this object has rooted objects and collects in @a seen all objects seen.
	virtual
	bool
	hasRootedObjects (ChildObjectSet & circle);

	///  Returns the number of instances that share ownership of this object, or �0� if this object has already been
	///  disposed or is a root object.
	size_t
	getReferenceCount () const
	{ return referenceCount; }

	///  @name Destruction

	///  Reclaims any resources consumed by this object, now or at any time in the future. If this object has already been
	///  disposed, further requests have no effect. Disposing an object does not remove the object itself.
	virtual
	void
	dispose ();

	///  Destructs the pbChildObject.
	virtual
	~pbChildObject ();


protected:

	///  @name Construction

	///  Constructs new Construction.
	pbChildObject ();

	///  @name Children handling

	template <typename ... Args>
	void
	addChildren (Args & ... args)
	{ basic::pass ((addChild (args), 1) ...); }

	void
	addChild (const pbChildObject & child);

	template <typename ... Args>
	void
	removeChildren (Args & ... args)
	{ basic::pass ((removeChild (args), 1) ...); }

	void
	removeChild (const pbChildObject & child);

	bool
	hasRootedObjectsDontCollectObject (ChildObjectSet & seen);


private:

	size_t         referenceCount;
	ChildObjectSet parents;
	pbChildObject* root;
	ChildObjectSet children;

};

} // pb
} // titania

#endif