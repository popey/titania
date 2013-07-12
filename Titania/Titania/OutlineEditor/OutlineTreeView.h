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

#ifndef __TITANIA_OUTLINE_EDITOR_OUTLINE_TREE_VIEW_H__
#define __TITANIA_OUTLINE_EDITOR_OUTLINE_TREE_VIEW_H__

#include <gtkmm.h>
#include <iostream>

#include "../Base/X3DBaseInterface.h"
#include "../OutlineEditor/OutlineSelection.h"
#include "../OutlineEditor/OutlineIterData.h"
#include "../OutlineEditor/OutlineUserData.h"
#include <Titania/X3D.h>

namespace titania {
namespace puck {

class OutlineTreeModel;

class OutlineTreeView :
	public Gtk::TreeView, public X3DBaseInterface
{
public:

	OutlineTreeView (const X3D::X3DSFNode <X3D::Browser> &);

	const Glib::RefPtr <OutlineTreeModel> &
	get_model () const
	{ return model; }

	OutlineIterType
	get_data_type (const Gtk::TreeModel::iterator &) const;

	X3D::X3DChildObject*
	get_object (const Gtk::TreeModel::iterator &) const;

	OutlineUserData*
	get_user_data (const Gtk::TreeModel::iterator &) const;

	OutlineUserData*
	get_user_data (X3D::X3DChildObject*) const;

	void
	set_animated (const Gtk::TreeModel::iterator &, bool);

	bool
	get_animated (const Gtk::TreeModel::iterator &) const;

	virtual
	~OutlineTreeView ();


private:

	void
	set_model (const Glib::RefPtr <OutlineTreeModel> &);

	void
	set_path (const Gtk::TreeModel::iterator &, const Gtk::TreeModel::Path &);

	Gtk::TreeModel::Path
	get_path (const Gtk::TreeModel::iterator &) const;

	void
	set_expanded (const Gtk::TreeModel::iterator &, bool);
	
	bool
	get_expanded (const Gtk::TreeModel::iterator &) const;

	void
	set_expand_all (const Gtk::TreeModel::iterator &, bool);
	
	bool
	get_expand_all (const Gtk::TreeModel::iterator &) const;

	void
	set_world ();

	virtual
	bool
	on_key_press_event (GdkEventKey*);

	virtual
	bool
	on_key_release_event (GdkEventKey*);

	virtual
	void
	on_row_activated (const Gtk::TreeModel::Path &, Gtk::TreeViewColumn*);

	void
	on_row_inserted (const Gtk::TreeModel::Path &, const Gtk::TreeModel::iterator &);

	void
	on_row_has_child_toggled (const Gtk::TreeModel::Path &, const Gtk::TreeModel::iterator &);

	virtual
	bool
	on_test_expand_row (const Gtk::TreeModel::iterator &, const Gtk::TreeModel::Path &);

	virtual
	void
	on_row_expanded (const Gtk::TreeModel::iterator &, const Gtk::TreeModel::Path &);

	virtual
	bool
	on_test_collapse_row (const Gtk::TreeModel::iterator &, const Gtk::TreeModel::Path &);

	virtual
	void
	on_row_collapsed (const Gtk::TreeModel::iterator & iter, const Gtk::TreeModel::Path & path);

	void
	watch (const Gtk::TreeModel::iterator &, const Gtk::TreeModel::Path &);

	void
	unwatch_tree (const Gtk::TreeModel::iterator &);

	void
	unwatch_tree (const Gtk::TreeModel::iterator &, std::set <X3D::X3DChildObject*> &);

	void
	unwatch (const Gtk::TreeModel::iterator &);

	void
	collapse_field (const Gtk::TreeModel::Path &);

	void
	collapse_clone (const Gtk::TreeModel::iterator &);

	void
	select_fields (const Gtk::TreeModel::iterator &, const Gtk::TreeModel::Path &);

	void
	toggle_expand (const Gtk::TreeModel::iterator &, const Gtk::TreeModel::Path & path);

	void
	auto_expand_fields (const Gtk::TreeModel::iterator &);

	void
	select (const Gtk::TreeModel::iterator &, const Gtk::TreeModel::Path &);

	class Keys
	{
	public:

		Keys () :
			shift_l (false),
			shift_r (false)
		{ }

		bool
		shift ()
		{ return shift_l or shift_r; }

		bool shift_l;
		bool shift_r;

	};

	Glib::RefPtr <OutlineTreeModel> model;
	OutlineSelection                selection;
	Keys                            keys;

};

} // puck
} // titania

#endif
