#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_
#include <vector>
#include <string>

class Player{

private:

	std::vector<int> attributedColors;
	std::vector<std::vector<int>> possibleSolutions;
	int colorNumber;
	int spotNumber;

public:

	Player(int, std::vector<int>, int);
	~Player() = default;

	inline std::vector<int> getAttributedColors()					const{return this->attributedColors;}
	inline std::vector<std::vector<int>> getPossibleSolutions()		const{return this->possibleSolutions;}	

	void generateSolutions();
	void printAttributedColors();
	std::vector<int> getPlausibleSolution(bool);

};


#endif