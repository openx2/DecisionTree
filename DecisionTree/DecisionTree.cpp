// DecisionTree.cpp : 决策树算法
//

#include "DTClass.h"

#include <vector>

#include <tuple>
#include <regex>
#include <string>
#include <cstring>

#include <fstream>

#include <boost/algorithm/string.hpp>

using namespace std;

vector<string> split(const string& str, const char* delimiters)
{
	vector<string> strs;
	char* ch = new char[str.size() + 1];
	strcpy_s(ch, str.size() + 1, str.c_str());
	char *p, *buf;
	p = strtok_s(ch, delimiters, &buf);
	while (p != NULL) {
		string s = p;
		boost::algorithm::trim(s);
		strs.push_back(s);
		p = strtok_s(NULL, delimiters, &buf);
	}
	return strs;
}

auto readARFF(const char* filename)
{
	vector<string> attr;
	vector<vector<string>> attrValues;
	vector<vector<string>> data;
	ifstream in(filename);
	if (!in.is_open()) {
		cout << "Fail to open file!" << endl;
		exit(1);
	}

	regex reg("@attribute(.*)\\{(.*?)\\}.*");
	smatch what;
	for (string line; getline(in, line); ) {
		if (regex_match(line, what, reg)) {
			attr.push_back(boost::algorithm::trim_copy(what[1].str()));
			attrValues.push_back(split(what[2].str(), " ,"));
		}
		else if (strncmp(line.c_str(), "@data", 5) == 0) {
			while (getline(in, line)) {
				if (line == "")
					continue;
				data.push_back(split(line, " ,"));
			}
		}
	}
	in.close();

	return make_tuple(attr, attrValues, data);
}

int main()
{
	vector<string> attr;
	vector<vector<string>> attrValues, data;
	//读取ARFF数据
	//tie(attr, attrValues, data) = readARFF("weather.nominal.arff");
	tie(attr, attrValues, data) = readARFF("buys_computer.arff");
	DecisionTree dt(attr, attrValues, data);
	//设置决策属性
	//dt.setDec("play");
	dt.setDec("buys_computer");
	//构建决策树
	boost::property_tree::ptree* root = dt.buildDecisionTree();
	//dt.write_xml("weather_decisiontree.xml");
	//dt.write_xml("buys_computer_decisiontree.xml");
	//cout << "生成xml文件成功！" << endl;
	dt.printDecisionTree();
	return 0;
}