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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QGridLayout>
#include<string>
#include"textrank.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void displayResult();
    void openFromFile();
    void saveFile();
    void slotSValueChanged();
    void slotSTextChanged();
    void slotDValueChanged();
    void slotDTextChanged();
    void slotInputTextChanged();
    void displayAbout();

private:
    void calculate();


    Ui::MainWindow *ui;
    QGridLayout* mainLayout;

    std::string result;
    std::string inputText;
};

#endif // MAINWINDOW_H
