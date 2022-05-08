/*
    SPDX-License-Identifier: GPL-2.0-or-later
    SPDX-FileCopyrightText: 2021 Helguli <helguli@yahoo.com>
*/

#include "cellgroup.h"

CellGroup::CellGroup(int group_id) :
    m_group_id(group_id) {}

CellGroup::CellGroup(int group_id, int start_row, int start_col, int end_row, int end_col) :
    m_start_row(start_row),
    m_start_col(start_col),
    m_end_row(end_row),
    m_end_col(end_col),
    m_group_id(group_id) {}

void CellGroup::setStartRow(int row)
{
    m_start_row = row;
    emit startRowChanged();
}

void CellGroup::setStartCol(int col)
{
    m_start_col = col;
    emit startColChanged();
}

void CellGroup::setEndRow(int row)
{
    m_end_row = row;
    emit endRowChanged();
}

void CellGroup::setEndCol(int col)
{
    m_end_col = col;
    emit endColChanged();
}

std::vector<QColor> CellGroup::m_colors
{
    QColor::fromRgb(0xff, 0xff, 0x00, OPACITY),
    QColor::fromRgb(0x00, 0xff, 0x00, OPACITY),
    QColor::fromRgb(0x00, 0xff, 0xff, OPACITY),
    QColor::fromRgb(0xff, 0x00, 0x00, OPACITY),
    QColor::fromRgb(0xff, 0x00, 0xff, OPACITY),
    QColor::fromRgb(0x00, 0x00, 0xff, OPACITY),
    QColor::fromRgb(0x00, 0xff, 0x7f, OPACITY),
    QColor::fromRgb(0xff, 0x7f, 0x00, OPACITY),
    QColor::fromRgb(0x7f, 0x00, 0xff, OPACITY),
    QColor::fromRgb(0x7f, 0xff, 0x00, OPACITY),
    QColor::fromRgb(0x00, 0x7f, 0xff, OPACITY),
    QColor::fromRgb(0xff, 0x00, 0x7f, OPACITY),
    QColor::fromRgb(0xff, 0xa5, 0x00, OPACITY),
    QColor::fromRgb(0xff, 0x45, 0x00, OPACITY),
    QColor::fromRgb(0x9a, 0xcd, 0x32, OPACITY),
    QColor::fromRgb(0x00, 0xfa, 0x9a, OPACITY),
    QColor::fromRgb(0x00, 0xbf, 0xff, OPACITY),
    QColor::fromRgb(0x32, 0xcd, 0x32, OPACITY),
    QColor::fromRgb(0xff, 0x8c, 0x00, OPACITY),
    QColor::fromRgb(0xad, 0xff, 0x2f, OPACITY)
};
