#include "HungarianAlgo.h"
#include <limits>
#include <algorithm>
#include <cassert>

constexpr size_t INVALID_ASSIGNMENT = std::numeric_limits<size_t>::max();

namespace
{

	bool optimization(int row, std::vector<size_t> &rows, std::vector<bool> &occupiedCols, const std::vector<std::vector<float>> &values) {
		if (row == rows.size()) // If all rows were assigned a cell
			return true;

		for (int col = 0; col < values.size(); col++) { // Try all columns
			if (values[row][col] == 0 && occupiedCols[col] == false) { // If the current cell at column `col` has a value of zero, and the column is not reserved by a previous row
				rows[row] = col; // Assign the current row the current column cell
				occupiedCols[col] = true; // Mark the column as reserved
				if (optimization(row + 1, rows, occupiedCols, values)) // If the next rows were assigned successfully a cell from a unique column, return true
					return true;
				occupiedCols[col] = false; // If the next rows were not able to get a cell, go back and try for the previous rows another cell from another column
			}
		}
		return false; // If no cell were assigned for the current row, return false to go back one row to try to assign to it another cell from another column
	}

	std::vector<size_t> optimization(const std::vector<std::vector<float>> &values) {
		std::vector<size_t> rows(values.size(), INVALID_ASSIGNMENT);
		std::vector<bool> occupiedCols(values.size(), false);
		if (optimization(0, rows, occupiedCols, values))
			return rows;
		else
			throw std::exception("No valid assignment.");
	} //End optimization


	  // A DFS based recursive function that returns true if a
	  // matching for vertex u is possible
	bool bpm(const std::vector<std::vector<bool>> &bpGraph, 
		int u, 
		std::vector<bool> &seen, 
		std::vector<size_t> &matchR)
	{
		// Try every job one by one
		for (int v = 0; v < bpGraph.front().size(); v++)
		{
			// If applicant u is interested in job v and v is
			// not visited
			if (bpGraph[u][v] && !seen[v])
			{
				seen[v] = true; // Mark v as visited

								// If job 'v' is not assigned to an applicant OR
								// previously assigned applicant for job v (which is matchR[v]) 
								// has an alternate job available. 
								// Since v is marked as visited in the above line, matchR[v] 
								// in the following recursive call will not get job 'v' again
				if (matchR[v] == INVALID_ASSIGNMENT || bpm(bpGraph, matchR[v], seen, matchR))
				{
					matchR[v] = u;
					return true;
				}
			}
		}
		return false;
	}

	// Returns maximum number of matching from M to N
	std::vector<size_t> maxBPM(const std::vector<std::vector<bool>> &bpGraph)
	{
		// An array to keep track of the applicants assigned to
		// jobs. The value of matchR[i] is the applicant number
		// assigned to job i, the value -1 indicates nobody is
		// assigned.
		std::vector<size_t> matchR(bpGraph.size(), INVALID_ASSIGNMENT);

		int result = 0; // Count of jobs assigned to applicants
		for (int u = 0; u < bpGraph.size(); u++)
		{
			// Mark all jobs as not seen for next applicant.
			std::vector<bool> seen(bpGraph.at(u).size(), false);

			// Find if the applicant 'u' can get a job
			if (bpm(bpGraph, u, seen, matchR))
				result++;
		}
		return matchR;
	}

	void markCostMatrix(size_t row, 
		size_t maxRow, 
		size_t maxCol,
		const std::vector<std::vector<float>> &tempCostMatrix,
		std::vector<bool> &ColMarked,
		std::vector<bool> &RowMarked,
		const std::vector<size_t> &coveredResults)
	{
		if (RowMarked[row] == false)
		{
			return;
		}

		std::vector<bool> ColMarkedInThisRow(maxCol, false);
		for (size_t col = 0; col < maxCol; ++col)
		{
			if (tempCostMatrix[row][col] == 0)
			{
				ColMarked[col] = true;
				ColMarkedInThisRow[col] = true;
			}
		}

		for (size_t col = 0; col < maxCol; ++col)
		{
			if (ColMarkedInThisRow[col])
			{
				for (size_t r = 0; r < maxRow; ++r)
				{
					if (coveredResults[r] == col && !RowMarked[r])
					{
						RowMarked[r] = true;
						markCostMatrix(r, maxRow, maxCol, tempCostMatrix, ColMarked, RowMarked, coveredResults);
					}
				}
			}
		}
	}
}

std::vector<size_t> AssignmentProblemSolver::DoAlgo(const std::vector<std::vector<float>>& CostMatrix)
{
	std::vector<std::vector<float>> tempCostMatrix = CostMatrix;
	const size_t maxRow = tempCostMatrix.size();
	const size_t maxCol = tempCostMatrix.front().size();
	assert(maxRow == maxCol);

	//step 1
	for (size_t row = 0; row < maxRow; ++row)
	{
		auto &RowVector = tempCostMatrix.at(row);
		float row_min_val = *std::min_element(RowVector.begin(), RowVector.end());
		for (auto &row_elem : RowVector)
		{
			row_elem -= row_min_val;
		}
	}

	//step 2
	for (size_t col = 0; col < maxCol; ++col)
	{
		float min_col_val = std::numeric_limits<float>::max();
		for (size_t row = 0; row < maxRow; row++)
		{
			if (tempCostMatrix[row][col] < min_col_val)
			{
				min_col_val = tempCostMatrix[row][col];
			}
		}

		for (size_t row = 0; row < maxRow; row++)
		{
			tempCostMatrix[row][col] -= min_col_val;
		}
	}


	while (true)
	{
		//step 3
		std::vector<bool> RowMarked(maxRow, false);
		std::vector<bool> ColMarked(maxCol, false);
		//auto coveredResults = optimization(tempCostMatrix);

		std::vector<std::vector<bool>> BipartiteGraph(maxRow); 
		for (size_t row = 0; row < maxRow; ++row)
		{
			for (size_t col = 0; col < maxCol; ++col)
			{
				BipartiteGraph.at(row).resize(maxCol);
				BipartiteGraph[row][col] = (tempCostMatrix[row][col] == 0);
			}
		}
		
		auto coveredResults = maxBPM(BipartiteGraph);

		for (size_t row = 0; row < coveredResults.size(); ++row)
		{
			if (coveredResults[row] == INVALID_ASSIGNMENT)
			{
				RowMarked[row] = true;
				markCostMatrix(row, maxRow, maxCol, tempCostMatrix, ColMarked, RowMarked, coveredResults);
			}
		}
		
		size_t lineCount = std::count_if(coveredResults.begin(), coveredResults.end(), [](size_t val) { 
			return val != INVALID_ASSIGNMENT; 
		});

		if (lineCount == maxCol )
		{
			break;
		}
		else
		{
			//step 4
			float minVal = std::numeric_limits<float>::max();
			for (size_t row = 0; row < maxRow; ++row)
			{
				for (size_t col = 0; col < maxCol; ++col)
				{
					if (!(!RowMarked[row] || ColMarked[col]))
					{
						if (minVal > tempCostMatrix[row][col])
						{
							minVal = tempCostMatrix[row][col];
						}
					}
				}
			}

			for (size_t row = 0; row < maxRow; ++row)
			{
				for (size_t col = 0; col < maxCol; ++col)
				{
					if (RowMarked[row])
					{
						tempCostMatrix[row][col] -= minVal;
					}

					if (ColMarked[col])
					{
						tempCostMatrix[row][col] += minVal;
					}
				}
			}
		}
	}

	return optimization(tempCostMatrix);
}