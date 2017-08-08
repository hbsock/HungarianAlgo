#include "HungarianAlgo.h"
#include <iostream>

int main()
{
	std::vector< std::vector<float> > SimpleCost = {
		{250, 400, 350},
		{400, 600, 350},
		{200, 400, 250}
	};
	
	std::vector< std::vector<float> > Cost = {
		{90, 75, 75, 80},
		{35, 85, 55, 65},
		{125, 95, 90, 105},
		{45, 110, 95, 115}
	};
	std::vector<int> assignment;

	AssignmentProblemSolver Hungarian;

	auto result = Hungarian.DoAlgo(Cost);
	//auto result = Hungarian.DoAlgo(SimpleCost);
	for (size_t i = 0; i < result.size(); ++i)
	{
		std::cout << "Row: " << i << " Col: " << result.at(i) << std::endl;
	}

	return 0;
}