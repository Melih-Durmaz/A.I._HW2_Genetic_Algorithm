#pragma once
#include <vector>
#include <memory>
#include "OperationTree.h"

class GeneticAlgorithm
{
private:
	std::vector<std::shared_ptr<Node>*> treeQueue;// The vector that is used only while creating a new random binary tree(Operation Trees).
	std::vector<std::shared_ptr<OperationTree>> population;//Stores population members of type OperationTree.
	std::vector<std::shared_ptr<OperationTree>> reproducers;// Members selected for reproduction is copied to this vector.
	int numOfGenerations;
	OperationTree thePerfectMember;// When found, stores the member with %100 fitness.
	int perfectMemberIndex;//Stores in which index the perfect member is.

	int numOfDeadAndNewBorn;// Stores the amount of members that will die and will be born each generation.
	
public:
	GeneticAlgorithm();
	static int const POPULATION_SIZE = 150;
	void evolve(int targetNumber);
	void generatePopulation(int* numbers, int amountOfNumbers, int treeSize);
	bool calculateFitness(int targetNumber);
	void naturalSelection();
	void reproduction();
	void mutation(OperationTree child);
	void crossOver(OperationTree parent1, OperationTree parent2, OperationTree child);
	void theWeakIsDead();
	void orderSelection();
	void sortPopulation();
	int calculateOperation(std::shared_ptr<Node > root);
	void printPopulation();
};

