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
 * \brief The Die class represents a single die that rolls a uniform
 * distribution of integers between 1 and the number of faces defined.
 *
 */

#ifndef DIE_H
#define DIE_H

#include <random>
#include <functional>
#include <algorithm>
#include <iterator>

class Die
{
    public:
        explicit Die(int i);
        Die(const Die& cSource);
        Die& operator=(const Die& cSource);
        ~Die();

        int Roll();

        int   RollValueCount(int i) const      { return m_pnDieRollValues[i-1];}
        int   TotalRolls() const               { return m_nTotalDieRolls; }
        float RollValuePercentage(int i) const { return (float)RollValueCount(i) / TotalRolls() * 100; }

    private:
        int m_nTotalDieRolls    = 0;
        int m_nLastDieRollValue = 0;
        int m_nFaces            = 0;
        int *m_pnDieRollValues;
};

#endif // DIE_H
