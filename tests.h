#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

class TableRecord
{
public:
	TableRecord(const std::string& filename, std::vector<unsigned int> &ids) :
		_filename(filename), _ids(ids) {}

	TableRecord GetRecord(unsigned int);
	void InsertRecord(unsigned int, std::string, std::string, size_t, std::string, std::string);
	void DeleteRecord(unsigned int);
	void EditRecord(unsigned int, std::string, std::string, size_t, std::string, std::string, bool deleteString = false);

private:
	std::vector<unsigned int> _ids;
	std::string _filename;

	std::string _id;
	std::string _source;
	std::string _name;
	size_t _docSize;
	std::string _firstEdit;
	std::string _remark;

	unsigned int GetIndex(unsigned int);
};

	
