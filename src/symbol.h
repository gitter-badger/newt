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

#ifndef SYMBOL_H_
#define SYMBOL_H_

#include <string>
#include <iostream>

#include <defaults.h>
#include <type.h>
#include <indent.h>

using namespace std;

class Expression;
class ExecutionContext;
class Array;
class CompoundTypeInstance;
class Function;
class Sum;

class Symbol {
	friend class SymbolContext;
	friend class ReturnStatement;
public:
	Symbol(const_shared_ptr<bool> value);
	Symbol(const_shared_ptr<int> value);
	Symbol(const_shared_ptr<double> value);
	Symbol(const_shared_ptr<string> value);
	Symbol(const_shared_ptr<Array> value);
	Symbol(const_shared_ptr<CompoundTypeInstance> value);
	Symbol(const_shared_ptr<Function> value);
	Symbol(const_shared_ptr<Sum> value);

	virtual ~Symbol();

	const_shared_ptr<TypeSpecifier> GetType() const {
		return m_type;
	}

	const_shared_ptr<void> GetValue() const {
		return m_value;
	}

	static const_shared_ptr<Symbol> GetDefaultSymbol();

	virtual const string ToString(const TypeTable& type_table,
			const Indent& indent) const;

	static const string ToString(const_shared_ptr<TypeSpecifier> type,
			const_shared_ptr<void> value, const TypeTable& type_table,
			const Indent& indent);

protected:
	Symbol(const_shared_ptr<TypeSpecifier> type, const_shared_ptr<void> value);

	virtual const_shared_ptr<Symbol> WithValue(
			const_shared_ptr<TypeSpecifier> type,
			const_shared_ptr<void> value) const;

private:
	const_shared_ptr<TypeSpecifier> m_type;
	const_shared_ptr<void> m_value;
};

#endif /* SYMBOL_H_ */
