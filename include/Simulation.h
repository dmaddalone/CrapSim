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
        Simulation();

        void AddTable(Table cTable)          { this->m_cTable = cTable; }
        void AddStrategy(Strategy cStrategy) { m_vStrategies.push_back(cStrategy); }

        void Run(int nRuns, bool bMusterReport, bool bTally);

        void Report();

    private:
        Dice                    m_cDice;
        Table                   m_cTable;
        std::vector<Strategy>   m_vStrategies;

        void MakeBets();
        void ResolveBets();
        bool PlayersStillLeft();
        void UpdateStatisticsAndReset();
        void Muster();
};

#endif // SIMULATION_H
