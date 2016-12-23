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

#include"textrank.hpp"
#include"graph.hpp"
#include<boost/algorithm/string/split.hpp>
#include<boost/algorithm/string/classification.hpp>
#include<regex>
#include<algorithm>
#include<iterator>
#include<iostream>
#include<cmath>

// Definieren einer Annäherungsschwelle, die bestimmt, ab wann die Werte annähernd gleich sind.
#define CONVERGENCE_TRESHOLD 0.0001

std::vector<std::string> TextRank::getSameWords(const std::string& sen1, const std::string& sen2){
    std::vector<std::string> words1;
    std::vector<std::string> words2;

    boost::split(words1, sen1, boost::is_any_of(".-:;, "), boost::token_compress_on);
    boost::split(words2, sen2, boost::is_any_of(".-:;, "), boost::token_compress_on);

    for(size_t i = 0; i < words1.size(); ++i){
        std::transform(words1[i].begin(), words1[i].end(), words1[i].begin(), ::tolower);
        std::regex_replace(words1[i], std::regex("[' ']{2,}"), " ");
        //boost::regex_replace(words1[i], boost::regex("[' ']{2,}"), " ");
    }

    for(size_t i = 0; i < words2.size(); ++i){
        std::transform(words2[i].begin(), words2[i].end(), words2[i].begin(), ::tolower);
        std::regex_replace(words2[i], std::regex("[' ']{2,}"), " ");
        //boost::regex_replace(words2[i], boost::regex("[' ']{2,}"), " ");
    }

    std::vector<std::string> sameWords(words1.size() + words2.size());

    std::sort(words1.begin(), words1.end());
    std::sort(words2.begin(), words2.end());

    std::vector<std::string>::iterator i = std::set_intersection(words1.begin(), words1.end(), words2.begin(), words2.end(), sameWords.begin());
    sameWords.erase(i, sameWords.end());

    return sameWords;
}

std::string TextRank::getKeyphrases(const std::string& text, float dampingFactor, float ratio){
    std::vector<std::string> vSentences;

    // Festlegen der Satzgrenzen
    std::regex re("(?: [\\.\\!\\?]\\s+" // Fall 1: Zeichensetzung gefolgt von Leerzeichen
        "|   \\.\\\",?\\s+"   // Fall 2: Beginn des Zitats
        "|   \\s+\\\")");    // Fall 3: Ende des Zitats

    // Iteriere durch Sätze
    //boost::sregex_token_iterator it(begin(text), end(text), re, -1);
    //boost::sregex_token_iterator endit;

    std::sregex_token_iterator it(text.begin(), text.end(), re, -1);
    std::sregex_token_iterator endit;

    std::copy(it, endit, std::back_inserter(vSentences));

    for(size_t i = 0; i < vSentences.size(); ++i){
        while(std::find(vSentences[i].begin(), vSentences[i].end(), '\n') != vSentences[i].end()){
            vSentences[i].replace(std::find(vSentences[i].begin(), vSentences[i].end(), '\n'), std::find(vSentences[i].begin(), vSentences[i].end(), '\n')+1, " ");
        }
    }

    size_t size = vSentences.size();

    Graph g(size);
    for(size_t i = 0; i < size; ++i){       //vSentences.size() == Größe der Matrix
        for(size_t j = 0; j < size; ++j){
            std::vector<std::string> sameWords = getSameWords(vSentences[i], vSentences[j]);
            double dividend = std::abs(int(sameWords.size()));
            double divisor = std::log(std::abs(int(vSentences[i].size()))) + std::log(std::abs(int(vSentences[i].size()))) + std::log(std::abs(int(vSentences[j].size())));

            g.setEdgeValue(i, j, dividend/divisor);
        }
    }

    g.saveInFile();

    std::vector<std::pair<int, double>> scores;
    // Jedes Paar muss mit einem Startwert initialisiert werden
    for(size_t i = 0; i < size; ++i)
        scores.push_back(std::pair<int, double>(i, 1.0/size));

    for(size_t in = 0; in < 100; ++in){
        size_t convergenceAchieved = 0;
        for(size_t i = 0; i < size; ++i){
            double rank = 1 - dampingFactor;
            for(size_t j = 0; j < size; ++j){
                double neightbarsSum = 0;
                for(size_t k = 0; k < size; ++k){
                    neightbarsSum += g.getAdjacencyMatrix()[j][k];
                }
                rank += dampingFactor * scores[j].second * g.getAdjacencyMatrix()[j][i] / neightbarsSum;
            }

            if(std::abs(scores[i].second - rank) <= CONVERGENCE_TRESHOLD){
                convergenceAchieved++;
            }
            scores[i].second = rank;
        }
        //Wenn alle Werte fest sind, wird die Loop abgebrochen und die Berechnung ist fertig
        if(convergenceAchieved == size){
            break;
        }
    }
    std::sort(scores.begin(), scores.end(), [](auto& left, auto& right){
        return left.second > right.second;
    });

    std::vector<std::string> vSorted;
    for(size_t i = 0; i < size; ++i){
        vSorted.push_back(vSentences[scores[i].first]);
        std::cout << scores[i].first << " " << scores[i].second << std::endl;
    }

    //vSorted auf gewollte Anzahl der Sätze beschränken
    size_t ws = size * ratio + 0.5; //0.5, um die Zahl grob zu runden
    std::string finalText;
    for(size_t i = 0; i < ws; ++i){
        finalText += vSorted[i] + ". ";
    }

    return finalText;
}
