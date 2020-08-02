#include "AST.h"

CompoundStatement::CompoundStatement(std::vector<std::unique_ptr<Statement>>&& stmts)
	: m_statements(std::move(stmts)) {}

VariableDeclaration::VariableDeclaration(Type type, std::string&& id, std::unique_ptr<Expression> init_value)
	: m_type(type), m_id(std::move(id)), m_initial_value(std::move(init_value)) {}

FunctionDeclaration::FunctionDeclaration(Type ret_type, std::string&& id, std::vector<TypedId>&& params, std::unique_ptr<CompoundStatement>&& stmt)
	: m_ret_type(ret_type), m_id(std::move(id)), m_parameters(std::move(params)), m_statement(std::move(stmt)) {}

IfStatement::IfStatement(std::unique_ptr<Expression> condition, std::unique_ptr<Statement> statement) :
	m_condition(std::move(condition)), m_statement(std::move(statement)) {}

ForStatement::ForStatement(std::unique_ptr<VariableDeclaration> var_decl, std::unique_ptr<Expression> condition_expr, std::unique_ptr<Expression> loop_expr, std::unique_ptr<Statement> stmt) :
		m_variable_declaration(std::move(var_decl)), m_condition_expr(std::move(condition_expr)), m_loop_expr(std::move(loop_expr)), m_statement(std::move(stmt)) {}

WhileStatement::WhileStatement(std::unique_ptr<Expression> condition_expr, std::unique_ptr<Statement> statement) :
	m_condition_expr(std::move(condition_expr)), m_statement(std::move(statement)) {}

ReturnStatement::ReturnStatement(std::unique_ptr<Expression> return_expr) :
	m_return_expr(std::move(return_expr)) {}

BinaryExpression::BinaryExpression(BinaryOp op, std::unique_ptr<Expression> left, std::unique_ptr<Expression> right)
	: m_op(op), m_left(std::move(left)), m_right(std::move(right)) {}

UnaryExpression::UnaryExpression(UnaryOp op, std::unique_ptr<Expression> expr)
	: m_op(op), m_expr(std::move(expr)) {}

IntLiteral::IntLiteral(int integer) :
	m_integer(integer) {}

FloatLiteral::FloatLiteral(float floating) :
	m_floating(floating) {}

BoolLiteral::BoolLiteral(bool boolean) :
	m_boolean(boolean) {}

TranslationUnit::TranslationUnit(std::vector<std::unique_ptr<Statement>>&& statements)
	: m_statements(std::move(statements)) {}