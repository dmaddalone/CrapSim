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
 * \brief The Table class represents the craps table.
 *
 */

#ifndef TABLE_H
#define TABLE_H

#include <cassert>
#include <map>
#include <string>
#include <locale>
#include "Dice.h"

enum class TableOdds
{
    TYPE_1X,
    TYPE_2X,
    TYPE_FULL_DOUBLE,
    TYPE_3X,
    TYPE_3X_4X_5X,
    TYPE_5X,
    TYPE_10X,
    TYPE_20X,
    TYPE_100X
};

class Table
{
    public:
        Table();

        //void SetTableOdds(TableOdds to) { m_ecTableOdds = to; }
        //TableOdds GetTableOdds() const  { return (m_ecTableOdds); }
        void   SetMinimumBet(int b)     { assert (b > 0); m_nMinimumBet = b; }
        int    MinimumBet() const       { return (m_nMinimumBet); }
        void   SetMaximumBet(int b)     { assert (b > 0); m_nMaximumBet = b; }
        int    MaximumBet() const       { return (m_nMaximumBet); }

        void   SetPoint(int p)          { m_bPuckOn = true; m_nPoint = p; }
        int    Point() const            { return (m_nPoint); }

        void   SetPuckOff()             { m_bPuckOn = false; m_nPoint = 0; }
        bool   IsPuckOn() const         { return m_bPuckOn; }

        void   SetNewShooter(bool b)    { m_bNewShooter = b; }
        bool   NewShooter() const       { return (m_bNewShooter); }

        bool   IsComingOutRoll() const  { return (!m_bPuckOn); }

        float GetMaxOdds(int i) const;
        void  Update(const Dice &cDice);

        void  Reset()                   { SetPuckOff(); SetNewShooter(true); }

        std::string TableType();
        bool SetTableType(std::string sTableType);

        std::string Bar()               { return "Bar 12"; }

    private:
        TableOdds   m_ecTableOdds   = TableOdds::TYPE_3X_4X_5X;
        // Map to associate the strings with the enum values
        std::map<std::string, TableOdds> m_mTableOdds;

        int         m_nMinimumBet   = 5;
        int         m_nMaximumBet   = 1000;

        bool        m_bPuckOn       = false;
        int         m_nPoint        = 0;

        bool        m_bNewShooter   = true;
};

#endif // TABLE_H
