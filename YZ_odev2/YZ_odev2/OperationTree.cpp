#include "stdafx.h"
#include "OperationTree.h"
#include <time.h>
#include <iostream>

OperationTree::OperationTree() :operations{ '/', '*', '-', '+' }, root(std::make_shared<Node>())  //std::shared_ptr<Node>(new Node))
{
	//root->parent = root;
	treeNodes.push_back(root);
}

std::shared_ptr<Node > OperationTree::getRoot()
{
	return root;
}

void OperationTree::addNode(std::shared_ptr<Node > node)
{
	treeNodes.push_back(node);
}

/*
It adds the number values to the initialized number nodes.
To make it more random, it picks a random index from 0 to amountOfNumbers, 
and starts adding numbers from there up. If it reaches the array limit before all nodes are intialized,
the index is set to 0.
*/
void OperationTree::addNumbers(int* numbers,int amountOfNumbers)
{
	int numberIndex = rand() % amountOfNumbers;
	for (int i = 0;i < treeNumbers.size();i++) {

		treeNumbers[i]->intValue = *(numbers + numberIndex);
		numberIndex++;
		if (numberIndex == amountOfNumbers)			
			numberIndex = 0;	
	}
}

//Adds operation nodes' by picking them randomly each time from the array "operations".
void OperationTree::addRandomOperations()
{	
	int randVal;
	for (int i = 0;i < treeNodes.size();i++) {
		randVal = rand() % 4;
		treeNodes.at(i)->charValue = operations[randVal];
	}
}

//Initializes the number nodes -leaf nodes- by acessing them through operation nodes.
void OperationTree::initializeNumberNodes()
{
	for (int i = 0;i < treeNodes.size();i++) {
		if (treeNodes[i]->left == nullptr) {
			treeNodes[i]->left = std::shared_ptr<Node>(new Node);
			treeNumbers.push_back(treeNodes[i]->left);
		}
		if (treeNodes[i]->right == nullptr) {
			treeNodes[i]->right = std::shared_ptr<Node>(new Node);
			treeNumbers.push_back(treeNodes[i]->right);
		}
	}
}

float OperationTree::getFitness()
{
	return fitness;
}

void OperationTree::setFitness(float i)
{
	fitness = i;
}

void OperationTree::setOperation(int index,char c)
{
	treeNodes.at(index)->charValue = c;
}

int OperationTree::getNumOfOperations()
{
	return treeNodes.size();
}

// Assigns one of the operations of the second parent, to one of the childs.
void OperationTree::getOperationFromSecondParent(Node* childNode, Node* parentNode)
{
	childNode->charValue = parentNode->charValue;
}

void OperationTree::setNumberTree(std::vector<std::shared_ptr<Node >> parentNumbers)
{
	for (int i = 0;i < treeNumbers.size(); i++) {
		treeNumbers[i]->intValue = parentNumbers[i]->intValue;
	}
}

void OperationTree::swapRandomNumbers()
{
	int index1 = rand() % treeNumbers.size();
	int index2;
	int temp;

	do {
		index2 = rand() % treeNumbers.size();
	} while (index1 == index2);

	temp = treeNumbers.at(index1)->intValue;
	treeNumbers.at(index1)->intValue = treeNumbers.at(index2)->intValue;
	treeNumbers.at(index2)->intValue = temp;
}

std::vector<std::shared_ptr<Node >> OperationTree::getNumberNodes()
{
	return treeNumbers;
}

Node* OperationTree::getOperationNodePtr(int index)
{
	return treeNodes[index].get();
}

std::string OperationTree::printTree(std::shared_ptr<Node> node)
{	
	std::string operationExpression ;

	if (node->intValue == 0) {
		return ("(" + printTree(node->left)+ node->charValue + printTree(node->right) + ")");
	}
	else {
		return(std::to_string(node->intValue));
	}
}





