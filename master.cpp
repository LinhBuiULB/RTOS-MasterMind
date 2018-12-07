#include "master.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>


Master::Master(int colorNumber, int spotNumber){

	this->colorNumber = colorNumber;
	this->spotNumber = spotNumber;
	buildSolution();

}

void Master::buildSolution(){

	srand (time(NULL));
	int color;
	for(int i = 0; i < this->spotNumber; i++){

		color = rand()%(this->colorNumber);

		while(std::find(this->solution.begin(), this->solution.end(), color) != this->solution.end()) {
 			color = rand()%(this->colorNumber);
		}

		this->solution.push_back(color);
	}

}

void Master::printSolution(){

	int index;
	printf("The solution is ");
	std::vector<int>::iterator it;
	for(it = this->solution.begin(); it != this->solution.end(); it++){
		printf("%d ", *it);
	}


}