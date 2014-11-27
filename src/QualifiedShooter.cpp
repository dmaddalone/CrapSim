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
#include "QualifiedShooter.h"

/**
  * Construct a QualifiedShooter.
  *
  * Sets up a map of strings and qualification methods.
  */

QualifiedShooter::QualifiedShooter()
{
    m_mQualificationMethod["NO_METHOD"]                               = QualificationMethod::QM_NO_METHOD;
    m_mQualificationMethod["5COUNT"]                                  = QualificationMethod::QM_5COUNT;
    m_mQualificationMethod["AFTER_POINT_ESTABLISHED"]                 = QualificationMethod::QM_AFTER_POINT_ESTABLISHED;
    m_mQualificationMethod["AFTER_POINT_MADE"]                        = QualificationMethod::QM_AFTER_POINT_MADE;
    m_mQualificationMethod["AFTER_LOSING_FIELD_THREE_TIMES_IN_A_ROW"] = QualificationMethod::QM_AFTER_LOSING_FIELD_THREE_TIMES_IN_A_ROW;
    m_mQualificationMethod["AFTER_FIVE_NON_SEVEN_ROLLS"]              = QualificationMethod::QM_AFTER_FIVE_NON_SEVEN_ROLLS;
    m_mQualificationMethod["AFTER_N_2_ROLLS_IN_A_ROW"]                = QualificationMethod::QM_AFTER_N_2_ROLLS_IN_A_ROW;
    m_mQualificationMethod["AFTER_N_3_ROLLS_IN_A_ROW"]                = QualificationMethod::QM_AFTER_N_3_ROLLS_IN_A_ROW;
    m_mQualificationMethod["AFTER_N_4_ROLLS_IN_A_ROW"]                = QualificationMethod::QM_AFTER_N_4_ROLLS_IN_A_ROW;
    m_mQualificationMethod["AFTER_N_5_ROLLS_IN_A_ROW"]                = QualificationMethod::QM_AFTER_N_5_ROLLS_IN_A_ROW;
    m_mQualificationMethod["AFTER_N_6_ROLLS_IN_A_ROW"]                = QualificationMethod::QM_AFTER_N_6_ROLLS_IN_A_ROW;
    m_mQualificationMethod["AFTER_N_8_ROLLS_IN_A_ROW"]                = QualificationMethod::QM_AFTER_N_8_ROLLS_IN_A_ROW;
    m_mQualificationMethod["AFTER_N_9_ROLLS_IN_A_ROW"]                = QualificationMethod::QM_AFTER_N_9_ROLLS_IN_A_ROW;
    m_mQualificationMethod["AFTER_N_10_ROLLS_IN_A_ROW"]               = QualificationMethod::QM_AFTER_N_10_ROLLS_IN_A_ROW;
    m_mQualificationMethod["AFTER_N_11_ROLLS_IN_A_ROW"]               = QualificationMethod::QM_AFTER_N_11_ROLLS_IN_A_ROW;
    m_mQualificationMethod["AFTER_N_12_ROLLS_IN_A_ROW"]               = QualificationMethod::QM_AFTER_N_12_ROLLS_IN_A_ROW;
}

/**
  * Set the qualification method.
  *
  * Based on a passed string, sets the qualification method for a shooter.
  * The string is set in the map of strings and QualificationMethod types.
  *
  * \param sMethod The qualification method, e.g., 5COUNT, AFTER_POINT_MADE.
  */

void QualifiedShooter::SetMethod(std::string sMethod)
{
    std::locale loc;
    for (std::string::size_type iii = 0; iii < sMethod.length(); iii++)
        sMethod[iii] = std::toupper(sMethod[iii], loc);

    std::map<std::string, QualificationMethod>::iterator it = m_mQualificationMethod.find(sMethod);

    if (it != m_mQualificationMethod.end())
    {
        m_ecQualificationMethod = it->second;
    }
    else
    {
        throw CrapSimException("QualifiedShooter::SetMethod: unknown method", sMethod);
    }
}

/**
  * Return the qualification method for a shooter in string format.
  *
  * Loops the known qualification methods and compares to this Strategy's
  * qualification method.  When found, return correspnding string. The
  * string is set in the map of strings and QualificationMethod types.
  *
  * \return String representing the qualification method.
  */

std::string QualifiedShooter::Method() const
{
    std::string sMethod("Unknown");

    for (std::map<std::string, QualificationMethod>::const_iterator it = m_mQualificationMethod.begin(); it != m_mQualificationMethod.end(); it++)
    {
        if (m_ecQualificationMethod == it->second)
        {
            sMethod = it->first;
            break;
        }
    }

    return(sMethod);
}

/**
  * Qualify the shooter based on the qualification method.
  *
  * Depending on the set qualification method, calls the corresponding
  * qualification method.  The qualification methods are based  on the Table
  * class, specifically NewShooter(), and the Dice class.
  *
  * \param cTable The Table.
  * \param cDice The Dice.
  *
  */

void QualifiedShooter::QualifyTheShooter(const Table &cTable, const Dice &cDice)
{
    switch (m_ecQualificationMethod)
    {
        case QualificationMethod::QM_NO_METHOD:
            m_bShooterQualified = true;
            break;
        case QualificationMethod::QM_5COUNT:
            Method5Count(cTable, cDice);
            break;
        case QualificationMethod::QM_AFTER_POINT_ESTABLISHED:
            MethodAfterPointEstablished(cTable, cDice);
            break;
        case QualificationMethod::QM_AFTER_POINT_MADE:
            MethodAfterPointMade(cTable, cDice);
            break;
        case QualificationMethod::QM_AFTER_LOSING_FIELD_THREE_TIMES_IN_A_ROW:
            MethodAfterLosingFieldThreeTimesInARow(cTable, cDice);
            break;
        case QualificationMethod::QM_AFTER_FIVE_NON_SEVEN_ROLLS:
            MethodAfterFiveNon7Rolls(cDice);
            break;
        case QualificationMethod::QM_AFTER_N_2_ROLLS_IN_A_ROW:
            MethodAfterNXRollsInARow(cTable, cDice, 2);
            break;
        case QualificationMethod::QM_AFTER_N_3_ROLLS_IN_A_ROW:
            MethodAfterNXRollsInARow(cTable, cDice, 3);
            break;
        case QualificationMethod::QM_AFTER_N_4_ROLLS_IN_A_ROW:
            MethodAfterNXRollsInARow(cTable, cDice, 4);
            break;
        case QualificationMethod::QM_AFTER_N_5_ROLLS_IN_A_ROW:
            MethodAfterNXRollsInARow(cTable, cDice, 5);
            break;
        case QualificationMethod::QM_AFTER_N_6_ROLLS_IN_A_ROW:
            MethodAfterNXRollsInARow(cTable, cDice, 6);
            break;
        case QualificationMethod::QM_AFTER_N_8_ROLLS_IN_A_ROW:
            MethodAfterNXRollsInARow(cTable, cDice, 8);
            break;
        case QualificationMethod::QM_AFTER_N_9_ROLLS_IN_A_ROW:
            MethodAfterNXRollsInARow(cTable, cDice, 9);
            break;
        case QualificationMethod::QM_AFTER_N_10_ROLLS_IN_A_ROW:
            MethodAfterNXRollsInARow(cTable, cDice, 10);
            break;
        case QualificationMethod::QM_AFTER_N_11_ROLLS_IN_A_ROW:
            MethodAfterNXRollsInARow(cTable, cDice, 11);
            break;
        case QualificationMethod::QM_AFTER_N_12_ROLLS_IN_A_ROW:
            MethodAfterNXRollsInARow(cTable, cDice, 12);
            break;
        default:
            throw CrapSimException("QualifiedShooter::QualifyTheShooter: unknown QualificationMethod");
            break;
    }
}

/**
  * Use the 5 Count method to qualifiy the shooter.
  *
  * The 5-Count is a method of slow-playing craps, as discussed in ’Golden
  * Touch Dice Control Revolution’ by Frank Scoblete and Dominator.
  *
  * Method:
  * 1) Begin counting when a new shooter rolls a point number.  This is the
  * 1-count.
  * 2) Count the next 3 rolls regardless of the number, unless the shooter
  * sevens out.  If so, go back to step 1.
  * 3) After the 4-count, count the next roll if the shooter rolls a point.
  * this is the 5-count.  If a point number is not rolled, hold at the 4-count
  * until a point number is rolled.  If the new shooter sevens out, go to
  * step 1.
  * 4) If the 5-count is reached, the shooter is qualified.
  *
  * \param cTable The Table.
  * \param cDice The Dice.
  *
  * \return True if the shooter has qualified, false otherwise.
  */

bool QualifiedShooter::Method5Count(const Table &cTable, const Dice &cDice)
{
    m_bShooterQualified = false;

    if (cTable.NewShooter() && cTable.IsComingOutRoll())
    {
        m_nCounter = 0;
        if (cDice.IsAPointNumber()) m_nCounter++;
    }
    else if ((m_nCounter >= 1) && (m_nCounter < 4))
    {
        if (!cDice.IsSeven()) m_nCounter++;
        if (cDice.IsSeven() && cTable.IsComingOutRoll()) m_nCounter++;
    }
    else if (m_nCounter == 4)
    {
        if (cDice.IsAPointNumber())
        {
            m_nCounter++;
            m_bShooterQualified = true;
        }
    }
    else if (m_nCounter == 5)
    {
        if (!cDice.IsSeven()) m_bShooterQualified = true;
        if (cDice.IsSeven() && cTable.IsComingOutRoll()) m_bShooterQualified = true;
    }

    return (m_bShooterQualified);
}

/**
  * Consider a new shooter qualified after she has established a point.
  *
  * Method:
  * Once a shooter establishes the point, the shooter is qualified.
  *
  * \param cTable The Table.
  *
  * \return True if the shooter has qualified, false otherwise.
  */

bool QualifiedShooter::MethodAfterPointEstablished(const Table &cTable, const Dice &cDice)
{
    m_bShooterQualified = false;

    // If puck is OFF and Dice is a point number, shooter has established a point
    if (!cTable.IsPuckOn() && cDice.IsAPointNumber())
        m_bShooterQualified = true;

    return (m_bShooterQualified);
}

/**
  * Consider a new shooter qualified after she has made her first point.
  *
  * Method:
  * Once a shooter establishes and hits the point, the shooter is qualified.
  *
  * \param cTable The Table.
  *
  * \return True if the shooter has qualified, false otherwise.
  */

bool QualifiedShooter::MethodAfterPointMade(const Table &cTable, const Dice &cDice)
{
    m_bShooterQualified = false;

    if (cTable.NewShooter())
    {
        if (cTable.Point() == cDice.RollValue()) m_bShooterQualified = true;
    }
    else
    {
        if (cTable.IsComingOutRoll()) m_bShooterQualified = true;
        if (!cTable.IsComingOutRoll() && !cDice.IsSeven()) m_bShooterQualified = true;
    }

    return (m_bShooterQualified);
}

/**
  * Begin betting after a new shooter rolls three non Field numbers in a row.
  *
  * Method:
  * 1) Beging counting with a new shooter.
  * 2) If a non Field number is rolled (a number where the Field loses),
  * count it.
  * 3) If three non Field numbers are rolled in a row, the shooter is
  * qualified.
  *
  * \param cTable The Table.
  * \param cDice The Dice.
  *
  * \return True if the shooter has qualified, false otherwise.
  */

bool QualifiedShooter::MethodAfterLosingFieldThreeTimesInARow(const Table &cTable, const Dice &cDice)
{
    m_bShooterQualified = false;

    if (cTable.NewShooter() && cTable.IsComingOutRoll())
    {
        if (cDice.IsSeven())
        {
            m_nCounter++;
        }
        else if (cDice.IsField())
        {
            m_nCounter = 0;
        }
        else
        {
            m_nCounter++;
        }
    }
    else if (cTable.NewShooter() && !cTable.IsComingOutRoll())
    {
        if (cDice.IsSeven())
        {
            m_nCounter = 0;
        }
        else if (cDice.IsField())
        {
            if (m_nCounter < 3) m_nCounter = 0;
        }
        else
        {
            m_nCounter++;
        }
    }
    else if (!cTable.NewShooter() && cTable.IsComingOutRoll())
    {
        if (cDice.IsSeven())
        {
            m_nCounter++;
        }
        else if (cDice.IsField())
        {
            if (m_nCounter < 3) m_nCounter = 0;
        }
        else
        {
            m_nCounter++;
        }
    }
    else
    {
        if (cDice.IsSeven())
        {
            m_nCounter = 0;
        }
        else if (cDice.IsField())
        {
            if (m_nCounter < 3) m_nCounter = 0;
        }
        else
        {
            m_nCounter++;
        }
    }

    if (m_nCounter >= 3) m_bShooterQualified = true;

    return (m_bShooterQualified);
}

/**
  * Begin betting after a new shooter rolls five non-7 rolls.
  *
  * Method:
  * 1) If a non 7 number is rolled beging counting.
  * 2) If five non 7 numbers are rolled in a row, the shooter is qualified.
  *
  * \param cTable The Table.
  * \param cDice The Dice.
  *
  * \return True if the shooter has qualified, false otherwise.
  */

bool QualifiedShooter::MethodAfterFiveNon7Rolls(const Dice &cDice)
{
    m_bShooterQualified = false;

    if (!cDice.IsSeven())
    {
        m_nCounter++;
    }
    else
    {
        m_nCounter = 0;
    }

    if (m_nCounter >= 5) m_bShooterQualified = true;

    return (m_bShooterQualified);
}

/**
  * Begin betting after N number of X values are rolled in a roll.  Works for
  * all values except 7.
  *
  * Method:
  * 1) If a X value is rolled, begin counting.
  * 2) If N number of X values are rolled in a row, the shooter is qualified.
  *
  * Qualification Methiod uses the m_bQualificationStopsWithShooter flag.  If
  * true, shooter is qualified until she sevens out.  If false, this is a
  * one-time betting qualification event.
  *
  * \param cTable The Table.
  * \param cDice The Dice.
  *
  * \return True if the shooter has qualified, false otherwise.
  */

bool QualifiedShooter::MethodAfterNXRollsInARow(const Table &cTable, const Dice &cDice, int nNumber)
{
    m_bShooterQualified = false;

    // If a new shooter on the come out, reset counter.
    if (cTable.NewShooter() && cTable.IsComingOutRoll())
    {
        m_nCounter = 0;
    }

    // If number hit, count it.
    if (cDice.RollValue() == nNumber)
    {
        m_nCounter++;
    }
    // Else if not the number AND shooter sevens out, reset counter
    else if (cDice.IsSeven() && cTable.Point() != 0)
    {
        m_nCounter = 0;
    }
    // Else if not the number AND haven't reached qualification count, reset counter
    else if (m_nCounter < m_nQualificationCount)
    {
        m_nCounter = 0;
    }

    // If qualifying shooter
    if (m_bQualificationStopsWithShooter)
    {
        // If counter meets or exceeds qualification count, shooter is qualified
        if (m_nCounter >= m_nQualificationCount)
        {
            m_bShooterQualified = true;
        }
    }
    // Else if one time qualification
    else
    {
        // If counter meets qualification count, qualify the shooter,
        // but reset the counter
        if (m_nCounter == m_nQualificationCount)
        {
            m_bShooterQualified = true;
            m_nCounter = 0;
        }
    }

    return (m_bShooterQualified);
}

/**
  * Reset the class.
  *
  * Reset values for a new simulation run.
  *
  */

void QualifiedShooter::Reset()
{
    m_nCounter = 0;
}
