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

#include "X3DNode.h"

#include "../../Bits/Cast.h"
#include "../../Bits/Traverse.h"
#include "../../Browser/X3DBrowser.h"
#include "../Core/MetadataBoolean.h"
#include "../Core/MetadataDouble.h"
#include "../Core/MetadataFloat.h"
#include "../Core/MetadataInteger.h"
#include "../Core/MetadataString.h"
#include "../Core/MetadataSet.h"
#include "../Layering/X3DLayerNode.h"

namespace titania {
namespace X3D {

class X3DNode::MetadataHandling
{
public:

	template <class Type, class Metadata>
	static
	typename std::enable_if <
		std::is_same <Type, MFBool>::value or
		std::is_same <Type, MFDouble>::value or
		std::is_same <Type, MFFloat>::value or
		std::is_same <Type, MFInt32>::value or
		std::is_same <Type, MFNode>::value or
		std::is_same <Type, MFString>::value,
		void
		>::type
	setMetaValue (X3DNode* const node, const std::string & key, const Type & value)
	throw (Error <DISPOSED>)
	{
		auto names = std::vector <std::string> ();
	
		basic::split (std::back_inserter (names), key, node -> SEPARATOR);
	
		const auto metadataSet = node -> getMetadataSet (names, false);
	
		metadataSet -> getValue <Metadata> (names .back (), false) -> value () = value;
	}
	
	template <class Type, class Metadata>
	static
	typename std::enable_if <
		not (std::is_same <Type, MFBool>::value or
		     std::is_same <Type, MFDouble>::value or
		     std::is_same <Type, MFFloat>::value or
		     std::is_same <Type, MFInt32>::value or
		     std::is_same <Type, MFNode>::value or
		     std::is_same <Type, MFString>::value),
		void
		>::type
	setMetaValue (X3DNode* const node, const std::string & key, const Type & value)
	throw (Error <DISPOSED>)
	{
		static constexpr auto s = std::tuple_size <typename Type::value_type::internal_type>::value;

		auto names = std::vector <std::string> ();
	
		basic::split (std::back_inserter (names), key, node -> SEPARATOR);
	
		const auto metadataSet = node -> getMetadataSet (names, false);
		auto  &    metaValue   = metadataSet -> getValue <Metadata> (names .back (), false) -> value ();
	
		metaValue .resize (value .size () * s);
	
		for (size_t i = 0, m = 0, size = value .size (); i < size; ++ i)
		{
			for (size_t v = 0; v < s; ++ v)
				metaValue [m ++] = value [i] .get1Value (v);
		}
	}
	
	template <class Type, class Metadata>
	static
	typename std::enable_if <
		std::is_same <Type, SFBool::internal_type>::value or
		std::is_same <Type, SFDouble::internal_type>::value or
		std::is_same <Type, SFFloat::internal_type>::value or
		std::is_same <Type, SFInt32::internal_type>::value or
		std::is_same <Type, SFNode>::value or
		std::is_same <Type, SFString::internal_type>::value,
		Type
		>::type
	getMetaValue (X3DNode* const node, const std::string & key, const Type & defaultValue)
	throw (Error <DISPOSED>)
	{
		try
		{
			std::vector <std::string> names;
		
			basic::split (std::back_inserter (names), key, node -> SEPARATOR);
		
			const auto   metadataSet = node -> getMetadataSet (names, true);
			const auto & metaValue   = metadataSet -> getValue <Metadata> (names .back (), true) -> value ();
	
			return metaValue .at (0);
		}
		catch (const Error <DISPOSED> &)
		{
			throw;
		}
		catch (const std::exception &)
		{
			return defaultValue;
		}
	}
	
	template <class Type, class Metadata>
	static
	typename std::enable_if <
		std::is_same <Type, MFBool>::value or
		std::is_same <Type, MFDouble>::value or
		std::is_same <Type, MFFloat>::value or
		std::is_same <Type, MFInt32>::value or
		std::is_same <Type, MFNode>::value or
		std::is_same <Type, MFString>::value,
		Type
		>::type
	getMetaValue (X3DNode* const node, const std::string & key, const Type & defaultValue)
	throw (Error <DISPOSED>)
	{
		try
		{
			std::vector <std::string> names;
		
			basic::split (std::back_inserter (names), key, node -> SEPARATOR);
		
			const auto   metadataSet = node -> getMetadataSet (names, true);
			const auto & metaValue   = metadataSet -> getValue <Metadata> (names .back (), true) -> value ();
	
			return metaValue;
		}
		catch (const Error <DISPOSED> &)
		{
			throw;
		}
		catch (const X3DError &)
		{
			return defaultValue;
		}
	}
	
	template <class Type, class Metadata>
	static
	typename std::enable_if <
		not (std::is_same <Type, SFBool::internal_type>::value or
		     std::is_same <Type, SFDouble::internal_type>::value or
		     std::is_same <Type, SFFloat::internal_type>::value or
		     std::is_same <Type, SFInt32::internal_type>::value or
		     std::is_same <Type, SFNode>::value or
		     std::is_same <Type, SFString::internal_type>::value or
		     std::is_same <Type, MFBool>::value or
		     std::is_same <Type, MFDouble>::value or
		     std::is_same <Type, MFFloat>::value or
		     std::is_same <Type, MFInt32>::value or
		     std::is_same <Type, MFNode>::value or
		     std::is_same <Type, MFString>::value),
		Type
		>::type
	getMetaValue (X3DNode* const node, const std::string & key, const Type & defaultValue)
	throw (Error <DISPOSED>)
	{
		try
		{
			static constexpr auto s = std::tuple_size <typename Type::value_type::internal_type>::value;

			std::vector <std::string> names;
		
			basic::split (std::back_inserter (names), key, node -> SEPARATOR);
		
			const auto   metadataSet = node -> getMetadataSet (names, true);
			const auto & metaValue   = metadataSet -> getValue <Metadata> (names .back (), true) -> value ();
	
			Type value (metaValue .size () / s);
	
			for (size_t i = 0, m = 0, size = value .size (); i < size; ++ i)
			{
				for (size_t v = 0; v < s; ++ v)
					value [i] .set1Value (v, metaValue [m ++]);
			}
	
			return value;
		}
		catch (const Error <DISPOSED> &)
		{
			throw;
		}
		catch (const X3DError &)
		{
			return defaultValue;
		}
	}

};

const std::string X3DNode::SEPARATOR = "/";

X3DNode::Fields::Fields () :
	metadata (new SFNode ())
{ }

X3DNode::X3DNode () :
	X3DBaseNode (),
	     fields ()
{
	addType (X3DConstants::X3DNode);

}
/*
 * Meta Data
 */

X3DPtr <MetadataSet>
X3DNode::createMetadataSet (const std::string & key)
throw (Error <NOT_SUPPORTED>,
       Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	auto names = std::vector <std::string> ();

	basic::split (std::back_inserter (names), key, SEPARATOR);

	const auto metadataSet = getMetadataSet (names, false);

	return metadataSet -> createValue <MetadataSet> (names .back ());
}

X3DPtr <MetadataSet>
X3DNode::getMetadataSet (const std::string & key) const
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	std::vector <std::string> names;

	basic::split (std::back_inserter (names), key, SEPARATOR);

	const auto metadataSet = getMetadataSet (names, true);
	const auto value       = metadataSet -> getValue <MetadataSet> (names .back ());

	return value;
}

template <>
void
X3DNode::setMetaData <SFBool::internal_type> (const std::string & key, const SFBool::internal_type & value)
throw (Error <DISPOSED>)
{
	setMetaData(key, MFBool ({ value }));
}

template <>
void
X3DNode::setMetaData <SFColor::internal_type> (const std::string & key, const SFColor::internal_type & value)
throw (Error <DISPOSED>)
{
	setMetaData (key, MFColor ({ value }));
}

template <>
void
X3DNode::setMetaData <SFColorRGBA::internal_type> (const std::string & key, const SFColorRGBA::internal_type & value)
throw (Error <DISPOSED>)
{
	setMetaData (key, MFColorRGBA ({ value }));
}

template <>
void
X3DNode::setMetaData <SFDouble::internal_type> (const std::string & key, const SFDouble::internal_type & value)
throw (Error <DISPOSED>)
{
	setMetaData (key, MFDouble ({ value }));
}

template <>
void
X3DNode::setMetaData <SFFloat::internal_type> (const std::string & key, const SFFloat::internal_type & value)
throw (Error <DISPOSED>)
{
	setMetaData (key, MFFloat ({ value }));
}

template <>
void
X3DNode::setMetaData <SFImage> (const std::string & key, const SFImage & value)
throw (Error <DISPOSED>)
{
	MFInt32 array;

	array .emplace_back (value .getWidth ());
	array .emplace_back (value .getHeight ());
	array .emplace_back (value .getComponents ());

	for (const auto & pixel : value .getArray ())
		array .emplace_back (pixel);

	setMetaData (key, array);
}

template <>
void
X3DNode::setMetaData <SFInt32::internal_type> (const std::string & key, const SFInt32::internal_type & value)
throw (Error <DISPOSED>)
{
	setMetaData (key, MFInt32 ({ value }));
}

template <>
void
X3DNode::setMetaData <SFMatrix3d::internal_type> (const std::string & key, const SFMatrix3d::internal_type & value)
throw (Error <DISPOSED>)
{
	setMetaData (key, MFMatrix3d ({ value }));
}

template <>
void
X3DNode::setMetaData <SFMatrix3f::internal_type> (const std::string & key, const SFMatrix3f::internal_type & value)
throw (Error <DISPOSED>)
{
	setMetaData (key, MFMatrix3f ({ value }));
}

template <>
void
X3DNode::setMetaData <SFMatrix4d::internal_type> (const std::string & key, const SFMatrix4d::internal_type & value)
throw (Error <DISPOSED>)
{
	setMetaData (key, MFMatrix4d ({ value }));
}

template <>
void
X3DNode::setMetaData <SFMatrix4f::internal_type> (const std::string & key, const SFMatrix4f::internal_type & value)
throw (Error <DISPOSED>)
{
	setMetaData (key, MFMatrix4f ({ value }));
}

template <>
void
X3DNode::setMetaData <SFNode> (const std::string & key, const SFNode & value)
throw (Error <DISPOSED>)
{
	setMetaData (key, MFNode ({ value }));
}

template <>
void
X3DNode::setMetaData <SFRotation::internal_type> (const std::string & key, const SFRotation::internal_type & value)
throw (Error <DISPOSED>)
{
	setMetaData (key, MFRotation ({ value }));
}

template <>
void
X3DNode::setMetaData <SFString::internal_type> (const std::string & key, const SFString::internal_type & value)
throw (Error <DISPOSED>)
{
	setMetaData (key, MFString ({ value }));
}

template <>
void
X3DNode::setMetaData <SFVec2d::internal_type> (const std::string & key, const SFVec2d::internal_type & value)
throw (Error <DISPOSED>)
{
	setMetaData (key, MFVec2d ({ value }));
}

template <>
void
X3DNode::setMetaData <SFVec2f::internal_type> (const std::string & key, const SFVec2f::internal_type & value)
throw (Error <DISPOSED>)
{
	setMetaData (key, MFVec2f ({ value }));
}

template <>
void
X3DNode::setMetaData <SFVec3d::internal_type> (const std::string & key, const SFVec3d::internal_type & value)
throw (Error <DISPOSED>)
{
	setMetaData (key, MFVec3d ({ value }));
}

template <>
void
X3DNode::setMetaData <SFVec3f::internal_type> (const std::string & key, const SFVec3f::internal_type & value)
throw (Error <DISPOSED>)
{
	setMetaData (key, MFVec3f ({ value }));
}

template <>
void
X3DNode::setMetaData <SFVec4d::internal_type> (const std::string & key, const SFVec4d::internal_type & value)
throw (Error <DISPOSED>)
{
	setMetaData (key, MFVec4d ({ value }));
}

template <>
void
X3DNode::setMetaData <SFVec4f::internal_type> (const std::string & key, const SFVec4f::internal_type & value)
throw (Error <DISPOSED>)
{
	setMetaData (key, MFVec4f ({ value }));
}

template <>
void
X3DNode::setMetaData <MFBool> (const std::string & key, const MFBool & value)
throw (Error <DISPOSED>)
{
	MetadataHandling::setMetaValue <MFBool, MetadataBoolean> (this, key, value);
}

template <>
void
X3DNode::setMetaData <MFColor> (const std::string & key, const MFColor & value)
throw (Error <DISPOSED>)
{
	MetadataHandling::setMetaValue <MFColor, MetadataFloat> (this, key, value);
}

template <>
void
X3DNode::setMetaData <MFColorRGBA> (const std::string & key, const MFColorRGBA & value)
throw (Error <DISPOSED>)
{
	MetadataHandling::setMetaValue <MFColorRGBA, MetadataFloat> (this, key, value);
}

template <>
void
X3DNode::setMetaData <MFDouble> (const std::string & key, const MFDouble & value)
throw (Error <DISPOSED>)
{
	MetadataHandling::setMetaValue <MFDouble, MetadataDouble> (this, key, value);
}

template <>
void
X3DNode::setMetaData <MFFloat> (const std::string & key, const MFFloat & value)
throw (Error <DISPOSED>)
{
	MetadataHandling::setMetaValue <MFFloat, MetadataFloat> (this, key, value);
}

template <>
void
X3DNode::setMetaData <MFImage> (const std::string & key, const MFImage & value)
throw (Error <DISPOSED>)
{
	MFInt32 array;

	for (const auto & image : value)
	{
		array .emplace_back (image .getWidth ());
		array .emplace_back (image .getHeight ());
		array .emplace_back (image .getComponents ());
	
		for (const auto & pixel : image .getArray ())
			array .emplace_back (pixel);
	}

	setMetaData (key, array);
}

template <>
void
X3DNode::setMetaData <MFInt32> (const std::string & key, const MFInt32 & value)
throw (Error <DISPOSED>)
{
	MetadataHandling::setMetaValue <MFInt32, MetadataInteger> (this, key, value);
}

template <>
void
X3DNode::setMetaData <MFMatrix3d> (const std::string & key, const MFMatrix3d & value)
throw (Error <DISPOSED>)
{
	MetadataHandling::setMetaValue <MFMatrix3d, MetadataDouble> (this, key, value);
}

template <>
void
X3DNode::setMetaData <MFMatrix3f> (const std::string & key, const MFMatrix3f & value)
throw (Error <DISPOSED>)
{
	MetadataHandling::setMetaValue <MFMatrix3f, MetadataFloat> (this, key, value);
}

template <>
void
X3DNode::setMetaData <MFMatrix4d> (const std::string & key, const MFMatrix4d & value)
throw (Error <DISPOSED>)
{
	MetadataHandling::setMetaValue <MFMatrix4d, MetadataDouble> (this, key, value);
}

template <>
void
X3DNode::setMetaData <MFMatrix4f> (const std::string & key, const MFMatrix4f & value)
throw (Error <DISPOSED>)
{
	MetadataHandling::setMetaValue <MFMatrix4f, MetadataFloat> (this, key, value);
}

template <>
void
X3DNode::setMetaData <MFNode> (const std::string & key, const MFNode & value)
throw (Error <DISPOSED>)
{
	MetadataHandling::setMetaValue <MFNode, MetadataSet> (this, key, value);
}

template <>
void
X3DNode::setMetaData <MFRotation> (const std::string & key, const MFRotation & value)
throw (Error <DISPOSED>)
{
	MetadataHandling::setMetaValue <MFRotation, MetadataDouble> (this, key, value);
}

template <>
void
X3DNode::setMetaData <MFString> (const std::string & key, const MFString & value)
throw (Error <DISPOSED>)
{
	MetadataHandling::setMetaValue <MFString, MetadataString> (this, key, value);
}

template <>
void
X3DNode::setMetaData <MFVec2d> (const std::string & key, const MFVec2d & value)
throw (Error <DISPOSED>)
{
	MetadataHandling::setMetaValue <MFVec2d, MetadataDouble> (this, key, value);
}

template <>
void
X3DNode::setMetaData <MFVec2f> (const std::string & key, const MFVec2f & value)
throw (Error <DISPOSED>)
{
	MetadataHandling::setMetaValue <MFVec2f, MetadataFloat> (this, key, value);
}

template <>
void
X3DNode::setMetaData <MFVec3d> (const std::string & key, const MFVec3d & value)
throw (Error <DISPOSED>)
{
	MetadataHandling::setMetaValue <MFVec3d, MetadataDouble> (this, key, value);
}

template <>
void
X3DNode::setMetaData <MFVec3f> (const std::string & key, const MFVec3f & value)
throw (Error <DISPOSED>)
{
	MetadataHandling::setMetaValue <MFVec3f, MetadataFloat> (this, key, value);
}

template <>
void
X3DNode::setMetaData <MFVec4d> (const std::string & key, const MFVec4d & value)
throw (Error <DISPOSED>)
{
	MetadataHandling::setMetaValue <MFVec4d, MetadataDouble> (this, key, value);
}

template <>
void
X3DNode::setMetaData <MFVec4f> (const std::string & key, const MFVec4f & value)
throw (Error <DISPOSED>)
{
	MetadataHandling::setMetaValue <MFVec4f, MetadataFloat> (this, key, value);
}

template <>
SFBool::internal_type
X3DNode::getMetaData <SFBool::internal_type> (const std::string & key, const SFBool::internal_type & defaultValue)
throw (Error <DISPOSED>)
{
	return MetadataHandling::getMetaValue <SFBool::internal_type, MetadataBoolean> (this, key, defaultValue);
}

template <>
SFColor::internal_type
X3DNode::getMetaData <SFColor::internal_type> (const std::string & key, const SFColor::internal_type & defaultValue)
throw (Error <DISPOSED>)
{
	const auto metaValue = getMetaData <MFFloat> (key);

	if (metaValue .size () < std::tuple_size <SFColor::internal_type>::value)
		return defaultValue;

	return SFColor::internal_type (metaValue [0], metaValue [1], metaValue [2]);
}

template <>
SFColorRGBA::internal_type
X3DNode::getMetaData <SFColorRGBA::internal_type> (const std::string & key, const SFColorRGBA::internal_type & defaultValue)
throw (Error <DISPOSED>)
{
	const auto metaValue = getMetaData <MFFloat> (key);

	if (metaValue .size () < std::tuple_size <SFColorRGBA::internal_type>::value)
		return defaultValue;

	return SFColorRGBA::internal_type (metaValue [0], metaValue [1], metaValue [2], metaValue [3]);
}

template <>
SFDouble::internal_type
X3DNode::getMetaData <SFDouble::internal_type> (const std::string & key, const SFDouble::internal_type & defaultValue)
throw (Error <DISPOSED>)
{
	return MetadataHandling::getMetaValue <SFDouble::internal_type, MetadataDouble> (this, key, defaultValue);
}

template <>
SFFloat::internal_type
X3DNode::getMetaData <SFFloat::internal_type> (const std::string & key, const SFFloat::internal_type & defaultValue)
throw (Error <DISPOSED>)
{
	return MetadataHandling::getMetaValue <SFFloat::internal_type, MetadataFloat> (this, key, defaultValue);
}

template <>
SFImage
X3DNode::getMetaData <SFImage> (const std::string & key, const SFImage & defaultValue)
throw (Error <DISPOSED>)
{
	auto array = MetadataHandling::getMetaValue <MFInt32, MetadataInteger> (this, key, MFInt32 ()); // XXX: defaultValue

	if (array .size () < 3)
		return defaultValue;

	const auto width      = std::max <int32_t> (0, array [0]);
	const auto height     = std::max <int32_t> (0, array [1]);
	const auto components = std::max <int32_t> (0, array [2]);
	const auto size       = width * height;

	if (int32_t (array .size ()) < size + 3)
		return defaultValue;

	array .erase (array .begin (), array .begin () + 3);

	return SFImage (width, height, components, array);
}

template <>
SFInt32::internal_type
X3DNode::getMetaData <SFInt32::internal_type> (const std::string & key, const SFInt32::internal_type & defaultValue)
throw (Error <DISPOSED>)
{
	return MetadataHandling::getMetaValue <SFInt32::internal_type, MetadataInteger> (this, key, defaultValue);
}

template <>
SFMatrix3d::internal_type
X3DNode::getMetaData <SFMatrix3d::internal_type> (const std::string & key, const SFMatrix3d::internal_type & defaultValue)
throw (Error <DISPOSED>)
{
	const auto metaValue = getMetaData <MFDouble> (key);

	if (metaValue .size () < std::tuple_size <SFMatrix3d::internal_type>::value)
		return defaultValue;

	return SFMatrix3d::internal_type (metaValue [0],
	                                  metaValue [1],
	                                  metaValue [2],
	                                  metaValue [3],
	                                  metaValue [4],
	                                  metaValue [5],
	                                  metaValue [6],
	                                  metaValue [7],
	                                  metaValue [8]);
}

template <>
SFMatrix3f::internal_type
X3DNode::getMetaData <SFMatrix3f::internal_type> (const std::string & key, const SFMatrix3f::internal_type & defaultValue)
throw (Error <DISPOSED>)
{
	const auto metaValue = getMetaData <MFFloat> (key);

	if (metaValue .size () < std::tuple_size <SFMatrix3f::internal_type>::value)
		return defaultValue;

	return SFMatrix3f::internal_type (metaValue [0],
	                                  metaValue [1],
	                                  metaValue [2],
	                                  metaValue [3],
	                                  metaValue [4],
	                                  metaValue [5],
	                                  metaValue [6],
	                                  metaValue [7],
	                                  metaValue [8]);
}

template <>
SFMatrix4d::internal_type
X3DNode::getMetaData <SFMatrix4d::internal_type> (const std::string & key, const SFMatrix4d::internal_type & defaultValue)
throw (Error <DISPOSED>)
{
	const auto metaValue = getMetaData <MFDouble> (key);

	if (metaValue .size () < std::tuple_size <SFMatrix4d::internal_type>::value)
		return defaultValue;

	return SFMatrix4d::internal_type (metaValue [ 0],
	                                  metaValue [ 1],
	                                  metaValue [ 2],
	                                  metaValue [ 3],
	                                  metaValue [ 4],
	                                  metaValue [ 5],
	                                  metaValue [ 6],
	                                  metaValue [ 7],
	                                  metaValue [ 8],
	                                  metaValue [ 9],
	                                  metaValue [10],
	                                  metaValue [11],
	                                  metaValue [12],
	                                  metaValue [13],
	                                  metaValue [14],
	                                  metaValue [15]);
}

template <>
SFMatrix4f::internal_type
X3DNode::getMetaData <SFMatrix4f::internal_type> (const std::string & key, const SFMatrix4f::internal_type & defaultValue)
throw (Error <DISPOSED>)
{
	const auto metaValue = getMetaData <MFFloat> (key);

	if (metaValue .size () < std::tuple_size <SFMatrix4f::internal_type>::value)
		return defaultValue;

	return SFMatrix4f::internal_type (metaValue [ 0],
	                                  metaValue [ 1],
	                                  metaValue [ 2],
	                                  metaValue [ 3],
	                                  metaValue [ 4],
	                                  metaValue [ 5],
	                                  metaValue [ 6],
	                                  metaValue [ 7],
	                                  metaValue [ 8],
	                                  metaValue [ 9],
	                                  metaValue [10],
	                                  metaValue [11],
	                                  metaValue [12],
	                                  metaValue [13],
	                                  metaValue [14],
	                                  metaValue [15]);
}

template <>
SFNode
X3DNode::getMetaData <SFNode> (const std::string & key, const SFNode & defaultValue)
throw (Error <DISPOSED>)
{
	return MetadataHandling::getMetaValue <SFNode, MetadataSet> (this, key, defaultValue);
}

template <>
SFRotation::internal_type
X3DNode::getMetaData <SFRotation::internal_type> (const std::string & key, const SFRotation::internal_type & defaultValue)
throw (Error <DISPOSED>)
{
	const auto metaValue = getMetaData <MFDouble> (key);

	if (metaValue .size () < std::tuple_size <SFRotation::internal_type>::value)
		return defaultValue;

	return SFRotation::internal_type (metaValue [0], metaValue [1], metaValue [2], metaValue [3]);
}

template <>
SFString::internal_type
X3DNode::getMetaData <SFString::internal_type> (const std::string & key, const SFString::internal_type & defaultValue)
throw (Error <DISPOSED>)
{
	return MetadataHandling::getMetaValue <SFString::internal_type, MetadataString> (this, key, defaultValue);
}

template <>
SFVec2d::internal_type
X3DNode::getMetaData <SFVec2d::internal_type> (const std::string & key, const SFVec2d::internal_type & defaultValue)
throw (Error <DISPOSED>)
{
	const auto metaValue = getMetaData <MFDouble> (key);

	if (metaValue .size () < std::tuple_size <SFVec2d::internal_type>::value)
		return defaultValue;

	return SFVec2d::internal_type (metaValue [0], metaValue [1]);
}

template <>
SFVec2f::internal_type
X3DNode::getMetaData <SFVec2f::internal_type> (const std::string & key, const SFVec2f::internal_type & defaultValue)
throw (Error <DISPOSED>)
{
	const auto metaValue = getMetaData <MFFloat> (key);

	if (metaValue .size () < std::tuple_size <SFVec2f::internal_type>::value)
		return defaultValue;

	return SFVec2f::internal_type (metaValue [0], metaValue [1]);
}

template <>
SFVec3d::internal_type
X3DNode::getMetaData <SFVec3d::internal_type> (const std::string & key, const SFVec3d::internal_type & defaultValue)
throw (Error <DISPOSED>)
{
	const auto metaValue = getMetaData <MFDouble> (key);

	if (metaValue .size () < std::tuple_size <SFVec3d::internal_type>::value)
		return defaultValue;

	return SFVec3d::internal_type (metaValue [0], metaValue [1], metaValue [2]);
}

template <>
SFVec3f::internal_type
X3DNode::getMetaData <SFVec3f::internal_type> (const std::string & key, const SFVec3f::internal_type & defaultValue)
throw (Error <DISPOSED>)
{
	const auto metaValue = getMetaData <MFFloat> (key);

	if (metaValue .size () < std::tuple_size <SFVec3f::internal_type>::value)
		return defaultValue;

	return SFVec3f::internal_type (metaValue [0], metaValue [1], metaValue [2]);
}

template <>
SFVec4d::internal_type
X3DNode::getMetaData <SFVec4d::internal_type> (const std::string & key, const SFVec4d::internal_type & defaultValue)
throw (Error <DISPOSED>)
{
	const auto metaValue = getMetaData <MFDouble> (key);

	if (metaValue .size () < std::tuple_size <SFVec4d::internal_type>::value)
		return defaultValue;

	return SFVec4d::internal_type (metaValue [0], metaValue [1], metaValue [2], metaValue [3]);
}

template <>
SFVec4f::internal_type
X3DNode::getMetaData <SFVec4f::internal_type> (const std::string & key, const SFVec4f::internal_type & defaultValue)
throw (Error <DISPOSED>)
{
	const auto metaValue = getMetaData <MFFloat> (key);

	if (metaValue .size () < std::tuple_size <SFVec4f::internal_type>::value)
		return defaultValue;

	return SFVec4f::internal_type (metaValue [0], metaValue [1], metaValue [2], metaValue [3]);
}

template <>
MFBool
X3DNode::getMetaData <MFBool> (const std::string & key, const MFBool & defaultValue)
throw (Error <DISPOSED>)
{
	return MetadataHandling::getMetaValue <MFBool, MetadataBoolean> (this, key, defaultValue);
}

template <>
MFColor
X3DNode::getMetaData <MFColor> (const std::string & key, const MFColor & defaultValue)
throw (Error <DISPOSED>)
{
	return MetadataHandling::getMetaValue <MFColor, MetadataFloat> (this, key, defaultValue);
}

template <>
MFColorRGBA
X3DNode::getMetaData <MFColorRGBA> (const std::string & key, const MFColorRGBA & defaultValue)
throw (Error <DISPOSED>)
{
	return MetadataHandling::getMetaValue <MFColorRGBA, MetadataFloat> (this, key, defaultValue);
}

template <>
MFDouble
X3DNode::getMetaData <MFDouble> (const std::string & key, const MFDouble & defaultValue)
throw (Error <DISPOSED>)
{
	return MetadataHandling::getMetaValue <MFDouble, MetadataDouble> (this, key, defaultValue);
}

template <>
MFFloat
X3DNode::getMetaData <MFFloat> (const std::string & key, const MFFloat & defaultValue)
throw (Error <DISPOSED>)
{
	return MetadataHandling::getMetaValue <MFFloat, MetadataFloat> (this, key, defaultValue);
}

template <>
MFImage
X3DNode::getMetaData <MFImage> (const std::string & key, const MFImage & defaultValue)
throw (Error <DISPOSED>)
{
	auto array = MetadataHandling::getMetaValue <MFInt32, MetadataInteger> (this, key, MFInt32 ());

	MFImage images;

	for (; ; )
	{	
		if (array .size () < 3)
			return images;
	
		const auto width      = std::max <int32_t> (0, array [0]);
		const auto height     = std::max <int32_t> (0, array [1]);
		const auto components = std::max <int32_t> (0, array [2]);
		const auto size       = width * height;

		if (int32_t (array .size ()) < size + 3)
			return images;

		array .erase (array .begin (), array .begin () + 3);

		images .emplace_back (width, height, components, MFInt32 (array .begin (), array .begin () + size));

		array .erase (array .begin (), array .begin () + size);
	}

	return images;
}

template <>
MFInt32
X3DNode::getMetaData <MFInt32> (const std::string & key, const MFInt32 & defaultValue)
throw (Error <DISPOSED>)
{
	return MetadataHandling::getMetaValue <MFInt32, MetadataInteger> (this, key, defaultValue);
}

template <>
MFMatrix3d
X3DNode::getMetaData <MFMatrix3d> (const std::string & key, const MFMatrix3d & defaultValue)
throw (Error <DISPOSED>)
{
	return MetadataHandling::getMetaValue <MFMatrix3d, MetadataDouble> (this, key, defaultValue);
}

template <>
MFMatrix3f
X3DNode::getMetaData <MFMatrix3f> (const std::string & key, const MFMatrix3f & defaultValue)
throw (Error <DISPOSED>)
{
	return MetadataHandling::getMetaValue <MFMatrix3f, MetadataFloat> (this, key, defaultValue);
}

template <>
MFMatrix4d
X3DNode::getMetaData <MFMatrix4d> (const std::string & key, const MFMatrix4d & defaultValue)
throw (Error <DISPOSED>)
{
	return MetadataHandling::getMetaValue <MFMatrix4d, MetadataDouble> (this, key, defaultValue);
}

template <>
MFMatrix4f
X3DNode::getMetaData <MFMatrix4f> (const std::string & key, const MFMatrix4f & defaultValue)
throw (Error <DISPOSED>)
{
	return MetadataHandling::getMetaValue <MFMatrix4f, MetadataFloat> (this, key, defaultValue);
}

template <>
MFNode
X3DNode::getMetaData <MFNode> (const std::string & key, const MFNode & defaultValue)
throw (Error <DISPOSED>)
{
	return MetadataHandling::getMetaValue <MFNode, MetadataSet> (this, key, defaultValue);
}

template <>
MFRotation
X3DNode::getMetaData <MFRotation> (const std::string & key, const MFRotation & defaultValue)
throw (Error <DISPOSED>)
{
	return MetadataHandling::getMetaValue <MFRotation, MetadataDouble> (this, key, defaultValue);
}

template <>
MFString
X3DNode::getMetaData <MFString> (const std::string & key, const MFString & defaultValue)
throw (Error <DISPOSED>)
{
	return MetadataHandling::getMetaValue <MFString, MetadataString> (this, key, defaultValue);
}

template <>
MFVec2d
X3DNode::getMetaData <MFVec2d> (const std::string & key, const MFVec2d & defaultValue)
throw (Error <DISPOSED>)
{
	return MetadataHandling::getMetaValue <MFVec2d, MetadataDouble> (this, key, defaultValue);
}

template <>
MFVec2f
X3DNode::getMetaData <MFVec2f> (const std::string & key, const MFVec2f & defaultValue)
throw (Error <DISPOSED>)
{
	return MetadataHandling::getMetaValue <MFVec2f, MetadataFloat> (this, key, defaultValue);
}

template <>
MFVec3d
X3DNode::getMetaData <MFVec3d> (const std::string & key, const MFVec3d & defaultValue)
throw (Error <DISPOSED>)
{
	return MetadataHandling::getMetaValue <MFVec3d, MetadataDouble> (this, key, defaultValue);
}

template <>
MFVec3f
X3DNode::getMetaData <MFVec3f> (const std::string & key, const MFVec3f & defaultValue)
throw (Error <DISPOSED>)
{
	return MetadataHandling::getMetaValue <MFVec3f, MetadataFloat> (this, key, defaultValue);
}

template <>
MFVec4d
X3DNode::getMetaData <MFVec4d> (const std::string & key, const MFVec4d & defaultValue)
throw (Error <DISPOSED>)
{
	return MetadataHandling::getMetaValue <MFVec4d, MetadataDouble> (this, key, defaultValue);
}

template <>
MFVec4f
X3DNode::getMetaData <MFVec4f> (const std::string & key, const MFVec4f & defaultValue)
throw (Error <DISPOSED>)
{
	return MetadataHandling::getMetaValue <MFVec4f, MetadataFloat> (this, key, defaultValue);
}

/*
 * getMetadataSet
 */

MetadataSet*
X3DNode::getMetadataSet (const std::vector <std::string> & names, const bool throw_) const
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	if (names .size () < 3)
		throw Error <INVALID_NAME> ("X3DNode::getMetadataSet: invalid key.");

	if (not names .front () .empty ())
		throw Error <INVALID_NAME> ("X3DNode::getMetadataSet: invalid key.");

	if (names [1] .empty ())
		throw Error <INVALID_NAME> ("X3DNode::getMetadataSet: invalid key.");

	auto metadataSet = x3d_cast <MetadataSet*> (metadata ());

	if (not metadataSet or metadataSet -> name () not_eq names [1])
	{
		if (throw_)
			throw Error <INVALID_NAME> ("X3DNode::getMetadataSet: invalid key.");

		const_cast <X3DNode*> (this) -> metadata () = getExecutionContext () -> createNode <MetadataSet> ();

		metadataSet = x3d_cast <MetadataSet*> (metadata ());
		metadataSet -> name ()      = names [1];
		metadataSet -> reference () = getBrowser () -> getProviderUrl ();

		getExecutionContext () -> addNamedNode (getExecutionContext () -> getUniqueName (names [1]), metadata ());
	}

	for (const auto & name : std::make_pair (names .begin () + 2, names .end () - 1))
	{
	   metadataSet = metadataSet -> getValue <MetadataSet> (name, throw_);
	}

	return metadataSet;
}

/*
 * removeMetaData
 */

void
X3DNode::removeMetaData (const std::string & key)
throw (Error <DISPOSED>)
{
	auto names = std::vector <std::string> ();

	basic::split (std::back_inserter (names), key, SEPARATOR);

	switch (names .size ())
	{
		case 0:
		{
			metadata () = nullptr;
			return;
		}
		case 1:
			return;
		case 2:
		{
			const auto metadataSet = x3d_cast <MetadataSet*> (metadata ());

			if (not metadataSet)
			   return;
			 
			if (metadataSet -> name () not_eq names [1])
			   return;

			metadata () = nullptr;
			return;
		}
		default:
		{
			std::vector <MetadataSet*> metadataSets;

			const auto metadataSet = x3d_cast <MetadataSet*> (metadata ());

			if (not metadataSet)
				return;
			
			if (metadataSet -> name () not_eq names [1])
				return;

			metadataSets .emplace_back (metadataSet);

			try
			{
				for (const auto & name : std::make_pair (names .begin () + 2, names .end () - 1))
					metadataSets .emplace_back (metadataSets .back () -> getValue <MetadataSet> (name, true));
			}
			catch (const Error <INVALID_NAME> &)
			{
				return;
			}

			metadataSets .back () -> removeValue (names .back ());

			while (metadataSets .back () -> value () .empty ())
			{
				names .pop_back ();
				metadataSets .pop_back ();

				if (metadataSets .empty ())
				{
					metadata () = nullptr;
					break;
				}

				metadataSets .back () -> removeValue (names .back ());
			}

			return;
		}
	}
}

/*
 * fromMetaData / toMetaData
 */

void
X3DNode::fromMetaData (const X3DPtr <MetadataSet> & metadataSetNode)
{
	for (const auto & fieldDefinition : getFieldDefinitions ())
		fromMetaData (metadataSetNode, fieldDefinition);
}

void
X3DNode::fromMetaData (const X3DPtr <MetadataSet> & metadataSetNode, X3DFieldDefinition* const fieldDefinition) const
{
	try
	{
		switch (fieldDefinition -> getType ())
		{
			case X3D::X3DConstants::SFBool:
			{
				auto &       field = static_cast <SFBool &> (*fieldDefinition);
				const auto & value = metadataSetNode -> getValue <MetadataBoolean> (field .getName (), true) -> value ();
	
				field = value .at (0);
				break;
			}
			case X3D::X3DConstants::SFColor:
			{
				auto &       field = static_cast <SFColor &> (*fieldDefinition);
				const auto & value = metadataSetNode -> getValue <MetadataFloat> (field .getName (), true) -> value ();
	
				field = Color3f (value .at (0), value .at (1), value .at (2));
				break;
			}
			case X3D::X3DConstants::SFColorRGBA:
			{
				auto &       field    = static_cast <SFColorRGBA &> (*fieldDefinition);
				const auto & value = metadataSetNode -> getValue <MetadataFloat> (field .getName (), true) -> value ();
	
				field = Color4f (value .at (0), value .at (1), value .at (2), value .at (3));
				break;
			}
			case X3D::X3DConstants::SFDouble:
			{
				auto &       field = static_cast <SFDouble &> (*fieldDefinition);
				const auto & value = metadataSetNode -> getValue <MetadataDouble> (field .getName (), true) -> value ();
	
				field = value .at (0);
				break;
			}
			case X3D::X3DConstants::SFFloat:
			{
				auto &       field = static_cast <SFFloat &> (*fieldDefinition);
				const auto & value = metadataSetNode -> getValue <MetadataFloat> (field .getName (), true) -> value ();
	
				field = value .at (0);
				break;
			}
			case X3D::X3DConstants::SFInt32:
			{
				auto &       field = static_cast <SFInt32 &> (*fieldDefinition);
				const auto & value = metadataSetNode -> getValue <MetadataInteger> (field .getName (), true) -> value ();
	
				field = value .at (0);
				break;
			}
			case X3D::X3DConstants::SFImage:
			{
				auto &       field = static_cast <SFImage &> (*fieldDefinition);
				const auto & value = metadataSetNode -> getValue <MetadataInteger> (field .getName (), true) -> value ();
	
				SFImage image;
	
				image .setWidth      (value .at (0));
				image .setHeight     (value .at (1));
				image .setComponents (value .at (2));
	
				for (size_t i = 0, size = image .getArray () .size (); i < size; ++ i)
					image .getArray () [i] = value .at (i + 3);
	
				field = image;
				break;
			}
			case X3D::X3DConstants::SFMatrix3d:
			{
				auto &       field = static_cast <SFMatrix3d &> (*fieldDefinition);
				const auto & value = metadataSetNode -> getValue <MetadataDouble> (field .getName (), true) -> value ();
	
				SFMatrix3d matrix;
	
				for (size_t i = 0; i < std::tuple_size <Matrix3d>::value; ++ i)
				   matrix .set1Value (i, value .at (i));
	
				field = matrix;
				break;
			}
			case X3D::X3DConstants::SFMatrix3f:
			{
				auto &       field = static_cast <SFMatrix3f &> (*fieldDefinition);
				const auto & value = metadataSetNode -> getValue <MetadataFloat> (field .getName (), true) -> value ();
	
				SFMatrix3f matrix;
	
				for (size_t i = 0; i < std::tuple_size <Matrix3f>::value; ++ i)
				   matrix .set1Value (i, value .at (i));
	
				field = matrix;
				break;
			}
			case X3D::X3DConstants::SFMatrix4d:
			{
				auto &       field = static_cast <SFMatrix4d &> (*fieldDefinition);
				const auto & value = metadataSetNode -> getValue <MetadataDouble> (field .getName (), true) -> value ();
	
				SFMatrix4d matrix;
	
				for (size_t i = 0; i < std::tuple_size <Matrix4d>::value; ++ i)
				   matrix .set1Value (i, value .at (i));
	
				field = matrix;
				break;
			}
			case X3D::X3DConstants::SFMatrix4f:
			{
				auto &       field = static_cast <SFMatrix4f &> (*fieldDefinition);
				const auto & value = metadataSetNode -> getValue <MetadataFloat> (field .getName (), true) -> value ();
	
				SFMatrix4f matrix;
	
				for (size_t i = 0; i < std::tuple_size <Matrix4f>::value; ++ i)
				   matrix .set1Value (i, value .at (i));
	
				field = matrix;
				break;
			}
			case X3D::X3DConstants::SFNode:
			{
				auto &       field = static_cast <SFNode &> (*fieldDefinition);
				const auto & value = metadataSetNode -> getValue <MetadataSet> (field .getName (), true) -> value ();
			
				field = value .at (0);
				break;
			}
			case X3D::X3DConstants::SFRotation:
			{
				auto &       field = static_cast <SFRotation &> (*fieldDefinition);
				const auto & value = metadataSetNode -> getValue <MetadataDouble> (field .getName (), true) -> value ();
	
				field = Rotation4d (value .at (0), value .at (1), value .at (2), value .at (3));
				break;
			}
			case X3D::X3DConstants::SFString:
			{
				auto &       field = static_cast <SFString &> (*fieldDefinition);
				const auto & value = metadataSetNode -> getValue <MetadataString> (field .getName (), true) -> value ();
	
				field = value .at (0);
				break;
			}
			case X3D::X3DConstants::SFTime:
			{
				auto &       field = static_cast <SFTime &> (*fieldDefinition);
				const auto & value = metadataSetNode -> getValue <MetadataDouble> (field .getName (), true) -> value ();
	
				field = value .at (0);
				break;
			}
			case X3D::X3DConstants::SFVec2d:
			{
				auto &       field = static_cast <SFVec2d &> (*fieldDefinition);
				const auto & value = metadataSetNode -> getValue <MetadataDouble> (field .getName (), true) -> value ();
	
				field = Vector2d (value .at (0), value .at (1));
				break;
			}
			case X3D::X3DConstants::SFVec2f:
			{
				auto &       field = static_cast <SFVec2f &> (*fieldDefinition);
				const auto & value = metadataSetNode -> getValue <MetadataFloat> (field .getName (), true) -> value ();
	
				field = Vector2f (value .at (0), value .at (1));
				break;
			}
			case X3D::X3DConstants::SFVec3d:
			{
				auto &       field = static_cast <SFVec3d &> (*fieldDefinition);
				const auto & value = metadataSetNode -> getValue <MetadataDouble> (field .getName (), true) -> value ();
	
				field = Vector3d (value .at (0), value .at (1), value .at (2));
				break;
			}
			case X3D::X3DConstants::SFVec3f:
			{
				auto &       field = static_cast <SFVec3f &> (*fieldDefinition);
				const auto & value = metadataSetNode -> getValue <MetadataFloat> (field .getName (), true) -> value ();
	
				field = Vector3f (value .at (0), value .at (1), value .at (2));
				break;
			}
			case X3D::X3DConstants::SFVec4d:
			{
				auto &       field = static_cast <SFVec4d &> (*fieldDefinition);
				const auto & value = metadataSetNode -> getValue <MetadataDouble> (field .getName (), true) -> value ();
	
				field = Vector4d (value .at (0), value .at (1), value .at (2), value .at (3));
				break;
			}
			case X3D::X3DConstants::SFVec4f:
			{
				auto &       field = static_cast <SFVec4f &> (*fieldDefinition);
				const auto & value = metadataSetNode -> getValue <MetadataFloat> (field .getName (), true) -> value ();
	
				field = Vector4f (value .at (0), value .at (1), value .at (2), value .at (3));
				break;
			}
			case X3D::X3DConstants::MFBool:
			{
				auto &       field = static_cast <MFBool &> (*fieldDefinition);
				const auto & value = metadataSetNode -> getValue <MetadataBoolean> (field .getName (), true) -> value ();
	
				field = value;
				break;
			}
			case X3D::X3DConstants::MFColor:
			{
				auto &       field = static_cast <MFColor &> (*fieldDefinition);
				const auto & value = metadataSetNode -> getValue <MetadataFloat> (field .getName (), true) -> value ();
	
				MFColor array;
	
				for (size_t i = 0, size = value .size (); i < size; i += std::tuple_size <Color3f>::value)
					array .emplace_back (value .at (i), value .at (i + 1), value .at (i + 2));
	
				field = array;
				break;
			}
			case X3D::X3DConstants::MFColorRGBA:
			{
				auto &       field = static_cast <MFColorRGBA &> (*fieldDefinition);
				const auto & value = metadataSetNode -> getValue <MetadataFloat> (field .getName (), true) -> value ();
	
				MFColorRGBA array;
	
				for (size_t i = 0, size = value .size (); i < size; i += std::tuple_size <Color4f>::value)
					array .emplace_back (value .at (i), value .at (i + 1), value .at (i + 2), value .at (i + 3));
	
				field = array;
				break;
			}
			case X3D::X3DConstants::MFDouble:
			{
				auto &       field = static_cast <MFDouble &> (*fieldDefinition);
				const auto & value = metadataSetNode -> getValue <MetadataDouble> (field .getName (), true) -> value ();
	
				field = value;
				break;
			}
			case X3D::X3DConstants::MFFloat:
			{
				auto &       field = static_cast <MFFloat &> (*fieldDefinition);
				const auto & value = metadataSetNode -> getValue <MetadataFloat> (field .getName (), true) -> value ();
	
				field = value;
				break;
			}
			case X3D::X3DConstants::MFImage:
			{
				auto &       field = static_cast <MFImage &> (*fieldDefinition);
				const auto & value = metadataSetNode -> getValue <MetadataInteger> (field .getName (), true) -> value ();
	
				SFImage image;
				MFImage images;
	
				for (size_t n = 0, size = value .size (), count = 0; n < size; n += count)
				{
					image .setWidth      (value .at (n + 0));
					image .setHeight     (value .at (n + 1));
					image .setComponents (value .at (n + 2));
		
					for (size_t i = 0, size = image .getArray () .size (); i < size; ++ i)
						image .getArray () [i] = value .at (n + i + 3);
	
					count = image .getArray () .size () + 3;
	
					images .emplace_back (std::move (image));
				}
	
				field = images;
				break;
			}
			case X3D::X3DConstants::MFInt32:
			{
				auto &       field = static_cast <MFInt32 &> (*fieldDefinition);
				const auto & value = metadataSetNode -> getValue <MetadataInteger> (field .getName (), true) -> value ();
	
				field = value;
				break;
			}
			case X3D::X3DConstants::MFMatrix3d:
			{
				auto &       field = static_cast <MFMatrix3d &> (*fieldDefinition);
				const auto & value = metadataSetNode -> getValue <MetadataDouble> (field .getName (), true) -> value ();
	
				MFMatrix3d array;
	
				for (size_t i = 0, size = value .size (); i < size; i += std::tuple_size <Matrix3d>::value)
					array .emplace_back (value .at (i),
					                     value .at (i + 1),
					                     value .at (i + 2),
					                     value .at (i + 3),
					                     value .at (i + 4),
					                     value .at (i + 5),
					                     value .at (i + 6),
					                     value .at (i + 7),
					                     value .at (i + 8));
	
				field = array;
				break;
			}
			case X3D::X3DConstants::MFMatrix3f:
			{
				auto &       field = static_cast <MFMatrix3f &> (*fieldDefinition);
				const auto & value = metadataSetNode -> getValue <MetadataFloat> (field .getName (), true) -> value ();
	
				MFMatrix3f array;
	
				for (size_t i = 0, size = value .size (); i < size; i += std::tuple_size <Matrix3f>::value)
					array .emplace_back (value .at (i),
					                     value .at (i + 1),
					                     value .at (i + 2),
					                     value .at (i + 3),
					                     value .at (i + 4),
					                     value .at (i + 5),
					                     value .at (i + 6),
					                     value .at (i + 7),
					                     value .at (i + 8));
	
				field = array;
				break;
			}
			case X3D::X3DConstants::MFMatrix4d:
			{
				auto &       field = static_cast <MFMatrix4d &> (*fieldDefinition);
				const auto & value = metadataSetNode -> getValue <MetadataDouble> (field .getName (), true) -> value ();
	
				MFMatrix4d array;
	
				for (size_t i = 0, size = value .size (); i < size; i += std::tuple_size <Matrix4d>::value)
					array .emplace_back (value .at (i),
					                     value .at (i + 1),
					                     value .at (i + 2),
					                     value .at (i + 3),
					                     value .at (i + 4),
					                     value .at (i + 5),
					                     value .at (i + 6),
					                     value .at (i + 7),
					                     value .at (i + 8),
					                     value .at (i + 9),
					                     value .at (i + 10),
					                     value .at (i + 11),
					                     value .at (i + 12),
					                     value .at (i + 13),
					                     value .at (i + 14),
					                     value .at (i + 15));
	
				field = array;
				break;
			}
			case X3D::X3DConstants::MFMatrix4f:
			{
				auto &       field = static_cast <MFMatrix4f &> (*fieldDefinition);
				const auto & value = metadataSetNode -> getValue <MetadataFloat> (field .getName (), true) -> value ();
	
				MFMatrix4f array;
	
				for (size_t i = 0, size = value .size (); i < size; i += std::tuple_size <Matrix4f>::value)
					array .emplace_back (value .at (i),
					                     value .at (i + 1),
					                     value .at (i + 2),
					                     value .at (i + 3),
					                     value .at (i + 4),
					                     value .at (i + 5),
					                     value .at (i + 6),
					                     value .at (i + 7),
					                     value .at (i + 8),
					                     value .at (i + 9),
					                     value .at (i + 10),
					                     value .at (i + 11),
					                     value .at (i + 12),
					                     value .at (i + 13),
					                     value .at (i + 14),
					                     value .at (i + 15));
	
				field = array;
				break;
			}
			case X3D::X3DConstants::MFNode:
			{
				auto &       field = static_cast <MFNode &> (*fieldDefinition);
				const auto & value = metadataSetNode -> getValue <MetadataSet> (field .getName (), true) -> value ();
			
				field = value;
				break;
			}
			case X3D::X3DConstants::MFRotation:
			{
				auto &       field    = static_cast <MFRotation &> (*fieldDefinition);
				const auto & value = metadataSetNode -> getValue <MetadataDouble> (field .getName (), true) -> value ();
	
				MFRotation array;
	
				for (size_t i = 0, size = value .size (); i < size; i += std::tuple_size <Rotation4d>::value)
					array .emplace_back (value .at (i), value .at (i + 1), value .at (i + 2), value .at (i + 3));
	
				field = array;
				break;
			}
			case X3D::X3DConstants::MFString:
			{
				auto &       field = static_cast <MFString &> (*fieldDefinition);
				const auto & value = metadataSetNode -> getValue <MetadataString> (field .getName (), true) -> value ();
	
				field = value;
				break;
			}
			case X3D::X3DConstants::MFTime:
			{
				auto &       field = static_cast <MFTime &> (*fieldDefinition);
				const auto & value = metadataSetNode -> getValue <MetadataDouble> (field .getName (), true) -> value ();
	
				field .clear ();
	
				for (const auto & v : value)
					field .emplace_back (v);
	
				break;
			}
			case X3D::X3DConstants::MFVec2d:
			{
				auto &       field = static_cast <MFVec2d &> (*fieldDefinition);
				const auto & value = metadataSetNode -> getValue <MetadataDouble> (field .getName (), true) -> value ();
	
				MFVec2d array;
	
				for (size_t i = 0, size = value .size (); i < size; i += std::tuple_size <Vector2d>::value)
					array .emplace_back (value .at (i), value .at (i + 1));
	
				field = array;
				break;
			}
			case X3D::X3DConstants::MFVec2f:
			{
				auto &       field = static_cast <MFVec2f &> (*fieldDefinition);
				const auto & value = metadataSetNode -> getValue <MetadataFloat> (field .getName (), true) -> value ();
	
				MFVec2f array;
	
				for (size_t i = 0, size = value .size (); i < size; i += std::tuple_size <Vector2f>::value)
					array .emplace_back (value .at (i), value .at (i + 1));
	
				field = array;
				break;
			}
			case X3D::X3DConstants::MFVec3d:
			{
				auto &       field = static_cast <MFVec3d &> (*fieldDefinition);
				const auto & value = metadataSetNode -> getValue <MetadataDouble> (field .getName (), true) -> value ();
	
				MFVec3d array;
	
				for (size_t i = 0, size = value .size (); i < size; i += std::tuple_size <Vector3d>::value)
					array .emplace_back (value .at (i), value .at (i + 1), value .at (i + 2));
	
				field = array;
				break;
			}
			case X3D::X3DConstants::MFVec3f:
			{
				auto &       field = static_cast <MFVec3f &> (*fieldDefinition);
				const auto & value = metadataSetNode -> getValue <MetadataFloat> (field .getName (), true) -> value ();
	
				MFVec3f array;
	
				for (size_t i = 0, size = value .size (); i < size; i += std::tuple_size <Vector3f>::value)
					array .emplace_back (value .at (i), value .at (i + 1), value .at (i + 2));
	
				field = array;
				break;
			}
			case X3D::X3DConstants::MFVec4d:
			{
				auto &       field = static_cast <MFVec4d &> (*fieldDefinition);
				const auto & value = metadataSetNode -> getValue <MetadataDouble> (field .getName (), true) -> value ();
	
				MFVec4d array;
	
				for (size_t i = 0, size = value .size (); i < size; i += std::tuple_size <Vector4d>::value)
					array .emplace_back (value .at (i), value .at (i + 1), value .at (i + 2), value .at (i + 3));
	
				field = array;
				break;
			}
			case X3D::X3DConstants::MFVec4f:
			{
				auto &       field = static_cast <MFVec4f &> (*fieldDefinition);
				const auto & value = metadataSetNode -> getValue <MetadataFloat> (field .getName (), true) -> value ();
	
				MFVec4f array;
	
				for (size_t i = 0, size = value .size (); i < size; i += std::tuple_size <Vector4f>::value)
					array .emplace_back (value .at (i), value .at (i + 1), value .at (i + 2), value .at (i + 3));
	
				field = array;
				break;
			}
		}
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
X3DNode::toMetaData (const X3DPtr <MetadataSet> & metadataSetNode) const
{
	metadataSetNode -> getValue <MetadataString> ("@typeName", false) -> value () = { getTypeName () };

	if (not getName () .empty ())
		metadataSetNode -> getValue <MetadataString> ("@name", false) -> value () = { getName () };

	// Output all fields, thus we can output non-standard nodes.

	for (const auto & fieldDefinition : getFieldDefinitions ())
		toMetaData (metadataSetNode, fieldDefinition);
}

void
X3DNode::toMetaData (const X3DPtr <MetadataSet> & metadataSetNode, const X3DFieldDefinition* const fieldDefinition) const
{
	switch (fieldDefinition -> getType ())
	{
		case X3D::X3DConstants::SFBool:
		{
			const auto & field    = static_cast <const SFBool &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> getValue <MetadataBoolean> (field .getName (), false);

			metadata -> value () = { field };
			break;
		}
		case X3D::X3DConstants::SFColor:
		{
			const auto & field    = static_cast <const SFColor &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> getValue <MetadataFloat> (field .getName (), false);

			metadata -> value () = { field .getRed   (),
			                         field .getGreen (),
			                         field .getBlue  () };

			break;
		}
		case X3D::X3DConstants::SFColorRGBA:
		{
			const auto & field    = static_cast <const SFColorRGBA &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> getValue <MetadataFloat> (field .getName (), false);

			metadata -> value () = { field .getRed   (),
			                         field .getGreen (),
			                         field .getBlue  (),
			                         field .getAlpha () };

			break;
		}
		case X3D::X3DConstants::SFDouble:
		{
			const auto & field    = static_cast <const SFDouble &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> getValue <MetadataDouble> (field .getName (), false);

			metadata -> value () = { field };
			break;
		}
		case X3D::X3DConstants::SFFloat:
		{
			const auto & field    = static_cast <const SFFloat &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> getValue <MetadataFloat> (field .getName (), false);

			metadata -> value () = { field };
			break;
		}
		case X3D::X3DConstants::SFInt32:
		{
			const auto & field    = static_cast <const SFInt32 &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> getValue <MetadataInteger> (field .getName (), false);

			metadata -> value () = { field };
			break;
		}
		case X3D::X3DConstants::SFImage:
		{
			const auto & field    = static_cast <const SFImage &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> getValue <MetadataInteger> (field .getName (), false);

			MFInt32 array ({ field .getWidth (),
			                 field .getHeight (),
			                 field .getComponents () });

			for (const auto & pixel : field .getArray ())
			   array .emplace_back (pixel);

			metadata -> value () = array;
			break;
		}
		case X3D::X3DConstants::SFMatrix3d:
		{
			const auto & field    = static_cast <const SFMatrix3d &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> getValue <MetadataDouble> (field .getName (), false);

			MFDouble array;

			for (size_t i = 0; i < std::tuple_size <Matrix3d>::value; ++ i)
			   array .emplace_back (field .get1Value (i));

			metadata -> value () = array;
			break;
		}
		case X3D::X3DConstants::SFMatrix3f:
		{
			const auto & field    = static_cast <const SFMatrix3f &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> getValue <MetadataFloat> (field .getName (), false);

			MFFloat array;

			for (size_t i = 0; i < std::tuple_size <Matrix3f>::value; ++ i)
			   array .emplace_back (field .get1Value (i));

			metadata -> value () = array;
			break;
		}
		case X3D::X3DConstants::SFMatrix4d:
		{
			const auto & field    = static_cast <const SFMatrix4d &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> getValue <MetadataDouble> (field .getName (), false);

			MFDouble array;

			for (size_t i = 0; i < std::tuple_size <Matrix4d>::value; ++ i)
			   array .emplace_back (field .get1Value (i));

			metadata -> value () = array;
			break;
		}
		case X3D::X3DConstants::SFMatrix4f:
		{
			const auto & field    = static_cast <const SFMatrix4f &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> getValue <MetadataFloat> (field .getName (), false);

			MFFloat array;

			for (size_t i = 0; i < std::tuple_size <Matrix4f>::value; ++ i)
			   array .emplace_back (field .get1Value (i));

			metadata -> value () = array;
			break;
		}
		case X3D::X3DConstants::SFNode:
		{
			const auto & field    = static_cast <const SFNode &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> getValue <MetadataSet> (field .getName (), false);
		
			metadata -> value () = { field };
			break;
		}
		case X3D::X3DConstants::SFRotation:
		{
			const auto & field    = static_cast <const SFRotation &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> getValue <MetadataDouble> (field .getName (), false);

			SFRotation::value_type x, y, z, angle;

			field .getValue (x, y, z, angle);

			metadata -> value () = { x, y, z, angle };
			break;
		}
		case X3D::X3DConstants::SFString:
		{
			const auto & field    = static_cast <const SFString &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> getValue <MetadataString> (field .getName (), false);

			metadata -> value () = { field };
			break;
		}
		case X3D::X3DConstants::SFTime:
		{
			const auto & field    = static_cast <const SFTime &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> getValue <MetadataDouble> (field .getName (), false);

			metadata -> value () = { field };
			break;
		}
		case X3D::X3DConstants::SFVec2d:
		{
			const auto & field    = static_cast <const SFVec2d &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> getValue <MetadataDouble> (field .getName (), false);

			metadata -> value () = { field .getX (),
			                         field .getY () };

			break;
		}
		case X3D::X3DConstants::SFVec2f:
		{
			const auto & field    = static_cast <const SFVec2f &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> getValue <MetadataFloat> (field .getName (), false);

			metadata -> value () = { field .getX (),
			                         field .getY () };

			break;
		}
		case X3D::X3DConstants::SFVec3d:
		{
			const auto & field    = static_cast <const SFVec3d &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> getValue <MetadataDouble> (field .getName (), false);

			metadata -> value () = { field .getX (),
			                         field .getY (),
			                         field .getZ () };

			break;
		}
		case X3D::X3DConstants::SFVec3f:
		{
			const auto & field    = static_cast <const SFVec3f &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> getValue <MetadataFloat> (field .getName (), false);

			metadata -> value () = { field .getX (),
			                         field .getY (),
			                         field .getZ () };

			break;
		}
		case X3D::X3DConstants::SFVec4d:
		{
			const auto & field    = static_cast <const SFVec4d &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> getValue <MetadataDouble> (field .getName (), false);

			metadata -> value () = { field .getX (),
			                         field .getY (),
			                         field .getZ (),
			                         field .getW () };

			break;
		}
		case X3D::X3DConstants::SFVec4f:
		{
			const auto & field    = static_cast <const SFVec4f &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> getValue <MetadataFloat> (field .getName (), false);

			metadata -> value () = { field .getX (),
			                         field .getY (),
			                         field .getZ (),
			                         field .getW () };

			break;
		}
		case X3D::X3DConstants::MFBool:
		{
			const auto & field    = static_cast <const MFBool &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> getValue <MetadataBoolean> (field .getName (), false);

			metadata -> value () = field;
			break;
		}
		case X3D::X3DConstants::MFColor:
		{
			const auto & field    = static_cast <const MFColor &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> getValue <MetadataFloat> (field .getName (), false);

			MFFloat & array = metadata -> value ();

			for (const auto & value : field)
			{
				array .emplace_back (value .getRed   ());
				array .emplace_back (value .getGreen ());
				array .emplace_back (value .getBlue  ());
			}

			break;
		}
		case X3D::X3DConstants::MFColorRGBA:
		{
			const auto & field    = static_cast <const MFColorRGBA &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> getValue <MetadataFloat> (field .getName (), false);

			MFFloat & array = metadata -> value ();

			for (const auto & value : field)
			{
				array .emplace_back (value .getRed   ());
				array .emplace_back (value .getGreen ());
				array .emplace_back (value .getBlue  ());
				array .emplace_back (value .getAlpha ());
			}

			break;
		}
		case X3D::X3DConstants::MFDouble:
		{
			const auto & field    = static_cast <const MFDouble &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> getValue <MetadataDouble> (field .getName (), false);

			metadata -> value () = field;
			break;
		}
		case X3D::X3DConstants::MFFloat:
		{
			const auto & field    = static_cast <const MFFloat &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> getValue <MetadataFloat> (field .getName (), false);

			metadata -> value () = field;
			break;
		}
		case X3D::X3DConstants::MFImage:
		{
			const auto & field    = static_cast <const MFImage &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> getValue <MetadataInteger> (field .getName (), false);

			MFInt32 & array = metadata -> value ();

			for (const auto & value : field)
			{
				array .emplace_back (value .getWidth ());
				array .emplace_back (value .getHeight ());
				array .emplace_back (value .getComponents ());

				for (const auto & pixel : value .getArray ())
				   array .emplace_back (pixel);
			}

			break;
		}
		case X3D::X3DConstants::MFInt32:
		{
			const auto & field    = static_cast <const MFInt32 &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> getValue <MetadataInteger> (field .getName (), false);

			metadata -> value () = field;
			break;
		}
		case X3D::X3DConstants::MFMatrix3d:
		{
			const auto & field    = static_cast <const MFMatrix3d &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> getValue <MetadataDouble> (field .getName (), false);

			MFDouble & array = metadata -> value ();

			for (const auto & value : field)
			{
				for (size_t i = 0; i < std::tuple_size <Matrix3d>::value; ++ i)
					array .emplace_back (value .get1Value (i));
			}

			break;
		}
		case X3D::X3DConstants::MFMatrix3f:
		{
			const auto & field    = static_cast <const MFMatrix3f &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> getValue <MetadataFloat> (field .getName (), false);

			MFFloat & array = metadata -> value ();

			for (const auto & value : field)
			{
				for (size_t i = 0; i < std::tuple_size <Matrix3f>::value; ++ i)
					array .emplace_back (value .get1Value (i));
			}

			break;
		}
		case X3D::X3DConstants::MFMatrix4d:
		{
			const auto & field    = static_cast <const MFMatrix4d &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> getValue <MetadataDouble> (field .getName (), false);

			MFDouble & array = metadata -> value ();

			for (const auto & value : field)
			{
				for (size_t i = 0; i < std::tuple_size <Matrix4d>::value; ++ i)
					array .emplace_back (value .get1Value (i));
			}

			break;
		}
		case X3D::X3DConstants::MFMatrix4f:
		{
			const auto & field    = static_cast <const MFMatrix4f &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> getValue <MetadataFloat> (field .getName (), false);

			MFFloat & array = metadata -> value ();

			for (const auto & value : field)
			{
				for (size_t i = 0; i < std::tuple_size <Matrix4f>::value; ++ i)
					array .emplace_back (value .get1Value (i));
			}

			break;
		}
		case X3D::X3DConstants::MFNode:
		{
			const auto & field    = static_cast <const MFNode &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> getValue <MetadataSet> (field .getName (), false);
		
			metadata -> value () = field;
			break;
		}
		case X3D::X3DConstants::MFRotation:
		{
			const auto & field    = static_cast <const MFRotation &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> getValue <MetadataDouble> (field .getName (), false);

			MFDouble & array = metadata -> value ();

			for (const auto & value : field)
			{
				SFRotation::value_type x, y, z, angle;

				value .getValue (x, y, z, angle);

				array .emplace_back (x);
				array .emplace_back (y);
				array .emplace_back (z);
				array .emplace_back (angle);
			}

			break;
		}
		case X3D::X3DConstants::MFString:
		{
			const auto & field    = static_cast <const MFString &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> getValue <MetadataString> (field .getName (), false);

			metadata -> value () = field;
			break;
		}
		case X3D::X3DConstants::MFTime:
		{
			const auto & field    = static_cast <const MFTime &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> getValue <MetadataDouble> (field .getName (), false);

			metadata -> value () .assign (field .begin (), field .end ());
			break;
		}
		case X3D::X3DConstants::MFVec2d:
		{
			const auto & field    = static_cast <const MFVec2d &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> getValue <MetadataDouble> (field .getName (), false);

			MFDouble & array = metadata -> value ();

			for (const auto & value : field)
			{
				array .emplace_back (value .getX ());
				array .emplace_back (value .getY ());
			}

			break;
		}
		case X3D::X3DConstants::MFVec2f:
		{
			const auto & field    = static_cast <const MFVec2f &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> getValue <MetadataFloat> (field .getName (), false);

			MFFloat & array = metadata -> value ();

			for (const auto & value : field)
			{
				array .emplace_back (value .getX ());
				array .emplace_back (value .getY ());
			}

			break;
		}
		case X3D::X3DConstants::MFVec3d:
		{
			const auto & field    = static_cast <const MFVec3d &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> getValue <MetadataDouble> (field .getName (), false);

			MFDouble & array = metadata -> value ();

			for (const auto & value : field)
			{
				array .emplace_back (value .getX ());
				array .emplace_back (value .getY ());
				array .emplace_back (value .getZ ());
			}

			break;
		}
		case X3D::X3DConstants::MFVec3f:
		{
			const auto & field    = static_cast <const MFVec3f &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> getValue <MetadataFloat> (field .getName (), false);

			MFFloat & array = metadata -> value ();

			for (const auto & value : field)
			{
				array .emplace_back (value .getX ());
				array .emplace_back (value .getY ());
				array .emplace_back (value .getZ ());
			}

			break;
		}
		case X3D::X3DConstants::MFVec4d:
		{
			const auto & field    = static_cast <const MFVec4d &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> getValue <MetadataDouble> (field .getName (), false);

			MFDouble & array = metadata -> value ();

			for (const auto & value : field)
			{
				array .emplace_back (value .getX ());
				array .emplace_back (value .getY ());
				array .emplace_back (value .getZ ());
				array .emplace_back (value .getW ());
			}

			break;
		}
		case X3D::X3DConstants::MFVec4f:
		{
			const auto & field    = static_cast <const MFVec4f &> (*fieldDefinition);
			const auto   metadata = metadataSetNode -> getValue <MetadataFloat> (field .getName (), false);

			MFFloat & array = metadata -> value ();

			for (const auto & value : field)
			{
				array .emplace_back (value .getX ());
				array .emplace_back (value .getY ());
				array .emplace_back (value .getZ ());
				array .emplace_back (value .getW ());
			}

			break;
		}
	}
}

/*
 * Misc
 */

std::vector <X3DLayerNode*>
X3DNode::getLayers () const
{
	return findParents <X3DLayerNode> (this, TRAVERSE_VISIBLE_NODES);
}

} // X3D
} // titania
