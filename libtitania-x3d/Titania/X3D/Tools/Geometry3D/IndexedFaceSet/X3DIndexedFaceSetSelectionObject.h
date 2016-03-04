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

#ifndef __TITANIA_X3D_TOOLS_GEOMETRY3D_X3DINDEXED_FACE_SET_SELECTION_OBJECT_H__
#define __TITANIA_X3D_TOOLS_GEOMETRY3D_X3DINDEXED_FACE_SET_SELECTION_OBJECT_H__

#include "../../Rendering/X3DComposedGeometryNodeTool.h"

namespace titania {
namespace X3D {

class FaceSelection;
class Switch;
class PlaneSensor;
class TouchSensor;
class CoordinateDouble;
class IndexedLineSet;
class IndexedFaceSet;

class X3DIndexedFaceSetSelectionObject :
	virtual public X3DComposedGeometryNodeTool
{
public:

	///  @name Fields

	virtual
	SFBool &
	convex () = 0;

	virtual
	const SFBool &
	convex () const = 0;

	virtual
	MFInt32 &
	coordIndex () = 0;

	virtual
	const MFInt32 &
	coordIndex () const = 0;

	///  @name Hidden fields

	SFBool &
	select ()
	{ return *fields .select; }

	const SFBool &
	select () const
	{ return *fields .select; }

	SFString &
	selectionType ()
	{ return *fields .selectionType; }

	const SFString &
	selectionType () const
	{ return *fields .selectionType; }

	SFBool &
	paintSelection ()
	{ return *fields .paintSelection; }

	const SFBool &
	paintSelection () const
	{ return *fields .paintSelection; }

	// Selection

	MFInt32 &
	replaceSelection ()
	{ return *fields .replaceSelection; }

	const MFInt32 &
	replaceSelection () const
	{ return *fields .replaceSelection; }

	MFInt32 &
	addSelection ()
	{ return *fields .addSelection; }

	const MFInt32 &
	addSelection () const
	{ return *fields .addSelection; }

	MFInt32 &
	removeSelection ()
	{ return *fields .removeSelection; }

	const MFInt32 &
	removeSelection () const
	{ return *fields .removeSelection; }

	// SelectedEdges

	MFInt32 &
	replaceSelectedEdges ()
	{ return *fields .replaceSelectedEdges; }

	const MFInt32 &
	replaceSelectedEdges () const
	{ return *fields .replaceSelectedEdges; }

	MFInt32 &
	addSelectedEdges ()
	{ return *fields .addSelectedEdges; }

	const MFInt32 &
	addSelectedEdges () const
	{ return *fields .addSelectedEdges; }

	MFInt32 &
	removeSelectedEdges ()
	{ return *fields .removeSelectedEdges; }

	const MFInt32 &
	removeSelectedEdges () const
	{ return *fields .removeSelectedEdges; }

	// SelectedFaces

	MFInt32 &
	replaceSelectedFaces ()
	{ return *fields .replaceSelectedFaces; }

	const MFInt32 &
	replaceSelectedFaces () const
	{ return *fields .replaceSelectedFaces; }

	MFInt32 &
	addSelectedFaces ()
	{ return *fields .addSelectedFaces; }

	const MFInt32 &
	addSelectedFaces () const
	{ return *fields .addSelectedFaces; }

	MFInt32 &
	removeSelectedFaces ()
	{ return *fields .removeSelectedFaces; }

	const MFInt32 &
	removeSelectedFaces () const
	{ return *fields .removeSelectedFaces; }

	// selected pep

	SFInt32 &
	selectedPoints_changed ()
	{ return *fields .selectedPoints_changed; }

	const SFInt32 &
	selectedPoints_changed () const
	{ return *fields .selectedPoints_changed; }

	SFInt32 &
	selectedEdges_changed ()
	{ return *fields .selectedEdges_changed; }

	const SFInt32 &
	selectedEdges_changed () const
	{ return *fields .selectedEdges_changed; }

	SFInt32 &
	selectedHoles_changed ()
	{ return *fields .selectedHoles_changed; }

	const SFInt32 &
	selectedHoles_changed () const
	{ return *fields .selectedHoles_changed; }

	SFInt32 &
	selectedFaces_changed ()
	{ return *fields .selectedFaces_changed; }

	const SFInt32 &
	selectedFaces_changed () const
	{ return *fields .selectedFaces_changed; }

	///  @name Destruction

	virtual
	void
	dispose ()
	{ }

	~X3DIndexedFaceSetSelectionObject ();


protected:

	///  @name Member types

	enum class SelectionType :
	   uint8_t
	{
	   POINTS,
	   EDGES,
	   FACES
	};

	enum class SelectActionType :
	   uint8_t
	{
	   REPLACE,
	   ADD,
	   REMOVE
	};

	using SelectedEdges = std::map <std::pair <int32_t, int32_t>, std::set <std::pair <size_t, size_t>>>;
	using SelectedHoles = std::vector <std::vector <int32_t>>;

	///  @name Construction

	X3DIndexedFaceSetSelectionObject ();

	virtual
	void
	initialize () override;

	///  @name Member access

	const X3DPtr <Switch> &
	getHotSwitch () const
	{ return hotSwitch; }

	const X3DPtr <FaceSelection> &
	getFaceSelection () const
	{ return selection; }

	void
	setHotPoints (const std::vector <int32_t> & value)
	{ hotPoints = value; }

	const std::vector <int32_t> &
	getHotPoints () const
	{ return hotPoints; }

	void
	setHotEdge (const std::vector <size_t> & value)
	{ hotEdge = value; }

	const std::vector <size_t> &
	getHotEdge () const
	{ return hotEdge; }

	void
	setHotFace (size_t value)
	{ hotFace = value; }

	size_t
	getHotFace () const
	{ return hotFace; }

	void
	setHotFaces (const std::set <size_t> & value)
	{ hotFaces = value; }

	const std::set <size_t> &
	getHotFaces () const
	{ return hotFaces; }

	const std::vector <int32_t> &
	getActivePoints () const
	{ return activePoints; }

	const std::vector <size_t> &
	getActiveEdge () const
	{ return activeEdge; }

	size_t
	getActiveFace () const
	{ return activeFace; }

	SelectionType
	getSelectionType () const
	{ return type; }

	int32_t
	getMasterPoint () const
	{ return masterPoint; }

	const std::map <int32_t, Vector3d> &
	getSelectedPoints () const
	{ return selectedPoints; }

	const SelectedEdges &
	getSelectedEdges () const
	{ return selectedEdges; }

	const SelectedHoles &
	getSelectedHoles () const
	{ return selectedHoles; }

	const std::set <size_t> &
	getSelectedFaces () const
	{ return selectedFaces; }

	void
	setTranslate (const bool value)
	{ translate = value; }

	bool
	getTranslate () const
	{ return translate; }

	///  @name Operations

	void
	updateMagicSelection ();

	SelectActionType
	getSelectActionType () const;

	bool
	isInSelection (const std::vector <size_t> &) const;

	double
	getDistance (const Vector3d &, const Vector3d &);

	double
	getArea (const std::vector <size_t> &);

	///  @name Undo/redo handling

	void
	undoRestoreSelection (const UndoStepPtr &);

	void
	redoRestoreSelection (const std::vector <int32_t> &, const UndoStepPtr &);

	void
	redoRestoreSelectedEdges (const std::vector <int32_t> &, const UndoStepPtr &);

	void
	redoRestoreSelectedFaces (const std::vector <int32_t> &, const UndoStepPtr &);


private:

	///  @name Event handler

	void
	set_loadState ();

	void
	set_select ();

	void
	set_selectionType ();

	void
	set_replaceSelection_ ();

	void
	set_addSelection_ ();

	void
	set_removeSelection_ ();

	void
	set_replaceSelectedEdges_ ();

	void
	set_addSelectedEdges_ ();

	void
	set_removeSelectedEdges_ ();

	void
	set_replaceSelectedFaces_ ();

	void
	set_addSelectedFaces_ ();

	void
	set_removeSelectedFaces_ ();

	void
	set_coord ();

	void
	set_coord_point ();

	void
	set_touch_sensor_hitPoint ();

	void
	set_touch_sensor_over (const bool);

	void
	set_touch_sensor_active (const bool);

	void
	set_touch_sensor_touchTime ();

	void
	set_selection (const std::vector <Vector3d> &) final override;

	void
	set_plane_sensor_active ();

	bool
	setMagicSelection ();

	void
	updateMagicPoints ();

	void
	updateMagicFace ();

	void
	clearSelection ();

	void
	select (const std::vector <int32_t> &, const SelectActionType);

	void
	selectPoints (const std::vector <int32_t> &, const SelectActionType);

	void
	selectEdges (const std::vector <int32_t> &, const SelectActionType);

	void
	selectEdge (const std::vector <size_t> &, const SelectActionType);

	void
	selectHoles ();

	void
	selectHoles (const std::set <std::pair <int32_t, int32_t>> &,
	             const std::multimap <int32_t, std::pair <int32_t, int32_t>> &,
		          std::vector <std::vector <int32_t>> &) const;

	void
	selectHole (std::set <int32_t> &,
	            const std::multimap <int32_t, std::pair <int32_t, int32_t>> &,
		         const std::pair <int32_t, int32_t> &,
		         std::vector <int32_t> &,
		         std::vector <std::vector <int32_t>> &) const;

	void
	selectFaces (const std::vector <int32_t> &, const SelectActionType);

	void
	selectFace (const size_t, const SelectActionType);

	void
	addSelectedPointsFunction (const std::vector <int32_t> &);

	void
	removeSelectedPointsFunction (const std::vector <int32_t> &);

	void
	updateSelectedPoints ();

	void
	addSelectedEdgesFunction (const std::vector <size_t> &);

	void
	removeSelectedEdgesFunction (const std::vector <size_t> &);

	void
	updateSelectedEdges ();

	void
	addSelectedFacesFunction (const std::set <size_t> &);

	void
	removeSelectedFacesFunction (const std::set <size_t> &);

	void
	updateSelectedFaces ();

	void
	updateGeometries ();

	static
	void
	restoreSelection (const SFNode &, const std::vector <int32_t> &);

	static
	void
	restoreSelectedEdges (const SFNode &, const std::vector <int32_t> &);

	static
	void
	restoreSelectedFaces (const SFNode &, const std::vector <int32_t> &);

	///  @name Members

	struct Fields
	{
		Fields ();

		SFBool* const select;
		SFString* const selectionType;
		SFBool* const paintSelection;
		MFInt32* const replaceSelection;
		MFInt32* const addSelection;
		MFInt32* const removeSelection;
		MFInt32* const replaceSelectedEdges;
		MFInt32* const addSelectedEdges;
		MFInt32* const removeSelectedEdges;
		MFInt32* const replaceSelectedFaces;
		MFInt32* const addSelectedFaces;
		MFInt32* const removeSelectedFaces;
		SFInt32* const selectedPoints_changed;
		SFInt32* const selectedEdges_changed;
		SFInt32* const selectedHoles_changed;
		SFInt32* const selectedFaces_changed;
	};

	Fields fields;

	X3DPtr <TouchSensor>       touchSensor;
	X3DPtr <PlaneSensor>       planeSensor;
	X3DPtr <PlaneSensor>       knifePlaneSensor;
	X3DPtr <Switch>            hotSwitch;
	X3DPtr <CoordinateDouble>  hotPointCoord;
	X3DPtr <IndexedLineSet>    hotEdgesGeometry;
	X3DPtr <CoordinateDouble>  activePointCoord;
	X3DPtr <IndexedLineSet>    activeEdgesGeometry;
	X3DPtr <CoordinateDouble>  selectionCoord;
	X3DPtr <IndexedLineSet>    selectedEdgesGeometry;
	X3DPtr <IndexedFaceSet>    selectedFacesGeometry;
	X3DPtr <X3DCoordinateNode> coordNode;
	X3DPtr <FaceSelection>     selection;

	std::vector <int32_t> hotPoints;    // coord indices
	std::vector <size_t>  hotEdge;      // index of coord indices
	size_t                hotFace;      // index of first coord index of face
	std::set <size_t>     hotFaces;     // adjacent faces of hot point, edge or the face itself 

	std::vector <int32_t> activePoints;   // coord indices
	std::vector <size_t>  activeEdge;     // index of coord indices
	size_t                activeFace;     // index of first coord index of face

	SelectionType                type;
	int32_t                      masterPoint;       // coord index,
	std::map <int32_t, Vector3d> selectedPoints;    // coord index, point
	SelectedEdges                selectedEdges;     // index to vertex of face to coordIndex array
	SelectedHoles                selectedHoles;     // index of coord indices
	std::set <size_t>            selectedFaces;     // index to first vertex of face to coordIndex array

	bool translate;
};

} // X3D
} // titania

#endif