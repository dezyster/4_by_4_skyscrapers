#include <iostream>
#include <cstring>

#include "PuzzleSolver.h"

// https://www.codewars.com/kata/5671d975d81d6c1c87000022

int** SolvePuzzle (int *clues) // transaction layer for codewars site
{
    std::array<int, 16> arr{};

    for(int i{0}; i < 16; ++i)
    {
        arr[i] = clues[i];
    }

    auto result{ PuzzleSolver::SolvePuzzle(arr) };

    int **answer = new int*[4];
    for(int i{0}; i < 4; ++i)
    {
        answer[i] = new int[4];
    }

    for(int i{0}; i < 4; i++)
    {
        for(int j{0}; j< 4; j++)
        {
            answer[i][j] = result[i][j];
        }
    }

    return answer;
}

static int clues[][16] = {
{ 2, 2, 1, 3,
  2, 2, 3, 1,
  1, 2, 2, 3,
  3, 2, 1, 3 },
{ 0, 0, 1, 2,
  0, 2, 0, 0,
  0, 3, 0, 0,
  0, 1, 0, 0 },
};

int outcomes[][4][4] = {
{ { 1, 3, 4, 2 },
  { 4, 2, 1, 3 },
  { 3, 4, 2, 1 },
  { 2, 1, 3, 4 } },
{ { 2, 1, 4, 3 },
  { 3, 4, 1, 2 },
  { 4, 2, 3, 1 },
  { 1, 3, 2, 4 } },
};

bool equal(int **puzzle, int expected[4][4])
{
    if (!puzzle || !expected)
    {
        return 0;
    }

    for (int i{0}; i < 4; ++i) if (memcmp (puzzle[i], expected[i], 4*sizeof (int)))
    {
        return false;
    }

    return true;
}

int main()
{
    std::cout << std::boolalpha << equal (SolvePuzzle (clues[0]), outcomes[0]) << std::endl;
    std::cout << std::boolalpha << equal (SolvePuzzle (clues[0]), outcomes[0]);

    return 0;
}
