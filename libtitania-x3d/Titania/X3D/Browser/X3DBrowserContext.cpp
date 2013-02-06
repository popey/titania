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

#include "X3DBrowserContext.h"

#include "../Components/Networking/Anchor.h"
#include "../Components/PointingDeviceSensor/X3DTouchSensorNode.h"

#include <Titania/Chrono/CountingClock.h>
#include <Titania/Chrono/SystemClock.h>

namespace titania {
namespace X3D {

X3DBrowserContext::X3DBrowserContext () :
	X3DExecutionContext (),                                        
	           reshaped (),                                        // [out]    reshape
	            exposed (),                                        // [out]    exposed
	          displayed (),                                        // [out]    displayed
	           finished (),                                        // [out]    finished
	            changed (),                                        // [out]    changed
	renderingProperties (new RenderingProperties (this)),          // SFSting  [ ] renderingProperties NULL  [RenderingProperties]
	  browserProperties (new BrowserProperties   (this)),          // SFSting  [ ] browserProperties   NULL  [BrowserProperties]
	     browserOptions (new BrowserOptions      (this)),          // SFSting  [ ] browserOptions      NULL  [BrowserOptions]
	   javaScriptEngine (new JavaScriptEngine    (this)),          // SFSting  [ ] javaScriptEngine    NULL  [JavaScriptEngine]
	              clock (new chrono::system_clock <time_type> ()), 
	             router (),                                        
	             layers (),                                        
	             lights (),                                        
	           textures (),                                        
	            sensors (),                                        
	                  x (0),                                       
	                  y (0),                                       
	     sensitiveNodes (),                                        
	               hits (),                                        
	        changedTime (clock -> cycle ()),                       
	      priorPosition (),                                        
	       currentSpeed (0),                                       
	   currentFrameRate (0)                                        
{
	//supportedFields, // make X3DBaseNodes of this
	//supportedNodes,
	//supportedComponents,
	//supportedProfiles,

	addField (initializeOnly, "renderingProperties", renderingProperties);
	addField (initializeOnly, "browserProperties",   browserProperties);
	addField (initializeOnly, "browserOptions",      browserOptions);
	addField (initializeOnly, "javaScriptEngine",    javaScriptEngine);
}

void
X3DBrowserContext::initialize ()
{
	X3DExecutionContext::initialize ();

	// Initialize clock

	clock -> advance ();

	// Properties

	renderingProperties -> setup ();
	browserProperties   -> setup ();
	browserOptions      -> setup ();
	javaScriptEngine    -> setup ();

	// Lights

	for (int32_t i = 0; i < renderingProperties -> maxLights; ++ i)
		lights .push (GL_LIGHT0 + i);

	// Initialize OpenGL context

	if (glXGetCurrentContext ())
	{
		glClearColor (0, 0, 0, 0);
		glClearDepth (1);

		glColorMaterial (GL_FRONT_AND_BACK, GL_DIFFUSE);
		glCullFace (GL_BACK);
		glEnable (GL_NORMALIZE);

		glDepthFunc (GL_LEQUAL);

		//glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendFuncSeparate (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquationSeparate (GL_FUNC_ADD, GL_FUNC_ADD);

		GLfloat light_model_ambient [ ] = { 0, 0, 0, 1 };

		glLightModelfv (GL_LIGHT_MODEL_AMBIENT,       light_model_ambient);
		glLightModeli  (GL_LIGHT_MODEL_LOCAL_VIEWER,  GL_FALSE);
		glLightModeli  (GL_LIGHT_MODEL_TWO_SIDE,      GL_TRUE);
		glLightModeli  (GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

		glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glHint (GL_GENERATE_MIPMAP_HINT,        GL_NICEST);
		glHint (GL_FOG_HINT,                    GL_NICEST);

		//	glHint (GL_POINT_SMOOTH_HINT,   GL_NICEST);
		//	glHint (GL_LINE_SMOOTH_HINT,    GL_NICEST);
		//	glHint (GL_POLYGON_SMOOTH_HINT, GL_NICEST);
		//
		//	glEnable (GL_POINT_SMOOTH);
		//	glEnable (GL_LINE_SMOOTH);
		//	glEnable (GL_POLYGON_SMOOTH);
	}

	initialized .addInterest (this, &X3DBrowserContext::set_initialized);
}

void
X3DBrowserContext::set_initialized ()
{
	clearHits ();

	overSensors   .clear ();
	activeSensors .clear ();
}

time_type
X3DBrowserContext::getCurrentTime () const
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	return clock -> cycle ();
}

double
X3DBrowserContext::getCurrentSpeed () const
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	return currentSpeed;
}

double
X3DBrowserContext::getCurrentFrameRate () const
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	return currentFrameRate;
}

// Event handling

Router &
X3DBrowserContext::getRouter ()
{
	return router;
}

// NavigationInfo handling

NavigationInfo*
X3DBrowserContext::getActiveNavigationInfo () const
{
	return getExecutionContext () -> getActiveLayer () -> getNavigationInfo ();
}

// Viewpoint handling

X3DViewpointNode*
X3DBrowserContext::getActiveViewpoint ()
{
	return getExecutionContext () -> getActiveLayer () -> getViewpoint ();
}

/// @name Sensors

void
X3DBrowserContext::addSensor (X3DSensorNode* const sensor)
{
	std::clog << "\tAdding sensor " << sensor -> getTypeName () << " to world: " << sensors .getRequesters () .size () << " are registered until now." << std::endl;
	sensors .addInterest (sensor, &X3DSensorNode::update);
	std::clog << "\tNow are " << sensors .getRequesters () .size () << " registered." << std::endl;
}

void
X3DBrowserContext::removeSensor (X3DSensorNode* const sensor)
{
	std::clog << "\tRemoving sensor " << sensor -> getTypeName () << " from world: " << sensors .getRequesters () .size () << " are registered until now." << std::endl;
	sensors .removeInterest (sensor, &X3DSensorNode::update);
	std::clog << "\tNow are " << sensors .getRequesters () .size () << " registered." << std::endl;
}

void
X3DBrowserContext::updateSensors ()
{
	sensors .processInterests ();
}

// Selection

void
X3DBrowserContext::pick (const double _x, const double _y)
{
	x = _x;
	y = _y;

	// Clear hits.

	clearHits ();

	// Pick.

	pick ();

	// Selection end.

	sensitiveNodes .clear ();

	std::sort (hits .begin (), hits .end (), hitComp);
}

Line3f
X3DBrowserContext::getHitRay () const
{
	GLint                viewport [4]; // x, y, width, heigth
	Matrix4d::array_type modelview, projection;

	glGetIntegerv (GL_VIEWPORT, viewport);
	glGetDoublev (GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev (GL_PROJECTION_MATRIX, projection);

	GLdouble px, py, pz;

	// Near plane point
	gluUnProject (x, y, 0, modelview, projection, viewport, &px, &py, &pz);
	Vector3f near (px, py, pz);

	// Far plane point
	gluUnProject (x, y, 1, modelview, projection, viewport, &px, &py, &pz);
	Vector3f far (px, py, pz);

	return Line3f (near, far);
}

void
X3DBrowserContext::addHit (const Line3f & hitRay, const Vector3f hitPoint)
{
	hits .emplace_back (new Hit (hitPoint, hitRay, getSensitiveNodes ()));
}

void
X3DBrowserContext::clearHits ()
{
	for (const auto & hit : hits)
		delete hit;

	hits .clear ();
}

void
X3DBrowserContext::motionNotifyEvent ()
{
	// Set isOver to FALSE for appropriate nodes

	std::deque <X3DBaseNode*> difference;

	if (getHits () .size ())
	{
		std::set_difference (overSensors .begin (), overSensors .end (),
		                     getHits () .front () -> nodes .begin (), getHits () .front () -> nodes .end (),
		                     std::inserter (difference, difference .begin ()));
	}
	else
		difference = overSensors;

	for (const auto & node : difference)
	{
		X3DPointingDeviceSensorNode* pointingDeviceSensorNode = dynamic_cast <X3DPointingDeviceSensorNode*> (node);

		if (pointingDeviceSensorNode)
			pointingDeviceSensorNode -> over (false);
	}

	// Set isOver to TRUE for appropriate nodes

	if (getHits () .size ())
	{
		overSensors = getHits () .front () -> nodes;

		for (const auto & node : overSensors)
		{
			X3DPointingDeviceSensorNode* pointingDeviceSensorNode = dynamic_cast <X3DPointingDeviceSensorNode*> (node);

			if (pointingDeviceSensorNode)
				pointingDeviceSensorNode -> over (true);
		}
	}
	else
		overSensors .clear ();
}

void
X3DBrowserContext::buttonPressEvent ()
{
	activeSensors = getHits () .front () -> nodes;

	for (const auto & node : activeSensors)
	{
		X3DPointingDeviceSensorNode* pointingDeviceSensorNode = dynamic_cast <X3DPointingDeviceSensorNode*> (node);

		if (pointingDeviceSensorNode)
			pointingDeviceSensorNode -> activate (true);
	}
}

void
X3DBrowserContext::buttonReleaseEvent ()
{
	for (const auto & node : activeSensors)
	{
		X3DPointingDeviceSensorNode* pointingDeviceSensorNode = dynamic_cast <X3DPointingDeviceSensorNode*> (node);

		if (pointingDeviceSensorNode)
			pointingDeviceSensorNode -> activate (false);
	}
}

void
X3DBrowserContext::touchEvent ()
{
	for (const auto & node : getHits () .front () -> nodes)
	{
		Anchor* anchor = dynamic_cast <Anchor*> (node);

		if (anchor)
		{
			anchor -> requestImmediateLoad ();
			break;
		}

		X3DTouchSensorNode* touchSensorNode = dynamic_cast <X3DTouchSensorNode*> (node);

		if (touchSensorNode)
			touchSensorNode -> update ();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void
X3DBrowserContext::registerEvent (X3DBaseNode* const node)
{
	router .registerEvent (node);

	if (changedTime == getCurrentTime ())
		return;

	changedTime = getCurrentTime ();
	changed .processInterests ();
}

void
X3DBrowserContext::pick ()
{
	getExecutionContext () -> pick ();
}

void
X3DBrowserContext::prepare ()
{
	clock -> advance ();
	exposed .processInterests ();

	for (const auto & layer : getExecutionContext () -> getLayerSet () -> getLayers ())
		layer -> getViewpoint () -> update ();
	
	currentFrameRate = 1 / clock -> interval ();

	Vector3d position = getActiveViewpoint () -> getTransformationMatrix () .translation ();
	currentSpeed  = abs (position - priorPosition) * currentFrameRate;
	priorPosition = position;

	updateSensors ();

	router .processEvents ();
	getGarbageCollector () .dispose ();
}

void
X3DBrowserContext::display ()
{
	glClearColor (0, 0, 0, 0);
	glClear (GL_COLOR_BUFFER_BIT);

	getExecutionContext () -> display ();

	glColorMask (FALSE, FALSE, FALSE, TRUE);
	glClearColor (0, 0, 0, 1);
	glClear (GL_COLOR_BUFFER_BIT);

	glColorMask (TRUE, TRUE, TRUE, TRUE);

	displayed .processInterests ();
}

void
X3DBrowserContext::finish ()
{
	finished .processInterests ();

	GLenum errorNum = glGetError ();

	if (errorNum not_eq GL_NO_ERROR)
		std::clog << "OpenGL Error at " << SFTime (getCurrentTime ()) .toLocaleString () << ": " << gluErrorString (errorNum) << std::endl;
}

void
X3DBrowserContext::dispose ()
{
	initialized .dispose ();
	reshaped    .dispose ();
	exposed     .dispose ();
	displayed   .dispose ();
	finished    .dispose ();
	shutdown    .dispose ();
	changed     .dispose ();

	X3DChildObject::dispose ();
	//X3DExecutionContext::dispose ();

	getGarbageCollector () .dispose ();
}

} // X3D
} // titania
