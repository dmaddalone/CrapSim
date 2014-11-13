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
 * \brief The Strategy class represents a craps player strategy.
 *
 */

#ifndef STRATEGY_H
#define STRATEGY_H

#include <algorithm>
#include <cassert>
#include <climits>
#include <string>
#include <list>
#include <vector>
#include "Table.h"
#include "Bet.h"
#include "Money.h"
class StrategyTracker;
#include "StrategyTracker.h"

// Used to identify the odds progression method
enum class OddsProgressionMethod
{
    ARITHMETIC,
    GEOMETRIC
};

class Strategy
{
    public:
        Strategy(std::string sName, std::string sDesc, int nInitBank, int nStdWager, bool TrackeResults);
        ~Strategy();
        // Set and return name and description
        void SetName(std::string sN)        { m_sName.assign(sN); }
        std::string Name() const            { return (m_sName); }
        void SetDescription(std::string sD) { m_sDescription.assign(sD); }
        std::string Description() const     { return (m_sDescription); }
        // Set and return the statndard (default) wager.  Is equal to one unit.
        void SetStandardWager(int i)        { assert( i > 0); m_nStandardWager = m_nWager = i; }
        int  StandardWager()                { return (m_nStandardWager); }
        // Used to flag use full payoff wager versus a wager that may not payoff fully
        void SetFullWager(bool b)           { m_bFullWager = b; }
        // Set whether we are using a type of bet (e.g., Pass) or the number
        // of bets used (e.g., Come bets)
        void SetPassBet(int i)              { assert((i == 0) || (i == 1)); m_nNumberOfPassBetsAllowed = i; } // TODO: change these asserts to run time checks
        void SetNumberOfComeBets(int i)     { assert((i >= 0) && (i <= 6)); m_nNumberOfComeBetsAllowed = i; }
        void SetDontPassBet(int i)          { assert((i == 0) || (i == 1)); m_nNumberOfDontPassBetsAllowed = i; }
        void SetNumberOfDontComeBets(int i) { assert((i >= 0) && (i <= 6)); m_nNumberOfDontComeBetsAllowed = i; }
        void SetNumberOfPlaceBets(int i)    { assert((i >= 0) && (i <= 6)); m_nNumberOfPlaceBetsAllowed = i; }
        // Set Place bet specifics
        void SetNumberOfPlaceBetsMadeAtOnce(int i) { assert((i >= 0) && (i <= 6)); m_nNumberOfPlaceBetsMadeAtOnce = i; }
        void SetPlaceAfterCome(bool b)      { m_bPlaceAfterCome = b; }
        void SetPlacePreferred(int i)       { assert ((i == 4) || (i == 5) || (i ==6) || (i == 8) ||(i == 9) || (i == 10)); m_nPreferredPlaceBet = i; }
        void SetPlaceBetUnits(int i)        { assert (i >= 1); m_nPlaceBetUnits = i; }
        // Set Field bet specifics
        void SetFieldBetAllowed(bool b)     { m_bFieldBetsAllowed = b; }
        void SetFieldBetUnits(int i)        { assert (i >= 1); m_nFieldBetUnits = i; }
        // Set Big 6 or 8 bet specifics
        void SetBig6BetAllowed(bool b)      { m_bBig6BetAllowed = b; }
        void SetBig8BetAllowed(bool b)      { m_bBig8BetAllowed = b; }
        // Set Any 7 bet specifics
        void SetAny7BetAllowed(bool b)      { m_bAny7BetAllowed = b; }
        // Set Any Craps bet specifics
        void SetAnyCrapsBetAllowed(bool b)  { m_bAnyCrapsBetAllowed = b; }
        // Set standard (default) odds for Pass and Don't Pass bets
        void SetStandardOdds(float f)       { assert( f >= 1.0); m_fStandardOdds = m_fOdds = f; }
        // Set significant winnings multiple and absolute figures (see Money.h)
        void SetSignificantWinningsMultiple(float f) { m_cMoney.SetSignificantWinningsMultiple(f); }
        void SetSignificantWinnings(int i)           { m_cMoney.SetSignificantWinnings(i); }
        // Default betting strategies
        void SetElementary();
        void SetConservative();
        void SetConventional();
        void SetAggressive();
        // Odds methods
        void SetComeOddsWorking(bool b)     { m_bComeOddsWorking = b; }
        void UseOddsProgression(bool b)     { m_bOddsProgression = b; }
        void IncreaseOdds()                 { if (IsArithmeticOddsProgression()) m_fOdds += 1; else m_fOdds *= 2; }
        void ResetOdds()                    { m_fOdds = m_fStandardOdds; }
        void SetOddsProgressionMethodArithmetic() { m_ecOddsProgressionMethod = OddsProgressionMethod::ARITHMETIC; }
        void SetOddsProgressionMethodGeometric()  { m_ecOddsProgressionMethod = OddsProgressionMethod::GEOMETRIC; }
        // Used before simulations are run to ensure that Strategies fit to
        // Table settings
        void SanityCheck(const Table &cTable);

        // Main drivers of the Strategy
        void MakeBets(const Table &cTable);
        void ResolveBets(const Table &cTable, const Dice &cDice);
        bool StillPlaying() const;
        // Return Strategy state
        int   Bankroll() const      { return m_cMoney.Bankroll(); }
        int   Wager() const         { return m_nWager; }
        float Odds() const          { return m_fOdds; }
        // Update stats
        void  UpdateStatistics();
        // Used before a new simulation run
        void  Reset();
        // Report a Strategy's basic settings
        void  Muster();
        // Report summary of Strategy's results over the Simulation
        void  Report();

    private:
        // Make different types of bets
        void MakePassBet(const Table &cTable);
        void MakeComeBet(const Table &cTable);
        void MakeDontPassBet(const Table &cTable);
        void MakeDontComeBet(const Table &cTable);
        void MakeOddsBet(const Table &cTable);
        void MakeFieldBet();
        void MakeBigBet();
        void MakeAny7Bet();
        void MakeAnyCrapsBet();
        // Set wager to an amount that creates a full payoff
        int  OddsBetFullPayoffWager(const int  nWager, const int nPointNumber);

        // Make appropriate Place bets
        void MakePlaceBets(const Table &cTable);
        // Maake a Place bet
        void MakePlaceBet();
        // Modify Place bet wager to create maximum payoff
        int  PlaceBetFullPayoffWager(const int nPlaceNumber);
        // Container for Place bets
        std::map<int, bool>  m_mPlaceBets { {4, false}, {5, false}, {6, false}, {8, false}, {9, false}, {10, false} };
        // Returns next Place bet number
        int  PlaceBetNumber();

        // Resolve bets
        void ResolvePass(const Dice &cDice);
        void ResolvePassOdds(const Table &cTable, const Dice &cDice);
        void ResolveDontPass(const Dice &cDice);
        void ResolveDontPassOdds(const Table &cTable, const Dice &cDice);
        void ResolveCome(const Dice &cDice);
        void ResolveComeOdds(const Table &cTable, const Dice &cDice);
        void ResolveDontCome(const Dice &cDice);
        void ResolveDontComeOdds(const Dice &cDice);
        void ResolvePlace(const Table &cTable, const Dice &cDice);
        void ResolveField(const Dice &cDice);
        void ResolveBig(const Dice &cDice);
        void ResolveAny7(const Dice &cDice);
        void ResolveAnyCraps(const Dice &cDice);

        // Check to see if a current bet covers 6 or 8
        bool SixOrEightCovered();
        // Checks to see if the odds progression method is Arithmetic
        bool IsArithmeticOddsProgression() { return (m_ecOddsProgressionMethod == OddsProgressionMethod::ARITHMETIC); }


        std::string m_sName;
        std::string m_sDescription;

        // Money class to track bankroll
        Money m_cMoney;

        // StrategyTracker class to provide detailed (roll-by-roll) records of
        // a Strategy.  Used for debugging.  Recommend to a use in conjunction
        // a single simulation run.
        StrategyTracker* m_pcStrategyTracker;

        // Set Strategy defaults
        int m_nNumberOfPassBetsAllowed      = 0;
        int m_nNumberOfComeBetsAllowed      = 0;
        int m_nNumberOfDontPassBetsAllowed  = 0;
        int m_nNumberOfDontComeBetsAllowed  = 0;
        bool m_bFieldBetsAllowed            = false;
        int m_nFieldBetUnits                = 1;
        int m_nNumberOfPassBetsMade         = 0;
        int m_nNumberOfComeBetsMade         = 0;
        int m_nNumberOfDontPassBetsMade     = 0;
        int m_nNumberOfDontComeBetsMade     = 0;
        int m_nNumberOfPlaceBetsAllowed     = 0;
        int m_nNumberOfPlaceBetsMade        = 0;
        int m_nNumberOfPlaceBetsMadeAtOnce  = 0;
        bool m_bPlaceAfterCome              = false;
        int m_nPlaceBetUnits                = 1;
        int m_bBig6BetAllowed               = false;
        int m_bBig8BetAllowed               = false;
        bool m_bAny7BetAllowed              = false;
        bool m_bAnyCrapsBetAllowed          = false;
        int m_nStandardWager                = 0;
        int m_nWager                        = 0;
        bool m_bFullWager                   = false;
        float m_fStandardOdds               = 1.0;
        float m_fOdds                       = 1.0;
        bool  m_bComeOddsWorking            = false;
        bool  m_bOddsProgression            = false;
        OddsProgressionMethod m_ecOddsProgressionMethod = OddsProgressionMethod::ARITHMETIC;
        int m_nPreferredPlaceBet            = 8;

        // Set counters to zero
        int m_nTimesStrategyRun             = 0;
        int m_nTimesStrategyWon             = 0;
        int m_nTimesStrategyLost            = 0;
        int m_nNumberOfRolls                = 0;
        int m_nWinRollsMin                  = INT_MAX;
        int m_nWinRollsMax                  = INT_MIN;
        int m_nWinRollsTotal                = 0;
        int m_nLossRollsMin                 = INT_MAX;
        int m_nLossRollsMax                 = INT_MIN;
        int m_nLossRollsTotal               = 0;

        // Don't track results by default
        bool m_bTrackResults                = false;

        // Container for bets
        std::list<Bet>       m_lBets;
};

#endif // STRATEGY_H
