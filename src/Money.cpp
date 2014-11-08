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

#include "Money.h"

void Money::Initialize(int n)
{
    if (n <= 0)
    {
        std::cerr << "ERROR (Money): Bankroll initialization set to: " << n << std::endl;
        std::cerr << "Exiting" << std::endl;
        exit (EXIT_FAILURE);
    }

    m_nInitialBankroll = m_nBankroll = m_nMaxBankroll = n;
}

bool Money::HasSignificantWinnings() const
{
    bool bSignificantWinnings = false;

    if (m_nBankroll > (static_cast<float>(m_nInitialBankroll) * m_fSignificantWinningsMultiple))
        bSignificantWinnings = true;

    if (m_nSignificantWinnings > 0)
        if (m_nBankroll > m_nInitialBankroll + m_nSignificantWinnings)
            bSignificantWinnings = true;

    return (bSignificantWinnings);
}
