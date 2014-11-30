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

#include "Table.h"


/**
  * Construct a Table.
  *
  * Set minimum and maximum wagers and sets up a map for Table Odds.
  *
  */

Table::Table(int nMin, int nMax)
{
    nMin > 0 ? m_nMinimumBet = nMin : m_nMinimumBet = 5;
    nMax > 0 ? m_nMaximumBet = nMax : m_nMaximumBet = 5000;

    m_mTableOdds["1X"]          = TableOdds::TYPE_1X;
    m_mTableOdds["2X"]          = TableOdds::TYPE_2X;
    m_mTableOdds["FULL_DOUBLE"] = TableOdds::TYPE_FULL_DOUBLE;
    m_mTableOdds["3X"]          = TableOdds::TYPE_3X;
    m_mTableOdds["3X_4X_5X"]    = TableOdds::TYPE_3X_4X_5X;
    m_mTableOdds["5X"]          = TableOdds::TYPE_5X;
    m_mTableOdds["10X"]         = TableOdds::TYPE_10X;
    m_mTableOdds["20X"]         = TableOdds::TYPE_20X;
    m_mTableOdds["100X"]        = TableOdds::TYPE_100X;
}

/**
  * Return the maximum odds offered by the Table.
  *
  * Based on the table odds and the point, return maximum odds offered.
  *
  * \param nPoint The point is used on some table odds.
  *
  * \return Maximum odds offered.  Default is 1.0.
  */

float Table::MaxOdds(int nPoint) const
{
    if ((nPoint != 4) && (nPoint != 5) && (nPoint != 6) && (nPoint != 8) && (nPoint != 9) && (nPoint !=10))
        throw CrapSimException("Table::MaxOdds unknown point", std::to_string(Point()));

    switch (m_ecTableOdds)
    {
        case TableOdds::TYPE_1X:
            return (1.0);
            break;
        case TableOdds::TYPE_2X:
            return (2.0);
            break;
        case TableOdds::TYPE_FULL_DOUBLE:
            if ((nPoint == 6) || (nPoint == 8))
                return (2.5);
            else
                return (2.0);
            break;
        case TableOdds::TYPE_3X:
            return (3.0);
            break;
        case TableOdds::TYPE_3X_4X_5X:
            switch (nPoint)
            {
                case 4:
                case 10:
                    return (3.0);
                    break;
                case 5:
                case 9:
                    return (4.0);
                    break;
                case 6:
                case 8:
                    return (5.0);
                    break;
                default:
                    break;
            }
            break;
        case TableOdds::TYPE_5X:
            return (5.0);
            break;
        case TableOdds::TYPE_10X:
            return (10.0);
            break;
        case TableOdds::TYPE_20X:
            return (20.0);
            break;
        case TableOdds::TYPE_100X:
            return (100.0);
            break;
        default:
            throw CrapSimException("Table::MaxOdds unknown TableOdds");
            break;
    }

    return(1.0);
}

/**
  * Set the table type.
  *
  * Based on a passed string, sets the table odds.  The string is set in the
  * map of strings and TableOdds types.
  *
  * \param sTableType The type of table odds, e.g., 3X_4X_5X, 10X.
  *
  * \return True if type of odds is known, false otherwise.
  */

bool Table::SetTableType(std::string sTableType)
{
    if (sTableType.empty())
        return (false);

    std::locale loc;
    for (std::string::size_type iii = 0; iii < sTableType.length(); ++iii)
        sTableType[iii] = std::toupper(sTableType[iii], loc);

    std::map<std::string, TableOdds>::iterator it = m_mTableOdds.find(sTableType);

    if (it == m_mTableOdds.end()) return (false);

    //SetTableOdds(it->second);
    m_ecTableOdds = it->second;
    return (true);
}

/**
  * Return the table type in string format.
  *
  * Loops the known table types and compares to this table's odds.  When found,
  * return correspnding string. The string is set in the map of strings and
  * TableOdds types.
  *
  * \return String representing the type of table odds.
  */

std::string Table::TableType()
{
    std::string sTableType("Unknown");

    for (std::map<std::string, TableOdds>::iterator it = m_mTableOdds.begin(); it != m_mTableOdds.end(); ++it)
    {
        if (m_ecTableOdds == it->second)
        {
            sTableType = it->first;
            break;
        }
    }

    return(sTableType);
}

/**
  * Update the Table state.
  *
  * Based on the roll, update the Table, including the puck, coming out,
  * table point, and the shooter.
  *
  * \param cDice Dice. Used for the roll value.
  */

void Table::Update(const Dice &cDice)
{
    if (IsPuckOn())
    {
        if (cDice.IsSeven())
        {
            SetPuckOff();
            SetNewShooter(true);
        }
        if (Point() == cDice.RollValue())
        {
            SetPuckOff();
            SetNewShooter(false);
        }
    }
    else // Puck is off or Coming Out Roll
    {
        if (!cDice.IsCraps() && !cDice.IsNatural())
        {
            SetPoint(cDice.RollValue());
        }
    }
}
