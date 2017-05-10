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
	root:			�����ĸ��ڵ�
	qualifiedData:	���ڸ�����������
	uncertainAttr:	δȷ��λ�õ�����
	*/
	void buildDecisionTree(ptree* root, const vector<int>& qualifiedData, const vector<int>& uncertainAttr);
	//������������Ƿ���ͬ���ľ�������
	bool infoPure(const vector<int>& qualifiedData);
	void printPTree(const basic_ptree<string, string>* root, int);
	double getEntropy(const vector<int>& qualifiedData);

private:
	vector<string> attr; //����
	vector<vector<string>> attrValues; //ÿ�����ԵĿ���ȡֵ
	vector<vector<string>> data; //����
	int decIndex; //Ŀ�����Ե�������

	ptree* root;
};
