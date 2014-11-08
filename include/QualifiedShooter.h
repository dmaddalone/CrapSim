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

enum class QualificationMethod
{
    QM_NO_METHOD,
    QM_5COUNT,
    QM_AFTER_POINT_MADE,
    QM_AFTER_LOSING_FIELD_THREE_TIMES,
    QM_AFTER_FILE_NON_SEVEN_ROLLS
};


class QualifiedShooter
{
    public:
        //QualifiedShooter();
        //virtual ~QualifiedShooter();
        void SetMethod(QualificationMethod qm)  { m_ecQualificationMethod = qm; }
        QualificationMethod Method() const      { return (m_ecQualificationMethod); }
        bool QualifyingTheShooter() const       { return (m_ecQualificationMethod != QualificationMethod::QM_NO_METHOD); }
        bool ShooterQualified() const           { return (m_bShooterQualified); }

        bool QualifyShooter(const Table &cTable, const Dice &cDice);

    private:
        QualificationMethod m_ecQualificationMethod = QualificationMethod::QM_NO_METHOD;
        bool m_bShooterQualified                    = false;

        bool Method5Count(const Table &cTable, const Dice &cDice);
        int m_n5Count = 0;

        bool MethodAfterPointMade(const Table &cTable);

        bool MethodAfterLosingFieldThreeTimes(const Table &cTable, Dice &cDice);
        int m_nLosingFieldInARow = 0;

        bool MethodAfterFiveNon7Rolls(const Table &cTable, const Dice &cDice);
};

#endif // QUALIFIEDSHOOTER_H
