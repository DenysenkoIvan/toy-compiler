#include "FileStream.h"

FileStream::FileStream(const std::filesystem::path& input_file)
	: m_file(input_file)
{
	m_char = m_file.get();
}

char FileStream::current() const {
	return m_char;
}

char FileStream::next() {
	m_char = m_file.get();

	if (m_char != EOF) {
		if (m_char == '\n') {
			m_line++;
			m_column = 1;
		} else
			m_column++;
	}

	return m_char;
}

int FileStream::line() const {
	return m_line;
}

int FileStream::column() const {
	return m_column;
}