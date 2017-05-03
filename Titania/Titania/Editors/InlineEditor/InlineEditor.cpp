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

#include "InlineEditor.h"

#include "../../Browser/BrowserSelection.h"
#include "../../Configuration/config.h"
#include "../../Dialogs/FileSaveDialog/FileSaveDialog.h"
#include "../../Dialogs/NodeIndex/NodeIndex.h"
#include "../../ComposedWidgets/MFStringURLWidget.h"

#include <Titania/X3D/Components/Grouping/Group.h>
#include <Titania/X3D/Components/Networking/Inline.h>
#include <Titania/X3D/Parser/Filter.h>

namespace titania {
namespace puck {

InlineEditor::InlineEditor (X3DBrowserWindow* const browserWindow) :
	        X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	X3DInlineEditorInterface (get_ui ("Editors/InlineEditor.glade")),
	                nodeName (this, getNameEntry (), getRenameButton ()),
	                    load (this, getLoadCheckButton (),  "load"),
	                     url (new MFStringURLWidget (this,
	                          getURLTreeView (),
	                          getURLCellRendererText (),
	                          getURLAddButton (),
	                          getURLRemoveButton (),
	                          getURLReloadButton (),
	                          getURLChooserColumn (),
	                          "url")),
	              inlineNode (),
	               loadState (false)
{
	addChildObjects (inlineNode);

	getLoadCheckButton () .signal_clicked () .connect (sigc::mem_fun (this, &InlineEditor::on_load_clicked));

	setup ();
}

void
InlineEditor::initialize ()
{
	X3DInlineEditorInterface::initialize ();
}

void
InlineEditor::set_selection (const X3D::MFNode & selection)
{
	X3DInlineEditorInterface::set_selection (selection);

	getConvertMasterSelectionButton () .set_sensitive (not selection .empty ());

	if (inlineNode)
		inlineNode -> checkLoadState () .removeInterest (&InlineEditor::set_loadState, this);

	if (selection .empty ())
		inlineNode = nullptr;
	else
		inlineNode = selection .back ();

	if (inlineNode)
	{
		inlineNode -> checkLoadState () .addInterest (&InlineEditor::set_loadState, this);
	
		loadState = inlineNode -> load ();

		set_loadState (inlineNode -> checkLoadState ());
	}
	else
	{
		loadState = false;
		set_loadState (X3D::NOT_STARTED_STATE);
	}

	const auto nodes = inlineNode ? X3D::MFNode ({ inlineNode }) : X3D::MFNode ();

	getInlineBox () .set_sensitive (inlineNode);

	nodeName   .setNode  (inlineNode);
	load       .setNodes (nodes);
	url ->      setNodes (nodes);

	getRemoveInlineButton () .set_sensitive (inlineNode);
}

void
InlineEditor::on_new_inline_clicked ()
{
	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Create New Inline"));
	getBrowserWindow () -> createNode ("Inline", undoStep);
	getBrowserWindow () -> addUndoStep (undoStep);
}

void
InlineEditor::on_remove_inline_clicked ()
{
	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Remove Inline"));

	getBrowserWindow () -> removeNodesFromScene (getCurrentContext (), { nodeName .getNode () }, true, undoStep);
	getBrowserWindow () -> addUndoStep (undoStep);
}

void
InlineEditor::on_index_clicked ()
{
	const auto nodeIndex = std::dynamic_pointer_cast <NodeIndex> (getBrowserWindow () -> addDialog ("NodeIndex"));
	nodeIndex -> setTypes ({ X3D::X3DConstants::Inline });
}

void
InlineEditor::on_convert_master_selection_clicked ()
{
	if (getBrowserWindow () -> getSelection () -> getNodes () .empty ())
		return;

	getWindow () .set_sensitive (false);

	const auto masterSelection = getBrowserWindow () -> getSelection () -> getNodes () .back ();
	const auto nodes           = X3D::MFNode ({ masterSelection });

	const auto fileSaveDialog = std::dynamic_pointer_cast <FileSaveDialog> (addDialog ("FileSaveDialog", false));
	const auto undoStep       = std::make_shared <X3D::UndoStep> (_ ("Convert Master Selection Into Inline File"));
	basic::uri worldURL;

	if (fileSaveDialog -> exportNodes (nodes, worldURL, undoStep))
	{
		const auto name       = X3D::GetNameFromURI (worldURL);
		const auto inlineNode = getCurrentContext () -> createNode <X3D::Inline> ();
		const auto url        = X3D::MFString ({ getCurrentContext () -> getWorldURL () .relative_path (worldURL) .str (), worldURL .str () });

		inlineNode -> url () = url;

		getBrowserWindow () -> getSelection () -> clearNodes (undoStep);
		X3D::X3DEditor::updateNamedNode (getCurrentContext (), name, inlineNode, undoStep);
		X3D::X3DEditor::replaceNodes (getCurrentContext (), masterSelection, inlineNode, undoStep);

		getBrowserWindow () -> getSelection () -> setNodes ({ inlineNode }, undoStep);
		getBrowserWindow () -> addUndoStep (undoStep);
		getBrowserWindow () -> expandNodes ({ inlineNode });
	}

	getWindow () .set_sensitive (true);
}

void
InlineEditor::on_update_bounding_box_fields_activate ()
{
	__LOG__ << std::endl;
}

void
InlineEditor::on_fold_back_into_scene_clicked ()
{
	const auto undoStep       = std::make_shared <X3D::UndoStep> (_ ("Fold Inline Back Into Scene"));
	const auto scene          = inlineNode -> getInternalScene ();
	const auto group          = X3D::SFNode (new X3D::Group (getCurrentContext ()));
	const auto name           = X3D::GetNameFromURI (scene -> getWorldURL ());
	const auto importedRoutes = X3D::X3DEditor::getImportedRoutes (getCurrentContext (), scene);

	const X3D::X3DPtr <X3D::Group> groupNode (group);

	X3D::X3DEditor::updateNamedNode (getCurrentContext (), name, group, undoStep);
	X3D::X3DEditor::replaceNodes (getCurrentContext (), inlineNode, group, undoStep);
	X3D::X3DEditor::importScene (getCurrentContext (), group, groupNode -> children (), scene, undoStep);
	group -> setup ();

	for (const auto & route : importedRoutes)
		X3D::X3DEditor::addRoute (getCurrentContext (), std::get <0> (route), std::get <1> (route), std::get <2> (route), std::get <3> (route), undoStep);

	getBrowserWindow () -> getSelection () -> setNodes ({ group }, undoStep);
	getBrowserWindow () -> addUndoStep (undoStep);
	getBrowserWindow () -> expandNodes ({ group });
}

void
InlineEditor::on_load_clicked ()
{
	if (not inlineNode)
		return;

	if (inlineNode -> load () == loadState)
		return;

	// Inline is loaded and should be unloaded.  Now create undo step for imported nodes.
	if (loadState and load .getOwnUndoStep ())
		X3D::X3DEditor::removeImportedNodes (X3D::X3DExecutionContextPtr (inlineNode -> getExecutionContext ()), { inlineNode }, load .getOwnUndoStep ());				

	loadState = inlineNode -> load ();
}

void
InlineEditor::set_loadState (const X3D::LoadState loadState)
{
	getFoldBackIntoSceneButton () .set_sensitive (false);

	switch (loadState)
	{
		case X3D::NOT_STARTED_STATE:
		{
			getLoadStateLabel () .set_text (_ ("NOT STARTED"));
			break;
		}
		case X3D::IN_PROGRESS_STATE:
		{
			getLoadStateLabel () .set_text (_ ("IN PROGRESS"));
			break;
		}
		case X3D::COMPLETE_STATE:
		{
			getFoldBackIntoSceneButton () .set_sensitive (true);
			getLoadStateLabel () .set_text (_ ("COMPLETE"));
			break;
		}
		case X3D::FAILED_STATE:
		{
			getLoadStateLabel () .set_text (_ ("FAILED"));
			break;
		}
	}
}

InlineEditor::~InlineEditor ()
{
	dispose ();
}

} // puck
} // titania
