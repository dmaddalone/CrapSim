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
    m_ofsBasics = new std::ofstream();
    m_ofsBasics->open(sFileName);
    if (!m_ofsBasics->is_open())
    {
        std::cerr << "ERROR (StrategyTracker): Could not open " << sFileName << " for writing" << std::endl;
        std::cerr << "Exiting" << std::endl;
        exit (EXIT_FAILURE);
    }

    sFileName = "CrapSim" + sStrategyName + "SingleBets.csv";
    m_ofsSingleBets = new std::ofstream();
    m_ofsSingleBets->open(sFileName);
    if (!m_ofsSingleBets->is_open())
    {
        std::cerr << "ERROR (StrategyTracker): Could not open " << sFileName << " for writing" << std::endl;
        std::cerr << "Exiting" << std::endl;
        exit (EXIT_FAILURE);
    }
}

StrategyTracker::StrategyTracker(const StrategyTracker &cSource) :
    m_ofsBasics(cSource.m_ofsBasics),
    m_ofsSingleBets(cSource.m_ofsSingleBets),
    m_mBeforeComeBetWager(cSource.m_mBeforeComeBetWager),
    m_mBeforeDontComeBetWager(cSource.m_mBeforeDontComeBetWager),
    m_mBeforePlaceBetWager(cSource.m_mBeforePlaceBetWager),
    m_mAfterComeBetWager(cSource.m_mAfterComeBetWager),
    m_mAfterDontComeBetWager(cSource.m_mAfterDontComeBetWager),
    m_mAfterPlaceBetWager(cSource.m_mAfterPlaceBetWager)
{
    m_nSequence          = cSource.m_nSequence;
    m_nBeginningBankroll = cSource.m_nBeginningBankroll;
    m_nWager             = cSource.m_nWager;
    m_fOdds              = cSource.m_fOdds;
    m_bTableComeOutRoll  = cSource.m_bTableComeOutRoll;
    m_nTablePoint        = cSource.m_nTablePoint;

    m_stBeforeSingleBets = cSource.m_stBeforeSingleBets;

    m_nBeforeBankroll    = cSource.m_nBeforeBankroll;
    m_nRoll              = cSource.m_nRoll;

    m_stAfterSingleBets  = cSource.m_stAfterSingleBets;

    m_nAfterBankroll     = cSource.m_nAfterBankroll;
}

StrategyTracker& StrategyTracker::operator=(const StrategyTracker& cSource)
{
    if (this == &cSource)
        return (*this);

    //m_ofsBasics(cSource.m_ofsBasics);
    m_ofsBasics = new std::ofstream();
    m_ofsBasics = cSource.m_ofsBasics;

    //m_ofsSingleBets(cSource.m_ofsSingleBets);
    m_ofsSingleBets = new std::ofstream();
    m_ofsSingleBets = cSource.m_ofsSingleBets;

    m_mBeforeComeBetWager     = cSource.m_mBeforeComeBetWager;
    m_mBeforeDontComeBetWager = cSource.m_mBeforeDontComeBetWager;
    m_mBeforePlaceBetWager    = cSource.m_mBeforePlaceBetWager;
    m_mAfterComeBetWager      = cSource.m_mAfterComeBetWager;
    m_mAfterDontComeBetWager  = cSource.m_mAfterDontComeBetWager;
    m_mAfterPlaceBetWager     = cSource.m_mAfterPlaceBetWager;

    m_nSequence          = cSource.m_nSequence;
    m_nBeginningBankroll = cSource.m_nBeginningBankroll;
    m_nWager             = cSource.m_nWager;
    m_fOdds              = cSource.m_fOdds;
    m_bTableComeOutRoll  = cSource.m_bTableComeOutRoll;
    m_nTablePoint        = cSource.m_nTablePoint;

    m_stBeforeSingleBets = cSource.m_stBeforeSingleBets;

    m_nBeforeBankroll    = cSource.m_nBeforeBankroll;
    m_nRoll              = cSource.m_nRoll;

    m_stAfterSingleBets  = cSource.m_stAfterSingleBets;

    m_nAfterBankroll     = cSource.m_nAfterBankroll;

    return (*this);
}

StrategyTracker::~StrategyTracker()
{
    if (m_ofsBasics->is_open()) m_ofsBasics->close();
    if (m_ofsSingleBets->is_open()) m_ofsSingleBets->close();
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
    *m_ofsBasics <<  m_nSequence                     << "," <<
                    m_nWager                        << "," <<
                    m_fOdds                         << "," <<
                    m_nBeginningBankroll            << "," <<
                    m_bTableComeOutRoll             << "," <<
                    m_nTablePoint                   << "," <<
                    m_nBeforeBankroll               << "," <<
                    m_nRoll                         << "," <<
                    m_nAfterBankroll                <<
                    std::endl;

    *m_ofsSingleBets <<  m_nSequence                  << "," <<
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
