#pragma once

#include <filesystem>
#include <fstream>

class FileStream {
public:
	FileStream(const std::filesystem::path& input_file);

	char current() const;
	char next();

	int line() const;
	int column() const;

private:
	std::ifstream m_file;
	int m_line = 1;
	int m_column = 1;
	char m_char = EOF;
};