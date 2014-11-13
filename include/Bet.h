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
 * \brief The Bet class represents a single craps bet and understands payoffs.
 *
 */

#ifndef BET_H
#define BET_H

// Used to identify the type of bet
enum class BetType
{
    TYPE_NOBET,
    TYPE_PASS,
    TYPE_DONT_PASS,
    TYPE_COME,
    TYPE_DONT_COME,
    TYPE_PLACE,
    TYPE_PLACE_TO_LOSE,
    TYPE_BUY,
    TYPE_LAY,
    TYPE_BIG_6,
    TYPE_BIG_8,
    TYPE_HARD_WAY,
    TYPE_PUT,
    TYPE_PASS_ODDS,
    TYPE_COME_ODDS,
    TYPE_DONT_PASS_ODDS,
    TYPE_DONT_COME_ODDS,
    TYPE_FIELD,
    TYPE_ANY_7,
    TYPE_ANY_CRAPS
};

class Bet
{
    public:
        // Set of bet identification checks
        bool IsPassBet() const                  { return (m_ecType == BetType::TYPE_PASS); }
        bool IsPassOddsBet() const              { return (m_ecType == BetType::TYPE_PASS_ODDS); }
        bool IsComeBet() const                  { return (m_ecType == BetType::TYPE_COME); }
        bool IsComeOddsBet() const              { return (m_ecType == BetType::TYPE_COME_ODDS); }
        bool IsDontPassBet() const              { return (m_ecType == BetType::TYPE_DONT_PASS); }
        bool IsDontPassOddsBet() const          { return (m_ecType == BetType::TYPE_DONT_PASS_ODDS); }
        bool IsDontComeBet() const              { return (m_ecType == BetType::TYPE_DONT_COME); }
        bool IsDontComeOddsBet() const          { return (m_ecType == BetType::TYPE_DONT_COME_ODDS); }
        bool IsPlaceBet() const                 { return (m_ecType == BetType::TYPE_PLACE); }
        bool IsFieldBet() const                 { return (m_ecType == BetType::TYPE_FIELD); }
        bool IsBig6Bet() const                  { return (m_ecType == BetType::TYPE_BIG_6); }
        bool IsBig8Bet() const                  { return (m_ecType == BetType::TYPE_BIG_8); }
        bool IsAny7Bet() const                  { return (m_ecType == BetType::TYPE_ANY_7); }
        bool IsAnyCrapsBet() const              { return (m_ecType == BetType::TYPE_ANY_CRAPS); }

        // Set and return whether come odds are working on the come out
        bool ComeOddsAreWorking() const         { return (m_bComeOddsAreWorking); }
        void SetComeOddsAreWorking(bool b)      { m_bComeOddsAreWorking = b; }

        // Set of methods to make bets
        void MakePassBet(int w)                 { m_ecType = BetType::TYPE_PASS; m_nWager = w; }
        void MakePassOddsBet(int w, int p)      { m_ecType = BetType::TYPE_PASS_ODDS; m_nWager = w; m_nPoint = p; }
        void MakeComeBet(int w)                 { m_ecType = BetType::TYPE_COME; m_nWager = w; }
        void MakeComeOddsBet(int w, int p)      { m_ecType = BetType::TYPE_COME_ODDS; m_nWager = w; m_nPoint = p; }
        void MakeDontPassBet(int w)             { m_ecType = BetType::TYPE_DONT_PASS; m_nWager = w; }
        void MakeDontPassOddsBet(int w, int p)  { m_ecType = BetType::TYPE_DONT_PASS_ODDS; m_nWager = w; m_nPoint = p; }
        void MakeDontComeBet(int w)             { m_ecType = BetType::TYPE_DONT_COME; m_nWager = w; }
        void MakeDontComeOddsBet(int w, int p)  { m_ecType = BetType::TYPE_DONT_COME_ODDS; m_nWager = w; m_nPoint = p; }
        void MakePlaceBet(int w, int p)         { m_ecType = BetType::TYPE_PLACE; m_nWager = w; m_nPoint = p; }
        void MakeFieldBet(int w)                { m_ecType = BetType::TYPE_FIELD; m_nWager = w; }
        void MakeBig6Bet(int w)                 { m_ecType = BetType::TYPE_BIG_6; m_nWager = w; }
        void MakeBig8Bet(int w)                 { m_ecType = BetType::TYPE_BIG_8; m_nWager = w; }
        void MakeAny7Bet(int w)                 { m_ecType = BetType::TYPE_ANY_7; m_nWager = w; }
        void MakeAnyCrapsBet(int w)             { m_ecType = BetType::TYPE_ANY_CRAPS; m_nWager = w;}

        // Return the bet's wager amount
        int Wager() const                       { return (m_nWager); }
        // Set and return the point for the bet (Pass, Don't Pass, Place)
        void SetPoint(int p)                    { m_nPoint = p; }
        int  Point() const                      { return (m_nPoint); }
        // Return whether the bet is on the come out
        bool OnTheComeOut() const               { return (m_nPoint == 0); }
        // Set and return whether the bet has an associated odds bet
        bool IsOddsBetMade() const              { return (m_bOddsBetMade); }
        void SetOddsBetMade()                   { m_bOddsBetMade = true; }
        // Calulate the payoff
        int  CalculatePayoff();

    private:
        // Set bet defaults
        BetType m_ecType = BetType::TYPE_NOBET;
        int m_nWager               = 0;
        int m_nPoint               = 0;
        bool m_bOddsBetMade        = false;
        bool m_bComeOddsAreWorking = false;
};

#endif // BET_H
