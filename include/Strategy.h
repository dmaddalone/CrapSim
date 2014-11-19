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
#include <climits>
#include <string>
#include <list>
#include <stdexcept>
#include <vector>
#include "Table.h"
#include "Bet.h"
#include "Money.h"
#include "QualifiedShooter.h"
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
        void SetStandardWager(int i)        { if (i >= 1) m_nStandardWager = m_nWager = i; throw std::domain_error("Strategy::SetStandardWager"); }
        int  StandardWager()                { return (m_nStandardWager); }
        // Used to flag use full payoff wager versus a wager that may not payoff fully
        void SetFullWager(bool b)           { m_bFullWager = b; }
        // Set whether we are using a type of bet (e.g., Pass) or the number
        // of bets used (e.g., Come bets)
        void SetPassBet(int i)              { if ((i == 0) || (i == 1)) m_nNumberOfPassBetsAllowed = i; else throw std::domain_error("Strategy::SetPassBet"); }
        void SetNumberOfComeBets(int i)     { if ((i >= 0) && (i <= 6)) m_nNumberOfComeBetsAllowed = i; else throw std::domain_error("Strategy::SetNumberOfComeBets"); }
        void SetDontPassBet(int i)          { if ((i == 0) || (i == 1)) m_nNumberOfDontPassBetsAllowed = i; else throw std::domain_error("Strategy::SetDontPassBet: argument"); }
        void SetNumberOfDontComeBets(int i) { if ((i >= 0) && (i <= 6)) m_nNumberOfDontComeBetsAllowed = i; else throw std::domain_error("Strategy::SetNumberOfDontComeBets"); }
        void SetNumberOfPlaceBets(int i)    { if ((i >= 0) && (i <= 6)) m_nNumberOfPlaceBetsAllowed = i; else throw std::domain_error("Strategy::SetNumberOfPlaceBets: argument"); }
        // Set Place bet specifics
        void SetNumberOfPlaceBetsMadeAtOnce(int i) { if ((i >= 0) && (i <= 6)) m_nNumberOfPlaceBetsMadeAtOnce = i; else throw std::domain_error("Strategy::SetNumberOfPlaceBetsMadeAtOnce"); }
        void SetPlaceAfterCome(bool b)      { m_bPlaceAfterCome = b; }
        void SetPlacePreferred(int i)       { if ((i == 4) || (i == 5) || (i ==6) || (i == 8) ||(i == 9) || (i == 10)) m_nPreferredPlaceBet = i; else throw std::domain_error("Strategy::SetPlacePreferred");  }
        void SetPlaceBetUnits(int i)        { if (i >= 1) m_nPlaceBetUnits = i; else throw std::domain_error("Strategy::SetPlaceBetUnits"); }
        // Set one roll bet specifics
        void SetFieldBetAllowed(bool b)     { m_bFieldBetsAllowed = b; }
        void SetFieldBetUnits(int i)        { if (i >= 1) m_nFieldBetUnits = i; else throw std::domain_error("Strategy::SetFieldBetUnits"); }
        void SetAny7BetAllowed(bool b)      { m_bAny7BetAllowed = b; }
        void SetAnyCrapsBetAllowed(bool b)  { m_bAnyCrapsBetAllowed = b; }
        void SetCraps2BetAllowed(bool b)    { m_bCraps2BetAllowed = b; }
        void SetCraps3BetAllowed(bool b)    { m_bCraps3BetAllowed = b; }
        void SetYo11BetAllowed(bool b)      { m_bYo11BetAllowed = b; }
        void SetCraps12BetAllowed(bool b)   { m_bCraps12BetAllowed = b; }
        // Set Big 6 or 8 bet specifics
        void SetBig6BetAllowed(bool b)      { m_bBig6BetAllowed = b; }
        void SetBig8BetAllowed(bool b)      { m_bBig8BetAllowed = b; }
        void SetStandardOdds(float f)       { if ( f >= 1.0) m_fStandardOdds = m_fOdds = f; else throw std::domain_error("Strategy::SetStandardOdds"); }
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
        bool SetOddsProgressionMethod(std::string sMethod);
        //void SetOddsProgressionMethodArithmetic() { m_ecOddsProgressionMethod = OddsProgressionMethod::ARITHMETIC; }
        //void SetOddsProgressionMethodGeometric()  { m_ecOddsProgressionMethod = OddsProgressionMethod::GEOMETRIC; }
        // Qualified Shooter methods
        bool SetQualifiedShooterMethod(std::string);
        void SetQualifiedShooterMethodCount(int n) { m_cQualifiedShooter.SetCount(n); }
        void QualifyTheShooter(const Table &cTable, const Dice &cDice) { m_cQualifiedShooter.QualifyTheShooter(cTable, cDice); }
        bool ShooterQualified()             { return (m_cQualifiedShooter.ShooterQualified()); }
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
        void  Muster() const;
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
        void MakeOneRollBets();
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
        //void ResolvePass(const Dice &cDice);
        bool ResolvePass(std::list<Bet>::iterator &it, const Dice &cDice);
        bool ResolvePassOdds(std::list<Bet>::iterator &it, const Table &cTable, const Dice &cDice);
        bool ResolveDontPass(std::list<Bet>::iterator &it, const Dice &cDice);
        bool ResolveDontPassOdds(std::list<Bet>::iterator &it, const Table &cTable, const Dice &cDice);
        bool ResolveCome(std::list<Bet>::iterator &it, const Dice &cDice);
        bool ResolveComeOdds(std::list<Bet>::iterator &it, const Table &cTable, const Dice &cDice);
        bool ResolveDontCome(std::list<Bet>::iterator &it, const Dice &cDice);
        bool ResolveDontComeOdds(std::list<Bet>::iterator &it, const Dice &cDice);
        bool ResolvePlace(std::list<Bet>::iterator &it, const Table &cTable, const Dice &cDice);
        bool ResolveBig(std::list<Bet>::iterator &it, const Dice &cDice);
        bool ResolveOneRollBets(std::list<Bet>::iterator &it, const Dice &cDice);

        // Check to see if a current bet covers 6 or 8
        bool SixOrEightCovered();
        // Checks to see if the odds progression method is Arithmetic
        bool IsArithmeticOddsProgression() const { return (m_ecOddsProgressionMethod == OddsProgressionMethod::ARITHMETIC); }
        // Name and description
        std::string m_sName;
        std::string m_sDescription;

        // Money class to track bankroll
        Money m_cMoney;

        // QualifiedShooter class to track qualified shooters
        QualifiedShooter m_cQualifiedShooter;

        // StrategyTracker class to provide detailed (roll-by-roll) records of
        // a Strategy.  Used for debugging.  Recommend to a use in conjunction
        // a single simulation run.
        StrategyTracker* m_pcStrategyTracker;

        // Set Strategy defaults
        int m_nNumberOfPassBetsAllowed      = 0;
        int m_nNumberOfComeBetsAllowed      = 0;
        int m_nNumberOfDontPassBetsAllowed  = 0;
        int m_nNumberOfDontComeBetsAllowed  = 0;

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
        bool m_bFieldBetsAllowed            = false;
        int m_nFieldBetUnits                = 1;
        bool m_bAny7BetAllowed              = false;
        bool m_bAnyCrapsBetAllowed          = false;
        bool m_bCraps2BetAllowed            = false;
        bool m_bCraps3BetAllowed            = false;
        bool m_bYo11BetAllowed              = false;
        bool m_bCraps12BetAllowed           = false;
        int m_nStandardWager                = 0;
        int m_nWager                        = 0;
        bool m_bFullWager                   = false;
        float m_fStandardOdds               = 1.0;
        float m_fOdds                       = 1.0;
        bool  m_bComeOddsWorking            = false;
        bool  m_bOddsProgression            = false;
        OddsProgressionMethod m_ecOddsProgressionMethod = OddsProgressionMethod::ARITHMETIC;
        int m_nPreferredPlaceBet            = 8;

        // Set counters to zero or defaults
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
