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

#include <invoke_statement.h>
#include <expression.h>
#include <invoke_expression.h>
#include <variable.h>
#include <variable_expression.h>

InvokeStatement::InvokeStatement(const_shared_ptr<Variable> variable,
		ArgumentListRef argument_list, const yy::location argument_list_position) :
		m_variable(variable), m_argument_list(argument_list), m_argument_list_position(
				argument_list_position) {
}

InvokeStatement::~InvokeStatement() {
}

const ErrorListRef InvokeStatement::preprocess(
		const shared_ptr<ExecutionContext> execution_context) const {
	//variable reference must be a reference to a function
	//argument list length and types must match
	const_shared_ptr<VariableExpression> variable_expression = make_shared<
			VariableExpression>(m_variable->GetLocation(), m_variable);
	const InvokeExpression* expression = new InvokeExpression(
			m_variable->GetLocation(), variable_expression, m_argument_list,
			m_argument_list_position);
	auto result = expression->Validate(execution_context);
	delete expression;

	return result;
}

const ErrorListRef InvokeStatement::execute(
		shared_ptr<ExecutionContext> execution_context) const {
	const_shared_ptr<VariableExpression> variable_expression = make_shared<
			VariableExpression>(m_variable->GetLocation(), m_variable);
	const InvokeExpression* expression = new InvokeExpression(
			m_variable->GetLocation(), variable_expression, m_argument_list,
			m_argument_list_position);
	auto result = expression->Evaluate(execution_context);
	delete expression;

	return result->GetErrors();
}
