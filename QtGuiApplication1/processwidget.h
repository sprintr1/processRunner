/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   processwidget.h
 * Author: ks
 *
 * Created on 4. september 2017, 08:12
 */

#ifndef PROCESSWIDGET_H
#define PROCESSWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QProcess>

#include "processlog.h"

class ProcessWidget : public QWidget 
{
    Q_OBJECT
public:
    ProcessWidget(QWidget *parent = 0);
    virtual ~ProcessWidget();

protected:
    void timerEvent(QTimerEvent *event);

private slots:
    void SelectFile();
    void UpdateProcess();
    void ShowLog();
    void PrintOutput();
    void PrintError();
	void Idle(bool);

private:
    void InitGui();
    void ConnectStuff();
    void StartProcess();
    
    QLabel*         m_label;
    QLineEdit*      m_lineEdit;
    QPushButton*    m_startStopButton;
    QPushButton*    m_fileButton;
    QPushButton*    m_showLogButton;
    
    ProcessLog*     m_processLog;
    
    QProcess*       m_process;

	bool m_idle;

};

#endif /* PROCESSWIDGET_H */

