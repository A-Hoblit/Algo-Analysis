///////////////////////////////////////////////////////////////////////////////
// poly_exp.cpp
//
// Definitions for two algorithms:
//
// soccer_exhaustive
// soccer_dyn_prog
//
///////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <stdexcept>
#include <math.h>

#include "poly_exp.hpp"

int algorithms::soccer_exhaustive(const std::vector<std::string> field) {
    
    // Valid input checks
    int rows = field.size();
    if (rows == 0) {
        throw std::invalid_argument("Empty field.");
    }
    
    int cols = 0;
    int prev_cols = field[0].size();
    for (int i=0; i < rows; i++)
    {
        cols = field[i].size();
        if (cols != prev_cols  || cols == 0)
        {
            throw std::invalid_argument("Rows not same length/# of columns.");
        }
        prev_cols = cols;

        for (int j=0; j < cols; j++)
        {
            if (field[i][j] != '.' && field[i][j] != 'X')
            {
                throw std::invalid_argument("Invalid character.");
            }
        }
    }
    cols = field[0].size();
    int length = rows + cols - 2;
    if (length > 31)
    {
        throw std::invalid_argument("Field too large.");
    }

    // Implementation of pseudocode
    int counter = 0;
    int bits = pow(2, length) - 1;
    for (int i=0; i <= bits; i++)
    {
        // Create candidate
        std::vector<int> candidate = {};
        for (int j=0; j <= length-1; j++)
        {
            bool bit = (i >> j) & 1;
            if (bit == 1) // Move right
            {
                candidate.push_back(1);
            }
            else // Move down
            {   
                candidate.push_back(0);
            }
        }  

        // Test candidate
        int down_pos = 0;
        int right_pos = 0;
        for(int k=0; k < candidate.size(); k++)
        {
            //Check position validity
            if (down_pos > rows-1 || right_pos > cols-1)
            {
                break;
            }
            if (field[down_pos][right_pos] == 'X')
            {
                break;
            }
            // Move
            if (candidate[k] == 1) // Move right
            {
                right_pos++;
            }
            else // Move down
            {
                down_pos++;
            }
        }
        if (right_pos == cols-1 && down_pos == rows-1 
            && field[down_pos][right_pos] == '.')
        {
            counter++;
        }
    }
    return counter;
}
 
int algorithms::soccer_dyn_prog(const std::vector<std::string> field) {
    
    // Valid input checks
    int rows = field.size();
    if (rows == 0) {
        throw std::invalid_argument("Empty field.");
    }
    
    int cols = 0;
    int prev_cols = field[0].size();
    for (int i=0; i < rows; i++)
    {
        cols = field[i].size();
        if (cols != prev_cols  || cols == 0)
        {
            throw std::invalid_argument("Rows not same length/# of columns.");
        }
        prev_cols = cols;

        for (int j=0; j < cols; j++)
        {
            if (field[i][j] != '.' && field[i][j] != 'X')
            {
                throw std::invalid_argument("Invalid character.");
            }
        }
    }
    cols = field[0].size();

    // Implementation of pseudocode
    if (field[0][0] == 'X')
    {
        return 0;
    }
    int answer[rows][cols];
    for (int i=0; i < rows; i++)
    {
        for (int j=0; j < cols; j++)
        {
            answer[i][j] = 0;
        }
    }
    answer[0][0] = 1;
    int above = 0;
    int left = 0;
    for (int i=0; i < rows; i++)
    {
        for (int j=0; j < cols; j++)
        {
            if (field[i][j] == 'X')
            {
                answer[i][j] = 0;
                continue;
            }
            above = 0;
            left = 0;
            if (i > 0 && field[i-1][j] == '.')
            {
                above = answer[i-1][j];
            }
            if (j > 0 && field[i][j-1] == '.')
            {
                left = answer[i][j-1];
            }
            if (i != 0 || j != 0)
            {
                answer[i][j] = above + left;
            }
            
        }
    }

    return answer[rows-1][cols-1];
}
