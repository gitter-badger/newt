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

#include <iostream>
#include <string.h>
#include <memory>

#include "error.h"
#include "symbol_table.h"
#include "type_table.h"

#include "driver.h"

using namespace std;

int get_exit_code(bool debug, int exit_code) {
	if (debug) {
		//return "success" so the test infrastructure doesn't barf
		return EXIT_SUCCESS;
	} else {
		return exit_code;
	}
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		cerr << "Input script must be specified." << endl;
		return 1;
	}

	bool debug = false;
	TRACE trace = NO_TRACE;
	for (int i = 1; i < argc - 1; i++) {
		if (strcmp(argv[i], "--debug") == 0) {
			debug = true;
		}

		if (strcmp(argv[i], "--trace-scanning") == 0) {
			trace = TRACE(trace | SCANNING);
		}

		if (strcmp(argv[i], "--trace-parsing") == 0) {
			trace = TRACE(trace | PARSING);
		}
	}

	char* filename = argv[argc - 1];

	if (debug) {
		cout << "Parsing file " << filename << "..." << endl;
	}

	Driver driver;
	int parse_result = driver.parse(filename, trace);

	if (parse_result != 0 || driver.GetErrorCount() != 0) {
		if (debug) {
			cout << "Parsed file " << filename << "." << endl;
		}

		cerr << driver.GetErrorCount() << " error";
		if (driver.GetErrorCount() > 1)
			cout << "s";
		cout << " found; giving up." << endl;

		return get_exit_code(debug, EXIT_FAILURE);
	}

	int exit_code = EXIT_SUCCESS;

	if (parse_result == 0) {
		auto main_statement_block = driver.GetStatementBlock();
		shared_ptr<ExecutionContext> root_context =
				make_shared<ExecutionContext>();
		ErrorListRef semantic_errors = main_statement_block->preprocess(
				root_context);

		if (ErrorList::IsTerminator(semantic_errors)) {
			if (debug) {
				cout << "Parsed file " << filename << "." << endl;
			}

			ErrorListRef execution_errors = main_statement_block->execute(
					root_context);

			bool has_execution_errors = false;
			while (!ErrorList::IsTerminator(execution_errors)) {
				has_execution_errors = true;
				cerr << execution_errors->GetData()->ToString() << endl;
				execution_errors = execution_errors->GetNext();
			}

			if (debug) {
				cout << "Root Symbol Table:" << endl;
				cout << "----------------" << endl;
				root_context->print(cout, *root_context->GetTypeTable(),
						Indent(0));
				cout << endl;
				cout << "Root Type Table:" << endl;
				cout << "----------------" << endl;
				root_context->GetTypeTable()->print(cout);
			}

			if (root_context->GetExitCode()) {
				exit_code = *root_context->GetExitCode();
			}

			return get_exit_code(debug,
					has_execution_errors ? EXIT_FAILURE : exit_code);
		} else {
			//reverse linked list of errors, which comes to us in reverse order
			semantic_errors = ErrorList::Reverse(semantic_errors);

			int semantic_error_count = 0;
			ErrorListRef error = semantic_errors;
			while (!ErrorList::IsTerminator(error)) {
				semantic_error_count++;
				cerr << *(error->GetData()) << endl;
				error = error->GetNext();
			}

			if (debug) {
				cout << "Parsed file " << filename << "." << endl;
			}

			if (semantic_error_count == 1) {
				cout << "1 error found; giving up." << endl;
			} else {
				cout << semantic_error_count << " errors found; giving up."
						<< endl;
			}

			return get_exit_code(debug, EXIT_FAILURE);
		}
	}
}
