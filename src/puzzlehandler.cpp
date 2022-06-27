/*
    SPDX-License-Identifier: GPL-2.0-or-later
    SPDX-FileCopyrightText: 2021 Helguli <helguli@yahoo.com>
*/

#include "puzzlehandler.h"
#include <stdlib.h>
#include <math.h>
#include <iostream>

#define signum(x)  ((signbit(x) ?  -1 : 1))

PuzzleHandler::PuzzleHandler()
{
    srand(time(0));
    newPuzzle();
}

void PuzzleHandler::setRowSize(int row_size)
{
    if (m_row_size == row_size)
    {
        return;
    }
    m_row_size = row_size;
    emit rowSizeChanged();
}

void PuzzleHandler::setColSize(int col_size)
{
    if (m_col_size == col_size)
    {
        return;
    }
    m_col_size = col_size;
    emit colSizeChanged();
}

void PuzzleHandler::setSelectedCellId(int selected_cell_id)
{
    if (!(selected_cell_id < 0 || selected_cell_id >= m_row_size * m_col_size))
    {
        m_selected_cell_id = selected_cell_id;
        int row1 = m_selected_cell_id / m_col_size, col1 = m_selected_cell_id % m_col_size;
        int row2 = m_selection_start_id / m_col_size, col2 = m_selection_start_id % m_col_size;
        switch (m_puzzle_state)
        {
        case normal_mode:
            m_selection_start_id = selected_cell_id;
            break;
        case finished_mode:
            break;
        case select_mode:
            CellGroup* g = m_cell_groups.last();
            g->setStartRow(std::min(row1, row2));
            g->setStartCol(std::min(col1, col2));
            g->setEndRow(std::max(row1, row2));
            g->setEndCol(std::max(col1, col2));
            break;
        }
        emit selectedCellIdChanged();
    }
}

void PuzzleHandler::setPuzzleState(mode puzzle_state)
{
    m_puzzle_state = puzzle_state;
    if (puzzle_state == finished_mode)
    {
        emit puzzleFinished();
    }
    emit puzzleStateChanged();
}

void PuzzleHandler::returnPressed()
{
    switch (m_puzzle_state)
    {
    case normal_mode:
        enterSelectMode();
        break;
    case select_mode:
        exitSelectMode();
        break;
    case finished_mode:
        newPuzzle();
        break;
    }
}

void PuzzleHandler::mousePressed(int cell_id)
{
    switch (m_puzzle_state) {

    case normal_mode:
        setSelectedCellId(cell_id);
        enterSelectMode();
        break;
    case finished_mode:
        newPuzzle();
        break;
    case select_mode:
        setSelectedCellId(cell_id);
        exitSelectMode();
        break;
    }
}

void PuzzleHandler::mouseReleased()
{
    if (m_puzzle_state == select_mode && m_selection_start_id != m_selected_cell_id)
    {
        exitSelectMode();
    }
}

int PuzzleHandler::getValueAt(int index)
{
    if (m_table.size() < 1)
    {
        return 0;
    }
    return m_table[index % m_row_size][index / m_row_size];
}

void PuzzleHandler::setSize(int row_size, int col_size)
{
    setRowSize(row_size);
    setColSize(col_size);
}

void PuzzleHandler::newPuzzle()
{
    std::cout << "New puzzle" << std::endl;
    m_table.resize(m_row_size);
    m_solved_table.resize(m_row_size);
    m_puzzle_values.clear();
    m_selected_cell_id = 0;
    m_selection_start_id = 0;
    m_puzzle_state = normal_mode;
    for (int i = 0; i < m_row_size; i++)
    {
        m_table[i].resize(m_col_size);
        m_solved_table[i].resize(m_col_size);
    }
    for (int row = 0; row < m_row_size; row++)
    {
        for (int col = 0; col < m_col_size; col++)
        {
            m_table[row][col] = 0;
            m_solved_table[row][col] = 0;
        }
    }
    int group_id = -1;
    int empty_cells = m_row_size * m_col_size;
    while (empty_cells != 0)
    {
        if (createNewGroup(group_id, empty_cells))
        {
            group_id--;
        }
        empty_cells = hasEmptyCell();
    }
    for (auto& i: m_table)
    {
        for (auto j: i)
        {
            std::cout << j << "  ";
        }
        std::cout << std::endl;
    }
    for (auto& i: m_table)
    {
        for (auto j: i)
        {
            if (j < 0)
            {
                int count = countGroup(j);
                if (count > 0)
                {
                    degroup(j, count, rand() % count);
                    deleteGroup(j);
                }
            }
        }
    }
    for (auto& i: m_table)
    {
        for (auto j: i)
        {
            if (j > 0)
            {
                m_puzzle_values << QString::number(j);
            }
            else
            {
                m_puzzle_values << "";
            }
        }
    }
    while (!m_cell_groups.isEmpty())
    {
        delete m_cell_groups.take(m_cell_groups.firstKey());
    }
    emit cellGroupsChanged();
    emit puzzleStateChanged();
    emit selectedCellIdChanged();
    emit puzzleValuesChanged();

}

PuzzleHandler::~PuzzleHandler()
{
    while (!m_cell_groups.isEmpty())
    {
        delete m_cell_groups.take(m_cell_groups.firstKey());
    }
}

int PuzzleHandler::hasEmptyCell()
{
    return countGroup(0);
}

bool PuzzleHandler::createNewGroup(int group_id, int empty_cells)
{
    std::vector<int> selected_cell = getEmptyCell(rand() % empty_cells);
    if (isIsolatedCell(selected_cell[0], selected_cell[1]))
    {
        if (placeToGroup(selected_cell[0], selected_cell[1]))
        {
            return true;
        }
        else
        {
            deleteGroup(getNeighbouringGroupId(selected_cell[0], selected_cell[1]));
        }
    }
    bool horizontal = std::rand() % 2;
    int first_available = getAvailableSpace(horizontal, selected_cell[0], selected_cell[1], 1);
    int first_selected = ((rand() % first_available) + 1) * signum(first_available);
    int second_available = getAvailableSpace(!horizontal, selected_cell[0], selected_cell[1], first_selected);
    int max_territory = abs(first_selected * second_available);
    int second_selected = second_available;
    if (max_territory <= 1)
    {
        return false;
    }
    if (max_territory > 3)
    {
        int territory = 0;
        do
        {
            second_selected = ((rand() % second_available) + 1) * signum(second_available);
            territory = first_selected * second_selected;
        } while(abs(territory) <= 1 || abs(territory) > m_row_size * m_col_size / 4);
    }
    assignGroup(group_id, selected_cell[0], selected_cell[1], horizontal?second_selected:first_selected, horizontal?first_selected:second_selected);
    return true;
}

bool PuzzleHandler::assignGroup(int group_id, int start_row, int start_col, int row_size, int col_size)
{
    int row_size_direction = signum(row_size);
    row_size = abs(row_size);
    int col_size_direction = signum(col_size);
    col_size = abs(col_size);
    for (int i = 0; abs(i) < row_size; i += row_size_direction)
    {
        for (int j = 0; abs(j) < col_size; j += col_size_direction)
        {
            m_table[start_row + i][start_col + j] = group_id;
        }
    }
    return true;
}

void PuzzleHandler::deleteGroup(int group_id)
{
    for (int row = 0; row < m_row_size; row++)
    {
        for (int col = 0; col < m_col_size; col++)
        {
            if (m_table[row][col] == group_id)
            {
                m_table[row][col] = 0;
            }
        }
    }
}

int PuzzleHandler::getAvailableSpace(bool horizontal, int start_row, int start_col, int width)
{
    int count_positive = 0;
    int count_negative = 0;
    int width_sign = signum(width);
    if (horizontal)
    {
        for (int col = start_col; col < m_col_size; col++, count_positive++)
        {
            if (m_table[start_row][col] != 0)
            {
                break;
            }
            if (abs(width) > 1)
            {
                for (int i = width_sign; abs(i) < abs(width); i += width_sign)
                {
                    if (m_table[start_row + i][col] != 0)
                    {
                        goto label_col;
                    }
                }
            }
        }
        label_col:
        for (int col = start_col; col >= 0; col--, count_negative--)
        {
            if (m_table[start_row][col] != 0)
            {
                break;
            }
            if (abs(width) > 1)
            {
                for (int i = width_sign; abs(i) < abs(width); i += width_sign)
                {
                    if (m_table[start_row + i][col] != 0)
                    {
                        goto return_statement;
                    }
                }
            }
        }
    }
    else
    {
        for (int row = start_row; row < m_row_size; row++, count_positive++)
        {
            if (m_table[row][start_col] != 0)
            {
                break;
            }
            if (abs(width) > 1)
            {
                for (int i = width_sign; abs(i) < abs(width); i += width_sign)
                {
                    if (m_table[row][start_col + i] != 0)
                    {
                        goto label_row;
                    }
                }
            }
        }
        label_row:
        for (int row = start_row; row >= 0; row--, count_negative--)
        {
            if (m_table[row][start_col] != 0)
            {
                break;
            }
            if (abs(width) > 1)
            {
                for (int i = width_sign; abs(i) < abs(width); i += width_sign)
                {
                    if (m_table[row][start_col + i] != 0)
                    {
                        goto return_statement;
                    }
                }
            }
        }
    }
    return_statement:
    return (-count_negative < count_positive ? count_positive : count_negative);
}

bool PuzzleHandler::isIsolatedCell(int row, int col)
{
    return !((row != 0 && m_table[row - 1][col] == 0)
        || (col != 0 && m_table[row][col - 1] == 0)
        || (row != m_row_size - 1 && m_table[row + 1][col] == 0)
        || (col != m_col_size - 1 && m_table[row][col + 1] == 0));
}

bool PuzzleHandler::placeToGroup(int row, int col)
{
    if (row != 0 && getGroupWidth(row - 1, col) == 1)
    {
        m_table[row][col] = m_table[row - 1][col];
        return true;
    }
    if (row != m_row_size - 1 && getGroupWidth(row + 1, col) == 1)
    {
        m_table[row][col] = m_table[row + 1][col];
        return true;
    }
    if (col != 0 && getGroupHeight(row, col - 1) == 1)
    {
        m_table[row][col] = m_table[row][col - 1];
        return true;
    }
    if (col != m_col_size - 1 && getGroupHeight(row, col + 1) == 1)
    {
        m_table[row][col] = m_table[row][col + 1];
        return true;
    }
    return false;
}

void PuzzleHandler::degroup(int group_id, int group_count, int nth)
{
    int count = 0;
    for (int row = 0; row < m_row_size; row++)
    {
        for (int col = 0; col < m_col_size; col++)
        {

            if (m_table[row][col] == group_id)
            {
                if (count == nth)
                {
                    m_table[row][col] = group_count;
                }
                count++;
            }
        }
    }
}

int PuzzleHandler::countGroup(int group_id)
{
    int count = 0;
    for (int row = 0; row < m_row_size; row++)
    {
        for (int col = 0; col < m_col_size; col++)
        {
            if (m_table[row][col] == group_id)
            {
                count++;
            }
        }
    }
    return count;
}

int PuzzleHandler::getGroupHeight(int group_id)
{
    for (int row = 0; row < m_row_size; row++)
    {
        for (int col = 0; col < m_col_size; col++)
        {
            if (m_table[row][col] == group_id)
            {
                return getGroupHeight(row, col);
            }
        }
    }
    return 0;
}

int PuzzleHandler::getGroupHeight(int cell_row, int cell_col)
{
    int group_id = m_table[cell_row][cell_col];
    int count = 0;
    for (int row = cell_row; row < m_row_size; row++, count++)
    {
        if (m_table[row][cell_col] != group_id)
        {
            break;
        }
    }
    for (int row = cell_row; row >= 0; row--, count++)
    {
        if (m_table[row][cell_col] != group_id)
        {
            break;
        }
    }
    return count - 1;
}

int PuzzleHandler::getGroupWidth(int group_id)
{
    for (int row = 0; row < m_row_size; row++)
    {
        for (int col = 0; col < m_col_size; col++)
        {
            if (m_table[row][col] == group_id)
            {
                return getGroupWidth(row, col);
            }
        }
    }
    return 0;
}

int PuzzleHandler::getGroupWidth(int cell_row, int cell_col)
{
    int group_id = m_table[cell_row][cell_col];
    int count = 0;
    for (int col = cell_col; col < m_col_size; col++, count++)
    {
        if (m_table[cell_row][col] != group_id)
        {
            break;
        }
    }
    for (int col = cell_col; col >= 0; col--, count++)
    {
        if (m_table[cell_row][col] != group_id)
        {
            break;
        }
    }
    return count - 1;
}

std::vector<int> PuzzleHandler::getEmptyCell(int cell_num)
{
    for (int row = 0, count = 0; row < m_row_size; row++)
    {
        for (int col = 0; col < m_col_size; col++)
        {
            if (m_table[row][col] == 0)
            {
                if (cell_num == count)
                {
                    std::vector<int> v(2);
                    v[0] = row;
                    v[1] = col;
                    return v;
                }
                count++;
            }
        }
    }
    return std::vector<int>();
}

int PuzzleHandler::getNeighbouringGroupId(int cell_row, int cell_col)
{
    while (true)
    {
        switch (rand() % 4)
        {
            case 0:
                if (cell_row > 0)
                {
                    return m_table[cell_row - 1][cell_col];
                }
                break;
            case 1:
                if (cell_col > 0)
                {
                    return m_table[cell_row][cell_col - 1];
                }
                break;
            case 2:
                if (cell_row < m_row_size - 1)
                {
                    return m_table[cell_row + 1][cell_col];
                }
                break;
            case 3:
                if (cell_col < m_col_size - 1)
                {
                    return m_table[cell_row][cell_col + 1];
                }
                break;
        }
    }
}

void PuzzleHandler::enterSelectMode()
{
    CellGroup* g;
    int row = m_selected_cell_id / m_col_size;
    int col = m_selected_cell_id % m_col_size;
    if (m_solved_table[row][col])
    {
        delete m_cell_groups.take(m_solved_table[row][col]);
        deleteSolvedGroup(m_solved_table[row][col]);
    }
    g = new CellGroup(m_last_solved_group_id + 1, row, col, row, col);
    m_cell_groups.insert(m_last_solved_group_id + 1, g);
    setPuzzleState(select_mode);
    emit cellGroupsChanged();
}

void PuzzleHandler::exitSelectMode()
{
    CellGroup* g = m_cell_groups.last();
    int row = m_selected_cell_id / m_col_size;
    int col = m_selected_cell_id % m_col_size;
    if (m_solved_table[row][col])
    {
        delete m_cell_groups.take(m_solved_table[row][col]);
        deleteSolvedGroup(m_solved_table[row][col]);
        emit cellGroupsChanged();
    }
    if (validateGroup(g->startRow(), g->startCol(), g->endRow(), g->endCol()))
    {
        m_last_solved_group_id++;
        assignSolvedGroup(g->startRow(), g->startCol(), g->endRow(), g->endCol());
        if (isPuzzleSolved())
        {
            setPuzzleState(finished_mode);
            return;
        }
    }
    else
    {
        g = m_cell_groups.take(m_cell_groups.lastKey());
        delete g;
        emit cellGroupsChanged();
    }
    m_selection_start_id = m_selected_cell_id;
    setPuzzleState(normal_mode);
}

bool PuzzleHandler::validateGroup(int start_row, int start_col, int end_row, int end_col)
{
    if (start_row > end_row || start_col > end_col || (start_row == end_row && start_col == end_col))
    {
        return false;
    }
    int count = 0, sum = 0;
    for (int row = start_row; row <= end_row; row++)
    {
        for (int col = start_col; col <= end_col; col++)
        {
            count++;
            int value = m_table[row][col];
            if ((value && sum) || m_solved_table[row][col])
            {
                return false;
            }
            sum += value;
        }
    }
    return (count == sum);
}

void PuzzleHandler::assignSolvedGroup(int start_row, int start_col, int end_row, int end_col)
{
    if (start_row > end_row || start_col > end_col)
    {
        return;
    }
    for (int row = start_row; row <= end_row; row++)
    {
        for (int col = start_col; col <= end_col; col++)
        {
            m_solved_table[row][col] = m_last_solved_group_id;
        }
    }
}

void PuzzleHandler::deleteSolvedGroup(int group_id)
{
    for (int row = 0; row < m_row_size; row++)
    {
        for (int col = 0; col < m_col_size; col++)
        {
            if (m_solved_table[row][col] == group_id)
            {
                m_solved_table[row][col] = 0;
            }
        }
    }

}

bool PuzzleHandler::isPuzzleSolved()
{
    for (auto i: m_solved_table)
    {
        for (auto j: i)
        {
            if (!j)
            {
                return false;
            }
        }
    }
    return true;
}
