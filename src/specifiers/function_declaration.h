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

#ifndef SPECIFIERS_FUNCTION_DECLARATION_H_
#define SPECIFIERS_FUNCTION_DECLARATION_H_

#include <declaration_statement.h>
#include <function_type_specifier.h>

class FunctionDeclaration: public FunctionTypeSpecifier {
public:
	FunctionDeclaration(const DeclarationList* parameter_list,
			const_shared_ptr<TypeSpecifier> return_type);
	virtual ~FunctionDeclaration();

	static const_shared_ptr<FunctionDeclaration> FromTypeSpecifier(
			const FunctionTypeSpecifier& type_specifier);

	virtual const_shared_ptr<void> DefaultValue(
			const TypeTable& type_table) const;

	virtual const DeclarationStatement* GetDeclarationStatement(
			const yy::location position, const_shared_ptr<TypeSpecifier> type,
			const yy::location type_position, const_shared_ptr<string> name,
			const yy::location name_position,
			const Expression* initializer_expression) const;

	const DeclarationList* GetParameterList() const {
		return m_parameter_list;
	}

	static const TypeSpecifierList* GetTypeList(
			const DeclarationList* parameter_list);
protected:
	static const Function* GetDefaultFunctionDeclaration(
			const FunctionDeclaration* function_declaration,
			const TypeTable& type_table);

private:
	const DeclarationList* m_parameter_list;
};

#endif /* SPECIFIERS_FUNCTION_DECLARATION_H_ */
