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
  * Sets up a map of strings and wager progression methods.
  */

Wager::Wager()
{
    m_mWagerProgressionMethod["NO_METHOD"]  = WagerProgressionMethod::WP_NO_METHOD;
    m_mWagerProgressionMethod["1_3_2_6"]    = WagerProgressionMethod::WP_1_3_2_6;
    m_mWagerProgressionMethod["FIBONACCI"]  = WagerProgressionMethod::WP_FIBONACCI;
    m_mWagerProgressionMethod["MARTINGALE"] = WagerProgressionMethod::WP_MARTINGALE;
    m_mWagerProgressionMethod["PAROLI"]     = WagerProgressionMethod::WP_PAROLI;
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
    else throw std::domain_error("Wager::Initialize: standard wager less than or equal to 0");
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
        throw std::domain_error("Wager::SetTableLimits: standard wager less than table minimum");

    if (m_nStandardWager > m_nTableMaximum)
        throw std::domain_error("Wager::SetTableLimits : standard wager greater than table maximum");
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
    // Set wager to standard waager multpled by number of units
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
        m_nWager = m_nWager - nModulo;  // TODO: is this right?
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
        m_nWager = m_nWager + nMultiple - nModulo; // TODO: is this right?
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

void Wager::SetMethod(std::string sMethod)
{
    std::locale loc;
    for (std::string::size_type iii = 0; iii < sMethod.length(); iii++)
        sMethod[iii] = std::toupper(sMethod[iii], loc);

    std::map<std::string, WagerProgressionMethod>::iterator it = m_mWagerProgressionMethod.find(sMethod);

    if (it != m_mWagerProgressionMethod.end())
    {
        m_ecWagerProgressionMethod = it->second;
    }
    else
    {
        throw std::domain_error("Wager::SetMethod: unknown method");
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

std::string Wager::Method() const
{
    std::string sMethod("Unknown");

    for (std::map<std::string, WagerProgressionMethod>::const_iterator it = m_mWagerProgressionMethod.begin(); it != m_mWagerProgressionMethod.end(); it++)
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
        case WagerProgressionMethod::WP_NO_METHOD:
            return (1);
            break;
        case WagerProgressionMethod::WP_MARTINGALE:
            return (MethodMartingale(it));
            break;
        case WagerProgressionMethod::WP_FIBONACCI:
            return (MethodFibonacci(it));
            break;
        case WagerProgressionMethod::WP_1_3_2_6:
            return (Method1_3_2_6(it));
            break;
        case WagerProgressionMethod::WP_PAROLI:
            return (MethodParoli(it));
            break;
        default:
            throw std::domain_error("Progression::Wager: unknown WagerProgressionMethod");
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
                throw std::domain_error("Progression::Method1_3_2_6: unknown number of units");
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
}
