#include "master.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <cstddef>



Master::Master(int colorNumber, int spotNumber){

	this->colorNumber = colorNumber;
	this->spotNumber = spotNumber;
	buildSolution();

}

void Master::buildSolution(){
	/*
	* Build a random solution regarding the number of colors and the size of the solution
	*/

	srand (time(NULL));
	int color;
	for(int i = 0; i < this->spotNumber; i++){

		color = rand()%(this->colorNumber);

		while(std::find(this->solution.begin(), this->solution.end(), color) != this->solution.end()) {
			//A color must appear one and only one time in the solution
 			color = rand()%(this->colorNumber);
		}

		this->solution.push_back(color);
	}

}

void Master::printSolution(){
	/*
	* Print the solution of the game
	*/

	int index;
	printf("The solution is ");
	std::vector<int>::iterator it;
	for(it = this->solution.begin(); it != this->solution.end(); it++){
		printf("%d ", *it);
	}
	printf("\n");
}


std::vector<int> Master::evaluate(std::vector<int> solution){
	/*
	* Give the evaluation of a given solution regarding the
	* true solution of the game
	*/

	std::vector<int>::iterator it;

	std::vector<int> result;
	int perfect, colorOnly = 0;

	for(it = solution.begin(); it != solution.end(); it++){
		//Iterate on the colors of the given solution
		ptrdiff_t pos = find(solution.begin(), solution.end(), *it) - solution.begin();
		ptrdiff_t pos2 = find(this->solution.begin(), this->solution.end(), *it) - this->solution.begin();

		if(int(pos2) < solution.size()){
			//If the color is found in both the solution given and the true solution
			if(int(pos) == int(pos2)){
				perfect++;
			}
			else{
				colorOnly++;
			}
		}
	}

	result.push_back(perfect);
	result.push_back(colorOnly);

	if(perfect == spotNumber){
		this->isNotOver = false;
	}

	return result;
}


