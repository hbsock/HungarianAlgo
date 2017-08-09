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
		auto coveredResults = getMaxFlow(tempCostMatrix);

		for (size_t row = 0; row < coveredResults.size(); ++row)
		{
			std::vector<bool> ColMarkedInThisRow(maxCol, false);
			if (coveredResults[row] == INVALID_ASSIGNMENT)
			{
				RowMarked[row] = true;
				
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
							if (tempCostMatrix[r][col] == 0)
							{
								RowMarked[r] = true;
							}
						}
					}
				}
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