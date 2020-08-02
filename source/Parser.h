#pragma once

#include "AST.h"
#include "Tokenizer.h"

#include <array>

class Parser {
public:
	Parser(const std::filesystem::path& input_file);

	std::unique_ptr<TranslationUnit> parse();

private:

	bool match_token(TokenKind kind);
	bool match_token(std::string_view str);
	bool is_token(TokenKind kind);

	void next_token();

	std::string& get_token_lexeme();
	Position get_token_position();

	std::unique_ptr<TranslationUnit> parse_translation_unit();
	
	std::unique_ptr<Statement> parse_statement();
	std::unique_ptr<CompoundStatement> parse_compound_statement();
	std::unique_ptr<Expression> parse_expression_statement();
	std::unique_ptr<DeclarationStatement> parse_declaration_statement();
	std::unique_ptr<FunctionDeclaration> parse_function_declaration();
	TypedId parse_typed_id();
	std::vector<TypedId> parse_parameters();
	Type parse_return_type();
	std::unique_ptr<VariableDeclaration> parse_variable_declaration();
	std::unique_ptr<Expression> parse_init_value();
	std::unique_ptr<IfStatement> parse_if_statement();
	std::unique_ptr<ForStatement> parse_for_statement();
	std::unique_ptr<WhileStatement> parse_while_statement();
	std::unique_ptr<ReturnStatement> parse_return_statement();

	std::unique_ptr<Expression> parse_expression();
	std::unique_ptr<Expression> parse_conditional_expression();
	std::unique_ptr<Expression> parse_logical_or_expression();
	std::unique_ptr<Expression> parse_logical_and_expression();
	std::unique_ptr<Expression> parse_equality_expression();
	std::unique_ptr<Expression> parse_relational_expression();
	std::unique_ptr<Expression> parse_additive_expression();
	std::unique_ptr<Expression> parse_multiplicative_expression();
	std::unique_ptr<Expression> parse_unary_expression();
	std::unique_ptr<Expression> parse_primary_expression();
	std::unique_ptr<Expression> parse_literal();

private:
	Tokenizer m_tokenizer;
};