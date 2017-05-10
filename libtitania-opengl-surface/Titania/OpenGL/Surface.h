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

#ifndef __TITANIA_OPEN_GL_SURFACE_H__
#define __TITANIA_OPEN_GL_SURFACE_H__

// include order is important
#include <gtkmm/drawingarea.h>

#include <Titania/Math/Numbers/Vector4.h>

#include <memory>
#include <thread>

namespace titania {
namespace OpenGL {

class Context;

class Surface :
	public Gtk::DrawingArea
{
public:

	///  @name Member access

	void
	setAttributes (const int32_t antialiasing, const bool accumBuffer);

	///  @name Operations

	bool
	makeCurrent () const;

	void
	setSwapInterval (const size_t value);

	void
	swapBuffers () const;

	///  @name Destruction

	virtual
	void
	dispose ();

	virtual
	~Surface () override;


protected:

	///  @name Construction

	Surface ();

	Surface (const Surface & sharingSurface);

	/// @name OpenGL handler

	virtual
	void
	setup () = 0;

	virtual
	void
	initialize ();

	virtual
	void
	reshape (const math::vector4 <int32_t> &) = 0;

	virtual
	void
	update () = 0;

	virtual
	void
	on_map () override;

	virtual
	bool
	on_configure_event (GdkEventConfigure* const event) override;

	virtual
	bool
	on_draw (const Cairo::RefPtr <Cairo::Context> & cairo) override;

	virtual
	void
	on_unmap () override;

	virtual
	void
	on_unrealize () override;


private:

	///  @name Construction

	Surface (const std::shared_ptr <Context> & sharingContext);

	void
	createContext ();

	///  @name Members

	std::thread::id           treadId;
	std::shared_ptr <Context> context;
	std::shared_ptr <Context> sharingContext;
	std::vector <int32_t>     visualAttributes;

};

} // OpenGL
} // titania

#endif
