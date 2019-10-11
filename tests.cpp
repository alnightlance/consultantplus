#include "tests.h"

unsigned int TableRecord::GetIndex(unsigned int id)
{
	unsigned int index;
	auto p = std::find(_ids.begin(), _ids.end(), id);
	if (p != _ids.end())
		index = p - _ids.begin();
	else
	{
		std::cout << "No such record with ID " << id << std::endl;
		return 0xffffffff;	//	error code (-1)
	}
	return index;
}

TableRecord TableRecord::GetRecord(unsigned int id)
{
	unsigned int index = GetIndex(id);
	if (index == 0xffffffff)
	{
		return *this;
	}
	std::ifstream dbfile(_filename);
	std::string str;
	for(unsigned int i = 0; i<index+1; ++i)
		getline(dbfile, str);
	getline(dbfile, str);
	size_t from = 0, len = 0;
	std::vector<std::string> fields;
	for (size_t i = 0; i < str.length(); ++i)
	{
		if (str[i] == 0x9)
		{
			len = i - from;
			fields.push_back(std::string(str, from, len));
			from = i + 1;
		}
	}
	_id = fields.at(0);
	_source = fields.at(1);
	_name = fields.at(2);
	_docSize = stoul(fields.at(3));
	_firstEdit = fields.at(4);
	_remark = fields.at(5);

	dbfile.close();

	return *this;
}

void TableRecord::InsertRecord(unsigned int id, std::string source, std::string name,
	size_t docSize, std::string firstEdit, std::string remark)
{
	std::ofstream dbfile(_filename, std::ios::app);
	dbfile << id << char(0x9) << source << char(0x9) << name << char(0x9) <<
		docSize << char(0x9) << firstEdit << char(0x9) << remark << char(0x9) << std::endl;
	
	dbfile.close();
}

void TableRecord::DeleteRecord(unsigned int id)
{
	EditRecord(id, "", "", 0, "", "", true);
}

void TableRecord::EditRecord(unsigned int id, std::string source, std::string name,
	size_t docSize, std::string firstEdit, std::string remark, bool deleteString)
{
	unsigned int index = GetIndex(id);
	if (index == 0xffffffff)
	{
		return;
	}
	std::fstream dbfile(_filename, std::ios::in);
	std::fstream dbTempFile(_filename + ".temp", std::ios::out);
	std::string str;
	for (unsigned int i = 0; i < index + 1; ++i)
	{
		getline(dbfile, str);
		dbTempFile << str + "\n";
	}
	getline(dbfile, str);
	if (!deleteString)
	{
		dbTempFile << id << char(0x9) << source << char(0x9) << name << char(0x9) <<
			docSize << char(0x9) << firstEdit << char(0x9) << remark << char(0x9) << std::endl;
	}
	while (getline(dbfile, str))
		dbTempFile << str + "\n";
	dbfile.close();
	dbTempFile.close();

	dbfile.open(_filename, std::ios::out);
	dbTempFile.open(_filename + ".temp", std::ios::in);
	while (getline(dbTempFile, str))
		dbfile << str + "\n";
	dbfile.close();
	dbTempFile.close();

	std::remove(std::string(_filename + ".temp").c_str());
}
