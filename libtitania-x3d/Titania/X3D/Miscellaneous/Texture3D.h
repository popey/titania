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

#ifndef __TITANIA_X3D_MISCELLANEOUS_TEXTURE3D_H__
#define __TITANIA_X3D_MISCELLANEOUS_TEXTURE3D_H__

#include "../Rendering/OpenGL.h"
#include "../Types/Numbers.h"

#include <Magick++.h>
#include <memory>
#include <string>

namespace titania {
namespace X3D {

using MagickImageArray    = std::list <Magick::Image>;
using MagickImageArrayPtr = std::shared_ptr <MagickImageArray>;

class Texture3D
{
public:

	typedef int32_t size_type;

	Texture3D (const MagickImageArrayPtr &);

	GLenum
	getFormat ()
	{ return format; }

	size_type
	getWidth () const
	{ return width; }

	size_type
	getHeight () const
	{ return height; }

	size_type
	getComponents () const
	{ return components; }

	size_type
	getDepth () const
	{ return depth; }

	const void*
	getData ()
	{ return data .data (); }

	virtual
	~Texture3D ()
	{ }


private:

	void
	refineImageFormats (const MagickImageArrayPtr &);

	void
	setData (const MagickImageArrayPtr &);

	void
	addImage (Magick::Image &, size_t);

	MagickImageArrayPtr images;
	
	GLenum    format;
	size_type width;
	size_type height;
	size_type components;
	size_type depth;

	std::vector <char> data;

};

typedef std::unique_ptr <Texture3D> Texture3DPtr;

} // X3D
} // titania

#endif
