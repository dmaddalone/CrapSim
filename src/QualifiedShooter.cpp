#include "QualifiedShooter.h"

QualifiedShooter::QualifiedShooter()
{
    m_mQualificationMethod["NO_METHOD"]                        = QualificationMethod::QM_NO_METHOD;
    m_mQualificationMethod["5COUNT"]                           = QualificationMethod::QM_5COUNT;
    m_mQualificationMethod["AFTER_POINT_MADE"]                 = QualificationMethod::QM_AFTER_POINT_MADE;
    m_mQualificationMethod["AFTER_LOSING_FIELD_THREE_TIMES"]   = QualificationMethod::QM_AFTER_LOSING_FIELD_THREE_TIMES;
    m_mQualificationMethod["AFTER_FIVE_NON_SEVEN_ROLLS"]       = QualificationMethod::QM_AFTER_FILE_NON_SEVEN_ROLLS;
}

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
