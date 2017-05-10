#pragma once
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <vector>
#include <iostream>

using namespace std;
using namespace boost::property_tree;

class DecisionTree {
public:
	DecisionTree(vector<string> a, vector<vector<string>> av, vector<vector<string>> data) : attr(a), attrValues(av), data(data) {};
	~DecisionTree();
	void setDec(string name);
	ptree* buildDecisionTree(void);
	void printDecisionTree(void);
	void write_xml(const string& filename);

private:
	void setDec(size_t index);
	/*
	root:			子树的根节点
	qualifiedData:	属于该子树的数据
	uncertainAttr:	未确定位置的属性
	*/
	void buildDecisionTree(ptree* root, const vector<int>& qualifiedData, const vector<int>& uncertainAttr);
	//检查所有数据是否都有同样的决策属性
	bool infoPure(const vector<int>& qualifiedData);
	void printPTree(const basic_ptree<string, string>* root, int);
	double getEntropy(const vector<int>& qualifiedData);

private:
	vector<string> attr; //属性
	vector<vector<string>> attrValues; //每个属性的可能取值
	vector<vector<string>> data; //数据
	int decIndex; //目标属性的索引号

	ptree* root;
};
