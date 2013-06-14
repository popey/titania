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

#ifndef __TITANIA_X3D_EXECUTION_BINDABLE_NODE_LIST_H__
#define __TITANIA_X3D_EXECUTION_BINDABLE_NODE_LIST_H__

#include "../Base/X3DOutput.h"

#include "../Components/EnvironmentalEffects/Fog.h"
#include "../Components/EnvironmentalEffects/X3DBackgroundNode.h"
#include "../Components/Navigation/NavigationInfo.h"
#include "../Components/Navigation/X3DViewpointNode.h"

namespace titania {
namespace X3D {

template <class Type>
class BindableNodeList :
	public X3DOutput
{
public:

	typedef Type*                              value_type;
	typedef std::deque <value_type>            list_type;
	typedef typename list_type::const_iterator const_iterator;
	typedef typename list_type::size_type      size_type;

	/// @name Constructors

	///  Default constructor.
	BindableNodeList () :
		list ()
	{ }

	/// @name Iterators

	const_iterator
	begin () const
	{ return list .cbegin (); }

	const_iterator
	cbegin () const
	{ return list .cbegin (); }

	const_iterator
	end () const
	{ return list .cend (); }

	const_iterator
	cend () const
	{ return list .cend (); }

	///  @a name Element access

	value_type
	operator [ ] (const size_type & index) const
	{ return list [index]; }

	value_type
	at (const size_type & index) const
	{ return list .at (index); }

	/// @name Capacity

	bool
	empty () const
	{ return list .empty (); }

	size_type
	size () const
	{ return list .size (); }

	size_type
	max_size () const
	{ return list .max_size (); }

	/// @name Modifiers

	void
	push_back (value_type node)
	{ temp .emplace_back (node); }

	void
	update ()
	{
		if (temp not_eq list)
		{
			for (auto & node : list)
				node -> shutdown .removeInterest (this, &BindableNodeList::erase);
			
			list = std::move (temp);
			
			for (auto & node : list)
				node -> shutdown .addInterest (this, &BindableNodeList::erase, node);
			
			processInterests ();
		}
		else
			temp .clear ();
	}

	void
	erase (value_type node)
	{
		auto end = std::remove (list .begin (), list .end (), node);
	
		if (end not_eq list .end ())
		{
			list .erase (end, list .end ());
			processInterests ();
		}
	}

	void
	dispose ()
	{
		for (auto & node : list)
			node -> shutdown .removeInterest (this, &BindableNodeList::erase);

		list .clear ();

		X3DOutput::dispose ();
	}

	/// @name Destructor

	~BindableNodeList ()
	{ }


private:

	list_type temp;
	list_type list;

};

extern template class BindableNodeList <NavigationInfo>;
extern template class BindableNodeList <X3DBackgroundNode>;
extern template class BindableNodeList <Fog>;
extern template class BindableNodeList <X3DViewpointNode>;

} // X3D
} // titania

#endif
