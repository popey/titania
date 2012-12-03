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
 ******************************************************************************/

#include "Script.h"

#include "../../Browser/Browser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Parser/RegEx.h"

namespace titania {
namespace X3D {

Script::Script (X3DExecutionContext* const executionContext) :
	 X3DBasicNode (executionContext -> getBrowser (), executionContext), 
	X3DScriptNode (),                                                    
	 directOutput (),                                                    // SFBool [ ]directOutput  FALSE
	 mustEvaluate (),                                                    // SFBool [ ]mustEvaluate  FALSE
	   javaScript (NULL)                                                 
{
	setComponent ("Scripting");
	setTypeName ("Script");

	appendField (inputOutput,    "metadata",     metadata);
	appendField (inputOutput,    "url",          url);
	appendField (initializeOnly, "directOutput", directOutput);
	appendField (initializeOnly, "mustEvaluate", mustEvaluate);
}

X3DBasicNode*
Script::create (X3DExecutionContext* const executionContext) const
{
	return new Script (executionContext);
}

void
Script::initialize ()
{
	X3DScriptNode::initialize ();

	for (const auto & URL : url)
	{
		std::string ecmascript;

		if (RegEx::ECMAScript .FullMatch (URL .str (), &ecmascript))
		{
			javaScript = new JavaScript (this, ecmascript);
			break;
		}
	}

	if (javaScript)
		javaScript -> initialize ();
}

void
Script::prepareEvents ()
{
	X3DScriptNode::prepareEvents ();

	if (javaScript)
		javaScript -> prepareEvents ();
}

void
Script::eventsProcessed ()
{
	X3DScriptNode::eventsProcessed ();

	if (javaScript)
		javaScript -> eventsProcessed ();
}

void
Script::dispose ()
{
	if (javaScript)
	{
		javaScript -> shutdown ();
		delete javaScript;
	}

	X3DScriptNode::dispose ();
}

} // X3D
} // titania
