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
 * \brief The Simulation class gathers Strategies at one table and runs
 * simulations.
 *
 */

#ifndef SIMULATION_H
#define SIMULATION_H

#include "Dice.h"
#include "Table.h"
#include "Strategy.h"
#include <vector>

class Simulation
{
    public:
        // Constructor creates two dice
        Simulation();

        // Add classes to the Simulation
        void AddTable(Table cTable)          { this->m_cTable = cTable; }
        void AddStrategy(Strategy cStrategy) { m_vStrategies.push_back(cStrategy); }

        // Run number of nRuns simulations.  Flags to generate a muster and the working tally.
        void Run(int nRuns, bool bMusterReport, bool bTally);
        // End of simulation report.
        void Report();

    private:
        // Dice and Table
        Dice                    m_cDice;
        Table                   m_cTable;
        // Container of Strategies
        std::vector<Strategy>   m_vStrategies;

        // Direct Strategies to make their bets
        void MakeBets();
        // Direct Strategies to resolve their bets
        void ResolveBets();
        // Check if Strategies (Players) are still playing
        bool PlayersStillLeft();
        // Capture stats
        void UpdateStatisticsAndReset();
        // Create a muster report before starting the simulation
        void Muster();
};

#endif // SIMULATION_H
