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

#include "TextureCoordinate3D.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const ComponentType TextureCoordinate3D::component      = ComponentType::TEXTURING_3D;
const std::string   TextureCoordinate3D::typeName       = "TextureCoordinate3D";
const std::string   TextureCoordinate3D::containerField = "texCoord";

TextureCoordinate3D::Fields::Fields () :
	point (new MFVec3f ())
{ }

TextureCoordinate3D::TextureCoordinate3D (X3DExecutionContext* const executionContext) :
	             X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DTextureCoordinateNode (),
	                  fields ()
{
	addType (X3DConstants::TextureCoordinate3D);

	addField (inputOutput, "metadata", metadata ());
	addField (inputOutput, "point",    point ());
}

X3DBaseNode*
TextureCoordinate3D::create (X3DExecutionContext* const executionContext) const
{
	return new TextureCoordinate3D (executionContext);
}

void
TextureCoordinate3D::init (TexCoordArray & texCoords, const size_t reserve) const
{
	texCoords .emplace_back ();
	texCoords .back () .reserve (reserve);
}

void
TextureCoordinate3D::addTexCoord (const size_t channel, TexCoordArray & texCoords, const size_t index) const
{
	if (index < point () .size ())
	{
		const Vector3f & point3 = point () [index];
		texCoords [channel] .emplace_back (point3 .x (), point3 .y (), point3 .z (), 1);
	}
	else
		texCoords [channel] .emplace_back (0, 0, 0, 1);
}

void
TextureCoordinate3D::getTexCoord (std::vector <Vector4f> & texCoords) const
{
	texCoords .reserve (point () .size ());

	for (const Vector3f & point3 : point ())
		texCoords .emplace_back (point3 .x (), point3 .y (), point3 .z (), 1);	
}

void
TextureCoordinate3D::enable (const int32_t unit, const size_t channel, const TexCoordArray & texCoords) const
{
	glClientActiveTexture (GL_TEXTURE0 + unit);
	glEnableClientState (GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer (4, GL_FLOAT, 0, texCoords [channel] .data ());
}

void
TextureCoordinate3D::disable (const int32_t unit) const
{
	glClientActiveTexture (GL_TEXTURE0 + unit);
	glDisableClientState (GL_TEXTURE_COORD_ARRAY);
}

} // X3D
} // titania
