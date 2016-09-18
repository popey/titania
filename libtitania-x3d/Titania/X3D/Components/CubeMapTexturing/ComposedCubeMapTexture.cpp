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

#include "ComposedCubeMapTexture.h"

#include "../../Browser/Core/Cast.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Texturing/X3DTexture2DNode.h"

namespace titania {
namespace X3D {

const ComponentType ComposedCubeMapTexture::component      = ComponentType::CUBE_MAP_TEXTURING;
const std::string   ComposedCubeMapTexture::typeName       = "ComposedCubeMapTexture";
const std::string   ComposedCubeMapTexture::containerField = "texture";

ComposedCubeMapTexture::Fields::Fields () :
	 front (new SFNode ()),
	  back (new SFNode ()),
	  left (new SFNode ()),
	 right (new SFNode ()),
	   top (new SFNode ()),
	bottom (new SFNode ())
{ }

ComposedCubeMapTexture::ComposedCubeMapTexture (X3DExecutionContext* const executionContext) :
	              X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DEnvironmentTextureNode (),
	                   fields (),
	                    nodes (),
	              transparent (false),
	                    width (0),
                      height (0),
	               components (0),
	                loadState (NOT_STARTED_STATE)
{
	addType (X3DConstants::ComposedCubeMapTexture);

	addField (inputOutput, "metadata", metadata ());
	addField (inputOutput, "front",    front ());
	addField (inputOutput, "back",     back ());
	addField (inputOutput, "left",     left ());
	addField (inputOutput, "right",    right ());
	addField (inputOutput, "top",      top ());
	addField (inputOutput, "bottom",   bottom ());

	addChildren (textureProperties ());
	addChildren (nodes, loadState);
}

X3DBaseNode*
ComposedCubeMapTexture::create (X3DExecutionContext* const executionContext) const
{
	return new ComposedCubeMapTexture (executionContext);
}

void
ComposedCubeMapTexture::initialize ()
{
	X3DEnvironmentTextureNode::initialize ();

	if (glXGetCurrentContext ())
	{
		front ()  .addInterest (this, &ComposedCubeMapTexture::set_texture_0, std::cref (front ()));
		back ()   .addInterest (this, &ComposedCubeMapTexture::set_texture_1, std::cref (back ()));
		left ()   .addInterest (this, &ComposedCubeMapTexture::set_texture_2, std::cref (left ()));
		right ()  .addInterest (this, &ComposedCubeMapTexture::set_texture_3, std::cref (right ()));
		top ()    .addInterest (this, &ComposedCubeMapTexture::set_texture_4, std::cref (top ()));
		bottom () .addInterest (this, &ComposedCubeMapTexture::set_texture_5, std::cref (bottom ()));
		
		nodes .resize (6);

		set_texture_0 (front ());
		set_texture_1 (back ());
		set_texture_2 (left ());
		set_texture_3 (right ());
		set_texture_4 (top ());
		set_texture_5 (bottom ());
	}
}

void
ComposedCubeMapTexture::set_texture_0 (const SFNode & node)
{
	constexpr size_t i = 0;

	if (nodes [i])
		nodes [i] -> removeInterest (this, &ComposedCubeMapTexture::set_texture_0);

	nodes [i] = node;

	if (nodes [i])
		nodes [i] -> addInterest (this, &ComposedCubeMapTexture::set_texture_0, std::cref (node));

	setTexture (getTargets () [i], node);
}

void
ComposedCubeMapTexture::set_texture_1 (const SFNode & node)
{
	constexpr size_t i = 1;

	if (nodes [i])
		nodes [i] -> removeInterest (this, &ComposedCubeMapTexture::set_texture_1);

	nodes [i] = node;

	if (nodes [i])
		nodes [i] -> addInterest (this, &ComposedCubeMapTexture::set_texture_1, std::cref (node));

	setTexture (getTargets () [i], node);
}

void
ComposedCubeMapTexture::set_texture_2 (const SFNode & node)
{
	constexpr size_t i = 2;

	if (nodes [i])
		nodes [i] -> removeInterest (this, &ComposedCubeMapTexture::set_texture_2);

	nodes [i] = node;

	if (nodes [i])
		nodes [i] -> addInterest (this, &ComposedCubeMapTexture::set_texture_2, std::cref (node));

	setTexture (getTargets () [i], node);
}

void
ComposedCubeMapTexture::set_texture_3 (const SFNode & node)
{
	constexpr size_t i = 3;

	if (nodes [i])
		nodes [i] -> removeInterest (this, &ComposedCubeMapTexture::set_texture_3);

	nodes [i] = node;

	if (nodes [i])
		nodes [i] -> addInterest (this, &ComposedCubeMapTexture::set_texture_3, std::cref (node));

	setTexture (getTargets () [i], node);
}

void
ComposedCubeMapTexture::set_texture_4 (const SFNode & node)
{
	constexpr size_t i = 4;

	if (nodes [i])
		nodes [i] -> removeInterest (this, &ComposedCubeMapTexture::set_texture_4);

	nodes [i] = node;

	if (nodes [i])
		nodes [i] -> addInterest (this, &ComposedCubeMapTexture::set_texture_4, std::cref (node));

	setTexture (getTargets () [i], node);
}

void
ComposedCubeMapTexture::set_texture_5 (const SFNode & node)
{
	constexpr size_t i = 5;

	if (nodes [i])
		nodes [i] -> removeInterest (this, &ComposedCubeMapTexture::set_texture_5);

	nodes [i] = node;

	if (nodes [i])
		nodes [i] -> addInterest (this, &ComposedCubeMapTexture::set_texture_5, std::cref (node));

	setTexture (getTargets () [i], node);
}

void
ComposedCubeMapTexture::setTexture (const GLenum target, const SFNode & node)
{
	const auto texture = x3d_cast <X3DTexture2DNode*> (node);

	if (texture)
	{
		// Get transparent and components

		transparent = false;
		components  = 0;

		for (const auto & node : nodes)
		{
			const auto texture = x3d_cast <X3DTexture2DNode*> (node);

			if (texture)
			{
				transparent = transparent or texture -> isTransparent ();
				components  = std::max (components, texture -> getComponents ());
			}
		}

		// Get texture 2d data as four component RGBA

		width  = texture -> getWidth ();
		height = texture -> getHeight ();

		std::vector <uint8_t> image (width * height * 4);

		glBindTexture (GL_TEXTURE_2D, texture -> getTextureId ());
		glGetTexImage (GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, image .data ());
		glBindTexture (GL_TEXTURE_2D, 0);

		// Flip image vertically

		const size_t width4   = width * 4;
		const size_t height_1 = height - 1;

		for (size_t r = 0, height1_2 = height / 2; r < height1_2; ++ r)
		{
			for (size_t c = 0; c < width4; ++ c)
			{
				std::swap (image [r * width4 + c], image [(height_1 - r) * width4 + c]);
			}
		}

		// Transfer image

		if (width == height)
		{
			setImage (target, GL_RGBA, GL_RGBA, image .data ());
			setLoadState (COMPLETE_STATE);
			return;
		}

		getBrowser () -> println ("Error: ComposedCubeMapTexture: width and height must be equal, and all images must be of the same size!");
	}

	width      = 0;
	height     = 0;
	components = 0;

	setImage (target, GL_RGBA, GL_RGBA, nullptr);
	setLoadState (FAILED_STATE);
}

void
ComposedCubeMapTexture::dispose ()
{
	removeChildren (textureProperties ());

	X3DEnvironmentTextureNode::dispose ();
}

} // X3D
} // titania
