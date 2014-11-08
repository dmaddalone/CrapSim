#include "QualifiedShooter.h"

/*
QualifiedShooter::QualifiedShooter()
{
    //ctor
}

QualifiedShooter::~QualifiedShooter()
{
    //dtor
}
*/

bool QualifiedShooter::QualifyShooter(const Table &cTable, const Dice &cDice)
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
            return (MethodAfterLosingFieldThreeTimes(cDice));
            break;
        default:
            break;
    }

    return (true);
}

bool QualifiedShooter::Method5Count(const Table &cTable, const Dice &cDice)
{
    bool bQualified = false;

    if (cTable.NewShooter())
    {
        m_n5Count = 0;
        if (cDice.IsPoint()) m_n5Count++;
    }
    else if ((m_n5Count >= 1) && (m_n5Count < 4))
    {
        m_n5Count++;
    }
    else if (m_n5Count == 4)
    {
        if (cDice.IsPoint())
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
