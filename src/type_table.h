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

#ifndef TYPE_TABLE_H_
#define TYPE_TABLE_H_

#include <member_declaration.h>
#include <type.h>
#include <map>

class CompoundType;

using namespace std;

typedef map<const string, const_shared_ptr<CompoundType>> type_map;

class TypeTable {
public:
	TypeTable();
	virtual ~TypeTable();

	void AddType(const string& name, const_shared_ptr<CompoundType> definition);

	const_shared_ptr<CompoundType> GetType(const string& name) const;

	const_shared_ptr<void> GetDefaultValue(const string& type_name) const;

	const void print(ostream &os) const;

	const static string DefaultTypeName;

	static volatile_shared_ptr<TypeTable> GetDefault();

private:
	const shared_ptr<type_map> table;
};

#endif /* TYPE_TABLE_H_ */
