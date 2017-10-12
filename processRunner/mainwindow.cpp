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
#include <QTimer>
#include <QTime>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
	, m_showHideButton(0)
	, m_minuteSpinBox(0)
	, m_minutesIdleLabel(0)
	, m_idleStatusLabel(0)
	, m_idle(false)
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
	unsigned int idleMs = GetIdleMS();
	QTime time;
	time.addMSecs(idleMs);
	m_minutesIdleLabel->setText(time.toString("hh:mm::ss"));
		
	bool idle = idleMs > m_minuteSpinBox->value()*1000*60;
	if(!m_idle && idle)
    {
        m_idleStatusLabel->setText("Idle");
		emit Idle(true);
    }
    
	if(m_idle && !idle)
    {
        m_idleStatusLabel->setText("Busy");
		emit Idle(false);
    }

	m_idle = idle;
}

void
MainWindow::InitGui()
{
	m_showHideButton = new QPushButton("<", this);
	m_showHideButton->setCheckable(true);
	m_showHideButton->setMaximumWidth(22);

	connect(m_showHideButton, &QPushButton::clicked,
		this, &MainWindow::ToggleProcessEdit);

	m_minuteLabel = new QLabel("Idle time: ", this);

	m_minuteSpinBox = new QSpinBox(this);
	m_minuteSpinBox->setMinimum(1);
	m_minuteSpinBox->setMaximum(120);
	m_minuteSpinBox->setValue(10);
	m_minuteSpinBox->setFixedWidth(50);

	m_minutesIdleLabel = new QLabel(this);

    m_idleStatusLabel = new QLabel("Busy", this);
	m_idleStatusLabel->setStyleSheet("font: bold 16px;");

	for (int i = 0; i < 3; i++)
	{
		m_processWidgets.append(new ProcessWidget(i, this));
	}

	QHBoxLayout* topLay = new QHBoxLayout;

	topLay->addWidget(m_showHideButton);
	topLay->addWidget(m_minutesIdleLabel);
	topLay->addWidget(m_minuteLabel);
	topLay->addWidget(m_minuteSpinBox);
	topLay->addStretch();
	topLay->addWidget(m_idleStatusLabel);
    
	QVBoxLayout* lay = new QVBoxLayout();
    lay->addLayout(topLay);
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

void
MainWindow::ToggleProcessEdit()
{
	if (m_showHideButton->isChecked())
	{
		m_showHideButton->setText(">");
	}
	else
	{
		m_showHideButton->setText("<");
	}

	emit ShowProcessEdit(m_showHideButton->isChecked());

	QTimer::singleShot(10, this, SLOT(ResizeMe()));
}

void
MainWindow::ResizeMe()
{
	resize(minimumSizeHint());
}
