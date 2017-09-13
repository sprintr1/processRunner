/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   mainwindow.cpp
 * Author: ks
 * 
 * Created on 3. september 2017, 10:14
 */

#include <QDebug>

#include <windows.h>
#include <iostream>

#include <QVBoxLayout>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_label(0)
{
    InitGui();
    
    startTimer(1000);   // 1-second timer
}

MainWindow::~MainWindow()
{
}

void
MainWindow::timerEvent(QTimerEvent *)
{
	bool idle = GetIdleMS() > 10000;
	if(!m_idle && idle)
    {
        m_label->setText("idle");
		emit Idle(true);
    }
    
	if(m_idle && !idle)
    {
        m_label->setText("busy");
		emit Idle(false);
    }

	m_idle = idle;
}

void
MainWindow::InitGui()
{
    QVBoxLayout* lay = new QVBoxLayout();
    
    m_label = new QLabel("test", this);

	for (int i = 0; i < 3; i++)
	{
		m_processWidgets.append(new ProcessWidget(i, this));
	}
    
    lay->addWidget(m_label);
	for (auto processWidget : m_processWidgets)
	{
		lay->addWidget(processWidget);
	}
    lay->addStretch();
    
    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(lay);
    
    this->setCentralWidget(centralWidget);
}

unsigned int
MainWindow::GetIdleMS()
{
   LASTINPUTINFO li = { 0, 0 };
   li.cbSize        = sizeof(LASTINPUTINFO);
   GetLastInputInfo(&li);
   return GetTickCount() - li.dwTime;
}
