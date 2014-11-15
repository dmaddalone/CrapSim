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
  *
  * \return True if qualification method is known, false otherwise.
  */

bool QualifiedShooter::SetMethod(std::string sMethod)
{
    if (sMethod.empty())
        return (false);

    std::locale loc;
    for (std::string::size_type iii = 0; iii < sMethod.length(); iii++)
        sMethod[iii] = std::toupper(sMethod[iii], loc);

    std::map<std::string, QualificationMethod>::iterator it = m_mQualificationMethod.find(sMethod);

    if (it == m_mQualificationMethod.end()) return (false);

    m_ecQualificationMethod = it->second;
    return (true);
}

/**
  * Return the qualification method for a shooter in string format.
  *
  * Loops the known qualification methods and compares to this Strategy's
  * qualification emthod.  When found, return correspnding string. The
  * string is set in the map of strings and QualificationMethod types.
  *
  * \return String representing the qualification method.
  */

std::string QualifiedShooter::Method()
{
    std::string sMethod("Unknown");

    for (std::map<std::string, QualificationMethod>::iterator it = m_mQualificationMethod.begin(); it != m_mQualificationMethod.end(); it++)
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
  * Return the table type in string format.
  *
  * Depending on the set qualification method, calls the corresponding
  * qualification method.  The qualification methods are based  on the Table
  * class, specifically NewShooter(), and the Dice class.
  *
  * \param cTable The Table.
  * \param cDice The Dice.
  *
  * \return True if the shooter has qualified, false otherwise.  Default is
  * true.
  */

//bool QualifiedShooter::QualifyTheShooter(const Table &cTable, const Dice &cDice)
void QualifiedShooter::QualifyTheShooter(const Table &cTable, const Dice &cDice)
{
    switch (m_ecQualificationMethod)
    {
        case QualificationMethod::QM_NO_METHOD:
            //return (m_bShooterQualified);
            break;
        case QualificationMethod::QM_5COUNT:
            //return (Method5Count(cTable, cDice));
            Method5Count(cTable, cDice);
            break;
        case QualificationMethod::QM_AFTER_POINT_MADE:
            //return (MethodAfterPointMade(cTable, cDice));
            MethodAfterPointMade(cTable, cDice);
            break;
        case QualificationMethod::QM_AFTER_LOSING_FIELD_THREE_TIMES_IN_A_ROW:
            //return (MethodAfterLosingFieldThreeTimesInARow(cTable, cDice));
            MethodAfterLosingFieldThreeTimesInARow(cTable, cDice);
            break;
        case QualificationMethod::QM_AFTER_FIVE_NON_SEVEN_ROLLS:
            //return (MethodAfterFiveNon7Rolls(cDice));
            MethodAfterFiveNon7Rolls(cDice);
            break;
        case QualificationMethod::QM_AFTER_N_2_ROLLS_IN_A_ROW:
            //return (MethodAfterNXRollsInARow(cTable, cDice, 2));
            MethodAfterNXRollsInARow(cTable, cDice, 2);
            break;
        case QualificationMethod::QM_AFTER_N_3_ROLLS_IN_A_ROW:
            //return (MethodAfterNXRollsInARow(cTable, cDice, 3));
            MethodAfterNXRollsInARow(cTable, cDice, 3);
            break;
        case QualificationMethod::QM_AFTER_N_4_ROLLS_IN_A_ROW:
            //return (MethodAfterNXRollsInARow(cTable, cDice, 4));
            MethodAfterNXRollsInARow(cTable, cDice, 4);
            break;
        case QualificationMethod::QM_AFTER_N_5_ROLLS_IN_A_ROW:
            //return (MethodAfterNXRollsInARow(cTable, cDice, 5));
            MethodAfterNXRollsInARow(cTable, cDice, 5);
            break;
        case QualificationMethod::QM_AFTER_N_6_ROLLS_IN_A_ROW:
            //return (MethodAfterNXRollsInARow(cTable, cDice, 6));
            MethodAfterNXRollsInARow(cTable, cDice, 6);
            break;
        case QualificationMethod::QM_AFTER_N_8_ROLLS_IN_A_ROW:
            //return (MethodAfterNXRollsInARow(cTable, cDice, 8));
            MethodAfterNXRollsInARow(cTable, cDice, 8);
            break;
        case QualificationMethod::QM_AFTER_N_9_ROLLS_IN_A_ROW:
            //return (MethodAfterNXRollsInARow(cTable, cDice, 9));
            MethodAfterNXRollsInARow(cTable, cDice, 9);
            break;
        case QualificationMethod::QM_AFTER_N_10_ROLLS_IN_A_ROW:
            //return (MethodAfterNXRollsInARow(cTable, cDice, 10));
            MethodAfterNXRollsInARow(cTable, cDice, 10);
            break;
        case QualificationMethod::QM_AFTER_N_11_ROLLS_IN_A_ROW:
            //return (MethodAfterNXRollsInARow(cTable, cDice, 11));
            MethodAfterNXRollsInARow(cTable, cDice, 11);
            break;
        case QualificationMethod::QM_AFTER_N_12_ROLLS_IN_A_ROW:
            //return (MethodAfterNXRollsInARow(cTable, cDice, 12));
            MethodAfterNXRollsInARow(cTable, cDice, 12);
            break;
        default:
            break;
    }

    //return (true);
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
        m_n5Count = 0;
        if (cDice.IsAPointNumber()) m_n5Count++;
    }
    else if ((m_n5Count >= 1) && (m_n5Count < 4))
    {
        if (!cDice.IsSeven()) m_n5Count++;
        if (cDice.IsSeven() && cTable.IsComingOutRoll()) m_n5Count++;
    }
    else if (m_n5Count == 4)
    {
        if (cDice.IsAPointNumber())
        {
            m_n5Count++;
            m_bShooterQualified = true;
        }
    }
    else if (m_n5Count == 5)
    {
        if (!cDice.IsSeven()) m_bShooterQualified = true;
        if (cDice.IsSeven() && cTable.IsComingOutRoll()) m_bShooterQualified = true;
    }

    std::cout << "DEBUG: m_n5Count=" << m_n5Count << "\t";

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
            m_nLosingFieldInARow++;
        }
        else if (cDice.IsField())
        {
            m_nLosingFieldInARow = 0;
        }
        else
        {
            m_nLosingFieldInARow++;
        }
    }
    else if (cTable.NewShooter() && !cTable.IsComingOutRoll())
    {
        if (cDice.IsSeven())
        {
            m_nLosingFieldInARow = 0;
        }
        else if (cDice.IsField())
        {
            if (m_nLosingFieldInARow < 3) m_nLosingFieldInARow = 0;
        }
        else
        {
            m_nLosingFieldInARow++;
        }
    }
    else if (!cTable.NewShooter() && cTable.IsComingOutRoll())
    {
        if (cDice.IsSeven())
        {
            m_nLosingFieldInARow++;
        }
        else if (cDice.IsField())
        {
            if (m_nLosingFieldInARow < 3) m_nLosingFieldInARow = 0;
        }
        else
        {
            m_nLosingFieldInARow++;
        }
    }
    else
    {
        if (cDice.IsSeven())
        {
            m_nLosingFieldInARow = 0;
        }
        else if (cDice.IsField())
        {
            if (m_nLosingFieldInARow < 3) m_nLosingFieldInARow = 0;
        }
        else
        {
            m_nLosingFieldInARow++;
        }
    }

    if (m_nLosingFieldInARow >= 3) m_bShooterQualified = true;

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
        m_nNon7InARow ++;
    }
    else
    {
        m_nNon7InARow = 0;
    }

    if (m_nNon7InARow >= 5) m_bShooterQualified = true;

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
