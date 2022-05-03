#include "PuzzleSolver.h"

int PuzzleSolver::getOpositeClue(int clue) // returns clue position from another side
{
    if(clue < 4)
    {
        return (11 - clue);
    }
    else
    {
        return (19 - clue);
    }
}

void PuzzleSolver::fillPossibleCombinations(std::vector<std::array<int, 4>> &possibleCombinations)
{
    for(int a{1}; a < 5; ++a)
    {
        for(int b{1}; b < 5; ++b)
        {
            if(a != b)
            {
                for(int c{1}; c < 5; ++c)
                {
                    if(c != b && c != a)
                    {
                        for(int d{1}; d < 5; ++d)
                        {
                            if(d != c && d != b && d != a)
                            {
                                possibleCombinations.push_back( {a,b,c,d} );
                            }
                        }
                    }
                }
            }
        }
    }
}

std::vector<std::array<int, 2>> PuzzleSolver::calculateViewedFromSides(const std::vector<std::array<int, 4>> &possibleCombinations)
{
    std::vector<std::array<int, 2>> viewedFromSides{};

    auto calculateViewedFromSide
    {
        [](const std::array<int, 4> &comb) -> int
        {
            int viewedFromSide = 1;

            int height = comb[0];

            for(int i{0}; i < 3; ++i)
            {
                if(comb[i+1] > height)
                {
                    viewedFromSide++;
                    height = comb[i+1];
                }
            }

            return viewedFromSide;
        }
    };

    for(const auto &it: possibleCombinations)
    {
        auto combination = it;

        int viewedFromLeft { calculateViewedFromSide(combination) };

        std::reverse(combination.begin(), combination.end()); // reverse combination to calculate from another side

        int viewedFromRight{ calculateViewedFromSide(combination) };

        viewedFromSides.push_back( {viewedFromLeft, viewedFromRight} );
    }

    return viewedFromSides;
}

void PuzzleSolver::fillCombinationsThatCanWork()
{
    std::vector<std::array<int, 4>> possibleCombinations{}; // will contain all possible combinations

    fillPossibleCombinations(possibleCombinations);

    const auto &viewedFromSides{ calculateViewedFromSides(possibleCombinations) }; // will contain all info about how many building can be viewed from both sides

    for(int i{0}; i < 8; ++i)
    {
        const auto &opClue{ getOpositeClue(i) };
        if(m_clues[i] != 0 || m_clues[opClue] != 0)
        {
            for(int it{0}; it < static_cast<int>(viewedFromSides.size()); ++it)
            {
                auto tempViewedFromSides{ viewedFromSides[it] };
                if(i >= 4)
                {
                    std::reverse(tempViewedFromSides.begin(), tempViewedFromSides.end()); // reverses row clues to to make work with combinations easier
                }

                if((m_clues[i] == 0 || m_clues[i] == tempViewedFromSides[0])
                   && (m_clues[opClue] == 0 || m_clues[opClue] == tempViewedFromSides[1]))
                {
                    m_combinationsThatCanWork[i].push_back(possibleCombinations[it]);
                }
            }
        }
    }
}

bool PuzzleSolver::checkFieldCorrectness()
{
    auto isAllUnique // checks if all elements in array is unique
    {
        [](std::array<int,4> arr) -> bool
        {
            std::sort(arr.begin(), arr.end());

            for(int i{0}; i < 3; ++i)
            {
                if(arr[i] == arr[i+1] && arr[i] != 0)
                {
                    return false;
                }
            }
            return true;
        }
    };

    for(int i{0}; i < 4; ++i)
    {
        if(!(isAllUnique(m_field[i]))) // checks if all rows lines are unique
        {
            return false;
        }
    }

    for(int i{0}; i < 4; ++i)
    {
        if(!(isAllUnique( { m_field[0][i], m_field[1][i], m_field[2][i], m_field[3][i] } ))) // checks if all columns are unique
        {
            return false;
        }
    }

    return true;
}

void PuzzleSolver::tryToPickUpOthers(int &row, int &column, bool &isFound) // tries to fill blank elements
{
    if(row == 4)
    {
        row = 0;
        column++;
    }

    if(column == 4)
    {
        isFound = true;
        return;
    }

    auto lowerTheRow
    {
        [&row, &column]()
        {
            if(row == 0)
            {
                row = 4;
                column--;
            }
            else
            {
                --row;
            }
        }
    };

    if(m_field[row][column] == 0)
    {
        std::array<int, 5> possibleList{ 0,1,2,3,4 };

        for(int i{0}; i < 4; ++i)
        {
            possibleList[m_field[row][i]] = 0;
            possibleList[m_field[i][column]] = 0;
        }

        for(int i{1}; i < 5; ++i)
        {
            if(possibleList[i] != 0)
            {
                m_field[row][column] = possibleList[i];

                tryToPickUpOthers(++row, column, isFound);

                if(isFound)
                {
                    break;
                }

                lowerTheRow();

                m_field[row][column] = 0;
            }
        }
    }
    else
    {
        tryToPickUpOthers(++row, column, isFound);
        lowerTheRow();
    }
}

bool PuzzleSolver::tryToPickUpOthers() // creates a base to fill blank elements
{
    int row{0};
    int column{0};
    bool isFound = false;

    tryToPickUpOthers(row, column, isFound);

    return isFound;
}

bool PuzzleSolver::needsToPickUpOthers() //checks if any blank elements left on field
{
    for(int i{0}; i < 4; ++i)
    {
        for(int j{0}; j < 4; ++j)
        {
            if(m_field[i][j] == 0)
            {
                return true;
            }
        }
    }
    return false;
}

void PuzzleSolver::tryToFinalize(bool &isFound) // checks if all applied combinations are correct and tries to fill blank elements if there are any
{
    if(checkFieldCorrectness())
    {
        if(needsToPickUpOthers())
        {
            isFound = tryToPickUpOthers();
        }
        else
        {
            isFound = true;
        }
    }
}

bool PuzzleSolver::putCombinationsOnField(const int &wcIt, const std::array<int,4> &wc, bool &isFound)
// puts working combinations on field and checks if there are any conflicts or end is reached
{
    if(wcIt < 4)
    {
        for(int i{0}; i < 4; ++i)
        {
            m_field[i][wcIt] = wc[i]; // puts columns combinations on field
        }

        return true;
    }
    else
    {
        const int curr = (wcIt - 4);

        for(int i{0}; i < 4; ++i)
        {
            if(m_field[curr][i] != 0 && m_field[curr][i] != wc[i]) // checks if there are any conflicts
            {
                return false;
            }
        }

        for(int i{0}; i < 4; ++i)
        {
            m_field[curr][i] = wc[i]; // puts rows combinations on field
        }

        if(wcIt == 7) // if end is reached tries to finalize
        {
            tryToFinalize(isFound);
            return false;
        }

        return true;
    }
}

void PuzzleSolver::fillFieldWithCombinations(int &wcIt,  bool &isFound)
{
    if(m_combinationsThatCanWork[wcIt].size() > 0)
    {
        for(const auto &it: m_combinationsThatCanWork[wcIt])
        {
            if(!isFound)
            {
                const auto previousState = m_field; // save current state of field

                bool neddsToContinue{ putCombinationsOnField(wcIt, it, isFound) }; // puts working combination and decides if loop needs to continue

                if(neddsToContinue)
                {
                    fillFieldWithCombinations(++wcIt, isFound);
                    --wcIt;
                }

                if(!isFound) // if fails to find right combination reverses to previous state
                {
                    m_field = previousState;
                }
            }
            else
            {
                break;
            }
        }
    }
    else
    {
        if(wcIt < 7)
        {
            fillFieldWithCombinations(++wcIt, isFound);
            --wcIt;
        }
        else // if end is reached tries to finalize
        {
            tryToFinalize(isFound);
        }
    }
}

void PuzzleSolver::fillFieldWithCombinations()
{
    int wcIt = 0; // combinations that can work iterator
    bool isFound = false;
    fillFieldWithCombinations(wcIt, isFound);
}
