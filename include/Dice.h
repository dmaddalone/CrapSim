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
        Dice(int nDice, int nFaces);
        ~Dice();

        int   Roll();

        int   RollValue() const                { return (m_nLastDiceRollValue); }
        bool  IsCraps() const                  { return ((m_nLastDiceRollValue == 2) || (m_nLastDiceRollValue == 3) || (m_nLastDiceRollValue == 12)); }
        bool  IsNatural() const                { return ((m_nLastDiceRollValue == 7) || (m_nLastDiceRollValue == 11)); }
        bool  IsSix() const                    { return (m_nLastDiceRollValue == 6); }
        bool  IsSeven() const                  { return (m_nLastDiceRollValue == 7); }
        bool  IsEight() const                  { return (m_nLastDiceRollValue == 8); }
        bool  IsEleven() const                 { return (m_nLastDiceRollValue == 11); }
        bool  IsBar() const                    { return (m_nLastDiceRollValue == 12); }
        bool  IsField() const                  { return (std::count(m_anFieldNumbers.begin(), m_anFieldNumbers.end(), m_nLastDiceRollValue)) ? true : false; }
        bool  IsAPointNumber() const           { return (std::count(m_anPointNumbers.begin(), m_anPointNumbers.end(), m_nLastDiceRollValue)) ? true : false; }
        int   NumberOfNonFieldInARow() const   { return (m_nNumberOfNonFieldInARow); }

        int   RollValueCount(int i) const      { return (m_pnDiceRollValues[i - 1]); }
        int   TotalRolls() const               { return (m_nTotalDiceRolls); }
        float RollValuePercentage(int i) const { return ((float)RollValueCount(i) / TotalRolls() * 100); }

    private:
        std::list<Die> m_lDice;
        int  m_nTotalDiceRolls       = 0;
        int  m_nLastDiceRollValue    = 0;
        int  *m_pnDiceRollValues;

        int m_nNumberOfNonFieldInARow = 0;

        std::array<int, 7> m_anFieldNumbers;  // Synchronize field numbers with Bet.cpp CalculatePayoff()
        std::array<int, 6> m_anPointNumbers;
};

#endif // DICE_H
