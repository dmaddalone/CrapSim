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

Table::Table()
{
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

//Table::~Table()
//{
//    //dtor
//}

float Table::GetMaxOdds(int nPoint) const
{
    assert ((nPoint == 4) || (nPoint == 5) || (nPoint == 6) || (nPoint == 8) || (nPoint == 9) || (nPoint ==10));

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
            return(1.0);
            break;
    }

    return(1.0);
}

bool Table::SetTableType(std::string sTableType)
{
    if (sTableType.empty())
        return (false);

    std::locale loc;
    for (std::string::size_type iii = 0; iii < sTableType.length(); iii++)
        sTableType[iii] = std::toupper(sTableType[iii], loc);

    std::map<std::string, TableOdds>::iterator it = m_mTableOdds.find(sTableType);

    if (it == m_mTableOdds.end()) return (false);

    SetTableOdds(it->second);
    return (true);
}

std::string Table::TableType()
{
    std::string sTableType("Unknown");

    switch (m_ecTableOdds)
    {
        case TableOdds::TYPE_1X:
            sTableType = "1x Odds";
            break;
        case TableOdds::TYPE_2X:
            sTableType = "2x Odds";
            break;
        case TableOdds::TYPE_FULL_DOUBLE:
            sTableType = "Full Double Odds";
            break;
        case TableOdds::TYPE_3X:
            sTableType = "3x Odds";
            break;
        case TableOdds::TYPE_3X_4X_5X:
            sTableType = "3x 4x 5x Odds";
            break;
        case TableOdds::TYPE_5X:
            sTableType = "5x Odds";
            break;
        case TableOdds::TYPE_10X:
            sTableType = "10x Odds";
            break;
        case TableOdds::TYPE_20X:
            sTableType = "20x Odds";
            break;
        case TableOdds::TYPE_100X:
            sTableType = "100x Odds";
            break;
        default:
            break;
    }

    return(sTableType);
}


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

