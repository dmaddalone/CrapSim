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

#include "Bet.h"

/**
  * Calculate a bet payoff.
  *
  * Based on bet type and wager, calculate the payoff.
  *
  *\return The value of the bet's payoff.
  */

int Bet::CalculatePayoff ()
{
    switch (m_ecType)
    {
        case BetType::TYPE_PASS:                        // 1:1
            return (m_nWager);

        case BetType::TYPE_COME:                        // 1:1
            return (m_nWager);

        case BetType::TYPE_PASS_ODDS:                   // 2:1 on 4 and 0; 3:2 on 5 and 9; 6:5 on 6 and 8
        case BetType::TYPE_COME_ODDS:
            switch (Point())
            {
                case 4:
                case 10:
                    return (m_nWager * 2);               // 2:1
                    break;

                case 5:
                case 9:
                    return ((static_cast<float>(m_nWager) / 2.0) * 3);         // 3:2
                    break;

                case 6:
                case 8:
                    return ((static_cast<float>(m_nWager) / 5.0) * 6);         // 6:5
                    break;

                default:
                    break;
            }

        case BetType::TYPE_DONT_PASS:                   // 1:1
            return (m_nWager);

        case BetType::TYPE_DONT_COME:                   // 1:1
            return (m_nWager);

        case BetType::TYPE_DONT_PASS_ODDS:                   // 2:1 on 4 and 0; 3:2 on 5 and 9; 6:5 on 6 and 8
        case BetType::TYPE_DONT_COME_ODDS:
            switch (Point())
            {
                case 4:
                case 10:
                    return (m_nWager / 2);               // 1:2
                    break;

                case 5:
                case 9:
                    return ((static_cast<float>(m_nWager) / 3.0) * 2);         // 2:3
                    break;

                case 6:
                case 8:
                    return ((static_cast<float>(m_nWager) / 6.0) * 5);         // 5:6
                    break;

                default:
                    break;
            }

        case BetType::TYPE_PLACE:
            switch (Point())
            {
                case 4:
                case 10:
                    return ((static_cast<float>(m_nWager) / 5.0) * 9);  // 9:5
                    break;

                case 5:
                case 9:
                    return ((static_cast<float>(m_nWager) / 5.0) * 7);  // 7:5
                    break;

                case 6:
                case 8:
                    return ((static_cast<float>(m_nWager) / 6.0) * 7);  // 7:6
                    break;

                default:
                    break;
            }

        case BetType::TYPE_FIELD:
            switch (Point())
            {
                case 3:
                case 4:
                case 9:
                case 10:
                case 11:
                    return (m_nWager);              //1:1
                    break;
                case 2:
                case 12:
                    return (m_nWager * 2);          // 2:1
                    break;
                default:
                    break;
            }

        case BetType::TYPE_BIG_6:
            switch (Point())
            {
                case 6:
                    return (m_nWager);              // 1:1
                    break;
                default:
                    break;
            }

        case BetType::TYPE_BIG_8:
            switch (Point())
            {
                case 8:
                    return (m_nWager);              // 1:1
                    break;
                default:
                    break;
            }

        case BetType::TYPE_ANY_7:
            switch (Point())
            {
                case 7:
                    return (m_nWager * 4);          // 4:1
                    break;
                default:
                    break;
            }

        case BetType::TYPE_ANY_CRAPS:
            switch (Point())
            {
                case 2:
                case 3:
                case 12:
                    return (m_nWager * 7);          // 7:1
                    break;
                default:
                    break;
            }

        case BetType::TYPE_CRAPS_2:
            switch (Point())
            {
                case 2:
                    return (m_nWager * 30);         // 30:1
                    break;
                default:
                    break;
            }

        case BetType::TYPE_CRAPS_3:
            switch (Point())
            {
                case 3:
                    return (m_nWager * 15);         // 15:1
                    break;
                default:
                    break;
            }

        case BetType::TYPE_YO_11:
            switch (Point())
            {
                case 11:
                    return (m_nWager * 15);         // 15:1
                    break;
                default:
                    break;
            }

        case BetType::TYPE_CRAPS_12:
            switch (Point())
            {
                case 12:
                    return (m_nWager * 30);         // 30:1
                    break;
                default:
                    break;
            }

        default:
            throw std::domain_error("Bet::CalculatePayoff: unknown BetType");
            break;
    }

    return (0);
}
