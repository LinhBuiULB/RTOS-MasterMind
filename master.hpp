#ifndef _MASTER_HPP_
#define _MASTER_HPP_
#include <vector>
#include <string>

class Master {

private:

	std::vector<int> solution;
	int colorNumber;
	int spotNumber;
	bool isNotOver = true;

public:

	Master(int, int);
	~Master() = default;

	void buildSolution();
	void printSolution();
	std::vector<int> evaluate(std::vector<int>);

	inline std::vector<int> getSolution() 	const{return this->solution;}
	inline int getColorNumber()				const{return this->colorNumber;}
	inline bool getIsNotOver()				const{return this->isNotOver;}

};


#endif