#include "Token.h"

Token::Token(TokenKind kind)
	: m_kind(kind) {}

void Token::set_type(TokenKind kind) {
	m_kind = kind;
}

void Token::set_lexeme(std::string lexeme) {
	m_lexeme = std::move(lexeme);
}

void Token::set_position(Position pos) {
	m_pos = pos;
}

TokenKind Token::kind() const {
	return m_kind;
}

Position Token::pos() const {
	return m_pos;
}

std::string& Token::lexeme() {
	return m_lexeme;
}

const std::string& Token::lexeme() const {
	return m_lexeme;
}