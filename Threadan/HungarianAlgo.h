#ifndef HUNGARIANALGORITHM_H
#define HUNGARIANALGORITHM_H

#include <vector>

// http://community.topcoder.com/tc?module=Static&d1=tutorials&d2=hungarianAlgorithm
class AssignmentProblemSolver
{
private:
	// --------------------------------------------------------------------------
	// Computes the optimal assignment (minimum overall costs) using Munkres algorithm.
	// --------------------------------------------------------------------------
	void assignmentoptimal(int* assignment, float* cost, float* distMatrix, int nOfRows, int nOfColumns);
	void buildassignmentvector(int* assignment, bool* starMatrix, int nOfRows, int nOfColumns);
	void computeassignmentcost(int* assignment, float* cost, float* distMatrix, int nOfRows);
	void step2a(int* assignment, float* distMatrix, bool* starMatrix, bool* newStarMatrix, bool* primeMatrix, bool* coveredColumns, bool* coveredRows, int nOfRows, int nOfColumns, int minDim);
	void step2b(int* assignment, float* distMatrix, bool* starMatrix, bool* newStarMatrix, bool* primeMatrix, bool* coveredColumns, bool* coveredRows, int nOfRows, int nOfColumns, int minDim);
	void step3(int* assignment, float* distMatrix, bool* starMatrix, bool* newStarMatrix, bool* primeMatrix, bool* coveredColumns, bool* coveredRows, int nOfRows, int nOfColumns, int minDim);
	void step4(int* assignment, float* distMatrix, bool* starMatrix, bool* newStarMatrix, bool* primeMatrix, bool* coveredColumns, bool* coveredRows, int nOfRows, int nOfColumns, int minDim, int row, int col);
	void step5(int* assignment, float* distMatrix, bool* starMatrix, bool* newStarMatrix, bool* primeMatrix, bool* coveredColumns, bool* coveredRows, int nOfRows, int nOfColumns, int minDim);

	void assignmentsuboptimal2(int* assignment, float* cost, float* distMatrixIn, int nOfRows, int nOfColumns);

public:
	enum TMethod { optimal, many_forbidden_assignments, without_forbidden_assignments };
	AssignmentProblemSolver();
	float Solve(std::vector< std::vector<float> >& DistMatrix, std::vector<int>& Assignment, TMethod Method = optimal);

	std::vector<size_t> DoAlgo(const std::vector< std::vector<float> >& CostMatrix);
};

#endif // !HUNGARIANALGORITHM_H
