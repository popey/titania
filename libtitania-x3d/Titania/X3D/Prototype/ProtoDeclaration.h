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

#ifndef __TITANIA_X3D_PROTOTYPE_PROTO_H__
#define __TITANIA_X3D_PROTOTYPE_PROTO_H__

#include "../Execution/X3DExecutionContext.h"
#include "../Prototype/X3DProtoDeclarationNode.h"

namespace titania {
namespace X3D {

class ProtoDeclaration :
	public X3DProtoDeclarationNode, public X3DExecutionContext
{
public:

	using X3DExecutionContext::getProtoDeclaration;
	using X3DProtoDeclarationNode::createInstance;

	///  @name Construction

	ProtoDeclaration (X3DExecutionContext* const);

	virtual
	ProtoDeclaration*
	create (X3DExecutionContext* const) const final override;

	virtual
	ProtoDeclaration*
	copy (const CopyType type) const
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>) final override
	{ return copy (getExecutionContext (), type); }

	virtual
	ProtoDeclaration*
	copy (X3DExecutionContext* const, const CopyType) const
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>) final override;

	///  @name Instance construction

	virtual
	X3DPrototypeInstance*
	createInstance (X3DExecutionContext* const) final override;

	///  @name Common members

	virtual
	ComponentType
	getComponent () const
	throw (Error <DISPOSED>) final override
	{ return component; }

	virtual
	const std::string &
	getTypeName () const
	throw (Error <DISPOSED>) final override
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const
	throw (Error <DISPOSED>) final override
	{ return containerField; }

	void
	setExecutionContext (X3DExecutionContext* const)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override;

	///  @name Tests

	virtual
	bool
	isProtoDeclaration () const
	throw (Error <DISPOSED>) final override
	{ return true; }

	virtual
	bool
	isExternproto () const
	throw (Error <DISPOSED>) final override
	{ return false; }

	///  @name Member access

	virtual
	std::string
	getTitle () const
	throw (Error <DISPOSED>) final override
	{ return getExecutionContext () -> getTitle (); }

	virtual
	const basic::uri &
	getWorldURL () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override
	{ return getExecutionContext () -> getWorldURL (); }

	virtual
	const std::string &
	getEncoding () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override
	{ return getExecutionContext () -> getEncoding (); }

	virtual
	SpecificationVersionType
	getSpecificationVersion () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override
	{ return getExecutionContext () -> getSpecificationVersion (); }

	virtual
	const std::string &
	getCharacterEncoding () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override
	{ return getExecutionContext () -> getCharacterEncoding (); }

	virtual
	const std::string &
	getComment () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override
	{ return getExecutionContext () -> getComment (); }

	///  @name Profile/Component handling

	virtual
	const ProfileInfoPtr &
	getProfile ()  const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override
	{ return getExecutionContext () -> getProfile (); }

	virtual
	void
	updateComponent (const ComponentInfoPtr & component)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override
	{ return getExecutionContext () -> updateComponent (component); }

	virtual
	const ComponentInfoArray &
	getComponents () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override
	{ return getExecutionContext () -> getComponents (); }

	///  @name Member access

	virtual
	ProtoDeclaration*
	getProtoDeclaration ()
	throw (Error <DISPOSED>) final override
	{ return this; }
	
	///  @name Operations

	virtual
	const SFEnum <LoadState> &
	checkLoadState () const final override
	{ return loadState; }
	
	virtual
	void
	requestImmediateLoad () final override
	{ }

	virtual
	void
	updateInterfaceAndInstances () final override;

	///  @name Input/Output

	virtual
	void
	isCompressed (const bool value) final override
	{ getExecutionContext () -> isCompressed (value); }

	virtual
	bool
	isCompressed () const final override
	{ return getExecutionContext () -> isCompressed (); }

	virtual
	void
	toStream (std::ostream &) const final override;

	virtual
	void
	toXMLStream (std::ostream &) const final override;

	///  @name Destuction

	virtual
	void
	dispose () final override;


private:

	///  @name Construction

	virtual
	void
	initialize () final override;

	///  @name Input/Output

	void
	toStreamField (std::ostream &, X3DFieldDefinition* const, const size_t, const size_t) const;

	///  @name Static members

	static const ComponentType component;
	static const std::string   typeName;
	static const std::string   containerField;

	///  @name Static members

	SFEnum <LoadState> loadState;

};

} // X3D
} // titania

#endif
