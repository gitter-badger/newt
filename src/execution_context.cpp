/*
 * execution_context.cpp
 *
 *  Created on: Jun 20, 2015
 *      Author: caleb
 */

#include <execution_context.h>
#include <symbol_table.h>

ExecutionContext::ExecutionContext(const SymbolTable* symbol_table) :
		m_symbol_table(symbol_table) {
}

ExecutionContext::ExecutionContext() :
		m_symbol_table(new SymbolTable()) {
}

ExecutionContext::~ExecutionContext() {
}
