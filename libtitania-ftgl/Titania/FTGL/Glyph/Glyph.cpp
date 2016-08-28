/*
 * FTGL - OpenGL font library
 *
 * Copyright (c) 2001-2004 Henry Maddocks <ftgl@opengl.geek.nz>
 * Copyright (c) 2008 Sam Hocevar <sam@zoy.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "Glyph.h"

#include "GlyphImpl.h"

namespace titania {
namespace FTGL {

//
//  Glyph
//

Glyph::Glyph (FT_GlyphSlot glyph)
{
	impl = new GlyphImpl (glyph);
}

Glyph::Glyph (GlyphImpl* pImpl)
{
	impl = pImpl;
}

Glyph::~Glyph ()
{
	delete impl;
}

double
Glyph::getAdvance () const
{
	return impl -> getAdvance ();
}

const BBox &
Glyph::getBBox () const
{
	return impl -> getBBox ();
}

FT_Error
Glyph::getError () const
{
	return impl -> getError ();
}

//
//  GlyphImpl
//

GlyphImpl::GlyphImpl (FT_GlyphSlot glyph, bool useList) :
	err (0)
{
	if (glyph)
	{
		bBox    = BBox (glyph);
		advance = Vector3d (glyph -> advance .x / 64.0,
		                    glyph -> advance .y / 64.0,
	                       0);
	}
}

GlyphImpl::~GlyphImpl ()
{ }

double
GlyphImpl::getAdvance () const
{
	return advance .x ();
}

const BBox &
GlyphImpl::getBBox () const
{
	return bBox;
}

FT_Error
GlyphImpl::getError () const
{
	return err;
}

} // FTGL
} // titania