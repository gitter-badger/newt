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

#ifndef PARAMETER_H_
#define PARAMETER_H_

#include <string>
#include "expression.h"

using namespace std;

class Parameter {
public:
	Parameter(const string* name, const_shared_ptr<Expression> expression) :
			m_name(name), m_expression(expression) {
	}

	virtual ~Parameter() {
	}

	const_shared_ptr<Expression> GetExpression() const {
		return m_expression;
	}

	const string* GetName() const {
		return m_name;
	}

private:
	const string* m_name;
	const_shared_ptr<Expression> m_expression;
};

typedef shared_ptr<const LinkedList<Parameter, NO_DUPLICATES>> ParameterList;

#endif /* PARAMETER_H_ */
