#include "mpi.h"
#include <stdlib.h>
#include <stdio.h>
#include "master.hpp"
#include "player.hpp"
#include <algorithm>

int main(int argc, char **argv) {
	/**
	*Main function handling the running of the game
	**/

	int value, rank, size;
	MPI_Status status;

	MPI_Init( &argc, &argv );
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int colorNumber = 9;
	int numberSpots = 5;
	std::vector<int> evaluation; //Evaluation of the master computed from a solution sent by a node
	int* sendArray;

	if(rank == 0){
		/*
		* Rank 0 is the master
		*/
		Master* master = new Master(colorNumber, numberSpots);
		master->printSolution();
		std::vector<int> trueSolution = master->getSolution();

		sendArray = &trueSolution[0];

		int* solutionArray = (int*) malloc(numberSpots * size * sizeof(int)); //Will receive all solutions from all nodes
		std::vector<std::vector<int>> solutions;

		bool legit;
		std::vector<bool> legitSol;
		/*
		* Determine whether or not the solution sent by the node is the "stopSolution"
		* A stopSolution is <0,0,...,0> and means that the node who sent this solution 
		* has no more plausible solution and must stop running 
		*/

		while(master->getIsNotOver()){


			MPI_Gather(sendArray, numberSpots, MPI_INT, solutionArray, numberSpots, MPI_INT, 0, MPI_COMM_WORLD); 

			std::vector<int> tempVector;

			for (int i = numberSpots; i < numberSpots * size ; i++) {
				//Add all solutions in solutions
	   			tempVector.push_back(solutionArray[i]);
	   			if((i + 1) % numberSpots == 0 && i != numberSpots){
	   				solutions.push_back(tempVector);
	   				tempVector.clear();
	   			}
	   		}

	   		printf("Received : ");
	   		for(std::vector<std::vector<int>>::iterator it2 = solutions.begin(); it2 != solutions.end(); it2++){
	   			//Print the solutions received and determine wheter ot not they are of the form <0,0,...,0>
	   			legit = false;
	   			for(std::vector<int>::iterator it3 = it2->begin(); it3 != it2->end(); it3++){
	   				printf("%d ", *it3);
	   				if(*it3 != 0) legit = true;
	   			}
	   			legitSol.push_back(legit);
	   			printf(" and ");
	   		}
	   		printf("\n");

	   		int randomIndex;
	   		do{
	   			//Pick a solution which is not of the form <0,0,0,...,0>
	   			randomIndex = rand() % solutions.size();
	   			legit = legitSol[randomIndex];
	   		}while(!legit);


	   		std::vector<int> &chosenSolution = solutions[randomIndex];

	   		printf("Evaluating the solution : ");
	   		for(std::vector<int>::iterator it = chosenSolution.begin(); it != chosenSolution.end(); it++){
	   			printf("%d ", *it);
	   		}
	   		printf("\n");


			evaluation = master->evaluate(chosenSolution);

			//Broadcast the solutions with it's evaluation
			sendArray = &chosenSolution[0];
			MPI_Bcast(sendArray, numberSpots, MPI_INT, 0, MPI_COMM_WORLD);
			sendArray = &evaluation[0];
			MPI_Bcast(sendArray, 2 ,MPI_INT, 0, MPI_COMM_WORLD);

			solutions.clear();
			legitSol.clear();
		}
		printf("SOLUTION FOUND, EXIT PROGRAM\n");
		MPI_Finalize();

	}

	else {
		std::vector<int> solution;
		std::vector<int> attributedColors;
		int* recvSol;
		int* recvEval;

		recvSol = (int*) malloc(numberSpots * sizeof(int));
		recvEval = (int*) malloc(2 * sizeof(int));

		int j = 0;
		for (int i = 0; i < colorNumber; i++){
			//Fairly attribute all the colors to the nodes
			if(rank == j+1){
				attributedColors.push_back(i);
			}
			j++;
			if(j > (size-2)){
				j = 0;
			}
		}

		Player* player = new Player(colorNumber, attributedColors, numberSpots);
		player->generateSolutions();
		std::vector<int> nodeSolution = player->getPlausibleSolution(true);

		while(true){

			sendArray = &nodeSolution[0];
			//Send the plausible solution to the master
			MPI_Gather(sendArray, numberSpots, MPI_INT, NULL, NULL, MPI_INT, 0, MPI_COMM_WORLD);

			//Receive the evaluated solution from the master
			MPI_Bcast(recvSol, numberSpots, MPI_INT, 0, MPI_COMM_WORLD);
			for (int i = 0; i < numberSpots; i++) solution.push_back(recvSol[i]);
			player->addSolution(solution);
	   		solution.clear();

	   		//Receive the evaluation from the master
			MPI_Bcast(recvEval, 2,MPI_INT, 0, MPI_COMM_WORLD);
			for (int j = 0; j < 2; j++) evaluation.push_back(recvEval[j]);
			player->addEvaluation(evaluation);
			if(evaluation[0] == numberSpots) break; //If all colors are perfect the solution is found
			evaluation.clear();

			nodeSolution = player->getPlausibleSolution(false);

		}
		//sendArray = &nodeSolution[0];
		//MPI_Gather(sendArray, numberSpots, MPI_INT, NULL, NULL, MPI_INT, 0, MPI_COMM_WORLD);

		printf("Node %d shutting down\n", rank);
		MPI_Finalize();
	}
	return 0;

}