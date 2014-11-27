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
 * \brief The QualifiedShoort class represents how a shooter is qualified,
 * if a Strategy uses such methods.
 *
 */

#ifndef QUALIFIEDSHOOTER_H
#define QUALIFIEDSHOOTER_H

#include <locale>
#include <map>
#include <string>
#include "CrapSimException.h"
#include "Dice.h"
#include "Table.h"

// Used to identify a qualification method
enum class QualificationMethod
{
    QM_NO_METHOD,
    QM_5COUNT,
    QM_AFTER_POINT_ESTABLISHED,
    QM_AFTER_POINT_MADE,
    QM_AFTER_LOSING_FIELD_THREE_TIMES_IN_A_ROW,
    QM_AFTER_FIVE_NON_SEVEN_ROLLS,
    QM_AFTER_N_LOSING_FIELD_N_TIMES_IN_A_ROW, // TODO
    QM_AFTER_N_NON_7_ROLLS_IN_A_ROW,
    QM_AFTER_N_2_ROLLS_IN_A_ROW,
    QM_AFTER_N_3_ROLLS_IN_A_ROW,
    QM_AFTER_N_4_ROLLS_IN_A_ROW,
    QM_AFTER_N_5_ROLLS_IN_A_ROW,
    QM_AFTER_N_6_ROLLS_IN_A_ROW,
    QM_AFTER_N_7_ROLLS_IN_A_ROW, //TODO
    QM_AFTER_N_8_ROLLS_IN_A_ROW,
    QM_AFTER_N_9_ROLLS_IN_A_ROW,
    QM_AFTER_N_10_ROLLS_IN_A_ROW,
    QM_AFTER_N_11_ROLLS_IN_A_ROW,
    QM_AFTER_N_12_ROLLS_IN_A_ROW
};

class QualifiedShooter
{
    public:
        QualifiedShooter();

        // Set and return the qualification method
        void SetMethod(std::string sMethod);
        std::string Method() const;
        // Set the count for the qualification method
        void SetCount(int i)
            { if (i >= 1) m_nQualificationCount = i;
              else throw CrapSimException("QualifiedShooter::SetCount", std::to_string(i)); }
        int Count() const                       { return (m_nQualificationCount); }
        // Set when the qualification method starts, usually with a new shooter
        void SetStopWithShooter(bool b)         { m_bQualificationStopsWithShooter = b; }
        // Update the qualifications the shooter
        void QualifyTheShooter(const Table &cTable, const Dice &cDice);
        // Check if shooter has been qualified
        bool ShooterQualified() const           { return (m_bShooterQualified); }

        // Reset the class - meant to be called before a new Simulation run
        void Reset();

    private:
        // Set defaults
        QualificationMethod m_ecQualificationMethod = QualificationMethod::QM_NO_METHOD;
        bool m_bShooterQualified                    = true;
        int  m_nQualificationCount                  = 1;
        bool m_bQualificationStopsWithShooter       = true;
        int  m_nCounter                             = 0;

        // Map to associate the strings with the enum values
        std::map<std::string, QualificationMethod> m_mQualificationMethod;

        // The 5 Count method
        bool Method5Count(const Table &cTable, const Dice &cDice);

        // The After Point Established method
        bool MethodAfterPointEstablished(const Table &cTable, const Dice &cDice);

        // The After Point Made method
        bool MethodAfterPointMade(const Table &cTable, const Dice &cDice);

        // The After Losing Field Three Times method
        bool MethodAfterLosingFieldThreeTimesInARow(const Table &cTable, const Dice &cDice);

        // The After Five Non Seven Rolls method
        bool MethodAfterFiveNon7Rolls(const Dice &cDice);

        // The After N (number) of X (value) rolls in a row
        bool MethodAfterNXRollsInARow(const Table &cTable, const Dice &cDice, int nNumber);
};

#endif // QUALIFIEDSHOOTER_H
