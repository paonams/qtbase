/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

//! [0]
#include <QtWidgets>
#ifndef QT_NO_PRINTDIALOG
#include <QtPrintSupport>
#endif

#include "mainwindow.h"
#include <iostream>
//! [0]

//! [1]
MainWindow::MainWindow()
    : textEdit(new QPlainTextEdit)
{
    mGcodeDock = NULL;
    mGcodeList = NULL;
    //setCentralWidget(textEdit);

    createActions();
    createStatusBar();
    createDockWindows();

    setWindowTitle(tr("Dock Widgets"));

    //newLetter();
    setUnifiedTitleAndToolBarOnMac(true);
}
//! [1]

//! [3]
void MainWindow::print()
{
#ifndef QT_NO_PRINTDIALOG
    QTextDocument *document = textEdit->document();
    QPrinter printer;

    QPrintDialog dlg(&printer, this);
    if (dlg.exec() != QDialog::Accepted) {
        return;
    }

    document->print(&printer);
    statusBar()->showMessage(tr("Ready"), 2000);
#endif
}
//! [3]

bool MainWindow::saveAs()
//! [11] //! [12]
{
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted)
        return false;
    return saveFile(dialog.selectedFiles().first());
}

bool MainWindow::saveFile(const QString &fileName)
//! [44] //! [45]
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName),
                                  file.errorString()));
        return false;
    }

    QTextStream out(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    out << textEdit->toPlainText();
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

bool MainWindow::save()
//! [9] //! [10]
{
    if (curFile.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}
//! [4]

//! [5]
void MainWindow::undo()
{
    QTextDocument *document = textEdit->document();
    document->undo();
}
//! [5]


void MainWindow::about()
{
   QMessageBox::about(this, tr("About Dock Widgets"),
            tr("The <b>Dock Widgets</b> example demonstrates how to "
               "use Qt's dock widgets. You can enter your own text, "
               "click a customer to add a customer name and "
               "address, and click standard paragraphs to add them."));
}
void MainWindow::newFile()
//! [5] //! [6]
{
    if (maybeSave()) {
        textEdit->clear();
        setCurrentFile(QString());
    }
}

bool MainWindow::maybeSave()
//! [40] //! [41]
{
    if (!textEdit->document()->isModified())
        return true;
    const QMessageBox::StandardButton ret
        = QMessageBox::warning(this, tr("Application"),
                               tr("The document has been modified.\n"
                                  "Do you want to save your changes?"),
                               QMessageBox::Save | QMessageBox::Discard | 
			       QMessageBox::Cancel);
    switch (ret) {
    case QMessageBox::Save:
        return save();
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
//! [46] //! [47]
{
    curFile = fileName;
    textEdit->document()->setModified(false);
    setWindowModified(false);

    QString shownName = curFile;
    if (curFile.isEmpty())
        shownName = "untitled.txt";
    setWindowFilePath(shownName);
}

void MainWindow::loadFile(const QString &fileName)
//! [42] //! [43]
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }

    QTextStream in(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    //textEdit->setPlainText(in.readAll());
    mGcodeList->clear();
    //std::cout<<"MainWindow::loadFile file "<<(in.readAll()).toStdString()<<"\n";
#if 1
    //mGcodeList->addItem(in.readAll());
    mGcodeList->setPlainText(in.readAll());
    mGcodeList->setReadOnly(true);
    mGcodeDock->setWidget(mGcodeList);
#endif

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
    std::cout<<"MainWindow::loadFile END\n";
    //addDockWidget(Qt::LeftDockWidgetArea, mGcodeDock);
}
void MainWindow::open()
//! [7] //! [8]
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}

void MainWindow::createActions()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QToolBar *fileToolBar = addToolBar(tr("File"));
    // 1. File
    // 1.1 New Gcode File
    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
    QAction *newGcode = new QAction(newIcon, tr("&New Gcode File"), this);
    newGcode->setShortcuts(QKeySequence::New);
    newGcode->setStatusTip(tr("Create a new Gcode file"));
    connect(newGcode, &QAction::triggered, this, &MainWindow::newFile);
    fileMenu->addAction(newGcode);
    fileToolBar->addAction(newGcode);

    // 1.2 Recent files 
    const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(":/images/save.png"));
    QAction *saveAct = new QAction(saveIcon, tr("&Recent Files ..."), this);
    saveAct->setShortcuts(QKeySequence::Open);
    saveAct->setStatusTip(tr("Open the recent file"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);
    fileMenu->addAction(saveAct);
    fileToolBar->addAction(saveAct);

    /*
    const QIcon printIcon = QIcon::fromTheme("document-print", QIcon(":/images/print.png"));
    QAction *printAct = new QAction(printIcon, tr("&Open Gcode File"), this);
    printAct->setShortcuts(QKeySequence::Print);
    printAct->setStatusTip(tr("open gcode file"));
    connect(printAct, &QAction::triggered, this, &MainWindow::print);
    fileMenu->addAction(printAct);
    fileToolBar->addAction(printAct);
    */
    // 1.3 Open Gcode File

    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
    QAction *openAct = new QAction(openIcon, tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open gcode file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);
    fileMenu->addAction(openAct);
    fileToolBar->addAction(openAct);

    // 1.4 Close Gcode File
    QAction *closeAct = fileMenu->addAction(tr("&Close Gcode File"), this, &QWidget::close);
    closeAct->setShortcuts(QKeySequence::Close);
    closeAct->setStatusTip(tr("Close gcode file"));

    fileMenu->addSeparator();

    // 1.5 Exit
    QAction *exitAct = fileMenu->addAction(tr("&Exit"), this, &QWidget::close);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Quit the application"));

    /*
    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    QToolBar *editToolBar = addToolBar(tr("Edit"));
    const QIcon undoIcon = QIcon::fromTheme("edit-undo", QIcon(":/images/undo.png"));
    QAction *undoAct = new QAction(undoIcon, tr("&Undo"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("Undo the last editing action"));
    connect(undoAct, &QAction::triggered, this, &MainWindow::undo);
    editMenu->addAction(undoAct);
    editToolBar->addAction(undoAct);
    */

    // 2. View
    mViewMenu = menuBar()->addMenu(tr("&View"));

    menuBar()->addSeparator();

    // 3. Help
    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));

    QAction *aboutAct = helpMenu->addAction(tr("&About"), this, &MainWindow::about);
    aboutAct->setStatusTip(tr("Show the application's About box"));

    QAction *aboutQtAct = helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    
    openAct->setEnabled(true);
    connect(textEdit, &QPlainTextEdit::copyAvailable, openAct, &QAction::setEnabled);
}

//! [8]
void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}
//! [8]

//! [9]

CommandLineWidget::CommandLineWidget(QWidget *parent)
	:QWidget(parent)
{
    mGcodeHistoryList = new CodeEditor(this);
    mGcodeHistoryList->clear();
    mGcodeHistoryList->setPlainText(QString());
    mGcodeHistoryList->setReadOnly(true);
    
    mGcodeBox = createComboBox();
    connect(mGcodeBox->lineEdit(), &QLineEdit::returnPressed,
	    this, &CommandLineWidget::animateExecuteClick);

    mExecuteButton = new QPushButton(tr("&Go"), this);
    connect(mExecuteButton, &QAbstractButton::clicked, this, &CommandLineWidget::executeGcode);

    QGridLayout *mainLayout = new QGridLayout(this);

    mainLayout->addWidget(mGcodeHistoryList, 0, 0, 1, 1);
    mainLayout->addWidget(mGcodeBox, 1, 0, 1, 1);
    mainLayout->addWidget(mExecuteButton, 1, 1, 1, 1);
}

QComboBox *CommandLineWidget::createComboBox(const QString &text)
{
    QComboBox *comboBox = new QComboBox;
    comboBox->setEditable(true);
    comboBox->addItem(text);
    comboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    return comboBox;
}

void CommandLineWidget::animateExecuteClick()
{
    mExecuteButton->animateClick();
}

void CommandLineWidget::executeGcode()
{
    std::cout<<__FUNCTION__<<" called\n";
    QString gCode = mGcodeBox->currentText();
    std::cout<<__FUNCTION__<<" gcode "<<gCode.toStdString()<<"\n";
    updateGcodeHistory(gCode);
    mGcodeBox->clearEditText();
}

void CommandLineWidget::updateGcodeHistory(QString& gCode)
{
    mGcodeHistoryList->appendPlainText(gCode);
}
void MainWindow::createDockWindows()
{
    QDockWidget *dock = new QDockWidget(tr("History"), this);
    //dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    //dock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::TopDockWidgetArea);
    dock->setAllowedAreas(Qt::RightDockWidgetArea);
    CommandLineWidget* commandLineWidget = new CommandLineWidget(dock);
#if 0
    customerList = new QListWidget(dock);
    customerList->addItems(QStringList()
            << "John Doe, Harmony Enterprises, 12 Lakeside, Ambleton"
            << "Jane Doe, Memorabilia, 23 Watersedge, Beaton"
            << "Tammy Shea, Tiblanka, 38 Sea Views, Carlton"
            << "Tim Sheen, Caraba Gifts, 48 Ocean Way, Deal"
            << "Sol Harvey, Chicos Coffee, 53 New Springs, Eccleston"
            << "Sally Hobart, Tiroli Tea, 67 Long River, Fedula");
    dock->setWidget(customerList);
    //dock->setFloating(false);
#endif 

    dock->setWidget(commandLineWidget);
    dock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    mViewMenu->addAction(dock->toggleViewAction());

//    setCentralWidget(dock);
#if 0
    mGcodeDock = new QDockWidget(tr("G code"), this);
    mGcodeDock->setAllowedAreas(Qt::LeftDockWidgetArea);
    //mGcodeList = new QListWidget(mGcodeDock);
    //mGcodeList = new QPlainTextEdit(mGcodeDock);
    mGcodeList = new CodeEditor(mGcodeDock);
    mGcodeList->addItems(QStringList()
            << "Thank you for your payment which we have received today."
            << "Your order has been dispatched and should be with you "
               "within 28 days."
            << "We have dispatched those items that were in stock. The "
               "rest of your order will be dispatched once all the "
               "remaining items have arrived at our warehouse. No "
               "additional shipping charges will be made."
            << "You made a small overpayment (less than $5) which we "
               "will keep on account for you, or return at your request."
            << "You made a small underpayment (less than $1), but we have "
               "sent your order anyway. We'll add this underpayment to "
               "your next bill."
            << "Unfortunately you did not send enough money. Please remit "
               "an additional $. Your order will be dispatched as soon as "
               "the complete amount has been received."
            << "You made an overpayment (more than $5). Do you wish to "
               "buy more items, or should we return the excess to you?");
    mGcodeDock->setWidget(mGcodeList);
    addDockWidget(Qt::LeftDockWidgetArea, mGcodeDock);
    mViewMenu->addAction(mGcodeDock->toggleViewAction());
#endif


    dock = new QDockWidget(tr("Program"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea);
    paragraphsList = new QListWidget(dock);
    paragraphsList->addItems(QStringList()
            << "Thank you for your payment which we have received today."
            << "Your order has been dispatched and should be with you "
               "within 28 days."
            << "We have dispatched those items that were in stock. The "
               "rest of your order will be dispatched once all the "
               "remaining items have arrived at our warehouse. No "
               "additional shipping charges will be made."
            << "You made a small overpayment (less than $5) which we "
               "will keep on account for you, or return at your request."
            << "You made a small underpayment (less than $1), but we have "
               "sent your order anyway. We'll add this underpayment to "
               "your next bill."
            << "Unfortunately you did not send enough money. Please remit "
               "an additional $. Your order will be dispatched as soon as "
               "the complete amount has been received."
            << "You made an overpayment (more than $5). Do you wish to "
               "buy more items, or should we return the excess to you?");
    dock->setWidget(paragraphsList);
    //dock->setFloating(false);
    dock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::LeftDockWidgetArea, dock, Qt::Vertical);
    mViewMenu->addAction(dock->toggleViewAction());
    //setCentralWidget(dock);

//    connect(customerList, &QListWidget::currentTextChanged,
//            this, &MainWindow::insertCustomer);
//    connect(gcodeList, &QListWidget::currentTextChanged,
//            this, &MainWindow::addParagraph);
//    connect(paragraphsList, &QListWidget::currentTextChanged,
//            this, &MainWindow::addParagraph);
}
//! [9]
