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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"TextRank.hpp"
#include<fstream>
#include<QFileDialog>
#include<QMessageBox>
#include<QRegExpValidator>
#include<iostream>
#include<streambuf>

#define PROGRAM_NAME "IsTextRankKo"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);

    mainLayout = new QGridLayout;
    mainLayout->addWidget(ui->groupBox, 0, 1);
    mainLayout->addWidget(ui->groupBox_2, 1, 0);
    mainLayout->addWidget(ui->groupBox_3, 2, 0);
    setLayout(mainLayout);

    ui->leAmountOfSent->setAlignment(Qt::Alignment(Qt::AlignRight));
    ui->leAmountOfSent->setText("30");
    QRegExp rx("100|[0-9]{1,2}");
    ui->leAmountOfSent->setValidator(new QRegExpValidator(rx, this));
    ui->amountOfSent->setValue(30);

    ui->amountOfSent->setRange(0, 100);

    ui->startCal->setEnabled(false);


    ui->leDampingFactor->setAlignment(Qt::Alignment(Qt::AlignRight));
    ui->leDampingFactor->setText("0.85");
    rx = QRegExp("1\\.00|0\\.([0-9]{1,2})");
    ui->leDampingFactor->setValidator(new QRegExpValidator(rx, this));
    ui->sDampingFactor->setRange(0, 100);
    ui->sDampingFactor->setValue(85);


    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(slotInputTextChanged()));

    connect(ui->startCal, SIGNAL(clicked()), this, SLOT(displayResult()));
    connect(ui->actionBeenden, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionOeffnen, SIGNAL(triggered()), this, SLOT(openFromFile()));
    connect(ui->actionErgebnis_exportieren_unter, SIGNAL(triggered(bool)), this, SLOT(saveFile()));
    connect(ui->action_ber_IsTextRankKo, SIGNAL(triggered(bool)), this, SLOT(displayAbout()));

    connect(ui->amountOfSent, SIGNAL(valueChanged(int)), this, SLOT(slotSValueChanged()));
    connect(ui->leAmountOfSent, SIGNAL(returnPressed()), this, SLOT(slotSTextChanged()));

    connect(ui->sDampingFactor, SIGNAL(valueChanged(int)), this, SLOT(slotDValueChanged()));
    connect(ui->leDampingFactor, SIGNAL(returnPressed()), this, SLOT(slotDTextChanged()));
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::displayResult(){
    calculate();
    ui->textOutput->setPlainText(QString::fromStdString(result));
    std::cout << result << std::endl;
}

void MainWindow::calculate(){
    result = TextRank::getKeyphrases(ui->textEdit->toPlainText().toStdString(), ui->sDampingFactor->value() * 0.01, ui->amountOfSent->value() * 0.01);
}

void MainWindow::openFromFile(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Öffne Datei"), "", tr("Textdokument (*.txt)"));
    std::fstream file(fileName.toStdString(), std::ios_base::in);

    inputText.clear();
    if(!file){
        return;
    }
    else{
        char c;
        while(file.get(c)){
            inputText.push_back(c);
        }
    }
    ui->textEdit->setPlainText(QString(inputText.c_str()));
}

void MainWindow::saveFile(){
    QString fileName = QFileDialog::getSaveFileName(this,
                 tr("Speichere Datei"), "",
                 tr("Textdokument (*.txt);;Alle Dateien (*)"));
    if (fileName.isEmpty())
        return;
    else {
        std::fstream file(fileName.toStdString(), std::ios_base::out);

        if(!file)
            return;
        else{
            file.write((ui->textOutput->toPlainText().toStdString() + ".txt").c_str(), ui->textOutput->toPlainText().size());
        }
        file.close();
    }
}

void MainWindow::displayAbout(){
    std::fstream readme("readme.txt", std::ios_base::in);
    if(!readme){
        std::cerr << "Fehler beim Öffen der Datei: readme.txt" << std::endl;
        return;
    }
    std::string strReadme((std::istreambuf_iterator<char>(readme)), std::istreambuf_iterator<char>());

    QMessageBox::about(this, QString("Über " PROGRAM_NAME), QString(strReadme.c_str()));
}

void MainWindow::slotSValueChanged(){
    ui->leAmountOfSent->setText(QString::number(ui->amountOfSent->value()));
}

void MainWindow::slotDValueChanged(){
    float damping = ui->sDampingFactor->value()*0.01;
    damping *= 100;                                             //Zahl auf 2 Nachkommastellen runden
    int idam = damping;
    damping = idam / 100.0;
    ui->leDampingFactor->setText(QString::number(damping));
}

void MainWindow::slotSTextChanged(){
    ui->amountOfSent->setValue(ui->leAmountOfSent->text().toInt());
}

void MainWindow::slotDTextChanged(){
    ui->sDampingFactor->setValue(ui->leDampingFactor->text().toFloat() * 100);
}

void MainWindow::slotInputTextChanged(){
    if(ui->textEdit->toPlainText() == ""){
        ui->startCal->setEnabled(false);
    }else{
        ui->startCal->setEnabled(true);
    }
}
