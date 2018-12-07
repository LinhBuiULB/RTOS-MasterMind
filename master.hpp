#ifndef _MASTER_HPP_
#define _MASTER_HPP_
#include <vector>
#include <string>

class Master {

private:

	std::vector<int> solution;
	int colorNumber;
	int spotNumber;

public:

	Master(int, int);
	~Master() = default;

	void buildSolution();
	void printSolution();

	inline std::vector<int> getSolution() 	const{return this->solution;}
	inline int getColorNumber()				const{return this->colorNumber;}

};


#endif