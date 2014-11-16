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
    m_cDice(2, 6),
    m_cTable(5, 5000)
{}

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
    std::cout << "Table" << std::endl;
    std::cout << "\tType:\t\t" << m_cTable.TableType() << std::endl;
    std::cout << "\tMinimum Wager:\t" << m_cTable.MinimumBet() << std::endl;
    std::cout << "\tMaximum Wager:\t" << m_cTable.MaximumBet() << std::endl;
    std::cout << "\tBar:\t\t" << m_cTable.Bar() << std::endl;

    std::cout << "Strategies" << std::endl;
    std::cout << "\tNumber of strategies:\t" << m_vStrategies.size() << std::endl;

    std::cout << "Simulation" << std::endl;
    std::cout << "\tNumber of runs:\t" << nNumberOfRuns << std::endl;

    // Ensure that each Strategy conforms to the environment
    std::cout << "Sanity-checking the Strategies." << std::endl;
    for (std::vector<Strategy>::iterator it = m_vStrategies.begin(); it != m_vStrategies.end(); it++)
    {
        it->SanityCheck(m_cTable);
    }
    std::cout << "Sanity-check complete." << std::endl;

    // Write Muster
    if (bMusterReport) Muster();

    std::cout << "\nStarting Simulation" << std::endl;
    for (int iii = 0; iii < nNumberOfRuns; iii++)
    {
        m_cTable.Reset();

        do
        {
            MakeBets();
            m_cDice.Roll();
            ResolveBets();
            QualifyTheShooter();
            m_cTable.Update(m_cDice);
        }
        while (PlayersStillLeft());

        UpdateStatisticsAndReset();

        if (bTally)
        if ((iii+1) % 100 == 0) std::cout << "\rCompleted " << iii+1 << " out of " << nNumberOfRuns << " runs" << std::flush;
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
    for (std::vector<Strategy>::iterator it = m_vStrategies.begin(); it != m_vStrategies.end(); it++)
    {
        if (it->ShooterQualified()) it->MakeBets(m_cTable);
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
    for (std::vector<Strategy>::iterator it = m_vStrategies.begin(); it != m_vStrategies.end(); it++)
    {
        it->ResolveBets(m_cTable, m_cDice);
    }
}

/**
  * Qualifty The Shooter.
  *
  * Loop through Strategies and direct them to qualify the shooter.
  *
  */

void Simulation::QualifyTheShooter()
{
    for (std::vector<Strategy>::iterator it = m_vStrategies.begin();it != m_vStrategies.end(); it++)
    {
        it->QualifyTheShooter(m_cTable, m_cDice);
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

    for (std::vector<Strategy>::iterator it = m_vStrategies.begin(); it != m_vStrategies.end(); it++)
    {
        bPlayerStillLeft = it->StillPlaying() || bPlayerStillLeft;
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
    for (std::vector<Strategy>::iterator it = m_vStrategies.begin(); it != m_vStrategies.end(); it++)
    {
        it->UpdateStatistics();
        it->Reset();
    }
}

/**
  * Print muster report.
  *
  * Loop through Strateges and direct them to print a muster.
  *
  */

void Simulation::Muster()
{
    std::cout << "Muster" << std::endl;

    for (std::vector<Strategy>::iterator it = m_vStrategies.begin(); it != m_vStrategies.end(); it++)
    {
        it->Muster();
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
    std::cout << std::setw(78) << std::right << "Rolls to Win " <<
              std::setw(18) << std::right << "Rolls to Lose" <<
              std::endl;

    std::cout << std::setw(40) << std::right << "Name " <<
              std::setw(6)  << std::left << "Plays" <<
              std::setw(5)  << std::left << "Wins" <<
              std::setw(7)  << std::left << "Losses" <<
              std::setw(6)  << std::left << "Win %" <<
              std::setw(5)  << std::right << "Avg" <<
              std::setw(5)  << std::right << "Min" <<
              std::setw(7)  << std::right << "Max" <<
              std::setw(5)  << std::right << "Avg" <<
              std::setw(5)  << std::right << "Min" <<
              std::setw(7)  << std::right << "Max" <<
              std::endl;

    for (std::vector<Strategy>::iterator it = m_vStrategies.begin(); it != m_vStrategies.end(); it++)
    {
        it->Report();
    }
}
