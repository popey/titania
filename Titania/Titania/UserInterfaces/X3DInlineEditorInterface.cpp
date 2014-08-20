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
#include "X3DInlineEditorInterface.h"

namespace titania {
namespace puck {

const std::string X3DInlineEditorInterface::m_widgetName = "InlineEditor";

void
X3DInlineEditorInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	// Get objects.
	m_BBoxCenterXAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BBoxCenterXAdjustment"));
	m_BBoxCenterYAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BBoxCenterYAdjustment"));
	m_BBoxCenterZAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BBoxCenterZAdjustment"));
	m_BBoxSizeXAdjustment   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BBoxSizeXAdjustment"));
	m_BBoxSizeYAdjustment   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BBoxSizeYAdjustment"));
	m_BBoxSizeZAdjustment   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BBoxSizeZAdjustment"));
	m_URLCellRendererText   = Glib::RefPtr <Gtk::CellRendererText>::cast_dynamic (m_builder -> get_object ("URLCellRendererText"));
	m_URLChooserColumn      = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("URLChooserColumn"));
	m_URLCellrendererPixbuf = Glib::RefPtr <Gtk::CellRendererPixbuf>::cast_dynamic (m_builder -> get_object ("URLCellrendererPixbuf"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_Window -> set_name ("Window");
	m_builder -> get_widget ("Widget", m_Widget);
	m_Widget -> set_name ("Widget");
	m_builder -> get_widget ("IndexButton", m_IndexButton);
	m_IndexButton -> set_name ("IndexButton");
	m_builder -> get_widget ("ConvertMasterSelectionButton", m_ConvertMasterSelectionButton);
	m_ConvertMasterSelectionButton -> set_name ("ConvertMasterSelectionButton");
	m_builder -> get_widget ("UpdateBoundingBoxFieldsSwitch", m_UpdateBoundingBoxFieldsSwitch);
	m_UpdateBoundingBoxFieldsSwitch -> set_name ("UpdateBoundingBoxFieldsSwitch");
	m_builder -> get_widget ("InlineExpander", m_InlineExpander);
	m_InlineExpander -> set_name ("InlineExpander");
	m_builder -> get_widget ("InlineBox", m_InlineBox);
	m_InlineBox -> set_name ("InlineBox");
	m_builder -> get_widget ("LoadCheckButton", m_LoadCheckButton);
	m_LoadCheckButton -> set_name ("LoadCheckButton");
	m_builder -> get_widget ("FoldBackIntoSceneButton", m_FoldBackIntoSceneButton);
	m_FoldBackIntoSceneButton -> set_name ("FoldBackIntoSceneButton");
	m_builder -> get_widget ("BBoxSizeBox", m_BBoxSizeBox);
	m_BBoxSizeBox -> set_name ("BBoxSizeBox");
	m_builder -> get_widget ("BBoxSizeXSpinButton", m_BBoxSizeXSpinButton);
	m_BBoxSizeXSpinButton -> set_name ("BBoxSizeXSpinButton");
	m_builder -> get_widget ("BBoxSizeYSpinButton", m_BBoxSizeYSpinButton);
	m_BBoxSizeYSpinButton -> set_name ("BBoxSizeYSpinButton");
	m_builder -> get_widget ("BBoxSizeZSpinButton", m_BBoxSizeZSpinButton);
	m_BBoxSizeZSpinButton -> set_name ("BBoxSizeZSpinButton");
	m_builder -> get_widget ("BBoxCenterBox", m_BBoxCenterBox);
	m_BBoxCenterBox -> set_name ("BBoxCenterBox");
	m_builder -> get_widget ("BBoxCenterXSpinButton", m_BBoxCenterXSpinButton);
	m_BBoxCenterXSpinButton -> set_name ("BBoxCenterXSpinButton");
	m_builder -> get_widget ("BBoxCenterYSpinButton", m_BBoxCenterYSpinButton);
	m_BBoxCenterYSpinButton -> set_name ("BBoxCenterYSpinButton");
	m_builder -> get_widget ("BBoxCenterZSpinButton", m_BBoxCenterZSpinButton);
	m_BBoxCenterZSpinButton -> set_name ("BBoxCenterZSpinButton");
	m_builder -> get_widget ("LoadStateLabel", m_LoadStateLabel);
	m_LoadStateLabel -> set_name ("LoadStateLabel");
	m_builder -> get_widget ("NameBox", m_NameBox);
	m_NameBox -> set_name ("NameBox");
	m_builder -> get_widget ("NameEntry", m_NameEntry);
	m_NameEntry -> set_name ("NameEntry");
	m_builder -> get_widget ("RenameButton", m_RenameButton);
	m_RenameButton -> set_name ("RenameButton");
	m_builder -> get_widget ("URLBox", m_URLBox);
	m_URLBox -> set_name ("URLBox");
	m_builder -> get_widget ("URLTreeView", m_URLTreeView);
	m_URLTreeView -> set_name ("URLTreeView");
	m_builder -> get_widget ("URLAddButton", m_URLAddButton);
	m_URLAddButton -> set_name ("URLAddButton");
	m_builder -> get_widget ("URLRemoveButton", m_URLRemoveButton);
	m_URLRemoveButton -> set_name ("URLRemoveButton");
	m_IndexButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DInlineEditorInterface::on_index_clicked));
	m_ConvertMasterSelectionButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DInlineEditorInterface::on_convert_master_selection_clicked));
	m_FoldBackIntoSceneButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DInlineEditorInterface::on_fold_back_into_scene_clicked));

	// Call construct handler of base class.
	construct ();
}

X3DInlineEditorInterface::~X3DInlineEditorInterface ()
{
	delete m_Window;
}

} // puck
} // titania