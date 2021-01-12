#include<Tools/MessageLibrary.hpp>

#include<Compiler/NodeInfo.hpp>

#include<Components/cmp_if_lt_op_s.hpp>
#include<Components/cmp_if_gt_op_s.hpp>
#include<Components/cmp_add_op_s.hpp>
#include<Components/cmp_sub_op_s.hpp>
#include<Components/cmp_mult_op_s.hpp>
#include<Components/cmp_div_op_s.hpp>

const string NodeInfo::Operation_UnknownOperation = "";
const string NodeInfo::Operation_LessThan = "<";
const string NodeInfo::Operation_LessEqualThan = "<=";
const string NodeInfo::Operation_Equal = "==";
const string NodeInfo::Operation_GreaterEqualThan = ">=";
const string NodeInfo::Operation_GreaterThan = ">";
const string NodeInfo::Operation_Different = "!=";
const string NodeInfo::Operation_Sum = "+";
const string NodeInfo::Operation_CompoundSum = "++";
const string NodeInfo::Operation_CompoundSum2 = "+=";
const string NodeInfo::Operation_Subtraction = "-";
const string NodeInfo::Operation_CompoundSubtraction = "--";
const string NodeInfo::Operation_CompoundSubtraction2 = "-=";
const string NodeInfo::Operation_Multiplication = "*";
const string NodeInfo::Operation_CompoundMultiplication = "*=";
const string NodeInfo::Operation_Division = "/";
const string NodeInfo::Operation_CompoundDivision = "/=";
const string NodeInfo::Operation_Mod = "%";
const string NodeInfo::Operation_Attribution = "=";

const string NodeInfo::Operation_And = "&&";
const string NodeInfo::Operation_Or = "||";
const string NodeInfo::Operation_Not = "!";

NodeInfo::NodeInfo(CXCursor cursor, CXCursor parentCursor, CXClientData clientData)
{
    SetCursor(cursor);
    SetParentCursor(parentCursor);
    SetClientData(ClientData);
}

void NodeInfo::SetCursor(CXCursor cursor)
{
    Cursor = cursor;
}

CXCursor NodeInfo::GetCursor()
{
    return Cursor;
}

void NodeInfo::SetParentCursor(CXCursor parentCursor)
{
    ParentCursor = parentCursor;
}

CXCursor NodeInfo::GetParentCursor()
{
    return ParentCursor;
}

CXCursorKind NodeInfo::GetCursorKind()
{
    return clang_getCursorKind(Cursor);
}

CXType NodeInfo::GetCursorType()
{
    return clang_getCursorType(Cursor);
}

CXTypeKind NodeInfo::GetCursorTypeKind()
{
    return clang_getCursorType(Cursor).kind;
}

CXCursorKind NodeInfo::GetParentCursorKind()
{
    return clang_getCursorKind(ParentCursor);
}

CXType NodeInfo::GetParentCursorType()
{
    return clang_getCursorType(ParentCursor);
}

CXTypeKind NodeInfo::GetParentCursorTypeKind()
{
    return clang_getCursorType(ParentCursor).kind;
}

bool NodeInfo::IsFromMainFile()
{
    return clang_Location_isFromMainFile(GetCursorLocation()) == 0;
}

bool NodeInfo::IsParentFromMainFile()
{
    return clang_Location_isFromMainFile(GetParentCursorLocation()) == 0;
}

string NodeInfo::GetName()
{
    CXString cursorName = clang_getCursorSpelling(Cursor);
	string cursorStringName = clang_getCString(cursorName);
	clang_disposeString(cursorName);
	return cursorStringName;
}

string NodeInfo::GetParentName()
{
    CXString cursorName = clang_getCursorSpelling(ParentCursor);
	string cursorStringName = clang_getCString(cursorName);
	clang_disposeString(cursorName);
	return cursorStringName;
}

unsigned NodeInfo::GetCursorHash()
{
    return clang_hashCursor(Cursor);
}

unsigned NodeInfo::GetParentCursorHash()
{
    return clang_hashCursor(ParentCursor);
}

void NodeInfo::SetCursorHistory(vector<NodeInfo*>* cursorHistory)
{
    CursorHistory = cursorHistory;
}

vector<NodeInfo*>* NodeInfo::GetCursorHistory()
{
    return CursorHistory;
}

NodeInfo* NodeInfo::GetCursorHistory(int idx)//Positive numbers counts from 0 to the number. Negative numbers counts from the last element decreasing the number to the quantity.
{
    if((idx >= 0 ? idx : -idx) < CursorHistory->size())
    {
        return (*CursorHistory)[(idx >= 0 ? idx : CursorHistory->size() - 1 + idx)];
    }
    else
    {
        return NULL;
    }
}

int NodeInfo::GetCursorDepth()
{
    return GetCursorHistory()->size() + 1;
}

int NodeInfo::GetParentCursorDepth()
{
    return NodeInfo::GetPreviousHistory(GetCursorHistory(), GetParentCursor())->size();
}

vector<CXToken> NodeInfo::GetCursorTokens()
{
    return NodeInfo::GetTokensFromCursor(GetCursor());
}

vector<CXToken> NodeInfo::GetParentCursorTokens()
{
    return NodeInfo::GetTokensFromCursor(GetParentCursor());
}

void NodeInfo::Push(vector<NodeInfo*>* vec, NodeInfo* info)
{
    vec->push_back(info);
}

NodeInfo* NodeInfo::GetLast(vector<NodeInfo*>* vec)
{
    return vec->back();
}

NodeInfo* NodeInfo::Pop(vector<NodeInfo*>* vec)
{
    NodeInfo* info = vec->back();
    vec->pop_back();
    return info;
}

NodeInfo* NodeInfo::Remove(vector<NodeInfo*>* vec, int pos)
{
    NodeInfo* info = (*vec)[pos];
    vec->erase(vec->begin() + pos);
    return info;
}

NodeInfo* NodeInfo::Get(vector<NodeInfo*>* vec, int pos)
{
    return (*vec)[pos];
}

int NodeInfo::Get(vector<NodeInfo*>* vec, CXCursor cursor)
{
    for(int i = 0; i < vec->size(); i++)
    {
        if(clang_equalCursors(cursor, (*vec)[i]->GetCursor()) == 1)
        {
            return i;
        }
    }
    return -1;
}

vector<NodeInfo*>* NodeInfo::GetPreviousHistory(vector<NodeInfo*>* vec, CXCursor cursor)
{
    vector<NodeInfo*>* ret = new vector<NodeInfo*>;

    int idx = NodeInfo::Get(vec, cursor);

    while(idx >= 0)
    {
        NodeInfo* info = NodeInfo::Get(vec, idx);
        NodeInfo::Push(ret, info);
        idx = NodeInfo::Get(vec, info->GetParentCursor());
    }

    return ret;
}

CXSourceLocation NodeInfo::GetCursorLocation()
{
    return clang_getCursorLocation(Cursor);
}

CXSourceLocation NodeInfo::GetParentCursorLocation()
{
    return clang_getCursorLocation(ParentCursor);
}

void NodeInfo::SetClientData(CXClientData clientData)
{
    ClientData = (int*) clientData;
}

int* NodeInfo::GetClientData()
{
    return ClientData == NULL ? 0 : ClientData;
}

int* NodeInfo::GetNextClientData()
{
    return ClientData == NULL ? 0 : ClientData;
}

void NodeInfo::PrintTokenInfo(CXTranslationUnit translationUnit, CXToken currentToken)
{
    CXString tokenString = clang_getTokenSpelling(translationUnit, currentToken);
    CXTokenKind kind = clang_getTokenKind(currentToken);

    switch (kind)
    {
        case CXToken_Comment:
            cout << "Token : " << clang_getCString(tokenString) << "\t| COMMENT" << endl;
            break;
        case CXToken_Identifier:
            cout << "Token : " << clang_getCString(tokenString) << " \t| IDENTIFIER" << endl;
            break;
        case CXToken_Keyword:
            cout << "Token : " << clang_getCString(tokenString) << " \t| KEYWORD" << endl;
            break;
        case CXToken_Literal:
            cout << "Token : " << clang_getCString(tokenString) << " \t| LITERAL" << endl;
            break;
        case CXToken_Punctuation:
            cout << "Token : " << clang_getCString(tokenString) << " \t| PUNCTUATION" << endl;
            break;
        default:
            break;
    }
}

void NodeInfo::PrintCursorTokens(CXCursor currentCursor)
{
    CXTranslationUnit translationUnit = clang_Cursor_getTranslationUnit(currentCursor);

    CXToken *tokens;
    unsigned int nbTokens;
    CXSourceRange srcRange;

    srcRange = clang_getCursorExtent(currentCursor);

    clang_tokenize(translationUnit, srcRange, &tokens, &nbTokens);

    cout << "Begin of tokens print:" << endl;
    for (int i = 0; i < nbTokens; ++i)
    {
        CXToken currentToken = tokens[i];

        NodeInfo::PrintTokenInfo(translationUnit,currentToken);
    }
    cout << "End of tokens print." << endl;

    clang_disposeTokens(translationUnit,tokens,nbTokens);
}

string NodeInfo::GetTokenName(CXTranslationUnit translationUnit, CXToken token)
{
    CXString tokenString = clang_getTokenSpelling(translationUnit, token);
    string ret = clang_getCString(tokenString);
    clang_disposeString(tokenString);
    return ret;
}

string NodeInfo::GetTokenFromCursor(CXCursor cursor, int tokenNumber)
{
    CXTranslationUnit translationUnit = clang_Cursor_getTranslationUnit(cursor);
    CXToken* tokens;
    unsigned int nbTokens;
    CXSourceRange srcRange;

    srcRange = clang_getCursorExtent(cursor);
    clang_tokenize(translationUnit, srcRange, &tokens, &nbTokens);

    string tokenName;

    if(tokenNumber < 0)
    {
        tokenName = NodeInfo::GetTokenName(translationUnit, tokens[nbTokens + tokenNumber]);
    }
    else
    {
        tokenName = NodeInfo::GetTokenName(translationUnit, tokens[tokenNumber]);
    }

    clang_disposeTokens(translationUnit,tokens,nbTokens);

    return tokenName;
}

vector<CXToken> NodeInfo::GetTokensFromCursor(CXCursor cursor)
{
    CXToken* tokens;
    unsigned int nbTokens;
    CXSourceRange srcRange;

    CXTranslationUnit tu = clang_Cursor_getTranslationUnit(cursor);
    srcRange = clang_getCursorExtent(cursor);
    clang_tokenize(tu, srcRange, &tokens, &nbTokens);

    vector<CXToken> ret;

    for(unsigned int i = 0; i < nbTokens; i++)
    {
        ret.push_back(tokens[i]);
    }

    clang_disposeTokens(tu,tokens,nbTokens);

    return ret;
}

bool NodeInfo::IsArray(CXType cursorType)
{
    if(cursorType.kind == CXType_ConstantArray | cursorType.kind == CXType_VariableArray | cursorType.kind == CXType_DependentSizedArray)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void NodeInfo::PrintHistory(vector<NodeInfo*>* vec)
{
    for(int i = 0; i < vec->size(); i++)
    {
        CXToken *tokens;
        unsigned int nbTokens;
        CXSourceRange srcRange;

        CXTranslationUnit translationUnit = clang_Cursor_getTranslationUnit((*vec)[i]->GetCursor());
        srcRange = clang_getCursorExtent((*vec)[i]->GetCursor());

        clang_tokenize(translationUnit, srcRange, &tokens, &nbTokens);

        NodeInfo* info = (*vec)[i];

        NodeInfo::PrintCursorInfo("Begin of tokens print from history [" + to_string(i) + "] => ",(*vec)[i]);
        for (int i = 0; i < nbTokens; ++i)
        {
            CXToken currentToken = tokens[i];

            NodeInfo::PrintTokenInfo(translationUnit,currentToken);
        }
        NodeInfo::PrintCursorInfo("End of tokens print from history [" + to_string(i) + "] => ",(*vec)[i]);

        clang_disposeTokens(translationUnit,tokens,nbTokens);
    }
}

void NodeInfo::PrintCursorInfo(NodeInfo* info)
{
    cout << "Cursor: '" << info->GetName() << "' (" << NodeInfo::GetCursorKindName(info->GetCursorKind()) << ", Kind: " << info->GetCursorKind() << ", Type: " << info->GetCursorType().kind << ", Hash: " << clang_hashCursor(info->GetCursor()) << ", Depth: " << info->GetCursorDepth() << ")" <<
        " - ParentCursor: '" << info->GetParentName() << "' (" << NodeInfo::GetCursorKindName(info->GetParentCursorKind()) << ", Kind: " << info->GetParentCursorKind() << ", Type: " << info->GetParentCursorType().kind << ", Hash: " << clang_hashCursor(info->GetParentCursor()) << ", Depth: " << info->GetParentCursorDepth() << ")" <<
    endl;
}

void NodeInfo::PrintCursorInfo(string previousText, NodeInfo* info)
{
    cout << previousText;
    PrintCursorInfo(info);
}

void NodeInfo::PrintHistoryNames(vector<NodeInfo*>* vec)
{
    for(int i = 0; i < vec->size(); i++)
    {
        NodeInfo::PrintCursorInfo("", (*vec)[i]);
    }
}

string NodeInfo::GetCursorKindName(CXCursorKind kind)
{
    switch(kind)
    {
        case 1:
            return "CXCursor_UnexposedDecl";
        case 2:
            return "CXCursor_StructDecl";
        case 3:
            return "CXCursor_UnionDecl";
        case 4:
            return "CXCursor_ClassDecl";
        case 5:
            return "CXCursor_EnumDecl";
        case 6:
            return "CXCursor_FieldDecl";
        case 7:
            return "CXCursor_EnumConstantDecl";
        case 8:
            return "CXCursor_FunctionDecl";
        case 9:
            return "CXCursor_VarDecl";
        case 10:
            return "CXCursor_ParmDecl";
        case 11:
            return "CXCursor_ObjCInterfaceDecl";
        case 12:
            return "CXCursor_ObjCCategoryDecl";
        case 13:
            return "CXCursor_ObjCProtocolDecl";
        case 14:
            return "CXCursor_ObjCPropertyDecl";
        case 15:
            return "CXCursor_ObjCIvarDecl";
        case 16:
            return "CXCursor_ObjCInstanceMethodDecl";
        case 17:
            return "CXCursor_ObjCClassMethodDecl";
        case 18:
            return "CXCursor_ObjCImplementationDecl";
        case 19:
            return "CXCursor_ObjCCategoryImplDecl";
        case 20:
            return "CXCursor_TypedefDecl";
        case 21:
            return "CXCursor_CXXMethod";
        case 22:
            return "CXCursor_Namespace";
        case 23:
            return "CXCursor_LinkageSpec";
        case 24:
            return "CXCursor_Constructor";
        case 25:
            return "CXCursor_Destructor";
        case 26:
            return "CXCursor_ConversionFunction";
        case 27:
            return "CXCursor_TemplateTypeParameter";
        case 28:
            return "CXCursor_NonTypeTemplateParameter";
        case 29:
            return "CXCursor_TemplateTemplateParameter";
        case 30:
            return "CXCursor_FunctionTemplate";
        case 31:
            return "CXCursor_ClassTemplate";
        case 32:
            return "CXCursor_ClassTemplatePartialSpecialization";
        case 33:
            return "CXCursor_NamespaceAlias";
        case 34:
            return "CXCursor_UsingDirective";
        case 35:
            return "CXCursor_UsingDeclaration";
        case 36:
            return "CXCursor_TypeAliasDecl";
        case 37:
            return "CXCursor_ObjCSynthesizeDecl";
        case 38:
            return "CXCursor_ObjCDynamicDecl";
        case 39:
            return "CXCursor_CXXAccessSpecifier";
        case 40:
            return "CXCursor_FirstRef";
        case 41:
            return "CXCursor_ObjCProtocolRef";
        case 42:
            return "CXCursor_ObjCClassRef";
        case 43:
            return "CXCursor_TypeRef";
        case 44:
            return "CXCursor_CXXBaseSpecifier";
        case 45:
            return "CXCursor_TemplateRef";
        case 46:
            return "CXCursor_NamespaceRef";
        case 47:
            return "CXCursor_MemberRef";
        case 48:
            return "CXCursor_LabelRef";
        case 49:
            return "CXCursor_OverloadedDeclRef";
        case 50:
            return "CXCursor_VariableRef";
        case 70:
            return "CXCursor_InvalidFile";
        case 71:
            return "CXCursor_NoDeclFound";
        case 72:
            return "CXCursor_NotImplemented";
        case 73:
            return "CXCursor_InvalidCode";
        case 100:
            return "CXCursor_FirstExpr";
        case 101:
            return "CXCursor_DeclRefExpr";
        case 102:
            return "CXCursor_MemberRefExpr";
        case 103:
            return "CXCursor_CallExpr";
        case 104:
            return "CXCursor_ObjCMessageExpr";
        case 105:
            return "CXCursor_BlockExpr";
        case 106:
            return "CXCursor_IntegerLiteral";
        case 107:
            return "CXCursor_FloatingLiteral";
        case 108:
            return "CXCursor_ImaginaryLiteral";
        case 109:
            return "CXCursor_StringLiteral";
        case 110:
            return "CXCursor_CharacterLiteral";
        case 111:
            return "CXCursor_ParenExpr";
        case 112:
            return "CXCursor_UnaryOperator";
        case 113:
            return "CXCursor_ArraySubscriptExpr";
        case 114:
            return "CXCursor_BinaryOperator";
        case 115:
            return "CXCursor_CompoundAssignOperator";
        case 116:
            return "CXCursor_ConditionalOperator";
        case 117:
            return "CXCursor_CStyleCastExpr";
        case 118:
            return "CXCursor_CompoundLiteralExpr";
        case 119:
            return "CXCursor_InitListExpr";
        case 120:
            return "CXCursor_AddrLabelExpr";
        case 121:
            return "CXCursor_StmtExpr";
        case 122:
            return "CXCursor_GenericSelectionExpr";
        case 123:
            return "CXCursor_GNUNullExpr";
        case 124:
            return "CXCursor_CXXStaticCastExpr";
        case 125:
            return "CXCursor_CXXDynamicCastExpr";
        case 126:
            return "CXCursor_CXXReinterpretCastExpr";
        case 127:
            return "CXCursor_CXXConstCastExpr";
        case 128:
            return "CXCursor_CXXFunctionalCastExpr";
        case 129:
            return "CXCursor_CXXTypeidExpr";
        case 130:
            return "CXCursor_CXXBoolLiteralExpr";
        case 131:
            return "CXCursor_CXXNullPtrLiteralExpr";
        case 132:
            return "CXCursor_CXXThisExpr";
        case 133:
            return "CXCursor_CXXThrowExpr";
        case 134:
            return "CXCursor_CXXNewExpr";
        case 135:
            return "CXCursor_CXXDeleteExpr";
        case 136:
            return "CXCursor_UnaryExpr";
        case 137:
            return "CXCursor_ObjCStringLiteral";
        case 138:
            return "CXCursor_ObjCEncodeExpr";
        case 139:
            return "CXCursor_ObjCSelectorExpr";
        case 140:
            return "CXCursor_ObjCProtocolExpr";
        case 141:
            return "CXCursor_ObjCBridgedCastExpr";
        case 142:
            return "CXCursor_PackExpansionExpr";
        case 143:
            return "CXCursor_SizeOfPackExpr";
        case 144:
            return "CXCursor_LambdaExpr";
        case 145:
            return "CXCursor_ObjCBoolLiteralExpr";
        case 146:
            return "CXCursor_ObjCSelfExpr";
        case 147:
            return "CXCursor_OMPArraySectionExpr";
        case 148:
            return "CXCursor_ObjCAvailabilityCheckExpr";
        case 200:
            return "CXCursor_FirstStmt";
        case 201:
            return "CXCursor_LabelStmt";
        case 202:
            return "CXCursor_CompoundStmt";
        case 203:
            return "CXCursor_CaseStmt";
        case 204:
            return "CXCursor_DefaultStmt";
        case 205:
            return "CXCursor_IfStmt";
        case 206:
            return "CXCursor_SwitchStmt";
        case 207:
            return "CXCursor_WhileStmt";
        case 208:
            return "CXCursor_DoStmt";
        case 209:
            return "CXCursor_ForStmt";
        case 210:
            return "CXCursor_GotoStmt";
        case 211:
            return "CXCursor_IndirectGotoStmt";
        case 212:
            return "CXCursor_ContinueStmt";
        case 213:
            return "CXCursor_BreakStmt";
        case 214:
            return "CXCursor_ReturnStmt";
        case 215:
            return "CXCursor_GCCAsmStmt";
        case 216:
            return "CXCursor_ObjCAtTryStmt";
        case 217:
            return "CXCursor_ObjCAtCatchStmt";
        case 218:
            return "CXCursor_ObjCAtFinallyStmt";
        case 219:
            return "CXCursor_ObjCAtThrowStmt";
        case 220:
            return "CXCursor_ObjCAtSynchronizedStmt";
        case 221:
            return "CXCursor_ObjCAutoreleasePoolStmt";
        case 222:
            return "CXCursor_ObjCForCollectionStmt";
        case 223:
            return "CXCursor_CXXCatchStmt";
        case 224:
            return "CXCursor_CXXTryStmt";
        case 225:
            return "CXCursor_CXXForRangeStmt";
        case 226:
            return "CXCursor_SEHTryStmt";
        case 227:
            return "CXCursor_SEHExceptStmt";
        case 228:
            return "CXCursor_SEHFinallyStmt";
        case 229:
            return "CXCursor_MSAsmStmt";
        case 230:
            return "CXCursor_NullStmt";
        case 231:
            return "CXCursor_DeclStmt";
        case 232:
            return "CXCursor_OMPParallelDirective";
        case 233:
            return "CXCursor_OMPSimdDirective";
        case 234:
            return "CXCursor_OMPForDirective";
        case 235:
            return "CXCursor_OMPSectionsDirective";
        case 236:
            return "CXCursor_OMPSectionDirective";
        case 237:
            return "CXCursor_OMPSingleDirective";
        case 238:
            return "CXCursor_OMPParallelForDirective";
        case 239:
            return "CXCursor_OMPParallelSectionsDirective";
        case 240:
            return "CXCursor_OMPTaskDirective";
        case 241:
            return "CXCursor_OMPMasterDirective";
        case 242:
            return "CXCursor_OMPCriticalDirective";
        case 243:
            return "CXCursor_OMPTaskyieldDirective";
        case 244:
            return "CXCursor_OMPBarrierDirective";
        case 245:
            return "CXCursor_OMPTaskwaitDirective";
        case 246:
            return "CXCursor_OMPFlushDirective";
        case 247:
            return "CXCursor_SEHLeaveStmt";
        case 248:
            return "CXCursor_OMPOrderedDirective";
        case 249:
            return "CXCursor_OMPAtomicDirective";
        case 250:
            return "CXCursor_OMPForSimdDirective";
        case 251:
            return "CXCursor_OMPParallelForSimdDirective";
        case 252:
            return "CXCursor_OMPTargetDirective";
        case 253:
            return "CXCursor_OMPTeamsDirective";
        case 254:
            return "CXCursor_OMPTaskgroupDirective";
        case 255:
            return "CXCursor_OMPCancellationPointDirective";
        case 256:
            return "CXCursor_OMPCancelDirective";
        case 257:
            return "CXCursor_OMPTargetDataDirective";
        case 258:
            return "CXCursor_OMPTaskLoopDirective";
        case 259:
            return "CXCursor_OMPTaskLoopSimdDirective";
        case 260:
            return "CXCursor_OMPDistributeDirective";
        case 261:
            return "CXCursor_OMPTargetEnterDataDirective";
        case 262:
            return "CXCursor_OMPTargetExitDataDirective";
        case 263:
            return "CXCursor_OMPTargetParallelDirective";
        case 264:
            return "CXCursor_OMPTargetParallelForDirective";
        case 265:
            return "CXCursor_OMPTargetUpdateDirective";
        case 266:
            return "CXCursor_OMPDistributeParallelForDirective";
        case 267:
            return "CXCursor_OMPDistributeParallelForSimdDirective";
        case 268:
            return "CXCursor_OMPDistributeSimdDirective";
        case 269:
            return "CXCursor_OMPTargetParallelForSimdDirective";
        case 270:
            return "CXCursor_OMPTargetSimdDirective";
        case 271:
            return "CXCursor_OMPTeamsDistributeDirective";
        case 272:
            return "CXCursor_OMPTeamsDistributeSimdDirective";
        case 273:
            return "CXCursor_OMPTeamsDistributeParallelForSimdDirective";
        case 274:
            return "CXCursor_OMPTeamsDistributeParallelForDirective";
        case 275:
            return "CXCursor_OMPTargetTeamsDirective";
        case 276:
            return "CXCursor_OMPTargetTeamsDistributeDirective";
        case 277:
            return "CXCursor_OMPTargetTeamsDistributeParallelForDirective";
        case 278:
            return "CXCursor_OMPTargetTeamsDistributeParallelForSimdDirective";
        case 279:
            return "CXCursor_OMPTargetTeamsDistributeSimdDirective";
        case 300:
            return "CXCursor_TranslationUnit";
        case 400:
            return "CXCursor_FirstAttr";
        case 401:
            return "CXCursor_IBActionAttr";
        case 402:
            return "CXCursor_IBOutletAttr";
        case 403:
            return "CXCursor_IBOutletCollectionAttr";
        case 404:
            return "CXCursor_CXXFinalAttr";
        case 405:
            return "CXCursor_CXXOverrideAttr";
        case 406:
            return "CXCursor_AnnotateAttr";
        case 407:
            return "CXCursor_AsmLabelAttr";
        case 408:
            return "CXCursor_PackedAttr";
        case 409:
            return "CXCursor_PureAttr";
        case 410:
            return "CXCursor_ConstAttr";
        case 411:
            return "CXCursor_NoDuplicateAttr";
        case 412:
            return "CXCursor_CUDAConstantAttr";
        case 413:
            return "CXCursor_CUDADeviceAttr";
        case 414:
            return "CXCursor_CUDAGlobalAttr";
        case 415:
            return "CXCursor_CUDAHostAttr";
        case 416:
            return "CXCursor_CUDASharedAttr";
        case 417:
            return "CXCursor_VisibilityAttr";
        case 418:
            return "CXCursor_DLLExport";
        case 419:
            return "CXCursor_DLLImport";
        case 500:
            return "CXCursor_PreprocessingDirective";
        case 501:
            return "CXCursor_MacroDefinition";
        case 502:
            return "CXCursor_MacroExpansion";
        case 503:
            return "CXCursor_InclusionDirective";
        case 600:
            return "CXCursor_ModuleImportDecl";
        case 601:
            return "CXCursor_TypeAliasTemplateDecl";
        case 602:
            return "CXCursor_StaticAssert";
        case 603:
            return "CXCursor_FriendDecl";
        case 700:
            return "CXCursor_OverloadCandidate";
    }
}

Component* NodeInfo::GetComponent(string operation, int width)
{
    Component* op = NULL;

    if(operation.compare(NodeInfo::Operation_LessThan) == 0)
    {
        op = new Cmp_if_lt_op_s(width);
        op->SetComponentName("if_lt_op_s");
        op->GetMap("w_in1")->SetValue(to_string(width));
        op->GetMap("w_in2")->SetValue(to_string(width));
        op->GetMap("w_out")->SetValue(to_string(1));
    }
    else if(operation.compare(NodeInfo::Operation_LessEqualThan) == 0)
    {
        throw invalid_argument("Unexpected binary operation '" + operation + "': " + to_string(MessageLibrary::ERROR_UNEXPECTED_OPERATION));
    }
    else if(operation.compare(NodeInfo::Operation_Equal) == 0)
    {
        throw invalid_argument("Unexpected binary operation '" + operation + "': " + to_string(MessageLibrary::ERROR_UNEXPECTED_OPERATION));
    }
    else if(operation.compare(NodeInfo::Operation_GreaterEqualThan) == 0)
    {
        throw invalid_argument("Unexpected binary operation '" + operation + "': " + to_string(MessageLibrary::ERROR_UNEXPECTED_OPERATION));
    }
    else if(operation.compare(NodeInfo::Operation_GreaterThan) == 0)
    {
        op = new Cmp_if_gt_op_s(width);
        op->SetComponentName("if_gt_op_s");
        op->GetMap("w_in1")->SetValue(to_string(width));
        op->GetMap("w_in2")->SetValue(to_string(width));
        op->GetMap("w_out")->SetValue(to_string(1));
    }
    else if(operation.compare(NodeInfo::Operation_Different) == 0)
    {
        throw invalid_argument("Unexpected binary operation '" + operation + "': " + to_string(MessageLibrary::ERROR_UNEXPECTED_OPERATION));
    }
    else if(operation.compare(NodeInfo::Operation_Sum) == 0 || operation.compare(NodeInfo::Operation_CompoundSum2) == 0)
    {
        op = new Cmp_add_op_s(width);
        op->SetComponentName("add_op_s");
        op->GetMap("w_in1")->SetValue(to_string(width));
        op->GetMap("w_in2")->SetValue(to_string(width));
        op->GetMap("w_out")->SetValue(to_string(width));
    }
    else if(operation.compare(NodeInfo::Operation_Subtraction) == 0 || operation.compare(NodeInfo::Operation_CompoundSubtraction2) == 0)
    {
        op = new Cmp_sub_op_s(width);
        op->SetComponentName("sub_op_s");
        op->GetMap("w_in1")->SetValue(to_string(width));
        op->GetMap("w_in2")->SetValue(to_string(width));
        op->GetMap("w_out")->SetValue(to_string(width));
    }
    else if(operation.compare(NodeInfo::Operation_Multiplication) == 0 || operation.compare(NodeInfo::Operation_CompoundMultiplication) == 0)
    {
        op = new Cmp_mult_op_s(width);
        op->SetComponentName("mult_op_s");
        op->GetMap("w_in1")->SetValue(to_string(width));
        op->GetMap("w_in2")->SetValue(to_string(width));
        op->GetMap("w_out")->SetValue(to_string(width));
    }
    else if(operation.compare(NodeInfo::Operation_Division) == 0 || operation.compare(NodeInfo::Operation_CompoundDivision) == 0)
    {
        op = new Cmp_div_op_s(width);
        op->SetComponentName("div_op_s");
        op->GetMap("w_in1")->SetValue(to_string(width));
        op->GetMap("w_in2")->SetValue(to_string(width));
        op->GetMap("w_out")->SetValue(to_string(width));
    }
    else if(operation.compare(NodeInfo::Operation_Mod) == 0)
    {
        throw invalid_argument("Unexpected binary operation '" + operation + "': " + to_string(MessageLibrary::ERROR_UNEXPECTED_OPERATION));
    }
    else if
    (
        operation.compare(NodeInfo::Operation_Attribution) != 0 &&
        operation.compare(NodeInfo::Operation_CompoundMultiplication) != 0 &&
        operation.compare(NodeInfo::Operation_CompoundDivision) != 0
    )
    {
        throw invalid_argument("Unexpected binary operation '" + operation + "': " + to_string(MessageLibrary::ERROR_UNEXPECTED_OPERATION));
    }

    return op;
}

Component* NodeInfo::GetCompoundComponent(string operation, int width)
{
    Component* op = NULL;

    if(operation.compare(NodeInfo::Operation_CompoundSum2) == 0)
    {
        op = new Cmp_add_op_s(width);
        op->SetComponentName("add_op_s");
    }
    else if(operation.compare(NodeInfo::Operation_CompoundSubtraction2) == 0)
    {
        op = new Cmp_sub_op_s(width);
        op->SetComponentName("sub_op_s");
    }
    else if(operation.compare(NodeInfo::Operation_CompoundMultiplication) == 0)
    {
        op = new Cmp_mult_op_s(width);
        op->SetComponentName("mult_op_s");
    }
    else// if(operation.compare(NodeInfo::Operation_CompoundDivision) == 0)
    {
        op = new Cmp_div_op_s(width);
        op->SetComponentName("div_op_s");
    }

    op->GetMap("w_in1")->SetValue(to_string(width));
    op->GetMap("w_in2")->SetValue(to_string(width));
    op->GetMap("w_out")->SetValue(to_string(width));

    return op;
}

string NodeInfo::GetOperation(CXCursor cursor)
{
    if(clang_getCursorKind(cursor) == CXCursor_BinaryOperator || clang_getCursorKind(cursor) == CXCursor_CompoundAssignOperator)
    {
        vector<CXToken> tokens = NodeInfo::GetTokensFromCursor(cursor);
        CXTranslationUnit translationUnit = clang_Cursor_getTranslationUnit(cursor);

        for(CXToken cxToken : tokens)
        {
            string token = NodeInfo::GetTokenName(translationUnit, cxToken);

            if
            (
               token.compare(NodeInfo::Operation_Sum) == 0 ||//operations
               token.compare(NodeInfo::Operation_Subtraction) == 0  ||
               token.compare(NodeInfo::Operation_Multiplication) == 0 ||
               token.compare(NodeInfo::Operation_Division) == 0 ||
               token.compare(NodeInfo::Operation_Mod) == 0 ||
               token.compare(NodeInfo::Operation_And) == 0 ||//boolean
               token.compare(NodeInfo::Operation_Or) == 0  ||
               token.compare(NodeInfo::Operation_Equal) == 0 ||
               token.compare(NodeInfo::Operation_Different) == 0 ||
               token.compare(NodeInfo::Operation_GreaterThan) == 0 ||
               token.compare(NodeInfo::Operation_GreaterEqualThan) == 0 ||
               token.compare(NodeInfo::Operation_LessThan) == 0 ||
               token.compare(NodeInfo::Operation_LessEqualThan) == 0 ||
               token.compare(NodeInfo::Operation_Not) == 0 ||
               token.compare(NodeInfo::Operation_CompoundSum2) == 0 ||//compound assign
               token.compare(NodeInfo::Operation_CompoundSubtraction2) == 0 ||
               token.compare(NodeInfo::Operation_CompoundMultiplication) == 0 ||
               token.compare(NodeInfo::Operation_CompoundDivision) == 0 ||
               token.compare(NodeInfo::Operation_Attribution) == 0//attribution
            )
            {
                return token;
            }
        }

        return "";
    }
    else
    {
        return "";
    }
}
