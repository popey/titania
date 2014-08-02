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

#ifndef __TITANIA_X3D_BASE_X3DPARENT_OBJECT_H__
#define __TITANIA_X3D_BASE_X3DPARENT_OBJECT_H__

#include "../Base/X3DReferenceObject.h"
#include "../Routing/ChildrenList.h"
#include "../Routing/ParentList.h"

#include <Titania/Utility/Pass.h>

namespace titania {
namespace X3D {

class X3DBrowser;

/**
 *  Class to represents an object that handles the events of its children.
 */
class X3DParentObject :
	public X3DReferenceObject
{
public:

	using X3DReferenceObject::addInterest;
	using X3DReferenceObject::removeInterest;

	/***
	 *  @name Construction
	 */

	///  Initializes this object.
	virtual
	void
	setup ();

	/***
	 *  @name Member access
	 */

	///  Returns a pointer to the browser this node belongs to.
	X3DBrowser*
	getBrowser () const
	{ return browser; }

	/***
	 *  @name Event handling
	 */

	///  Marks this object as tainted, i.e. all interests of this object will be processed later.
	virtual
	void
	addEvent () override;

	///  This function is called by the router when all events are processed.  You normally do not need to call this
	///  function directly.
	void
	eventsProcessed ();

	/***
	 *  @name Interest service
	 */

	///  Adds an interest to this object.  The @a requester is then notified about a change of this object.  This version
	///  of the function effectivly calls addEvent on @a requester.
	void
	addInterest (X3DParentObject* const requester) const
	{ addInterest (requester, (void (X3DParentObject::*)()) & X3DParentObject::addEvent); }

	///  Removes an interest from this object.  The @a requester will not further notified about a change of this object.
	void
	removeInterest (X3DParentObject* const requester) const
	{ removeInterest (requester, (void (X3DParentObject::*)()) & X3DParentObject::addEvent); }

	/***
	 *  @name Destruction
	 */

	///  Disposes this object.  You normally do not need to call this function directly.
	virtual
	void
	dispose () override;

	///  Destructs this object.
	virtual
	~X3DParentObject ();


protected:

	/***
	 *  @name Construction
	 */

	///  Constructs new X3DParentObject.
	X3DParentObject (X3DBrowser* const);

	/***
	 *  @name Children handling
	 */

	///  Add this node as parent to all @a children.  See addChild.
	template <typename ... Args>
	void
	addChildren (Args & ... children)
	{ basic::pass ((addChild (children), 1) ...); }

	///  Adds a private child object to this object.  The child object is then able to paricipate on event routing.
	void
	addChild (X3DChildObject &);

	///  Remove this node as parent from all @a children.  See removeChild.
	template <typename ... Args>
	void
	removeChildren (Args & ... children)
	{ basic::pass ((removeChild (children), 1) ...); }

	///  Removes a private field from this object.  If the reference count of @a object becomes 0 the child will be disposed.
	void
	removeChild (X3DChildObject &);

	/***
	 *  @name Event handling
	 */

	///  If extended event handling is set to true, initializeOnly field behave like inputOutput fields.  Otherwise
	///  initializeOnly fields will not process any events.  The default is true.
	void
	setExtendedEventHandling (const bool value)
	{ extendedEventHandling = value; }

	///  Returns whether extended event handling is enabled.
	bool
	getExtendedEventHandling () const
	{ return extendedEventHandling; }

	///  Handler that is called when a child (field) of this object should be marked tainted.
	virtual
	void
	addEvent (X3DChildObject* const) override;

	///  Handler that is called when a child (field) of this object should be added to the event queue.
	virtual
	void
	addEvent (X3DChildObject* const, const EventPtr &) override;


private:

	/***
	 *  @name Event handling
	 */

	///  Removes all fields from the event queue of the router.
	void
	removeEvents ();

	/***
	 *  @name Members
	 */

	X3DBrowser* const     browser;               // This objects Browser
	bool                  extendedEventHandling; // Handle initializeOnlys like inputOutput
	ChildObjectSet        children;              // Internal used fields
	ParentId              parentId;              // This object within Router
	std::vector <ChildId> events;                // Children within Router

};

} // X3D
} // titania

#endif