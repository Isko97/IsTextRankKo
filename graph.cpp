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

#include"graph.hpp"
#include<fstream>

Graph::Graph(size_t c)
    : m_VertexCount{c}{
    m_AdjacencyMatrix.assign(m_VertexCount, std::vector<double>(m_VertexCount));
}

void Graph::setEdgeValue(size_t i, size_t j, double value) {
    m_AdjacencyMatrix[i][j] = value;
}

double Graph::getEdgeValue(size_t i, size_t j){
    return m_AdjacencyMatrix[i][j];
}

const std::vector<std::vector<double>>& Graph::getAdjacencyMatrix() const{
    return m_AdjacencyMatrix;
}

void Graph::saveInFile() const{
    std::ofstream save("matrix.txt", std::ios_base::trunc);

    for(size_t i = 0; i < m_AdjacencyMatrix.size(); ++i){
        for(size_t j = 0; j < m_AdjacencyMatrix[i].size(); ++j){
            save.precision(5);
            save << std::fixed << m_AdjacencyMatrix[i][j] << " ";
        }
        save << std::endl;
    }
    save.close();
}

