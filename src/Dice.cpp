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

/**
  * Construct a Dice.
  *
  * Create an array / pointer to the number of values that the dice may
  * generate; construct 2 Dice with six faces.
  *
  */

Dice::Dice() : m_cDie1(6), m_cDie2(6)
{
    m_pnDiceRollValues  = new int[2 * 6]();
}

/**
  * Construct a Dice.
  *
  * Create an array / pointer to the number of values that the dice may
  * generate; construct the number of Die specified; populate the number
  * arrays.
  *
  * \param nDice Specifices the number of dice
  * \param nFaces Specifices the number of faces of the die
  */
/*
Dice::Dice(int nDice, int nFaces)
{
    // TODO: Check nDice and nfaces values

    m_pnDiceRollValues  = new int[nDice * nFaces]();

    for (int iii = 0; iii < nDice; iii++)
    {
        m_lDice.emplace_back(nFaces);
    }
}
*/

/**
  * Copy constructor for Dice.
  *
  * \param cSource The source of the copy
  */

Dice::Dice(const Dice& cSource) : m_cDie1(cSource.m_cDie1), m_cDie2(cSource.m_cDie2)
{
    m_nTotalDiceRolls         = cSource.m_nTotalDiceRolls;
    m_nRollValue              = cSource.m_nRollValue;

    if (cSource.m_pnDiceRollValues)
    {
        m_pnDiceRollValues = new int[2 * 6];
        for (std::list<Die>::size_type iii = 0; iii < 2 * 6; iii++)
        {
            m_pnDiceRollValues[iii] = cSource.m_pnDiceRollValues[iii];
        }
    }
    else
    {
        m_pnDiceRollValues = 0;
    }
}
/*
Dice::Dice(const Dice& cSource) : m_lDice(cSource.m_lDice)
{
    m_nTotalDiceRolls         = cSource.m_nTotalDiceRolls;
    m_nRollValue              = cSource.m_nRollValue;

    if (cSource.m_pnDiceRollValues)
    {
        m_pnDiceRollValues = new int[m_lDice.size() * m_lDice.front().Faces()];
        for (std::list<Die>::size_type iii = 0; iii < m_lDice.size() * m_lDice.front().Faces(); iii++)
        {
            m_pnDiceRollValues[iii] = cSource.m_pnDiceRollValues[iii];
        }
    }
    else
    {
        m_pnDiceRollValues = 0;
    }
}
*/
/**
  * Copy assignment constructor for Dice.
  *
  * \param cSource The source of the copy assignment
  */

Dice& Dice::operator=(const Dice& cSource)
{
    if (this == &cSource)
        return (*this);

    m_cDie1 = cSource.m_cDie1;
    m_cDie2 = cSource.m_cDie2;

    m_nTotalDiceRolls         = cSource.m_nTotalDiceRolls;
    m_nRollValue              = cSource.m_nRollValue;

    delete[] m_pnDiceRollValues;

    if (cSource.m_pnDiceRollValues)
    {
        m_pnDiceRollValues = new int[2 * 6];
        for (std::list<Die>::size_type iii = 0; iii < 2 * 6; iii++)
        {
            m_pnDiceRollValues[iii] = cSource.m_pnDiceRollValues[iii];
        }
    }
    else
    {
        m_pnDiceRollValues = 0;
    }

    return (*this);
}
/*
Dice& Dice::operator=(const Dice& cSource)
{
    if (this == &cSource)
        return (*this);

    m_lDice = cSource.m_lDice;

    m_nTotalDiceRolls         = cSource.m_nTotalDiceRolls;
    m_nRollValue              = cSource.m_nRollValue;

    delete[] m_pnDiceRollValues;

    if (cSource.m_pnDiceRollValues)
    {
        m_pnDiceRollValues = new int[m_lDice.size() * m_lDice.front().Faces()];
        for (std::list<Die>::size_type iii = 0; iii < m_lDice.size() * m_lDice.front().Faces(); iii++)
        {
            m_pnDiceRollValues[iii] = cSource.m_pnDiceRollValues[iii];
        }
    }
    else
    {
        m_pnDiceRollValues = 0;
    }

    return (*this);
}
*/

/**
  * Destructor for Die.
  */

Dice::~Dice()
{
    delete[] m_pnDiceRollValues;
}

/**
  * Roll the Dice.
  *
  * Roll each Die and capture tha value; record number of rolls and
  * other Dice statistics.
  *
  *\return The value of the rolled Dice.
  */

int Dice::Roll()
{
    // Roll the dice
    m_nRollValue = m_cDie1.Roll() + m_cDie2.Roll();

    // Increment the dice value count
    m_pnDiceRollValues[m_nRollValue - 1]++;

    // Increment the total number of throws
    m_nTotalDiceRolls++;

    return(m_nRollValue);
}

/*
int Dice::Roll()
{
    m_nRollValue = 0;

    // Roll the dice
    std::list<Die>::iterator it;
    it = m_lDice.begin();
    while (it != m_lDice.end())
    {
        // Record the roll value
        m_nRollValue += it->Roll();
        it++;
    }

    // Increment the dice value count
    m_pnDiceRollValues[m_nRollValue - 1]++;

    // Increment the total number of throws
    m_nTotalDiceRolls++;

    return(m_nRollValue);
}
*/

/**
  * Check for a hard way.
  *
  * Check last values of the dice and return true if they are equal to
  * each other.
  *
  *\return True if Die values are equal, false oherwise
  */

/*
bool Dice::IsHard() const
{
    bool bHard = false;

    const Die &cFirstDie = m_lDice.front();
    int nFirstValue = cFirstDie.RollValue();

    const Die &cSecondDie = m_lDice.back();
    if (nFirstValue == cSecondDie.RollValue())
        bHard = true;

    return(bHard);
}
*/
