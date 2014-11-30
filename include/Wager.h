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

#ifndef WAGER_H
#define WAGER_H

#include <list>
#include <locale>
#include <map>
#include <string>

#include "CrapSimException.h"
#include "Bet.h"

// Wager progression method
enum class WagerProgressionMethod
{
    WP_NO_METHOD,
    WP_1_3_2_6,
    WP_FIBONACCI,
    WP_MARTINGALE,
    WP_PAROLI
};

class Wager
{
    public:
        Wager();

        void Initialize(const int nStdWager);
        // Set and return the standard (default) wager.  Is equal to one unit.
        int  StandardWager() const { return (m_nStandardWager); }

        // Set table limits
        void SetTableLimits(int nMin, int nMax);

        // Used to flag use full payoff wager versus a wager that may not payoff fully
        void SetFullWager(const bool b)       { m_bFullWager = b; }

        // Call the correct wager progression method and return the number of units for the next bet
        int WagerUnits(const std::list<Bet>::iterator &it);

        // Update the wager amount for a non-Odds bet
        int BetWager(const int nBankroll);

        // Update the wager amount for an Odds bet for full payoff amount
        int OddsBetWager(const int nBankroll, const int nWager, const int nPoint, const float fOdds);

        // Update the wager amount for a bet with a set number of units
        int BetUnitsWager(const int nBankroll, int nUnits);

        // Update the wager amount for a bet with a set number of units
        int PlaceBetUnitsWager(const int nBankroll, const int nUnits, const int nPoint);

        // Set and return the wager progression method
        void SetMethod(std::string sMethod);
        std::string Method() const;

        bool WagerProgressionMethodSet() { return (m_ecWagerProgressionMethod != WagerProgressionMethod::WP_NO_METHOD); }

        // Reset the class - meant to be called before a new Simulation run
        void Reset();


    private:
        // Check wager against environment.
        void CheckWager(const int nBankroll);
        // Set wager to an amount that creates a full payoff.
        void OddsBetFullPayoffWager(const int nPointNumber);
        // Update the wager amount for an Place bet for full payoff amount
        void PlaceBetFullPayoffWager(const int nPlaceNumber);

        //
        // Wager Progression Methods
        //
        // The 1-3-2-6 method
        int Method1_3_2_6(const std::list<Bet>::iterator &it);
        // The Fibonacci method
        int MethodFibonacci(const std::list<Bet>::iterator &it);
        // The Martingale method
        int MethodMartingale(const std::list<Bet>::iterator &it);
        // The Paroli method
        int MethodParoli(const std::list<Bet>::iterator &it);

        //
        // Wager Regression Methods
        //
        // The 2:1 Method
        int Method2_to_1(const std::list<Bet>::iterator &it);

        // Set defaults
        int m_nStandardWager  {0};
        int m_nWager          {0};
        int m_nTableMinimum   {0};
        int m_nTableMaximum   {0};
        bool m_bFullWager     {false};

        WagerProgressionMethod m_ecWagerProgressionMethod = WagerProgressionMethod::WP_NO_METHOD;
        int m_nUnits          {1};
        int m_nPreviousUnits1 {1};
        int m_nPreviousUnits2 {0};

        // Map to associate the strings with the enum values
        std::map<std::string, WagerProgressionMethod> m_mWagerProgressionMethod;
};

#endif // WAGER_H
