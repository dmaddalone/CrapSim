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
 * \brief The StrategyTracker class is able to record a Strategy's play for
 * later analysis.
 *
 */

#ifndef STRATEGYTRACKER_H
#define STRATEGYTRACKER_H

#include <string>
#include <fstream>
#include "Strategy.h"
#include "Table.h"
#include "Bet.h"

class Strategy;

class StrategyTracker
{
    public:
        // Constructor
        StrategyTracker(Strategy *pcStrategy);
        // Copy constructor
        StrategyTracker(const StrategyTracker& cSource);
        // Assignment constructor
        StrategyTracker& operator=(const StrategyTracker& cSource);
        // Destructor
        virtual ~StrategyTracker();
        // Create a new record
        void RecordNew(const Strategy *pcStrategy, const Table &cTable);
        // Update record with current and newly made bets
        void RecordBetsBeforeRoll(const Strategy *pcStrategy, const std::list<Bet> &lBets);
        // Update record with bet results after the roll
        void RecordBetsAfterRoll(const Strategy *pcStrategy, const std::list<Bet> &lBets, int nRoll);
        // Post the record
        void Post();

    private:
        // Create a file for basic records
        std::ofstream *m_ofsBasics                   {};
        // Create a file for Single bets (e.g., Pass, Field)
        std::ofstream *m_ofsSingleBets               {};

        // Basic record fields
        int   m_nSequence                           {};
        int   m_nBeginningBankroll                  {};
        float m_fOdds                               {};
        bool  m_bTableComeOutRoll                   {};
        int   m_nTablePoint                         {};

        // Single bet fields before the roll
        struct BeforeSingleBets
        {
            int   nPassBetWager {};
            int   nPassOddsWager {};
            int   nPassPoint {};

            int   nFieldBetWager {};

            int   nDontPassBetWager {};
            int   nDontPassOddsWager {};
            int   nDontPassPoint {};
        } m_stBeforeSingleBets;

        // Containers for multiple bets (e.g., Come, Place) before the roll
        std::map<int, int> m_mBeforeComeBetWager    {};
        std::map<int, int> m_mBeforeDontComeBetWager{};
        std::map<int, int> m_mBeforePlaceBetWager   {};

        int   m_nBeforeBankroll                     {};
        int   m_nRoll                               {};

        // Single bet field after the roll
        struct AfteSingleBets
        {
            int   nPassBetWager {};
            int   nPassOddsWager {};
            int   nPassPoint {};

            int   nFieldBetWager {};

            int   nDontPassBetWager {};
            int   nDontPassOddsWager {};
            int   nDontPassPoint {};
        } m_stAfterSingleBets;

        // Containers for multiple bets after the roll
        std::map<int, int> m_mAfterComeBetWager     {};
        std::map<int, int> m_mAfterDontComeBetWager {};
        std::map<int, int> m_mAfterPlaceBetWager    {};

        int   m_nAfterBankroll                      {};
};

#endif // STRATEGYTRACKER_H
