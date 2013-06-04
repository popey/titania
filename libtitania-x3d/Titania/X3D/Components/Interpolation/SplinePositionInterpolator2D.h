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

#ifndef __TITANIA_X3D_COMPONENTS_INTERPOLATION_SPLINE_POSITION_INTERPOLATOR2D_H__
#define __TITANIA_X3D_COMPONENTS_INTERPOLATION_SPLINE_POSITION_INTERPOLATOR2D_H__

#include "../Interpolation/X3DInterpolatorNode.h"
#include <Titania/Math/Algorithms/CatmullRomSplineInterpolator.h>

namespace titania {
namespace X3D {

class SplinePositionInterpolator2D :
	public X3DInterpolatorNode
{
public:

	SplinePositionInterpolator2D (X3DExecutionContext* const);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const) const final;

	///  @name Fields

	SFBool &
	closed ()
	{ return *fields .closed; }

	const SFBool &
	closed () const
	{ return *fields .closed; }

	MFVec2f &
	keyValue ()
	{ return *fields .keyValue; }

	const MFVec2f &
	keyValue () const
	{ return *fields .keyValue; }

	MFVec2f &
	keyVelocity ()
	{ return *fields .keyVelocity; }

	const MFVec2f &
	keyVelocity () const
	{ return *fields .keyVelocity; }

	SFBool &
	normalizeVelocity ()
	{ return *fields .normalizeVelocity; }

	const SFBool &
	normalizeVelocity () const
	{ return *fields .normalizeVelocity; }

	SFVec2f &
	value_changed ()
	{ return *fields .value_changed; }

	const SFVec2f &
	value_changed () const
	{ return *fields .value_changed; }


private:

	virtual
	void
	initialize ();

	virtual
	void
	set_keyValue ();

	void
	set_keyVelocity ();

	virtual
	void
	interpolate (size_t, size_t, float);

	struct Fields
	{
		Fields ();

		SFBool* const closed;
		MFVec2f* const keyValue;
		MFVec2f* const keyVelocity;
		SFBool* const normalizeVelocity;
		SFVec2f* const value_changed;
	};

	Fields fields;

	math::catmull_rom_spline_interpolator <Vector2f, float> spline;

};

} // X3D
} // titania

#endif
