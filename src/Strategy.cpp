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

/**
  * Construct a Strategy.
  *
  * Set basic information for a Strategy (e.g., name, description, bankroll)
  * create a StrategyTracker if tracking results is set to true.
  *
  * INI File:
  * Name=string
  * Description=string
  * InitialBankroll=integer
  * TrackResults=true|false
  *
  * \param sName         Name of the Strategy
  * \param sDesc         Description of the Strategy
  * \param nInitBank     Initial bankroll amount, used for Money
  * \param bTrackResults Flag to track results
  */

Strategy::Strategy(std::string sName, std::string sDesc, int nInitiBank, int nStdWager, bool bTrace)
{
    m_sName.assign(sName);
    m_sDescription.assign(sDesc);

    m_cMoney.Initialize(nInitiBank);

    m_cWager.Initialize(nStdWager);

    m_bTrace = bTrace;
    if (m_bTrace) m_pcStrategyTracker = new StrategyTracker(this);
}

/**
  * Destructor for Strategy.
  */

Strategy::~Strategy()
{
    // Do not delete pointer to Strategy Tracker.
    // TODO: Need copy and assignment constructor to pick up object.
    //if (m_bTrackResults) delete m_pcStrategyTracker;
    m_pcStrategyTracker = 0;
}

/**
  * Set an Elementary Strategy.
  *
  * As described in "Beat the Craps Table" by Martin Jensen.
  * -Make Pass bets and take single odds.
  * -Take double odds after winning twice original wager and odds.
  * -After losing, go back to single odds.
  *
  * INI File:
  * Predefined=Elementary
  */

void Strategy::SetElementary()
{
    m_nNumberOfPassBetsAllowed     = 1;
    m_nNumberOfComeBetsAllowed     = 0;
    m_nNumberOfPlaceBetsAllowed    = 0;
    m_fStandardOdds                = 1.0;
    m_ecOddsProgressionMethod      = OddsProgressionMethod::OP_ARITHMETIC;

    if (m_sName.empty()) SetName("Elementary");
    if (m_sDescription.empty()) SetDescription("Pass only, single odds to start");
}

/**
  * Set a Conservative Strategy.
  *
  * As described in "Beat the Craps Table" by Martin Jensen.
  * -Make Pass bets and one Come bet and take single odds.
  * -Take double odds after winning twice original wager and odds for both
  *  Pass and Come bets.
  * -Increase odds incrementally as winnings increase.
  * -After losing, go back to single odds.
  *
  * INI File:
  * Predefined=Conservative
  */

void Strategy::SetConservative()
{
    m_nNumberOfPassBetsAllowed     = 1;
    m_nNumberOfComeBetsAllowed     = 1;
    m_nNumberOfPlaceBetsAllowed    = 0;
    m_fStandardOdds                = 1.0;
    m_ecOddsProgressionMethod      = OddsProgressionMethod::OP_ARITHMETIC;

    if (m_sName.empty()) SetName("Conservative");
    if (m_sDescription.empty()) SetDescription("Pass and one Come, single odds to start");
}

/**
  * Set a Conventional Strategy.
  *
  * As described in "Beat the Craps Table" by Martin Jensen.
  * -Make Pass bets and two Come bets and take single odds.
  * -Take double odds after winning twice original wager and odds for both
  *  Pass and Come bets.
  * -Increase odds incrementally as winnings increase.
  * -After losing, go back to single odds.
  *
  * INI File:
  * Predefined=Conventional
  */

void Strategy::SetConventional()
{
    m_nNumberOfPassBetsAllowed     = 1;
    m_nNumberOfComeBetsAllowed     = 2;
    m_nNumberOfPlaceBetsAllowed    = 0;
    m_fStandardOdds                = 1.0;
    m_ecOddsProgressionMethod      = OddsProgressionMethod::OP_ARITHMETIC;

    if (m_sName.empty()) SetName("Conventional");
    if (m_sDescription.empty()) SetDescription("Pass and two Comes, single odds to start");
}

/**
  * Set an Aggressive Strategy.
  *
  * As described in "Beat the Craps Table" by Martin Jensen, this strategy
  * extends the Conventional Strategyout to four bets.
  * -Make Pass bets and two Come bets and take double odds.
  * -Make a Place bet after the previous bets are made, on a 6 or 8.  If 6 and 8
  * are already covered, make a third Come bet.
  * -Increase odds incrementally as winnings increase.
  * -After losing, go back to double odds.
  *
  * INI File:
  * Predefined=Aggressive
  */

void Strategy::SetAggressive()
{
    m_nNumberOfPassBetsAllowed     = 1;
    m_nNumberOfComeBetsAllowed     = 2;
    m_nNumberOfPlaceBetsAllowed    = 1;
    m_bPlaceAfterCome              = true;
    m_fStandardOdds                = 2.0;
    m_ecOddsProgressionMethod      = OddsProgressionMethod::OP_ARITHMETIC;

    if (m_sName.empty()) SetName("Aggressive");
    if (m_sDescription.empty()) SetDescription("Pass and either: 1) three Comes or 2) two Comes and one Place, double odds to start");
}

/**
  * Set the odds progression method.
  *
  * Set the method to progress (increase) the odds taken.
  *
  * INI File:
  * OddsProgressionMethod=ARITHMETIC|GEOMETRIC
  *
  *\param sOddsProgressionMethod The selected odds progression method.
  */

void Strategy::SetOddsProgressionMethod(std::string sOddsProgressionMethod)
{
    std::locale loc;
    for (std::string::size_type iii = 0; iii < sOddsProgressionMethod.length(); ++iii)
        sOddsProgressionMethod[iii] = std::toupper(sOddsProgressionMethod[iii], loc);

    if (sOddsProgressionMethod == "ARITHMETIC")     m_ecOddsProgressionMethod = OddsProgressionMethod::OP_ARITHMETIC;
    else if (sOddsProgressionMethod == "GEOMETRIC") m_ecOddsProgressionMethod = OddsProgressionMethod::OP_GEOMETRIC;
    else throw CrapSimException("Strategy::SetOddsProgressionMethod unknown odds progression method");
}

/**
  * Set the qualified shooter method.
  *
  * Set the method to qualify the shooter.
  *
  * INI File:
  * QualifiedShooterMethod=[a lot of chices]
  *
  *\param sQualifiedShooterMethod The selected shooter qualification method.
  */

void Strategy::SetQualifiedShooterMethod(std::string sQualifiedShooterMethod)
{
    m_cQualifiedShooter.SetMethod(sQualifiedShooterMethod);
}

/**
  * Check and correct a Strategy's settings to conform to the Table.
  *
  * Check and correct non-fatal configuration items.
  *
  * Because the Strategy settings are set independent of the Table, check and
  * correct Strategy settings against Table settings, which take precedence.
  * An example: Strategy's standard wager must be less than or eaual to the
  * Table's minimum bet.
  *
  * Ensure bet settings make sense.  An example: the number of Place bets made
  * at one time cannot exceed the number of total Place bets.
  *
  *\param cTable The Table.
  */

void Strategy::SanityCheck(const Table &cTable)
{
    // Record Table minimum and maximum bet amounts
    m_cWager.SetTableLimits(cTable.MinimumBet(), cTable.MaximumBet());

    if (m_cWager.WagerProgressionMethodSet())
    {
        if (m_nFieldBetUnits > 1)
        {
            std::cout << "\tChanging Strategy" << m_sName << ": disregard Field Bet Units and use Wager Progression Method" << std::endl;
            m_nFieldBetUnits = 1;
        }

        if (m_nPlaceBetUnits > 1)
        {
            std::cout << "\tChanging Strategy" << m_sName << ": disregard Place Bet Units and use Wager Progression Method" << std::endl;
            m_nPlaceBetUnits = 0;
        }
    }

    // Ensure that Strategy's number of place bets at once is less than or equal to the number of place bets allowed
    if (m_nNumberOfPlaceBetsMadeAtOnce > m_nNumberOfPlaceBetsAllowed)
    {
        std::cout << "\tChanging Strategy " << m_sName << ": number of place bets at once to number of place bets allowed [" << m_nNumberOfPlaceBetsMadeAtOnce << " to " << m_nNumberOfPlaceBetsAllowed << "]" << std::endl;
        m_nNumberOfPlaceBetsMadeAtOnce = m_nNumberOfPlaceBetsAllowed;
    }

    // Ensure that Strategy's number of place bets at once is less than or equal to the number of place bets allowed
    if ((m_nNumberOfPlaceBetsMadeAtOnce <= 0) && (m_nNumberOfPlaceBetsAllowed > 0))
    {
        std::cout << "\tChanging Strategy " << m_sName << ": number of place bets made at once to one [" << m_nNumberOfPlaceBetsMadeAtOnce << " to " << 1 << "]" << std::endl;
        m_nNumberOfPlaceBetsMadeAtOnce = 1;
    }

    // TODO: if not bets selected, discard strategy
}

/**
  * Make Bets.
  *
  * Call each bet type.  MakeXXXBet create and push a Bet onto the bets
  * container. If tracking results, call StrategyTracker.
  *
  *\param cTable The Table.
  */

void Strategy::MakeBets(const Table &cTable)
{
    if (StillPlaying() && ShooterQualified())
    {
        // If tracking results, start a new record, which updates bankroll, and update table stats
        if (m_bTrace) m_pcStrategyTracker->RecordNew(this, cTable);

        // Mark beinning bankroll
        m_cMoney.MarkBeforeBetting();

        MakePassBet(cTable);
        MakeComeBet(cTable);

        MakeDontPassBet(cTable);
        MakeDontComeBet(cTable);

        MakeOddsBet(cTable);

        MakePlaceBets(cTable);

        MakePutBet(cTable);

        MakeHardWayBets();

        MakeBigBets();

        MakeOneRollBets();

        // Mark bankroll after making bets
        m_cMoney.MarkAfterBetting();

        // If tracking results, capture bankroll post bets
        if (m_bTrace) m_pcStrategyTracker->RecordBetsBeforeRoll(this, m_lBets);
    }
}

/**
  * Resolve Bets.
  *
  * Check to see if the Strategy is sill playing.  If so, Loop through all bets
  * and call each bet type. If the bet is resolved, update the eager units.
  * If tracking results, call StrategyTracker. If Odds Progression is set,
  * compare before and after bankroll and adjust accordingly.
  *
  *\param cTable The Table.
  *\param cDice The Dice.
  */

void Strategy::ResolveBets(const Table &cTable, const Dice &cDice)
{
    if (!StillPlaying()) return;

    // Increment number of rolls
    ++m_nNumberOfRolls;

    // Resolve all bets
    for (std::list<Bet>::iterator it = m_lBets.begin(); it != m_lBets.end(); ++it)
    {
        if (it->IsPassBet())         ResolvePass(it, cDice);
        if (it->IsPassOddsBet())     ResolvePassOdds(it, cTable, cDice);

        if (it->IsComeBet())         ResolveCome(it, cDice);
        if (it->IsComeOddsBet())     ResolveComeOdds(it, cTable, cDice);

        if (it->IsDontPassBet())     ResolveDontPass(it, cDice);
        if (it->IsDontPassOddsBet()) ResolveDontPassOdds(it, cTable, cDice);

        if (it->IsDontComeBet())     ResolveDontCome(it, cDice);
        if (it->IsDontComeOddsBet()) ResolveDontComeOdds(it, cDice);

        if (it->IsPlaceBet())        ResolvePlace(it, cTable, cDice);

        if (it->IsPutBet())          ResolvePut(it, cDice);
        if (it->IsPutOddsBet())      ResolvePutOdds(it, cDice);

        if (it->IsHardBet())         ResolveHardWayBets(it, cDice);

        if (it->IsBigBet())          ResolveBig(it, cDice);

        if (it->IsOneRollBet())      ResolveOneRollBets(it, cDice);

        if (it->Pushed())
            it->SetUnresolved();

        if (it->Resolved())
        {
            // Update wager units for next bet based on this resolved bet
            m_cWager.WagerUnits(it);
        }
    }

    // Mark ending bankroll
    m_cMoney.MarkAfterResolvingBets();

    // Check Odds Progression tactic
    if (IsUsingOddsProgession())
    {
        // If bankroll has increased, increase Odds
        if (m_cMoney.GainAfterBetting()) IncreaseOdds();
        // Else reset odds
        else ResetOdds();
    }

    // If tracking results, record ending bankroll and post results
    if (m_bTrace) m_pcStrategyTracker->RecordBetsAfterRoll(this, m_lBets, cDice.RollValue());
    if (m_bTrace) m_pcStrategyTracker->Post();
}

/**
  * Modify Bets.
  *
  * Check to see if the Strategy is sill playing and a bet modification
  * method has beet set.  If so, loop through all bets and modify each bet
  * appropriately.  If tracking results, call StrategyTracker.
  *
  *\param cTable The Table.
  *\param cDice The Dice.
  */

void Strategy::ModifyBets(const Table &cTable, const Dice &cDice)
{
    if (!StillPlaying()) return;

    if (m_cWager.ModifyBets(m_cMoney, cTable, cDice, m_lBets))
        m_cQualifiedShooter.Reset();

    // Mark ending bankroll
    //m_cMoney.MarkAfterResolvingBets();

    // If tracking results, record ending bankroll and post results
    //if (m_bTrace) m_pcStrategyTracker->RecordBetsAfterRoll(this, m_lBets, cDice.RollValue());
    //if (m_bTrace) m_pcStrategyTracker->Post();
}

void Strategy::FinalizeBets()
{
    if (!StillPlaying()) return;

    std::list<Bet>::iterator it = m_lBets.begin();
    while(it != m_lBets.end())
    {
        if (it->Resolved())
        {
            if (it->IsPassBet()) --m_nNumberOfPassBetsMade;
            //if (it->IsPassOddsBet()) {}
            if (it->IsDontPassBet()) --m_nNumberOfDontPassBetsMade;
            //if (it->IsDontPassOddsBet()) {}
            if (it->IsComeBet()) --m_nNumberOfComeBetsMade;
            //if (it->IsComeOddsBet()) {}
            if (it->IsDontComeBet()) --m_nNumberOfDontComeBetsMade;
            //if (it->IsDontComeOddsBet()) {}
            if (it->IsPutBet()) m_bPutBetMade = false;
            //if (it->IsPutOddsBet()) {}
            if (it->IsPlaceBet())
            {
                // Set Place bet number to false (bet not made)
                m_mPlaceBets[it->Point()] = false;
                // Decrement the number of Place bets made
                --m_nNumberOfPlaceBetsMade;
            }
            if (it->IsBigBet())
            {
                if (it->IsBig6Bet()) --m_nNumberOfBig6BetsMade;
                if (it->IsBig8Bet()) --m_nNumberOfBig8BetsMade;
            }
            if (it->IsHardBet())
            {
                if (it->IsHard4Bet())  --m_nNumberOfHard4BetsMade;
                if (it->IsHard6Bet())  --m_nNumberOfHard6BetsMade;
                if (it->IsHard8Bet())  --m_nNumberOfHard8BetsMade;
                if (it->IsHard10Bet()) --m_nNumberOfHard10BetsMade;
            }
            //if (it->IsOneRollBet()) {}
            it = m_lBets.erase(it);
        }
        else
        {
            ++it;
        }
    }

}

/**
  * Make a Pass Bet.
  *
  * If Table is coming out, make a Pass bet and add it to bets container.
  *
  * INI File:
  * PassBet=1
  *
  *\param cTable The Table.
  */

void Strategy::MakePassBet(const Table &cTable)
{
    if (cTable.IsComingOutRoll())
    {
        if (m_nNumberOfPassBetsMade < m_nNumberOfPassBetsAllowed)
        {
            // If money is not available to bet, do not make a bet
            int nWager = m_cWager.BetWager(m_cMoney.Bankroll());
            if (nWager < 1) return;

            Bet cBet;
            cBet.MakePassBet(nWager);
            m_cMoney.Decrement(nWager);
            ++m_nNumberOfPassBetsMade;
            m_lBets.push_back(cBet);
        }
    }
}

/**
  * Make a Don't Pass Bet.
  *
  * If Table is coming out, make a Don't Pass bet and add it to bets container.
  *
  * INI File:
  * DontPassBet=1
  *
  *\param cTable The Table.
  */

void Strategy::MakeDontPassBet(const Table &cTable)
{
    if (cTable.IsComingOutRoll())
    {
        if (m_nNumberOfDontPassBetsMade < m_nNumberOfDontPassBetsAllowed)
        {
            // If money is not available to bet, do not make a bet
            int nWager = m_cWager.BetWager(m_cMoney.Bankroll());
            if (nWager < 1) return;

            Bet cBet;
            cBet.MakeDontPassBet(nWager);
            m_cMoney.Decrement(nWager);
            ++m_nNumberOfDontPassBetsMade;
            m_lBets.push_back(cBet);
        }
    }
}

/**
  * Make a Come Bet.
  *
  * If Table is not coming out, make a Don't Pass bet and add it to bets
  * container.
  *
  * INI File:
  * ComeBets=[0-6]
  *
  *\param cTable The Table.
  */

void Strategy::MakeComeBet(const Table &cTable)
{
    if (!cTable.IsComingOutRoll())
    {
        if (m_nNumberOfComeBetsMade < m_nNumberOfComeBetsAllowed)
        {
            // If money is not available to bet, do not make a bet
            int nWager = m_cWager.BetWager(m_cMoney.Bankroll());
            if (nWager < 1) return;

            Bet cBet;
            cBet.MakeComeBet(nWager);
            m_cMoney.Decrement(nWager);
            ++m_nNumberOfComeBetsMade;
            m_lBets.push_back(cBet);
        }
    }
}

/**
  * Make a Don't Come Bet.
  *
  * If Table is not coming out, make a Don't Come bet and add it to bets
  * container.
  *
  * INI File:
  * DontComeBets=[0-6]
  *
  *\param cTable The Table.
  */

void Strategy::MakeDontComeBet(const Table &cTable)
{
    if (!cTable.IsComingOutRoll())
    {
        if (m_nNumberOfDontComeBetsMade < m_nNumberOfDontComeBetsAllowed)
        {
            // If money is not available to bet, do not make a bet
            int nWager = m_cWager.BetWager(m_cMoney.Bankroll());
            if (nWager < 1) return;

            Bet cBet;
            cBet.MakeDontComeBet(nWager);
            m_cMoney.Decrement(nWager);
            ++m_nNumberOfDontComeBetsMade;
            m_lBets.push_back(cBet);
        }
    }
}

/**
  * Make an Odds Bet.
  *
  * Loop through the bets.  If a bet may have an odds bet and it has not yet
  * been made, make it and add it to bets container.
  *
  * INI File:
  * StandardOdds=[0-9]+\.[0-9]
  * FullWager=true|false
  *
  *\param cTable The Table.
  */

void Strategy::MakeOddsBet(const Table &cTable)
{
    // 0.0 Odds indicates no Odds Bets
    if (m_fStandardOdds == 0.0) return;

    for (std::list<Bet>::iterator it = m_lBets.begin();it != m_lBets.end(); ++it)
    {
        // Pass Bet or Dont Pass Bet
        if (it->IsPassBet() || it->IsDontPassBet() || it->IsComeBet() || it->IsDontComeBet() || it->IsPutBet())
        {
            // Not on the come out for the bet
            if (!it->OnTheComeOut())
            {
                // Bet without Odds Bet?
                if (!it->IsOddsBetMade())
                {
                    // Calculate maximum odds wager, based on Strategy and Table type
                    int nWager = m_cWager.OddsBetWager(m_cMoney.Bankroll(),
                                                       it->Wager(),
                                                       it->Point(),
                                                       std::min(cTable.MaxOdds(it->Point()), m_fOdds)); // Odds to use
                    // If no money, do not make bet
                    if (nWager < 1) return;

                    // Generate a new bet
                    Bet cBet;

                    // Methods for Put
                    if (it->IsPutBet())
                    {
                        // Create Bet as Put Odds
                        cBet.MakePutOddsBet(nWager, it->Point());
                    }
                    else if (it->IsPassBet() || it->IsDontPassBet())
                    {
                        // First, create Bet as Pass Odds
                        cBet.MakePassOddsBet(nWager, it->Point());

                        // Second, if this is a Don't Pass Odds Bet, calculate payoff and use as wager
                        if (it->IsDontPassBet())
                        {
                            nWager = cBet.CalculatePayoff();
                            // Third, change bet to Dont Pass Odds
                            cBet.MakeDontPassOddsBet(nWager, it->Point());
                        }
                    }
                    else // Is a ComeBet or a DontComeBet
                    {
                        // First, create Bet as Come Odds
                        cBet.MakeComeOddsBet(nWager, it->Point());

                        if (it->IsComeBet())
                        {
                            // Set whether odds are working for the Come Bet on the Table come out
                            cBet.SetComeOddsAreWorking(m_bComeOddsWorking);
                        }

                        else // Is a DontComeBet
                        {
                            // Second, calculate payoff and use as wager
                            nWager = cBet.CalculatePayoff();
                            // Third, change bet to Dont Pass Odds
                            cBet.MakeDontComeOddsBet(nWager, it->Point());
                        }
                    }

                    // Indicate that original bet now has an Odds bet
                    it->SetOddsBetMade();

                    m_cMoney.Decrement(nWager);
                    m_lBets.push_back(cBet);
                }
            }
        }
    }
}

/**
  * Make a Place Bets.
  *
  * Check conditions for making a Place bet.  If right, Make a Place bet.
  *
  * INI File:
  * PlaceBets=[0-6]
  * PlaceBetsMadeAtOnce=[0-6]
  * PlaceAfterCome=true|false
  *
  *\param cTable The Table.
  */

void Strategy::MakePlaceBets(const Table &cTable)
{
    // If we make Place bets only after Come bets are exhausted
    if (m_bPlaceAfterCome)
    {
        // If number of Come bets made equals number allowed
        if (m_nNumberOfComeBetsMade == m_nNumberOfComeBetsAllowed)
        {
            // If the table is not coming out and number of place bets made is less than number of allowed
            if (!cTable.IsComingOutRoll() && (m_nNumberOfPlaceBetsMade < m_nNumberOfPlaceBetsAllowed))
            {
                // If 6 or 8 are already covered by a bet, make another Come bet instead of a place bet
                if (SixOrEightCovered())
                {
                    // If money is not available to bet, do not make a bet
                    int nWager = m_cWager.BetWager(m_cMoney.Bankroll());
                    if (nWager < 1) return;

                    Bet cBet;
                    cBet.MakeComeBet(nWager);
                    m_cMoney.Decrement(nWager);
                    ++m_nNumberOfComeBetsMade;
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
    // Else make place bets regardless of Come bets, but check table for coming
    // out roll and the number of Place bets made is less than number allowed
    else if (!cTable.IsComingOutRoll())
    {
        // Count the number of Place bets made on this turn
        int nNumberOfPlaceBetsMadeThisTurn = 0;

        // Make Place bets up to allowed limit
        while(m_nNumberOfPlaceBetsMade < m_nNumberOfPlaceBetsAllowed)
        {
            MakePlaceBet();

            ++nNumberOfPlaceBetsMadeThisTurn;
            // If number of Place bets made this turn equals number allowed at one time, break;
            if (nNumberOfPlaceBetsMadeThisTurn == m_nNumberOfPlaceBetsMadeAtOnce)
                break;
        }
    }
}

/**
  * Make a Place Bet.
  *
  * Make a Place bet and add it to bets container.
  *
  * INI File:
  * PlaceBetUnits=[1-6]
  */

void Strategy::MakePlaceBet()
{
    // Get Place bet number
    int nPlaceBetNumber = PlaceBetNumber();
    // If Place bet number less than zero, all numbers taken
    if (nPlaceBetNumber < 0) return;

    // If money is not available to bet, do not make a bet
    int nWager = m_cWager.PlaceBetUnitsWager(m_cMoney.Bankroll(),m_nPlaceBetUnits, nPlaceBetNumber);
    if (nWager < 1) return;

    Bet cBet;

    // Set Place bet number to true (bet  made)
    m_mPlaceBets[nPlaceBetNumber] = true;

    // Make Place bet, with nWager (versus m_nWager)
    cBet.MakePlaceBet(nWager, nPlaceBetNumber);
    // Reduce bankroll by wager amounts
    m_cMoney.Decrement(nWager);
    // Increment the number of Place bets made
    ++m_nNumberOfPlaceBetsMade;

    m_lBets.push_back(cBet);
}

/**
  * Return preferred Place bet number.
  *
  * Keeps track of Place bet numbers made and resolved.
  *
  * INI File:
  * PlacePreferred=[4,5,6,8,9,10]
  *
  *\return Next preferred place bet number.
  */

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

/**
  * Is Six or Eight covered by a bet.
  *
  * Loop through the bets to see if six or eight are alredy covered by a bet.
  *
  *\return True if six or eight are already covered, otherwise false.
  */

bool Strategy::SixOrEightCovered()
{
    for (const Bet &cBet : m_lBets)
    {
        if ((cBet.Point() == 6) || (cBet.Point() == 8))
            return (true);
    }

    return (false);
}

/**
  * Make a Put Bet.
  *
  * After Table is coming out, make a Put bet, and an Odds Bet, and add it
  * to bets container.
  *
  * INI File:
  * PutBet=true|false
  *
  *\param cTable The Table.
  */

void Strategy::MakePutBet(const Table &cTable)
{
    if (!cTable.IsComingOutRoll() && m_bPutBetsAllowed && !m_bPutBetMade)
    {
        // If money is not available to bet, do not make a bet
        int nWager = m_cWager.BetWager(m_cMoney.Bankroll());
        if (nWager < 1) return;

        Bet cBet;
        cBet.MakePutBet(nWager, cTable.Point());
        m_cMoney.Decrement(nWager);
        m_bPutBetMade = true;
        m_lBets.push_back(cBet);

        MakeOddsBet(cTable); // Take Odds on Put Bet
    }
}

/**
  * Make a Hard Way Bet.
  *
  * Make a Hard 4, 6, 8, and/or 10 and add it to bets container.
  *
  * INI File:
  * HardWay4Bet=true|false
  * HardWay6Bet=true|false
  * HardWay8Bet=true|false
  * HardWay10Bet=true|false
  */

void Strategy::MakeHardWayBets()
{
    if (m_bHard4BetAllowed && m_nNumberOfHard4BetsMade == 0)
    {
        // If money is not available to bet, do not make a bet
        int nWager = m_cWager.BetWager(m_cMoney.Bankroll());
        if (nWager < 1) return;

        Bet cBet;
        cBet.MakeHard4Bet(nWager);
        m_cMoney.Decrement(nWager);
        ++m_nNumberOfHard4BetsMade;
        m_lBets.push_back(cBet);
    }

    if (m_bHard6BetAllowed && m_nNumberOfHard6BetsMade == 0)
    {
        // If money is not available to bet, do not make a bet
        int nWager = m_cWager.BetWager(m_cMoney.Bankroll());
        if (nWager < 1) return;

        Bet cBet;
        cBet.MakeHard6Bet(nWager);
        m_cMoney.Decrement(nWager);
        ++m_nNumberOfHard6BetsMade;
        m_lBets.push_back(cBet);
    }

    if (m_bHard8BetAllowed && m_nNumberOfHard8BetsMade)
    {
        // If money is not available to bet, do not make a bet
        int nWager = m_cWager.BetWager(m_cMoney.Bankroll());
        if (nWager < 1) return;

        Bet cBet;
        cBet.MakeHard8Bet(nWager);
        m_cMoney.Decrement(nWager);
        ++m_nNumberOfHard8BetsMade;
        m_lBets.push_back(cBet);
    }

    if (m_bHard10BetAllowed && m_nNumberOfHard10BetsMade)
    {
        // If money is not available to bet, do not make a bet
        int nWager = m_cWager.BetWager(m_cMoney.Bankroll());
        if (nWager < 1) return;

        Bet cBet;
        cBet.MakeHard10Bet(nWager);
        m_cMoney.Decrement(nWager);
        ++m_nNumberOfHard10BetsMade;
        m_lBets.push_back(cBet);
    }
}

/**
  * Make a Big Bet.
  *
  * Make a Big 6 and/or 8 Bet and add it to bets container.
  *
  * INI File:
  * Big6Bet=true|false
  * Big8Bet=true|false
  */

void Strategy::MakeBigBets()
{
    if (m_bBig6BetAllowed && m_nNumberOfBig6BetsMade == 0)
    {
        // If money is not available to bet, do not make a bet
        int nWager = m_cWager.BetWager(m_cMoney.Bankroll());
        if (nWager < 1) return;

        Bet cBet;
        cBet.MakeBig6Bet(nWager);
        m_cMoney.Decrement(nWager);
        ++m_nNumberOfBig6BetsMade;
        m_lBets.push_back(cBet);
    }


    if (m_bBig8BetAllowed && m_nNumberOfBig8BetsMade == 0)
    {
        // If money is not available to bet, do not make a bet
        int nWager = m_cWager.BetWager(m_cMoney.Bankroll());
        if (nWager < 1) return;

        Bet cBet;
        cBet.MakeBig8Bet(nWager);
        m_cMoney.Decrement(nWager);
        ++m_nNumberOfBig8BetsMade;
        m_lBets.push_back(cBet);
    }
}

/**
  * Make One Roll Bets.
  *
  * Make one roll bets: Field, Any 7, Any Craps, Craps 2, Craps 3, Yo 11, and
  * Craps 12. Add it to bets container.
  *
  * INI File:
  * FieldBet=true|false
  * FieldBetUnits=[1-infinity]
  * Any7Bet=true|false
  * AnyCrapsBet=true|false
  * Craps2Bet=true|false
  * Craps3Bet=true|false
  * Yo11Bet=true|false
  * Crap12Bet=true|false
  *
  */

void Strategy::MakeOneRollBets()
{
    // Field Bet
    if (m_bFieldBetsAllowed)
    {
        int nWager = 0;
        // If FieldBetUnits is greater than 1, use the BetUnitsWager method
        if (m_nFieldBetUnits > 1)
            nWager = m_cWager.BetUnitsWager(m_cMoney.Bankroll(), m_nFieldBetUnits);
        // Else use the BetWager method
        else
            nWager = m_cWager.BetWager(m_cMoney.Bankroll());

        if (nWager >= 1)
        {
            Bet cBet;
            cBet.MakeFieldBet(nWager);
            m_cMoney.Decrement(nWager);
            m_lBets.push_back(cBet);
        }
    }

    // Any 7 Bet
    if (m_bAny7BetAllowed)
    {
        int nWager = m_cWager.BetWager(m_cMoney.Bankroll());
        if (nWager >= 1)
        {
            Bet cBet;
            cBet.MakeAny7Bet(nWager);
            m_cMoney.Decrement(nWager);
            m_lBets.push_back(cBet);
        }
    }

    // Any Craps Bet
    if (m_bAnyCrapsBetAllowed)
    {
        int nWager = m_cWager.BetWager(m_cMoney.Bankroll());
        if (nWager >= 1)
        {
            Bet cBet;
            cBet.MakeAnyCrapsBet(nWager);
            m_cMoney.Decrement(nWager);
            m_lBets.push_back(cBet);
        }
    }

    // Craps 2 Bet
    if (m_bCraps2BetAllowed)
    {
        int nWager = m_cWager.BetWager(m_cMoney.Bankroll());
        if (nWager >= 1)
        {
            Bet cBet;
            cBet.MakeCraps2Bet(nWager);
            m_cMoney.Decrement(nWager);
            m_lBets.push_back(cBet);
        }
    }

    // Craps 3 Bet
    if (m_bCraps3BetAllowed)
    {
        int nWager = m_cWager.BetWager(m_cMoney.Bankroll());
        if (nWager >= 1)
        {
            Bet cBet;
            cBet.MakeCraps3Bet(nWager);
            m_cMoney.Decrement(nWager);
            m_lBets.push_back(cBet);
        }
    }

    // Yo 11 Bet
    if (m_bYo11BetAllowed)
    {
        int nWager = m_cWager.BetWager(m_cMoney.Bankroll());
        if (nWager >= 1)
        {
            Bet cBet;
            cBet.MakeYo11Bet(nWager);
            m_cMoney.Decrement(nWager);
            m_lBets.push_back(cBet);
        }
    }

    // Craps 12 Bet
    if (m_bCraps12BetAllowed)
    {
        int nWager = m_cWager.BetWager(m_cMoney.Bankroll());
        if (nWager >= 1)
        {
            Bet cBet;
            cBet.MakeCraps12Bet(nWager);
            m_cMoney.Decrement(nWager);
            m_lBets.push_back(cBet);
        }
    }
}

/**
  * Resolve Pass Bet.
  *
  * Loop through all bets looking for a Pass Bet.  Resolve based on
  * the state of the bet and the dice.
  *
  *\param cDice The Dice.
  */

void Strategy::ResolvePass(std::list<Bet>::iterator &it, const Dice &cDice)
{
    if (it->OnTheComeOut())                                 // Pass Bet On the Come Out?
    {
        if (cDice.IsCraps())                                    // Pass Bet On the Come Out and a Craps Roll?
        {
            ////--m_nNumberOfPassBetsMade;
            it->SetLost();
        }
        else if (cDice.IsNatural())                             // PassBet One the Come Out and a Natural Roll?
        {
            m_cMoney.Increment(it->Wager() + it->CalculatePayoff());// Payoff
            ////--m_nNumberOfPassBetsMade;
            it->SetWon();
        }
        else                                                    // Pass Bet on the Come Out = Set Point
        {
            it->SetPoint(cDice.RollValue());                        // Set Point
        }
    }
    else                                                    // Pass Bet, but not on the Come Out
    {
        if (cDice.IsSeven())                                    // Pass Bet, but not on the Come Out and Seven Roll?
        {
            ////--m_nNumberOfPassBetsMade;
            it->SetLost();
        }
        else if (it->Point() == cDice.RollValue())              // Pass Bet, but not on the Come Out and Hit the Point?
        {
            m_cMoney.Increment(it->Wager() + it->CalculatePayoff());// Payoff
            ////--m_nNumberOfPassBetsMade;
            it->SetWon();
        }
    }
}

/**
  * Resolve Don't Pass Bet.
  *
  * Loop through all bets looking for a Don't Pass Bet.  Resolve based on
  * the state of the bet and the dice.
  *
  *\param cDice The Dice.
  */

void Strategy::ResolveDontPass(std::list<Bet>::iterator &it, const Dice &cDice)
{
    if (it->OnTheComeOut())                                 // Dont Pass Bet On the Come Out?
    {
        if (cDice.IsCraps())                                    // Dont Pass Bet On the Come Out and a Craps Roll?
        {
            if (!cDice.IsBar())                                     // Dont Pass Bet On the Come Out and a Craps Roll and it's not a bar dice roll
            {
                m_cMoney.Increment(it->Wager() + it->CalculatePayoff());// Payoff
                ////--m_nNumberOfDontPassBetsMade;
                it->SetWon();
            }

        }
        else if (cDice.IsNatural())                             // Dont Pass Bet One the Come Out and a Natural Roll?
        {
            ////--m_nNumberOfDontPassBetsMade;
            it->SetLost();
        }
        else                                                    // Dont Pass Bet on the Come Out = Set Point
        {
            it->SetPoint(cDice.RollValue());                        // Set Point
        }
    }
    else                                                    // Dont Pass Bet, but not on the Come Out
    {
        if (cDice.IsSeven())                                    // Dont Pass Bet, but not on the Come Out and Seven Roll?
        {
            m_cMoney.Increment(it->Wager() + it->CalculatePayoff());// Payoff
            ////--m_nNumberOfDontPassBetsMade;
            it->SetWon();
        }
        else if (it->Point() == cDice.RollValue())              // Dont Pass Bet, but not on the Come Out and Hit the Point?
        {
            ////--m_nNumberOfDontPassBetsMade;
            it->SetLost();
        }
    }
}

/**
  * Resolve Come Bet.
  *
  * Loop through all bets looking for a Come Bet.  Resolve based on
  * the state of the bet and the dice.
  *
  *\param cDice The Dice.
  */

void Strategy::ResolveCome(std::list<Bet>::iterator &it, const Dice &cDice)
{
    if (it->OnTheComeOut())                                 // Come Bet On the Come Out?
    {
        if (cDice.IsCraps())                                    // Come Bet On the Come Out and a Craps Roll?
        {
            ////--m_nNumberOfComeBetsMade;
            it->SetLost();
        }
        else if (cDice.IsNatural())                             // Come Bet On the Come Out and a Natural Roll?
        {
            m_cMoney.Increment(it->Wager() + it->CalculatePayoff());// Payoff
            ////--m_nNumberOfComeBetsMade;
            it->SetWon();
        }
        else                                                    // Come Bet On the Come Out = Point Roll
        {
            it->SetPoint(cDice.RollValue());                        // Set Point
        }
    }
    else                                                    // Come Bet, but not on the Come Out
    {
        if (cDice.IsSeven())                                    // Come Bet, but not on the Come Out and a Seven Roll?
        {
            ////--m_nNumberOfComeBetsMade;
            it->SetLost();
        }
        else if (it->Point() == cDice.RollValue())           //  Come Bet, but not on the Come Out and Hit the Point?
        {
            m_cMoney.Increment(it->Wager() + it->CalculatePayoff());// Payoff
            ////--m_nNumberOfComeBetsMade;
            it->SetWon();
        }
    }
}

/**
  * Resolve Don't Come Bet.
  *
  * Loop through all bets looking for a Don't Come Bet.  Resolve based on
  * the state of the bet and the dice.
  *
  *\param cDice The Dice.
  */

void Strategy::ResolveDontCome(std::list<Bet>::iterator &it, const Dice &cDice)
{
    if (it->OnTheComeOut())                                 // Dont Come Bet On the Come Out?
    {
        if (cDice.IsCraps())                                    // Dont Come Bet On the Come Out and a Craps Roll?
        {
            if (!cDice.IsBar())                                     // It not a bar dice roll
            {
                m_cMoney.Increment(it->Wager() + it->CalculatePayoff());// Payoff
                ////--m_nNumberOfDontComeBetsMade;
                it->SetWon();
            }
        }
        else if (cDice.IsNatural())                             // Dont Come Bet On the Come Out and a Natural Roll?
        {
            ////--m_nNumberOfDontComeBetsMade;
            it->SetLost();
        }
        else                                                    // Dont Come Bet On the Come Out = Point Roll
        {
            it->SetPoint(cDice.RollValue());                        // Set Point
        }
    }
    else                                                    // Dont Come Bet, but not on the Come Out
    {
        if (cDice.IsSeven())                                    // Dont Come Bet, but not on the Come Out and a Seven Roll?
        {
            m_cMoney.Increment(it->Wager() + it->CalculatePayoff());// Payoff
            ////--m_nNumberOfDontComeBetsMade;
            it->SetWon();
        }
        else if (it->Point() == cDice.RollValue())           //  Dont Come Bet, but not on the Come Out and Hit the Point?
        {
            ////--m_nNumberOfDontComeBetsMade;
            it->SetLost();
        }
    }
}

/**
  * Resolve Put Bet.
  *
  * Resolve the Put bet based on the state of the bet and the dice.
  *
  *\param cDice The Dice.
  */

void Strategy::ResolvePut(std::list<Bet>::iterator &it, const Dice &cDice)
{
    if (cDice.IsSeven())                                    // Put Bet and a Seven Roll?
    {
        ////m_bPutBetMade = false;
        it->SetLost();
    }
    else if (it->Point() == cDice.RollValue())              // Put Bet and Hit the Point?
    {
        m_cMoney.Increment(it->Wager() + it->CalculatePayoff());// Payoff
        ////m_bPutBetMade = false;
        it->SetWon();
    }
}

/**
  * Resolve Pass Odds Bet.
  *
  * Loop through all bets looking for a Pass Odds Bet.  Resolve based on
  * the state of the bet, the table, and the dice.
  *
  *\param cTable The Table.
  *\param cDice The Dice.
  */

void Strategy::ResolvePassOdds(std::list<Bet>::iterator &it, const Table &cTable, const Dice &cDice)
{
    if (cTable.IsComingOutRoll() == true)             // There should not be a Pass Odds bet if this is coming out roll
        throw CrapSimException("Strategy::ResolvePassOdds called when Table is coming out");

    if (cDice.IsSeven())                                    // Pass Odds Bets and a Seven Roll?
    {
        it->SetLost();
    }
    else if (it->Point() == cDice.RollValue())               // Pass Odds Bet and Hit the Point?
    {
        m_cMoney.Increment(it->Wager() + it->CalculatePayoff());// Payoff
        it->SetWon();
    }
}

/**
  * Resolve Don't Pass Odds Bet.
  *
  * Loop through all bets looking for a Don't Pass Odds Bet.  Resolve based on
  * the state of the bet, the table, and the dice.
  *
  *\param cTable The Table.
  *\param cDice The Dice.
  */

void Strategy::ResolveDontPassOdds(std::list<Bet>::iterator &it, const Table &cTable, const Dice &cDice)
{
    if (cTable.IsComingOutRoll() == true)             // There should not be a Don't Pass Odds bet if this is coming out roll
        throw CrapSimException("Strategy::ResolveDontPassOdds called when Table is coming out");

    if (cDice.IsSeven())                                    // Dont Pass Odds Bets and a Seven Roll?
    {
        m_cMoney.Increment(it->Wager() + it->CalculatePayoff());// Payoff
        it->SetWon();
    }
    else if (it->Point() == cDice.RollValue())               // Dont Pass Odds Bet and Hit the Point?
    {
        it->SetLost();
    }
}

/**
  * Resolve Come Odds Bet.
  *
  * Loop through all bets looking for a Come Odds Bet.  Resolve based on
  * the state of the bet, the table, and the dice.
  *
  * INI File:
  * ComeOddsWorking=true|false
  *
  *\param cTable The Table.
  *\param cDice The Dice.
  */

void Strategy::ResolveComeOdds(std::list<Bet>::iterator &it, const Table &cTable, const Dice &cDice)
{
    if (cTable.IsComingOutRoll())                           // Come Odds Bet and Come Out Roll for the Table?
    {
        if (it->ComeOddsAreWorking())                           // Come Odds Bet, Come Out Roll for the Table, and are odds working on the Come Out Roll?
        {
            if (cDice.IsSeven())                                    // Come Odds Bet, Come Out Roll for the Table, and odss working, and a Seven Roll?
            {
                it->SetLost();
            }

            else if (it->Point() == cDice.RollValue())              // Come Odds Bet, Come Out Roll for the Table, and odss working, and Hit the Point?
            {
                m_cMoney.Increment(it->Wager() + it->CalculatePayoff());// Payoff
                it->SetWon();
            }
        }

        else                                                    // Come Odds Bet, Come Out Roll for the Table, and odds are NOT working on the Come Out Roll?
        {
            if (cDice.IsSeven())                                    // Come Odds Bet, Come Out Roll for the Table,  odds are NOT working on the Come Out Roll, and Seven Roll?
            {
                m_cMoney.Increment(it->Wager());                        // Return wager
                it->SetReturned();
            }
            else if (it->Point() == cDice.RollValue())              // Come Odds Bet, Come Out Roll for the Table, odds are NOT working on the Come Out Roll, and Hit the Point?
            {
                m_cMoney.Increment(it->Wager());                        // Return wager
                it->SetReturned();
            }
        }
    }

    else                                                    // Come Odds Bet, but not on Come Out Roll for the Table
    {
        if (cDice.IsSeven())                                    // Come Odds Bet, but not on Come Out Roll for the Table, and a Seven Roll?
        {
            it->SetLost();
        }

        else if (it->Point() == cDice.RollValue())              // Come Odds Bet, but not on Come Out Roll for the Table, and Hit the Point?
        {
            m_cMoney.Increment(it->Wager() + it->CalculatePayoff());// Payoff
            it->SetWon();
        }
    }
}

/**
  * Resolve Don't Come Odds Bet.
  *
  * Loop through all bets looking for a Don't Pass Come Bet.  Resolve based on
  * the state of the bet and the dice.
  *
  *\param cDice The Dice.
  */

void Strategy::ResolveDontComeOdds(std::list<Bet>::iterator &it, const Dice &cDice)
{
    if (cDice.IsSeven())                                        // Dont Come Odds Bet and a Seven Roll?
    {
        it->SetLost();
    }

    else if (it->Point() == cDice.RollValue())                  // Dont Come Odds Bet and Hit the Point?
    {
        m_cMoney.Increment(it->Wager() + it->CalculatePayoff());    // Payoff
        it->SetWon();
    }
 }

 /**
  * Resolve Put Odds Bet.
  *
  * Resolve Put Odds bet based on the state of the bet, the table, and the dice.
  *
  *\param cTable The Table.
  *\param cDice The Dice.
  */

void Strategy::ResolvePutOdds(std::list<Bet>::iterator &it, const Dice &cDice)
{
    if (cDice.IsSeven())                                    // Put Odds Bets and a Seven Roll?
    {
        it->SetLost();
    }
    else if (it->Point() == cDice.RollValue())               // Put Odds Bet and Hit the Point?
    {
        m_cMoney.Increment(it->Wager() + it->CalculatePayoff());// Payoff
        it->SetWon();
    }
}

/**
  * Resolve Place Bets.
  *
  * Resolve all Place Bets based on the state of the bet, the table, and the dice.
  *
  *\param cTable The Table.
  *\param cDice The Dice.
  */

void Strategy::ResolvePlace(std::list<Bet>::iterator &it, const Table &cTable, const Dice &cDice)
{
    if (!cTable.IsComingOutRoll() || m_bPlaceWorking)       // Place Bets are off on the Table come out roll unless explicitly turned on
    {
        if (cDice.IsSeven())                                    // Place Bet and not a come out roll and a Seven Roll?
        {
            //// Set Place bet number to false (bet not made)
            ////m_mPlaceBets[it->Point()] = false;
            //// Decrement the number of Place bets made
            ////--m_nNumberOfPlaceBetsMade;
            it->SetLost();
        }
        else if (it->Point() == cDice.RollValue())              // Place Bet and not a come out roll and Hit the Point?
        {
            // Gather winnings
            m_cMoney.Increment(it->Wager() + it->CalculatePayoff());// Payoff
            //// Set Place bet number to false (bet not made)
            ////m_mPlaceBets[it->Point()] = false;
            //// Decrement the number of Place bets made
            ////--m_nNumberOfPlaceBetsMade;
            it->SetWon();
        }
    }
}

/**
  * Resolve Hard Way Bets.
  *
  * Resolve based on the state of the bet, the table, and the dice.
  *
  *\param cDice The Dice.
  */

void Strategy::ResolveHardWayBets(std::list<Bet>::iterator &it, const Dice &cDice)
{
    if (cDice.IsSeven() || !cDice.IsHard())
    {
        it->SetLost();
        ////if (it->IsHard4Bet())  --m_nNumberOfHard4BetsMade;
        ////if (it->IsHard6Bet())  --m_nNumberOfHard6BetsMade;
        ////if (it->IsHard8Bet())  --m_nNumberOfHard8BetsMade;
        ////if (it->IsHard10Bet()) --m_nNumberOfHard10BetsMade;
    }

    if (cDice.IsFour() && it->IsHard4Bet())
    {
        m_cMoney.Increment(it->Wager() + it->CalculatePayoff());
        it->SetWon();
        ////--m_nNumberOfHard4BetsMade;
    }

    if (cDice.IsSix() && it->IsHard6Bet())
    {
        m_cMoney.Increment(it->Wager() + it->CalculatePayoff());
        it->SetWon();
        ////--m_nNumberOfHard6BetsMade;
    }

    if (cDice.IsEight() && it->IsHard8Bet())
    {
        m_cMoney.Increment(it->Wager() + it->CalculatePayoff());
        it->SetWon();
        ////--m_nNumberOfHard8BetsMade;
    }

    if (cDice.IsTen() && it->IsHard10Bet())
    {
        m_cMoney.Increment(it->Wager() + it->CalculatePayoff());
        it->SetWon();
        ////--m_nNumberOfHard10BetsMade;
    }
}

/**
  * Resolve Big Bets.
  *
  * Resolve based on the state of the bet and the dice.
  *
  *\param cDice The Dice.
  */

void Strategy::ResolveBig(std::list<Bet>::iterator &it, const Dice &cDice)
{
    if (cDice.IsSeven())
    {
        it->SetLost();
        ////if (it->IsBig6Bet()) --m_nNumberOfBig6BetsMade;
        ////if (it->IsBig8Bet()) --m_nNumberOfBig8BetsMade;
    }

    if (cDice.IsSix() && it->IsBig6Bet())
    {
        m_cMoney.Increment(it->Wager() + it->CalculatePayoff());
        it->SetWon();
        ////--m_nNumberOfBig6BetsMade;
    }

    if (cDice.IsEight() && it->IsBig8Bet())
    {
        m_cMoney.Increment(it->Wager() + it->CalculatePayoff());
        it->SetWon();
        ////--m_nNumberOfBig8BetsMade;
    }
}

/**
  * Resolve One Roll Bets.
  *
  * Loop through all bets looking for a One Roll Bets.  Resolve based on
  * the state of the bet and the dice.
  *
  *\param cDice The Dice.
  */

void Strategy::ResolveOneRollBets(std::list<Bet>::iterator &it, const Dice &cDice)
{
    if (it->IsFieldBet())                                   // Field Bet?
    {
        if (cDice.IsField())                                    // Field Bet and Field Number Roll?
        {
            it->SetPoint(cDice.RollValue());                        // Set the bet point to last dice roll value to make CalculatePayoff work
            m_cMoney.Increment(it->Wager() + it->CalculatePayoff());// Payoff
            it->SetWon();
        }
    }

    else if (it->IsAny7Bet())                                   // Any 7 Bet?
    {
        if (cDice.IsSeven())                                    // Any 7 Bet and Field Number Roll?
        {
            it->SetPoint(cDice.RollValue());                        // Set the bet point to last dice roll value to make CalculatePayoff work
            m_cMoney.Increment(it->Wager() + it->CalculatePayoff());// Payoff
            it->SetWon();
        }
    }

    else if (it->IsAnyCrapsBet())                           // Any Craps Bet?
    {
        if (cDice.IsCraps())
        {
            it->SetPoint(cDice.RollValue());
            m_cMoney.Increment(it->Wager() + it->CalculatePayoff());
            it->SetWon();
        }
    }

    else if (it->IsCraps2Bet())                           // Craps 2 Bet?
    {
        if (cDice.IsTwo())
        {
            it->SetPoint(cDice.RollValue());
            m_cMoney.Increment(it->Wager() + it->CalculatePayoff());
            it->SetWon();
        }
    }

    else if (it->IsCraps3Bet())                           // Craps 3 Bet?
    {
        if (cDice.IsThree())
        {
            it->SetPoint(cDice.RollValue());
            m_cMoney.Increment(it->Wager() + it->CalculatePayoff());
            it->SetWon();
        }
    }

    else if (it->IsYo11Bet())                           // Yo 11 Bet?
    {
        if (cDice.IsEleven())
        {
            it->SetPoint(cDice.RollValue());
            m_cMoney.Increment(it->Wager() + it->CalculatePayoff());
            it->SetWon();
        }
    }

    else if (it->IsCraps12Bet())                           // Craps 12 Bet?
    {
        if (cDice.IsTweleve())
        {
            it->SetPoint(cDice.RollValue());
            m_cMoney.Increment(it->Wager() + it->CalculatePayoff());
            it->SetWon();
        }
    }

    if (!it->Won())                                // If Bet was not resolved, set it to Lost
        it->SetLost();
}

/**
  * Check to see if Strategy is still playing.
  *
  * If Strategy has run out of money to wager, it stops playing.
  * If Strategy has gained significant winnings, it stop playing.
  *
  *\return Whether the Stategy is still playing
  */

bool Strategy::StillPlaying() const
{
    bool bStillPlaying = true;

    // If using the PlayForNumberRolls configuration, play until number of
    // rolls acheived
    if (m_nPlayForNumberOfRolls > 0)
    {
        if (m_nPlayForNumberOfRolls == m_nNumberOfRolls)
            bStillPlaying = false;
    }
    // Else play until bankroll meets targes
    else
    {
        // If bankroll is out of money and no more bets are on the table,
        // then stop playing
        if ((m_cMoney.Bankroll() < m_cWager.StandardWager()) && (m_lBets.empty()))
            bStillPlaying = false;

        // If bankroll has significant winnings and no more bets are on the table,
        // then stop playing
        if ((m_cMoney.HasSignificantWinnings()) && (m_lBets.empty()))
            bStillPlaying = false;
    }

    return (bStillPlaying);
}

/**
  * Update statistcs on the last simluation run.
  *
  * Record the win or loss and the nuber of dice rolls.
  *
  */

void Strategy::UpdateStatistics()
{
    ++m_nTimesStrategyRun;

    // If using the PlayForNumberRolls configuration, a win is achieved when
    // the current bankroll is greater than the initial bankroll
    if (m_nPlayForNumberOfRolls > 0)
    {
        if (m_cMoney.Bankroll() >= m_cMoney.InitialBankroll())
        {
            ++m_nTimesStrategyWon;

            m_nWinBankrollTotal += m_cMoney.Bankroll();
            if (m_cMoney.Bankroll() < m_nWinBankrollMin) m_nWinBankrollMin = m_cMoney.Bankroll();
            if (m_cMoney.Bankroll() > m_nWinBankrollMax) m_nWinBankrollMax = m_cMoney.Bankroll();
        }
        else
        {
            ++m_nTimesStrategyLost;

            m_nLossBankrollTotal += m_cMoney.Bankroll();
            if (m_cMoney.Bankroll() < m_nLossBankrollMin) m_nLossBankrollMin = m_cMoney.Bankroll();
            if (m_cMoney.Bankroll() > m_nLossBankrollMax) m_nLossBankrollMax = m_cMoney.Bankroll();
        }
    }
    // Else if Strategy still has money, consider it a win.
    else
    {
        if (m_cMoney.Bankroll() > m_cWager.StandardWager())
        {
            ++m_nTimesStrategyWon;

            m_nWinRollsTotal += m_nNumberOfRolls;
            if (m_nNumberOfRolls < m_nWinRollsMin) m_nWinRollsMin = m_nNumberOfRolls;
            if (m_nNumberOfRolls > m_nWinRollsMax) m_nWinRollsMax = m_nNumberOfRolls;
        }
        else
        {
            ++m_nTimesStrategyLost;

            m_nLossRollsTotal += m_nNumberOfRolls;
            if (m_nNumberOfRolls < m_nLossRollsMin) m_nLossRollsMin = m_nNumberOfRolls;
            if (m_nNumberOfRolls > m_nLossRollsMax) m_nLossRollsMax = m_nNumberOfRolls;
        }
    }
}

/**
  * Resets the Strategy for a new simulation run.
  *
  * Sets to zero the counts.  Resets the standard wager and odds.
  * Calls Money.Reset().
  *
  */

void Strategy::Reset()
{
    // Bet Counters - Sync with Strategy.h
    m_nNumberOfPassBetsMade         = 0;
    m_nNumberOfComeBetsMade         = 0;
    m_nNumberOfDontPassBetsMade     = 0;
    m_nNumberOfDontComeBetsMade     = 0;
    m_nNumberOfPlaceBetsMade        = 0;
    m_bPutBetMade                  = false;
    m_nNumberOfBig6BetsMade         = 0;
    m_nNumberOfBig8BetsMade         = 0;
    m_nNumberOfHard4BetsMade        = 0;
    m_nNumberOfHard6BetsMade        = 0;
    m_nNumberOfHard8BetsMade        = 0;
    m_nNumberOfHard10BetsMade       = 0;

    m_nNumberOfRolls         = 0;

    m_fOdds  = m_fStandardOdds;

    m_lBets.clear();

    m_cMoney.Reset();
    m_cWager.Reset();
    m_cQualifiedShooter.Reset();
}

/**
  * Generates a muster of the strategy.
  *
  * Prints a listing of the Strategy's settings.
  *
  */

void Strategy::Muster() const
{
        const int left = 26;
        const int right = 25;

        std::cout << std::setw(left) << std::right << "Name: "                << Name()                                   << std::endl;
        std::cout << std::setw(left) << std::right << "Description: "         << Description()                            << std::endl;


        std::cout << std::setw(left) << std::right << "Pass Bet: "            << m_nNumberOfPassBetsAllowed               <<
                     std::setw(right) << std::right << "  Standard Wager: "    << m_cWager.StandardWager()                 << std::endl;

        std::cout << std::setw(left) << std::right << "Come Bets: "           << m_nNumberOfComeBetsAllowed               <<
                     std::setw(right) << std::right << "  Full Wager: "        << std::boolalpha << m_bFullWager           << std::endl;

        std::cout << std::setw(left) << std::right << "Come Odds Working: "   << std::boolalpha << m_bComeOddsWorking     <<
                     std::setw(right) << std::right << "  Initial Bankroll: "  << m_cMoney.InitialBankroll()               << std::endl;

        std::cout << std::setw(left) << std::right << "Dont Pass Bet: "       << m_nNumberOfDontPassBetsAllowed           <<
                     std::setw(right) << std::right << "  Standard Odds: "     << m_fStandardOdds                          << std::endl;

        std::cout << std::setw(left) << std::right << "Dont Come Bets: "      << m_nNumberOfDontComeBetsAllowed           << std::endl;

        std::cout << std::setw(left) << std::right << "Place Bets: "          << m_nNumberOfPlaceBetsAllowed              <<
                     std::setw(right) << std::right << "  Place Preferred: "   << m_nPreferredPlaceBet                     << std::endl;

        std::cout << std::setw(left) << std::right << "Place After Come: "    << std::boolalpha << m_bPlaceAfterCome      << std::endl;
        std::cout << std::setw(left) << std::right << "Place Made At Once: "  << m_nNumberOfPlaceBetsMadeAtOnce           << std::endl;


        std::cout << std::setw(left) << std::right << "Field Bets: "          << std::boolalpha << m_bFieldBetsAllowed    <<
                     std::setw(right) << std::right << "  Trace Results: "     << std::boolalpha << m_bTrace              << std::endl;

        std::cout << std::setw(left) << std::right << "Play For Number of Rolls: " << m_nPlayForNumberOfRolls            << std::endl;

        std::cout << std::setw(left) << std::right << "Sig. Win. Mult.: ";
        if (m_nPlayForNumberOfRolls < 1 )
            std::cout << m_cMoney.SignificantWinningsMultiple() << std::endl;
        else
            std::cout << "NOT USED" << std::endl;

        int nSigWin = m_cMoney.SignificantWinnings();
        if (nSigWin > 0 && m_nPlayForNumberOfRolls < 1)
        {
            std::cout << std::setw(left) << std::right << "Sig. Winnings: "       << m_cMoney.SignificantWinnings()       << std::endl;
        }
        else
        {
            std::cout << std::setw(left) << std::right << "Sig. Winnings: "       << "NOT USED"                           << std::endl;
        }

        std::cout << std::setw(left) << std::right << "Odds Prog. Method: ";

        if (!IsUsingOddsProgession())
            std::cout << "NOT USED" << std::endl;
        else
        {
            if (IsArithmeticOddsProgression())
                std::cout << "Arithmetic" << std::endl;
            else
                std::cout << "Geometric" << std::endl;
        }

        std::cout << std::setw(left) << std::right << "Wager Progression Method: " << m_cWager.WagerProgressionMethod() << std::endl;
        std::cout << std::setw(left) << std::right << "Bet Modification Method: " << m_cWager.BetModificationMethod() << std::endl;
        std::cout << std::setw(left) << std::right << "Qual. Shooter Method: "     << m_cQualifiedShooter.Method() << std::endl;
        std::cout << std::setw(left) << std::right << "Qual. Shooter Method Cnt: " << m_cQualifiedShooter.Count() << std::endl;

        std::cout << std::endl;
}

/**
  * Report results of simulation.
  *
  * Presents details of the strategy's success, including win percentage
  * and a average rolls to win or lose.
  *
  */

void Strategy::Report()
{
    if (m_bReportHeaderPrinted == false)
    {
        m_bReportHeaderPrinted = true;

        if (m_nPlayForNumberOfRolls < 1)
        {
            std::cout << std::setw(78) << std::right << "Rolls to Win " <<
                std::setw(18) << std::right << "Rolls to Lose" <<
                std::endl;
        }
        else
        {
            std::cout << std::setw(82) << std::right << "Bankroll at Win " <<
                std::setw(17) << std::right << "Bankroll at Loss" <<
                std::endl;
        }

        std::cout << std::setw(40) << std::right << "Name " <<
            std::setw(6)  << std::left << "Runs" <<
            std::setw(5)  << std::left << "Wins" <<
            std::setw(7)  << std::left << "Losses" <<
            std::setw(6)  << std::left << "Win %" <<
            std::setw(5)  << std::right << "Avg" <<
            std::setw(5)  << std::right << "Min" <<
            std::setw(7)  << std::right << "Max" <<
            std::setw(5)  << std::right << "Avg" <<
            std::setw(5)  << std::right << "Min" <<
            std::setw(7)  << std::right << "Max" <<
            std::endl;
    }

    float fWinPercentage  = 0.0;

    if (m_nTimesStrategyRun > 0)
        fWinPercentage = (static_cast<float>(m_nTimesStrategyWon) / static_cast<float>(m_nTimesStrategyRun)) * 100;

    std::cout << std::setw(40) << std::right << m_sName << " " <<
                 std::setw(6)  << std::left << m_nTimesStrategyRun <<
                 std::setw(5)  << std::left << m_nTimesStrategyWon <<
                 std::setw(7)  << std::left << m_nTimesStrategyLost <<
                 std::showpoint  << std::setprecision(3) << fWinPercentage;

    if (m_nPlayForNumberOfRolls < 1)
    {
        int nWinRollsAvg      = 0;
        int nLossRollsAvg     = 0;

        if (m_nTimesStrategyWon > 0)
            nWinRollsAvg    = (static_cast<int>(m_nWinRollsTotal / m_nTimesStrategyWon));

        if (m_nTimesStrategyLost > 0)
            nLossRollsAvg    = (static_cast<int>(m_nLossRollsTotal / m_nTimesStrategyLost));

        if (m_nWinRollsMin == INT_MAX) m_nWinRollsMin = 0;
        if (m_nWinRollsMax == INT_MIN) m_nWinRollsMax = 0;
        if (m_nLossRollsMin == INT_MAX) m_nLossRollsMin = 0;
        if (m_nLossRollsMax == INT_MIN) m_nLossRollsMax = 0;

        std::cout << std::setw(6) << std::right << nWinRollsAvg <<
                 std::setw(5)  << std::right << m_nWinRollsMin <<
                 std::setw(7)  << std::right << m_nWinRollsMax <<
                 std::setw(5)  << std::right << nLossRollsAvg <<
                 std::setw(5)  << std::right << m_nLossRollsMin <<
                 std::setw(7)  << std::right << m_nLossRollsMax <<
                 std::endl;
    }
    else
    {
        int nWinBankrollAvg   = 0;
        int nLossBankrollAvg = 0;

        if (m_nTimesStrategyWon > 0)
            nWinBankrollAvg = (static_cast<int>(m_nWinBankrollTotal / m_nTimesStrategyWon));

        if (m_nTimesStrategyLost > 0)
            nLossBankrollAvg = (static_cast<int>(m_nLossBankrollTotal / m_nTimesStrategyLost));

        if (m_nWinBankrollMin == INT_MAX) m_nWinBankrollMin = 0;
        if (m_nWinBankrollMax == INT_MIN) m_nWinBankrollMax = 0;
        if (m_nLossBankrollMin == INT_MAX) m_nLossBankrollMin = 0;
        if (m_nLossBankrollMax == INT_MIN) m_nLossBankrollMax = 0;

        std::cout << std::setw(6) << std::right << nWinBankrollAvg <<
                 std::setw(5)  << std::right << m_nWinBankrollMin <<
                 std::setw(7)  << std::right << m_nWinBankrollMax <<
                 std::setw(5)  << std::right << nLossBankrollAvg <<
                 std::setw(5)  << std::right << m_nLossBankrollMin <<
                 std::setw(7)  << std::right << m_nLossBankrollMax <<
                 std::endl;
    }
}
