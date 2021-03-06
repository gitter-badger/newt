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

#ifndef SPECIFIERS_TYPE_SPECIFIER_H_
#define SPECIFIERS_TYPE_SPECIFIER_H_

#include <defaults.h>
#include <string>
#include <linked_list.h>

class Expression;
class DeclarationStatement;
class TypeTable;
class Symbol;

using namespace std;
class TypeSpecifier {
public:
	virtual ~TypeSpecifier() {
	}

	virtual const string ToString() const = 0;
	virtual const bool IsAssignableTo(
			const_shared_ptr<TypeSpecifier> other) const = 0;
	virtual const_shared_ptr<void> DefaultValue(
			const TypeTable& type_table) const = 0;

	virtual const_shared_ptr<DeclarationStatement> GetDeclarationStatement(
			const yy::location position, const_shared_ptr<TypeSpecifier> type,
			const yy::location type_position, const_shared_ptr<string> name,
			const yy::location name_position,
			const_shared_ptr<Expression> initializer_expression) const = 0;

	virtual bool operator==(const TypeSpecifier &other) const = 0;
	virtual bool operator!=(const TypeSpecifier &other) const {
		return !(*this == other);
	}
};

typedef const LinkedList<const TypeSpecifier, ALLOW_DUPLICATES> TypeSpecifierList;
typedef shared_ptr<TypeSpecifierList> TypeSpecifierListRef;

#endif /* SPECIFIERS_TYPE_SPECIFIER_H_ */
