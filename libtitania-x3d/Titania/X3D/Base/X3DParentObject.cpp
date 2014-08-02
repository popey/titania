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

#include "X3DParentObject.h"

#include <Titania/X3D/Browser/X3DBrowser.h>

#include <cassert>

namespace titania {
namespace X3D {

X3DParentObject::X3DParentObject (X3DBrowser* const browser) :
	   X3DReferenceObject (),
	              browser (browser),
	extendedEventHandling (true),
	             children (),
	             parentId ({ 0 }),
	               events ()
{
	assert (browser);
}

/***
 *  Initializes this object: after setup all fields can process events.
 */
void
X3DParentObject::setup ()
{
	for (const auto & child : children)
		child -> isTainted (false);
}

/***
 *  Adds this object as parent of @a child. After setup @a child is eventable.
 */
void
X3DParentObject::addChild (X3DChildObject & child)
{
	child .isTainted (true);

	children .emplace (&child);

	child .addParent (this);
}

/***
 *  This object will now not be longer a parent of @a child. If the reference count of @a child goes
 *  to zero @a child will be disposed and garbage collected.
 */
void
X3DParentObject::removeChild (X3DChildObject & child)
{
	children .erase (&child);

	child .removeParent (this);
}

/***
 *  Adds @a object to the router event queue.
 */
void
X3DParentObject::addEvent (X3DChildObject* const object)
{
	if (object -> isTainted ())
		return;

	object -> isTainted (true);

	addEvent (object, std::make_shared <Event> (object));
}

/***
 *  Adds @a object to the router event queue.
 */
void
X3DParentObject::addEvent (X3DChildObject* const object, const EventPtr & event)
{
	//__LOG__ << object << " : " << object -> getName () << " : " << object -> getTypeName () << " : " << getName () << " : " << getTypeName () << " : " << this << std::endl;

	getBrowser () -> addEvent ();

	// Register for processEvent

	events .emplace_back (getBrowser () -> addTaintedChild (object, event));

	// Register for eventsProcessed

	if (not isTainted ())
	{
		if (object -> isInput () or (extendedEventHandling and not object -> isOutput ()))
		{
			isTainted (true);
		}
	}

	// Register always on every first event.

	if (not parentId .time)
		parentId = getBrowser () -> addTaintedParent (this);
}

/***
 *  Marks this node as changed. Call this function if you want to inform the requesters of this object about a change.
 */
void
X3DParentObject::addEvent ()
{
	if (not isTainted ())
	{
		isTainted (true);

		if (not parentId .time)
			parentId = getBrowser () -> addTaintedParent (this);

		getBrowser () -> addEvent ();
	}
}

/***
 *  The eventsProcessed () function represents the service request that is called after some set of events has been received.
 *  Requester of this object are now informed about a change of this object.
 */
void
X3DParentObject::eventsProcessed ()
{
	events .clear ();
	parentId .time = 0;

	if (isTainted ())
	{
		isTainted (false);
		processInterests ();
	}
}

/***
 *  Removes all outstanding events from the event queue of the router.
 */
void
X3DParentObject::removeEvents ()
{
	for (const auto & event : events)
		getBrowser () -> removeTaintedChild (event);

	events .clear ();

	if (parentId .time)
	{
		getBrowser () -> removeTaintedParent (parentId);
		parentId .time = 0;
	}
}

/***
 *  Disposes this object.  Note: it is normaly not needed to call this function.
 */
void
X3DParentObject::dispose ()
{
	X3DReferenceObject::dispose ();

	removeEvents ();

	for (const auto & child : children)
		child -> dispose ();

	children .clear ();
}

/***
 *  Destructs this object.
 */
X3DParentObject::~X3DParentObject ()
{ }

} // X3D
} // titania