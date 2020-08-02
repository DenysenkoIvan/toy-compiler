#include "Parser.h"

#include <iostream>

static void error(Position pos, std::string_view message) {
	std::cout << '[' << pos.line << ',' << pos.column << "] Error: " << message << '\n';
}

static Type parse_type(const std::string& type_str) {
	if (type_str == "int")
		return Type::INT;
	else if (type_str == "float")
		return Type::FLOAT;
	else if (type_str == "bool")
		return Type::BOOL;
	else
		return Type::VOID;
}

Parser::Parser(const std::filesystem::path& input_file)
	: m_tokenizer(input_file) {}

bool Parser::match_token(TokenKind kind) {
	if (m_tokenizer.current().kind() == kind) {
		m_tokenizer.next();
		return true;
	} else
		return false;
}

bool Parser::match_token(std::string_view str) {
	if (m_tokenizer.current().kind() == TokenKind::NAME && m_tokenizer.current().lexeme() == str) {
		m_tokenizer.next();
		return true;
	} else
		return false;
}

bool Parser::is_token(TokenKind kind) {
	return m_tokenizer.current().kind() == kind;
}

void Parser::next_token() {
	m_tokenizer.next();
}

std::string& Parser::get_token_lexeme() {
	return m_tokenizer.current().lexeme();
}

Position Parser::get_token_position() {
	return m_tokenizer.current().pos();
}

std::unique_ptr<TranslationUnit> Parser::parse() {
	return parse_translation_unit();
}

std::unique_ptr<TranslationUnit> Parser::parse_translation_unit() {
	std::vector<std::unique_ptr<Statement>> statements;
	std::unique_ptr<Statement> stmt;

	while (true) {
		stmt = parse_statement();

		if (!stmt) {
			if (m_tokenizer.current().kind() != TokenKind::EOS) {
				// TODO Error Messages
				error(m_tokenizer.current().pos(), "Expected statement");
				m_tokenizer.next();
				continue;
			} else
				break;
		}

		statements.push_back(std::move(stmt));
	}

	return std::make_unique<TranslationUnit>(std::move(statements));
}

std::unique_ptr<Statement> Parser::parse_statement() {
	std::unique_ptr<Statement> stmt;

	stmt = parse_compound_statement();
	if (stmt) return stmt;

	stmt = parse_expression_statement();
	if (stmt) return stmt;

	stmt = parse_declaration_statement();
	if (stmt) return stmt;

	stmt = parse_if_statement();
	if (stmt) return stmt;

	stmt = parse_for_statement();
	if (stmt) return stmt;

	stmt = parse_while_statement();
	if (stmt) return stmt;

	stmt = parse_return_statement();
	if (stmt) return stmt;

	return nullptr;
}

std::unique_ptr<CompoundStatement> Parser::parse_compound_statement() {
	if (match_token(TokenKind::LEFT_BRACE)) {
		std::vector<std::unique_ptr<Statement>> statements;

		while (true) {
			if (match_token(TokenKind::RIGHT_BRACE))
				return std::make_unique<CompoundStatement>(std::move(statements));
			else if (match_token(TokenKind::EOS)) {
				error(m_tokenizer.current().pos(), "Unexpected end of file");
				return std::make_unique<CompoundStatement>(std::move(statements));
			}

			statements.push_back(parse_statement());
		}

		return std::make_unique<CompoundStatement>(std::move(statements));
	}

	return nullptr;
}

std::unique_ptr<Expression> Parser::parse_expression_statement() {
	std::unique_ptr<Expression> expr = parse_expression();

	bool semicolon = match_token(TokenKind::SEMICOLON);
	if (expr) {
		if (!semicolon)
			error(m_tokenizer.current().pos(), "Expected ';'");
	} else
		if (semicolon)
			return std::make_unique<Expression>();

	return expr;
}

std::unique_ptr<DeclarationStatement> Parser::parse_declaration_statement() {
	std::unique_ptr<VariableDeclaration> var_decl = parse_variable_declaration();
	if (var_decl) {
		match_token(TokenKind::SEMICOLON);
		return var_decl;
	} else
		return parse_function_declaration();
}

std::unique_ptr<FunctionDeclaration> Parser::parse_function_declaration() {
	if (match_token("def")) {
		std::string id;
		if (is_token(TokenKind::NAME)) {
			id = move(get_token_lexeme());
			next_token();
		} else
			error(m_tokenizer.current().pos(), "Expected ID");

		if (!match_token(TokenKind::LEFT_PAREN))
			error(get_token_position(), "Expected '('");

		std::vector<TypedId> params = parse_parameters();

		if (!match_token(TokenKind::RIGHT_PAREN))
			error(get_token_position(), "Expected ')'");

		Type ret_type = parse_return_type();

		std::unique_ptr<CompoundStatement> statement = parse_compound_statement();
		if (!statement)
			error(get_token_position(), "Expected statements enclosed in '{', '}'");

		return std::make_unique<FunctionDeclaration>(ret_type, std::move(id), std::move(params), std::move(statement));
	}

	return nullptr;
}

TypedId Parser::parse_typed_id() {
	if (is_token(TokenKind::NAME)) {
		TypedId typed_id;
		typed_id.type = parse_type(get_token_lexeme());

		if (typed_id.type == Type::VOID)
			return typed_id;

		next_token();

		if (!is_token(TokenKind::NAME)) {
			error(get_token_position(), "Expected ID");
			return typed_id;
		}

		typed_id.id = std::move(get_token_lexeme());

		next_token();

		return typed_id;
	}

	return { Type::VOID };
}

std::vector<TypedId> Parser::parse_parameters() {
	std::vector<TypedId> params;

	TypedId typed_id = parse_typed_id();
	if (typed_id.type == Type::VOID)
		return params;

	params.push_back(std::move(typed_id));

	while (match_token(TokenKind::COMA)) {
		TypedId typed_id = parse_typed_id();
		if (typed_id.type == Type::VOID)
			break;

		params.push_back(std::move(typed_id));
	}

	return params;
}

Type Parser::parse_return_type() {
	Type type;
	
	if (match_token(TokenKind::ARROW)) {
		if (!is_token(TokenKind::NAME)) {
			error(get_token_position(), "Expected return type, 'void' assumed");
			return Type::VOID;
		}

		std::string_view ret_type = get_token_lexeme();
		if (ret_type == "int")
			type = Type::INT;
		else if (ret_type == "float")
			type = Type::FLOAT;
		else if (ret_type == "bool")
			type = Type::BOOL;
		else {
			error(get_token_position(), "Invlalid return type");
			type = Type::VOID;
		}

		next_token();

		return type;
	} else
		return Type::VOID;
}

std::unique_ptr<VariableDeclaration> Parser::parse_variable_declaration() {
	TypedId typed_id = parse_typed_id();

	if (typed_id.type == Type::VOID)
		return nullptr;

	std::unique_ptr<Expression> init_expr = parse_init_value();
	
	return std::make_unique<VariableDeclaration>(typed_id.type, std::move(typed_id.id), std::move(init_expr));
}

std::unique_ptr<Expression> Parser::parse_init_value() {
	if (match_token(TokenKind::EQUAL)) {
		return parse_conditional_expression();
	} else
		return nullptr;
}

std::unique_ptr<IfStatement> Parser::parse_if_statement() {
	if (match_token("if")) {
		next_token();

		if (!match_token(TokenKind::LEFT_PAREN))
			error(get_token_position(), "Expected '('");

		std::unique_ptr<Expression> cond_expr = parse_conditional_expression();
		if (!cond_expr)
			error(get_token_position(), "Expected conditional expression");

		std::unique_ptr<Statement> stmt = parse_statement();

		return std::make_unique<IfStatement>(std::move(cond_expr), std::move(stmt));
	}

	return nullptr;
}

std::unique_ptr<ForStatement> Parser::parse_for_statement() {
	if (match_token("for")) {
		if (!match_token(TokenKind::LEFT_PAREN))
			error(get_token_position(), "Expected '('");

		std::unique_ptr<VariableDeclaration> var_decl = parse_variable_declaration();

		if (!match_token(TokenKind::SEMICOLON))
			error(get_token_position(), "Expected ';'");

		std::unique_ptr<Expression> cond_expr = parse_conditional_expression();

		if (!match_token(TokenKind::SEMICOLON))
			error(get_token_position(), "Expected ';'");

		std::unique_ptr<Expression> expr = parse_expression();

		if (!match_token(TokenKind::RIGHT_PAREN))
			error(get_token_position(), "Expected ')'");

		std::unique_ptr<Statement> stmt = parse_statement();
		if (!stmt)
			error(get_token_position(), "Expected statement");

		return std::make_unique<ForStatement>(std::move(var_decl), std::move(cond_expr), std::move(expr), std::move(stmt));
	} else
		return nullptr;
}

std::unique_ptr<WhileStatement> Parser::parse_while_statement() {
	if (match_token("while")) {
		if (!match_token(TokenKind::LEFT_PAREN))
			error(get_token_position(), "Expected '('");

		std::unique_ptr<Expression> cond_expr = parse_conditional_expression();
		if (!cond_expr)
			error(get_token_position(), "Expected conditional expression");

		if (!match_token(TokenKind::RIGHT_PAREN))
			error(get_token_position(), "Expected ')'");

		std::unique_ptr<Statement> stmt = parse_statement();
		if (!stmt)
			error(get_token_position(), "Expected statement");

		return std::make_unique<WhileStatement>(std::move(cond_expr), std::move(stmt));
	} else
		return nullptr;
}

std::unique_ptr<ReturnStatement> Parser::parse_return_statement() {
	return nullptr;
}

std::unique_ptr<Expression> Parser::parse_expression() {
	return nullptr;
}

std::unique_ptr<Expression> Parser::parse_conditional_expression() {
	return nullptr;
}

std::unique_ptr<Expression> Parser::parse_logical_or_expression() {
	return nullptr;
}

std::unique_ptr<Expression> Parser::parse_logical_and_expression() {
	return nullptr;
}

std::unique_ptr<Expression> Parser::parse_equality_expression() {
	return nullptr;
}

std::unique_ptr<Expression> Parser::parse_relational_expression() {
	return nullptr;
}

std::unique_ptr<Expression> Parser::parse_additive_expression() {
	return nullptr;
}

std::unique_ptr<Expression> Parser::parse_multiplicative_expression() {
	return nullptr;
}

std::unique_ptr<Expression> Parser::parse_unary_expression() {
	return nullptr;
}

std::unique_ptr<Expression> Parser::parse_primary_expression() {
	return nullptr;
}

std::unique_ptr<Expression> Parser::parse_literal() {
	return nullptr;
}