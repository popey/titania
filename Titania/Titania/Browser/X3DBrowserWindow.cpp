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

#include "X3DBrowserWindow.h"

#include "../Widgets/Console/Console.h"
#include "../Widgets/HistoryView/HistoryView.h"
#include "../Widgets/LibraryView/LibraryView.h"
#include "../Widgets/OutlineEditor/OutlineEditor.h"
#include "../Widgets/OutlineEditor/OutlineTreeModel.h"
#include "../Widgets/OutlineEditor/OutlineTreeViewEditor.h"
#include "../Widgets/ScriptEditor/ScriptEditor.h"
#include "../Widgets/ViewpointList/ViewpointList.h"

namespace titania {
namespace puck {

X3DBrowserWindow::X3DBrowserWindow (const X3D::BrowserPtr & browser) :
	X3DBrowserEditor (browser),
	   viewpointList (new ViewpointList (this, true)),
	   historyEditor (new HistoryView (this)),
	     libraryView (new LibraryView (this)),
	   outlineEditor (new OutlineEditor (this)),
	         console (new Console (this)),
	    scriptEditor (new ScriptEditor (this)),
	            keys (),
	    accelerators (true),
	    saveAsOutput ()
{ }

void
X3DBrowserWindow::initialize ()
{
	X3DBrowserEditor::initialize ();

	viewpointList -> reparent (getViewpointListBox (), getWindow ());
	historyEditor -> reparent (getHistoryViewBox (),   getWindow ());
	libraryView   -> reparent (getLibraryViewBox (),   getWindow ());
	outlineEditor -> reparent (getOutlineEditorBox (), getWindow ());
	console       -> reparent (getConsoleBox (),       getWindow ());
	scriptEditor  -> reparent (getScriptEditorBox (),  getWindow ());
}

void
X3DBrowserWindow::setBrowser (const X3D::BrowserPtr & value)
{
	X3DBrowserEditor::setBrowser (value);
}

const std::shared_ptr <OutlineTreeViewEditor> &
X3DBrowserWindow::getOutlineTreeView () const
{
	return outlineEditor -> getTreeView ();
}

// Menu

void
X3DBrowserWindow::hasAccelerators (bool value)
{
	accelerators = value;

	for (const auto & child : getMenuBar () .get_children ())
	{
		const auto menuItem = dynamic_cast <Gtk::MenuItem*> (child);

		if (menuItem and menuItem -> get_visible ())
		{
			const auto menu = menuItem -> get_submenu ();

			if (menu)
				menu -> set_sensitive (accelerators);
		}
	}
}

void
X3DBrowserWindow::expandNodes (const X3D::MFNode & nodes)
{
	if (getConfig () .getBoolean ("followPrimarySelection"))
	{
		getBrowser () -> addEvent ();
		getBrowser () -> finished () .addInterest (this, &X3DBrowserWindow::expandNodesImpl, nodes);
	}
}

void
X3DBrowserWindow::expandNodesImpl (const X3D::MFNode & nodes)
{
	getBrowser () -> finished () .removeInterest (this, &X3DBrowserWindow::expandNodesImpl);

	for (const auto & node : nodes)
	{
		getOutlineTreeView () -> expand_to (node);

		for (const auto & iter : getOutlineTreeView () -> get_iters (node))
		{
			const auto path = getOutlineTreeView () -> get_model () -> get_path (iter);

			getOutlineTreeView () -> expand_row (path, false);
			getOutlineTreeView () -> scroll_to_row (path);
		}
	}
}

X3DBrowserWindow::~X3DBrowserWindow ()
{ }

} // puck
} // titania