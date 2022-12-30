#include <iostream>
#include "minesweeper.h"
#include "minesweeper_lib.h"

char *createBoard(std::size_t xdim, std::size_t ydim)
{
    // create a char array with xdim and ydim elements and intialize each element with 0 (the value/integer)
    // the data type of the array is char - not because the values are characters, but because chars are 8 bits
    char *final{new char[xdim * ydim]{}};
    for (std::size_t i{0}; i < (ydim * xdim); i++)
    {
        final[i] = 0;
    }

    return final;
}

void cleanBoard(char *board)
{
    // deallocate the given board
    delete[] board;
    board = nullptr;
}

void hideBoard(char *board, std::size_t xdim, std::size_t ydim)
{
    // go through board,
    // for each iteration, find the integer that its referencing using the asterisk, and then do bit shift on that using hiddenbit
    for (std::size_t i{0}; i < (ydim * xdim); i++)
    {
        board[i] |= hiddenBit();
    }
}

void printBoard(char *board, std::size_t xdim, std::size_t ydim)
{
    // int m = 0;
    for (int i{0}; i < (xdim * ydim); i += xdim)
    {
        // std::cout << "i = " << i << " " << std::endl;
        for (int m = i; m < (xdim + i); m++)
        {
            // if its marked, use M
            // std::cout << "m = " << m << " " << std::endl;
            // std::cout << "i+m = " << (i+m) << " " << std::endl;

            if ((board[m] & markedBit()) != 0)
            {
                // std::cout << "element" << (i+m) << "is marked" << std::endl;
                std::cout << "M";
            }

            // if its hidden, use *
            else if ((board[m] & hiddenBit()) != 0)
            {
                std::cout << "*";
            }

            else
            {
                int temp = (board[m] & valueMask());
                std::cout << temp;
            }
        }
        std::cout << std::endl;
    }

    // if its marked, use M
    // otherwise, use the last four binary digits to come up with a number from 0-9
}

int mark(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc)
{

    int loc = (yloc * xdim) + xloc;
    // if the hidden bit is 1/if this is hidden:
    if ((board[loc] & hiddenBit()) != 0)
    {
        // toggle the marked bit between 1 and 0
        board[loc] ^= markedBit();
        // std::cout << "true" << std::endl;
        return 0;
    }

    // if its not hidden,
    else
    {
        return 2;
    }
}

void computeNeighbors(char *board, std::size_t xdim, std::size_t ydim)
{
    // duplicate the board array
    // board is an array of just 0s and 9s
    // update the board array so that all fields with a 0 are replaced with the number of adjacent geeese
    // check if the north, northwest, northeast, west, east, southwest, southeast is equal to 9
    // increase the board index by 1

    // create a duplicate of the board array (this will be used to check the neighbors of every cell)
    // check the neighnors of the cell on the copy of the array and update the actual board array accordingly

    int size = (xdim + 2) * (ydim + 2);
    int copy[size];

    // creates a row of 1s at the top
    int counter{0};
    for (std::size_t j{0}; j < (xdim + 2); j++)
    {
        copy[j] = 1;
        counter += 1;
    }

    // iterate through each row of the grid
    for (std::size_t k{0}; k < ydim; k++)
    {
        // start off each row with a 1
        copy[counter] = 1;
        counter += 1;

        // copy the grid from board
        for (std::size_t n{0}; n < xdim; n++)
        {
            int location = n + (k * xdim);
            int temp = (board[location] & valueMask());
            copy[counter] = temp;
            counter += 1;
        }

        // finish off the row with a 1
        copy[counter] = 1;
        counter += 1;
    }

    // last row of 1s
    for (std::size_t j{0}; j < (xdim + 2); j++)
    {
        copy[counter] = 1;
        counter += 1;
    }

    // loop through the copy array
    // if its a 1, skip it, if its a 9 skip it as well
    // if its a 0, check the numbers around it to see if any of them is a 9
    // when checking and adding 1 to neighbours, add one to the counter for the board array

    int boardCount{0};
    for (std::size_t copyloc{0}; copyloc < size; copyloc++)
    {
        if (copy[copyloc] == 9)
        {
            boardCount++;
        }

        else if (copy[copyloc] == 0)
        {
            int neighbours{0};
            // south
            if (copy[copyloc + (xdim + 2)] == 9)
            {
                neighbours++;
            }

            // southwest
            if (copy[(copyloc + (xdim + 2)) - 1] == 9)
            {
                neighbours++;
            }

            // southeast
            if (copy[(copyloc + (xdim + 2)) + 1] == 9)
            {
                neighbours++;
            }

            // north
            if (copy[copyloc - (xdim + 2)] == 9)
            {
                neighbours++;
            }

            // northwest
            if (copy[(copyloc - (xdim + 2)) - 1] == 9)
            {
                neighbours++;
            }

            // northeast
            if (copy[(copyloc - (xdim + 2)) + 1] == 9)
            {
                neighbours++;
            }

            // east
            if (copy[copyloc + 1] == 9)
            {
                neighbours++;
            }

            // west
            if (copy[copyloc - 1] == 9)
            {
                neighbours++;
            }

            int newValueMask = ~valueMask();
            int boardCell = (board[boardCount] & newValueMask);
            int temp = (neighbours | boardCell);
            board[boardCount] = temp;
            // std::cout << "temp: " << temp << "; neighbours: " << neighbours << std::endl;
            // std::cout << "Count:" << boardCount << "--" << neighbours << std::endl;
            boardCount++;
        }
    }
}

int reveal(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc)
{
    int copyloc;
    int loc = (yloc * xdim) + xloc;

    // if the cell is marked, return 1
    if ((board[loc] & markedBit()) != 0)
    {
        return 1;
    }

    // if the cell is revealed, return 2
    else if ((board[loc] & hiddenBit()) == 0)
    {
        return 2;
    }

    // if there is a goose in the cell
    else if ((board[loc] & valueMask()) == 9)
    {
        // reveal the field
        board[loc] &= (~hiddenBit());
        // return 9
        return 9;
    }

    // for all other cases
    else
    {

        board[loc] &= (~hiddenBit());

        //for each neighbour, check if they are marked or not

        // if its a 0, revela off its neighbours
        if ((board[loc] & valueMask()) == 0)
        {
            // check if the bit is a 0 before revelaing all of its neighbours
            int index{0};
            ////////////////////CREATE COPY OF ARRAY////////////////////
            int size = (xdim + 2) * (ydim + 2);
            int copy[size];

            // creates a row of 11s at the top
            int counter{0};
            for (std::size_t j{0}; j < (xdim + 2); j++)
            {
                copy[j] = 11;
                counter += 1;
            }

            // iterate through each row of the grid
            for (std::size_t k{0}; k < ydim; k++)
            {
                // start off each row with a 11
                copy[counter] = 11;
                counter += 1;

                // copy the grid from board
                for (std::size_t n{0}; n < xdim; n++)
                {
                    int location = n + (k * xdim);
                    int given = xloc + (yloc * xdim);
                    // if the loop is at the array that was specified, store it in the index variable
                    if (given == location)
                    {
                        index = counter;
                    }
                    int temp = (board[location] & valueMask());
                    copy[counter] = temp;
                    counter += 1;
                }

                // finish off the row with a 11
                copy[counter] = 11;
                counter += 1;
            }

            // last row of 11s
            for (std::size_t j{0}; j < (xdim + 2); j++)
            {
                copy[counter] = 11;
                counter += 1;
            }
            ////////////////////CREATE COPY OF ARRAY////////////////////

            // before revelaing the ones around it, check if they are marked or hidden
            // index is equal to the location of the index that we are looking for, but in the copy array
            //  south
            if ((copy[index + (xdim + 2)] != 9) && (copy[index + (xdim + 2)] != 11))
            {
                board[loc + xdim] &= (~hiddenBit());
            }

            // southwest
            if ((copy[(index + (xdim + 2)) - 1] != 9) && (copy[(index + (xdim + 2)) - 1] != 11))
            {
                board[loc + xdim - 1] &= (~hiddenBit());
            }

            // southeast
            if ((copy[(index + (xdim + 2)) + 1] != 9) && (copy[(index + (xdim + 2)) + 1] != 11))
            {
                board[loc + xdim + 1] &= (~hiddenBit());
            }

            // north
            if ((copy[index - (xdim + 2)] != 9) && (copy[index - (xdim + 2)] != 11))
            {
                board[loc - xdim] &= (~hiddenBit());
            }

            // northwest
            if ((copy[(index - (xdim + 2)) - 1] != 9) && (copy[(index - (xdim + 2)) - 1] != 11))
            {
                board[loc - xdim - 1] &= (~hiddenBit());
            }

            // northeast
            if ((copy[(index - (xdim + 2)) + 1] != 9) && (copy[(index - (xdim + 2)) + 1] != 11))
            {
                board[loc - xdim + 1] &= (~hiddenBit());
            }

            // east
            if ((copy[index + 1] != 9) && (copy[index + 1] != 11))
            {
                board[loc + 1] &= (~hiddenBit());
            }

            // west
            if ((copy[index - 1] != 9) && (copy[index - 1] != 11))
            {
                board[loc - 1] &= (~hiddenBit());
            }
        }
        return 0;
    }
}

bool isGameWon(char *board, std::size_t xdim, std::size_t ydim)
{

    // iterate through all of the fields and count the number of geese
    int mines{0};
    for (int i{0}; i < (xdim * ydim); i++)
    {
        if ((board[i] & valueMask()) == 9)
        {
            mines++;
        }
    }

    // if the num of geese are equal to the number of cells in the board return true
    if (mines == (xdim * ydim))
    {
        return true;
    }

    // if not,
    else
    {
        // the leftover cells are the cells on the board minus the number of geese
        int leftover = (xdim * ydim) - mines;

        // go thorugh every cell
        int counter{0};
        for (int j{0}; j < (xdim * ydim); j++)
        {
            // if its revealed and its not 9,
            if (((board[j] & hiddenBit()) == 0) && ((board[j] & valueMask()) != 9))
            {
                // increase the counter
                counter++;
            }
        }

        // counter of how many are revealed and not 9 should be equal to the leftover for the player to have won
        if (counter == leftover)
        {
            return true;
        }

        // else, return false
        else
        {
            return false;
        }
    }
}