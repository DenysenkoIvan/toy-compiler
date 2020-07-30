#pragma once

#include <filesystem>

#include "FileStream.h"
#include "Token.h"

class Tokenizer {
public:
	Tokenizer(const std::filesystem::path& input_file);

	Token& next();
	Token& current();
	
private:
	void scan_next_token();

	bool is_eof() const;
	bool is_name_start() const;
	bool is_number_start() const;
	bool is_white_space() const;
	
	Token scan_white_space();
	Token scan_name();
	Token scan_number();
	Token scan_operator_or_punctuation_mark();

private:
	FileStream m_stream;
	Token m_token;
};