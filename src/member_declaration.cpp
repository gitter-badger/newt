/*
 Copyright (C) 2015 The newt Authors.

 This file is part of newt.

 newt is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 newt is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with newt.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <member_declaration.h>

MemberDeclaration::MemberDeclaration(const_shared_ptr<TypeSpecifier> type,
		const yy::location type_position, const std::string* name,
		const yy::location name_position, const_shared_ptr<Expression> initializer_expression,
		const yy::location initializer_expression_position) :
		m_type(type), m_type_position(type_position), m_name(name), m_name_position(
				name_position), m_initializer_expression(
				initializer_expression), m_initializer_expression_position(
				initializer_expression_position) {
}

MemberDeclaration::~MemberDeclaration() {
}

