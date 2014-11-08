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
        StrategyTracker(Strategy *pcStrategy);
        virtual ~StrategyTracker();
        void RecordNew(const Strategy *pcStrategy, const Table &cTable);
        void RecordBetsBeforeRoll(const Strategy *pcStrategy, const std::list<Bet> &lBets);
        void RecordBetsAfterRoll(const Strategy *pcStrategy, const std::list<Bet> &lBets, int nRoll);
        void Post();

    private:
        std::ofstream m_ofsBasics                   {};
        std::ofstream m_ofsSingleBets               {};

        int   m_nSequence                           {};
        int   m_nBeginningBankroll                  {};
        int   m_nWager                              {};
        float m_fOdds                               {};

        bool  m_bTableComeOutRoll                   {};
        int   m_nTablePoint                         {};

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

        std::map<int, int> m_mBeforeComeBetWager    {};
        std::map<int, int> m_mBeforeDontComeBetWager{};
        std::map<int, int> m_mBeforePlaceBetWager   {};

        int   m_nBeforeBankroll                     {};

        int   m_nRoll                               {};

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

        std::map<int, int> m_mAfterComeBetWager     {};
        std::map<int, int> m_mAfterDontComeBetWager {};
        std::map<int, int> m_mAfterPlaceBetWager    {};

        int   m_nAfterBankroll                      {};

};

#endif // STRATEGYTRACKER_H
