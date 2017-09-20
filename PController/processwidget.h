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
#include "startidlestopbutton.h"

class ProcessWidget : public QWidget 
{
    Q_OBJECT
public:
    ProcessWidget(int id, QWidget *parent = 0);
    virtual ~ProcessWidget();

public slots:
	void SubStringFound(QString);

protected:
    void timerEvent(QTimerEvent *event);

private slots:
	void SaveSettings();
    void SelectFile();
    void UpdateProcess();
    void ShowLog();
    void PrintOutput();
    void PrintError();
	void Idle(bool);

private:
    void InitGui();
    void ConnectStuff();
	void LoadSettings();
    void StartProcess();
    
	int				m_id;
    QLabel*         m_processLabel;
    QLineEdit*      m_processLineEdit;
	QLabel*         m_argumentsLabel;
	QLineEdit*      m_argLineEdit;
	StartIdleStopButton* m_startIdleStopButton;
    QPushButton*    m_fileButton;
    QPushButton*    m_showLogButton;
	QLabel*         m_substringStatusLabel;
    
    ProcessLog*     m_processLog;
    
    QProcess*       m_process;

	bool m_idle;

};

#endif /* PROCESSWIDGET_H */

