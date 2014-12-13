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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_V8_V8BROWSER_H__
#define __TITANIA_X3D_JAVA_SCRIPT_V8_V8BROWSER_H__

#include <v8.h>

#include "Context.h"

namespace titania {
namespace X3D {
namespace GoogleV8 {

class Browser
{
public:

	static
	void
	initialize (Context* const, const v8::Local <v8::Object> &);


private:

	// X3D properties

	static
	v8::Handle <v8::Value>
	name (v8::Local <v8::String>, const v8::AccessorInfo &);

	static
	v8::Handle <v8::Value>
	version (v8::Local <v8::String>, const v8::AccessorInfo &);

	static
	v8::Handle <v8::Value>
	currentSpeed (v8::Local <v8::String>, const v8::AccessorInfo &);

	static
	v8::Handle <v8::Value>
	currentFrameRate (v8::Local <v8::String>, const v8::AccessorInfo &);

	static
	void
	description (v8::Local <v8::String>, v8::Local <v8::Value>, const v8::AccessorInfo &);

	static
	v8::Handle <v8::Value>
	description (v8::Local <v8::String>, const v8::AccessorInfo &);

//	static
//	v8::Handle <v8::Value>
//	supportedComponents (v8::Local <v8::String>, const v8::AccessorInfo &);
//
//	static
//	v8::Handle <v8::Value>
//	supportedProfiles (v8::Local <v8::String>, const v8::AccessorInfo &);

	static
	v8::Handle <v8::Value>
	currentScene (v8::Local <v8::String>, const v8::AccessorInfo &);

	// VRML functions

	static
	v8::Handle <v8::Value>
	getName (const v8::Arguments &);

	static
	v8::Handle <v8::Value>
	getVersion (const v8::Arguments &);

	static
	v8::Handle <v8::Value>
	getCurrentSpeed (const v8::Arguments &);

	static
	v8::Handle <v8::Value>
	getCurrentFrameRate (const v8::Arguments &);

	static
	v8::Handle <v8::Value>
	getWorldURL (const v8::Arguments &);

	static
	v8::Handle <v8::Value>
	setDescription (const v8::Arguments &);

	static
	v8::Handle <v8::Value>
	createVrmlFromString (const v8::Arguments &);

	static
	v8::Handle <v8::Value>
	createVrmlFromURL (const v8::Arguments &);

	static
	v8::Handle <v8::Value>
	addRoute (const v8::Arguments &);

	static
	v8::Handle <v8::Value>
	deleteRoute (const v8::Arguments &);

};

} // GoogleV8
} // X3D
} // titania

#endif
