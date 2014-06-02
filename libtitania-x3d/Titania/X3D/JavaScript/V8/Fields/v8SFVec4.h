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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_V8_FIELDS_V8SFVEC4_H__
#define __TITANIA_X3D_JAVA_SCRIPT_V8_FIELDS_V8SFVEC4_H__

#include <v8.h>

#include "../v8Context.h"
#include "../v8String.h"

namespace titania {
namespace X3D {
namespace GoogleV8 {

template <class Type>
class SFVec4
{
public:

	static
	void
	initialize (Context* const, const v8::Local <v8::Object> &);


private:

	static
	v8::Handle <v8::Value>
	construct (const v8::Arguments &);

	static
	void
	realize (Context* const, const v8::Local <v8::Object> &, Type* const);

	static
	v8::Handle <v8::Value>
	toString (const v8::Arguments &);

	static
	void
	finalize (v8::Persistent <v8::Value>, void*);

};

template <class Type>
void
SFVec4 <Type>::initialize (Context* const context, const v8::Local <v8::Object> & globalObject)
{
	const auto className        = make_v8_string ("SFVec4f");
	const auto functionTemplate = v8::FunctionTemplate::New ();

	functionTemplate -> SetCallHandler (construct, v8::External::New (context));	
	functionTemplate -> SetClassName (className);

	const auto instanceTemplate = functionTemplate -> InstanceTemplate ();

	instanceTemplate -> SetInternalFieldCount (2);
	instanceTemplate -> Set (make_v8_string ("toString"), v8::FunctionTemplate::New (toString) -> GetFunction (), v8::PropertyAttribute (v8::ReadOnly | v8::DontDelete));

	globalObject -> Set (className, functionTemplate -> GetFunction (), v8::PropertyAttribute (v8::ReadOnly | v8::DontDelete));
}

template <class Type>
inline
Type
get_object (const v8::Arguments & args)
{
	return static_cast <Type> (v8::Handle <v8::External>::Cast (args .This () -> GetInternalField (0)) -> Value ());
}

template <class Type>
inline
Type
get_object (const v8::Local <v8::Object> & object)
{
	return static_cast <Type> (v8::Handle <v8::External>::Cast (object -> GetInternalField (0)) -> Value ());
}

template <class Type>
v8::Handle <v8::Value>
SFVec4 <Type>::toString (const v8::Arguments & args)
{
	const auto field = get_object <X3D::X3DChildObject*> (args);

	X3D::Generator::NicestStyle ();

	return make_v8_string (field -> toString ());
}

//template <class Type>
//v8::Handle <v8::Value>
//SFVec4 <Type>::create (Context* const context, Type* const field)
//{
//	try
//	{
//		return context -> getObject (field);
//	}
//	catch (const std::out_of_range &)
//	{
//		const auto object = ... NewInstance ();
//
//		realize (context, object, field);
//
//		return object;
//	}
//}

template <class Type>
void
SFVec4 <Type>::realize (Context* const context, const v8::Local <v8::Object> & object, Type* const field)
{
	v8::V8::AdjustAmountOfExternalAllocatedMemory (sizeof (Type));

   auto persistent = v8::Persistent <v8::Object>::New (object);

   persistent .MakeWeak (context, finalize);

	object -> SetInternalField (0, v8::External::New (field));

	//context -> addObject (field, object);
}

template <class Type>
v8::Handle <v8::Value>
SFVec4 <Type>::construct (const v8::Arguments & args)
{
	__LOG__ << std::endl;

	if (args .IsConstructCall ())
	{
		const auto context = get_context (args);
		const auto object  = args .This ();

		switch (args .Length ())
		{
			case 0:
			{
				realize (context, object, new Type ());
				break;
			}
			case 4:
			{
				realize (context, object, new Type (args [0] -> ToNumber () -> Value (),
				                                    args [1] -> ToNumber () -> Value (),
				                                    args [2] -> ToNumber () -> Value (),
				                                    args [3] -> ToNumber () -> Value ()));
				break;
			}
			default:
				return v8::ThrowException (make_v8_string ("Wrong number of arguments."));
		}
	}

	return v8::Undefined ();
}

template <class Type>
void
SFVec4 <Type>::finalize (v8::Persistent <v8::Value> value, void* parameter)
{
	__LOG__ << std::endl;

	const auto context = static_cast <Context*> (parameter);
	const auto field   = get_object <X3D::X3DFieldDefinition*> (value -> ToObject ());

	v8::V8::AdjustAmountOfExternalAllocatedMemory (-sizeof (Type));

	//context -> removeObject (field);

	delete field;
}

extern template class SFVec4 <X3D::SFVec4d>;
extern template class SFVec4 <X3D::SFVec4f>;

using SFVec4d = SFVec4 <X3D::SFVec4d>;
using SFVec4f = SFVec4 <X3D::SFVec4f>;

} // GoogleV8
} // X3D
} // titania

#endif
