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

#include <cassert>
#include <climits>
#include <iostream>

class Money
{
    public:
        void Initialize(int n);

        int  InitialBankroll() const                 { return (m_nInitialBankroll); }
        int  Bankroll() const                        { return (m_nBankroll); }
        int  MaxBankroll() const                     { return (m_nMaxBankroll); }
        void Decrement(int w)                        { m_nBankroll -= w; }
        void Increment(int w)                        { m_nBankroll += w; if (m_nMaxBankroll < m_nBankroll) m_nMaxBankroll = m_nBankroll; }

        float SignificantWinningsMultiple() const    { return (m_fSignificantWinningsMultiple); }
        void SetSignificantWinningsMultiple(float f) { assert (f > 1.0); m_fSignificantWinningsMultiple = f; }

        int SignificantWinnings() const              { return (m_nSignificantWinnings); }
        void SetSignificantWinnings(int i)           { assert (i > 1); m_nSignificantWinnings = i; }

        bool HasSignificantWinnings() const;

        void Reset()                                 { m_nBankroll = m_nMaxBankroll = m_nInitialBankroll; }

    private:
        int m_nInitialBankroll  = 0;
        int m_nBankroll         = 0;
        int m_nMaxBankroll      = 0;

        float m_fSignificantWinningsMultiple  = 2.0;
        int   m_nSignificantWinnings = INT_MIN;
};

#endif // MONEY_H
