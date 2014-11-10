#include "QualifiedShooter.h"

/**
  * Construct a QualifiedShooter.
  *
  * Sets up a map of strings and qualification methods.
  *
  */

QualifiedShooter::QualifiedShooter()
{
    m_mQualificationMethod["NO_METHOD"]                        = QualificationMethod::QM_NO_METHOD;
    m_mQualificationMethod["5COUNT"]                           = QualificationMethod::QM_5COUNT;
    m_mQualificationMethod["AFTER_POINT_MADE"]                 = QualificationMethod::QM_AFTER_POINT_MADE;
    m_mQualificationMethod["AFTER_LOSING_FIELD_THREE_TIMES"]   = QualificationMethod::QM_AFTER_LOSING_FIELD_THREE_TIMES;
    m_mQualificationMethod["AFTER_FIVE_NON_SEVEN_ROLLS"]       = QualificationMethod::QM_AFTER_FILE_NON_SEVEN_ROLLS;
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
  * qualification method.
  *
  * \param cTable The Table.
  * \param cDice The Dice.
  *
  * \return True if the shooter has qualified, false otherwise.  Default is
  * true.
  */

bool QualifiedShooter::ShooterQualified(const Table &cTable, const Dice &cDice)
{
    switch (m_ecQualificationMethod)
    {
        case QualificationMethod::QM_NO_METHOD:
            return (true);
            break;
        case QualificationMethod::QM_5COUNT:
            return (Method5Count(cTable, cDice));
            break;
        case QualificationMethod::QM_AFTER_POINT_MADE:
            return (MethodAfterPointMade(cTable));
            break;
        case QualificationMethod::QM_AFTER_LOSING_FIELD_THREE_TIMES:
            return (MethodAfterLosingFieldThreeTimes(cTable, cDice));
            break;
        default:
            break;
    }

    return (true);
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
    bool bQualified = false;

    if (cTable.NewShooter() && cTable.IsComingOutRoll())
    {
        m_n5Count = 0;
        if (cDice.IsAPointNumber()) m_n5Count++;
    }
    else if ((m_n5Count >= 1) && (m_n5Count < 4))
    {
        m_n5Count++;
    }
    else if (m_n5Count == 4)
    {
        if (cDice.IsAPointNumber())
        {
            m_n5Count++;
            bQualified = true;
        }
    }
    else if (m_n5Count == 5)
    {
        bQualified = true;
    }

    return (bQualified);
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

bool QualifiedShooter::MethodAfterPointMade(const Table &cTable)
{
    bool bQualified = false;

    if (!cTable.NewShooter())
    {
        bQualified = true;
    }
    else
    {
        bQualified = false;
    }

    return (bQualified);
}

/**
  * Begin betting after a new shooter rolls three non Field numbers in a row.
  *
  * Method:
  * 1) Begin counting with a new shooter.
  * 2) If a non Field numbers is rolled (a number where the Field loses)
  * beging counting.
  * 3) If three non Field numbers are rolled in a row, the shooter is
  * qualified.
  *
  * \param cTable The Table.
  * \param cDice The Dice.
  *
  * \return True if the shooter has qualified, false otherwise.
  */

bool QualifiedShooter::MethodAfterLosingFieldThreeTimes(const Table &cTable, const Dice &cDice)
{
    bool bQualified = false;

    if (cTable.NewShooter())
    {
        m_nLosingFieldInARow = 0;
    }
    else if (cDice.IsField())
    {
        m_nLosingFieldInARow = 0;
    }
    else
    {
        m_nLosingFieldInARow++;

        if (m_nLosingFieldInARow == 3)
        {
            bQualified = true;
            m_nLosingFieldInARow = 0;
        }
    }

    return (bQualified);
}

/**
  * Begin betting after a new shooter rolls five non-7 rolls.
  *
  * Method:
  * 1) Begin counting with a new shooter.
  * 2) If a non 7 number is rolled beging counting.
  * 3) If five non 7 numbers are rolled in a row, the shooter is qualified.
  *
  * \param cTable The Table.
  * \param cDice The Dice.
  *
  * \return True if the shooter has qualified, false otherwise.
  */

bool QualifiedShooter::MethodAfterFiveNon7Rolls(const Table &cTable, const Dice &cDice)
{
    bool bQualified = false;

    if (cTable.NewShooter())
    {
        m_n7InARow = 0;
    }
    else if (!cDice.IsSeven())
    {
        if (m_n7InARow == 5)
        {
            bQualified = true;
        }
        else
        {
            m_n7InARow = 0;
        }
    }
    else
    {
        m_n7InARow++;

        if (m_n7InARow == 5)
        {
            bQualified = true;
        }
    }

    return (bQualified);
}
