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

#ifndef STATEMENTS_STRUCT_DECLARATION_STATEMENT_H_
#define STATEMENTS_STRUCT_DECLARATION_STATEMENT_H_

#include <statement.h>
#include <modifier.h>
#include <string>
#include <declaration_statement.h>
#include <compound_type_specifier.h>

class StructDeclarationStatement: public DeclarationStatement {
public:
	StructDeclarationStatement(const yy::location position,
			const_shared_ptr<CompoundTypeSpecifier> type,
			const_shared_ptr<string> name, const yy::location name_position,
			DeclarationListRef member_declaration_list,
			const yy::location member_declaration_list_position,
			ModifierListRef modifier_list,
			const yy::location modifiers_location);
	virtual ~StructDeclarationStatement();

	virtual const ErrorListRef preprocess(
			const shared_ptr<ExecutionContext> execution_context) const;

	virtual const ErrorListRef execute(
			shared_ptr<ExecutionContext> execution_context) const;

	DeclarationListRef GetMemberDeclarationListRef() const {
		return m_member_declaration_list;
	}

	virtual const_shared_ptr<TypeSpecifier> GetType() const {
		return m_type;
	}

	virtual const DeclarationStatement* WithInitializerExpression(
			const_shared_ptr<Expression> expression) const;

	const yy::location GetMemberDeclarationListRefPosition() const {
		return m_member_declaration_list_position;
	}

	ModifierListRef GetModifierListRef() const {
		return m_modifier_list;
	}

	const yy::location GetModifiersLocation() const {
		return m_modifiers_location;
	}

private:
	const yy::location m_type_position;
	DeclarationListRef m_member_declaration_list;
	const yy::location m_member_declaration_list_position;
	ModifierListRef m_modifier_list;
	const yy::location m_modifiers_location;
	const_shared_ptr<CompoundTypeSpecifier> m_type;
};

#endif /* STATEMENTS_STRUCT_DECLARATION_STATEMENT_H_ */
