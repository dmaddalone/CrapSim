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

#include <iostream>
#include <climits>
#include <cfloat>
#include "CrapSimException.h"
#include "Die.h"
#include "Dice.h"
#include "Table.h"
#include "Money.h"
#include "Bet.h"
#include "Strategy.h"
#include "Simulation.h"
#include "CDataFile.h"
#include "CrapSimVersion.h"

/**
  * Create a Strategy.
  *
  * Construct the Simulation and the Table.  Read the configuration file for
  * all non-Strategy configuration sections. Create strategies.  Run
  * simulations.  Report.
  *
  *\param sStrategy Section name of strategy from the configuration file
  *\param cConfigFile Configfile class containing key, value settings
  *\param nDefaultInitBank Default initial bankroll
  *\param nDefaultStdWager Default standard wager
  *\param fDefaultSWM Default significant winnings multiple
  *\param nDefaultSigWin Default significant winnings amount
  *\param cSim The Simulation class
  */

void CreateStrategy(const std::string sStrategy, CDataFile &cConfigFile, const int nDefaultInitBank, const int nDefaultStdWager, const float fDefaultSWM, const int nDefaultSigWin, Simulation &cSim)
{
    //
    // Read config file for parameters this Strategy
    //

    // Basic
    std::string sName                   = cConfigFile.GetString("Name", sStrategy);
    std::string sDescription            = cConfigFile.GetString("Description", sStrategy);

    // Money
    int         nStandardWager          = cConfigFile.GetInt("StandardWager", sStrategy);
    bool        bFullWager              = cConfigFile.GetBool("FullWager", sStrategy);
    int         nInitialBankroll        = cConfigFile.GetInt("InitialBankroll", sStrategy);
    int         nSignificantWinnings    = cConfigFile.GetInt("SignificantWinnings", sStrategy);
    float       fSWM                    = cConfigFile.GetFloat("SWM", sStrategy);

    // Predefined Strategies
    std::string sPredefined             = cConfigFile.GetString("Predefined", sStrategy);

    // Types and numbers of bets allowed
    int         nPassBet                = cConfigFile.GetInt("PassBet", sStrategy);
    int         nDontPassBet            = cConfigFile.GetInt("DontPassBet", sStrategy);
    int         nComeBets               = cConfigFile.GetInt("ComeBets", sStrategy);
    int         nDontComeBets           = cConfigFile.GetInt("DontComeBets", sStrategy);

    int         nPlaceBets              = cConfigFile.GetInt("PlaceBets", sStrategy);
    int         nPlaceBetsMadeAtOnce    = cConfigFile.GetInt("PlaceBetsMadeAtOnce", sStrategy);
    bool        bPlaceAfterCome         = cConfigFile.GetBool("PlaceAfterCome", sStrategy);
    std::string sPlaceAfterCome         = cConfigFile.GetString("PlaceAfterCome", sStrategy); // Needed for Predefined strategies
    int         nPlacePreferred         = cConfigFile.GetInt("PlacePreferred");
    int         nPlaceBetUnits          = cConfigFile.GetInt("PlaceBetUnits",sStrategy);
    bool        bPlaceWorking           = cConfigFile.GetBool("PlaceWorking", sStrategy);

    bool        bPutBet                 = cConfigFile.GetBool("PutBet", sStrategy);

    bool        bFieldBet               = cConfigFile.GetBool("FieldBet", sStrategy);
    int         nFieldBetUnits          = cConfigFile.GetInt("FieldBetUnits",sStrategy);

    bool        bBig6Bet                = cConfigFile.GetBool("Big6Bet", sStrategy);
    bool        bBig8Bet                = cConfigFile.GetBool("Big8Bet", sStrategy);
    bool        bAny7Bet                = cConfigFile.GetBool("Any7Bet", sStrategy);
    bool        bAnyCrapsBet            = cConfigFile.GetBool("AnyCrapsBet", sStrategy);
    bool        bCraps2Bet              = cConfigFile.GetBool("Craps2Bet", sStrategy);
    bool        bCraps3Bet              = cConfigFile.GetBool("Craps3Bet", sStrategy);
    bool        bYo11Bet                = cConfigFile.GetBool("Yo11Bet", sStrategy);
    bool        bCraps12Bet             = cConfigFile.GetBool("Craps12Bet", sStrategy);

    // Odds settings
    float       fStandardOdds           = cConfigFile.GetFloat("StandardOdds", sStrategy);
    bool        bComeOddsWorking        = cConfigFile.GetBool("ComeOddsWorking", sStrategy);
    std::string sOddsProgressionMethod  = cConfigFile.GetString("OddsProgressionMethod", sStrategy);

    // Wager progression settings
    std::string sWagerProgressionMethod = cConfigFile.GetString("WagerProgressionMethod", sStrategy);

    // Shooter qualification methods
    std::string sQualifiedShooterMethod = cConfigFile.GetString("QualifiedShooterMethod", sStrategy);
    int         nQualifiedShooterMethodCount = cConfigFile.GetInt("QualifiedShooterMethodCount", sStrategy);

    // Tracking results (used for debugging)
    bool        bTrace                  = cConfigFile.GetBool("Trace", sStrategy);

    //
    // Set the Strategy's parameters
    //

    // Set initial bankroll.  Use DefaultStrategy setting if individual strategy's InitialBankroll is not set.
    if (nInitialBankroll == INT_MIN) nInitialBankroll = nDefaultInitBank;

    // Set standard wager.  Use DefaultStrategy setting if individual strategy's StandardWager is not set.
    if (nStandardWager == INT_MIN) nStandardWager = nDefaultStdWager;

    //Create Strategy
    Strategy cStrategy(sName, sDescription, nInitialBankroll, nStandardWager, bTrace);

    // If Predefined is defined, use it to modify Strategy.  Exit if entry is unknown.
    if (sPredefined != "")
    {
        // TODO: make this a utility
        std::locale loc;
        for (std::string::size_type iii = 0; iii < sPredefined.length(); iii++)
            sPredefined[iii] = std::toupper(sPredefined[iii], loc);

        if      (sPredefined == "ELEMENTARY")   cStrategy.SetElementary();
        else if (sPredefined == "CONSERVATIVE") cStrategy.SetConservative();
        else if (sPredefined == "CONVENTIONAL") cStrategy.SetConventional();
        else if (sPredefined == "AGGRESSIVE")   cStrategy.SetAggressive();
        else  // Don't know what the Predefined parameter is.  Exit with error.
        {
            std::cerr << "ERROR (main): Unknown Predefined setting: " << sPredefined << std::endl;
            std::cerr << "Exiting" << std::endl;
            exit (EXIT_FAILURE);
        }
    }

    // If Name is blank, use the Strategy section name
    if (cStrategy.Name() == "") cStrategy.SetName(sStrategy);

    // Pass choice to make Full Wagers to Strategy
    cStrategy.SetFullWager(bFullWager);
    // Set significant winnings multiple.  Use DefaultStrategy setting if individual strategy's SWM is not set.
    if (fSWM == FLT_MIN) fSWM = fDefaultSWM;
    // If Significant Winngings Multiple has been set, pass to Strategy
    if (fSWM != FLT_MIN) cStrategy.SetSignificantWinningsMultiple(fSWM);
    // Set significant winnings.  Use DefaultStrategy setting if individual strategy's Significant Winnings is not set.
    if (nSignificantWinnings == INT_MIN) nSignificantWinnings = nDefaultSigWin;
    // If Significant Winngings Multiple has been set, pass to Strategy
    if (nSignificantWinnings != INT_MIN) cStrategy.SetSignificantWinnings(nSignificantWinnings);

    // If Pass Bet has been set, pass to Strategy
    if (nPassBet != INT_MIN) cStrategy.SetPassBet(nPassBet);

    // If Dont Pass Bet has been set, pass to Strategy
    if (nDontPassBet != INT_MIN) cStrategy.SetDontPassBet(nDontPassBet);

    // If number of Come Bets has been set, pass to Strategy
    if (nComeBets != INT_MIN) cStrategy.SetNumberOfComeBets(nComeBets);

    // If number of Dont Come Bets has been set, pass to Strategy
    if (nDontComeBets != INT_MIN) cStrategy.SetNumberOfDontComeBets(nDontComeBets);

    // If number of Place Bets has been set, pass to Strategy
    if (nPlaceBets != INT_MIN) cStrategy.SetNumberOfPlaceBets(nPlaceBets);

    // If number of Place Bets has been set, pass to Strategy
    if (nPlaceBetsMadeAtOnce != INT_MIN) cStrategy.SetNumberOfPlaceBetsMadeAtOnce(nPlaceBetsMadeAtOnce);

    // If PlaceAfterCome was set (checked using string) then updated settings
    if (!sPlaceAfterCome.empty())
    {
        // Pass choice to make Place bets only after Come bets to Strategy
        cStrategy.SetPlaceAfterCome(bPlaceAfterCome);
    }

    // If preferred Place bet number has been set, pass to Strategy
    if (nPlacePreferred != INT_MIN) cStrategy.SetPlacePreferred(nPlacePreferred);

    // If the number of bet units for Place bets has been set, pass to Strategy
    if (nPlaceBetUnits != INT_MIN) cStrategy.SetPlaceBetUnits(nPlaceBetUnits);

    // Pass Place bets working to Strategy
    cStrategy.SetPlaceWorking(bPlaceWorking);

    // Pass Put Bet to Strategy; default is false
    cStrategy.SetPutBetsAllowed(bPutBet);

    // Pass Field Bet to Strategy; default is false
    cStrategy.SetFieldBetAllowed(bFieldBet);

    // If the number of bet units for Place bets has been set, pass to Strategy
    if (nFieldBetUnits != INT_MIN) cStrategy.SetFieldBetUnits(nFieldBetUnits);

    // Pass Any 7 Bet to Strategy; default is false
    cStrategy.SetAny7BetAllowed(bAny7Bet);
    // Pass Any Craps Bet to Strategy; default is false
    cStrategy.SetAnyCrapsBetAllowed(bAnyCrapsBet);
    // Pass Any Craps Bet to Strategy; default is false
    cStrategy.SetCraps2BetAllowed(bCraps2Bet);
    // Pass Any Craps Bet to Strategy; default is false
    cStrategy.SetCraps3BetAllowed(bCraps3Bet);
    // Pass Any Craps Bet to Strategy; default is false
    cStrategy.SetYo11BetAllowed(bYo11Bet);
    // Pass Any Craps Bet to Strategy; default is false
    cStrategy.SetCraps12BetAllowed(bCraps12Bet);
    // Pass Big 6 Bet to Strategy; default is false
    cStrategy.SetBig6BetAllowed(bBig6Bet);
    // Pass Big 8 Bet to Strategy; default is false
    cStrategy.SetBig8BetAllowed(bBig8Bet);

    // If Standard Odds has been set, pass to Strategy
    if (fStandardOdds != FLT_MIN) cStrategy.SetStandardOdds(fStandardOdds);
    // Pass choice to make odds working on a table come out roll to Strategy
    cStrategy.SetComeOddsWorking(bComeOddsWorking);

    // If OddsProgression was set then updated Strategy
    if (!sOddsProgressionMethod.empty()) cStrategy.SetOddsProgressionMethod(sOddsProgressionMethod);

    // If WagerProgressionOnLossMethod was set then update Strategy
    if (!sWagerProgressionMethod.empty()) cStrategy.SetWagerProgressionMethod(sWagerProgressionMethod);

    // If Qualified Shooter Method was set then update Strategy
    if (!sQualifiedShooterMethod.empty()) cStrategy.SetQualifiedShooterMethod(sQualifiedShooterMethod);

    if (nQualifiedShooterMethodCount != INT_MIN) cStrategy.SetQualifiedShooterMethodCount(nQualifiedShooterMethodCount);

    // Add Strategy to Simulation
    cSim.AddStrategy(cStrategy);
}

/**
  * Simulation Driver.
  *
  * Construct the Simulation and the Table.  Read the configuration file for
  * all non-Strategy configuration sections. Create strategies.  Run
  * simulations.  Report.
  *
  *\param sINIFile Name of the INI configuration file
  *
  */

int CrapsSimulation(std::string sINIFile)
{
    std::cout << "Craps Simulation version " << CrapSimVersion::SemanticVersion() << " " << CrapSimVersion::DateVersion() << std::endl;

    // Construct Simualtion and Table
    Simulation  cSim;
    Table       cTable(5, 5000);

    // No config file, no simulation.
    if (sINIFile.empty())
    {
        // TODO: Make a utility or expand into exception management
        std::cerr << "ERROR (main): No configuration file." << std::endl;
        std::cerr << "Exiting" << std::endl;
        exit (EXIT_FAILURE);
    }

    // Open the configuration file and load the configuration.
    CDataFile cConfigFile(sINIFile);

    // Stop CDataFile cConfigFile from Save() and writing out (aka corrupting) the file
    cConfigFile.ClearDirty();

    // Set non-Strategy configuration items.
    std::string sTableType = cConfigFile.GetString("Type", "Table");
    int nMinimumWager      = cConfigFile.GetInt("MinimumWager", "Table");
    int nMaximumWager      = cConfigFile.GetInt("MaximumWager", "Table");

    int nDefaultInitBank   = cConfigFile.GetInt("InitialBankroll", "DefaultStrategy");
    int nDefaultStdWager   = cConfigFile.GetInt("StandardWager", "DefaultStrategy");
    float fDefaultSWM      = cConfigFile.GetFloat("SWM", "DefaultStrategy");
    int nDefaultSigWin     = cConfigFile.GetInt("SignificantWinnings", "DefaultStrategy");

    int nNumberOfRuns      = cConfigFile.GetInt("Runs", "Simulation");
    bool bMusterReport     = cConfigFile.GetBool("Muster", "Simulation");
    bool bTally            = cConfigFile.GetBool("Tally", "Simulation");

    // No simulation runs, no simulation,
    if (nNumberOfRuns <= 0)
    {
        std::cerr << "ERROR (main): Number of simulation runs not set." << std::endl;
        std::cerr << "Exiting" << std::endl;
        exit (EXIT_FAILURE);
    }

    // Set Table attributes and add it to the Simulation.
    if (nMinimumWager != INT_MIN) cTable.SetMinimumBet(nMinimumWager);
    if (nMaximumWager != INT_MIN) cTable.SetMaximumBet(nMaximumWager);

    if (!sTableType.empty())
    {
        if (!cTable.SetTableType(sTableType))
        {
            std::cerr << "ERROR (main): Unknown Table Type: " << sTableType << std::endl;
            std::cerr << "Exiting" << std::endl;
            exit (EXIT_FAILURE);
        }
    }

    cSim.AddTable(cTable);

    // Loop though possible Strategy sections (StrategyXX).  If it exists,
    // call function to create it.
    std::string sStrategyName;
    for (int iii = 1; iii <= 24; iii++)
    {
        sStrategyName = "Strategy" + std::to_string(iii);
        if (cConfigFile.CheckSectionName(sStrategyName))
            CreateStrategy(sStrategyName, cConfigFile, nDefaultInitBank, nDefaultStdWager, fDefaultSWM, nDefaultSigWin, cSim);
    }

    // Run the Simulation.
    cSim.Run(nNumberOfRuns, bMusterReport, bTally);

    // After action report.
    cSim.Report();

    return (0);
}

/**
  * Main
  *
  * Check for a configuration file then call the CrapSimulation driver.
  *
  */

int main(int argc, char* argv[])
{
    if (argc > 2)
    {
        std::cerr << "Usage: CrapSim [FILE]" << std::endl;
        exit (EXIT_FAILURE);
    }

    try
    {
        return (CrapsSimulation(argv[1]));
    }
    catch (const CrapSimException &e)
    {
        std::cerr << "ERROR" << std::endl;
        std::cerr << e.what() << " " << e.m_sArg << std::endl;
        std::cerr << "Terminating" << std::endl;
        exit(EXIT_FAILURE);
    }

}
