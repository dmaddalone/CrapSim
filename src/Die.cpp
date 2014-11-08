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

#include "Die.h"

std::random_device   rdev{};
std::mt19937         generator{rdev()};
std::function<int()> rollDie;

Die::Die(int nFaces)
{
    std::uniform_int_distribution<int>   distribution(1, nFaces);
    rollDie         = std::bind(distribution, std::ref(generator));
    m_nFaces = nFaces;
	m_pnDieRollValues  = new int[m_nFaces]();
}

Die::Die(const Die& cSource)
{
        m_nTotalDieRolls    = cSource.m_nTotalDieRolls;
        m_nLastDieRollValue = cSource.m_nLastDieRollValue;
        m_nFaces            = cSource.m_nFaces;

        if (cSource.m_pnDieRollValues)
        {
            m_pnDieRollValues = new int[m_nFaces];
            for (int iii = 0; iii < m_nFaces; iii++)
            {
                m_pnDieRollValues[iii] = cSource.m_pnDieRollValues[iii];
            }
        }
        else
        {
            m_pnDieRollValues = 0;
        }
}

Die& Die::operator=(const Die& cSource)
{
    if (this == &cSource)
        return (*this);

    m_nTotalDieRolls    = cSource.m_nTotalDieRolls;
    m_nLastDieRollValue = cSource.m_nLastDieRollValue;
    m_nFaces            = cSource.m_nFaces;

    delete[] m_pnDieRollValues;

    if (cSource.m_pnDieRollValues)
        {
            m_pnDieRollValues = new int[m_nFaces];
            for (int iii = 0; iii < m_nFaces; iii++)
            {
                m_pnDieRollValues[iii] = cSource.m_pnDieRollValues[iii];
            }
        }
        else
        {
            m_pnDieRollValues = 0;
        }

    return (*this);
}

Die::~Die()
{
	delete[] m_pnDieRollValues;
}

int Die::Roll()
{
    m_nLastDieRollValue = rollDie();
    m_pnDieRollValues[m_nLastDieRollValue - 1]++;
    m_nTotalDieRolls++;
    return(m_nLastDieRollValue);
}




