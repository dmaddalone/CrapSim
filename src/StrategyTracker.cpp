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

#include <iostream>
#include "StrategyTracker.h"

StrategyTracker::StrategyTracker(Strategy *pcStrategy)
{
    std::string sFileName {};
    std::string sStrategyName(pcStrategy->Name());
    std::string chars = ".()-'";

    for (size_t iii = 0; iii < chars.length(); iii++)
    {
        sStrategyName.erase(std::remove(sStrategyName.begin(), sStrategyName.end(), chars[iii]), sStrategyName.end());
    }

    sFileName = "CrapSim" + sStrategyName + "Basics.csv";
    m_ofsBasics.open(sFileName);
    if (!m_ofsBasics.is_open())
    {
        std::cerr << "ERROR (StrategyTracker): Could not open " << sFileName << " for writing" << std::endl;
        std::cerr << "Exiting" << std::endl;
        exit (EXIT_FAILURE);
    }

    sFileName = "CrapSim" + sStrategyName + "SingleBets.csv";
    m_ofsSingleBets.open(sFileName);
    if (!m_ofsSingleBets.is_open())
    {
        std::cerr << "ERROR (StrategyTracker): Could not open " << sFileName << " for writing" << std::endl;
        std::cerr << "Exiting" << std::endl;
        exit (EXIT_FAILURE);
    }
}

StrategyTracker::~StrategyTracker()
{
    m_ofsBasics.close();
    m_ofsSingleBets.close();
}

// Start a new record, which updates bankroll, and update table stats
void StrategyTracker::RecordNew(const Strategy *pcStrategy, const Table &cTable)
{
    // Increment sequence number
    m_nSequence++;
    // Record current bankroll
    m_nBeginningBankroll = pcStrategy->Bankroll();
    // Record current wager amount
    m_nWager = pcStrategy->Wager();
    // Record current odds multiple
    m_fOdds = pcStrategy->Odds();
    // Record Table status
    m_bTableComeOutRoll = cTable.IsComingOutRoll();
    m_nTablePoint       = cTable.Point();
}

// Record additions and changes to bets before roll
void StrategyTracker::RecordBetsBeforeRoll(const Strategy *pcStrategy, const std::list<Bet> &lBets)
{
    // Clear struct before recording bets
    m_stBeforeSingleBets = {};

    for (std::list<Bet>::const_iterator it = lBets.begin(); it != lBets.end(); it++)
    {
        if ((*it).IsPassBet())
        {
            m_stBeforeSingleBets.nPassBetWager = (*it).Wager();
            m_stBeforeSingleBets.nPassPoint = (*it).Point();
        }

        if ((*it).IsPassOddsBet()) m_stBeforeSingleBets.nPassOddsWager = (*it).Wager();

        if ((*it).IsDontPassBet())
        {
            m_stBeforeSingleBets.nDontPassBetWager = (*it).Wager();
            m_stBeforeSingleBets.nDontPassPoint = (*it).Point();
        }

        if ((*it).IsDontPassOddsBet()) m_stBeforeSingleBets.nDontPassOddsWager = (*it).Wager();

        if ((*it).IsComeBet())
        {
            std::map<int, int>::iterator bit = m_mBeforeComeBetWager.find((*it).Point());
            if (bit == m_mBeforeComeBetWager.end())
            {
                m_mBeforeComeBetWager[(*it).Point()] = (*it).Wager();
            }
            else
            {
                bit->second = (*it).Wager();
            }
        }

        if ((*it).IsFieldBet()) m_stBeforeSingleBets.nFieldBetWager = (*it).Wager();
    }

    m_nBeforeBankroll = pcStrategy->Bankroll();
}

// Record additions and changes to bets after roll
void StrategyTracker::RecordBetsAfterRoll(const Strategy *pcStrategy, const std::list<Bet> &lBets, int nRoll)
{
    // Clear struct before recording bets
    m_stAfterSingleBets = {};

    for (std::list<Bet>::const_iterator it = lBets.begin(); it != lBets.end(); it++)
    {
        if ((*it).IsPassBet())
        {
            m_stAfterSingleBets.nPassBetWager = (*it).Wager();
            m_stAfterSingleBets.nPassPoint = (*it).Point();
        }

        if ((*it).IsPassOddsBet()) m_stAfterSingleBets.nPassOddsWager = (*it).Wager();

        if ((*it).IsDontPassBet())
        {
            m_stAfterSingleBets.nDontPassBetWager = (*it).Wager();
            m_stAfterSingleBets.nDontPassPoint = (*it).Point();
        }

        if ((*it).IsDontPassOddsBet()) m_stAfterSingleBets.nDontPassOddsWager = (*it).Wager();

        if ((*it).IsComeBet())
        {
            std::map<int, int>::iterator ait = m_mAfterComeBetWager.find((*it).Point());
            if (ait == m_mAfterComeBetWager.end())
            {
                m_mAfterComeBetWager[(*it).Point()] = (*it).Wager();
            }
            else
            {
                ait->second = (*it).Wager();
            }
        }

        if ((*it).IsFieldBet()) m_stAfterSingleBets.nFieldBetWager = (*it).Wager();
    }

    m_nRoll          = nRoll;
    m_nAfterBankroll = pcStrategy->Bankroll();
}

// Record the ending bankroll and post the record
void StrategyTracker::Post()
{
    m_ofsBasics <<  m_nSequence                     << "," <<
                    m_nWager                        << "," <<
                    m_fOdds                         << "," <<
                    m_nBeginningBankroll            << "," <<
                    m_bTableComeOutRoll             << "," <<
                    m_nTablePoint                   << "," <<
                    m_nBeforeBankroll               << "," <<
                    m_nRoll                         << "," <<
                    m_nAfterBankroll                <<
                    std::endl;

    m_ofsSingleBets <<  m_nSequence                  << "," <<
                        m_stBeforeSingleBets.nPassBetWager        << "," <<
                        m_stBeforeSingleBets.nPassOddsWager       << "," <<
                        m_stBeforeSingleBets.nPassPoint           << "," <<
                        m_stBeforeSingleBets.nDontPassBetWager    << "," <<
                        m_stBeforeSingleBets.nDontPassOddsWager   << "," <<
                        m_stBeforeSingleBets.nDontPassPoint       << "," <<
                        m_stBeforeSingleBets.nFieldBetWager       << "," <<

                        m_stAfterSingleBets.nPassBetWager         << "," <<
                        m_stAfterSingleBets.nPassOddsWager        << "," <<
                        m_stAfterSingleBets.nPassPoint            << "," <<
                        m_stAfterSingleBets.nDontPassBetWager     << "," <<
                        m_stAfterSingleBets.nDontPassOddsWager    << "," <<
                        m_stAfterSingleBets.nDontPassPoint        << "," <<
                        m_stAfterSingleBets.nFieldBetWager        <<
                        std::endl;
}
