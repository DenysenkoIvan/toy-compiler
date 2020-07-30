#pragma once

#include <string>
#include <variant>

enum class TokenKind {
	EOS = 0,
	INVALID,
	WHITE_SPACE,
	COMMENT,

	NAME,

	EQUAL,
	EQUAL_EQUAL,
	NOT_EQUAL,

	LOGICAL_AND,
	LOGICAL_OR,
	LOGICAL_NOT,
	
	LESS,
	GREATER,
	LESS_EQUAL,
	GREATER_EQUAL,
	
	PLUS_PLUS,
	MINUS_MINUS,
	
	PLUS,
	MINUS,
	STAR,
	SLASH,
	COMA,
	
	LEFT_PAREN,
	RIGHT_PAREN,
	SEMICOLON,
	LEFT_BRACKET,
	RIGHT_BRACKET
};

struct Position {
	int line;
	int column;
};

class Token {
public:
	Token(TokenKind kind=TokenKind::INVALID);

	void set_type(TokenKind kind);
	void set_lexeme(std::string lexeme);
	void set_position(Position pos);
	
	TokenKind kind() const;
	Position pos() const;
	std::string& lexeme();
	const std::string& lexeme() const;
	
private:
	std::string m_lexeme;
	Position m_pos = { -1, -1 };
	TokenKind m_kind;
};