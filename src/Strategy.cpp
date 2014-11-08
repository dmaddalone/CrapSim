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

#include "Strategy.h"

#include <fstream>
#include <iostream>
#include <iomanip>

Strategy::Strategy(std::string sName, std::string sDesc, int nInitiBank, int nStdWager, bool bTrackResults) //: m_pcStrategyTracker(this)
{
    m_sName.assign(sName);
    m_sDescription.assign(sDesc);

    m_cMoney.Initialize(nInitiBank);

    assert(nStdWager > 0);
    m_nStandardWager = m_nWager = nStdWager;

    m_bTrackResults = bTrackResults;
    if (m_bTrackResults) m_pcStrategyTracker = new StrategyTracker(this);
}

Strategy::~Strategy()
{
    if (m_bTrackResults) delete m_pcStrategyTracker;
    m_pcStrategyTracker = 0;
}

void Strategy::SetElementary()
{
    m_nNumberOfPassBetsAllowed     = 1;
    m_nNumberOfComeBetsAllowed     = 0;
    m_nNumberOfPlaceBetsAllowed    = 0;
    m_fStandardOdds                = 1.0;

    if (m_sName.empty()) SetName("Elementary");
    if (m_sDescription.empty()) SetDescription("Pass only, single odds to start");
}

void Strategy::SetConservative()
{
    m_nNumberOfPassBetsAllowed     = 1;
    m_nNumberOfComeBetsAllowed     = 1;
    m_nNumberOfPlaceBetsAllowed    = 0;
    m_fStandardOdds                = 1.0;

    if (m_sName.empty()) SetName("Conservative");
    if (m_sDescription.empty()) SetDescription("Pass and one Come, single odds to start");
}

void Strategy::SetConventional()
{
    m_nNumberOfPassBetsAllowed     = 1;
    m_nNumberOfComeBetsAllowed     = 2;
    m_nNumberOfPlaceBetsAllowed    = 0;
    m_fStandardOdds                = 1.0;

    if (m_sName.empty()) SetName("Conventional");
    if (m_sDescription.empty()) SetDescription("Pass and two Comes, single odds to start");
}

void Strategy::SetAggressive()
{
    m_nNumberOfPassBetsAllowed     = 1;
    m_nNumberOfComeBetsAllowed     = 2;
    m_nNumberOfPlaceBetsAllowed    = 1;
    m_fStandardOdds                = 2.0;

    if (m_sName.empty()) SetName("Aggressive");
    if (m_sDescription.empty()) SetDescription("Pass and either: 1) three Comes or 2) two Comes and one Place , double odds to start");
}

void Strategy::SanityCheck(const Table &cTable)
{
    // Ensure that Strategy's standard wager is no less than Table minimum and no more than Table maximum
    if (m_nStandardWager < cTable.MinimumBet())
    {
        std::cout << "Changing Strategy " << m_sName << " standard wager to table minimum [" << m_nStandardWager << " to " << cTable.MinimumBet() << "]" << std::endl;
        m_nStandardWager = m_nWager = cTable.MinimumBet();
    }
    if (m_nStandardWager > cTable.MaximumBet())
    {
        std::cout << "Changing Strategy " << m_sName << " standard wager to table minimum [" << m_nStandardWager << " to " << cTable.MaximumBet() << "]" << std::endl;
        m_nStandardWager = m_nWager = cTable.MaximumBet();
    }
    // Ensure that Strategy's number of place bets at once is less than or equal to the number of place bets allowed
    if (m_nNumberOfPlaceBetsMadeAtOnce > m_nNumberOfPlaceBetsAllowed)
    {
        std::cout << "Changing Strategy " << m_sName << " number of place bets at once to number of place bets allowed [" << m_nNumberOfPlaceBetsMadeAtOnce << " to " << m_nNumberOfPlaceBetsAllowed << "]" << std::endl;
        m_nNumberOfPlaceBetsMadeAtOnce = m_nNumberOfPlaceBetsAllowed;
    }

    // Ensure that Strategy's number of place bets at once is less than or equal to the number of place bets allowed
    if ((m_nNumberOfPlaceBetsMadeAtOnce <= 0) && (m_nNumberOfPlaceBetsAllowed > 0))
    {
        std::cout << "Changing Strategy " << m_sName << " number of place bets made at once to one [" << m_nNumberOfPlaceBetsMadeAtOnce << " to " << 1 << "]" << std::endl;
        m_nNumberOfPlaceBetsMadeAtOnce = 1;
    }

    // TODO: if not bets selected, discard strategy
}

void Strategy::MakeBets(const Table &cTable)
{
    if (StillPlaying())
    {
        // If tracking results, start a new record, which updates bankroll, and update table stats
        if (m_bTrackResults) m_pcStrategyTracker->RecordNew(this, cTable);

        MakePassBet(cTable);
        MakeComeBet(cTable);

        MakeDontPassBet(cTable);
        MakeDontComeBet(cTable);

        MakeOddsBet(cTable);

        MakePlaceBets(cTable);

        MakeFieldBet();

        MakeBigBet();

        // If tracking results, capture bankroll post bets
        if (m_bTrackResults) m_pcStrategyTracker->RecordBetsBeforeRoll(this, m_lBets);
    }
}

void Strategy::ResolveBets(const Table &cTable, const Dice &cDice)
{
    if (StillPlaying())
    {
        // Used for Odds Progression
        int nStartingBankroll = m_cMoney.Bankroll();

        // Increment number of rolls
        m_nNumberOfRolls++;

        // Resolve all bets
        ResolvePass(cDice);
        ResolvePassOdds(cTable, cDice);

        ResolveCome(cDice);
        ResolveComeOdds(cTable, cDice);

        ResolveDontPass(cDice);
        ResolveDontPassOdds(cTable, cDice);

        ResolveDontCome(cDice);
        ResolveDontComeOdds(cDice);

        ResolvePlace(cTable, cDice);

        ResolveField(cDice);

        ResolveBig(cDice);

        if (m_bOddsProgression)
        {
            // If using Odds Porgression, and bankroll has increased, increase Odds
            if (m_cMoney.Bankroll() > nStartingBankroll)
                IncreaseOdds();
            // Else reset odds
            else
                ResetOdds();
        }

        // If tracking results, record ending bankroll and post results
        if (m_bTrackResults) m_pcStrategyTracker->RecordBetsAfterRoll(this, m_lBets, cDice.RollValue());
        if (m_bTrackResults) m_pcStrategyTracker->Post();
    }
}

void Strategy::MakePassBet(const Table &cTable)
{
    // If money is not available to bet, do not make a bet
    if (m_nWager > m_cMoney.Bankroll()) return;

    if (cTable.IsComingOutRoll())
    {
        if (m_nNumberOfPassBetsMade < m_nNumberOfPassBetsAllowed)
        {
            Bet cBet;
            cBet.MakePassBet(m_nWager);
            m_cMoney.Decrement(m_nWager);
            m_nNumberOfPassBetsMade++;
            m_lBets.push_back(cBet);
        }
    }
}

void Strategy::MakeDontPassBet(const Table &cTable)
{
    // If money is not available to bet, do not make a bet
    if (m_nWager > m_cMoney.Bankroll()) return;

    if (cTable.IsComingOutRoll())
    {
        if (m_nNumberOfDontPassBetsMade < m_nNumberOfDontPassBetsAllowed)
        {
            Bet cBet;
            cBet.MakeDontPassBet(m_nWager);
            m_cMoney.Decrement(m_nWager);
            m_nNumberOfDontPassBetsMade++;
            m_lBets.push_back(cBet);
        }
    }
}

void Strategy::MakeComeBet(const Table &cTable)
{
    // If money is not available to bet, do not make a bet
    if (m_nWager > m_cMoney.Bankroll()) return;

    if (!cTable.IsComingOutRoll())
    {
        if (m_nNumberOfComeBetsMade < m_nNumberOfComeBetsAllowed)
        {
            Bet cBet;
            cBet.MakeComeBet(m_nWager);
            m_cMoney.Decrement(m_nWager);
            m_nNumberOfComeBetsMade++;
            m_lBets.push_back(cBet);
        }
    }
}

void Strategy::MakeDontComeBet(const Table &cTable)
{
    // If money is not available to bet, do not make a bet
    if (m_nWager > m_cMoney.Bankroll()) return;

    if (!cTable.IsComingOutRoll())
    {
        if (m_nNumberOfDontComeBetsMade < m_nNumberOfDontComeBetsAllowed)
        {
            Bet cBet;
            cBet.MakeDontComeBet(m_nWager);
            m_cMoney.Decrement(m_nWager);
            m_nNumberOfDontComeBetsMade++;
            m_lBets.push_back(cBet);
        }
    }
}

void Strategy::MakeOddsBet(const Table &cTable)
{
    // If money is not available to bet, do not make a bet
    if (m_nWager > m_cMoney.Bankroll()) return;

    for (std::list<Bet>::iterator it = m_lBets.begin();it != m_lBets.end(); it++)
    {
        // Pass Bet
        if (it->IsPassBet())
        {
            if (!it->OnTheComeOut())
            {
                if (!it->IsOddsBetMade())          // Pass Bet without Odds Bet?
                {
                    // Calculate maximum odds wager, based on Strategy and Table type
                    int nWager = it->Wager() * std::min(cTable.GetMaxOdds(it->Point()), m_fOdds);

                    // Check for Full Wager; if set, update nWager
                    if (m_bFullWager) nWager = OddsBetFullPayoffWager(nWager, it->Point());

                    Bet cBet;
                    cBet.MakePassOddsBet(nWager, it->Point());

                    it->SetOddsBetMade();

                    m_cMoney.Decrement(nWager);
                    m_lBets.push_back(cBet);
                }
            }
        }

        // Dont Pass Bet
        if (it->IsDontPassBet())
        {
            if (!it->OnTheComeOut())
            {
                if (!it->IsOddsBetMade())          // Dont Pass Bet without Odds Bet?
                {
                    // Calculate maximum odds wager, based on Strategy and Table type
                    int nWager = it->Wager() * std::min(cTable.GetMaxOdds(it->Point()), m_fOdds);
                    // Check for Full Wager; if set, update nWager
                    if (m_bFullWager) nWager = OddsBetFullPayoffWager(nWager, it->Point());

                    Bet cBet;
                    // First, create Bet as Pass Odds
                    cBet.MakePassOddsBet(nWager, it->Point());
                    // Second, calculate payoff and use as wager
                    nWager = cBet.CalculatePayoff();
                    // Third, change bet to Dont Pass Odds
                    cBet.MakeDontPassOddsBet(nWager, it->Point());

                    it->SetOddsBetMade();

                    m_cMoney.Decrement(nWager);
                    m_lBets.push_back(cBet);
                }
            }
        }

        // Come Bet
        if (it->IsComeBet())
        {
            if (!it->OnTheComeOut())
            {
                if (!it->IsOddsBetMade())             // Come Bet without Odds Bet?
                {
                    // Calculate maximum odds wager, based on Strategy and Table type
                    int nWager = it->Wager() * std::min(cTable.GetMaxOdds(it->Point()), m_fOdds);
                    // Check for Full Wager; if set, update nWager
                    if (m_bFullWager) nWager = OddsBetFullPayoffWager(nWager, it->Point());

                    Bet cBet;
                    cBet.MakeComeOddsBet(nWager, it->Point());

                    // Set whether odds are working for the Come Bet on the Table come out
                    cBet.SetComeOddsAreWorking(m_bComeOddsWorking);

                    it->SetOddsBetMade();

                    m_cMoney.Decrement(nWager);
                    m_lBets.push_back(cBet);
                }
            }

        }

        // Dont Come Bet
        if (it->IsDontComeBet())
        {
            if (!it->OnTheComeOut())
            {
                if (!it->IsOddsBetMade())          // Dont Pass Bet without Odds Bet?
                {
                    // Calculate maximum odds wager, based on Strategy and Table type
                    int nWager = it->Wager() * std::min(cTable.GetMaxOdds(it->Point()), m_fOdds);
                    // Check for Full Wager; if set, update nWager
                    if (m_bFullWager) nWager = OddsBetFullPayoffWager(nWager, it->Point());

                    Bet cBet;
                    // First, create Bet as Come Odds
                    cBet.MakeComeOddsBet(nWager, it->Point());
                    // Second, calculate payoff and use as wager
                    nWager = cBet.CalculatePayoff();
                    // Third, change bet to Dont Pass Odds
                    cBet.MakeDontComeOddsBet(nWager, it->Point());

                    it->SetOddsBetMade();

                    m_cMoney.Decrement(nWager);
                    m_lBets.push_back(cBet);
                }
            }
        }
    }
}

int Strategy::OddsBetFullPayoffWager(const int nWager, const int nPointNumber)
{
    int nModulo = 0;
    int nMultiple = 0;

    switch (nPointNumber)
    {
        case 4:
        case 10:
            nMultiple = 1;
            break;
        case 5:
        case 9:
            nMultiple = 2;
            break;
        case 6:
        case 8:
            nMultiple = 5;
            break;
        default:
            break;
    }

    nModulo = nWager % nMultiple;
    if (nModulo != 0)
    {
        int nNewWager = nWager - nModulo;
        if (nNewWager < m_cMoney.Bankroll())
            return (nNewWager);
    }

    return (nWager);
}

void Strategy::MakePlaceBets(const Table &cTable)
{
    // If money is not available to bet, do not make a bet
    if (m_nWager > m_cMoney.Bankroll()) return;

    // If we make Place bets only after Come bets are exhausted
    if (m_bPlaceAfterCome)
    {
        // If number of Come bets made equals number allowed
        if (m_nNumberOfComeBetsMade == m_nNumberOfComeBetsAllowed)
        {
            // If the table is not coming out and number of place bets made is less than number of allowed
            if (!cTable.IsComingOutRoll() && (m_nNumberOfPlaceBetsMade < m_nNumberOfPlaceBetsAllowed))
            {
                // If 6 or 8 are already covered by a bet, make another come bet instead of a place bet
                if (SixOrEightCovered())
                {
                    Bet cBet;
                    cBet.MakeComeBet(m_nWager);
                    m_cMoney.Decrement(m_nWager);
                    m_nNumberOfComeBetsMade++;
                    m_lBets.push_back(cBet);
                }
                // Make a Place bet // TODO: ensure wager is multuple of 5 for 4, 5, 9, 10 and a multiple of 6 got 6 and 8
                else
                {
                    MakePlaceBet();
                }
            }
        }
    }
    // Else make place bets regardless of Come bets, but check table for coming out roll and the number of Place bets made is less than number allowed
    else if (!cTable.IsComingOutRoll()) //&& (m_nNumberOfPlaceBetsMade < m_nNumberOfPlaceBetsAllowed))
    {
        // Count the number of Place bets made on this turn
        int nNumberOfPlaceBetsMadeThisTurn = 0;

        // Make Place bets up to allowed limit
        while(m_nNumberOfPlaceBetsMade < m_nNumberOfPlaceBetsAllowed)
        {
            MakePlaceBet();

            nNumberOfPlaceBetsMadeThisTurn++;
            // If number of Place bets made this turn equals number alloed at one time, break;
            if (nNumberOfPlaceBetsMadeThisTurn == m_nNumberOfPlaceBetsMadeAtOnce)
                break;
        }
    }
}

void Strategy::MakePlaceBet()
{
    // Set wager to number of Place bet units
    int nWager = m_nWager * m_nPlaceBetUnits;
    // If number of PLaceBet units is greater than the current bankroll, revert to one unit (which should have already been checked before calling this method.)
    if (nWager > m_cMoney.Bankroll()) nWager = m_nWager;

    Bet cBet;
    // Get Place bet number
    int nPlaceBetNumber = PlaceBetNumber();
    // Set Place bet number to true (bet  made)
    m_mPlaceBets[nPlaceBetNumber] = true;
    // Check for Full Wager; if set, update nWager
    if (m_bFullWager) nWager = PlaceBetFullPayoffWager(nPlaceBetNumber);
    // Make Place bet, with nWager (versus m_nWager)
    cBet.MakePlaceBet(nWager, nPlaceBetNumber);
    // Reduce bankroll by wager amounts
    m_cMoney.Decrement(nWager);
    // Increment the number of Place bets made
    m_nNumberOfPlaceBetsMade++;

    m_lBets.push_back(cBet);
}

int Strategy::PlaceBetNumber()
{
    if (m_mPlaceBets[m_nPreferredPlaceBet] == false) return (m_nPreferredPlaceBet);
    if (m_mPlaceBets[8]  == false) return (8);
    if (m_mPlaceBets[6]  == false) return (6);
    if (m_mPlaceBets[5]  == false) return (5);
    if (m_mPlaceBets[9]  == false) return (9);
    if (m_mPlaceBets[4]  == false) return (4);
    if (m_mPlaceBets[10] == false) return (10);

    return (-1);
}

int Strategy::PlaceBetFullPayoffWager(const int nPlaceNumber)
{
    int nModulo = 0;
    int nMultiple = 0;

    switch (nPlaceNumber)
    {
        case 4:
        case 5:
        case 9:
        case 10:
            nMultiple = 5;
            break;
        case 6:
        case 8:
            nMultiple = 6;
            break;
        default:
            break;
    }

    nModulo = m_nWager % nMultiple;
    if (nModulo != 0)
    {
        int nNewWager = m_nWager + nMultiple - nModulo;
        if (nNewWager < m_cMoney.Bankroll())
            return (nNewWager);
    }

    return (m_nWager);
}

bool Strategy::SixOrEightCovered()
{
    for (std::list<Bet>::iterator it = m_lBets.begin();it != m_lBets.end(); it++)
    {
        if ((it->Point() == 6) || (it->Point() == 8))
            return (true);
    }

    return (false);
}

void Strategy::MakeFieldBet()
{
    // Set wager to number of Field bet units
    int nWager = m_nWager * m_nFieldBetUnits;
    // If wager is greater than Bankroll, set wager to current wager
    if (nWager > m_cMoney.Bankroll()) nWager = m_nWager;

    // Check to see that current wager is not greather than bankroll
    if (nWager <= m_cMoney.Bankroll())
    {
        if (m_bFieldBetsAllowed)
        {
            Bet cBet;
            cBet.MakeFieldBet(nWager);
            m_cMoney.Decrement(nWager);
            m_lBets.push_back(cBet);
        }
    }
}

void Strategy::MakeBigBet()
{
    if (m_nWager > m_cMoney.Bankroll()) return;
    if (m_bBig6BetAllowed)
    {
        Bet cBet;
        cBet.MakeBig6Bet(m_nWager);
        m_cMoney.Decrement(m_nWager);
        m_lBets.push_back(cBet);
    }

    if (m_nWager > m_cMoney.Bankroll()) return;
    if (m_bBig8BetAllowed)
    {
        Bet cBet;
        cBet.MakeBig8Bet(m_nWager);
        m_cMoney.Decrement(m_nWager);
        m_lBets.push_back(cBet);
    }
}

void Strategy::ResolvePass(const Dice &cDice)
{
    std::list<Bet>::iterator it = m_lBets.begin();
    while(it != m_lBets.end())
    {
        if (it->IsPassBet())                                    // Pass Bet?
        {
            if (it->OnTheComeOut())                                 // Pass Bet On the Come Out?
            {
                if (cDice.IsCraps())                                    // Pass Bet On the Come Out and a Craps Roll?
                {
                    m_nNumberOfPassBetsMade--;
                    it = m_lBets.erase(it);                                 // Remove Bet

                }
                else if (cDice.IsNatural())                             // PassBet One the Come Out and a Natural Roll?
                {
                    m_cMoney.Increment(it->Wager() + it->CalculatePayoff());// Payoff
                    m_nNumberOfPassBetsMade--;
                    it = m_lBets.erase(it);                                 // Remove Bet
                }
                else                                                    // Pass Bet on the Come Out = Set Point
                {
                    it->SetPoint(cDice.RollValue());                        // Set Point
                    it++;
                }
            }
            else                                                    // Pass Bet, but not on the Come Out
            {
                if (cDice.IsSeven())                                    // Pass Bet, but not on the Come Out and Seven Roll?
                {
                    m_nNumberOfPassBetsMade--;
                    it = m_lBets.erase(it);                                 // Remove Bet
                }
                else if (it->Point() == cDice.RollValue())              // Pass Bet, but not on the Come Out and Hit the Point?
                {
                    m_cMoney.Increment(it->Wager() + it->CalculatePayoff());// Payoff
                    m_nNumberOfPassBetsMade--;
                    it = m_lBets.erase(it);                                 // Remove Bet
                }
                else                                                    // Pass Bet, but not on the Come Out = no impact
                {
                    it++;
                }
            }
        }
        else // Not a Pass Bet
        {
            it++;
        }
    }
}

void Strategy::ResolveDontPass(const Dice &cDice)
{
    std::list<Bet>::iterator it = m_lBets.begin();
    while(it != m_lBets.end())
    {
        if (it->IsDontPassBet())                                    // Dont Pass Bet?
        {
            if (it->OnTheComeOut())                                 // Dont Pass Bet On the Come Out?
            {
                if (cDice.IsCraps())                                    // Dont Pass Bet On the Come Out and a Craps Roll?
                {
                    if (!cDice.IsBar())                                     // Dont Pass Bet On the Come Out and a Craps Roll and it's not a bar dice roll
                    {
                        m_cMoney.Increment(it->Wager() + it->CalculatePayoff());// Payoff
                        m_nNumberOfDontPassBetsMade--;
                        it = m_lBets.erase(it);                                 // Remove Bet
                    }
                    else                                                    // Dont Pass Bet On the Come Out and a Craps Roll and it is a bar dice roll
                    {
                        it++;                                                   // Push
                    }
                }
                else if (cDice.IsNatural())                             // Dont Pass Bet One the Come Out and a Natural Roll?
                {
                    m_nNumberOfDontPassBetsMade--;
                    it = m_lBets.erase(it);                                 // Remove Bet
                }
                else                                                    // Dont Pass Bet on the Come Out = Set Point
                {
                    it->SetPoint(cDice.RollValue());                        // Set Point
                    it++;
                }
            }
            else                                                    // Dont Pass Bet, but not on the Come Out
            {
                if (cDice.IsSeven())                                    // Dont Pass Bet, but not on the Come Out and Seven Roll?
                {
                    m_cMoney.Increment(it->Wager() + it->CalculatePayoff());// Payoff
                    m_nNumberOfDontPassBetsMade--;
                    it = m_lBets.erase(it);                                 // Remove Bet
                }
                else if (it->Point() == cDice.RollValue())              // Dont Pass Bet, but not on the Come Out and Hit the Point?
                {
                    m_nNumberOfDontPassBetsMade--;
                    it = m_lBets.erase(it);                                 // Remove Bet
                }
                else                                                    // Dont Pass Bet, but not on the Come Out and no impact
                {
                    it++;
                }
            }
        }
        else // Not a Dont Pass Bet
        {
            it++;
        }
    }
}

void Strategy::ResolveCome(const Dice &cDice)
{
    std::list<Bet>::iterator it = m_lBets.begin();
    while(it != m_lBets.end())
    {
        if (it->IsComeBet())                                    // Come Bet?
        {
            if (it->OnTheComeOut())                                 // Come Bet On the Come Out?
            {
                if (cDice.IsCraps())                                    // Come Bet On the Come Out and a Craps Roll?
                {
                    m_nNumberOfComeBetsMade--;
                    it = m_lBets.erase(it);                                 // Remove Bet
                }
                else if (cDice.IsNatural())                             // Come Bet On the Come Out and a Natural Roll?
                {
                    m_cMoney.Increment(it->Wager() + it->CalculatePayoff());// Payoff
                    m_nNumberOfComeBetsMade--;
                    it = m_lBets.erase(it);                                 // Remove Bet

                }
                else                                                    // Come Bet On the Come Out = Point Roll
                {
                    it->SetPoint(cDice.RollValue());                        // Set Point
                    it++;
                }
            }
            else                                                    // Come Bet, but not on the Come Out
            {
                if (cDice.IsSeven())                                    // Come Bet, but not on the Come Out and a Seven Roll?
                {
                    m_nNumberOfComeBetsMade--;
                    it = m_lBets.erase(it);                                 // Remove Bet
                }
                else if (it->Point() == cDice.RollValue())           //  Come Bet, but not on the Come Out and Hit the Point?
                {
                    m_cMoney.Increment(it->Wager() + it->CalculatePayoff());// Payoff
                    m_nNumberOfComeBetsMade--;
                    it = m_lBets.erase(it);                                 // Remove Bet
                }
                else                                                //  Come Bet, but not on the Come Ouy, and no impact
                {
                    it++;
                }
            }
        }
        else                                                    // Not a Come Bet
        {
            it++;
        }
    }
}

void Strategy::ResolveDontCome(const Dice &cDice)
{
    std::list<Bet>::iterator it = m_lBets.begin();
    while(it != m_lBets.end())
    {
        if (it->IsDontComeBet())                                    // Dont Come Bet?
        {
            if (it->OnTheComeOut())                                 // Dont Come Bet On the Come Out?
            {
                if (cDice.IsCraps())                                    // Dont Come Bet On the Come Out and a Craps Roll?
                {
                    if (!cDice.IsBar())                                     // It not a bar dice roll
                    {
                        m_cMoney.Increment(it->Wager() + it->CalculatePayoff());// Payoff
                        m_nNumberOfDontComeBetsMade--;
                        it = m_lBets.erase(it);                                 // Remove Bet
                    }
                    else
                    {
                        it++;
                    }
                }
                else if (cDice.IsNatural())                             // Dont Come Bet On the Come Out and a Natural Roll?
                {
                    m_nNumberOfDontComeBetsMade--;
                    it = m_lBets.erase(it);                                 // Remove Bet
                }
                else                                                    // Dont Come Bet On the Come Out = Point Roll
                {
                    it->SetPoint(cDice.RollValue());                        // Set Point
                    it++;
                }
            }
            else                                                    // Dont Come Bet, but not on the Come Out
            {
                if (cDice.IsSeven())                                    // Dont Come Bet, but not on the Come Out and a Seven Roll?
                {
                    m_cMoney.Increment(it->Wager() + it->CalculatePayoff());// Payoff
                    m_nNumberOfDontComeBetsMade--;
                    it = m_lBets.erase(it);                                 // Remove Bet
                }
                else if (it->Point() == cDice.RollValue())           //  Dont Come Bet, but not on the Come Out and Hit the Point?
                {
                    m_nNumberOfDontComeBetsMade--;
                    it = m_lBets.erase(it);                                 // Remove Bet
                }
                else                                                //  Dont Come Bet, but not on the Come Ouy, and no impact
                {
                    it++;
                }
            }
        }
        else                                                    // Not a Come Bet
        {
            it++;
        }
    }
}

void Strategy::ResolvePassOdds(const Table &cTable, const Dice &cDice)
{
    std::list<Bet>::iterator it = m_lBets.begin();
    while(it != m_lBets.end())
    {
        if (it->IsPassOddsBet())                                // Pass Odss Bet?
        {
            assert (cTable.IsComingOutRoll() == false);             // Test - There should not be a Pass Odds bet if this is coming out roll

            if (cDice.IsSeven())                                    // Pass Odds Bets and a Seven Roll?
            {
                it = m_lBets.erase(it);                                 // Remove Bet
            }
            else if (it->Point() == cDice.RollValue())               // Pass Odds Bet and Hit the Point?
            {
                m_cMoney.Increment(it->Wager() + it->CalculatePayoff());// Payoff
                it = m_lBets.erase(it);                                 // Remove Bet
            }
            else                                                     // Pass Odds Bet, but no impact
            {
                it++;
            }
        }
        else                                                    // Not a Pass Odss Bet
        {
            it++;
        }
    }
}

void Strategy::ResolveDontPassOdds(const Table &cTable, const Dice &cDice)
{
    std::list<Bet>::iterator it = m_lBets.begin();
    while(it != m_lBets.end())
    {
        if (it->IsDontPassOddsBet())                                // Dont Pass Odss Bet?
        {
            assert (cTable.IsComingOutRoll() == false);             // Test - There should not be a Pass Odds bet if this is coming out roll

            if (cDice.IsSeven())                                    // Dont Pass Odds Bets and a Seven Roll?
            {
                m_cMoney.Increment(it->Wager() + it->CalculatePayoff());// Payoff
                it = m_lBets.erase(it);                                 // Remove Bet
            }
            else if (it->Point() == cDice.RollValue())               // Dont Pass Odds Bet and Hit the Point?
            {
                it = m_lBets.erase(it);                                 // Remove Bet
            }
            else                                                     // Dont Pass Odds Bet, but no impact
            {
                it++;
            }
        }
        else                                                    // Not a Dont Pass Odss Bet
        {
            it++;
        }
    }
}

void Strategy::ResolveComeOdds(const Table &cTable, const Dice &cDice)
{
    std::list<Bet>::iterator it = m_lBets.begin();
    while(it != m_lBets.end())
    {
        if (it->IsComeOddsBet())                                // Come Odds Bet?
        {
            if (cTable.IsComingOutRoll())                           // Come Odds Bet and Come Out Roll for the Table?
            {
                if (it->ComeOddsAreWorking())                           // Come Odds Bet, Come Out Roll for the Table, and are odds working on the Come Out Roll?
                {
                    if (cDice.IsSeven())                                    // Come Odds Bet, Come Out Roll for the Table, and odss working, and a Seven Roll?
                    {
                        it = m_lBets.erase(it);                                 // Remove Bet
                    }

                    else if (it->Point() == cDice.RollValue())              // Come Odds Bet, Come Out Roll for the Table, and odss working, and Hit the Point?
                    {
                        m_cMoney.Increment(it->Wager() + it->CalculatePayoff());// Payoff
                        it = m_lBets.erase(it);                                 // Remove Bet
                    }
                    else                                                    // Come Odds Bet, Come Out Roll for the Table, and odss working, and no impact
                    {
                        it++;
                    }
                }

                else                                                    // Come Odds Bet, Come Out Roll for the Table, and odds are NOT working on the Come Out Roll?
                {
                    if (cDice.IsSeven())                                    // Come Odds Bet, Come Out Roll for the Table,  odds are NOT working on the Come Out Roll, and Seven Roll?
                    {
                        m_cMoney.Increment(it->Wager());                        // Return wager
                        it = m_lBets.erase(it);                                 // Remove Bet
                    }

                    else if (it->Point() == cDice.RollValue())              // Come Odds Bet, Come Out Roll for the Table, odds are NOT working on the Come Out Roll, and Hit the Point?
                    {
                        m_cMoney.Increment(it->Wager());                        // Return wager
                        it = m_lBets.erase(it);                                 // Remove Bet
                    }
                    else                                                    // Come Odds Bet, Come Out Roll for the Table, odds are NOT working on the Come Out Roll, and no impact
                    {
                        it++;
                    }
                }
            }

            else                                                    // Come Odds Bet, but not on Come Out Roll for the Table
            {
                if (cDice.IsSeven())                                    // Come Odds Bet, but not on Come Out Roll for the Table, and a Seven Roll?
                {
                    it = m_lBets.erase(it);                                 // Remove Bet
                }

                else if (it->Point() == cDice.RollValue())              // Come Odds Bet, but not on Come Out Roll for the Table, and Hit the Point?
                {
                    m_cMoney.Increment(it->Wager() + it->CalculatePayoff());// Payoff
                    it = m_lBets.erase(it);                                 // Remove Bet
                }
                else                                                    // Come Odds Bet, but not on Come Out Roll for the Table, and no impact
                {
                    it++;
                }
            }
        }
        else                                                    // Not a Come Odds Bet
        {
            it++;
        }
    }
}

void Strategy::ResolveDontComeOdds(const Dice &cDice)
{
    std::list<Bet>::iterator it = m_lBets.begin();
    while(it != m_lBets.end())
    {
        if (it->IsDontComeOddsBet())                                // Dont Come Odds Bet?
        {
            if (cDice.IsSeven())                                        // Dont Come Odds Bet and a Seven Roll?
            {
                it = m_lBets.erase(it);                                     // Remove Bet
            }

            else if (it->Point() == cDice.RollValue())                  // Dont Come Odds Bet and Hit the Point?
            {
                m_cMoney.Increment(it->Wager() + it->CalculatePayoff());    // Payoff
                it = m_lBets.erase(it);                                     // Remove Bet
            }
            else                                                        // Dont Come Odds Bet and no impact
            {
                it++;
            }
        }
        else                                                        // Not a Dont Come Odds Bet
        {
            it++;
        }
    }
}

void Strategy::ResolvePlace(const Table &cTable, const Dice &cDice)
{
    std::list<Bet>::iterator it = m_lBets.begin();
    while(it != m_lBets.end())
    {
        if (it->IsPlaceBet())                                   // Place Bet?
        {
            if (!cTable.IsComingOutRoll())                          // Place Bets and not a come out roll for th etable (Place bets are turned off on the come out roll)
            {
                if (cDice.IsSeven())                                    // Place Bet and not a come out roll and a Seven Roll?
                {
                    // Set Place bet number to false (bet not made)
                    m_mPlaceBets[it->Point()] = false;
                    // Decrement the number of Place bets made
                    m_nNumberOfPlaceBetsMade--;
                    // Remove bet
                    it = m_lBets.erase(it);                                 // Remove Bet
                }
                else if (it->Point() == cDice.RollValue())              // Place Bet and not a come out roll and Hit the Point?
                {
                    // Gather winnings
                    m_cMoney.Increment(it->Wager() + it->CalculatePayoff());// Payoff
                    // Set Place bet number to false (bet not made)
                    m_mPlaceBets[it->Point()] = false;
                    // Decrement the number of Place bets made
                    m_nNumberOfPlaceBetsMade--;
                    // Remove bet
                    it = m_lBets.erase(it);                                 // Remove Bet
                }
                else                                                    // Place Bet and not a come out roll and no impact?
                {
                    it++;
                }
            }
            else                                                    // Place Bets and a come out roll for the table
            {
                it++;
            }
        }
        else                                                    // Not a Place Bet
        {
            it++;
        }
    }
}

void Strategy::ResolveField(const Dice &cDice)
{
    std::list<Bet>::iterator it = m_lBets.begin();
    while(it != m_lBets.end())
    {
        if (it->IsFieldBet())                                   // Field Bet?
        {
            if (cDice.IsField())                                    // Field Bet and Field Number Roll?
            {
                it->SetPoint(cDice.RollValue());                        // Set the bet point to last dice roll value to make CalculatePayoff work
                m_cMoney.Increment(it->Wager() + it->CalculatePayoff());// Payoff
            }

            it = m_lBets.erase(it);                                 // Remove Bet
        }
        else                                                    // Not a Field Bet
        {
            it++;
        }
    }
}

void Strategy::ResolveBig(const Dice &cDice)
{
    std::list<Bet>::iterator it = m_lBets.begin();
    while (it != m_lBets.end())
    {
        if (it->IsBig6Bet())                                        // Big 6 Bet?
        {
            if (cDice.IsSeven())                                        // Big 6 Bet and seven roll?
            {
                it = m_lBets.erase(it);                                     // Remove Bet
            }

            else if (cDice.IsSix())                                     // Big 6 Bet and a six roll?
            {
                it->SetPoint(cDice.RollValue());                            // Set the bet point to last dice roll value to make CalculatePayoff work
                m_cMoney.Increment(it->Wager() + it->CalculatePayoff());    // Payoff
                it = m_lBets.erase(it);                                     // Remove Bet
            }

            else
            {
                it++;                                                   // Big 6 Bet, but no impact
            }
        }

        else if (it->IsBig8Bet())                                   // Big 8 Bet?
        {
            if (cDice.IsSeven())                                        // Big 8 Bet and seven roll?
            {
                it = m_lBets.erase(it);                                     // Remove Bet
            }

            if (cDice.IsEight())                                    // Big 8 Bet and an eight roll?
            {
                it->SetPoint(cDice.RollValue());                            // Set the bet point to last dice roll value to make CalculatePayoff work
                m_cMoney.Increment(it->Wager() + it->CalculatePayoff());    // Payoff
                it = m_lBets.erase(it);                                     // Remove Bet
            }

            else
            {
                it++;                                               // Big 8 bet, but no impact
            }
        }
        else                                                    // Not a Big 6 or 8 Bet
        {
            it++;
        }
    }
}

bool Strategy::StillPlaying() const
{
    if ((m_cMoney.Bankroll() < m_nStandardWager) && (m_lBets.empty()))
        return (false);

    if ((m_cMoney.HasSignificantWinnings()) && (m_lBets.empty()))
        return (false);

    return (true);
}

void Strategy::UpdateStatistics()
{
    m_nTimesStrategyRun++;

    if (m_cMoney.Bankroll() > m_nStandardWager)
    {
        m_nTimesStrategyWon++;

        m_nWinRollsTotal += m_nNumberOfRolls;
        if (m_nNumberOfRolls < m_nWinRollsMin) m_nWinRollsMin = m_nNumberOfRolls;
        if (m_nNumberOfRolls > m_nWinRollsMax) m_nWinRollsMax = m_nNumberOfRolls;
    }
    else
    {
        m_nTimesStrategyLost++;

        m_nLossRollsTotal += m_nNumberOfRolls;
        if (m_nNumberOfRolls < m_nLossRollsMin) m_nLossRollsMin = m_nNumberOfRolls;
        if (m_nNumberOfRolls > m_nLossRollsMax) m_nLossRollsMax = m_nNumberOfRolls;
    }
}

void Strategy::Reset()
{
    m_nNumberOfPassBetsMade  = 0;
    m_nNumberOfComeBetsMade  = 0;

    m_nNumberOfDontPassBetsMade  = 0;
    m_nNumberOfDontComeBetsMade  = 0;

    m_nNumberOfPlaceBetsMade = 0;
    m_nNumberOfRolls         = 0;

    m_nWager = m_nStandardWager;
    m_fOdds  = m_fStandardOdds;

    m_cMoney.Reset();
}

void Strategy::Muster()
{
        std::cout << std::setw(20) << std::right << "Name: "                << Name()                                   << std::endl;
        std::cout << std::setw(20) << std::right << "Description: "         << Description()                            << std::endl;

        std::cout << std::setw(20) << std::right << "Pass Bet: "            << m_nNumberOfPassBetsAllowed               <<
                     std::setw(25) << std::right << "  Standard Wager: "    << m_nStandardWager                         << std::endl;

        std::cout << std::setw(20) << std::right << "Come Bets: "           << m_nNumberOfComeBetsAllowed               <<
                     std::setw(25) << std::right << "  Full Wager: "        << std::boolalpha << m_bFullWager           << std::endl;

        std::cout << std::setw(20) << std::right << "Come Odds Working: "   << std::boolalpha << m_bComeOddsWorking     <<
                     std::setw(25) << std::right << "  Initial Bankroll: "  << m_cMoney.InitialBankroll()               << std::endl;

        std::cout << std::setw(20) << std::right << "Dont Pass Bet: "       << m_nNumberOfDontPassBetsAllowed           <<
                     std::setw(25) << std::right << "  Standard Odds: "     << m_fStandardOdds                          << std::endl;

        std::cout << std::setw(20) << std::right << "Dont Come Bets: "      << m_nNumberOfDontComeBetsAllowed           << std::endl;

        std::cout << std::setw(20) << std::right << "Place Bets: "          << m_nNumberOfPlaceBetsAllowed              <<
                     std::setw(25) << std::right << "  Place Preferred: "   << m_nPreferredPlaceBet                     << std::endl;

        std::cout << std::setw(20) << std::right << "Place After Come: "    << std::boolalpha << m_bPlaceAfterCome      << std::endl;
        std::cout << std::setw(20) << std::right << "Place Made At Once: "  << m_nNumberOfPlaceBetsMadeAtOnce           << std::endl;


        std::cout << std::setw(20) << std::right << "Field Bets: "          << std::boolalpha << m_bFieldBetsAllowed    <<
                     std::setw(25) << std::right << "  Track Results: "     << std::boolalpha << m_bTrackResults        << std::endl;

        std::cout << std::setw(20) << std::right << "Sig. Win. Mult.: "     << m_cMoney.SignificantWinningsMultiple()   << std::endl;

        int nSigWin = m_cMoney.SignificantWinnings();
        if (nSigWin > 0)
        {
            std::cout << std::setw(20) << std::right << "Sig. Winnings: "       << m_cMoney.SignificantWinnings()       << std::endl;
        }
        else
        {
            std::cout << std::setw(20) << std::right << "Sig. Winnings: "       << "NOT USED"                           << std::endl;
        }

        std::cout << std::setw(20) << std::right << "Odds Progression: "    << std::boolalpha << m_bOddsProgression     << std::endl;
        std::cout << std::setw(20) << std::right << "Odds Prog. Method: ";
        if (!m_bOddsProgression)
            std::cout << "n/a" << std::endl;
        else
        {
            if (IsArithmeticOddsProgression())
                std::cout << "Arithmetic" << std::endl;
            else
                std::cout << "Geometric" << std::endl;
        }

        std::cout << std::endl;
}

void Strategy::Report()
{
    float fWinPercentage = 0.0;
    int nWinRollsAvg     = 0;
    int nLossRollsAvg    = 0;

    if (m_nTimesStrategyRun > 0)  fWinPercentage = (static_cast<float>(m_nTimesStrategyWon) / static_cast<float>(m_nTimesStrategyRun)) * 100;
    if (m_nTimesStrategyWon > 0)  nWinRollsAvg   = (static_cast<int>(m_nWinRollsTotal / m_nTimesStrategyWon));
    if (m_nTimesStrategyLost > 0) nLossRollsAvg  = (static_cast<int>(m_nLossRollsTotal / m_nTimesStrategyLost));


    std::cout << std::setw(40) << std::right << m_sName << " " <<
                 std::setw(6)  << std::left << m_nTimesStrategyRun <<
                 std::setw(5)  << std::left << m_nTimesStrategyWon <<
                 std::setw(7)  << std::left << m_nTimesStrategyLost <<
                 std::showpoint  << std::setprecision(3) << fWinPercentage <<

                 std::setw(6) << std::right << nWinRollsAvg <<
                 std::setw(5)  << std::right << m_nWinRollsMin <<
                 std::setw(7)  << std::right << m_nWinRollsMax <<
                 std::setw(5)  << std::right << nLossRollsAvg <<
                 std::setw(5)  << std::right << m_nLossRollsMin <<
                 std::setw(7)  << std::right << m_nLossRollsMax <<
                 std::endl;
}
