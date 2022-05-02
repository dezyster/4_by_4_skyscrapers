#pragma once

#include <vector>
#include <array>
#include <algorithm>

class PuzzleSolver
{
    std::array<std::array<int, 4>, 4> m_field{};

    std::array<int, 16> m_clues{};

    std::array<std::vector<std::array<int,4>>, 8> m_combinationsThatCanWork{};

    PuzzleSolver(std::array<int, 16> clues): m_clues{ clues }{}

    int getOpositeClue(int);

    void fillPossibleCombinations(std::vector<std::array<int, 4>>&);
    std::vector<std::array<int, 2>> calculateViewedFromSides(const std::vector<std::array<int, 4>>&);

    void fillCombinationsThatCanWork();

    bool checkFieldCorrectness();

    void tryToPickUpOthers(int&, int&, bool&);
    bool tryToPickUpOthers();
    bool needsToPickUpOthers();

    void tryToFinalize(bool&);
    bool putCombinationsOnField(const int&, const std::array<int, 4>&, bool&);
    void fillFieldWithCombinations(int&, bool&);
    void fillFieldWithCombinations();

public:

    static std::array<std::array<int,4>,4> SolvePuzzle(std::array<int, 16> clues)
    {
        PuzzleSolver puzzle(clues);

        puzzle.fillCombinationsThatCanWork();

        puzzle.fillFieldWithCombinations();

        return puzzle.m_field;
    }
};
