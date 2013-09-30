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

#include "X3DUserInterface.h"

#include "../Configuration/config.h"
#include <iostream>

namespace titania {
namespace puck {

const std::string X3DUserInterface::componentName  = "BrowserWindow";
const std::string X3DUserInterface::containerField = "widgets";

X3DUserInterface::Array X3DUserInterface::userInterfaces;

X3DUserInterface::X3DUserInterface (const std::string & widgetName, const std::string & configKey) :
         X3DBaseInterface (),
	                 gconf (configKey, widgetName),
	initialized_connection (),
	         userInterface (),
	               dialogs ()
{ }

void
X3DUserInterface::construct ()
{
	initialized_connection = getWidget () .signal_map () .connect (sigc::mem_fun (*this, &X3DUserInterface::set_initialized));

	getWidget () .signal_map ()          .connect (sigc::mem_fun (*this, &X3DUserInterface::on_map));
	getWindow () .signal_delete_event () .connect (sigc::mem_fun (*this, &X3DUserInterface::on_delete_event));

	userInterfaces .emplace_back (this);
	userInterface = -- userInterfaces .end ();

	restoreInterface ();
}

void
X3DUserInterface::set_initialized ()
{
	initialized_connection .disconnect ();

	std::clog << "Initializing " << getWidgetName () << "." << std::endl;

	initialize ();

	std::clog << "Done initializing " << getWidgetName () << "." << std::endl;
}

void
X3DUserInterface::on_map ()
{
	// Restore position before window becomes visible.
	restoreInterface ();
}

bool
X3DUserInterface::on_delete_event (GdkEventAny*)
{
	if (this == userInterfaces .front ())
		saveInterfaces ();
	else
		saveInterface ();

	return close ();
}

void
X3DUserInterface::initialize ()
{ }

bool
X3DUserInterface::is_initialized ()
{
	return not initialized_connection .connected ();
}

void
X3DUserInterface::addDialog (X3DUserInterface* const dialog)
{
	dialog -> getWindow () .signal_hide () .connect (sigc::bind (sigc::mem_fun (*this, &X3DUserInterface::removeDialog), dialog));
	dialog -> getWindow () .present ();
	dialogs .insert (std::make_pair (dialog, dialog));
}

void
X3DUserInterface::removeDialog (X3DUserInterface* const dialog)
{
	dialogs .erase (dialog);
}

void
X3DUserInterface::reparent (Gtk::Widget & widget, Gtk::Window & window)
{
	getWindow () .set_transient_for (window);
	getWidget () .reparent (widget);
}

void
X3DUserInterface::toggleWidget (Gtk::Widget & widget, bool active)
{
	if (active)
		widget .show ();

	else
		widget .hide ();
}

void
X3DUserInterface::restoreInterface ()
{
	getWindow () .move (getConfig ().integer ("x"),
	                    getConfig ().integer ("y"));

	if (getConfig ().integer ("width") > 0 and getConfig ().integer ("height") > 0)
	{
		getWindow () .resize (getConfig ().integer ("width"),
		                      getConfig ().integer ("height"));
	}
}

void
X3DUserInterface::saveInterfaces ()
{
	for (const auto & userInterface : userInterfaces)
		userInterface -> saveInterface ();
}

void
X3DUserInterface::saveInterface ()
{
	int x, y, width, height;

	getWindow () .get_position (x, y);
	getConfig ().setItem ("x", x);
	getConfig ().setItem ("y", y);

	getWindow () .get_size (width, height);
	getConfig ().setItem ("width",  width);
	getConfig ().setItem ("height", height);
}

void
X3DUserInterface::saveSession (const std::string & path)
{
	//	printStatistics ();

	getConfig () .setPath (path);

	saveSession ();
}

void
X3DUserInterface::saveSession ()
{ }

bool
X3DUserInterface::close ()
{
	saveSession ();

	getWindow () .hide ();

	// Prevent destroying Window.
	return true;
}

X3DUserInterface::~X3DUserInterface ()
{
	userInterfaces .erase (userInterface);
}

} // puck
} // titania
