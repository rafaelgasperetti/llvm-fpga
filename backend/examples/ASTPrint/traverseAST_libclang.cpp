// Code to traverse the clang AST using libclang (and not a clang plugin)

extern "C" {
	#include "clang-c/Index.h"
}
#include "llvm/Support/CommandLine.h"
#include <iostream>

using namespace llvm;

// Command line namespace, receives a single paramater that
// stores the bitcode filename
static cl::opt<std::string>

FileName(cl::Positional, cl::desc("Input file"), cl::Required);

/*
	•	Return CXChildVisit_Recurse when we want clang_visitChildren()
	to continue its AST traversal by visiting the children of the node we are
	currently in
	•	 Return CXChildVisit_Continue when we want it to continue visiting,
	but skip the children of the current node we are in
	•	 Return CXChildVisit_Break when we are satisfied and want clang_
	visitChildren() to no longer visit any more nodes
*/
enum CXChildVisitResult visitNode (CXCursor cursor, CXCursor parent,
						    	   CXClientData client_data) {
	
			
	if (clang_getCursorKind(cursor) == CXCursor_CXXMethod ||
		clang_getCursorKind(cursor) == CXCursor_FunctionDecl) {

		CXType cursorType = clang_getCursorType(cursor);

		CXString type = clang_getTypeKindSpelling(cursorType.kind);
		CXString name = clang_getCursorSpelling(cursor);
		CXSourceLocation loc = clang_getCursorLocation(cursor);
		CXString fName;
		unsigned line = 0, col = 0;
		clang_getPresumedLocation(loc, &fName, &line, &col);
		
		std::cout << clang_getCString(type) << ":" << clang_getCString(fName) << ":"
			<< line << ":"<< col << " declares "
			<< clang_getCString(name) << std::endl;
	*}

	if (clang_getCursorKind(cursor) == CXCursor_VarDecl) {
		CXSourceRange range = clang_getCursorExtent(cursor);
		CXSourceLocation location = clang_getRangeStart(range);

		CXFile file;
		unsigned line;
		unsigned column;
		clang_getFileLocation(location, &file, &line, &column, NULL);

		CXString filename = clang_getFileName(file);
		CXString name = clang_getCursorSpelling(cursor);

		fprintf(stderr, "%s:%u:%u: found variable %s\n", clang_getCString(filename), 
				line, column, clang_getCString(name));

		clang_disposeString(name);
		clang_disposeString(filename);
	}
	
	return CXChildVisit_Recurse;
}

int main(int argc, char** argv)
{
	cl::ParseCommandLineOptions(argc, argv, "AST Traversal Example");
	CXIndex index = clang_createIndex(0, 0);
	
	const char *args[] = {
		"-I/usr/include",
		"-I."
	};
	
	CXTranslationUnit translationUnit = clang_parseTranslationUnit
		(index, FileName.c_str(), args, 2, NULL, 0,
		 CXTranslationUnit_None);
	CXCursor cur = clang_getTranslationUnitCursor(translationUnit);
	
	// clang_visistChildren visits recursively all child nodes of the cursor
	// passed as a parameter, calling a callback function in each visit
	clang_visitChildren(cur, visitNode, NULL);
	clang_disposeTranslationUnit(translationUnit);
	clang_disposeIndex(index);
	
	return 0;
}
