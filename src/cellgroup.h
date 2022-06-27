/*
    SPDX-License-Identifier: GPL-2.0-or-later
    SPDX-FileCopyrightText: 2021 Helguli <helguli@yahoo.com>
*/

#pragma once

#include <QObject>
#include <QColor>
#include <qqml.h>

#define COLORS_SIZE 20
#define OPACITY 180

class CellGroup : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int    startRow READ startRow WRITE setStartRow NOTIFY startRowChanged)
    Q_PROPERTY(int    startCol READ startCol WRITE setStartCol NOTIFY startColChanged)
    Q_PROPERTY(int    endRow   READ endRow   WRITE setEndRow   NOTIFY endRowChanged)
    Q_PROPERTY(int    endCol   READ endCol   WRITE setEndCol   NOTIFY endColChanged)
    Q_PROPERTY(int    groupId  READ groupId                    CONSTANT)
    Q_PROPERTY(QColor color    READ color                      CONSTANT)
    QML_ELEMENT
public:
    CellGroup() : m_group_id(0){};
    CellGroup(int group_id);
    CellGroup(int group_id, int start_row, int start_col, int end_row, int end_col);
    virtual ~CellGroup() {}
    int    startRow() {return m_start_row;}
    int    startCol() {return m_start_col;}
    int    endRow()   {return m_end_row;}
    int    endCol()   {return m_end_col;}
    int    groupId()  {return m_group_id;}
    QColor color()    {return m_colors[m_group_id % COLORS_SIZE];}
public slots:
    void setStartRow(int row);
    void setStartCol(int col);
    void setEndRow(int row);
    void setEndCol(int col);
signals:
    void startRowChanged();
    void startColChanged();
    void endRowChanged();
    void endColChanged();
private:
    int m_start_row;
    int m_start_col;
    int m_end_row;
    int m_end_col;
    const int m_group_id;
    static std::vector<QColor> m_colors;
};
