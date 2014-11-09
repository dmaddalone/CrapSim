/*
    Copyright 2014 Dom Maddalone

    This file is part of CrapSim.

    CrapSim is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CrapSim is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with CrapSim.  If not, see <http://www.gnu.org/licenses/>.
*/

/** \file
 *
 * \brief The Dice class represents a two or more dice (in this case, only two)
 * built upon the Die class.  This class has been designed to be Craps
 * knowledgeable.
 *
 */

#ifndef DICE_H
#define DICE_H

#include <algorithm>
#include <array>
#include <list>
#include "Die.h"

class Dice
{
    public:
        // Create nDice number of die with nFaces number of faces
        Dice(int nDice, int nFaces);
        // Copy constructor
        Dice(const Dice& cSource);
        // Assignment constructor
        Dice& operator=(const Dice& cSource);
        // Destructor
        ~Dice();

        // Roll the dice
        int   Roll();

        // Return last roll value
        int   RollValue() const                { return (m_nLastDiceRollValue); }
        // Set of Craps specific dice value checks
        bool  IsCraps() const                  { return ((m_nLastDiceRollValue == 2) || (m_nLastDiceRollValue == 3) || (m_nLastDiceRollValue == 12)); }
        bool  IsNatural() const                { return ((m_nLastDiceRollValue == 7) || (m_nLastDiceRollValue == 11)); }
        bool  IsSix() const                    { return (m_nLastDiceRollValue == 6); }
        bool  IsSeven() const                  { return (m_nLastDiceRollValue == 7); }
        bool  IsEight() const                  { return (m_nLastDiceRollValue == 8); }
        bool  IsEleven() const                 { return (m_nLastDiceRollValue == 11); }
        bool  IsBar() const                    { return (m_nLastDiceRollValue == 12); }
        bool  IsField() const                  { return (std::count(m_anFieldNumbers.begin(), m_anFieldNumbers.end(), m_nLastDiceRollValue)) ? true : false; }
        bool  IsAPointNumber() const           { return (std::count(m_anPointNumbers.begin(), m_anPointNumbers.end(), m_nLastDiceRollValue)) ? true : false; }
        // Return the number of rolls in a row that are not Field numbers
        int   NumberOfNonFieldInARow() const   { return (m_nNumberOfNonFieldInARow); }
        // Return the number times a value has been rolled
        int   RollValueCount(int i) const      { return (m_pnDiceRollValues[i - 1]); }
        // Return the total number of rolls
        int   TotalRolls() const               { return (m_nTotalDiceRolls); }
        // Return the percentage of times a value is rolled
        float RollValuePercentage(int i) const { return ((float)RollValueCount(i) / TotalRolls() * 100); }

    private:
        // Container of die
        std::list<Die> m_lDice;
        // Set counters to zero
        int  m_nTotalDiceRolls       = 0;
        int  m_nLastDiceRollValue    = 0;
        int m_nNumberOfNonFieldInARow = 0;

        // Pointer / array to roll values
        int  *m_pnDiceRollValues;

        // Array to hold field numbers
        std::array<int, 7> m_anFieldNumbers {{2, 3, 4, 9, 10, 11, 12}};  // Synchronize field numbers with Bet.cpp CalculatePayoff()
        // Array to hold point numbers
        std::array<int, 6> m_anPointNumbers {{4, 5, 6, 8, 9, 10}};
};

#endif // DICE_H
