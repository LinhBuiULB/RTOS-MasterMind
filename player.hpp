#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_
#include <vector>
#include <string>

class Player{

private:

	std::vector<int> attributedColors;
	std::vector<std::vector<int>> possibleSolutions;
	std::vector<std::vector<int>> evaluations;
	std::vector<std::vector<int>> solutions;
	int colorNumber;
	int spotNumber;
	bool running = true;
	int currentPos = 0;

public:

	Player(int, std::vector<int>, int);
	~Player() = default;

	inline std::vector<int> getAttributedColors()					const{return this->attributedColors;}
	inline std::vector<std::vector<int>> getPossibleSolutions()		const{return this->possibleSolutions;}	
	inline std::vector<std::vector<int>> getEvaluations()			const{return this->evaluations;}
	inline std::vector<std::vector<int>> getSolutions()				const{return this->solutions;}	
	inline bool getRunning()										const{return this->running;}		


	void generateSolutions();
	void printAttributedColors();
	std::vector<int> getPlausibleSolution(bool);
	bool isPlausible(std::vector<int>, std::vector<int>, std::vector<int>);
	void addEvaluation(std::vector<int>);
	void addSolution(std::vector<int>);
	std::vector<int> createStopSolution();

};


#endif