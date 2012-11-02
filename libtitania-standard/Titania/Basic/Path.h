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

#ifndef __TITANIA_BASIC_URI_H__
#define __TITANIA_BASIC_URI_H__

#include <map>
#include <sstream>
#include <stdexcept>
#include <string>

namespace titania {
namespace basic {

template <class StringT>
class basic_uri;

template <class StringT, class Traits>
std::basic_ostream <typename StringT::value_type, Traits> &
operator << (std::basic_ostream <typename StringT::value_type, Traits> &, const basic_uri <StringT> &);

/**
 *  Template to URI adresses.
 *  http://de.wikipedia.org/wiki/Uniform_Resource_Identifier:
 *
 *
 *  URI = scheme ":" hierarchy part [ "?" query ] [ "#" fragment ]
 *
 *
 *   foo://example.com:8042/over/there?name=ferret#nose
 *   \_/   \______________/\_________/ \_________/ \__/
 *    |           |            |            |        |
 *  scheme   authority       path        query   fragment
 *    |   _____________________|__
 *   / \ /                        \
 *   urn:example:animal:ferret:nose
 *
 *
 *  * * * * * * * * * * * * * * * *
 *  *             URI             *
 *  *    *********************    *
 *  *    * URL   * *   URN   *    *
 *  *    *********************    *
 *  * * * * * * * * * * * * * * * *
 *
 *
 *  Universal Resource Identifiers (URI)
 *  http://www.ietf.org/rfc/rfc1630.txt
 *
 *  Uniform Resource Locators (URL)
 *  http://www.ietf.org/rfc/rfc1738.txt
 *
 *  Uniform Resource Name (URN)
 *  http://www.ietf.org/rfc/rfc2141.txt
 */
template <class StringT>
class basic_uri
{
public:

	typedef std::invalid_argument        parse_error;
	typedef StringT                      string_type;
	typedef typename StringT::value_type char_type;
	typedef size_t                       size_type;

	struct Value
	{
		bool local;
		bool absolute;

		string_type scheme;
		string_type slashs;
		string_type authority;
		string_type host;
		size_type port;
		string_type path;
		string_type query;
		string_type fragment;

		string_type string;
	};

	typedef Value value_type;

	///  @name Constructors

	///  Default constructor.
	///  This constructs an URI with the empty URI ''.
	constexpr
	basic_uri () :
		value ({ true, false, string_type (), string_type (), string_type (), string_type (), 0 })
	{ }

	///  Copy constructor.
	constexpr
	basic_uri (const basic_uri & uri) :
		value (uri .value)
	{ }

	///  Move constructor.
	constexpr
	basic_uri (basic_uri && uri) :
		value (std::move (uri .value))
	{ }

	///  Construct a URI from @a string.
	basic_uri (const string_type &)
	throw (parse_error);

	///  Construct a URI from @a string.
	basic_uri (const char_type*)
	throw (parse_error);

	///  Construct a URI from @a uri and @a base uri.
	basic_uri (const basic_uri &, const basic_uri &);

	///  @name Assignment operator

	///  Assign @a uri to this URI.
	basic_uri &
	operator = (const basic_uri &);

	///  Assign @a uri to this URI by moving the contents.
	basic_uri &
	operator = (basic_uri &&);

	///  Assign @a uri to this URI.
	basic_uri &
	operator = (const string_type &);

	///  Assign @a uri to this URI.
	basic_uri &
	operator = (const char_type*);

	/// @name Capacity

	///  Returns true if this uri is the empty uri ''.
	constexpr bool
	empty () const
	{ return value .string .empty (); }

	constexpr size_type
	length () const
	{ return value .string .length (); }

	constexpr size_type
	size () const
	{ return value .string .size (); }

	/// @name Filename Test

	///  Returns true if this uri is relative otherwise false.
	constexpr bool
	is_relative () const
	{ return not is_absolute (); }

	///  Returns true if this uri is an absolute uri otherwise false.
	constexpr bool
	is_absolute () const
	{ return value .absolute; }

	///  Returns true if this uri is local,
	//// i.e. it has a 'file:' scheme or no scheme and no authority, otherwise false.
	constexpr bool
	is_local () const
	{ return value .local; }

	///  Returns true if this uri is a network address, i.e. if it is not local.
	constexpr bool
	is_network () const
	{ return not is_local (); }

	///  Returns true if this uri looks like a directory, i.e. it ends with a '/', otherwise false.
	constexpr bool
	is_directory () const
	{ return path () .length () and path () .back () == '/'; }

	///  Returns true if this uri looks like a file, i.e. it is not a directory, otherwise false.
	constexpr bool
	is_file () const
	{ return not is_directory (); }

	/// @name Element access

	///  Returns the hierarchy part of this URI.
	string_type
	hierarchy () const;

	///  Returns the authority of this URI.
	const string_type &
	authority () const
	{ return value .authority; }

	///  Returns the scheme of this URI.
	const string_type &
	scheme () const
	{ return value .scheme; }

	///  Returns the host of this URI.
	const string_type &
	host () const
	{ return value .host; }

	///  Returns the port of this URI.
	const size_type &
	port () const
	{ return value .port; }

	///  Returns the well_known_port of this URI.
	size_type
	well_known_port () const;

	///  Returns the path of this URI.
	const string_type &
	path () const
	{ return value .path; }

	///  Returns the query of this URI.
	const string_type &
	query () const
	{ return value .query; }

	///  Returns the fragment of this URI.
	const string_type &
	fragment () const
	{ return value .fragment; }

	///  Returns the string representation of this URI.
	const string_type &
	str () const
	{ return value .string; }

	///  Returns the string representation of this URI.
	operator const string_type & () const
	{ return value .string; }

	/// @name Path Operations

	///  Returns the root directory of this URI.
	constexpr basic_uri
	root () const
	{
		return basic_uri ({ is_local (),
		                    is_absolute (),
		                    scheme (),
		                    value .slashs,
		                    authority (),
		                    host (),
		                    port (),
		                    is_local () ? string_type (1, Signs::Slash) : string_type () });
	}

	///  Returns the base directory of this URI.
	basic_uri
	base () const;

	///  Returns the parent directory of this URI.
	constexpr basic_uri
	parent () const
	{
		return basic_uri ({ is_local (),
		                    is_absolute (),
		                    scheme (),
		                    value .slashs,
		                    authority (),
		                    host (),
		                    port (),
		                    path () .substr (0, path (). rfind (Signs::Slash) + 1) });
	}

	///  Transforms @a reference to this URI's base.
	basic_uri
	transform (const basic_uri &) const;

	///  Returns a relative path, relative from base path.
	string_type
	relative_path (const basic_uri &) const;
	//@}

	/// @name Filename Operations

	//@{
	///  Returns the full filename of this URI.
	string_type
	filename () const;

	///  Returns the basename of this URI's filename.
	string_type
	basename (const string_type &) const;

	///  Returns the basename of this URI's filename stript of @a list of suffixes.
	string_type
	basename (std::initializer_list <string_type>) const;

	///  Returns the suffix of this URI's filename.
	string_type
	suffix () const;
	//@}


private:

	///  Component constructor.
	basic_uri (Value && value);

	///  Composes a string from all values.
	string_type
	to_string () const;

	class parser
	{
	public:

		typedef typename string_type::size_type size_type;

		static
		void
		parse (basic_uri & uri, const string_type & string, size_type first = 0);


	private:

		constexpr
		parser (basic_uri & uri, const string_type & string) :
			uri (uri),
			string (string)
		{ }

		void
		uriString (size_type first) const;

		size_type
		scheme (size_type) const;

		size_type
		authority (size_type) const;

		void
		host (const string_type &, size_type, size_type) const;

		void
		port (string_type &, size_type, size_type) const;

		void
		tidyUpAuthority (const string_type &, string_type &) const;

		void
		path (size_type) const;

		void
		query (size_type) const;

		void
		fragment (size_type) const;

		basic_uri &         uri;
		const string_type & string;
	};

	struct Signs
	{
		static const char_type Zero;
		static const char_type Colon;
		static const char_type Slash;
		static const char_type QuestionMark;
		static const char_type NumberSign;
		static const char_type Dot;
		static const char_type DotSlash [2];
		static const char_type SlashQuestionNumber [3];
		static const char_type QuestionNumber [2];
	};

	static const string_type                 FileSchemeId;
	static std::map <string_type, size_type> well_known_ports;

	Value value;

};

template <class StringT>
const StringT basic_uri <StringT>::FileSchemeId = "file";

template <class StringT>
const typename StringT::value_type basic_uri <StringT>::Signs::Zero = '0';
template <class StringT>
const typename StringT::value_type basic_uri <StringT>::Signs::Colon = ':';
template <class StringT>
const typename StringT::value_type basic_uri <StringT>::Signs::Slash = '/';
template <class StringT>
const typename StringT::value_type basic_uri <StringT>::Signs::QuestionMark = '?';
template <class StringT>
const typename StringT::value_type basic_uri <StringT>::Signs::NumberSign = '#';
template <class StringT>
const typename StringT::value_type basic_uri <StringT>::Signs::Dot = '.';
template <class StringT>
const typename StringT::value_type basic_uri <StringT>::Signs::DotSlash [2] = { basic_uri <StringT>::Signs::Dot,
	                                                                             basic_uri <StringT>::Signs::Slash };
template <class StringT>
const typename StringT::value_type basic_uri <StringT>::Signs::SlashQuestionNumber [3] = { basic_uri <StringT>::Signs::Slash,
	                                                                                        basic_uri <StringT>::Signs::QuestionMark,
	                                                                                        basic_uri <StringT>::Signs::NumberSign };
template <class StringT>
const typename StringT::value_type basic_uri <StringT>::Signs::QuestionNumber [2] = { basic_uri <StringT>::Signs::QuestionMark,
	                                                                                   basic_uri <StringT>::Signs::NumberSign };

template <class StringT>
std::map <typename basic_uri <StringT>::string_type, typename basic_uri <StringT>::size_type> basic_uri <StringT>::well_known_ports = {
	std::make_pair ("ftp",    21),
	std::make_pair ("http",   80),
	std::make_pair ("https", 443),
	std::make_pair ("ftps",  990)

};

template <class StringT>
basic_uri <StringT>::basic_uri (Value && value) :
	value (value)
{
	if (value .string .empty ())
		this -> value .string = std::move (to_string ());
}

template <class StringT>
basic_uri <StringT>::basic_uri (const string_type & string)
throw (parse_error) :
	value ()
{
	parser::parse (*this, string);
}

template <class StringT>
basic_uri <StringT>::basic_uri (const char_type* string)
throw (parse_error) :
	value ()
{
	parser::parse (*this, string);
}

template <class StringT>
basic_uri <StringT>::basic_uri (const basic_uri & base, const basic_uri & uri)
{
	*this = std::move (base .transform (uri));
}

template <class StringT>
typename basic_uri <StringT>::basic_uri &
basic_uri <StringT>::operator = (const basic_uri & uri)
{
	value = uri .value;
	return *this;
}

template <class StringT>
typename basic_uri <StringT>::basic_uri &
basic_uri <StringT>::operator = (basic_uri && uri)
{
	value = std::move (uri .value);
	return *this;
}

template <class StringT>
inline
typename basic_uri <StringT>::basic_uri &
basic_uri <StringT>::operator = (const string_type & string)
{
	return *this = std::move (basic_uri (string));
}

template <class StringT>
inline
typename basic_uri <StringT>::basic_uri &
basic_uri <StringT>::operator = (const char_type* string)
{
	return *this = std::move (basic_uri (string));
}

template <class StringT>
typename basic_uri <StringT>::string_type
basic_uri <StringT>::hierarchy () const
{
	string_type hierarchy;

	hierarchy += value .slashs;
	hierarchy += authority ();
	hierarchy += path ();

	return hierarchy;
}

template <class StringT>
typename basic_uri <StringT>::size_type
basic_uri <StringT>::well_known_port () const
{
	auto well_known_port = well_known_ports .find (scheme ());

	if (well_known_port not_eq well_known_ports .end ())
		return well_known_port -> second;

	return 0;
}

template <class StringT>
basic_uri <StringT>
basic_uri <StringT>::base () const
{
	return is_directory ()
	       ? basic_uri ({ is_local (),
	                      is_absolute (),
	                      scheme (),
	                      value .slashs,
	                      authority (),
	                      host (),
	                      port (),
	                      path () })
			 : parent ();
}

template <class StringT>
basic_uri <StringT>
basic_uri <StringT>::transform (const basic_uri & reference) const
{
	bool T_local;
	bool T_absolute;

	string_type T_scheme;
	string_type T_slashs;
	string_type T_authority;
	string_type T_host;
	size_type   T_port;
	string_type T_path;
	string_type T_query;
	string_type T_fragment;

	if (reference .scheme () .size ())
	{
		T_local     = reference .is_local ();
		T_absolute  = reference .is_absolute ();
		T_scheme    = reference .scheme ();
		T_slashs    = reference .value .slashs;
		T_authority = reference .authority ();
		T_host      = reference .host ();
		T_port      = reference .port ();
		T_path      = reference .path ();    // remove_dot_segments(reference .path);
		T_query     = reference .query ();
	}
	else
	{
		if (reference .authority () .size ())
		{
			T_local     = reference .is_local ();
			T_absolute  = reference .is_absolute ();
			T_authority = reference .authority ();
			T_host      = reference .host ();
			T_port      = reference .port ();
			T_path      = reference .path (); // remove_doT_segments(reference .path);
			T_query     = reference .query ();
		}
		else
		{
			if (reference .path () .empty ())
			{
				T_path = path ();

				if (reference .query () .size ())
					T_query = reference .query ();
				else
					T_query = query ();
			}
			else
			{
				if (reference .path () .front () == Signs::Slash)
				{
					T_path = reference .path (); // remove_dot_segments (reference .path ());
				}
				else
				{
					// merge (Base .path (), reference .path ());

					basic_uri Base = base ();

					if (Base .path () .empty ())
						T_path = Signs::Slash;
					else
						T_path += Base .path ();

					T_path += reference .path ();

					// T_path = remove_dot_segments (T_path);
				}

				T_query = reference .query ();
			}

			T_local     = is_local ();
			T_absolute  = is_absolute ();
			T_authority = authority ();
			T_host      = host ();
			T_port      = port ();
		}

		T_scheme = scheme ();
		T_slashs = value .slashs;
	}

	T_fragment = reference .fragment ();

	return basic_uri ({ T_local,
	                    T_absolute,
	                    T_scheme,
	                    T_slashs,
	                    T_authority,
	                    T_host,
	                    T_port,
	                    T_path,
	                    T_query,
	                    T_fragment });
}

template <class StringT>
typename basic_uri <StringT>::string_type
basic_uri <StringT>::relative_path (const basic_uri & descendant) const
{
	//		path = array ();
	//
	//		basic_uri baseURI = base ();
	//
	//		if (baseURI .scheme () not_eq descendant .scheme ())
	//			return "";
	//
	//		if (baseURI .host () not_eq descendant .host ())
	//			return "";
	//
	//		baseURI       = explode ('/', baseURI       -> path ());
	//
	//		baseURI .pop_back ();
	//
	//		descendant = explode ('/', descendant -> path ());
	//
	//		size_type i; j;
	//
	//		for (i = 0; i < count (baseURI) and i < count (descendant); ++ i)
	//		{
	//			if (baseURI [i] not_eq descendant [i])
	//				break;
	//		}
	//
	//		for (j = i; j < count (baseURI); ++ j)
	//			array_push (path, '..');
	//
	//		for (j = i; j < count (descendant); ++ j)
	//			array_push (path, descendant [j]);
	//
	//		path = implode ('/', path);
	//
	//		return path ? path : './';
	//
	return string_type ();
}

// Filename operations

template <class StringT>
typename basic_uri <StringT>::string_type
basic_uri <StringT>::filename () const
{
	if (path () .length () and is_file ())
		return path () .substr (path (). rfind (Signs::Slash) + 1);

	return string_type ();
}

template <class StringT>
typename basic_uri <StringT>::string_type
basic_uri <StringT>::basename (const string_type & suffix) const
{
	if (path () .length () and is_file () and path () .substr (path () .size () - suffix .length (), suffix .length ()) == suffix)
	{
		string_type basename = filename ();
		return basename .substr (0, basename .size () - suffix .length ());
	}

	return filename ();
}

template <class StringT>
typename basic_uri <StringT>::string_type
basic_uri <StringT>::basename (std::initializer_list <string_type> suffixes) const
{
	for (const auto & suffix : suffixes)
	{
		if (path () .length () and is_file () and path () .substr (path () .size () - suffix .length (), suffix .length ()) == suffix)
		{
			string_type basename = filename ();
			return basename .substr (0, basename .size () - suffix .length ());
		}
	}

	return filename ();
}

template <class StringT>
typename basic_uri <StringT>::string_type
basic_uri <StringT>::suffix () const
{
	typename string_type::size_type dot = path () .rfind (Signs::DotSlash, 2);

	if (dot not_eq string_type::npos and path () [dot] == Signs::Dot)
		return path () .substr (dot + 1);

	return string_type ();
}

// Private Funtions

template <class StringT>
typename basic_uri <StringT>::string_type
basic_uri <StringT>::to_string () const
{
	string_type string;

	string += scheme ();

	if (scheme () .length ())
		string += Signs::Colon;

	string += hierarchy ();

	if (query () .length ())
	{
		string += Signs::QuestionMark
		          + query ();
	}

	if (fragment () .length ())
	{
		string += Signs::NumberSign
		          + fragment ();
	}

	return string;
}

// Parser

template <class StringT>
void
basic_uri <StringT>::parser::parse (basic_uri & uri, const string_type &string, size_type first)
{
	if (string .size ())
	{
		uri .value .string = string;
		parser (uri, string) .uriString (first);
	}
}

template <class StringT>
void
basic_uri <StringT>::parser::uriString (size_type first) const
{
	first = scheme (first);

	// Network path

	if (string [first] == Signs::Colon)
	{
		++ first;
		uri .value .absolute = true;
	}

	size_type begin = first;

	if (string [begin ++] == Signs::Slash)
	{
		if (string [begin ++] == Signs::Slash)
		{
			uri .value .absolute = true;
			uri .value .slashs   = string_type (2, Signs::Slash);
			first                = authority (begin);
		}
	}

	switch (string [first])
	{
		case 0:
			break;
		case Signs::Slash:
			path (first);
			uri .value .absolute = true;
			break;
		case Signs::QuestionMark:
			query (first + 1);
			break;
		case Signs::NumberSign:
			fragment (first + 1);
			break;
		default:
			path (first);
			break;
	}

	uri .value .local = uri .value .scheme == FileSchemeId
	                    or (not uri .value .scheme .length () and not uri .value .authority .length ());
}

template <class StringT>
typename StringT::size_type
basic_uri <StringT>::parser::scheme (size_type first) const
{
	if (std::isalpha (string [0]))
	{
		size_type last = string .find (Signs::Colon, first);

		if (last == string_type::npos)
			return first;

		uri .value .scheme = std::move (string .substr (first, last - first));

		return last;
	}

	return first;
}

template <class StringT>
typename StringT::size_type
basic_uri <StringT>::parser::authority (size_type first) const
{
	size_type last = string .find_first_of (Signs::SlashQuestionNumber, first, 3);

	// Return if no authority.
	if (first == last)
		return last;

	if (last == string_type::npos)
		last = string .length ();

	string_type authority = std::move (string .substr (first, last - first));

	size_type colon = authority .find (Signs::Colon);

	if (colon not_eq string_type::npos)
	{
		host (authority, 0, colon);
		port (authority, colon + 1, authority .length ());
	}
	else
		uri .value .host = authority;

	uri .value .authority = std::move (authority);

	return last;
}

template <class StringT>
void
basic_uri <StringT>::parser::host (const string_type &authority, size_type first, size_type last) const
{
	uri .value .host = std::move (authority .substr (first, last - first));
}

template <class StringT>
void
basic_uri <StringT>::parser::port (string_type & authority, size_type first, size_type last) const
{
	size_type pos = first;

	string_type portString = std::move (authority .substr (first));

	try
	{
		uri .value .port = std::stoul (portString, &pos);
	}
	catch (...)
	{
		if (pos not_eq last - first)
			throw parse_error ("basic_uri parse error: Invalid port number.");
	}

	tidyUpAuthority (portString, authority);
}

template <class StringT>
void
basic_uri <StringT>::parser::tidyUpAuthority (const string_type &portString, string_type & authority) const
{
	// Tidy up authority. Remove leading zeros.
	size_type zero = 0;

	while (portString [zero ++] == Signs::Zero and zero not_eq portString .length ())
		;

	if (zero not_eq string_type::npos)
		authority = std::move (uri .value .host + Signs::Colon + portString .substr (zero - 1));
}

template <class StringT>
void
basic_uri <StringT>::parser::path (size_type first) const
{
	size_type last = string .find_first_of (Signs::QuestionNumber, first, 2);

	if (last == string_type::npos)
		last = string .length ();

	uri .value .path = std::move (string .substr (first, last - first));

	switch (string [last])
	{
		case Signs::QuestionMark:
			query (last + 1);
			break;
		case Signs::NumberSign:
			fragment (last + 1);
			break;
	}
}

template <class StringT>
void
basic_uri <StringT>::parser::query (size_type first) const
{
	size_type last = string .find (Signs::NumberSign, first);

	if (last == string_type::npos)
		last = string .length ();

	uri .value .query = std::move (string .substr (first, last - first));

	if (string [last] == Signs::NumberSign)
		fragment (last + 1);
}

template <class StringT>
inline
void
basic_uri <StringT>::parser::fragment (size_type first) const
{
	uri .value .fragment = std::move (string .substr (first));
}

///  @relates basic_uri
///  @name Comparision operations

///@{
//@{
///  Compares two basic_uri's.
///  Return true if @a a is equal to @a b.
template <class StringT>
constexpr bool
operator == (const basic_uri <StringT> & lhs, const basic_uri <StringT> & rhs)
{
	return lhs .str () == rhs .str ();
}

///  Compares two basic_uri's.
///  Return false if URI @a a is not equal to @a b.
template <class StringT>
inline
constexpr bool
operator not_eq (const basic_uri <StringT> & lhs, const basic_uri <StringT> & rhs)
{
	return lhs .str () not_eq rhs .str ();
}

///  Compares two basic_uri's.
///  Return false if URI @a a is not equal to @a b.
template <class StringT>
inline
constexpr bool
operator == (const basic_uri <StringT> & lhs, const StringT & rhs)
{
	return lhs .str () == rhs;
}

///  Compares two basic_uri's.
///  Return false if URI @a a is not equal to @a b.
template <class StringT>
inline
constexpr bool
operator == (const StringT & lhs, const basic_uri <StringT> & rhs)
{
	return lhs == rhs .str ();
}

///  Compares two basic_uri's.
///  Return false if URI @a a is not equal to @a b.
template <class StringT>
inline
constexpr bool
operator not_eq (const basic_uri <StringT> & lhs, const StringT & rhs)
{
	return not (lhs == rhs);
}

///  Compares two basic_uri's.
///  Return false if URI @a a is not equal to @a b.
template <class StringT>
inline
constexpr bool
operator not_eq (const StringT & lhs, const basic_uri <StringT> & rhs)
{
	return not (lhs == rhs);
}

///  Compares two basic_uri's.
///  Return false if URI @a a is not equal to @a b.
template <class StringT>
inline
constexpr bool
operator == (const basic_uri <StringT> & lhs, const typename StringT::value_type* rhs)
{
	return lhs .str () == rhs;
}

///  Compares two basic_uri's.
///  Return false if URI @a a is not equal to @a b.
template <class StringT>
inline
constexpr bool
operator == (const typename StringT::value_type* lhs, const basic_uri <StringT> & rhs)
{
	return lhs == rhs .str ();
}

///  Compares two basic_uri's.
///  Return false if URI @a a is not equal to @a b.
template <class StringT>
inline
constexpr bool
operator not_eq (const basic_uri <StringT> & lhs, const typename StringT::value_type* rhs)
{
	return not (lhs == rhs);
}

///  Compares two basic_uri's.
///  Return false if URI @a a is not equal to @a b.
template <class StringT>
inline
constexpr bool
operator not_eq (const typename StringT::value_type* lhs, const basic_uri <StringT> & rhs)
{
	return not (lhs == rhs);
}
//@}

//@{
///  Compares two basic_uri's.
///  Return true if URI @a a is less than @a b.
template <class StringT>
constexpr bool
operator < (const basic_uri <StringT> & lhs, const basic_uri <StringT> & rhs)
{
	return lhs .str () < rhs .str ();
}

///  Compares two basic_uri's.
///  Return false if URI @a a is greater than @a b.
template <class StringT>
inline
constexpr bool
operator > (const basic_uri <StringT> & lhs, const basic_uri <StringT> & rhs)
{
	return lhs .str () > rhs .str ();
}
//@}
///@}

///  @relates basic_uri
///  @name String concatanation

///@{
//@{
///  Compares two basic_uri's.
///  Returns a string containing characters from @a uri followed by the characters from @a string.
template <class StringT>
inline
constexpr StringT
operator + (const basic_uri <StringT> & uri, const StringT & string)
{
	return uri .str () + string;
}

///  Returns a string containing characters from @a string followed by the characters from @a uri.
template <class StringT>
inline
constexpr StringT
operator + (const StringT & string, const basic_uri <StringT> & uri)
{
	return string + uri .str ();
}

///  Returns a string containing characters from @a uri followed by the characters from @a string.
template <class StringT>
inline
constexpr StringT
operator + (const basic_uri <StringT> & uri, const typename StringT::value_type* string)
{
	return uri .str () + string;
}

///  Returns a string containing characters from @a string followed by the characters from @a uri.
template <class StringT>
inline
constexpr StringT
operator + (const typename StringT::value_type* string, const basic_uri <StringT> & uri)
{
	return string + uri .str ();
}
//@}
///@}

///  @relates basic_uri
///  @name Input/Output operations

///@{
//@{
///  Insertion operator for URI values.
template <class StringT, class Traits>
inline
std::basic_ostream <typename StringT::value_type, Traits> &
operator << (std::basic_ostream <typename StringT::value_type, Traits> & ostream, const basic_uri <StringT> & uri)
{
	return ostream << uri .str ();
}
//@}
///@}

///  @relates basic_uri

///@{
//@{

typedef basic_uri <std::string>  uri;
typedef basic_uri <std::wstring> wuri;

//
extern template class basic_uri <std::string>;
extern template class basic_uri <std::wstring>;

//
extern template std::ostream & operator << (std::ostream &, const uri &);
extern template std::wostream & operator << (std::wostream &, const wuri &);
//@}
///@}

} // basic
} // titania

#endif
