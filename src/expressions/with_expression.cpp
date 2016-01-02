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

#include <with_expression.h>
#include <execution_context.h>
#include <compound_type.h>
#include <member_definition.h>
#include <member_instantiation.h>
#include <compound_type_specifier.h>

WithExpression::WithExpression(const yy::location position,
		const_shared_ptr<Expression> source_expression,
		MemberInstantiationListRef member_instantiation_list,
		const yy::location member_instantiation_list_position) :
		Expression(position), m_source_expression(source_expression), m_member_instantiation_list(
				member_instantiation_list), m_member_instantiation_list_position(
				member_instantiation_list_position) {
}

WithExpression::~WithExpression() {
}

const_shared_ptr<TypeSpecifier> WithExpression::GetType(
		const_shared_ptr<ExecutionContext> execution_context) const {
	return m_source_expression->GetType(execution_context);
}

const_shared_ptr<Result> WithExpression::Evaluate(
		const_shared_ptr<ExecutionContext> execution_context) const {
	ErrorListRef errors(ErrorList::GetTerminator());
	const_shared_ptr<Result> source_result = m_source_expression->Evaluate(
			execution_context);

	errors = source_result->GetErrors();
	if (errors == ErrorList::GetTerminator()) {
		plain_shared_ptr<void> new_value;
		const_shared_ptr<TypeSpecifier> type_specifier =
				m_source_expression->GetType(execution_context);

		const_shared_ptr<CompoundTypeSpecifier> as_compound =
				std::dynamic_pointer_cast<const CompoundTypeSpecifier>(
						type_specifier);
		if (as_compound) {
			const string type_name = as_compound->GetTypeName();
			const_shared_ptr<CompoundType> type =
					execution_context->GetTypeTable()->GetType(type_name);

			if (type != CompoundType::GetDefaultCompoundType()) {
				auto raw_result = source_result->GetData();
				auto as_compound = static_pointer_cast<
						const CompoundTypeInstance>(raw_result);
				const_shared_ptr<SymbolContext> existing_context =
						as_compound->GetDefinition();

				auto new_values = make_shared<symbol_map>(
						existing_context->GetTable()->begin(),
						existing_context->GetTable()->end());

				MemberInstantiationListRef subject = m_member_instantiation_list;
				while (!MemberInstantiationList::IsTerminator(subject)) {
					const_shared_ptr<MemberInstantiation> instantiation =
							subject->GetData();
					auto member_name = instantiation->GetName();
					const_shared_ptr<MemberDefinition> member_definition =
							type->GetMember(*member_name);

					if (member_definition
							!= MemberDefinition::GetDefaultMemberDefinition()) {
						const_shared_ptr<TypeSpecifier> member_type =
								member_definition->GetType();
						const_shared_ptr<TypeSpecifier> expression_type =
								instantiation->GetExpression()->GetType(
										execution_context);
						if (expression_type->IsAssignableTo(member_type)) {
							const_shared_ptr<MemberInstantiation> memberInstantiation =
									subject->GetData();
							const_shared_ptr<Result> evaluation_result =
									memberInstantiation->GetExpression()->Evaluate(
											execution_context);

							if (evaluation_result->GetErrors()
									== ErrorList::GetTerminator()) {
								auto void_value = evaluation_result->GetData();

								new_values->at(*member_name) =
										CompoundTypeInstance::GetSymbol(
												member_type, void_value);
							} else {
								errors = ErrorList::Concatenate(errors,
										evaluation_result->GetErrors());
							}
						} else {
							//undefined member
							errors =
									ErrorList::From(
											make_shared<Error>(Error::SEMANTIC,
													Error::ASSIGNMENT_TYPE_ERROR,
													instantiation->GetExpressionPosition().begin.line,
													instantiation->GetExpressionPosition().begin.column,
													member_type->ToString(),
													expression_type->ToString()),
											errors);
						}
					} else {
						//undefined member
						errors =
								ErrorList::From(
										std::make_shared<Error>(Error::SEMANTIC,
												Error::UNDECLARED_MEMBER,
												instantiation->GetNamePosition().begin.line,
												instantiation->GetNamePosition().begin.column,
												*member_name,
												member_definition->GetType()->ToString()),
										errors);

					}

					subject = subject->GetNext();
				}

				volatile_shared_ptr<SymbolContext> new_symbol_context =
						std::make_shared<SymbolContext>(
								SymbolContext(existing_context->GetModifiers(),
										execution_context->GetSymbolContext()->GetParent(),
										new_values));

				new_value = std::make_shared<CompoundTypeInstance>(
						as_compound->GetTypeSpecifier(), new_symbol_context);
			} else {
				errors = ErrorList::From(
						std::make_shared<Error>(Error::SEMANTIC,
								Error::UNDECLARED_TYPE,
								m_source_expression->GetPosition().begin.line,
								m_source_expression->GetPosition().begin.column,
								type_name), errors);
			}
		} else {
			errors = ErrorList::From(
					std::make_shared<Error>(Error::SEMANTIC,
							Error::VARIABLE_NOT_A_COMPOUND_TYPE,
							m_source_expression->GetPosition().begin.line,
							m_source_expression->GetPosition().begin.column),
					errors);
		}

		return make_shared<Result>(new_value, errors);
	} else {
		return source_result;
	}
}

const bool WithExpression::IsConstant() const {
	if (m_source_expression->IsConstant()) {
		MemberInstantiationListRef subject = m_member_instantiation_list;
		while (!MemberInstantiationList::IsTerminator(subject)) {
			auto data = subject->GetData();
			if (!data->GetExpression()->IsConstant())
				return false;
			else
				subject = subject->GetNext();
		}
		return true;
	} else {
		return false;
	}
}

const ErrorListRef WithExpression::Validate(
		const_shared_ptr<ExecutionContext> execution_context) const {
	ErrorListRef errors = m_source_expression->Validate(execution_context);

	if (errors == ErrorList::GetTerminator()) {
		const_shared_ptr<TypeSpecifier> type_specifier =
				m_source_expression->GetType(execution_context);

		const_shared_ptr<CompoundTypeSpecifier> as_compound =
				std::dynamic_pointer_cast<const CompoundTypeSpecifier>(
						type_specifier);
		if (as_compound) {
			const string type_name = as_compound->GetTypeName();
			const_shared_ptr<CompoundType> type =
					execution_context->GetTypeTable()->GetType(type_name);

			if (type != CompoundType::GetDefaultCompoundType()) {
				MemberInstantiationListRef instantiation_list =
						m_member_instantiation_list;
				while (!MemberInstantiationList::IsTerminator(
						instantiation_list)) {
					const_shared_ptr<MemberInstantiation> instantiation =
							instantiation_list->GetData();
					auto member_name = instantiation->GetName();
					const_shared_ptr<MemberDefinition> member_definition =
							type->GetMember(*member_name);

					if (member_definition
							!= MemberDefinition::GetDefaultMemberDefinition()) {
						const_shared_ptr<TypeSpecifier> member_type =
								member_definition->GetType();
						const_shared_ptr<TypeSpecifier> expression_type =
								instantiation->GetExpression()->GetType(
										execution_context);
						if (!expression_type->IsAssignableTo(member_type)) {
							//undefined member
							errors =
									ErrorList::From(
											make_shared<Error>(Error::SEMANTIC,
													Error::ASSIGNMENT_TYPE_ERROR,
													instantiation->GetExpressionPosition().begin.line,
													instantiation->GetExpressionPosition().begin.column,
													member_type->ToString(),
													expression_type->ToString()),
											errors);
						}
					} else {
						//undefined member
						errors =
								ErrorList::From(
										make_shared<Error>(Error::SEMANTIC,
												Error::UNDECLARED_MEMBER,
												instantiation->GetNamePosition().begin.line,
												instantiation->GetNamePosition().begin.column,
												*member_name,
												member_definition->GetType()->ToString()),
										errors);

					}

					instantiation_list = instantiation_list->GetNext();
				}
			} else {
				errors = ErrorList::From(
						make_shared<Error>(Error::SEMANTIC,
								Error::UNDECLARED_TYPE,
								m_source_expression->GetPosition().begin.line,
								m_source_expression->GetPosition().begin.column,
								type_name), errors);
			}
		} else {
			errors = ErrorList::From(
					make_shared<Error>(Error::SEMANTIC,
							Error::VARIABLE_NOT_A_COMPOUND_TYPE,
							m_source_expression->GetPosition().begin.line,
							m_source_expression->GetPosition().begin.column),
					errors);
		}
	}

	return errors;
}
