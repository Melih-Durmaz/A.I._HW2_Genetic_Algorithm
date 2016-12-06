#include "stdafx.h"
#include "GeneticAlgorithm.h"
#include "OperationTree.h"
#include <time.h>
#include <iostream>
#include <math.h>

GeneticAlgorithm::GeneticAlgorithm():
	numOfGenerations(0),
	thePerfectMember()
{
	srand(time(NULL));
	numOfDeadAndNewBorn = (int)(POPULATION_SIZE*0.1);// %30 of the population.
}

/*
This function creates random binary tree structures(population members) to store the operation info.
It creates a shared pointer of type OperationTree and adds is to the population with push_back().
Then it's information is passed to local "currentTree" and "root" variables to create the rest of the tree.

Root's child nodes (left and right) are pushed into treeQueue vector by their adresses,
then a random node is picked from treeQueue and added to treeNodes, then the child nodes of the picked node
is added to treeQueue. This process is repeated until all the operation nodes are randomly picked from treeQueue.

treeQueue is storing pointers to shared pointers because push_back() function creates a copy of the parameter which it passes and 
adds it to the vector. So, initializing an object in the vector would only affect the copy. Therefore we copy the pointer 
and use that to access and initialize the original shared pointer to point to a new Node.

Shared pointers are used in all the vectors in this program to avoid copying objects and just copy pointers. 
Additionally, when a shared pointer is deleted, and there is no other shared pointers left pointing to that object,
it also deletes the allocated memory it points to, and therefore prevents memory leaks.

*/
void GeneticAlgorithm::generatePopulation(int * numbers,int amountOfNumbers, int numOfOperations)
{	
	int selectedNodeIndex;
	population.clear();
	for (int i = 0;i < POPULATION_SIZE;i++) {
		population.push_back(std::shared_ptr<OperationTree>(new OperationTree)); 
		std::shared_ptr<OperationTree> currentTree(population[i]); 
		std::shared_ptr<Node > root(currentTree->getRoot());

		treeQueue.clear();// To delete anything from previous trees.
		treeQueue.push_back(&(root->left));
		treeQueue.push_back(&(root->right));

		for (int j = 0; j < (numOfOperations - 1); j++) {
			selectedNodeIndex = (int)(rand() % treeQueue.size());
			(*treeQueue[selectedNodeIndex]) = std::make_shared<Node>();
			treeQueue.push_back(&((*treeQueue[selectedNodeIndex])->left));
			treeQueue.push_back(&((*treeQueue[selectedNodeIndex])->right));
			currentTree->addNode(*treeQueue[selectedNodeIndex]);
			treeQueue.erase(treeQueue.begin() + selectedNodeIndex);
		}
		treeQueue.clear();		

		currentTree->addRandomOperations();
		currentTree->initializeNumberNodes();
		currentTree->addNumbers(numbers,amountOfNumbers);

		//The code below is only for test purposes.It can be uncommented if needed or desired
		//It prints out the first generation. Similar to what printPopulation() does for each generation.

		//std::cout << "\n" << i+1 << ". member generated : "  << 
		//	currentTree->printTree(root) << " " << calculateOperation(currentTree->getRoot()) << std::endl;
	}
	std::cout << "\n\nPopulation generated.\n";
	system("PAUSE");
}

// The function that imitates the evolution process.
void GeneticAlgorithm::evolve(int targetNumber)
{
	bool targetFound = false;
	int numOfGenerations = 1;

	while (!targetFound) {
		std::cout << "Generation: " << numOfGenerations << std::endl;		
		//The belove function is for test purposes and can be uncommented to see each member of each generation on the console.
		//printPopulation();
		targetFound = calculateFitness(targetNumber);
		if (!targetFound) {
			naturalSelection();
			reproduction();
		}
		else { // If targetFound == true
			//Victory message!
			std::cout << "\n\nPERFECT MEMBER HAS BEEN FOUND!\n" << std::endl;
			std::cout << numOfGenerations << ". generation " << perfectMemberIndex+1 << ". member : " << thePerfectMember.printTree(thePerfectMember.getRoot()) << " "
				<< calculateOperation(thePerfectMember.getRoot()) << "\n" << std::endl;
		}
		numOfGenerations++;
	}
}

//Calculates the fitness of each member and returns true if a member with %100 fitness is found.
//Returns false otherwise.
bool GeneticAlgorithm::calculateFitness(int targetNumber)
{
	int operationResult;
	float fitness;
	int difference;
	for (int i = 0;i < population.size();i++) {
		operationResult = calculateOperation(population[i]->getRoot());
		difference = abs(targetNumber - operationResult);
		fitness = 1/(difference + 1);// Difference and fitness are inversely proportional. Fitness' interval is (0,1].
		if (fitness == 1) {			
			thePerfectMember = *(population[i].get());
			perfectMemberIndex = i;
			return true;
		}
		population[i]->setFitness(fitness);
	}	
	return false;
}

// A recursive function to calculate the result of an operation and return it.
// Note: This calculation does integer division.
int GeneticAlgorithm::calculateOperation(std::shared_ptr<Node > root)
{
	int result;
	switch (root->charValue)
	{
	case '/':
		return calculateOperation(root->left) / ((result = calculateOperation(root->right)) ? result :1);//to avoid divide by 0, returns 1 if the denominater results 0.
		break;
	case '*':
		return calculateOperation(root->left) * calculateOperation(root->right);
		break;
	case '-':
		return calculateOperation(root->left) - calculateOperation(root->right);
		break;
	case '+':
		return calculateOperation(root->left) + calculateOperation(root->right);
		break;
	case 0:
		return root->intValue;
		break;
	default:
		std::cout << "Error calculatingOperation!!";
		return -1;
		break;
	}
}

// The function that imitates the natural selection.
// First, a section of the population is killed to simulate death of the weak (lower fitness) ones.
// Then a selection is made with a higher possibility of selecting the stronger ones.
void GeneticAlgorithm::naturalSelection()
{
	theWeakIsDead();
	orderSelection();	
}

//Generates a new child tree by taking features from two parents and adds it to the population.
void GeneticAlgorithm::reproduction()
{
	OperationTree child;

	for (int i = 0;(i + 1) < reproducers.size();i += 2) {
		OperationTree parent1 = *reproducers[i];
		OperationTree parent2 = *reproducers[i + 1];
		// Instead of taking one half of the tree from one parent and one half from the other,
		//we first copy one of the parents directly to the child and take 2 random operations from the other
		//parent, inside crossOver().
		child = parent1;
		child.setFitness(0); //Because child's fitness will need to be calculated again

		crossOver(parent1, parent2, child);
		mutation(child);

		//The new member is added to the population.
		population.push_back(std::make_shared<OperationTree>(child));
	}	
}

void GeneticAlgorithm::mutation(OperationTree child)
{
	char operations[] = {'/','*','+','-'};
	int probOfMutation = 1 + rand() % 100;
	// The probability of an operation-mutation is directly proportional with the number of operation nodes.
	int percentageOfOperationMutation = 10;//child.getNumOfOperations(); // For this program,this is 5, because we have 6 numers each time.
	
	//This number-mutation has a big possibility because 
	//our chromosomes (trees) are not big enough to create much diversity,even with cross-over.
	int percentageOfNumberMutation = 10;

	int numOfOperationMutation = 1 + (probOfMutation % 5);// This gives a number in the range of [1,5]
	if (probOfMutation < percentageOfOperationMutation) {
		for (int i = 0;i < numOfOperationMutation;i++) {
			int randomIndex = rand() % 4;
			child.setOperation(i, operations[randomIndex]);
		}		
	}
	// Probability of mutation randomized again to make two mutations independent.
	probOfMutation = 1 + rand() % 100;
	if (probOfMutation < percentageOfNumberMutation) {
		child.swapRandomNumbers();
	}
}

// It may technically not be a cross-over here, but it provides the same results.
void GeneticAlgorithm::crossOver(OperationTree parent1, OperationTree parent2, OperationTree child)
{
	int randomOperation1, randomOperation2;	
	randomOperation1 = rand() % parent1.getNumOfOperations();
	do {
		randomOperation2 = rand() % parent1.getNumOfOperations();
	} while (randomOperation1 == randomOperation2);

	// In the end, the child gets 2 of it's operations from parent2, 3 of them from parent 1.
	child.getOperationFromSecondParent(child.getOperationNodePtr(randomOperation1), parent2.getOperationNodePtr(randomOperation2));
	child.getOperationFromSecondParent(child.getOperationNodePtr(randomOperation2), parent2.getOperationNodePtr(randomOperation1));

	//// With %50 chance, the child gets parent2's numbers.
	int randomForSwappingNumbers = rand() % 2;
	if (!randomForSwappingNumbers)
		child.setNumberTree(parent2.getNumberNodes());
}


// This function basically sorts the population, and picks a random member from the last "numOfDeadAndNewBorn" members each time,and kills it.
// This gives a higher possibility for the weak ones to die, but gives them a chance to live.
void GeneticAlgorithm::theWeakIsDead()
{
	sortPopulation();
	int i = 1;
	int whoWillDie;
	while(i < (numOfDeadAndNewBorn + 1)) {
		whoWillDie = 1 + rand() % numOfDeadAndNewBorn;
		population.erase(population.end() - whoWillDie);
		i++;		
	}
}

/*Applies order selection to the sorted population.

For population size = n:

sampleSpace = 1 + 2 + 3 + ...... (n - 1) + n = n*(n + 1)/2

Then a random number is selected from [0,sampleSpace).
Then, Binary Search is used to find the interval of randomIntervalValue.

If, for example, the randomIntervalValue is 5, it's in between 2(1 + 2) and 3(1 + 2 + 3)
In this case, lowerLimit is selected to determine selectedMemberIndex.
If, lowerLimit is 2, as in the example above, (n - 2) becomes selectedMemberIndex.

Basically, members with lower indexes (and higher fitness values) have higher possibilities of being selected.
So the first member is selected if randomIntervalValue happens to be in the last n numbers (interval of (sampleSpace - n,sampleSpace] ),
and the last member is selected if randomIntervalValue is 0.
*/
void GeneticAlgorithm::orderSelection()
{
	int sampleSpace = (population.size()*(population.size() + 1)) / 2;
	int selectedMemberIndex;
	int randomIntervalValue;
	int lowerLimit;
	int upperLimit;
	int currentIndex;
	int totalSumUntilCurrentIndex;

	reproducers.clear();
	 
	for (int i = 0;i < (numOfDeadAndNewBorn * 2);i++) {
		OperationTree currentMember;
		randomIntervalValue = rand() % sampleSpace;
		lowerLimit = 0;
		upperLimit = population.size();

		//Binary Search
		while (upperLimit != (lowerLimit + 1)) {
			currentIndex = (lowerLimit + upperLimit) / 2;
			totalSumUntilCurrentIndex = currentIndex*(currentIndex + 1) / 2;
			if (randomIntervalValue >= totalSumUntilCurrentIndex) {
				lowerLimit = currentIndex;
			}
			else {
				upperLimit = currentIndex;
			}		
		}
		selectedMemberIndex = population.size() - lowerLimit - 1;
		currentMember = *population[selectedMemberIndex];
		reproducers.push_back(std::make_shared<OperationTree>(currentMember));
	}
}

//Sorts the population based on their fitness values in descending order.
//Uses Shell Sort algortihm.
void GeneticAlgorithm::sortPopulation()
{
	int k = 1;
	int gap = (int)(population.size() / pow(2,k));
	int i,j;
	while (gap > 0) {
		for (j = 0;j < (population.size() - gap);j++) {
			i = j;
			while (i >= 0) {
				if (population[i]->getFitness() < population[i + gap]->getFitness()) {
					std::iter_swap(population.begin() + i, population.begin() + (i + gap));
				}
				i -= gap;
			}
		}
		k++;
		gap = (int)(population.size() / pow(2, k));
	}
}

void GeneticAlgorithm::printPopulation()
{
	std::cout << "" << std::endl;
	for (int i = 0; i < population.size();i++) {
		std::cout << i + 1 << "--> " << population[i]->printTree(population[i]->getRoot()) 
			<< " = " << calculateOperation(population[i]->getRoot()) << std::endl;
	}
}


