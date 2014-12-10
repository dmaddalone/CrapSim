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

#include "Wager.h"

/**
  * Construct a Progression.
  *
  * Sets up a maps for wager progression and bet modification methods.
  */

Wager::Wager()
{
    m_mWagerProgressionMethod["NO_METHOD"]  = WagerProgressionMethods::WP_NO_METHOD;
    m_mWagerProgressionMethod["1_3_2_6"]    = WagerProgressionMethods::WP_1_3_2_6;
    m_mWagerProgressionMethod["FIBONACCI"]  = WagerProgressionMethods::WP_FIBONACCI;
    m_mWagerProgressionMethod["MARTINGALE"] = WagerProgressionMethods::WP_MARTINGALE;
    m_mWagerProgressionMethod["PAROLI"]     = WagerProgressionMethods::WP_PAROLI;

    m_mBetModificationMethod["NO_METHOD"]                  = BetModificationMethods::BM_NO_METHOD;
    m_mBetModificationMethod["COLLECT_PRESS_REGRESS"]      = BetModificationMethods::BM_COLLECT_PRESS_REGRESS;
    m_mBetModificationMethod["CLASSIC_REGRESSION"]         = BetModificationMethods::BM_CLASSIC_REGRESSION;
    m_mBetModificationMethod["PRESS_ONCE"]                 = BetModificationMethods::BM_PRESS_ONCE;
    m_mBetModificationMethod["PRESS_TWICE"]                = BetModificationMethods::BM_PRESS_TWICE;
    m_mBetModificationMethod["TAKE_DOWN_AFTER_ONE_HIT"]    = BetModificationMethods::BM_TAKE_DOWN_AFTER_ONE_HIT;
    m_mBetModificationMethod["TAKE_DOWN_AFTER_TWO_HITS"]   = BetModificationMethods::BM_TAKE_DOWN_AFTER_TWO_HITS;
    m_mBetModificationMethod["TAKE_DOWN_AFTER_THREE_HITS"] = BetModificationMethods::BM_TAKE_DOWN_AFTER_THREE_HITS;
}

/**
  * Initialize the wager class.
  *
  * Set standard wager and wager values.
  *
  * \param n The amount used for the standard wager.
  */

void Wager::Initialize(const int nStdWager)
{
    if (nStdWager > 0) m_nStandardWager = m_nWager = nStdWager;
    else throw CrapSimException("Wager::Initialize standard wager less than or equal to 0", std::to_string(nStdWager));
}

/**
  * Set the wager progression method.
  *
  * Based on a passed string, sets the wager progression method.
  * The string is set in the map of strings and WagerProgressionMethod types.
  *
  * \param sMethod The wager progression method, e.g., 1-3-2-5, Martingale.
  *
  */

void Wager::SetWagerProgressionMethod(std::string sMethod)
{
    std::locale loc;
    for (std::string::size_type iii = 0; iii < sMethod.length(); ++iii)
        sMethod[iii] = std::toupper(sMethod[iii], loc);

    std::map<std::string, WagerProgressionMethods>::iterator it = m_mWagerProgressionMethod.find(sMethod);

    if (it != m_mWagerProgressionMethod.end())
    {
        m_ecWagerProgressionMethod = it->second;
    }
    else
    {
        throw CrapSimException("Wager::SetWagerProgressionMethod unknown method", sMethod);
    }
}

/**
  * Return the wager progression method for a shooter in string format.
  *
  * Loops the known wager progression methods and compares to this Strategy's
  * wager progression method.  When found, return correspnding string. The
  * string is set in the map of strings and WagerProgressionMethod types.
  *
  * \return String representing the wager progression method.
  */

std::string Wager::WagerProgressionMethod() const
{
    std::string sMethod("Unknown");

    for (std::map<std::string, WagerProgressionMethods>::const_iterator it = m_mWagerProgressionMethod.begin();
         it != m_mWagerProgressionMethod.end();
         ++it)
    {
        if (m_ecWagerProgressionMethod == it->second)
        {
            sMethod = it->first;
            break;
        }
    }

    return(sMethod);
}

/**
  * Set the bet modification method.
  *
  * Based on a passed string, sets the bet modification method.
  * The string is set in the map of strings and BetModificationMethod types.
  *
  * \param sMethod The wager progression method, e.g., Classic_Regression, Press_Once
  *
  */

void Wager::SetBetModificationMethod(std::string sMethod)
{
    std::locale loc;
    for (std::string::size_type iii = 0; iii < sMethod.length(); ++iii)
        sMethod[iii] = std::toupper(sMethod[iii], loc);

    std::map<std::string, BetModificationMethods>::iterator it = m_mBetModificationMethod.find(sMethod);

    if (it != m_mBetModificationMethod.end())
    {
        m_ecBetModificationMethod = it->second;
    }
    else
    {
        throw CrapSimException("Wager::SetBetModificationMethod unknown method", sMethod);
    }
}

/**
  * Return the bet modification method in string format.
  *
  * Loops the known bet modification methods and compares to this Strategy's
  * bet modification method.  When found, return corresponding string. The
  * string is set in the map of strings and BetModificationMethod types.
  *
  * \return String representing the bet modification method.
  */

std::string Wager::BetModificationMethod() const
{
    std::string sMethod("Unknown");

    for (std::map<std::string, BetModificationMethods>::const_iterator it = m_mBetModificationMethod.begin();
         it != m_mBetModificationMethod.end();
         ++it)
    {
        if (m_ecBetModificationMethod == it->second)
        {
            sMethod = it->first;
            break;
        }
    }

    return(sMethod);
}

/**
  * Record the table limits.
  *
  * Capture the table limits and compare to the standard wager.
  *
  * \param nMin The table minimum.
  * \param nmax The table maximum.
  */

void Wager::SetTableLimits(const int nMin, const int nMax)
{
    // Capture table min and max
    m_nTableMinimum = nMin;
    m_nTableMaximum = nMax;

    // Ensure that Strategy's standard wager is no less than Table minimum and no more than Table maximum
    if (m_nStandardWager < m_nTableMinimum)
        throw CrapSimException("Wager::SetTableLimits standard wager less than table minimum", std::to_string(m_nStandardWager));

    if (m_nStandardWager > m_nTableMaximum)
        throw CrapSimException("Wager::SetTableLimits standard wager greater than table maximum", std::to_string(m_nStandardWager));
}

/**
  * Check and update wager so that it correspods to the environment.
  *
  * Compare to table maximum and current bankroll, and update
  * accordingly.
  *
  * \param nBankroll The current bankroll.
  */

void Wager::CheckWager(const int nBankroll)
{
    // Cannot be more than table maximum
    m_nWager = std::min(m_nWager, m_nTableMaximum);

    // If wager is more than bankroll, set to the max of the standard wager or the bankroll
    if (m_nWager > nBankroll)
        m_nWager = std::max(m_nStandardWager, nBankroll);

    // If wager is still more than bankroll, set it to the bankroll amount
    if (m_nWager > nBankroll)
        m_nWager = nBankroll;
}

/**
  * Update the wager amount for a non-odds bet.
  *
  * Set the wager amount to the standard wager multiplied by the number of
  * units.  Compare to table maximum and current bankroll, and update
  * accordingly.
  *
  * \param nBankroll The current bankroll.
  *
  * \return The wager amount.
  */

int Wager::BetWager(const int nBankroll)
{
    // Set wager to standard wager multiplied by number of units
    m_nWager = m_nStandardWager * m_nUnits;

    CheckWager(nBankroll);

    return (m_nWager);
}

/**
  * Update the wager amount for an odds bet.
  *
  * Set the wager amount to the standard wager multiplied by the number of
  * units.  Compare to table maximum and current bankroll, and update
  * accordingly.
  *
  * \param nBankroll The current bankroll.
  *
  * \return The wager amount.
  */

int Wager::OddsBetWager(const int nBankroll, const int nWager, const int nPoint, const float fOdds)
{
    // Calculate maximum odds wager, based on previous bet's wager and odds
    m_nWager = nWager * fOdds;

    // Check for Full Wager; if set, update nWager
    if (m_bFullWager)
        OddsBetFullPayoffWager(nPoint);

    CheckWager(nBankroll);

    return (m_nWager);
}

/**
  * Update the wager amount for a Place bet with a set number of units.
  *
  * Set the wager amount to the standard wager multiplied by the number of
  * units passed.  Compare to table maximum and current bankroll, and update
  * accordingly.
  *
  * \param nBankroll The current bankroll.
  * \param nUnits The number of units to use.
  * \param nPoint The Place bet number.
  *
  * \return The wager amount.
  */

int Wager::PlaceBetUnitsWager(const int nBankroll, const int nUnits, const int nPoint)
{
    // If nUnits is positive, calculate Place bet based on number of units to bet
    if (nUnits > 0)
    {
        m_nWager = m_nStandardWager * nUnits;
    }
    // Else use m_nUnits
    else
    {
        m_nWager = m_nStandardWager * m_nUnits;
    }

    // Check for Full Wager; if set, update nWager
    if (m_bFullWager)
        PlaceBetFullPayoffWager(nPoint);

    CheckWager(nBankroll);

    return (m_nWager);
}

/**
  * Update the wager amount for a bet with a set number of units.
  *
  * Set the wager amount to the standard wager multiplied by the number of
  * units passed.  Compare to table maximum and current bankroll, and update
  * accordingly.
  *
  * \param nBankroll The current bankroll.
  * \param nUnits The number of units to use.
  *
  * \return The wager amount.
  */

int Wager::BetUnitsWager(const int nBankroll, int nUnits)
{
    if (nUnits < 1)
        nUnits = 1;

    // Calculate maximum odds wager, based on previous bet's wager and odds
    m_nWager = m_nStandardWager * nUnits;

    CheckWager(nBankroll);

    return (m_nWager);
}

/**
  * Calculate odd's bet full payoff wager.
  *
  * Ensure that the wager with the full payoff is made for an odds bet.
  *
  * INI File:
  * FullWager=true|false
  *
  *\param cTable The Table.
  */

void Wager::OddsBetFullPayoffWager(const int nPoint)
{
    int nModulo = 0;
    int nMultiple = 0;

    switch (nPoint)
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

    nModulo = m_nWager % nMultiple;
    if (nModulo != 0)
        m_nWager = m_nWager - nModulo;
}

/**
  * Return full payoff wager for a Place bet.
  *
  * Based on the Place bet number, return the full payoff wager.
  *
  *\param nPlaceNumber The Place number being bet on.
  */

void Wager::PlaceBetFullPayoffWager(const int nPlaceNumber)
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
        m_nWager = m_nWager + nMultiple - nModulo;
}

/**
  * Return the number of units of wagers based on the wager progression method.
  *
  * Depending on the set wager progression method, calls the corresponding
  * wager progression method.  The wager progression methods operate from the
  * Bet class and expect to have a resolved Bet.
  *
  * \param cBet The Bet.
  *
  * \return The number of wager units to bet.
  */

int Wager::WagerUnits(const std::list<Bet>::iterator &it)
{
    switch (m_ecWagerProgressionMethod)
    {
        case WagerProgressionMethods::WP_NO_METHOD:
            return (1);
            break;
        case WagerProgressionMethods::WP_MARTINGALE:
            return (MethodMartingale(it));
            break;
        case WagerProgressionMethods::WP_FIBONACCI:
            return (MethodFibonacci(it));
            break;
        case WagerProgressionMethods::WP_1_3_2_6:
            return (Method1_3_2_6(it));
            break;
        case WagerProgressionMethods::WP_PAROLI:
            return (MethodParoli(it));
            break;
        default:
            throw CrapSimException("Wager::WagerUnits unknown Wager Progression Method");
            break;
    }
}

/**
  * The Martingale method.
  *
  * Establish a standard wager of one unit.  On a lost bet, double the amount
  * of wager units for the next bet.  On a won bet, set the number of units to
  * one.
  *
  * \param cBet The Bet.
  *
  * \return The number of wager units to bet.
  */

int Wager::MethodMartingale(const std::list<Bet>::iterator &it)
{
    if (it->Lost())
        m_nUnits = m_nUnits * 2;
    else
        m_nUnits = 1;

    return (m_nUnits);
}

/**
  * The 1-3-2-6 method.
  *
  * Meant to be used with even money bets, like the Pass line bet.
  *
  * Establish a standard wager of one unit.  If the single unit bet loses,
  * make another single unit bet (-1-).  If it wins, leave the original wager and
  * the winnings, and add another unit for the second bet of three units (-3-).
  *
  * If the the three unit bet loses, go back to a sinlge unit bet.  If it wins,
  * six units are on the table; pull down four, leaving two units for the next
  * bet (-2-).
  *
  * If the two unit bet loses, go back to the single unit bet.  If it wins, a
  * total of four units are on the table; add two units for the next bet (-6-).
  * Whether this six unit bet wins or loses, return to a sinle unit for the next
  * bet.
  *
  * \param cBet The Bet.
  *
  * \return The number of wager units to bet.
  */

int Wager::Method1_3_2_6(const std::list<Bet>::iterator &it)
{
    if (it->Lost())
    {
        m_nUnits = 1;
    }
    else
    {
        switch (m_nUnits)
        {
            case 1:
                m_nUnits = 3;
                break;
            case 3:
                m_nUnits = 2;
                break;
            case 2:
                m_nUnits = 6;
                break;
            case 6:
                m_nUnits = 1;
                break;
            default:
                throw CrapSimException("Progression::Method1_3_2_6 unknown number of units", std::to_string(m_nUnits));
                break;
        }
    }

    return (m_nUnits);
}

/**
  * The Fibonacci method.
  *
  * Meant to be used with even money bets, like the Pass line bet.
  *
  * Establish a standard wager of one unit.
  *
  * If the bet loses, use the next number in the Fibonacci sequence as the
  * number of units for the next bet.
  *
  * If any bet wins, return to a single unit for the next bet.
  *
  * \param cBet The Bet.
  *
  * \return The number of wager units to bet.
  */

int Wager::MethodFibonacci(const std::list<Bet>::iterator &it)
{
    if (it->Won())
    {
        m_nUnits          = 1;
        m_nPreviousUnits1 = 1;
        m_nPreviousUnits2 = 0;
    }

    m_nUnits = m_nPreviousUnits1 + m_nPreviousUnits2;
    m_nPreviousUnits2 = m_nPreviousUnits1;
    m_nPreviousUnits1 = m_nUnits;

    return (m_nUnits);
}

/**
  * The Paroli method.
  *
  * Meant to be used with even money bets, like the Pass line bet.
  *
  * Establish a standard wager of one unit.  If the bet wins, increase
  * unit by one. If any bet loses, return to a single unit for the next bet.
  *
  * \param cBet The Bet.
  *
  * \return The number of wager units to bet.
  */

int Wager::MethodParoli(const std::list<Bet>::iterator &it)
{
    if (it->Lost())
    {
        m_nUnits = 1;
    }
    else
    {
        m_nUnits += 1;
    }

    return (m_nUnits);
}

/**
  * Modify the existing bets.
  *
  * Depending on the set bet modification method, calls the corresponding
  * bet modification method.  The bet modifications methods operate from the
  * Strategy's list of bets.
  *
  * \param cBet The Bet.
  *
  */

bool Wager::ModifyBets(Money &cMoney, const Table &cTable, std::list<Bet> &lBets)
{
    ////if (lBets.empty()) return (false);

    switch (m_ecBetModificationMethod)
    {
        case BetModificationMethods::BM_NO_METHOD:
            return (false);
            break;
        case BetModificationMethods::BM_COLLECT_PRESS_REGRESS:
            return (MethodCollectPressRegress(cMoney, cTable, lBets));
            break;
        case BetModificationMethods::BM_CLASSIC_REGRESSION:
            return (MethodClassicRegression(cMoney, cTable, lBets));
            break;
        case BetModificationMethods::BM_PRESS_ONCE:
            return (MethodPress(cMoney, cTable, lBets, 1));
            break;
        case BetModificationMethods::BM_PRESS_TWICE:
            return (MethodPress(cMoney, cTable, lBets, 2));
            break;
        case BetModificationMethods::BM_TAKE_DOWN_AFTER_ONE_HIT:
            return (MethodTakeDownAfterHits(cMoney, cTable, lBets,1));
            break;
        case BetModificationMethods::BM_TAKE_DOWN_AFTER_TWO_HITS:
            return (MethodTakeDownAfterHits(cMoney, cTable, lBets,2));
            break;
        case BetModificationMethods::BM_TAKE_DOWN_AFTER_THREE_HITS:
            return (MethodTakeDownAfterHits(cMoney, cTable, lBets,3));
            break;
        default:
            throw CrapSimException("Wager::ModifyBets unknown Bet Modification Method");
            break;
    }
}

/**
  * Modifiable Bets Exist
  *
  * Check to see if any modifiable bets exists.
  *
  * \param lBets The list container of bets
  *
  * \return True if modifiable bets exists, false otherwise
  *
  */

bool Wager::ModifiableBetsExist(const std::list<Bet> &lBets) const
{
    if (lBets.empty()) return (false);

    for (std::list<Bet>::const_iterator it = lBets.begin(); it != lBets.end(); ++it)
    {
        if (it->Modifiable()) return (true);
    }

    return (false);
}

/**
  * Bet Modification Setup
  *
  * Reset counter if coming out and no bets are currently laid.
  * Loop through bets and check for a won bet.  If found, set flag to true.
  *
  * \param lBets The list container of bets
  *
  */

void Wager::BetModificationSetup(const Table &cTable, const std::list<Bet> &lBets)
{
    // If table is coming out and no modifiable bets exist, reset counter
    if (cTable.IsComingOutRoll() && !ModifiableBetsExist(lBets))  // This counts all bets  TODO: count only modifiable bets
    {
        m_nBetModCounter = 0;
    }

    // Loop through bets and find a winner
    // If found, increment by one
    m_bWon = false;

    for (std::list<Bet>::const_iterator it = lBets.begin(); it != lBets.end(); ++it)
    {
        if (it->Won())
        {
            m_bWon = true;
            break;
        }
    }
}

/**
  * The Collect, Press, Regress method.
  *
  * After first win, collect winnings and make the same bet.  After second
  * win, press the bet.  After third win, reduce bet to original wager.
  * Usually used with Place bets.
  *
  * The method expects Won bets to have been resolved and winnings already
  * pocketed (aka, Money.Increment() already called.)
  *
  * \param cMoney The bankroll
  * \param cTable The table
  * \param cDice The dice
  * \param lBets The list container of bets
  *
  * \return True if no more bets should be made, false otherwise
  */

bool Wager::MethodCollectPressRegress(Money &cMoney, const Table &cTable, std::list<Bet> &lBets)
{
    BetModificationSetup(cTable, lBets);
    if (m_bWon) ++m_nBetModCounter;

    // If a bet won and counter is 1, a this is the Collect.
    // 1) Loop through bets, find those that are modifiable and have won.
    // 2) Set their state to Unresolved

    if (m_bWon && m_nBetModCounter == 1)
    {
        for (std::list<Bet>::iterator it = lBets.begin(); it != lBets.end(); ++it)
        {
            if (it->Modifiable() && it->Won())
            {
                cMoney.Decrement(it->Wager());
                it->SetUnresolved();
            }
        }
    }

    // If a bet won and counter is 2, this is the Press.
    // 1) Loop through bets, find those that are modifiable and have won.
    // 2) Make a new bet at two times the original wager.
    // 3) Set their state to Unresolved.

    int nNewWager = 0;

    if (m_bWon && m_nBetModCounter == 2)
    {
        for (std::list<Bet>::iterator it = lBets.begin(); it != lBets.end(); ++it)
        {
            if (it->Modifiable()  && it->Won())
            {
                nNewWager = it->Wager() * 2;
                cMoney.Decrement(nNewWager);
                it->SetWager(nNewWager);
                it->SetUnresolved();
            }
        }
    }

    // If a bet won and counter is 3, this is the Regress.
    // 1) Loop through bets, find those that are modifiable and have won.
    // 2) Make a new bet at half the original wager.
    // 3) Set their state to Unresolved.
    // 4) Reset the counter

    if (m_bWon && m_nBetModCounter == 3)
    {
        for (std::list<Bet>::iterator it = lBets.begin(); it != lBets.end(); ++it)
        {
            if (it->Modifiable()  && it->Won())
            {
                nNewWager = it->Wager() / 2;
                cMoney.Decrement(nNewWager);
                it->SetWager(nNewWager);
                it->SetUnresolved();

                m_nBetModCounter = 0;
            }
        }
    }

    return (false);
}

/**
  * The Classic Regression method.
  *
  * After first win, reduce all bet wagers by half.  After second win, take
  * down all bets.  Usually used with Place bets of two units on first bet.
  *
  * The method expects Won bets to have been resolved and winnings already
  * pocketed (aka, Money.Increment() already called.)
  *
  * \param cMoney The bankroll
  * \param cTable The table
  * \param cDice The dice
  * \param lBets The list container of bets
  *
  * \return True if no more bets should be made, false otherwise
  */

bool Wager::MethodClassicRegression(Money &cMoney, const Table &cTable, std::list<Bet> &lBets)
{
    bool bStopMakingBets = false;

    BetModificationSetup(cTable, lBets);
    if (m_bWon) ++m_nBetModCounter;

    // If a bet won and counter is 1, a this is our first regression.
    // 1) Loop through bets, find those that can be taken down but are not
    // lost.
    // 2) Reduce their wager amount by half, if possible
    // 3) Set their state to Unresolved
    int nOldWager = 0;
    int nNewWager = 0;

    if (m_bWon && m_nBetModCounter == 1)
    {
        for (std::list<Bet>::iterator it = lBets.begin(); it != lBets.end(); ++it)
        {
            // If bet is modifiable
            if (it->Modifiable())
            {
                // If bet won or is unresolved
                if (it->Won() || !it->Resolved())
                {
                    // Calculate new wager amount
                    nOldWager = it->Wager();
                    if (nOldWager >= m_nStandardWager * 2)
                    {
                        nNewWager = it->Wager() / 2;
                    }
                    else
                    {
                        nNewWager = m_nStandardWager;
                    }

                    // If won, make a new bet at half the original wager
                    if (it->Won())
                    {
                        it->SetWager(nNewWager);
                        cMoney.Decrement(nNewWager);
                        it->SetUnresolved();
                    }

                    // If not resolved, reduce current wager to half the
                    // original wager
                    else if (!it->Resolved())
                    {
                        it->SetWager(nNewWager);
                        cMoney.Increment(nOldWager - nNewWager);
                        it->SetUnresolved();
                    }
                }
            }
        }
    }

    // If a bet won and counter is 2, this is our second regression.
    // 1) Loop through bets, find those that can be taken down and are not
    // resolved.
    // 2) Reduce their wager to zero
    // 3) Set their state to Returned
    // 4) Return true to stop making further bets, until a new qualification is achieved

    if (m_bWon && m_nBetModCounter == 2)
    {
        for (std::list<Bet>::iterator it = lBets.begin(); it != lBets.end(); ++it)
        {
            if (it->Modifiable())
            {
                if (!it->Resolved())
                {
                    cMoney.Increment(it->Wager());
                    it->SetReturned();
                }
            }
        }

        bStopMakingBets = true;
    }

    return (bStopMakingBets);
}

/**
  * The Press method.
  *
  * Press the bet for a number of times in a row.
  *
  * The method expects Won bets to have been resolved and winnings already
  * pocketed (aka, Money.Increment() already called.)  Usually used with
  * Place bets.
  *
  * \param cMoney The bankroll
  * \param cTable The table
  * \param cDice The dice
  * \param lBets The list container of bets
  * \param nTimes The numbers of times to press
  *
  * \return True if no more bets should be made, false otherwise
  */

bool Wager::MethodPress(Money &cMoney, const Table &cTable, std::list<Bet> &lBets, const int nTimes)
{
    BetModificationSetup(cTable, lBets);
    if (m_bWon) ++m_nBetModCounter;

    // If a bet won and counter is less than or equal to nTimes, Press the bet.
    // 1) Loop through bets, find those that are modifiable and have won.
    // 2) Double their wager
    // 3) Set their state to Unresolved

    int nNewWager = 0;

    if (m_bWon)
    {
        if (m_nBetModCounter <= nTimes)
        {
            for (std::list<Bet>::iterator it = lBets.begin(); it != lBets.end(); ++it)
            {
                if (it->Modifiable() && it->Won())
                {
                    nNewWager = it->Wager() * 2;
                    cMoney.Decrement(nNewWager);
                    it->SetWager(nNewWager);
                    it->SetUnresolved();
                }
            }
        }
        else
        {
            m_nBetModCounter = 0;
        }
    }

    return (false);
}

/**
  * The Take Down After Hits method.
  *
  * Take down bets after a number of wins.
  *
  * The method expects Won bets to have been resolved and winnings already
  * pocketed (aka, Money.Increment() already called.)  Usually used with
  * Place bets.
  *
  * \param cMoney The bankroll
  * \param cTable The table
  * \param cDice The dice
  * \param lBets The list container of bets
  * \param nTimes The numbers of times to press
  *
  * \return True if no more bets should be made, false otherwise
  */

bool Wager::MethodTakeDownAfterHits(Money &cMoney, const Table &cTable, std::list<Bet> &lBets, const int nTimes)
{
    BetModificationSetup(cTable, lBets);
    if (m_bWon) ++m_nBetModCounter;

    // If a bet won and counter is less than or equal to nTimes, Press the bet.
    // 1) Loop through bets, find those that are modifiable and have won.
    // 2) Make a new bet at the same wager.
    // 3) Set their state to Unresolved

    if (m_bWon)
    {
        if (m_nBetModCounter <= nTimes)
        {
            for (std::list<Bet>::iterator it = lBets.begin(); it != lBets.end(); ++it)
            {
                if (it->Modifiable() && it->Won())
                {
                    cMoney.Decrement(it->Wager());
                    it->SetUnresolved();
                }
            }
        }
        // Else remove all modifiable unresolved bets and reset thge counter
        else
        {
            for (std::list<Bet>::iterator it = lBets.begin(); it != lBets.end(); ++it)
            {
                if (it->Modifiable() && !it->Resolved())
                {
                    cMoney.Increment(it->Wager());
                    it->SetReturned();
                }
            }
            m_nBetModCounter = 0;
        }
    }

    return (false);
}


/**
  * Reset the class.
  *
  * Reset values for a new simulation run.
  *
  */

void Wager::Reset()
{
    m_nUnits          = 1;
    m_nPreviousUnits1 = 1;
    m_nPreviousUnits2 = 0;
    m_nBetModCounter  = 0;
    m_bWon            = false;
}
