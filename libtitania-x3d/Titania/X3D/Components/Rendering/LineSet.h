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

#ifndef __TITANIA_X3D_COMPONENTS_RENDERING_LINE_SET_H__
#define __TITANIA_X3D_COMPONENTS_RENDERING_LINE_SET_H__

#include "../Rendering/X3DGeometryNode.h"

namespace titania {
namespace X3D {

class LineSet :
	public X3DGeometryNode
{
public:

	LineSet (X3DExecutionContext* const);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const) const final;

	///  @name Fields

	MFInt32 &
	vertexCount ()
	{ return *fields .vertexCount; }

	const MFInt32 &
	vertexCount () const
	{ return *fields .vertexCount; }

	MFNode &
	attrib ()
	{ return *fields .attrib; }

	const MFNode &
	attrib () const
	{ return *fields .attrib; }

	SFNode <X3DBaseNode> &
	fogCoord ()
	{ return *fields .fogCoord; }

	const SFNode <X3DBaseNode> &
	fogCoord () const
	{ return *fields .fogCoord; }

	SFNode <X3DBaseNode> &
	color ()
	{ return *fields .color; }

	const SFNode <X3DBaseNode> &
	color () const
	{ return *fields .color; }

	SFNode <X3DBaseNode> &
	coord ()
	{ return *fields .coord; }

	const SFNode <X3DBaseNode> &
	coord () const
	{ return *fields .coord; }

	virtual
	void
	build ();

	virtual
	void
	draw ();


private:

	struct Fields
	{
		Fields ();

		MFInt32* const vertexCount;
		MFNode* const attrib;
		SFNode <X3DBaseNode>* const fogCoord;
		SFNode <X3DBaseNode>* const color;
		SFNode <X3DBaseNode>* const coord;
	};

	Fields fields;

};

} // X3D
} // titania

#endif
