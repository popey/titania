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

#include "ColorRGBA.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const ComponentType ColorRGBA::component      = ComponentType::RENDERING;
const std::string   ColorRGBA::typeName       = "ColorRGBA";
const std::string   ColorRGBA::containerField = "color";

ColorRGBA::Fields::Fields () :
	color (new MFColorRGBA ())
{ }

ColorRGBA::ColorRGBA (X3DExecutionContext* const executionContext) :
	        X3DBaseNode (executionContext -> getBrowser (), executionContext),
	       X3DColorNode (),
	             fields (),
	        transparent (true),
	dynamicTransparency (false)
{
	addType (X3DConstants::ColorRGBA);

	addField (inputOutput, "metadata", metadata ());
	addField (inputOutput, "color",    color ());
}

X3DBaseNode*
ColorRGBA::create (X3DExecutionContext* const executionContext) const
{
	return new ColorRGBA (executionContext);
}

void
ColorRGBA::setDynamicTransparency (const bool value)
{
	dynamicTransparency = value;
	
	if (dynamicTransparency)
	{
		color () .addInterest (this, &ColorRGBA::set_color);

		set_color ();
	}
	else
	{
		color () .removeInterest (this, &ColorRGBA::set_color);

		transparent = true;
	}
}

void
ColorRGBA::set1Color (const size_t index, const Color4f & value)
{
	color () .set1Value (index, value);
}

Color4f
ColorRGBA::get1Color (const size_t index)
{
	return color () .get1Value (index);
}

void
ColorRGBA::addColor (std::vector <Color4f> & colors, const size_t index) const
{
	if (index < color () .size ())
		colors .emplace_back (color () [index]);
		
	else
		colors .emplace_back (1, 1, 1, 1);
}

void
ColorRGBA::getHSVA (std::vector <Vector4f> & colors) const
{
	colors .reserve (color () .size ());

	for (const Color4f & color4 : color ())
	{
		float h, s, v;
		color4 .get_hsv (h, s, v);
		colors .emplace_back (h, s, v, color4 .a ());
	}
}

void
ColorRGBA::set_color ()
{
	transparent = std::any_of (color () .begin (),
	                           color () .end (),
	                           [ ] (const Color4f & value) { return value .a () < 1; });
}

} // X3D
} // titania
