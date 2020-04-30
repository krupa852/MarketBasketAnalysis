#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iterator>
#include <fstream>
#include <unordered_map>
#include <set>
#include <vector>
#include <string>
#include <sstream>
#include <ctime>
#include<iostream>
#include <map>

using namespace std;
fstream in;
struct FrequentItemsets
{
    	vector<vector<string>> itemsets;
    	vector<int> supports;
};

struct AssociationRule
{
    	vector<string> lhs;
    	vector<string> rhs;
};

struct Association
{
    	vector<AssociationRule> rules;
    	vector<int> supports;
    	vector<double> confidences;
};

void LoadFile(string filename)
{
  	in.open(filename, fstream::in);

    	if (!in) {
       		cout << "Fail to load file." << endl;
        	exit(1);
    	}
}

/* Read one transaction at a time */
bool  read_transactions(string filename,vector<string>&transaction)
{
     	string  line;
     	transaction.clear();
     	size_t start, end;
     	while (true)
	{
        	if (!getline(in, line))
            		return false;
		start = line.find_first_of('[');
        	end = line.find_first_of(']');
        	if (start == line.npos || end == line.npos)
            		continue;
        	break;
    	}
    	istringstream iss(line.substr(start + 1, end - start - 1));
    	string it;
	while (getline(iss, it, ','))
	      	transaction.push_back(it);
        return true;
}
void Reset()
{
	in.clear();
	in.seekg(0);
}

/*Function to generate frequent one itemset */
FrequentItemsets Frequent_one_itemsets(string filename,vector<string> transaction ,int minSup)
{
    	unordered_map<string, int> map;
  	Reset();
    	while(read_transactions(filename,transaction))
	{
		for (auto &item : transaction)
        	{
			map[item]++;
		}
	}

    	FrequentItemsets result;
    	for (auto &pair : map)
        	if (pair.second >= minSup)
		{
            		result.itemsets.push_back(vector<string>(1, pair.first));
            		result.supports.push_back(pair.second);
        	}
    	return result;
}

/* Function to return merged result of frequent k-itemsets */
FrequentItemsets Merge(FrequentItemsets &It, const FrequentItemsets &result)
{
    	for (auto &itemset : result.itemsets)
        	It.itemsets.push_back(itemset);

    	for (auto support : result.supports)
        	It.supports.push_back(support);

	return It;
}

/* helper function that checks conditions mentioned & returns true for  merging a pair of frequent (k −1)-itemsets
 only if their first k −2 items, arranged in lexicographic order, are identical*/
bool IsJoinItemsets(const vector<string> &itemset1,const vector<string> &itemset2)
{
	size_t j = itemset1.size();
    	for (size_t i = 0; i < j - 1; ++i)
        	if (itemset1[i] != itemset2[i])
			       return false;
    	return itemset1[j - 1] < itemset2[j - 1];

}

/*helper function to join itemsets obtained from IsJoinItemset function */
vector<string> JoinItemsets(const vector<string> &itemset1, const vector<string> &itemset2)
{
    	vector<string> NewItemset = itemset1;
    	NewItemset.push_back(itemset2.back());
    	return NewItemset;
}

/*Candidate pruning before candidate-generation of K-itemsets by checking for infrequent subsets of K-itemset*/
bool hasInfrequentSubset(const vector<string> &candidate,const vector<vector<string>> &freqItemsets)
{
    	if (candidate.size() == 2)
        	return false;
    	for (size_t i = 0; i < candidate.size();i++)
	{
        	vector<string> candidateSubset;
        	for (size_t j = 0; j < candidate.size(); j++)
           		if (j != i)
                		candidateSubset.push_back(candidate[j]);
        		auto pred =[&](const vector<string> &itemset) {
			return candidateSubset == itemset;};

        		if (find_if(freqItemsets.begin(), freqItemsets.end(), pred)== freqItemsets.end())
            		return true;
    	}
    	return false;
}

/* Candidate generate function using F(k-1) * F(k-1) method */
vector<vector<string>> genCandidates(const vector<vector<string>> &freqItemsets, int minSup)
{

    	vector<vector<string>> candidates;
    	for (auto &itemset1 : freqItemsets)
        	for (auto &itemset2 : freqItemsets)
            		if (IsJoinItemsets(itemset1, itemset2))
			{
                    		auto candidate = JoinItemsets(itemset1, itemset2);
				if (!hasInfrequentSubset(candidate, freqItemsets))
                    			candidates.push_back(candidate);

			}
    	return candidates;

}

/*Candidate pruning based on min_support after Candidate generation of K-itemset*/
FrequentItemsets pruneCandidates(const vector<vector<string>> &candidates,string filename,vector<string> transaction, int minSup)
{
    	FrequentItemsets result;
    	vector<int> ctr(candidates.size(), 0);
    	Reset();
    	while (read_transactions(filename,transaction))
	{
        	sort(transaction.begin(), transaction.end());
        	for (size_t i = 0; i < candidates.size(); ++i)
		{
            		const vector<string> &candidate = candidates[i];
            		if (includes(transaction.begin(), transaction.end(),candidate.begin(), candidate.end()))
                		ctr[i]++;
        	}

    	}
    	for (size_t i = 0; i < candidates.size(); ++i)
        	if (ctr[i] >= minSup)
		{
            		result.itemsets.push_back(candidates[i]);
            		result.supports.push_back(ctr[i]);
        	}

    	return result;
}

/* Apriori Algorithm to find frequent itemsets, given minimum support */
FrequentItemsets AprioriAlgo(string filename,vector<string> transaction, int minSup)
{
	int i=0;
    	FrequentItemsets result;
	auto FrequentItemsets = Frequent_one_itemsets(filename,transaction, minSup);
    	while (!FrequentItemsets.itemsets.empty())
	{

		result =  Merge(result, FrequentItemsets);
        	auto candidates = genCandidates(FrequentItemsets.itemsets, minSup);
        	FrequentItemsets = pruneCandidates(candidates,filename,transaction, minSup);

    	}

    	return result;
}

/* Analysing each itemset from frequent itemsets , construct association rules by determining left hand side & right hand side items*/
void ConstructRule(vector<string> &lhs, const vector<string> &itemset,int itemsetSupport, int depth,const map<vector<string>, int> &SupMap, double minConf,Association &result)
{
    	if (depth == itemset.size())
	{
		if (lhs.empty()||lhs.size() == itemset.size())
            	return;
        	double conf = (double)itemsetSupport / SupMap.at(lhs);
        	if (conf >= minConf)
		{
            		AssociationRule rule;
            		rule.lhs = lhs;
            		set_difference(itemset.begin(), itemset.end(),lhs.begin(), lhs.end(),back_inserter(rule.rhs));

            		result.rules.push_back(rule);
            		result.supports.push_back(itemsetSupport);
            		result.confidences.push_back(conf);
        	}
        return;
    	}

    	ConstructRule(lhs, itemset, itemsetSupport, depth + 1,SupMap, minConf, result);
    	lhs.push_back(itemset[depth]);
    	ConstructRule(lhs, itemset, itemsetSupport, depth + 1,SupMap, minConf, result);
    	lhs.pop_back();

}

void ConstructRule(const vector<string> &itemset, const map<vector<string>, int> &SupMap,double minConf, Association &result)
{
    	if (itemset.size() <= 1)
        	return;
    	vector<string> lhs;
    	ConstructRule(lhs, itemset, SupMap.at(itemset), 0,SupMap, minConf, result);
}

/* Function to generate association rule from frequent itemset based on confidence */
Association genAssociationRules(FrequentItemsets &result,double minConfidence)
{
	map<vector<string>, int> SupMap;
    	for (size_t i = 0; i < result.itemsets.size(); ++i)
        SupMap.insert(make_pair(result.itemsets[i], result.supports[i]));

    	Association assocResult;
    	for (auto &itemset : result.itemsets)
        ConstructRule(itemset, SupMap, minConfidence, assocResult);

    	return assocResult;
}

ostream &operator<<(ostream &os, const vector<string> &itemset)
{
    	for (size_t i = 0; i < itemset.size(); ++i)
	{
        	os << itemset[i];
        	if (i + 1 != itemset.size())
            		os << ", ";
    	}
    	return os;
}

ostream &operator<<(ostream &os, const vector<vector<string>> &itemsets)
{
    	for (auto &itemset : itemsets)
        	os << itemset << endl;
    	return os;
}

ostream &operator<<(ostream &os, const FrequentItemsets &result)
{
        size_t size = result.itemsets.size();
        os << "The number of Frequent itemsets: " << size << endl;
        for (size_t i = 0; i < size; ++i)
        os << result.itemsets[i] << ": " << result.supports[i] << endl;
        return os;
}

ostream &operator<<(ostream &os, const AssociationRule &rule)
{
    	os << rule.lhs << " -> " << rule.rhs;
    	return os;
}

ostream &operator<<(ostream &os, const Association &result)
{
    	size_t size = result.rules.size();
    	os << "The number of association rules: " << size << endl;
    	for (size_t i = 0; i < size; ++i) {
        os << result.rules[i] << " [support = " << result.supports[i]
            << ", confidence = "
            << 100 * result.confidences[i] << "%]" << endl;
    	}
    	return os;
}

int main(int argc, char* argv[])
{
	string db_filename;
	vector<string> transaction;
	if(argc!=4)
	{
		printf("Error");
		return 0;
	}
  	db_filename = string(argv[1]);
	float minSup = (int) ceil(atof(argv[2]));
	double minConf = (double) atof(argv[3]);
	LoadFile(db_filename);
	FrequentItemsets aprioriOutput = AprioriAlgo(db_filename,transaction, minSup );
	cout<<aprioriOutput<<endl;
	Association AssociationOutput = genAssociationRules(aprioriOutput, minConf);
  	cout << AssociationOutput << endl;

}
