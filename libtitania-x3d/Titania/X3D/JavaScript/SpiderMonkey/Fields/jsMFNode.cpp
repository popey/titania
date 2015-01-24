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

#include "jsMFNode.h"

namespace titania {
namespace X3D {
namespace MozillaSpiderMonkey {

template <>
JSClass jsX3DArrayField <jsSFNode, X3D::MFNode>::static_class = {
	"MFNode", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, get1Value, set1Value,
	(JSEnumerateOp) enumerate, JS_ResolveStub, JS_ConvertStub, finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <>
JSBool
jsX3DArrayField <jsSFNode, X3D::MFNode>::construct (JSContext* cx, uint32_t argc, jsval* vp)
{
	try
	{
		if (argc == 0)
		{
			return create (cx, new X3D::MFNode (), &JS_RVAL (cx, vp));
		}
		else
		{
			const auto array = new X3D::MFNode ();
			const auto argv  = JS_ARGV (cx, vp);

			for (uint32_t i = 0; i < argc; ++ i)
			{
				try
				{
					array -> emplace_back (*getArgument <jsSFNode> (cx, argv, i));
				}
				catch (const std::domain_error &)
				{
					array -> emplace_back ();
				}	
			}

			return create (cx, array, &JS_RVAL (cx, vp));
		}
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .new: %s.", getClass () -> name, error .what ());
	}
}

template <>
JSBool
jsX3DArrayField <jsSFNode, X3D::MFNode>::set1Value (JSContext* cx, JSObject* obj, jsid id, JSBool strict, jsval* vp)
{
	try
	{
		if (not JSID_IS_INT (id))
			return true;

		const auto array = getThis <jsX3DArrayField <jsSFNode, X3D::MFNode>> (cx, obj);
		const auto index = JSID_TO_INT (id);

		if (index < 0)
			return ThrowException (cx, "%s: array index out of range.", getClass () -> name);

		try
		{
			array -> set1Value (index, *getArgument <jsSFNode> (cx, vp, 0));
		}
		catch (const std::domain_error &)
		{
			array -> set1Value (index, nullptr);
		}

		*vp = JSVAL_VOID;
		return true;
	}
	catch (const std::bad_alloc &)
	{
		return ThrowException (cx, "%s: out of memory.", getClass () -> name);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .set1Value: %s.", getClass () -> name, error .what ());
	}
}

template <>
JSBool
jsX3DArrayField <jsSFNode, X3D::MFNode>::unshift (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .unshift: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv  = JS_ARGV (cx, vp);
		const auto array = getThis <jsX3DArrayField <jsSFNode, X3D::MFNode>> (cx, vp);
		
		try
		{
			array -> emplace_front (*getArgument <jsSFNode> (cx, argv, 0));
		}
		catch (const std::domain_error &)
		{
			array -> emplace_front (nullptr);
		}

		return JS_NewNumberValue (cx, array -> size (), vp);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .unshift: %s.", getClass () -> name, error .what ());
	}
}

template <>
JSBool
jsX3DArrayField <jsSFNode, X3D::MFNode>::push (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .push: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv  = JS_ARGV (cx, vp);
		const auto array = getThis <jsX3DArrayField <jsSFNode, X3D::MFNode>> (cx, vp);
		
		try
		{
			array -> emplace_back (*getArgument <jsSFNode> (cx, argv, 0));
		}
		catch (const std::domain_error &)
		{
			array -> emplace_back (nullptr);
		}

		return JS_NewNumberValue (cx, array -> size (), vp);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .push: %s.", getClass () -> name, error .what ());
	}
}

template class jsX3DArrayField <jsSFNode, X3D::MFNode>;

} // MozillaSpiderMonkey
} // X3D
} // titania