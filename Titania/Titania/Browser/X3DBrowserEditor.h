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

#ifndef __TITANIA_BROWSER_X3DBROWSER_EDITOR_H__
#define __TITANIA_BROWSER_X3DBROWSER_EDITOR_H__

#include "../Browser/X3DBrowserWidget.h"
#include "../Undo/UndoHistory.h"

namespace titania {
namespace puck {

class X3DBrowserEditor :
	public X3DBrowserWidget
{
public:

	/// @name Tests

	bool
	isSaved ();

	/// @name Member access

	void
	setEdited (bool);

	bool
	getEdited () const
	{ return edited; }

	/// @name File operations

	void
	import (const basic::uri &);

	void
	import (const X3D::X3DSFNode <X3D::Scene> &);

	virtual
	void
	save (const basic::uri &, bool) final;

	void
	undo ();
	
	void
	redo ();

	virtual
	bool
	close () final;

	void
	openNodePropertiesEditor (const X3D::SFNode &);


protected:

	/// @name Construction

	X3DBrowserEditor (const basic::uri &);

	virtual
	void
	initialize () override;

	virtual
	void
	restoreSession () final;

	/// @name Undo

	/// @name Clipboard operations

	void
	cutNodes (X3D::MFNode, const UndoStepPtr &);

	void
	copyNodes (X3D::MFNode);

	void
	pasteNodes (const X3D::MFNode &, const UndoStepPtr &);

	void
	updatePasteStatus ();

	bool
	getPasteStatus ();

	/// @name Edit operations

	void
	addNode (const std::string &, const UndoStepPtr &)
	throw (X3D::Error <X3D::INVALID_NAME>);

	void
	removeNodes (const X3D::MFNode &, const UndoStepPtr &);

	X3D::SFNode
	groupNodes (const X3D::MFNode &, const UndoStepPtr &);

	X3D::MFNode
	ungroupNodes (const X3D::MFNode &, const UndoStepPtr &);

	void
	addToGroup (const X3D::SFNode &, const X3D::MFNode &, const UndoStepPtr &);

	void
	detachFromGroup (X3D::MFNode, bool, const UndoStepPtr &);

	X3D::MFNode
	createParentGroup (const X3D::MFNode &, const UndoStepPtr &);

	/// @name Selection operations

	void
	select (const X3D::MFNode &, const UndoStepPtr &);

	void
	deselect (const X3D::MFNode &, const UndoStepPtr &);

	void
	selectAll (const UndoStepPtr &);

	void
	deselectAll (const UndoStepPtr &);


private:

	void
	set_initialized ();

	void
	set_shutdown ();
	
	void
	set_selection_active (bool);

	// Clipboard

	std::string
	toString (X3D::MFNode &) const;

	// Edit

	void
	removeNode (const X3D::X3DSFNode <X3D::Scene> &, X3D::SFNode, const UndoStepPtr &);

	void
	removeExportedNodes (const X3D::X3DSFNode <X3D::Scene> &, const X3D::SFNode &);

	void
	removeNode (X3D::X3DExecutionContext* const, const X3D::SFNode &, const UndoStepPtr &);

	void
	removeNodeFromSceneGraph (X3D::X3DExecutionContext* const, const X3D::SFNode &, const UndoStepPtr &);

	void
	removeNode (const X3D::SFNode &, X3D::MFNode &, const X3D::SFNode &, const UndoStepPtr &);

	void
	removeNamedNode (X3D::X3DExecutionContext* const, const X3D::SFNode &);

	void
	removeImportedNodes (X3D::X3DExecutionContext* const, const X3D::SFNode &);

	void
	deleteRoutes (X3D::X3DExecutionContext* const, const X3D::SFNode &);

	X3D::Matrix4f
	findModelViewMatrix (X3D::X3DBaseNode* const) const;

	bool
	findModelViewMatrix (X3D::X3DBaseNode* const, X3D::Matrix4f &, std::set <X3D::X3DBaseNode*> &) const;

	std::deque <X3D::X3DBaseNode*>
	getParentNodes (X3D::X3DBaseNode* const) const;

	X3D::X3DFieldDefinition*
	getContainerField (const X3D::SFNode &, const X3D::SFNode &) const
	throw (X3D::Error <X3D::INVALID_NODE>);

	X3D::MFNode*
	getGroupingField (const X3D::SFNode & node) const
	throw (X3D::Error <X3D::INVALID_NODE>);

	///  @name Undo functions

	void
	undoInsertNode (const X3D::SFNode &, X3D::MFNode &, size_t, const X3D::SFNode &);

	void
	undoEraseNode (const X3D::SFNode &, X3D::MFNode &, const X3D::SFNode &, const std::vector <size_t> &);

	void
	undoSetValue (const X3D::SFNode &, X3D::SFNode &, const X3D::SFNode &);

	void
	undoSetMatrix (const X3D::X3DSFNode <X3D::X3DTransformNode> &, const X3D::Matrix4f &);

	void
	undoSetMatrixWithCenter (const X3D::X3DSFNode <X3D::X3DTransformNode> &, const X3D::Matrix4f &, const X3D::Vector3f &);

	///  @name Members

	bool edited;
	bool saveConfirmed;

	UndoHistory undoHistory;
	std::map <X3D::TransformHandle*, std::pair <X3D::Matrix4f, X3D::Vector3f>> matrices;

	X3D::X3DSFNode <X3D::Scene> scene;

};

} // puck
} // titania

#endif
