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

#ifndef STATEMENTS_SUM_DECLARATION_STATEMENT_H_
#define STATEMENTS_SUM_DECLARATION_STATEMENT_H_

#include <declaration_statement.h>

class SumTypeSpecifier;
class Symbol;

class SumDeclarationStatement: public DeclarationStatement {
public:
	SumDeclarationStatement(const yy::location position,
			const_shared_ptr<SumTypeSpecifier> type,
			const yy::location type_position, const_shared_ptr<string> name,
			const yy::location name_location,
			const_shared_ptr<Expression> initializer_expression);
	virtual ~SumDeclarationStatement();

	virtual const ErrorListRef preprocess(
			const shared_ptr<ExecutionContext> execution_context) const;

	virtual const ErrorListRef execute(
			shared_ptr<ExecutionContext> execution_context) const;

	virtual const DeclarationStatement* WithInitializerExpression(
			const_shared_ptr<Expression> expression) const;

	virtual const_shared_ptr<TypeSpecifier> GetType() const;

private:
	const_shared_ptr<SumTypeSpecifier> m_type;
	const yy::location m_type_position;
};

#endif /* STATEMENTS_SUM_DECLARATION_STATEMENT_H_ */
