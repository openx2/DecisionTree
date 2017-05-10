#include <unordered_map>
#include <float.h>
#include <cmath>

#include "DTClass.h"

using namespace boost::property_tree;

DecisionTree::~DecisionTree()
{
	delete root;
}

void DecisionTree::setDec(string name)
{
	for (size_t i = 0; i < attr.size(); i++) {
		if (name == attr[i]) {
			setDec(i);
			break;
		}
	}
}

void DecisionTree::setDec(size_t index)
{
	if (index < 0 || index > attr.size()) {
		perror("决策变量指定错误。");
		exit(2);
	}
	decIndex = index;
}

void DecisionTree::write_xml(const string & filename)
{
	boost::property_tree::write_xml(filename, *root, locale(),
		xml_writer_make_settings<string>(' ', 4));
}

void DecisionTree::printDecisionTree()
{
	printPTree(root, 0);
}

ptree* DecisionTree::buildDecisionTree()
{
	root = new ptree;
	vector<int> rows, uncertainAttr;
	for (size_t i = 0; i < data.size(); i++)
		rows.push_back(i);
	for (size_t i = 0; i < attr.size(); i++)
		if (i != decIndex)
			uncertainAttr.push_back(i);
	ptree pt;
	buildDecisionTree(&pt, rows, uncertainAttr);
	root->add_child("DecisionTree", pt);
	return root;
}

void DecisionTree::buildDecisionTree(ptree * root, const vector<int>& qualifiedData, const vector<int>& uncertainAttr)
{
	if (infoPure(qualifiedData)) {
		root->put_value(data[qualifiedData[0]][decIndex]);
		return;
	}
	int minIndex = -1;
	double minEntropy = DBL_MAX;
	unordered_map<string, vector<int>> selectData;
	for each(int i in uncertainAttr) {
		unordered_map<string, vector<int>> attrClassifiedData;
		for each(int j in qualifiedData)
			attrClassifiedData[data[j][i]].push_back(j);
		double entropy = 0.0;
		for (auto it = attrClassifiedData.begin(); it != attrClassifiedData.end(); it++) //计算条件熵
			entropy += getEntropy(it->second) * it->second.size() / qualifiedData.size();
		if (entropy < minEntropy) {
			minIndex = i;
			minEntropy = entropy;
			selectData = attrClassifiedData;
		}
	}
	vector<int> nowUncertainAttr;
	for each(int i in uncertainAttr)
		if (i != minIndex)
			nowUncertainAttr.push_back(i);
	for (auto it = selectData.begin(); it != selectData.end(); it++) {
		ptree pt;
		pt.add("<xmlattr>.value", it->first);
		buildDecisionTree(&pt, it->second, nowUncertainAttr);
		root->add_child(attr[minIndex], pt);
	}
}

bool DecisionTree::infoPure(const vector<int>& qualifiedData)
{
	string& value = data[qualifiedData[0]][decIndex];
	for each(int i in qualifiedData) {
		if (value != data[i][decIndex])
			return false;
	}
	return true;
}

double DecisionTree::getEntropy(const vector<int>& qualifiedData)
{
	if (qualifiedData.size() == 0)
		return 0.0;
	unordered_map<string, int> counter;
	for each(int i in qualifiedData) {
		counter[data[i][decIndex]]++;
	}
	double entropy = 0.0;
	for (auto it = counter.begin(); it != counter.end(); it++)
		entropy -= it->second * log2(DBL_MIN + it->second);
	double sum = qualifiedData.size();
	entropy += sum * log2(sum);
	entropy /= sum;
	return entropy;
}

void DecisionTree::printPTree(const basic_ptree<string, string>* root, int depth)
{
	if (root->data() != "") {
		for (int i = 0; i < depth; i++)
			cout << "    ";
		cout << root->data() << endl;
		return;
	}
	for each(auto& sub in root->get_child("")) {
		if (sub.first == "<xmlattr>")
			continue;
		for (int i = 0; i < depth; i++)
			cout << "    ";
		cout << sub.first;
		if (auto& c = sub.second.get_child_optional("<xmlattr>.value")) {
			cout << "  " << c.get().data();
		}
		cout << endl;
		printPTree(&sub.second, depth+1);
	};
}