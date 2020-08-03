#pragma once

#include <memory>
#include <string>
#include <variant>
#include <vector>

class Statement {
public:
	virtual ~Statement() {};
};

class Expression : public Statement {
public:
	virtual ~Expression() {}
};

class CompoundStatement final : public Statement {
public:
	CompoundStatement(std::vector<std::unique_ptr<Statement>>&& stmts);

private:
	std::vector<std::unique_ptr<Statement>> m_statements;
};

class DeclarationStatement : public Statement {
public:
	virtual ~DeclarationStatement() {}
};

enum class Type {
	VOID = 0,
	BOOL,
	INT,
	FLOAT
};

class VariableDeclaration final : public DeclarationStatement {
public:
	VariableDeclaration(Type type, std::string&& id, std::unique_ptr<Expression> init_value);

private:
	Type m_type;
	std::string m_id;
	std::unique_ptr<Expression> m_initial_value;
};

struct TypedId {
	Type type;
	std::string id;
};

class FunctionDeclaration final : public DeclarationStatement {
public:
	FunctionDeclaration(Type ret_type, std::string&& id, std::vector<TypedId>&& params, std::unique_ptr<CompoundStatement>&& stmt);

private:
	std::vector<TypedId> m_parameters;
	std::unique_ptr<CompoundStatement> m_statement;
	std::string m_id;
	Type m_ret_type;
};

class IfStatement final : public Statement {
public:
	IfStatement(std::unique_ptr<Expression> condition, std::unique_ptr<Statement> statement);

private:
	std::unique_ptr<Expression> m_condition;
	std::unique_ptr<Statement> m_statement;
};

class ForStatement final : public Statement {
public:
	ForStatement(std::unique_ptr<VariableDeclaration> var_decl, std::unique_ptr<Expression> condition_expr, std::unique_ptr<Expression> loop_expr, std::unique_ptr<Statement> stmt);

private:
	std::unique_ptr<VariableDeclaration> m_variable_declaration;
	std::unique_ptr<Expression> m_condition_expr;
	std::unique_ptr<Expression> m_loop_expr;
	std::unique_ptr<Statement> m_statement;
};

class WhileStatement final : public Statement {
public:
	WhileStatement(std::unique_ptr<Expression> condition_expr, std::unique_ptr<Statement> statement);

private:
	std::unique_ptr<Expression> m_condition_expr;
	std::unique_ptr<Statement> m_statement;
};

class ReturnStatement final : public Statement {
public:
	ReturnStatement(std::unique_ptr<Expression> return_expr);

private:
	std::unique_ptr<Expression> m_return_expr;
};

enum class BinaryOp {
	COMA,
	LOGICAL_OR,
	LOGICAL_AND,
	LOGICAL_EQUAL,
	LOGICAL_NOT_EQUAL,
	LESS,
	GREATER,
	LESS_EQUAL,
	GREATER_EQUAL,
	PLUS,
	MINUS,
	MULTIPLY,
	DIVIDE
};

enum class UnaryOp {
	PRE_INCREMENT,
	PRE_DECREMENT,
	CAST
};

class BinaryExpression final : public Expression {
public:
	BinaryExpression(BinaryOp op, std::unique_ptr<Expression> left, std::unique_ptr<Expression> rigth);

private:
	BinaryOp m_op;
	std::unique_ptr<Expression> m_left;
	std::unique_ptr<Expression> m_right;
};

class UnaryExpression final : public Expression {
public:
	UnaryExpression(UnaryOp op, std::unique_ptr<Expression> expr);

private:
	UnaryOp m_op;
	std::unique_ptr<Expression> m_expr;
};

class Atom : public Expression {
public:
	virtual ~Atom() {}
};

class IdAtom final : public Atom {
public:
	IdAtom(std::string&& id);

private:
	std::string m_id;
};

class FuncCallAtom final : public Atom {
public:
	FuncCallAtom(std::string&& id, std::vector<std::unique_ptr<Expression>>&& arguments);

private:
	std::string m_id;
	std::vector<std::unique_ptr<Expression>> m_arguments;
};

class Literal : public Atom {
public:
	virtual ~Literal() {}
};

class IntLiteral final : public Literal {
public:
	IntLiteral(int integer);

private:
	int m_integer;
};

class FloatLiteral final : public Literal {
public:
	FloatLiteral(float floating);

private:
	float m_floating;
};

class BoolLiteral final : public Literal {
public:
	BoolLiteral(bool boolean);

private:
	bool m_boolean;
};

class TranslationUnit {
public:
	TranslationUnit(std::vector<std::unique_ptr<Statement>>&& statements);

private:
	std::vector<std::unique_ptr<Statement>> m_statements;
};