#include "mpi.h"
#include <stdlib.h>
#include <stdio.h>
#include "player.hpp"
#include <algorithm>


Player::Player(int colorNumber, std::vector<int> colors, int spotNumber){

	this->attributedColors = colors;
	this->colorNumber = colorNumber;
	this->spotNumber = spotNumber;

}

void Player::generateSolutions(){
	/*
	* Generate all solutions beginning with the atributed colors by brute force
	* and store it in possibleSolutions
	*/

	std::vector<int>::iterator it;
	std::vector<std::vector<int>>::iterator it2;
	std::vector<int> colors;
	std::vector<int> solution;
	int currentPos = 0;

	for (int i = 0; i < this->colorNumber; i++){
		colors.push_back(i);
	}

	for(it = this->attributedColors.begin(); it != this->attributedColors.end(); it++){  //*it is the basic color (One of the color attributed to the process)

		ptrdiff_t pos = find(colors.begin(), colors.end(), *it) - colors.begin();

		colors.erase(colors.begin() + pos);
		
		std::vector<bool> v(colors.size());
    	std::fill(v.end() - this->spotNumber+1, v.end(), true); //if [true, false, true, true, true] we compute all permutations of the subset [0,2,3,4]

	    do {
   			solution.clear();

   			int j = 0; 											//Actually false, because if we have *it = 3 then the subset must be [0,2,4,5]
   			                                                    //Which is why j is computed
	    	for (int i = 0; i < colors.size(); ++i) {	
	    		//Compute the subset		
	    		if(i == *it) j++;
            	if (v[i]) {
                	solution.push_back(j);
                	sort(solution.begin(), solution.end());
                }
                j++;
            }

            do{
            	this->possibleSolutions.push_back(solution);					//Add the subset
            }while (std::next_permutation(solution.begin(), solution.end()));	//Permute the subset

	    } while (std::next_permutation(v.begin(), v.end()));	//Permute v
	    														//By permuting v and then permuting the subset computed from v, we get all the permutation of n elements in x colors

	    for (it2 = this->possibleSolutions.begin() + currentPos; it2 != this->possibleSolutions.end(); it2++){
	    	it2->insert(it2->begin(), *it);		//insert the basic color at the beginning of all solutions
	    }

	    currentPos = this->possibleSolutions.size(); 

		colors.push_back(*it);
		sort(colors.begin(), colors.end());
	}
	
}

void Player::printAttributedColors(){
	/*
	* Print the atributed colors
	*/

	std::vector<int>::iterator it;
	printf("My colors are ");
	for(it = this->attributedColors.begin(); it != this->attributedColors.end(); it++){
		printf("%d ", *it);
	}
	printf("\n");

}

std::vector<int> Player::getPlausibleSolution(bool isBegin){
	/*
	* Run through all possible solutions and return the first
	* plausible one. If no plausible solution found, return a 
	* solution of the form <0,0,...,0>.
	* isBegin determine whether or not this is the first time
	* looking for a plausible solution. If it is, there's no
	* evaluation yet and the function return the first solution.
	*/

	if(isBegin){
		return this->possibleSolutions[0];
	}

	if(this->running){

		std::vector<std::vector<int>>::iterator it;
		std::vector<int> evaluatedSolution;
		std::vector<int> evaluation;
		bool isPlausible;

		for (it = this->possibleSolutions.begin() + this->currentPos; it != this->possibleSolutions.end(); it++){
			//Iterate on each possible solutions
			isPlausible = true;

			for(int i = 0; i < this->evaluations.size(); i++){
				//Iterate on all previous evaluations
				evaluatedSolution = this->solutions[i];
				evaluation = this->evaluations[i];

				if(!this->isPlausible(*it, evaluatedSolution, evaluation)){
					//If the solution contradict one of the evaluation, it is not plausible
					isPlausible = false;
					this->currentPos++;
					break;
				}
			}
			if(isPlausible){
				return *it;
			}

		}
		this->running = false;
	}
	return this->createStopSolution();
}

bool Player::isPlausible(std::vector<int> solution, std::vector<int> evaluatedSolution, std::vector<int> evaluation){
	/*
	* Determine whether or not a solution is plausible regarding the evaluation of another solution
	*/

	int & perfect = evaluation[0];
	int & colorOnly = evaluation[1];

	int correctColors = perfect + colorOnly;
	int matchingColors = 0;
	int matchingPerfect = 0;

	std::vector<int>::iterator it;
	std::vector<int>::iterator it2;


	/* 
	* Let x be the number of Perfect and y the number of ColorOnly.
	* a solution is plausible regarding an evaluation iff it matches 
	* (x+y) colors in the evaluation including x times perfectly.
	*/
	for(it = solution.begin(); it != solution.end(); it++){
		//Iterate on our solution
		for(it2 = evaluatedSolution.begin(); it2 != evaluatedSolution.end(); it2++){
			//Iterate on the evaluated solution
			if(*it == *it2){
				matchingColors++;

				ptrdiff_t pos = find(solution.begin(), solution.end(), *it) - solution.begin();
				ptrdiff_t pos2 = find(evaluatedSolution.begin(), evaluatedSolution.end(), *it2) - evaluatedSolution.begin();

				if(int(pos) == int(pos2)){
					matchingPerfect++;
				}
			}
		}
	}

	bool isPlausible = false;

	if(matchingColors == correctColors && matchingPerfect == perfect)
		 isPlausible = true;

	return isPlausible;

}

void Player::addEvaluation(std::vector<int> evaluation){
	/*
	* Add the evaluation in all evaluations
	*/
	this->evaluations.push_back(evaluation);
}

void Player::addSolution(std::vector<int> solution){
	/*
	* Add the evaluated solution in all the evaluated solutions
	*/
	this->solutions.push_back(solution);
}

std::vector<int> Player::createStopSolution(){
	/*
	* Create a solution of the form <0,0,...,0>
	*/
	std::vector<int> stopSolution;

	for(int i = 0; i < this->spotNumber; i++){
		stopSolution.push_back(0);
	}

	return stopSolution;
}