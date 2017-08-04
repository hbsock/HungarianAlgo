#include "HungarianAlgo.h"
#include <iostream>

int main()
{
	std::vector< std::vector<float> > Cost = {
		{90, 75, 75, 80},
		{35, 85, 55, 65},
		{125, 95, 90, 105},
		{45, 110, 95, 115}
	};
	std::vector<int> assignment;

	AssignmentProblemSolver Hungarian;
	Hungarian.Solve(Cost, assignment, AssignmentProblemSolver::optimal);

	for (size_t i = 0; i < assignment.size(); ++i)
	{
		std::cout << "Row: " << i << " Col: " << assignment.at(i) << std::endl;
	}

	return 0;
}