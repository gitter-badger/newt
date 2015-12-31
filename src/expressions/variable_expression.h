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

#ifndef VARIABLE_EXPRESSION_H_
#define VARIABLE_EXPRESSION_H_

#include "expression.h"

class Variable;

class VariableExpression: public Expression {
public:
	VariableExpression(const yy::location position, const_shared_ptr<Variable> variable);

	virtual const_shared_ptr<TypeSpecifier> GetType(const_shared_ptr<ExecutionContext> execution_context) const;

	virtual const_shared_ptr<Result> Evaluate(
			const_shared_ptr<ExecutionContext> execution_context) const;

	virtual const bool IsConstant() const {
		return false;
	}

	virtual const ErrorList Validate(const_shared_ptr<ExecutionContext> execution_context) const;

	const_shared_ptr<Variable> GetVariable() const {
		return m_variable;
	}

private:
	const_shared_ptr<Variable> m_variable;
};

#endif /* VARIABLE_EXPRESSION_H_ */
