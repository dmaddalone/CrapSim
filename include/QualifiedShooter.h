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

#include "Dice.h"
#include "Table.h"

// Used to identify a qualification method
enum class QualificationMethod
{
    QM_NO_METHOD,
    QM_5COUNT,
    QM_AFTER_POINT_MADE,
    QM_AFTER_LOSING_FIELD_THREE_TIMES,
    QM_AFTER_FIVE_NON_SEVEN_ROLLS,
    QM_AFTER_N_LOSING_FIELD_ROLLS_IN_A_ROW,
    QM_AFTER_N_NON_7_ROLLS_IN_A_ROW,
    QM_AFTER_N_2_ROLLS_IN_A_ROW,
    QM_AFTER_N_3_ROLLS_IN_A_ROW,
    QM_AFTER_N_4_ROLLS_IN_A_ROW,
    QM_AFTER_N_5_ROLLS_IN_A_ROW,
    QM_AFTER_N_6_ROLLS_IN_A_ROW,
    QM_AFTER_N_7_ROLLS_IN_A_ROW,
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
        bool SetMethod(std::string sMethod);
        std::string Method();
        // Set the count for the qualification method
        void SetCount(int i)                    { m_nQualificationCount = i; }
        // Set when the qualification method starts, usually with a new shooter
        void SetStopWithShooter(bool b)         {m_bQualificationStopsWithShooter = b;}
        // Check whether we are qualifying a shooter
        bool QualifyingTheShooter() const       { return (m_ecQualificationMethod != QualificationMethod::QM_NO_METHOD); }
        // Check whether the shooter has been qualified
        bool ShooterQualified(const Table &cTable, const Dice &cDice);

    private:
        // Set defaults
        QualificationMethod m_ecQualificationMethod = QualificationMethod::QM_NO_METHOD;
        bool m_bShooterQualified                    = false;
        int  m_nQualificationCount                  = 0;
        bool m_bQualificationStopsWithShooter       = true;

        // Map to associate the strings with the enum values
        std::map<std::string, QualificationMethod> m_mQualificationMethod;

        // The 5 Count method
        bool Method5Count(const Table &cTable, const Dice &cDice);
        int m_n5Count = 0;
        // The After Point Made method
        bool MethodAfterPointMade(const Table &cTable, const Dice &cDice);
        // The After Losing Field Three Times method
        bool MethodAfterLosingFieldThreeTimes(const Table &cTable, const Dice &cDice);
        int m_nLosingFieldInARow = 0;
        // The After Five Non Seven Rolls method
        bool MethodAfterFiveNon7Rolls(const Dice &cDice);
        int m_nNon7InARow = 0;

        bool MethodAfterNXRolls(const Table &cTable, const Dice &cDice, int nNumber);
        int m_nCounter = 0;
};

#endif // QUALIFIEDSHOOTER_H
