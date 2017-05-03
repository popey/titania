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

#include "Inline.h"

#include "../../Browser/X3DBrowser.h"
#include "../../InputOutput/Loader.h"
#include "../../Thread/SceneFuture.h"

namespace titania {
namespace X3D {

static constexpr bool X3D_PARALLEL = true;

const ComponentType Inline::component      = ComponentType::NETWORKING;
const std::string   Inline::typeName       = "Inline";
const std::string   Inline::containerField = "children";

Inline::Fields::Fields () :
	load (new SFBool (true))
{ }

Inline::Inline (X3DExecutionContext* const executionContext) :
	     X3DBaseNode (executionContext -> getBrowser (), executionContext),
	    X3DChildNode (),
	X3DBoundedObject (),
	    X3DUrlObject (),
	          fields (),
	           scene (),
	           group (new Group (executionContext)),
	          buffer (),
	     preventLoad (false),
	          future ()
{
	addType (X3DConstants::Inline);

	addField (inputOutput,    "metadata",   metadata ());
	addField (inputOutput,    "load",       load ());
	addField (inputOutput,    "url",        url ());
	addField (initializeOnly, "bboxSize",   bboxSize ());
	addField (initializeOnly, "bboxCenter", bboxCenter ());

	addChildObjects (scene, group, buffer, future);
}

X3DBaseNode*
Inline::create (X3DExecutionContext* const executionContext) const
{
	return new Inline (executionContext);
}

void
Inline::initialize ()
{
	X3DChildNode::initialize ();
	X3DBoundedObject::initialize ();
	X3DUrlObject::initialize ();

	getExecutionContext () -> isLive () .addInterest (&Inline::set_live, this);
	isLive () .addInterest (&Inline::set_live, this);

	load () .addInterest (&Inline::set_load, this);
	url ()  .addInterest (&Inline::set_url, this);
	buffer  .addInterest (&Inline::set_buffer, this);

	if (scene)
	{
		setLoadState (COMPLETE_STATE);
		setScene (std::move (scene));
	}
	else
	{
		if (X3D_PARALLEL)
		{
			setScene (X3DScenePtr (getBrowser () -> getPrivateScene ()));

			if (load ())
				buffer .addEvent ();
		}
		else
		{
			if (load ())
				requestImmediateLoad ();

			else
				setScene (X3DScenePtr (getBrowser () -> getPrivateScene ()));
		}
	}

	group -> isCameraObject () .addInterest (&Inline::setCameraObject, static_cast <X3DChildNode*> (this));

	group -> setPrivate (true);
	group -> setup ();
}

void
Inline::setExecutionContext (X3DExecutionContext* const executionContext)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	getExecutionContext () -> isLive () .removeInterest (&Inline::set_live, this);

	if (scene == getExecutionContext () -> getBrowser () -> getPrivateScene ())
		scene = executionContext -> getBrowser () -> getPrivateScene ();
	else if (scene)
		scene -> setExecutionContext (executionContext);

	group -> setExecutionContext (executionContext);

	if (future)
		future -> setExecutionContext (executionContext);

	X3DUrlObject::setExecutionContext (executionContext);
	X3DChildNode::setExecutionContext (executionContext);

	if (isInitialized ())
	{
		getExecutionContext () -> isLive () .addInterest (&Inline::set_live, this);

		set_live ();
	}
}

void
Inline::setSceneAsync (X3DScenePtr && value)
{
	if (value)
	{
		setLoadState (COMPLETE_STATE);
		setScene (std::move (value));
	}
	else
	{
		setLoadState (FAILED_STATE);
		setScene (X3DScenePtr (getBrowser () -> getPrivateScene ()));
	}
}

void
Inline::setScene (X3DScenePtr && value)
{
	if (scene)
		scene -> getRootNodes () .removeInterest (group -> children ());

	// First initialize,

	if (checkLoadState () == COMPLETE_STATE)
	{
		value -> setExecutionContext (getExecutionContext ());
		value -> isLive () = getExecutionContext () -> isLive () and isLive ();
		value -> setPrivate (getExecutionContext () -> getPrivate ());
		value -> getRootNodes () .addInterest (group -> children ());

		if (isInitialized ())
			value -> setup ();

		else
			getExecutionContext () -> addUninitializedNode (value);
	}

	// then assign.

	scene = std::move (value);

	group -> children () = scene -> getRootNodes ();
}

const X3DScenePtr &
Inline::accessScene () const
throw (Error <NODE_NOT_AVAILABLE>,
	    Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (load ())
	{
		if (checkLoadState () == COMPLETE_STATE)
			return scene;

		if (isInitialized ())
		{
			if (X3D_PARALLEL and checkLoadState () == IN_PROGRESS_STATE)
				future -> wait ();

			return scene;
		}
		else
		{
			// Thread save part

			try
			{
				const_cast <Inline*> (this) -> scene .set (getBrowser () -> createScene (false));

				Loader (getExecutionContext ()) .parseIntoScene (scene, url ());

				const_cast <Inline*> (this) -> setLoadState (COMPLETE_STATE);

				return scene;
			}
			catch (const X3DError & error)
			{ }

			// End thread save part
		}
	}

	return scene;
}

Box3d
Inline::getBBox () const
{
	if (bboxSize () == Vector3f (-1, -1, -1))
		return group -> getBBox ();

	return Box3d (bboxSize () .getValue (), bboxCenter () .getValue ());
}

SFNode
Inline::getExportedNode (const std::string & exportedName) const
throw (Error <INVALID_NAME>,
       Error <NODE_NOT_AVAILABLE>,
	    Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	return accessScene () -> getExportedNode (exportedName);
}

const ExportedNodeIndex &
Inline::getExportedNodes () const
throw (Error <NODE_NOT_AVAILABLE>,
	    Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	return accessScene () -> getExportedNodes ();
}

void
Inline::requestImmediateLoad ()
{
	if (not glXGetCurrentContext ())
		return;

	if (checkLoadState () == COMPLETE_STATE or checkLoadState () == IN_PROGRESS_STATE)
		return;

	if (future)
	{
		future -> wait ();
		return;
	}

	setLoadState (IN_PROGRESS_STATE);

	Loader loader (getExecutionContext ());

	try
	{
		setLoadState (COMPLETE_STATE);
		setScene (loader .createX3DFromURL (url ()));
	}
	catch (const X3DError & error)
	{
		setLoadState (FAILED_STATE);
		setScene (X3DScenePtr (getBrowser () -> getPrivateScene ()));

		for (const auto & string : loader .getUrlError ())
			getBrowser () -> println (string .str ());

		getBrowser () -> println (error .what ());
	}
}

void
Inline::requestAsyncLoad ()
{
	using namespace std::placeholders;

	if (not glXGetCurrentContext ())
		return;

	if (checkLoadState () == COMPLETE_STATE or checkLoadState () == IN_PROGRESS_STATE)
		return;

	setLoadState (IN_PROGRESS_STATE);

	future .setValue (new SceneFuture (getExecutionContext (),
	                                   url (),
	                                   std::bind (&Inline::setSceneAsync, this, _1)));
}

void
Inline::requestUnload ()
{
	if (checkLoadState () == NOT_STARTED_STATE or checkLoadState () == FAILED_STATE)
		return;

	future .setValue (nullptr);

	setLoadState (NOT_STARTED_STATE);

	setScene (X3DScenePtr (getBrowser () -> getPrivateScene ()));
}

void
Inline::set_live ()
{
	if (checkLoadState () == COMPLETE_STATE)
	{
		const bool value = getExecutionContext () -> isLive () and isLive ();
		
		if (value not_eq scene -> isLive ())
			scene -> isLive () = value;
	}
}

void
Inline::set_load ()
{
	if (load ())
		buffer .addEvent ();

	else
		requestUnload ();
}

void
Inline::set_url ()
{
	buffer .addEvent ();
}

void
Inline::set_buffer ()
{
	if (not load ())
		return;

	if (preventLoad)
	{
		preventLoad = false;
		return;
	}

	setLoadState (NOT_STARTED_STATE);

	if (X3D_PARALLEL)
		requestAsyncLoad ();

	else
		requestImmediateLoad ();
}

void
Inline::traverse (const TraverseType type, X3DRenderObject* const renderObject)
{
	switch (type)
	{
		case TraverseType::POINTER:
		{
			getBrowser () -> getHierarchy () .emplace_back (this);

			group -> traverse (type, renderObject);

			getBrowser () -> getHierarchy () .pop_back ();
			break;
		}
		default:
		{
			group -> traverse (type, renderObject);
			break;
		}
	}
}

void
Inline::dispose ()
{
	X3DUrlObject::dispose ();
	X3DBoundedObject::dispose ();
	X3DChildNode::dispose ();
}

Inline::~Inline ()
{ }

} // X3D
} // titania
