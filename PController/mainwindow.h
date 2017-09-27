/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   mainwindow.h
 * Author: ks
 *
 * Created on 3. september 2017, 10:14
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QVector>

#include "processwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();
signals:
	void Idle(bool);
	void ShowProcessEdit(bool);
protected:
    void timerEvent(QTimerEvent *event);
private slots:
	void ToggleProcessEdit();
	void ResizeMe();
private:
    void InitGui();
    void ConnectStuff();
    unsigned int GetIdleMS();
    
	QPushButton*   m_showHideButton;
	QLabel*        m_minuteLabel;
	QSpinBox*      m_minuteSpinBox;
	QLabel*        m_minutesIdleLabel;
    QLabel*        m_idleStatusLabel;

	QVector<ProcessWidget*> m_processWidgets;

	bool m_idle;
};

#endif /* MAINWINDOW_H */

