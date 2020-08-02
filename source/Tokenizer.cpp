#include "Tokenizer.h"

Tokenizer::Tokenizer(const std::filesystem::path& input_file)
	: m_stream(input_file)
{
	scan_next_token();
}

Token& Tokenizer::current() {
	return m_token;
}

Token& Tokenizer::next() {
	scan_next_token();
	return m_token;
}

void Tokenizer::scan_next_token() {
	Position pos = { m_stream.line(), m_stream.column() };
	
	do {
		if (is_eof())
			m_token.set_type(TokenKind::EOS);
		else if (is_white_space())
			m_token = scan_white_space();
		else if (is_name_start())
			m_token = scan_name();
		else if (is_number_start())
			m_token = scan_number();
		else
			m_token = scan_operator_or_punctuation_mark();
	} while (m_token.kind() == TokenKind::WHITE_SPACE ||
			 m_token.kind() == TokenKind::COMMENT ||
			 m_token.kind() == TokenKind::INVALID);

	if (m_token.kind() == TokenKind::INVALID) {
		Position new_pos = { m_stream.line(), m_stream.column() };

		// TODO Error messages
		// 
		// error(pos, new_pos, "Invlaid characters");
		// For now it just skips all the invlaid characters
	}

	m_token.set_position(pos);
}

bool Tokenizer::is_eof() const {
	return m_stream.current() == EOF;
}

bool Tokenizer::is_name_start() const {
	return std::isalpha(m_stream.current());
}

bool Tokenizer::is_number_start() const {
	return std::isdigit(m_stream.current());
}

bool Tokenizer::is_white_space() const {
	return std::isspace(m_stream.current());
}
	
Token Tokenizer::scan_white_space() {
	while (is_white_space())
		m_stream.next();

	return Token(TokenKind::WHITE_SPACE);
}

Token Tokenizer::scan_name() {
	Token token(TokenKind::NAME);
	std::string name;

	while (std::isalnum(m_stream.current()) || m_stream.current() == '_') {
		name += m_stream.current();
		m_stream.next();
	}

	token.set_lexeme(std::move(name));

	return token;
}

Token Tokenizer::scan_number() {
	Token token;
	std::string lexeme;

	while (std::isdigit(m_stream.current())) {
		lexeme += m_stream.current();
		m_stream.next();
		token.set_type(TokenKind::INT);
	}
	
	if (m_stream.current() == '.') {
		if (!std::isdigit(m_stream.next())) {
			token.set_type(TokenKind::INVALID);
			return token;
		}

		while (std::isdigit(m_stream.current())) {
			lexeme += m_stream.current();
			m_stream.next();
		}

		token.set_type(TokenKind::FLOAT);
	}

	token.set_lexeme(std::move(lexeme));

	return token;
}

Token Tokenizer::scan_operator_or_punctuation_mark() {
	Token token;

	switch (m_stream.current()) {
	case '/':
		if (m_stream.next() == '/') {
			char c = m_stream.next();
			while (c != '\n' && c != EOF) {
				c = m_stream.next();
			}
			token.set_type(TokenKind::COMMENT);
			m_stream.next();
		} else
			token.set_type(TokenKind::SLASH);
		break;
	case '=':
		if (m_stream.next() == '=') {
			m_stream.next();
			token.set_type(TokenKind::EQUAL_EQUAL);
		} else
			token.set_type(TokenKind::EQUAL);
		break;
	case '!':
		if (m_stream.next() == '=') {
			m_stream.next();
			token.set_type(TokenKind::NOT_EQUAL);
		} else
			token.set_type(TokenKind::LOGICAL_NOT);
		break;
	case '+':
		if (m_stream.next() == '+') {
			m_stream.next();
			token.set_type(TokenKind::PLUS_PLUS);
		} else
			token.set_type(TokenKind::PLUS);
		break;
	case '-':
		m_stream.next();
		if (m_stream.current() == '-') {
			m_stream.next();
			token.set_type(TokenKind::MINUS_MINUS);
		} else if (m_stream.current() == '>') {
			m_stream.next();
			token.set_type(TokenKind::ARROW);
		} else
			token.set_type(TokenKind::MINUS);
		break;
	case '*':
		token.set_type(TokenKind::STAR);
		m_stream.next();
		break;
	case '<':
		if (m_stream.next() == '=') {
			m_stream.next();
			token.set_type(TokenKind::LESS_EQUAL);
		} else
			token.set_type(TokenKind::LESS);
		break;
	case '>':
		if (m_stream.next() == '=') {
			m_stream.next();
			token.set_type(TokenKind::GREATER_EQUAL);
		} else
			token.set_type(TokenKind::GREATER);
		break;
	case '(':
		m_stream.next();
		token.set_type(TokenKind::LEFT_PAREN);
		break;
	case ')':
		m_stream.next();
		token.set_type(TokenKind::RIGHT_PAREN);
		break;
	case '&':
		if (m_stream.next() == '&') {
			m_stream.next();
			token.set_type(TokenKind::LOGICAL_AND);
		} else
			token.set_type(TokenKind::INVALID);
	case '|':
		if (m_stream.next() == '|') {
			m_stream.next();
			token.set_type(TokenKind::LOGICAL_OR);
		} else
			token.set_type(TokenKind::INVALID);
	case ';':
		m_stream.next();
		token.set_type(TokenKind::SEMICOLON);
		break;
	case '{':
		m_stream.next();
		token.set_type(TokenKind::LEFT_BRACE);
		break;
	case '}':
		m_stream.next();
		token.set_type(TokenKind::RIGHT_BRACE);
		break;
	default:
		token.set_type(TokenKind::INVALID);
	}

	return token;
}