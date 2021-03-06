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
 * Copyright 1999, 2016 Holger Seelig <holger.seelig@yahoo.de>.
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

#include "X3DTransformMatrix3DNode.h"

#include "../../Rendering/X3DRenderObject.h"

namespace titania {
namespace X3D {

X3DTransformMatrix3DNode::X3DTransformMatrix3DNode () :
	           X3DGroupingNode (),
	X3DTransformMatrix3DObject ()
{
	addType (X3DConstants::X3DTransformMatrix3DNode);
}

void
X3DTransformMatrix3DNode::initialize ()
{
	X3DGroupingNode::initialize ();
	X3DTransformMatrix3DObject::initialize ()	;
}

Box3d
X3DTransformMatrix3DNode::getBBox () const
{
	if (isHidden ())
		return Box3d ();

	return X3DGroupingNode::getBBox () * getMatrix ();
}

void
X3DTransformMatrix3DNode::traverse (const TraverseType type, X3DRenderObject* const renderObject)
{
	renderObject -> getModelViewMatrix () .push ();
	renderObject -> getModelViewMatrix () .mult_left (getMatrix ());

	X3DGroupingNode::traverse (type, renderObject);

	renderObject -> getModelViewMatrix () .pop ();
}

void
X3DTransformMatrix3DNode::dispose ()
{
	X3DTransformMatrix3DObject::dispose ();
	X3DGroupingNode::dispose ();
}

} // X3D
} // titania
