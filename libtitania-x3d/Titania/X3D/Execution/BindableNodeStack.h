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

#ifndef __TITANIA_X3D_EXECUTION_BINDABLE_NODE_STACK_H__
#define __TITANIA_X3D_EXECUTION_BINDABLE_NODE_STACK_H__

#include "../Basic/X3DBaseNode.h"

#include <Titania/Basic/BindStack.h>
#include <initializer_list>

namespace titania {
namespace X3D {

template <class Type>
class X3DBindableNodeStack :
	virtual public X3DBaseNode
{
public:

	typedef Type*                            pointer_type;
	typedef basic::bind_stack <pointer_type> stack_type;
	typedef typename stack_type::size_type   size_type;

	X3DBindableNodeStack (X3DExecutionContext* const executionContext, const pointer_type & node) :
		X3DBaseNode (executionContext -> getBrowser (), executionContext),
		fields (),
		stack ({ node })
	{
		setComponent ("Browser");
		setTypeName ("X3DBindableNodeList");

		addField (outputOnly, "bindTime", *fields .bindTime);
   }

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const executionContext) const final
	{ return new X3DBindableNodeStack (executionContext, bottom ()); }

	/// @name Fields

	const SFTime &
	bindTime () const
	{ return *fields .bindTime; }

	///  @name Element access

	const pointer_type &
	top () const
	{ return stack .top (); }

	const pointer_type &
	bottom () const
	{ return stack .bottom (); }

	size_type
	size () const
	{ return stack .size (); }

	void
	push (const pointer_type & node)
	{
		if (stack .top () not_eq node)
		{
			if (stack .top () -> isBound ())
			{
				stack .top () -> set_bind () = false;
				stack .top () -> isBound ()  = false;
			}

			if (not node -> isBound ())
			{
				node -> isBound ()  = true;
				node -> bindTime () = node -> getCurrentTime ();
			}

			if (stack .push (node))
				node -> shutdown .addInterest (this, &X3DBindableNodeStack::erase, node);

			*fields .bindTime = getCurrentTime ();
		}
	}

	bool
	pop (const pointer_type & node)
	{
		if (stack .top () == node)
		{
			if (node -> isBound ())
				node -> isBound () = false;
			
			node -> shutdown .removeInterest (this, &X3DBindableNodeStack::erase);
			
			stack .pop ();
			
			if (not stack .top () -> isBound ())
			{
				stack .top () -> set_bind () = true;
				stack .top () -> isBound ()  = true;
				stack .top () -> bindTime () = stack .top () -> getCurrentTime ();
			}
			
			*fields .bindTime = getCurrentTime ();
			
			return true;
		}

		return false;
	}


private:

	void
	erase (pointer_type node)
	{
		if (not pop (node))
			stack .erase (node);
	}

	struct Fields
	{
		Fields ();

		SFTime* const bindTime;

	};

	Fields fields;

	stack_type stack;

};

template <class Type>
X3DBindableNodeStack <Type>::Fields::Fields () :
	bindTime (new SFTime ())
{ }

} // X3D

} // titania

#endif
