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

#include "X3DTextureCoordinateGeneratorEditor.h"

namespace titania {
namespace puck {

X3DTextureCoordinateGeneratorEditor::X3DTextureCoordinateGeneratorEditor () :
	X3DGeometryPropertiesEditorInterface ("", ""),
	                       geometryNodes (),
	          textureCoordinateGenerator (),
	                            undoStep (),
	                            changing (false)
{ }

void
X3DTextureCoordinateGeneratorEditor::initialize ()
{
	getBrowser () -> getSelection () -> getChildren () .addInterest (this, &X3DTextureCoordinateGeneratorEditor::set_selection);

	set_selection ();
}

void
X3DTextureCoordinateGeneratorEditor::set_selection ()
{
	for (const auto & geometry : geometryNodes)
	{
		try
		{
			geometry -> getField <X3D::SFFloat> ("texCoord") .removeInterest (this, &X3DTextureCoordinateGeneratorEditor::set_textureCoordinateGenerator);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	undoStep .reset ();

	// Find X3DGeometryNodes

	auto selection = getBrowser () -> getSelection () -> getChildren ();

	geometryNodes .clear ();

	X3D::traverse (selection, [&] (X3D::SFNode & node)
	               {
	                  for (const auto & type: node -> getType ())
	                  {
	                     if (type == X3D::X3DConstants::X3DGeometryNode)
	                     {
	                        geometryNodes .emplace_back (node);
	                        return true;
								}
							}

	                  return true;
						});

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			geometry -> getField <X3D::SFFloat> ("texCoord") .addInterest (this, &X3DTextureCoordinateGeneratorEditor::set_textureCoordinateGenerator);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	set_textureCoordinateGenerator ();
}

/***********************************************************************************************************************
 *
 *  textureCoordinateGenerator
 *
 **********************************************************************************************************************/

void
X3DTextureCoordinateGeneratorEditor::on_textureCoordinateGenerator_unlink_clicked ()
{
	unlinkClone (geometryNodes, "texCoord", undoStep);
}

void
X3DTextureCoordinateGeneratorEditor::on_textureCoordinateGenerator_toggled ()
{
	if (changing)
		return;

	addUndoFunction <X3D::SFNode> (geometryNodes, "texCoord", undoStep);

	getTextureCoordinateGeneratorCheckButton () .set_inconsistent (false);
	getTextureCoordinateGeneratorBox ()         .set_sensitive (getTextureCoordinateGeneratorCheckButton () .get_active ());

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			auto & field = geometry -> getField <X3D::SFNode> ("texCoord");

			field .removeInterest (this, &X3DTextureCoordinateGeneratorEditor::set_textureCoordinateGenerator);
			field .addInterest (this, &X3DTextureCoordinateGeneratorEditor::connectTextureCoordinateGenerator);

			if (getTextureCoordinateGeneratorCheckButton () .get_active ())
				getBrowserWindow () -> replaceNode (X3D::SFNode (geometry), field, X3D::SFNode (textureCoordinateGenerator), undoStep);
			else
				getBrowserWindow () -> replaceNode (X3D::SFNode (geometry), field, nullptr, undoStep);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::SFNode> (geometryNodes, "texCoord", undoStep);

	getTextureCoordinateGeneratorUnlinkButton () .set_sensitive (getTextureCoordinateGeneratorCheckButton () .get_active () and textureCoordinateGenerator -> getCloneCount () > 1);
}

void
X3DTextureCoordinateGeneratorEditor::set_textureCoordinateGenerator ()
{
	if (textureCoordinateGenerator)
		textureCoordinateGenerator -> mode () .removeInterest (this, &X3DTextureCoordinateGeneratorEditor::set_textureCoordinateGenerator_mode);

	textureCoordinateGenerator = nullptr;

	// Find last �texCoord� field.

	bool hasField = false;
	int  active   = -1;

	for (const auto & geometry : basic::reverse_adapter (geometryNodes))
	{
		try
		{
			const X3D::X3DPtr <X3D::TextureCoordinateGenerator> field (geometry -> getField <X3D::SFNode> ("texCoord"));

			if (active < 0)
			{
				textureCoordinateGenerator = std::move (field);
				hasField                   = true;
				active                     = bool (textureCoordinateGenerator);
			}
			else if (field not_eq textureCoordinateGenerator)
			{
				if (not textureCoordinateGenerator)
					textureCoordinateGenerator = std::move (field);

				active = -1;
				break;
			}
		}
		catch (const X3D::X3DError &)
		{ }
	}

	if (not textureCoordinateGenerator)
	{
		textureCoordinateGenerator = new X3D::TextureCoordinateGenerator (getExecutionContext ());
		textureCoordinateGenerator -> setup ();
	}

	changing = true;

	getTextureCoordinateGeneratorMainBox ()     .set_sensitive (hasField);
	getTextureCoordinateGeneratorCheckButton () .set_active (active > 0);
	getTextureCoordinateGeneratorCheckButton () .set_inconsistent (active < 0);

	getTextureCoordinateGeneratorUnlinkButton () .set_sensitive (active > 0 and textureCoordinateGenerator -> getCloneCount () > 1);
	getTextureCoordinateGeneratorBox ()          .set_sensitive (active > 0);

	changing = false;

	textureCoordinateGenerator -> mode () .addInterest (this, &X3DTextureCoordinateGeneratorEditor::set_textureCoordinateGenerator_mode);

	set_textureCoordinateGenerator_mode ();
}

void
X3DTextureCoordinateGeneratorEditor::connectTextureCoordinateGenerator (const X3D::SFNode & field)
{
	field .removeInterest (this, &X3DTextureCoordinateGeneratorEditor::connectTextureCoordinateGenerator);
	field .addInterest (this, &X3DTextureCoordinateGeneratorEditor::set_textureCoordinateGenerator);
}

/***********************************************************************************************************************
 *
 *  mode
 *
 **********************************************************************************************************************/

void
X3DTextureCoordinateGeneratorEditor::on_textureCoordinateGenerator_mode_changed ()
{
	if (changing)
		return;

	addUndoFunction (textureCoordinateGenerator, textureCoordinateGenerator -> mode (), undoStep);

	textureCoordinateGenerator -> mode () .removeInterest (this, &X3DTextureCoordinateGeneratorEditor::set_textureCoordinateGenerator_mode);
	textureCoordinateGenerator -> mode () .addInterest (this, &X3DTextureCoordinateGeneratorEditor::connectTextureCoordinateGeneratorMode);

	textureCoordinateGenerator -> mode () = getTextureCoordinateGeneratorModeButton () .get_active_text ();

	addRedoFunction (textureCoordinateGenerator -> mode (), undoStep);
}

void
X3DTextureCoordinateGeneratorEditor::set_textureCoordinateGenerator_mode ()
{
	changing = true;

	getTextureCoordinateGeneratorModeButton () .set_active_text (textureCoordinateGenerator -> mode ());

	changing = false;
}

void
X3DTextureCoordinateGeneratorEditor::connectTextureCoordinateGeneratorMode (const X3D::SFString & field)
{
	field .removeInterest (this, &X3DTextureCoordinateGeneratorEditor::connectTextureCoordinateGeneratorMode);
	field .addInterest (this, &X3DTextureCoordinateGeneratorEditor::set_textureCoordinateGenerator_mode);
}

} // puck
} // titania
