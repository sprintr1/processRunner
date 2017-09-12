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
#include <QLabel>

#include "processwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();
signals:
	void Idle(bool);
protected:
    void timerEvent(QTimerEvent *event);
private:
    void InitGui();
    void ConnectStuff();
    unsigned int GetIdleMS();
    
    QLabel*        m_label;
    ProcessWidget* m_processWidget;

	bool m_idle;
};

#endif /* MAINWINDOW_H */

