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

#ifndef FUNCTION_H_
#define FUNCTION_H_

#include <expression.h>

class FunctionDeclaration;
class StatementBlock;
class Result;
class ExecutionContext;

class Function {
public:
	Function(const_shared_ptr<FunctionDeclaration> declaration,
			const StatementBlock* body, const ExecutionContext* closure);
	virtual ~Function();

	const_shared_ptr<FunctionDeclaration> GetType() const {
		return m_declaration;
	}

	const Result* Evaluate(const ArgumentList* argument_list,
			const ExecutionContext* invocation_context) const;

private:
	const_shared_ptr<FunctionDeclaration> m_declaration;
	const StatementBlock* m_body;
	const ExecutionContext* m_closure;
};

#endif /* FUNCTION_H_ */
