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

#include "ParticleSystem.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

ParticleSystem::Fields::Fields () :
	createParticles (new SFBool (true)),
	enabled (new SFBool (true)),
	lifetimeVariation (new SFFloat (0.25)),
	maxParticles (new SFInt32 (200)),
	particleLifetime (new SFFloat (5)),
	particleSize (new SFVec2f (0.02, 0.02)),
	isActive (new SFBool ()),
	colorRamp (new SFNode ()),
	colorKey (new MFFloat ()),
	emitter (new SFNode ()),
	geometryType (new SFString ("QUAD")),
	physics (new MFNode ()),
	texCoordRamp (new SFNode ()),
	texCoordKey (new MFFloat ())
{ }

ParticleSystem::ParticleSystem (X3DExecutionContext* const executionContext) :
	 X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DShapeNode (),
	      fields ()
{
	setComponent ("ParticleSystems");
	setTypeName ("ParticleSystem");

	addField (inputOutput,    "metadata",          metadata ());
	addField (initializeOnly, "bboxSize",          bboxSize ());
	addField (initializeOnly, "bboxCenter",        bboxCenter ());
	addField (inputOutput,    "appearance",        appearance ());
	addField (inputOutput,    "geometry",          geometry ());
	addField (inputOutput,    "createParticles",   createParticles ());
	addField (inputOutput,    "enabled",           enabled ());
	addField (inputOutput,    "lifetimeVariation", lifetimeVariation ());
	addField (inputOutput,    "maxParticles",      maxParticles ());
	addField (inputOutput,    "particleLifetime",  particleLifetime ());
	addField (inputOutput,    "particleSize",      particleSize ());
	addField (outputOnly,     "isActive",          isActive ());
	addField (initializeOnly, "colorRamp",         colorRamp ());
	addField (initializeOnly, "colorKey",          colorKey ());
	addField (initializeOnly, "emitter",           emitter ());
	addField (initializeOnly, "geometryType",      geometryType ());
	addField (initializeOnly, "physics",           physics ());
	addField (initializeOnly, "texCoordRamp",      texCoordRamp ());
	addField (initializeOnly, "texCoordKey",       texCoordKey ());
}

X3DBaseNode*
ParticleSystem::create (X3DExecutionContext* const executionContext) const
{
	return new ParticleSystem (executionContext);
}

} // X3D
} // titania
