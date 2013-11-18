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

#ifndef __TITANIA_BROWSER_BROWSER_WINDOW_H__
#define __TITANIA_BROWSER_BROWSER_WINDOW_H__

#include "../Browser/X3DBrowserEditor.h"
#include "../HistoryEditor/HistoryEditor.h"
#include "../LibraryView/LibraryView.h"
#include "../MotionBlurEditor/MotionBlurEditor.h"
#include "../OutlineEditor/OutlineEditor.h"
#include "../ViewpointList/ViewpointList.h"

namespace titania {
namespace puck {

class BrowserWindow :
	public X3DBrowserEditor
{
public:

	/// @name Construction

	BrowserWindow (const X3D::X3DSFNode <X3D::Browser> &, const basic::uri &);

	/// @name Widgets
	
	const X3D::X3DSFNode <X3D::Browser> &
	getBrowserSurface () const
	{ return browserSurface; }

	MotionBlurEditor &
	getMotionBlurEditor ()
	{ return motionBlurEditor; }

	const MotionBlurEditor &
	getMotionBlurEditor () const
	{ return motionBlurEditor; }

	LibraryView &
	getLibraryView ()
	{ return libraryView; }

	const LibraryView &
	getLibraryView () const
	{ return libraryView; }

	ViewpointList &
	getViewpointList ()
	{ return viewpointList; }

	const ViewpointList &
	getViewpointList () const
	{ return viewpointList; }

	HistoryEditor &
	getHistoryEditor ()
	{ return historyEditor; }

	const HistoryEditor &
	getHistoryEditor () const
	{ return historyEditor; }

	OutlineEditor &
	getOutlineEditor ()
	{ return outlineEditor; }

	const OutlineEditor &
	getOutlineEditor () const
	{ return outlineEditor; }

	OutlineTreeViewEditor &
	getOutlineTreeView ()
	{ return outlineEditor .getTreeView (); }

	const OutlineTreeViewEditor &
	getOutlineTreeView () const
	{ return outlineEditor .getTreeView (); }

	/// @name Member access

	const X3D::Keys &
	getKeys () const
	{ return keys; }

	/// @name Menu

	void
	enableMenus (bool) const;


private:

	/// @name Construction

	virtual
	void
	initialize ();

	std::string
	getStyles () const;

	///  @name Selection handling

	void
	set_selection (const X3D::MFNode & children);
	
	///  @name Key events

	virtual
	bool
	on_key_press_event (GdkEventKey*) final;

	virtual
	bool
	on_key_release_event (GdkEventKey*) final;

	/// @name File menu

	virtual
	void
	on_new () final;

	virtual
	void
	on_open () final;

	virtual
	void
	on_open_location () final;

	virtual
	void
	on_toolbar_drag_data_received (const Glib::RefPtr <Gdk::DragContext>&, int, int, const Gtk::SelectionData &, guint info, guint) final;

	virtual
	void
	on_import () final;

	virtual
	void
	on_surface_box_drag_data_received (const Glib::RefPtr <Gdk::DragContext>&, int, int, const Gtk::SelectionData &, guint info, guint) final;

	void
	dragDataHandling (const Glib::RefPtr <Gdk::DragContext>&, const Gtk::SelectionData &, guint, bool);

	virtual
	void
	on_save () final;

	virtual
	void
	on_save_as () final;

	virtual
	void
	on_revert_to_saved () final;

	virtual
	void
	on_close () final;

	/// @name Dialog response

	virtual
	void
	on_open_location_entry_changed () final;

	virtual
	bool
	on_open_location_entry_key_release_event (GdkEventKey*) final;

	virtual
	void
	on_undo () final;

	virtual
	void
	on_redo () final;

	///  @name Clipboard handling

	void
	on_clipboard_owner_change (GdkEventOwnerChange*);

	virtual
	void
	on_cut_nodes_activate () final;

	virtual
	void
	on_copy_nodes_activate () final;

	virtual
	void
	on_paste_nodes_activate () final;

	///  @name Edit menu 

	virtual
	void
	on_delete_nodes_activate () final;

	virtual
	void
	on_group_selected_nodes_activate () final;

	virtual
	void
	on_ungroup_node_activate () final;

	virtual
	void
	on_add_to_group_activate () final;

	virtual
	void
	on_detach_from_group_activate () final;

	virtual
	void
	on_create_parent_group_activate () final;

	/// @name Bar view handling

	virtual
	void
	on_toolBar_toggled () final;

	virtual
	void
	on_sideBar_toggled () final;

	virtual
	void
	on_footer_toggled () final;

	virtual
	void
	on_browser_toggled () final;

	virtual
	void
	on_editor_toggled () final;

	void
	enableEditor (bool);

	/// @name Shading

	virtual
	void
	phong_activate () final;

	virtual
	void
	gouraud_activate () final;

	virtual
	void
	flat_activate () final;

	virtual
	void
	wireframe_activate () final;

	virtual
	void
	pointset_activate () final;

	///  @name Rendering quality

	virtual
	void
	on_low_quality_activate () final;

	virtual
	void
	on_medium_quality_activate () final;

	virtual
	void
	on_high_quality_activate () final;

	///  @name Rendering properties

	virtual
	void
	on_rendering_properties_toggled () final;

	///  @name Fullscreen modus

	virtual
	void
	on_fullscreen () final;

	virtual
	void
	on_unfullscreen () final;

	///  @name Selection

	virtual
	void
	on_select_all_activate () final;

	virtual
	void
	on_deselect_all_activate () final;

	///  @name Navigation

	virtual
	void
	on_rubberband_toggled () final;

	///  @name Editor handling

	virtual
	void
	on_motion_blur_editor_activate () final;

	///  @name Help menu

	virtual
	void
	on_info () final;

	virtual
	void
	on_standard_size () final;

	/// Toolbar
	
	virtual
	void
	on_node_properties () final;

	///  @name Browser dashboard handling

	void
	set_dashboard (bool);

	virtual
	void
	on_hand_button_toggled () final;

	virtual
	void
	on_arrow_button_toggled () final;

	void
	set_viewer (X3D::ViewerType);

	void
	set_examine_viewer (bool);

	void
	set_walk_viewer (bool);

	void
	set_fly_viewer (bool);

	void
	set_none_viewer (bool);

	virtual
	void
	on_examine_viewer_activate () final;

	virtual
	void
	on_walk_viewer_activate () final;

	virtual
	void
	on_fly_viewer_activate () final;

	virtual
	void
	on_none_viewer_activate () final;

	virtual
	void
	on_look_at_all_clicked () final;

	void
	set_look_at (bool);

	virtual
	void
	on_look_at_toggled () final;

	///  @name Dialog response handling

	void
	on_messageDialog_response (int);

	///  @name Members

	const X3D::X3DSFNode <X3D::Browser> browserSurface;

	MotionBlurEditor motionBlurEditor;
	LibraryView      libraryView;
	ViewpointList    viewpointList;
	HistoryEditor    historyEditor;
	OutlineEditor    outlineEditor;

	X3D::Keys   keys;
	basic::uri  importURL;

};

} // puck
} // titania

#endif
