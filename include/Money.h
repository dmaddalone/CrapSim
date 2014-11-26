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
 * \brief The Money class represents a player's (or a Strategy's) bankroll.
 *
 */

#ifndef MONEY_H
#define MONEY_H

#include <stdexcept>
#include <climits>
#include <iostream>

class Money
{
    public:
        // Initialize bankroll - meant to be called only once
        void Initialize(int n);

        // Return the initial bankroll
        int  InitialBankroll() const                 { return (m_nInitialBankroll); }
        // Return the current bankroll
        int  Bankroll() const                        { return (m_nBankroll); }
        // Return the max bankroll attained
        int  MaxBankroll() const                     { return (m_nMaxBankroll); }
        // Decrement from the bankroll
        void Decrement(int w)                        { m_nBankroll -= w; }
        // Increment bankroll and update max bankroll attained
        void Increment(int w)                        { m_nBankroll += w; if (m_nMaxBankroll < m_nBankroll) m_nMaxBankroll = m_nBankroll; }
        // Mark the bankroll figure before betting for later comparison
        void MarkBeforeBetting()                     { m_nBankrollBeforeBetting = m_nBankroll; }
        // Mark the bankroll figure after betting for later comparison
        void MarkAfterBetting()                      { m_nBankrollAfterBetting = m_nBankroll; }
        // Mark bankroll figure after resolving bets for ater comparison
        void MarkAfterResolvingBets()                { m_nBankrollAfterResolving = m_nBankroll; }
        // Bankroll gain comparisons
        bool GainBeforeBetting()                     { if (m_nBankrollAfterResolving > m_nBankrollBeforeBetting) return (true); else return (false); }
        bool GainAfterBetting()                      { if (m_nBankrollAfterResolving > m_nBankrollAfterBetting) return(true); else return (false); }
        bool GainOverInitial()                       { if (m_nBankrollAfterResolving > m_nInitialBankroll) return (true); else return (false); }

        // The significant winnings are used to determine when a Strategy has won and may stop playing
        // Return the significant winning multiple
        float SignificantWinningsMultiple() const    { return (m_fSignificantWinningsMultiple); }
        // Set the significant winning multiple
        void SetSignificantWinningsMultiple(float f) { if (f >= 1.0) m_fSignificantWinningsMultiple = f; else throw std::domain_error("Money::SetSignificantWinningsMultiple"); }
        // Return the significant winnings absolute amount
        int SignificantWinnings() const              { return (m_nSignificantWinnings); }
        // Set the significant winnings absolute amount
        void SetSignificantWinnings(int i)           { if (i >= 1) m_nSignificantWinnings = i; else throw std::domain_error("Money::SetSignificantWinnings"); }
        // Return whether the significant winnings has been attained
        bool HasSignificantWinnings() const;

        // Reset the class - meant to be called before a new Simulation run
        void Reset();

    private:
        //  Set counters to zero
        int m_nInitialBankroll           {0};
        int m_nBankroll                  {0};
        int m_nMaxBankroll               {0};
        int m_nBankrollBeforeBetting     {0};
        int m_nBankrollAfterBetting      {0};
        int m_nBankrollAfterResolving    {0};

        // Set significant winnings multiple to default
        float m_fSignificantWinningsMultiple {2.0};
        // Set significant winnings to zero (effectively)
        int   m_nSignificantWinnings {INT_MIN};
};

#endif // MONEY_H
