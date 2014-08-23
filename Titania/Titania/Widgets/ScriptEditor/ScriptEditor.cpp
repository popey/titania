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

#include "ScriptEditor.h"

#include "../../Browser/BrowserWindow.h"
#include "../../Configuration/config.h"
#include "../../Dialogs/NodeIndex/NodeIndex.h"

#include <Titania/String.h>
#include <gtksourceviewmm/init.h>
#include <gtksourceviewmm/languagemanager.h>
#include <gtksourceviewmm/styleschememanager.h>

namespace titania {
namespace puck {

ScriptEditor::ScriptEditor (BrowserWindow* const browserWindow) :
	        X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	X3DScriptEditorInterface (get_ui ("ScriptEditor.xml"), gconf_dir ()),
	              textBuffer (Gsv::Buffer::create ()),
	                textView (textBuffer),
	               nodeIndex (new NodeIndex (browserWindow)),
	                nodeName (getBrowserWindow (), getNameEntry (), getRenameButton ()),
	                    node ()
{
	Gsv::init ();
	setup ();
}

void
ScriptEditor::initialize ()
{
	X3DScriptEditorInterface::initialize ();

	getTextBuffer () -> get_undo_manager () -> signal_can_undo_changed () .connect (sigc::mem_fun (*this, &ScriptEditor::on_can_undo_changed));
	getTextBuffer () -> get_undo_manager () -> signal_can_redo_changed () .connect (sigc::mem_fun (*this, &ScriptEditor::on_can_redo_changed));

	getTextBuffer () -> set_highlight_syntax (true);
	getTextBuffer () -> set_highlight_matching_brackets (true);
	getTextBuffer () -> set_style_scheme (Gsv::StyleSchemeManager::get_default () -> get_scheme ("tango"));

	getTextView () .signal_focus_in_event ()  .connect (sigc::mem_fun (*this, &ScriptEditor::on_focus_in_event));
	getTextView () .signal_focus_out_event () .connect (sigc::mem_fun (*this, &ScriptEditor::on_focus_out_event));

	setTabs (3);
	getTextView () .set_show_right_margin (true);
	getTextView () .set_right_margin_position (100);
	getTextView () .set_highlight_current_line (true);
	getTextView () .set_show_line_numbers (true);
	getTextView () .set_show_line_marks (true);
	getTextView () .set_auto_indent (true);
	getTextView () .set_insert_spaces_instead_of_tabs (false);
	getTextView () .set_indent_on_tab (true);
	getTextView () .show ();

	getScrolledWindow () .add (getTextView ());

	nodeIndex -> getNode () .addInterest (this, &ScriptEditor::set_node);
	nodeIndex -> getHeaderBox () .set_visible (false);
	nodeIndex -> getFooterBox () .set_visible (false);
	nodeIndex -> getScrolledWindow () .set_size_request (0, 0);
	nodeIndex -> reparent (getNodeIndexBox (), getWindow ());

	nodeIndex -> setTypes ({
	                          X3D::X3DConstants::Script,
	                          X3D::X3DConstants::ShaderPart,
	                          X3D::X3DConstants::ShaderProgram
								  });
}

void
ScriptEditor::on_map ()
{
	getBrowserWindow () -> getFooterLabel () .set_text (_ ("Script Editor"));
}

void
ScriptEditor::set_node (const X3D::SFNode & value)
{
	if (node)
	{
		const auto cdata = node -> getCDATA ();

		cdata -> removeInterest (this, &ScriptEditor::set_cdata);
		
		// Load state

		X3D::X3DPtr <X3D::X3DUrlObject> urlObject (node);
		
		if (urlObject)
			urlObject -> checkLoadState () .removeInterest (this, &ScriptEditor::set_loadState);
	}

	node = value;

	nodeName .setNode (node);

	if (node)
	{
		getBrowserWindow () -> getFooterLabel () .set_text (basic::sprintf (_ ("Script Editor »%s«"), node -> getName () .c_str ()));

		const auto cdata = node -> getCDATA ();

		cdata -> addInterest (this, &ScriptEditor::set_cdata);

		getScriptEditorBox () .set_sensitive (true);
		set_cdata ();
	
		// Load state

		X3D::X3DPtr <X3D::X3DUrlObject> urlObject (node);
		
		if (urlObject)
		{
			urlObject -> checkLoadState () .addInterest (this, &ScriptEditor::set_loadState);
			set_loadState (urlObject -> checkLoadState ());
		}
	}
	else
	{
		on_map ();
		getScriptEditorBox () .set_sensitive (false);

		getTextBuffer () -> begin_not_undoable_action ();
		getTextBuffer () -> set_text ("");
		getTextBuffer () -> end_not_undoable_action ();
		
		set_loadState (X3D::NOT_STARTED_STATE);
	}
}

void
ScriptEditor::setTabs (const size_t width)
{
   const auto layout     = getTextView () .create_pango_layout ("A");
   int        charWidth  = 0;
   int        charHeight = 0;

   layout -> get_pixel_size (charWidth, charHeight);   

	Pango::TabArray tabs (1, true);
	tabs .set_tab (0, Pango::TAB_LEFT, width * charWidth);

	getTextView () .set_tabs (tabs);
	getTextView () .set_indent_width (width);
}

bool
ScriptEditor::on_focus_in_event (GdkEventFocus*)
{
	getBrowserWindow () -> hasShortcuts (false);
	return false;
}

bool
ScriptEditor::on_focus_out_event (GdkEventFocus*)
{
	getBrowserWindow () -> hasShortcuts (true);
	return false;
}

void
ScriptEditor::on_save_clicked ()
{
	if (not node)
		return;

	const auto cdata = node -> getCDATA ();
	const auto text  = getTextBuffer () -> get_text ();

	if (text == cdata -> get1Value (0))
		return;

	cdata -> removeInterest (this, &ScriptEditor::set_cdata);
	cdata -> addInterest (this, &ScriptEditor::connectCDATA);

	const auto undoStep = std::make_shared <UndoStep> (basic::sprintf (_ ("Edit Field »%s«"), cdata -> getName () .c_str ()));

	undoStep -> addObjects (node);

	undoStep -> addUndoFunction (&X3D::MFString::setValue, cdata, *cdata);
	cdata -> set1Value (0, text);
	undoStep -> addRedoFunction (&X3D::MFString::setValue, cdata, *cdata);

	getBrowserWindow () -> addUndoStep (undoStep);
	getBrowser () -> println (X3D::SFTime (chrono::now ()) .toUTCString (), ": ", basic::sprintf (_ ("Script »%s« saved."), node -> getName () .c_str ()));
	return;
}

void
ScriptEditor::on_undo_clicked ()
{
	getTextBuffer () -> undo ();
}

void
ScriptEditor::on_redo_clicked ()
{
	getTextBuffer () -> redo ();
}

void
ScriptEditor::on_can_undo_changed ()
{
	getUndoButton () .set_sensitive (getTextBuffer () -> can_undo ());
}

void
ScriptEditor::on_can_redo_changed ()
{
	getRedoButton () .set_sensitive (getTextBuffer () -> can_redo ());
}

void
ScriptEditor::set_cdata ()
{
	getTextBuffer () -> begin_not_undoable_action ();

	const auto cdata = node -> getCDATA ();

	for (const auto & type : basic::make_reverse_range (node -> getType ()))
	{
		switch (type)
		{
			case X3D::X3DConstants::Script:
			{
				getTextBuffer () -> set_language (Gsv::LanguageManager::get_default () -> guess_language ("", "application/javascript"));

				if (cdata -> get1Value (0) .empty ())
					getTextBuffer () -> set_text ("ecmascript:\n");
				else
					getTextBuffer () -> set_text (cdata -> get1Value (0));
				break;
			}
			case X3D::X3DConstants::ShaderPart:
			case X3D::X3DConstants::ShaderProgram:
			{
				getTextBuffer () -> set_language (Gsv::LanguageManager::get_default () -> guess_language ("", "text/x-c"));

				if (cdata -> get1Value (0) .empty ())
					getTextBuffer () -> set_text ("data:text/plain,\n");
				else
					getTextBuffer () -> set_text (cdata -> get1Value (0));
				break;
			}
			default:
				continue;
		}

		break;
	}

	getTextBuffer () -> end_not_undoable_action ();
}

void
ScriptEditor::connectCDATA (const X3D::MFString & field)
{
	field .removeInterest (this, &ScriptEditor::connectCDATA);
	field .addInterest (this, &ScriptEditor::set_cdata);
}

void
ScriptEditor::on_loadState_clicked ()
{
	getBrowserWindow () -> getFooterNotebook () .set_current_page (0);
}

void
ScriptEditor::set_loadState (const X3D::LoadState loadState)
{
	switch (loadState)
	{
		case X3D::NOT_STARTED_STATE:
		{
			getLoadStateButton () .set_stock_id (Gtk::StockID ("gtk-stop"));
			break;
		}
		case X3D::IN_PROGRESS_STATE:
		{
			getLoadStateButton () .set_stock_id (Gtk::StockID ("gtk-refresh"));
			break;
		}
		case X3D::COMPLETE_STATE:
		{
			getLoadStateButton () .set_stock_id (Gtk::StockID ("gtk-ok"));
			break;
		}
		case X3D::FAILED_STATE:
		{
			getLoadStateButton () .set_stock_id (Gtk::StockID ("gtk-no"));
			break;
		}
	}
}

ScriptEditor::~ScriptEditor ()
{
	dispose ();
}

} // puck
} // titania