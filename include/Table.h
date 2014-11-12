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

// Used to identify the type of table odds
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
        // Create a table with minimum and maximum wager limits
        Table(int nMin, int nMax);

        // Set and return the minimum and maximum bet wagers
        void   SetMinimumBet(int b)     { assert (b > 0); m_nMinimumBet = b; }
        int    MinimumBet() const       { return (m_nMinimumBet); }
        void   SetMaximumBet(int b)     { assert (b > 0); m_nMaximumBet = b; }
        int    MaximumBet() const       { return (m_nMaximumBet); }
        // Set and return the table point
        void   SetPoint(int p)          { m_bPuckOn = true; m_nPoint = p; }
        int    Point() const            { return (m_nPoint); }
        // Set puck off
        void   SetPuckOff()             { m_bPuckOn = false; m_nPoint = 0; }
        // Return whether the puck is on
        bool   IsPuckOn() const         { return m_bPuckOn; }
        // Set and return whether a new shooter
        void   SetNewShooter(bool b)    { m_bNewShooter = b; }
        bool   NewShooter() const       { return (m_bNewShooter); }
        // Return whether the table is coming out
        bool   IsComingOutRoll() const  { return (!m_bPuckOn); }
        // Get maximum table odds for a bet
        float  MaxOdds(int i) const;
        // Update the table state based on the dice roll
        void   Update(const Dice &cDice);
        // Reset the table - meant to be called before a new Simulation run
        void   Reset()                  { SetPuckOff(); SetNewShooter(true); }
        // Set and return the table type (odds offered)
        std::string TableType();
        bool SetTableType(std::string sTableType);
        // Return the bar, for Don't Pass bets.  TODO: make configurable
        std::string Bar()               { return "Bar 12"; }

    private:
        // Set table defaults
        TableOdds   m_ecTableOdds   = TableOdds::TYPE_3X_4X_5X;
        int         m_nMinimumBet   = 5;
        int         m_nMaximumBet   = 1000;
        bool        m_bPuckOn       = false;
        int         m_nPoint        = 0;
        bool        m_bNewShooter   = true;

        // Map used to associate the string names with the enum values
        std::map<std::string, TableOdds> m_mTableOdds;
};

#endif // TABLE_H
