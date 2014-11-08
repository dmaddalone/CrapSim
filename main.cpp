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
#include <cassert>
#include <climits>
#include <cfloat>
#include "Die.h"
#include "Dice.h"
#include "Table.h"
#include "Money.h"
#include "Bet.h"
#include "Strategy.h"
#include "Simulation.h"
#include "CDataFile.h"

using std::cerr;
using std::cout;
using std::endl;

void CreateStrategy(const std::string &sStrategy, CDataFile &cConfigFile, const int nAllInitialBankroll, const int nAllStandardWager, const float fAllSWM, const int nAllSignificantWinnings, Simulation &cSim)
{
    // Read config file for parameters this Strategy
    std::string sName                   = cConfigFile.GetString("Name", sStrategy);
    std::string sDescription            = cConfigFile.GetString("Description", sStrategy);
    int         nStandardWager          = cConfigFile.GetInt("StandardWager", sStrategy);
    bool        bFullWager              = cConfigFile.GetBool("FullWager", sStrategy);
    int         nInitialBankroll        = cConfigFile.GetInt("InitialBankroll", sStrategy);
    std::string sPredefined             = cConfigFile.GetString("Predefined", sStrategy);
    int         nPassBet                = cConfigFile.GetInt("PassBet", sStrategy);
    int         nDontPassBet            = cConfigFile.GetInt("DontPassBet", sStrategy);
    int         nComeBets               = cConfigFile.GetInt("ComeBets", sStrategy);
    int         nDontComeBets           = cConfigFile.GetInt("DontComeBets", sStrategy);
    int         nPlaceBets              = cConfigFile.GetInt("PlaceBets", sStrategy);
    int         nPlaceBetsMadeAtOnce    = cConfigFile.GetInt("PlaceBetsMadeAtOnce", sStrategy);
    bool        bPlaceAfterCome         = cConfigFile.GetBool("PlaceAfterCome", sStrategy);
    int         nPlacePreferred         = cConfigFile.GetInt("PlacePreferred");
    int         nPlaceBetUnits          = cConfigFile.GetInt("PlaceBetUnits",sStrategy);
    bool        bFieldBet               = cConfigFile.GetBool("FieldBet", sStrategy);
    int         nFieldBetUnits          = cConfigFile.GetInt("FieldBetUnits",sStrategy);
    bool        bBig6Bet                = cConfigFile.GetBool("Big6Bet", sStrategy);
    bool        bBig8Bet                = cConfigFile.GetBool("Big8Bet", sStrategy);
    float       fStandardOdds           = cConfigFile.GetFloat("StandardOdds", sStrategy);
    int         nSignificantWinnings    = cConfigFile.GetInt("SignificantWinnings", sStrategy);
    float       fSWM                    = cConfigFile.GetFloat("SWM", sStrategy);
    bool        bComeOddsWorking        = cConfigFile.GetBool("ComeOddsWorking", sStrategy);
    bool        bOddsProgression        = cConfigFile.GetBool("OddsProgression", sStrategy);
    std::string sOddsProgressionMethod  = cConfigFile.GetString("OddsProgressionMethod", sStrategy);
    bool        bTrackResults           = cConfigFile.GetBool("TrackResults", sStrategy);

    // Set initial bankroll.  Use AllStrategies setting if individual strategy's InitialBankroll is not set.
    if (nInitialBankroll== INT_MIN) nInitialBankroll = nAllInitialBankroll;
    //if (nInitialBankroll <= 0) // If InitialBankroll still not set, exit with error
    //{
    //    cerr << "ERROR: InitialBankroll not set: " << nInitialBankroll << endl;
    //    cerr << "Exiting" << endl;
    //    exit(EXIT_FAILURE);
    //}

    // Set standard wager.  Use AllStrategies setting if individual strategy's StandardWager is not set.
    if (nStandardWager == INT_MIN) nStandardWager = nAllStandardWager;
    if (nStandardWager <= 0) // If InitialBankroll still not set, exit with error
    {
        cerr << "ERROR (main): StandardWager not set: " << nStandardWager << endl;
        cerr << "Exiting" << endl;
        exit(EXIT_FAILURE);
    }

    //Create Strategy
    Strategy cStrategy(sName, sDescription, nInitialBankroll, nStandardWager, bTrackResults);

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
            cerr << "ERROR (main): Unknown Predefined setting: " << sPredefined << endl;
            cerr << "Exiting" << endl;
            exit (EXIT_FAILURE);
        }
    }

    // Pass choice to make Full Wagers to Strategy
    cStrategy.SetFullWager(bFullWager);

    // If Name is blank, use the Strategy section name
    if (cStrategy.Name() == "") cStrategy.SetName(sStrategy);

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

    // Pass choice to make Place bets only after Come bets to Strategy
    cStrategy.SetPlaceAfterCome(bPlaceAfterCome);

    // If preferred Place bet number has been set, pass to Strategy
    if (nPlacePreferred != INT_MIN) cStrategy.SetPlacePreferred(nPlacePreferred);

    // If the number of bet units for Place bets has been set, pass to Strategy
    if (nPlaceBetUnits != INT_MIN) cStrategy.SetPlaceBetUnits(nPlaceBetUnits);

    // Pass Field Bet to Strategy; default is false
    cStrategy.SetFieldBetAllowed(bFieldBet);

    // If the number of bet units for Place bets has been set, pass to Strategy
    if (nFieldBetUnits != INT_MIN) cStrategy.SetFieldBetUnits(nFieldBetUnits);

    // Pass Big 6 Bet to Strategy; default is false
    cStrategy.SetBig6BetAllowed(bBig6Bet);

    // Pass Big 8 Bet to Strategy; default is false
    cStrategy.SetBig8BetAllowed(bBig8Bet);

    // If Standard Odds has been set, pass to Strategy
    if (fStandardOdds != FLT_MIN) cStrategy.SetStandardOdds(fStandardOdds);

    // Pass choice to make odds working on a table come out roll to Strategy
    cStrategy.SetComeOddsWorking(bComeOddsWorking);

    // Set significant winnings multiple.  Use AllStrategies setting if individual strategy's SWM is not set.
    if (fSWM == FLT_MIN) fSWM = fAllSWM;
    // If Significant Winngings Multiple has been set, pass to Strategy
    if (fSWM != FLT_MIN) cStrategy.SetSignificantWinningsMultiple(fSWM);

    // Set significant winnings.  Use AllStrategies setting if individual strategy's Significant Winnings is not set.
    if (nSignificantWinnings == INT_MIN) nSignificantWinnings = nAllSignificantWinnings;
    // If Significant Winngings Multiple has been set, pass to Strategy
    if (nSignificantWinnings != INT_MIN) cStrategy.SetSignificantWinnings(nSignificantWinnings);

    // If Use Confidence is set to true, pass Use Confidence to Strategy; default is false
    // And check for confidence type of Arithmetic or Geometric.  If neither is set, exit with error.
    if (bOddsProgression)
    {
        cStrategy.UseOddsProgression(bOddsProgression);

        std::locale loc;
        for (std::string::size_type iii = 0; iii < sOddsProgressionMethod.length(); iii++)
            sOddsProgressionMethod[iii] = std::toupper(sOddsProgressionMethod[iii], loc);

        if (sOddsProgressionMethod == "ARITHMETIC") cStrategy.SetOddsProgressionMethodArithmetic();
        else if (sOddsProgressionMethod == "GEOMETRIC") cStrategy.SetOddsProgressionMethodGeometric();
        else
        {
            cerr << "ERROR (main): Unknown OddsProgressionMethod setting: " << sOddsProgressionMethod << endl;
            cerr << "Exiting" << endl;
            exit (EXIT_FAILURE);
        }
    }

    // Add Strategy to Simulation
    cSim.AddStrategy(cStrategy);
}

int CrapsSimulation(std::string sINIFile)
{
    cout << "Craps Simulation" << endl;

    Simulation  cSim;
    Table       cTable;

    if (sINIFile.empty())
    {
        // TODO: Make a utility or expand into exception management
        cerr << "ERROR (main): No configuration file." << endl;
        cerr << "Exiting" << endl;
        exit (EXIT_FAILURE);
    }

    CDataFile cConfigFile(sINIFile);

    std::string sTableType          = cConfigFile.GetString("Type", "Table");
    int nMinimumWager               = cConfigFile.GetInt("MinimumWager", "Table");
    int nMaximumWager               = cConfigFile.GetInt("MaximumWager", "Table");

    int nDefaultInitialBankroll     = cConfigFile.GetInt("InitialBankroll", "DefaultStrategy");
    int nDefaultStandardWager       = cConfigFile.GetInt("StandardWager", "DefaultStrategy");
    float fDefaultSWM               = cConfigFile.GetFloat("SWM", "DefaultStrategy");
    int nDefaultSignificantWinnings = cConfigFile.GetInt("SignificantWinnings", "DefaultStrategy");

    int nNumberOfRuns               = cConfigFile.GetInt("Runs", "Simulation");
    bool bMusterReport              = cConfigFile.GetBool("Muster", "Simulation");
    bool bTally                     = cConfigFile.GetBool("Tally", "Simulation");

    if (nNumberOfRuns <= 0)
    {
        cerr << "ERROR (main): Number of simulation runs not set." << endl;
        cerr << "Exiting" << endl;
        exit (EXIT_FAILURE);
    }

    if (!sTableType.empty())
    {
        if (!cTable.SetTableType(sTableType))
        {
            cerr << "ERROR (main): Unknown Table Type: " << sTableType << endl;
            cerr << "Exiting" << endl;
            exit (EXIT_FAILURE);
        }
    }

    if (nMinimumWager != INT_MIN) cTable.SetMinimumBet(nMinimumWager);
    if (nMaximumWager != INT_MIN) cTable.SetMaximumBet(nMaximumWager);
    cSim.AddTable(cTable);

    if (cConfigFile.CheckSectionName("Strategy1"))  CreateStrategy("Strategy1", cConfigFile, nDefaultInitialBankroll, nDefaultStandardWager, fDefaultSWM, nDefaultSignificantWinnings, cSim);
    if (cConfigFile.CheckSectionName("Strategy2"))  CreateStrategy("Strategy2", cConfigFile, nDefaultInitialBankroll, nDefaultStandardWager, fDefaultSWM, nDefaultSignificantWinnings, cSim);
    if (cConfigFile.CheckSectionName("Strategy3"))  CreateStrategy("Strategy3", cConfigFile, nDefaultInitialBankroll, nDefaultStandardWager, fDefaultSWM, nDefaultSignificantWinnings, cSim);
    if (cConfigFile.CheckSectionName("Strategy4"))  CreateStrategy("Strategy4", cConfigFile, nDefaultInitialBankroll, nDefaultStandardWager, fDefaultSWM, nDefaultSignificantWinnings, cSim);
    if (cConfigFile.CheckSectionName("Strategy5"))  CreateStrategy("Strategy5", cConfigFile, nDefaultInitialBankroll, nDefaultStandardWager, fDefaultSWM, nDefaultSignificantWinnings, cSim);
    if (cConfigFile.CheckSectionName("Strategy6"))  CreateStrategy("Strategy6", cConfigFile, nDefaultInitialBankroll, nDefaultStandardWager, fDefaultSWM, nDefaultSignificantWinnings, cSim);
    if (cConfigFile.CheckSectionName("Strategy7"))  CreateStrategy("Strategy7", cConfigFile, nDefaultInitialBankroll, nDefaultStandardWager, fDefaultSWM, nDefaultSignificantWinnings, cSim);
    if (cConfigFile.CheckSectionName("Strategy8"))  CreateStrategy("Strategy8", cConfigFile, nDefaultInitialBankroll, nDefaultStandardWager, fDefaultSWM, nDefaultSignificantWinnings, cSim);
    if (cConfigFile.CheckSectionName("Strategy9"))  CreateStrategy("Strategy9", cConfigFile, nDefaultInitialBankroll, nDefaultStandardWager, fDefaultSWM, nDefaultSignificantWinnings, cSim);
    if (cConfigFile.CheckSectionName("Strategy10")) CreateStrategy("Strategy10", cConfigFile, nDefaultInitialBankroll, nDefaultStandardWager, fDefaultSWM, nDefaultSignificantWinnings, cSim);

    // Stop CDataFile cConfigFile from Save() and writing out the file
    cConfigFile.Clear();

    cSim.Run(nNumberOfRuns, bMusterReport, bTally);
    cSim.Report();

    return (0);
}

int main(int argc, char* argv[])
{
    if (argc > 2)
    {
        cerr << "Usage: CrapSim [FILE]" << endl;
        exit (EXIT_FAILURE);
    }

    return (CrapsSimulation(argv[1]));
}


