/*
    SPDX-License-Identifier: GPL-2.0-or-later
    SPDX-FileCopyrightText: 2021 Helguli <helguli@yahoo.com>
*/

#pragma once

#include <vector>
#include <QObject>
#include <QKeyEvent>
#include <qqml.h>
#include "cellgroup.h"

enum mode {normal_mode, select_mode, finished_mode};
enum direction {none, left, right, up, down};

/**
 * Class that generates and
 */
class PuzzleHandler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int               rowSize        READ rowSize        WRITE setRowSize        NOTIFY rowSizeChanged)
    Q_PROPERTY(int               colSize        READ colSize        WRITE setColSize        NOTIFY colSizeChanged)
    Q_PROPERTY(int               selectedCellId READ selectedCellId WRITE setSelectedCellId NOTIFY selectedCellIdChanged)
    Q_PROPERTY(mode              puzzleState    READ puzzleState    WRITE setPuzzleState    NOTIFY puzzleStateChanged)
    Q_PROPERTY(QVariantList      puzzleValues   READ puzzleValues                           NOTIFY puzzleValuesChanged)
    Q_PROPERTY(QList<CellGroup*> cellGroups     READ cellGroups                             NOTIFY cellGroupsChanged)
    QML_ELEMENT
public:
    PuzzleHandler();
    virtual ~PuzzleHandler();
    int               rowSize()        {return m_row_size;}
    int               colSize()        {return m_col_size;}
    int               selectedCellId() {return m_selected_cell_id;}
    mode              puzzleState()    {return m_puzzle_state;}
    QVariantList      puzzleValues()   {return m_puzzle_values;}
    QList<CellGroup*> cellGroups()     {return m_cell_groups.values();}
public slots:
    void setRowSize(int row_size);
    void setColSize(int col_size);
    void setSelectedCellId(int selected_cell_id);
    void setPuzzleState(mode puzzle_state);
    void returnPressed();
    void mousePressed(int cell_id);
    void mouseReleased();
    int getValueAt(int index);
    void setSize(int row_size, int col_size);
    void newPuzzle();
signals:
    void rowSizeChanged();
    void colSizeChanged();
    void selectedCellIdChanged();
    void puzzleStateChanged();
    void puzzleValuesChanged();
    void cellGroupsChanged();
    void puzzleFinished();
private:
    int m_row_size = 5;
    int m_col_size = 7;
    int m_selected_cell_id = 0;
    int m_selection_start_id = 0;
    int m_last_solved_group_id = 0;
    mode m_puzzle_state = normal_mode;
    std::vector<std::vector<int>> m_table;
    std::vector<std::vector<int>> m_solved_table;
    QVariantList m_puzzle_values;
    QMap<int, CellGroup*> m_cell_groups;
    int hasEmptyCell(void);
    bool createNewGroup(int group_id, int empty_cells);
    bool assignGroup(int group_id, int start_row, int start_col, int row_size, int col_size);
    void deleteGroup(int group_id);
    int getAvailableSpace(bool horizontal, int start_row, int start_col, int width);
    bool isIsolatedCell(int row, int col);
    bool placeToGroup(int row, int col);
    void degroup(int group_id, int group_count, int nth);
    int countGroup(int group_id);
    int getGroupHeight(int group_id);
    int getGroupHeight(int cell_row, int cell_col);
    int getGroupWidth(int group_id);
    int getGroupWidth(int cell_row, int cell_col);
    std::vector<int> getEmptyCell(int cell_num);;
    int getNeighbouringGroupId(int cell_row, int cell_col);
    void enterSelectMode();
    void exitSelectMode();
    bool validateGroup(int start_row, int start_col, int end_row, int end_col);
    void assignSolvedGroup(int start_row, int start_col, int end_row, int end_col);
    void deleteSolvedGroup(int group_id);
    bool isPuzzleSolved();

};
