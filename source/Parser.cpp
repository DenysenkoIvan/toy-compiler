#include "Parser.h"

#include <iostream>

static void error(Position pos, std::string_view message) {
	std::cout << '[' << pos.line << ',' << pos.column << "] Error: " << message << '\n';
}

Parser::Parser(const std::filesystem::path& input_file)
	: m_tokenizer(input_file) {}

/*void Parser::expect_token(TokenKind kind) {
	if (m_tokenizer.current().kind() == kind) {
		m_tokenizer.next();
	} else
		error(m_tokenizer.current().pos(), "Expected ");
}*/

bool Parser::match_token(TokenKind kind) {
	if (m_tokenizer.current().kind() == kind) {
		m_tokenizer.next();
		return true;
	} else
		return false;
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
	if (match_token(TokenKind::LEFT_BRACKET)) {
		std::vector<std::unique_ptr<Statement>> statements;

		while (true) {
			if (match_token(TokenKind::RIGHT_BRACKET))
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
	return nullptr;
}

std::unique_ptr<FunctionDeclaration> Parser::parse_function_declaration() {
	return nullptr;
}

std::unique_ptr<VariableDeclaration> Parser::parse_variable_declaration() {
	return nullptr;
}

std::unique_ptr<Statement> Parser::parse_parameters() {
	return nullptr;
}

std::unique_ptr<IfStatement> Parser::parse_if_statement() {
	return nullptr;
}

std::unique_ptr<ForStatement> Parser::parse_for_statement() {
	return nullptr;
}

std::unique_ptr<WhileStatement> Parser::parse_while_statement() {
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