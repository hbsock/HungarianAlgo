#ifndef HUNGARIANALGORITHM_H
#define HUNGARIANALGORITHM_H

#include <vector>
#include <memory>

class AssignmentProblemSolver
{
public:
	AssignmentProblemSolver();
	~AssignmentProblemSolver();

	std::vector<size_t> DoHungarianAlgo(const std::vector< std::vector<float> >& CostMatrix);
private:
	class impl;
	std::unique_ptr<impl> pimpl;
};

#endif // !HUNGARIANALGORITHM_H
