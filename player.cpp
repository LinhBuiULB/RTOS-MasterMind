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
	    	for (int i = 0; i < colors.size(); ++i) {			//Compute the subset
	    		if(i == *it) j++;
            	if (v[i]) {
                	solution.push_back(j);
                	sort(solution.begin(), solution.end());
                }
                j++;
            }

            do{
            	this->possibleSolutions.push_back(solution);					//Add the solution
            }while (std::next_permutation(solution.begin(), solution.end()));	//Permute the subset

	    } while (std::next_permutation(v.begin(), v.end()));	//Permute v
	    														//By permuting v and then permuting the subset computed from v, we get all the permutation of n elements in x colors

	    for (it2 = this->possibleSolutions.begin() + currentPos; it2 != this->possibleSolutions.end(); it2++){
	    	it2->insert(it2->begin(), *it);		//insert the basic color at the beginning of all solutions


	    	for (std::vector<int>::iterator it3 = it2->begin(); it3 != it2->end(); it3++)
	    		printf("%d ", *it3);
	    	printf("\n");
	    }

	    currentPos = this->possibleSolutions.size(); 

		colors.push_back(*it);
		sort(colors.begin(), colors.end());
	}
	
}

void Player::printAttributedColors(){

	std::vector<int>::iterator it;
	printf("My colors are ");
	for(it = this->attributedColors.begin(); it != this->attributedColors.end(); it++){
		printf("%d ", *it);
	}
	printf("\n");

}

std::vector<int> Player::getPlausibleSolution(bool isBegin){

	if(isBegin){
		return this->possibleSolutions[0];
	}
	return this->possibleSolutions[0];
}
