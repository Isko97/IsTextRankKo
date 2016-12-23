/* IsTextRankKo is a GUI-Implementation of the TextRank algorithm
** Copyright (C) 2016  Florian Nitschke
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
** Die Klasse stellt einen ungerichtenden Graph mit Kantengewichten und ohne Mehrfachkanten als Adjazenzmatrix dar.
*/

#pragma once

#include<vector>
#include<cstddef> //size_t

class Graph {
public:
    Graph(size_t c);

    void setEdgeValue(size_t i, size_t j, double value);
    double getEdgeValue(size_t i, size_t j);
    const std::vector<std::vector<double>>& getAdjacencyMatrix() const;

    void saveInFile() const;

private:
    std::vector<std::vector<double>> m_AdjacencyMatrix;
    size_t m_VertexCount;
};
