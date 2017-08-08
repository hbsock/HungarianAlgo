#include "HungarianAlgo.h"
#include <limits>
#include <algorithm>

std::vector<size_t> AssignmentProblemSolver::DoAlgo(const std::vector<std::vector<float>>& CostMatrix)
{
	std::vector<std::vector<float>> C = CostMatrix;
	const size_t N = C.size();
	const size_t M = C.front().size();

	//step 1
	for (size_t row = 0; row < N; ++row)
	{
		auto &RowVector = C.at(row);
		float row_min_val = *std::min_element(RowVector.begin(), RowVector.end());
		for (auto &row_elem : RowVector)
		{
			row_elem -= row_min_val;
		}
	}

	//step 2
	for (size_t col = 0; col < M; ++col)
	{
		float min_col_val = std::numeric_limits<float>::max();
		for (size_t row = 0; row < N; row++)
		{
			if (C[row][col] < min_col_val)
			{
				min_col_val = C[row][col];
			}
		}

		for (size_t row = 0; row < N; row++)
		{
			C[row][col] -= min_col_val;
		}
	}

	//step 3
	std::vector<bool> RowCover(N, false);
	std::vector<bool> ColCover(M, false);

	return std::vector<size_t>();
}