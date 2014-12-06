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

#include "Simulation.h"

#include <iostream>
#include <iomanip>

/**
  * Construct the Simulatiom
  *
  */

Simulation::Simulation() :
    m_cDice(),
    m_cTable(5, 5000)
{
}

/**
  * Run the simulatioms.
  *
  * Print configuration information, sanity check the Strategies, print a
  * muster report, and execute the simulation.
  *
  *\param nNumberOfRuns Number of simulation runs to execute
  *\param bMusterReport Flag to print muster report
  *\param bTally Flag to print tally as simulation runs execute
  */

void Simulation::Run(int nNumberOfRuns, bool bMusterReport, bool bTally)
{
    // Ensure that each Strategy conforms to the environment
    std::cout << "Sanity-checking the Strategies." << std::endl;
    for (std::vector<Strategy>::iterator it = m_vStrategies.begin(); it != m_vStrategies.end(); ++it)
    {
        it->SanityCheck(m_cTable);
    }
    std::cout << "Sanity-check complete." << std::endl;

    // Write Muster
    if (bMusterReport) Muster();

    std::cout << "\nTable" << std::endl;
    std::cout << "\tType:\t\t" << m_cTable.TableType() << std::endl;
    std::cout << "\tMinimum Wager:\t" << m_cTable.MinimumBet() << std::endl;
    std::cout << "\tMaximum Wager:\t" << m_cTable.MaximumBet() << std::endl;
    std::cout << "\tBar:\t\t" << m_cTable.Bar() << std::endl;

    std::cout << "Strategies" << std::endl;
    std::cout << "\tNumber of strategies:\t" << m_vStrategies.size() << std::endl;

    std::cout << "Simulation" << std::endl;
    std::cout << "\tNumber of runs:\t" << nNumberOfRuns << std::endl;

    std::cout << "\nStarting Simulation" << std::endl;

    // Loop through the number of runs
    for (int iii = 0; iii < nNumberOfRuns; ++iii)
    {
        do
        {
            MakeBets();
            m_cDice.Roll();
            ResolveBets();
            QualifyTheShooter();
            ModifyBets();
            FinalizeBets();
            m_cTable.Update(m_cDice);
        }
        while (PlayersStillLeft());

        UpdateStatisticsAndReset();

        if (bTally)
            if ((iii+1) % 100 == 0)
                std::cout << "\rCompleted " << iii+1 << " out of " << nNumberOfRuns << " runs" << std::flush;
    }

    std::cout << std::endl;
}

/**
  * Make Bets.
  *
  * Loop through Strategies and direct them to make bets.  Bets are made if
  * the shooter is qualified.
  *
  */

void Simulation::MakeBets()
{
    for (Strategy &cStrategy : m_vStrategies)
    {
        cStrategy.MakeBets(m_cTable);
    }

}

/**
  * Resolve Bets.
  *
  * Loop through Strategies and direct them to resolve bets.
  *
  */

void Simulation::ResolveBets()
{
    for (Strategy &cStrategy : m_vStrategies)
    {
        cStrategy.ResolveBets(m_cTable, m_cDice);
    }
}

/**
  * Qualify The Shooter.
  *
  * Loop through Strategies and direct them to qualify the shooter.
  *
  */

void Simulation::QualifyTheShooter()
{
    for (Strategy &cStrategy : m_vStrategies)
    {
        cStrategy.QualifyTheShooter(m_cTable, m_cDice);
    }
}

/**
  * Modify Bets.
  *
  * Loop through Strategies and direct them to modify their bets.
  *
  */

void Simulation::ModifyBets()
{
    for (Strategy &cStrategy : m_vStrategies)
    {
        cStrategy.ModifyBets(m_cTable, m_cDice);
    }
}

/**
  * Finalize Bets.
  *
  * Loop through Strategies and direct them to finalize their bets.
  *
  */

void Simulation::FinalizeBets()
{
    for (Strategy &cStrategy : m_vStrategies)
    {
        cStrategy.FinalizeBets();
    }
}

/**
  * Check to see if players (Strategies) are still playing.
  *
  * Loop through Strategies and check to see if they are still in the game.
  *
  */

bool Simulation::PlayersStillLeft()
{
    bool bPlayerStillLeft = false;

    for (const Strategy &cStrategy : m_vStrategies)
    {
        bPlayerStillLeft = cStrategy.StillPlaying() || bPlayerStillLeft;
    }

    return (bPlayerStillLeft);
}

/**
  * Update stats and reset for a new simulation run.
  *
  * Loop through Strategies, update stats and reset.
  *
  */

void Simulation::UpdateStatisticsAndReset()
{
    for (Strategy &cStrategy : m_vStrategies)
    {
        cStrategy.UpdateStatistics();
        cStrategy.Reset();
    }

    m_cTable.Reset();
}

/**
  * Print muster report.
  *
  * Loop through Strategies and direct them to print a muster.
  *
  */

void Simulation::Muster()
{
    std::cout << "Muster" << std::endl;

    for (const Strategy &cStrategy : m_vStrategies)
    {
        cStrategy.Muster();
    }
}

/**
  * Print end of simulation report.
  *
  * Print header for report. // TODO - remove header.
  *
  */

void Simulation::Report()
{
    for (std::vector<Strategy>::iterator it = m_vStrategies.begin(); it != m_vStrategies.end(); ++it)
    {
        it->Report();
    }

    std::cout << "\nDice History" << std::endl;
    for (int iii = 2; iii <= (2 * 6); ++iii)
    {
        std::cout << "\tValue " << iii << " rolled " << m_cDice.RollValueCount(iii) <<
            " times (" << m_cDice.RollValuePercentage(iii) << "%)" << std::endl;
    }
    std::cout << "\tTotal Rolls for all Simluations: " << m_cDice.TotalRolls() << std::endl;
}
