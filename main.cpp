#include <iostream>
#include <string>
#include <vector>
#include "tests.h"

using namespace std;

int main()
{
	string filename = "table.txt";
	fstream dbfile(filename);
	string str;
	vector<unsigned int> ids;
	getline(dbfile, str);
	while (getline(dbfile, str))
	{
		for (unsigned int i = 0; i<str.length(); ++i)
		{
			if (str[i] == 0x9)
			{
				ids.push_back(stoul(string(str, 0, i)));
				break;
			}
		}
	}
	dbfile.close();
	
	TableRecord Tr(filename, ids);
	Tr.DeleteRecord(122034);

	return 0;
}