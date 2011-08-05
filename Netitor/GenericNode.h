/*******************************************************************
Copyright (c) 2006-2008 Braden Pellett and Jordan Van Aalsburg

This file is part of NetVR.

NetVR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

NetVR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with NetVR.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************/

#ifndef NETITOR_GENERIC_NODE_H_INCLUDED
#define NETITOR_GENERIC_NODE_H_INCLUDED

#include "Netitor/Node.h"
#include "XML/Document"
#include "XML/common.h"
#include <string>

namespace Netitor
{
	class GenericNode : public Node
	{
	public:
		const std::string typeName;

		GenericNode(const std::string& typeName) : Node(), typeName(typeName) {} // Prototype

		// Return the name of this class
		virtual const std::string& getTypeName() const
		{
			return typeName;
		}

		XML::AttributeMap getAttributeMap() const { return attrs; }
		void setAttributeMap(XML::AttributeMap attrs) { this->attrs = attrs; }

	protected:

		// Return pointer to new GenericNode
		GenericNode* create() const
		{
			return new GenericNode(typeName);
		}

		XML::AttributeMap attrs;
	};
} // namespace Netitor

#endif
