/*
 * FTGL - OpenGL font library
 *
 * Copyright (c) 2001-2004 Henry Maddocks <ftgl@opengl.geek.nz>
 * Copyright (c) 2008 Éric Beets <ericbeets@free.fr>
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

#include "PolygonGlyph.h"

#include "../Vectorizer.h"
#include "PolygonGlyphImpl.h"

namespace titania {
namespace FTGL {

//
//  FTGLPolyGlyph
//

PolygonGlyph::PolygonGlyph (FT_GlyphSlot glyph, double outset, bool useDisplayList) :
	Glyph (new PolygonGlyphImpl (glyph, outset, useDisplayList))
{ }

PolygonGlyph::~PolygonGlyph ()
{ }

const Vector3d &
PolygonGlyph::render (const Vector3d & pen, FTGL::RenderMode renderMode)
{
	PolygonGlyphImpl* myimpl = dynamic_cast <PolygonGlyphImpl*> (impl);

	return myimpl -> renderImpl (pen, renderMode);
}

//
//  FTGLPolyGlyphImpl
//

PolygonGlyphImpl::PolygonGlyphImpl (FT_GlyphSlot glyph, double _outset, bool useDisplayList) :
	GlyphImpl (glyph),
	   glList (0)
{
	if (ft_glyph_format_outline not_eq glyph -> format)
	{
		err = 0x14; // Invalid_Outline
		return;
	}

	vectoriser = new Vectorizer (glyph);

	if ((vectoriser -> getContourCount () < 1) or (vectoriser -> getPointCount () < 3))
	{
		delete vectoriser;
		vectoriser = nullptr;
		return;
	}

	hscale = glyph -> face -> size -> metrics .x_ppem * 64;
	vscale = glyph -> face -> size -> metrics .y_ppem * 64;
	outset = _outset;

	if (useDisplayList)
	{
		glList = glGenLists (1);
		glNewList (glList, GL_COMPILE);

		doRender ();

		glEndList ();

		delete vectoriser;
		vectoriser = nullptr;
	}
}

PolygonGlyphImpl::~PolygonGlyphImpl ()
{
	if (glList)
	{
		glDeleteLists (glList, 1);
	}
	else if (vectoriser)
	{
		delete vectoriser;
	}
}

const Vector3d &
PolygonGlyphImpl::renderImpl (const Vector3d & pen, FTGL::RenderMode renderMode)
{
	glTranslatef (pen.x (), pen .y (), pen .z ());

	if (glList)
	{
		glCallList (glList);
	}
	else if (vectoriser)
	{
		doRender ();
	}

	glTranslatef (-pen.x (), -pen.y (), -pen.z ());

	return advance;
}

void
PolygonGlyphImpl::doRender ()
{
	vectoriser -> makeMesh (1, 1, outset);

	const Mesh* mesh = vectoriser -> getMesh ();

	for (uint32_t t = 0; t < mesh -> getTesselationCount (); ++ t)
	{
		const Tesselation* subMesh     = mesh -> getTesselation (t);
		uint32_t           polygonType = subMesh -> PolygonType ();

		glBegin (polygonType);

		for (uint32_t i = 0; i < subMesh -> PointCount (); ++ i)
		{
			Vector3d point = subMesh -> getPoint (i);
			glTexCoord2f (point.x () / hscale, point .y () / vscale);
			glVertex3f (point.x () / 64.0, point .y () / 64.0, 0);
		}

		glEnd ();
	}
}

} // FTGL
} // titania