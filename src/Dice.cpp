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

#include <iostream>

#include "Dice.h"

Dice::Dice(int nDice, int nFaces)
{
    m_pnDiceRollValues  = new int[nDice * nFaces]();

    for (int iii = 0; iii < nDice; iii++)
    {
        m_lDice.push_back(Die(nFaces));
    }

    m_anFieldNumbers = {2, 3, 4, 9, 10, 11, 12};  // Synchronize field numbers with Bet.cpp CalculatePayoff()
    m_anPointNumbers = {4, 5, 6, 8, 9, 10};
}

Dice::~Dice()
{
    delete[] m_pnDiceRollValues;
}

int Dice::Roll()
{
    m_nLastDiceRollValue = 0;

    // Roll the dice
    std::list<Die>::iterator it;
    it = m_lDice.begin();
    while (it != m_lDice.end())
    {
        // Record the roll value
        m_nLastDiceRollValue += it->Roll();
        it++;
    }

    // Increment the dice value count
    m_pnDiceRollValues[m_nLastDiceRollValue - 1]++;

    // Increment the total number of throws
    m_nTotalDiceRolls++;

    // Record number of non Field numbers thrown in a row
    if (!IsField())
    {
        m_nNumberOfNonFieldInARow++;
    }
    else
    {
        m_nNumberOfNonFieldInARow = 0;
    }

    return(m_nLastDiceRollValue);
}




