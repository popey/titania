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

#include "Parser.h"

#include "../Execution/jsExecutionContext.h"
#include "../Expressions.h"
#include "../Objects.h"
#include "../Parser/Grammar.h"
#include "../Primitives.h"

#include <Titania/Backtrace.h>
#include <Titania/Math/Utility/strtol.h>

namespace titania {
namespace pb {

Parser::Parser (jsExecutionContext* const executionContext, std::istream & istream) :
	             rootContext (executionContext),
	       executionContexts ({ executionContext }),
	                 istream (istream),
	             whiteSpaces (),
	       commentCharacters (),
	isLeftHandSideExressions ()
{ }

void
Parser::parseIntoContext ()
throw (SyntaxError)
{
	//__LOG__ << std::endl;

	istream .imbue (std::locale::classic ());

	program ();
}

void
Parser::setState (const State & value)
{
	istream .clear (std::get <0> (value));
	istream .seekg (std::get <1> (value) - istream .tellg (), std::ios_base::cur);
}

Parser::State
Parser::getState ()
{
	return State (istream .rdstate (), istream .tellg ());
}

// A.1 Lexical Grammar

void
Parser::comments ()
{
	//__LOG__ << this << " " << std::endl;

	while (comment ())
		;
}

bool
Parser::comment ()
{
	//__LOG__ << this << " " << std::endl;

	Grammar::WhiteSpaces (istream, whiteSpaces);

	if (Grammar::MultiLineComment (istream, commentCharacters))
		return true;

	if (Grammar::SingleLineComment (istream, commentCharacters))
		return true;

	return false;
}

bool
Parser::identifier (std::string & identifierCharacters)
{
	//__LOG__ << std::endl;

	const auto state = getState ();

	comments ();

	if (identifierName (identifierCharacters))
	{
		if (not reservedWord (identifierCharacters))
		{
			return true;
		}

		setState (state);
	}

	return false;
}

bool
Parser::identifierName (std::string & identifierNameCharacters)
{
	//__LOG__ << std::endl;

	if (identifierStart (identifierNameCharacters))
	{
		while (identifierStart (identifierNameCharacters) or identifierPart (identifierNameCharacters))
			;

		return true;
	}

	return false;
}

bool
Parser::identifierStart (std::string & identifierStartCharacters)
{
	//__LOG__ << std::endl;

	// ...

	static const io::sequence IdentifierStart ("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ$_");

	return IdentifierStart (istream, identifierStartCharacters);
}

bool
Parser::identifierPart (std::string & identifierPartCharacters)
{
	//__LOG__ << std::endl;

	static const io::sequence UnicodeDigit ("1234567890");

	// ...

	const auto size = identifierPartCharacters .size ();

	while (identifierStart (identifierPartCharacters) or UnicodeDigit (istream, identifierPartCharacters))
		;

	return identifierPartCharacters .size () not_eq size;
}

bool
Parser::reservedWord (const std::string & string)
{
	//__LOG__ << std::endl;

	if (Grammar::Keyword .count (string))
		return true;

	if (Grammar::FutureReservedWord .count (string))
		return true;

	static const std::set <std::string> primitives = {
		Grammar::null (),
		Grammar::_true (),
		Grammar::_false ()
	};

	if (primitives .count (string))
		return true;

	return false;
}

bool
Parser::literal (var & value)
{
	if (nullLiteral (value))
		return true;

	if (booleanLiteral (value))
		return true;

	if (numericLiteral (value))
		return true;

	if (stringLiteral (value))
		return true;

	//if (regularExpressionLiteral ())
	//	return true;

	return false;
}

bool
Parser::nullLiteral (var & value)
{
	//__LOG__ << std::endl;

	comments ();

	if (Grammar::null (istream))
	{
		value = get_null ();
		return true;
	}

	return false;
}

bool
Parser::booleanLiteral (var & value)
{
	//__LOG__ << std::endl;

	comments ();

	if (Grammar::_true (istream))
	{
		value = get_true ();
		return true;
	}

	if (Grammar::_false (istream))
	{
		value = get_false ();
		return true;
	}

	return false;
}

bool
Parser::numericLiteral (var & value)
{
	//__LOG__ << std::endl;

	if (binaryIntegerLiteral (value))
		return true;

	if (octalIntegerLiteral (value))
		return true;

	if (hexIntegerLiteral (value))
		return true;

	if (decimalLiteral (value))
		return true;

	return false;
}

bool
Parser::decimalLiteral (var & value)
{
	//__LOG__ << std::endl;

	const auto state = getState ();

	comments ();

	double number = 0;

	if (istream >> std::dec >> number)
	{
		value .reset (new Number (number));
		return true;
	}

	setState (state);

	return false;
}

bool
Parser::binaryIntegerLiteral (var & value)
{
	//__LOG__ << std::endl;

	comments ();

	if (Grammar::bin (istream) or Grammar::BIN (istream))
	{
		std::string digits;

		if (Grammar::BinaryDigits (istream, digits))
		{
			value .reset (new Number (math::strtoul (digits .c_str (), 2)));
			return true;
		}

		istream .clear ();
	}

	return false;
}

bool
Parser::octalIntegerLiteral (var & value)
{
	//__LOG__ << std::endl;

	comments ();

	if (Grammar::oct (istream) or Grammar::OCT (istream))
	{
		uint32_t number = 0;

		if (istream >> std::oct >> number)
		{
			value .reset (new Number (number));
			return true;
		}

		istream .clear ();
	}

	return false;
}

bool
Parser::hexIntegerLiteral (var & value)
{
	//__LOG__ << std::endl;

	comments ();

	if (Grammar::hex (istream) or Grammar::HEX (istream))
	{
		uint32_t number = 0;

		if (istream >> std::hex >> number)
		{
			value .reset (new Number (number));
			return true;
		}

		istream .clear ();
	}

	return false;
}

bool
Parser::stringLiteral (var & value)
{
	static const io::quoted_string doubleQuotedString ('"');
	static const io::quoted_string singleQuotedString ('\'');

	comments ();

	std::string characters;

	if (doubleQuotedString (istream, characters))
	{
		value .reset (new String (characters));
		return true;
	}

	if (singleQuotedString (istream, characters))
	{
		value .reset (new String (characters));
		return true;
	}

	return false;
}

// A.2 Number Conversions

// A.3 Expressions

bool
Parser::primaryExpression (var & value)
{
	//__LOG__ << std::endl;

	comments ();

	if (Grammar::_this (istream))
	{
		// ...
		return true;
	}

	std::string identifierCharacters;

	if (identifier (identifierCharacters))
	{
		value .reset (new Identifier (getExecutionContext (), std::move (identifierCharacters)));
		return true;
	}

	if (literal (value))
		return true;

	//if (arrayLiteral (value))
	//	return true;

	//if (objectLiteral (value))
	//	return true;

	if (Grammar::OpenParenthesis (istream))
	{
		if (expression (value))
		{
			comments ();

			if (Grammar::CloseParenthesis (istream))
				return true;

			throw SyntaxError ("Expected ')' after expression.");
		}

		throw SyntaxError ("Expected expression after '('.");
	}

	return false;
}

bool
Parser::memberExpression (var & value)
{
	//__LOG__ << std::endl;

	if (primaryExpression (value))
		return true;

	if (functionExpression (value))
		return true;

	/// ...

	return false;
}

bool
Parser::newExpression (var & value)
{
	//__LOG__ << std::endl;

	if (memberExpression (value))
		return true;

	// ...

	return false;
}

bool
Parser::leftHandSideExpression (var & value)
{
	//__LOG__ << std::endl;

	if (newExpression (value))
	{
		isLeftHandSideExressions .back () = true;
		return true;
	}

	//if (callExpression ())
	//{
	//	isLeftHandSideExressions .back () = true;
	//	return true;
	//}

	return false;
}

bool
Parser::postfixExpression (var & value)
{
	//__LOG__ << std::endl;

	if (leftHandSideExpression (value))
		return true;

	// ...
	//		isLeftHandSideExressions .back () = false;

	return false;
}

bool
Parser::unaryExpression (var & value)
{
	//__LOG__ << std::endl;

	if (postfixExpression (value))
		return true;

	comments ();

	if (Grammar::_delete (istream))
	{
		isLeftHandSideExressions .back () = false;

		var expression;

		if (unaryExpression (expression))
		{
			//value .reset (new Delete (std::move (expression)));
			return true;
		}

		throw SyntaxError ("Expected expression after delete.");
	}

	if (Grammar::_void (istream))
	{
		isLeftHandSideExressions .back () = false;

		var expression;

		if (unaryExpression (expression))
		{
			//value .reset (new Void (std::move (expression)));
			return true;
		}

		throw SyntaxError ("Expected expression after void.");
	}

	if (Grammar::typeof (istream))
	{
		isLeftHandSideExressions .back () = false;

		var expression;

		if (unaryExpression (expression))
		{
			//value .reset (new TypeOf (std::move (expression)));
			return true;
		}

		throw SyntaxError ("Expected expression after typeof.");
	}

	if (Grammar::Increment (istream))
	{
		isLeftHandSideExressions .back () = false;

		var expression;

		if (unaryExpression (expression))
		{
			//value .reset (new PostIncrement (std::move (expression)));
			return true;
		}

		throw SyntaxError ("Expected expression after ++.");
	}

	if (Grammar::Decrement (istream))
	{
		isLeftHandSideExressions .back () = false;

		var expression;

		if (unaryExpression (expression))
		{
			//value .reset (new PostDecrement (std::move (expression)));
			return true;
		}

		throw SyntaxError ("Expected expression after --.");
	}

	if (Grammar::PlusSign (istream))
	{
		isLeftHandSideExressions .back () = false;

		var expression;

		if (unaryExpression (expression))
		{
			//value .reset (new ToNumber (std::move (expression)));
			return true;
		}

		throw SyntaxError ("Expected expression after +.");
	}

	if (Grammar::MinusSign (istream))
	{
		isLeftHandSideExressions .back () = false;

		var expression;

		if (unaryExpression (expression))
		{
			//value .reset (new Negate (std::move (expression)));
			return true;
		}

		throw SyntaxError ("Expected expression after -.");
	}

	if (Grammar::Tilde (istream))
	{
		isLeftHandSideExressions .back () = false;

		var expression;

		if (unaryExpression (expression))
		{
			//value .reset (new BitwiseNOT (std::move (expression)));
			return true;
		}

		throw SyntaxError ("Expected expression after ~.");
	}

	if (Grammar::ExclamationMark (istream))
	{
		isLeftHandSideExressions .back () = false;

		var expression;

		if (unaryExpression (expression))
		{
			//value .reset (new LogicalNOT (std::move (expression)));
			return true;
		}

		throw SyntaxError ("Expected expression after !.");
	}

	return false;
}

bool
Parser::multiplicativeExpression (var & lhs)
{
	//__LOG__ << std::endl;

	if (unaryExpression (lhs))
	{
		comments ();

		if (Grammar::Multiplication (istream))
		{
			if (Grammar::Assignment .lookahead (istream))
				return Grammar::Multiplication .rewind (istream);

			isLeftHandSideExressions .back () = false;

			var rhs = get_undefined ();

			if (multiplicativeExpression (rhs))
			{
				lhs = make_multiplication (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after '*'.");
		}

		if (Grammar::Division (istream))
		{
			if (Grammar::Assignment .lookahead (istream))
				return Grammar::Division .rewind (istream);

			isLeftHandSideExressions .back () = false;

			var rhs = get_undefined ();

			if (multiplicativeExpression (rhs))
			{
				lhs = make_division (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after '/'.");
		}

		if (Grammar::Remainder (istream))
		{
			if (Grammar::Assignment .lookahead (istream))
				return Grammar::Remainder .rewind (istream);

			isLeftHandSideExressions .back () = false;

			var rhs = get_undefined ();

			if (multiplicativeExpression (rhs))
			{
				lhs = make_remainder (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after '/'.");
		}

		return true;
	}

	return false;
}

bool
Parser::additiveExpression (var & lhs)
{
	//__LOG__ << std::endl;

	if (multiplicativeExpression (lhs))
	{
		comments ();

		if (Grammar::Addition (istream))
		{
			if (Grammar::Assignment .lookahead (istream))
				return Grammar::Addition .rewind (istream);

			isLeftHandSideExressions .back () = false;

			var rhs = get_undefined ();

			if (additiveExpression (rhs))
			{
				lhs = make_addition (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after '+'.");
		}

		if (Grammar::Subtraction (istream))
		{
			if (Grammar::Assignment .lookahead (istream))
				return Grammar::Subtraction .rewind (istream);

			isLeftHandSideExressions .back () = false;

			var rhs = get_undefined ();

			if (additiveExpression (rhs))
			{
				lhs = make_subtraction (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after '-'.");
		}

		return true;
	}

	return false;
}

bool
Parser::shiftExpression (var & lhs)
{
	//__LOG__ << std::endl;

	if (additiveExpression (lhs))
	{
		comments ();

		if (Grammar::LeftShift (istream))
		{
			if (Grammar::Assignment .lookahead (istream))
				return Grammar::LeftShift .rewind (istream);

			isLeftHandSideExressions .back () = false;

			var rhs = get_undefined ();

			if (shiftExpression (rhs))
			{
				lhs = make_left_shift (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after '<<'.");
		}

		if (Grammar::UnsignedRightShift (istream))
		{
			if (Grammar::Assignment .lookahead (istream))
				return Grammar::UnsignedRightShift .rewind (istream);

			isLeftHandSideExressions .back () = false;

			var rhs = get_undefined ();

			if (shiftExpression (rhs))
			{
				//lhs = make_unsigned_right_shift (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after '>>>'.");
		}

		if (Grammar::RightShift (istream))
		{
			if (Grammar::Assignment .lookahead (istream))
				return Grammar::RightShift .rewind (istream);

			isLeftHandSideExressions .back () = false;

			var rhs = get_undefined ();

			if (shiftExpression (rhs))
			{
				//lhs = make_right_shift (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after '>>'.");
		}

		return true;
	}

	return false;
}

bool
Parser::relationalExpression (var & lhs)
{
	//__LOG__ << std::endl;

	if (shiftExpression (lhs))
	{
		comments ();

		if (Grammar::LessEqual (istream))
		{
			if (Grammar::Assignment .lookahead (istream))
				return Grammar::LessEqual .rewind (istream);

			isLeftHandSideExressions .back () = false;

			var rhs = get_undefined ();

			if (relationalExpression (rhs))
			{
				//lhs = make_less_equal (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after '<='.");
		}

		if (Grammar::GreaterEqual (istream))
		{
			if (Grammar::Assignment .lookahead (istream))
				return Grammar::GreaterEqual .rewind (istream);

			isLeftHandSideExressions .back () = false;

			var rhs = get_undefined ();

			if (relationalExpression (rhs))
			{
				//lhs = make_greater_equal (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after '>='.");
		}

		if (Grammar::Less (istream))
		{
			if (Grammar::Assignment .lookahead (istream))
				return Grammar::Less .rewind (istream);

			isLeftHandSideExressions .back () = false;

			var rhs = get_undefined ();

			if (relationalExpression (rhs))
			{
				//lhs = make_less (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after '<'.");
		}

		if (Grammar::Greater (istream))
		{
			if (Grammar::Assignment .lookahead (istream))
				return Grammar::Greater .rewind (istream);

			isLeftHandSideExressions .back () = false;

			var rhs = get_undefined ();

			if (relationalExpression (rhs))
			{
				//lhs = make_greater (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after '>'.");
		}

		if (Grammar::instanceof (istream))
		{
			if (Grammar::Assignment .lookahead (istream))
				return Grammar::instanceof .rewind (istream);

			isLeftHandSideExressions .back () = false;

			var rhs = get_undefined ();

			if (relationalExpression (rhs))
			{
				//lhs = make_instanceof (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after 'instanceof'.");
		}

		if (Grammar::in (istream))
		{
			if (Grammar::Assignment .lookahead (istream))
				return Grammar::in .rewind (istream);

			isLeftHandSideExressions .back () = false;

			var rhs = get_undefined ();

			if (relationalExpression (rhs))
			{
				//lhs = make_in (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after 'in'.");
		}

		return true;
	}

	return false;
}

bool
Parser::equalityExpression (var & lhs)
{
	//__LOG__ << std::endl;

	if (relationalExpression (lhs))
	{
		comments ();

		if (Grammar::StrictEqual (istream))
		{
			isLeftHandSideExressions .back () = false;

			var rhs = get_undefined ();

			if (equalityExpression (rhs))
			{
				//lhs = make_strict_equal (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after '==='.");
		}

		if (Grammar::StrictNotEqual (istream))
		{
			isLeftHandSideExressions .back () = false;

			var rhs = get_undefined ();

			if (equalityExpression (rhs))
			{
				//lhs = make_strict_not_equal (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after '!=='.");
		}

		if (Grammar::Equal (istream))
		{
			isLeftHandSideExressions .back () = false;

			var rhs = get_undefined ();

			if (equalityExpression (rhs))
			{
				//lhs = make_equal (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after '=='.");
		}

		if (Grammar::NotEqual (istream))
		{
			isLeftHandSideExressions .back () = false;

			var rhs = get_undefined ();

			if (equalityExpression (rhs))
			{
				//lhs = make_not_equal (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after '!='.");
		}

		return true;
	}

	return false;
}

bool
Parser::bitwiseANDExpression (var & lhs)
{
	//__LOG__ << std::endl;

	if (equalityExpression (lhs))
	{
		comments ();

		if (Grammar::LogicalAND .lookahead (istream))
			return true;

		if (Grammar::BitwiseAND (istream))
		{
			if (Grammar::Assignment .lookahead (istream))
				return Grammar::BitwiseAND .rewind (istream);

			isLeftHandSideExressions .back () = false;

			var rhs = get_undefined ();

			if (bitwiseANDExpression (rhs))
			{
				//lhs = make_bitwise_and (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after '&'.");
		}

		return true;
	}

	return false;
}

bool
Parser::bitwiseXORExpression (var & lhs)
{
	//__LOG__ << std::endl;

	if (bitwiseANDExpression (lhs))
	{
		comments ();

		if (Grammar::BitwiseXOR (istream))
		{
			if (Grammar::Assignment .lookahead (istream))
				return Grammar::BitwiseXOR .rewind (istream);

			isLeftHandSideExressions .back () = false;

			var rhs = get_undefined ();

			if (bitwiseXORExpression (rhs))
			{
				//lhs = make_bitwise_xor (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after '^'.");
		}

		return true;
	}

	return false;
}

bool
Parser::bitwiseORExpression (var & lhs)
{
	//__LOG__ << std::endl;

	if (bitwiseXORExpression (lhs))
	{
		comments ();

		if (Grammar::LogicalOR .lookahead (istream))
			return true;

		if (Grammar::BitwiseOR (istream))
		{
			if (Grammar::Assignment .lookahead (istream))
				return Grammar::BitwiseOR .rewind (istream);

			isLeftHandSideExressions .back () = false;

			var rhs = get_undefined ();

			if (bitwiseORExpression (rhs))
			{
				//lhs = make_bitwise_or (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after '|'.");
		}

		return true;
	}

	return false;
}

bool
Parser::logicalANDExpression (var & lhs)
{
	//__LOG__ << std::endl;

	if (bitwiseORExpression (lhs))
	{
		comments ();

		if (Grammar::LogicalAND (istream))
		{
			isLeftHandSideExressions .back () = false;

			var rhs = get_undefined ();

			if (logicalANDExpression (rhs))
			{
				//lhs = make_logical_and (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after '&&'.");
		}

		return true;
	}

	return false;
}

bool
Parser::logicalORExpression (var & lhs)
{
	//__LOG__ << std::endl;

	if (logicalANDExpression (lhs))
	{
		comments ();

		if (Grammar::LogicalOR (istream))
		{
			isLeftHandSideExressions .back () = false;

			var rhs = get_undefined ();

			if (logicalORExpression (rhs))
			{
				//lhs = make_logical_or (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after '||'.");
		}

		return true;
	}

	return false;
}

bool
Parser::conditionalExpression (var & first)
{
	//__LOG__ << std::endl;

	if (logicalORExpression (first))
	{
		comments ();

		if (Grammar::QuestionMark (istream))
		{
			isLeftHandSideExressions .back () = false;

			var second = get_undefined ();

			if (assignmentExpression (second))
			{
				comments ();

				if (Grammar::Colon (istream))
				{
					var third = get_undefined ();

					if (assignmentExpression (third))
					{
						//first = make_conditional (std::move (first), std::move (second), std::move (third));
						return true;
					}
				}

				throw SyntaxError ("Expected expression after ':'.");
			}

			throw SyntaxError ("Expected expression after '?'.");
		}

		return true;
	}

	return false;
}

bool
Parser::assignmentExpression (var & value)
{
	//__LOG__ << std::endl;

	isLeftHandSideExressions .push_back (false);

	if (conditionalExpression (value))
	{
		const bool isLeftHandSideExression = isLeftHandSideExressions .back ();

		isLeftHandSideExressions .pop_back ();

		if (isLeftHandSideExression)
		{
			comments ();

			if (Grammar::Assignment (istream))
			{
				var expression = get_undefined ();

				if (assignmentExpression (expression))
				{
					//value .reset (new Assignment (std::move (value), std::move (expression), AssignmentOperatorType::ASSIGNMENT));
					return true;
				}

				throw SyntaxError ("Expected expression after '='.");
			}

			AssignmentOperatorType type;

			if (assignmentOperator (type))
			{
				var expression = get_undefined ();

				if (assignmentExpression (expression))
				{
					//value .reset (new Assignment (std::move (value), std::move (expression), type));
					return true;
				}

				throw SyntaxError ("Expected expression after '" + to_ustring (type) + "'.");
			}
		}

		return true;
	}

	isLeftHandSideExressions .pop_back ();

	return false;
}

bool
Parser::assignmentOperator (AssignmentOperatorType & type)
{
	comments ();

	if (Grammar::MultiplicationAssigment (istream))
	{
		type = AssignmentOperatorType::MULTIPLICATION_ASSIGMENT;
		return true;
	}

	if (Grammar::DivisionAssignment (istream))
	{
		type = AssignmentOperatorType::DIVISION_ASSIGNMENT;
		return true;
	}

	if (Grammar::RemainderAssignment (istream))
	{
		type = AssignmentOperatorType::REMAINDER_ASSIGNMENT;
		return true;
	}

	if (Grammar::AdditionAssignment (istream))
	{
		type = AssignmentOperatorType::ADDITION_ASSIGNMENT;
		return true;
	}

	if (Grammar::SubtractionAssignment (istream))
	{
		type = AssignmentOperatorType::SUBTRACTION_ASSIGNMENT;
		return true;
	}

	if (Grammar::LeftShiftAssignment (istream))
	{
		type = AssignmentOperatorType::LEFT_SHIFT_ASSIGNMENT;
		return true;
	}

	if (Grammar::RightShiftAssignment (istream))
	{
		type = AssignmentOperatorType::RIGHT_SHIFT_ASSIGNMENT;
		return true;
	}

	if (Grammar::UnsignedRightShiftAssignment (istream))
	{
		type = AssignmentOperatorType::UNSIGNED_RIGHT_SHIFT_ASSIGNMENT;
		return true;
	}

	if (Grammar::BitwiseANDAssignment (istream))
	{
		type = AssignmentOperatorType::BITWISE_AND_ASSIGNMENT;
		return true;
	}

	if (Grammar::BitwiseXORAssignment (istream))
	{
		type = AssignmentOperatorType::BITWISE_XOR_ASSIGNMENT;
		return true;
	}

	if (Grammar::BitwiseORAssignment (istream))
	{
		type = AssignmentOperatorType::BITWISE_OR_ASSIGNMENT;
		return true;
	}

	return false;
}

bool
Parser::expression (var & value)
{
	//__LOG__ << std::endl;

	if (assignmentExpression (value))
	{
		for ( ; ;)
		{
			comments ();

			if (Grammar::Comma (istream))
			{
				if (assignmentExpression (value))
					continue;

				throw SyntaxError ("Expected expression after ','.");
			}

			return true;
		}
	}

	return false;
}

// A.4 Statements

bool
Parser::statement ()
{
	//__LOG__ << istream .tellg () << std::endl;

	if (variableStatement ())
		return true;

	if (expressionStatement ())
		return true;

	if (emptyStatement ())
		return true;

	return false;
}

bool
Parser::variableStatement ()
{
	comments ();

	if (Grammar::var (istream))
	{
		if (variableDeclarationList ())
		{
			comments ();

			if (Grammar::Semicolon (istream))
				return true;

			throw SyntaxError ("Expected a ';' after variable declaration.");
		}

		throw SyntaxError ("Expected variable name after �var�.");
	}

	return false;
}

bool
Parser::variableDeclarationList ()
{
	//__LOG__ << std::endl;

	if (variableDeclaration ())
	{
		for ( ; ;)
		{
			comments ();

			if (Grammar::Comma (istream))
			{
				if (variableDeclaration ())
					continue;

				throw SyntaxError ("Expected variable name after ','.");
			}

			return true;
		}
	}

	return false;
}

bool
Parser::variableDeclaration ()
{
	std::string identifierCharacters;

	if (identifier (identifierCharacters))
	{
		var value = get_undefined ();

		initialiser (value);

		getExecutionContext () -> addExpression (new VariableDeclaration (getExecutionContext (), std::move (identifierCharacters), std::move (value)));

		return true;
	}

	return false;
}

bool
Parser::initialiser (var & value)
{
	comments ();

	if (Grammar::Assignment (istream))
	{
		if (assignmentExpression (value))
			return true;
	}

	return false;
}

bool
Parser::expressionStatement ()
{
	//__LOG__ << std::endl;

	comments ();

	if (Grammar::OpenBrace .lookahead (istream))
		return false;

	if (Grammar::function .lookahead (istream))
		return false;

	var value = get_undefined ();

	if (expression (value))
	{
		comments ();

		if (Grammar::Semicolon (istream))
		{
			getExecutionContext () -> addExpression (std::move (value));
			return true;
		}

		throw SyntaxError ("Expected ';' after expression.");
	}

	return false;
}

bool
Parser::emptyStatement ()
{
	//__LOG__ << std::endl;

	comments ();

	if (Grammar::Semicolon (istream))
		return true;

	return false;
}

// A.5 Functions and Programs

bool
Parser::functionDeclaration ()
{
	//__LOG__ << std::endl;

	comments ();

	if (Grammar::function (istream))
	{
		std::string name;

		if (identifier (name))
		{
			comments ();

			if (Grammar::OpenParenthesis (istream))
			{
				std::vector <std::string> formalParameters;

				formalParameterList (formalParameters);

				comments ();

				if (Grammar::CloseParenthesis (istream))
				{
					comments ();

					if (Grammar::OpenBrace (istream))
					{
						const auto function = make_ptr <Function> (getExecutionContext (), name, std::move (formalParameters));

						pushExecutionContext (function .get ());

						functionBody ();

						popExecutionContext ();

						comments ();

						if (Grammar::CloseBrace (istream))
						{
							getExecutionContext () -> replaceFunction (function);

							return true;
						}

						throw SyntaxError ("Expected a '}' after function body.");
					}

					throw SyntaxError ("Expected a '{' after parameter list.");
				}

				throw SyntaxError ("Expected a ')' after formal parameters.");
			}

			throw SyntaxError ("Expected a '(' after function name.");
		}

		throw SyntaxError ("Function statement requires a name.");
	}

	return false;
}

bool
Parser::functionExpression (var & value)
{
	//__LOG__ << std::endl;

	comments ();

	if (Grammar::function (istream))
	{
		std::string name;

		identifier (name);

		comments ();

		if (Grammar::OpenParenthesis (istream))
		{
			std::vector <std::string> formalParameters;

			formalParameterList (formalParameters);

			comments ();

			if (Grammar::CloseParenthesis (istream))
			{
				comments ();

				if (Grammar::OpenBrace (istream))
				{
					const auto function = make_ptr <Function> (getExecutionContext (), name, std::move (formalParameters));

					pushExecutionContext (function .get ());

					functionBody ();

					popExecutionContext ();

					comments ();

					if (Grammar::CloseBrace (istream))
					{
						value = std::move (function);

						return true;
					}

					throw SyntaxError ("Expected a '}' after function body.");
				}

				throw SyntaxError ("Expected a '{' after parameter list.");
			}

			throw SyntaxError ("Expected a ')' after formal parameters.");
		}

		throw SyntaxError ("Expected a '(' before formal parameters.");
	}

	return false;
}

bool
Parser::formalParameterList (std::vector <std::string> & formalParameters)
{
	//__LOG__ << std::endl;

	std::string identifierCharacters;

	if (identifier (identifierCharacters))
	{
		formalParameters .emplace_back (std::move (identifierCharacters));

		for ( ; ;)
		{
			comments ();

			if (Grammar::Comma (istream))
			{
				std::string identifierCharacters;

				if (identifier (identifierCharacters))
				{
					formalParameters .emplace_back (std::move (identifierCharacters));
					continue;
				}

				throw SyntaxError ("Expected expression after ','.");
			}

			return true;
		}
	}

	return false;
}

void
Parser::functionBody ()
{
	//__LOG__ << std::endl;

	sourceElements ();
}

void
Parser::program ()
{
	//__LOG__ << std::endl;

	sourceElements ();

	if (istream .peek () not_eq std::char_traits <char>::eof ())
		throw SyntaxError ("Unexpected statement.");
}

void
Parser::sourceElements ()
{
	//__LOG__ << std::endl;

	while (sourceElement ())
		;
}

bool
Parser::sourceElement ()
{
	//__LOG__ << istream .tellg () << std::endl;

	if (statement ())
		return true;

	if (functionDeclaration ())
		return true;

	return false;
}

} // pb
} // titania
