#pragma once
#include <vector>
#include <memory>
#include <string>

struct Node{
	char charValue;
	int intValue;
	std::shared_ptr<Node > left;
	std::shared_ptr<Node > right;	

	Node() : intValue(0)
	{
		charValue = 0;
	}
};

class OperationTree
{
private:
	std::vector<std::shared_ptr<Node >> treeNodes;// Stores a tree's operation nodes.
	std::vector<std::shared_ptr<Node >> treeNumbers;// Stores a tree's number nodes.

	float fitness;
	std::shared_ptr<Node > root;
	char operations[4];
	int operationResult;
	
public:
	OperationTree();
	std::shared_ptr<Node > getRoot();
	void addNode(std::shared_ptr<Node > node);
	void addNumbers(int* numbers,int amountOfNumbers);
	void addRandomOperations();
	void initializeNumberNodes();

	float getFitness();
	void setFitness(float i);
	void setOperation(int index,char c);
	void setNumberTree(std::vector<std::shared_ptr<Node >> parentNumbers);
	void swapRandomNumbers();
	int getNumOfOperations();
	void getOperationFromSecondParent(Node* childNode, Node* parentNode);
	std::vector<std::shared_ptr<Node >> getNumberNodes();
	Node* getOperationNodePtr(int index);
	std::string printTree(std::shared_ptr<Node> root);
};