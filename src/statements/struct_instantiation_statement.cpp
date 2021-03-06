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

#include <compound_type_instance.h>
#include <struct_instantiation_statement.h>
#include <map>
#include <execution_context.h>
#include <type_table.h>
#include <result.h>
#include <member_instantiation.h>
#include <expression.h>
#include <member_definition.h>
#include <compound_type.h>
#include <symbol.h>

StructInstantiationStatement::StructInstantiationStatement(
		const yy::location position,
		const_shared_ptr<CompoundTypeSpecifier> type_specifier,
		const yy::location type_position, const_shared_ptr<string> name,
		const yy::location name_position,
		const_shared_ptr<Expression> initializer_expression) :
		DeclarationStatement(position, name, name_position,
				initializer_expression), m_type_specifier(type_specifier), m_type_position(
				type_position) {
}

StructInstantiationStatement::~StructInstantiationStatement() {
}

const ErrorListRef StructInstantiationStatement::preprocess(
		const shared_ptr<ExecutionContext> execution_context) const {
	ErrorListRef errors = ErrorList::GetTerminator();
	//TODO: validate that all members are initialized for readonly structs (?)

	const_shared_ptr<CompoundType> type =
			execution_context->GetTypeTable()->GetType(
					m_type_specifier->GetTypeName());

	if (type != CompoundType::GetDefaultCompoundType()) {
		auto existing = execution_context->GetSymbol(GetName(), SHALLOW);
		if (existing == Symbol::GetDefaultSymbol()) {
			plain_shared_ptr<const CompoundTypeInstance> instance;
			if (GetInitializerExpression()) {
				const_shared_ptr<TypeSpecifier> expression_type =
						GetInitializerExpression()->GetType(execution_context);
				errors = GetInitializerExpression()->Validate(
						execution_context);

				if (ErrorList::IsTerminator(errors)) {
					const_shared_ptr<CompoundTypeSpecifier> as_compound_specifier =
							std::dynamic_pointer_cast<
									const CompoundTypeSpecifier>(
									expression_type);

					if (as_compound_specifier
							&& m_type_specifier->GetTypeName().compare(
									as_compound_specifier->GetTypeName())
									== 0) {
						if (GetInitializerExpression()->IsConstant()) {
							const_shared_ptr<Result> result =
									GetInitializerExpression()->Evaluate(
											execution_context);
							errors = result->GetErrors();
							if (ErrorList::IsTerminator(errors)) {
								instance = static_pointer_cast<
										const CompoundTypeInstance>(
										result->GetData());
							}
						} else {
							//generate default instance
							instance = CompoundTypeInstance::GetDefaultInstance(
									m_type_specifier->GetTypeName(), type);
						}
					} else {
						errors =
								ErrorList::From(
										make_shared<Error>(Error::SEMANTIC,
												Error::ASSIGNMENT_TYPE_ERROR,
												GetInitializerExpression()->GetPosition().begin.line,
												GetInitializerExpression()->GetPosition().begin.column,
												m_type_specifier->GetTypeName(),
												expression_type->ToString()),
										errors);
					}
				}
			} else {
				instance = CompoundTypeInstance::GetDefaultInstance(
						m_type_specifier->GetTypeName(), type);
			}

			if (ErrorList::IsTerminator(errors)) {
				//we've been able to get a good initial value (that is, no errors have occurred)
				auto symbol = const_shared_ptr<Symbol>(new Symbol(instance));
				const InsertResult insert_result =
						execution_context->InsertSymbol(*GetName(), symbol);

				if (insert_result != INSERT_SUCCESS) {
					assert(false);
				}
			}
		} else {
			//symbol already exists
			errors = ErrorList::From(
					make_shared<Error>(Error::SEMANTIC,
							Error::PREVIOUS_DECLARATION,
							m_type_position.begin.line,
							m_type_position.begin.column, *GetName()), errors);
		}
	} else {
		//type does not exist
		errors = ErrorList::From(
				make_shared<Error>(Error::SEMANTIC, Error::UNDECLARED_TYPE,
						m_type_position.begin.line,
						m_type_position.begin.column,
						m_type_specifier->GetTypeName()), errors);
	}

	return errors;
}

const_shared_ptr<TypeSpecifier> StructInstantiationStatement::GetType() const {
	return m_type_specifier;
}

const ErrorListRef StructInstantiationStatement::execute(
		shared_ptr<ExecutionContext> execution_context) const {
	ErrorListRef errors = ErrorList::GetTerminator();

	if (GetInitializerExpression()) {
		const_shared_ptr<Result> evaluation =
				GetInitializerExpression()->Evaluate(execution_context);

		errors = evaluation->GetErrors();

		if (ErrorList::IsTerminator(errors)) {
			auto void_value = evaluation->GetData();
			const_shared_ptr<const CompoundTypeInstance> instance =
					static_pointer_cast<const CompoundTypeInstance>(void_value);
			execution_context->SetSymbol(*GetName(), instance);
		}
	}

	return errors;
}

const DeclarationStatement* StructInstantiationStatement::WithInitializerExpression(
		const_shared_ptr<Expression> expression) const {
	return new StructInstantiationStatement(GetPosition(), m_type_specifier,
			m_type_position, GetName(), GetNamePosition(), expression);
}
