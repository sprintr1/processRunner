/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   processwidget.cpp
 * Author: ks
 * 
 * Created on 4. september 2017, 08:12
 */
#include <QDebug>
#include <iostream>

#include <QGridLayout>
#include <QString>
#include <QFileDialog>
#include <QSettings>

#include "processwidget.h"

ProcessWidget::ProcessWidget(int id, QWidget *parent)
    : QWidget(parent)
	, m_id(id)
    , m_processLabel(0)
    , m_processLineEdit(0)
	, m_argumentsLabel(0)
	, m_argLineEdit(0)
	, m_startIdleStopButton(0)
    , m_fileButton(0)
    , m_showLogButton(0)
    , m_processLog(0)
    , m_process(0)
	, m_idle(false)
{
    InitGui();
	LoadSettings();
    ConnectStuff();

	connect(parent, SIGNAL(Idle(bool)),
		this, SLOT(Idle(bool)));
    
    startTimer(100);   // 1-second timer
}

ProcessWidget::~ProcessWidget()
{
}

void
ProcessWidget::timerEvent(QTimerEvent *)
{
	QColor color(Qt::red);
    if(m_process)
    {
        switch(m_process->state())
        {
            case QProcess::NotRunning: { color = QColor(Qt::red);    } break;
            case QProcess::Starting:   { color = QColor(Qt::yellow); } break;
            case QProcess::Running:    { color = QColor(Qt::green);  } break;
        }
    }

	QColor light = color.lighter();
	QString ss = QString(
		"background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %1, stop: 1 %2);"
	).arg(color.name()).arg(light.name());
	m_startIdleStopButton->setStyleSheet(ss);

	UpdateProcess();
}

void
ProcessWidget::InitGui()
{
	QGridLayout* lay = new QGridLayout();
    
    m_processLabel = new QLabel("Process:", this);

    m_processLineEdit = new QLineEdit(this);
	m_processLineEdit->setMinimumWidth(400);

	m_argumentsLabel = new QLabel("Arguments:", this);

	m_argLineEdit = new QLineEdit(this);
	m_argLineEdit->setMinimumWidth(300);

	m_startIdleStopButton = new StartIdleStopButton(this);
    
    m_fileButton = new QPushButton("...", this);
    
    m_showLogButton = new QPushButton("Show Log", this);
    
    lay->addWidget(m_processLabel, 0, 0);
    lay->addWidget(m_processLineEdit, 1, 0);
	lay->addWidget(m_argumentsLabel, 0, 1);
	lay->addWidget(m_argLineEdit, 1, 1);
    lay->addWidget(m_fileButton, 1, 2);
    lay->addWidget(m_startIdleStopButton, 1, 3);
    lay->addWidget(m_showLogButton, 1, 4);
    
    this->setLayout(lay);

	m_processLog = new ProcessLog(this);
	m_processLog->hide();
}

void
ProcessWidget::ConnectStuff()
{
    connect(m_fileButton, &QPushButton::clicked,
            this, &ProcessWidget::SelectFile);
    
    connect(m_startIdleStopButton, &QPushButton::clicked,
            this, &ProcessWidget::UpdateProcess);
    
    connect(m_showLogButton, &QPushButton::clicked,
            this, &ProcessWidget::ShowLog);

	connect(m_processLineEdit, &QLineEdit::textChanged,
		this, &ProcessWidget::SaveSettings);

	connect(m_argLineEdit, &QLineEdit::textChanged,
		this, &ProcessWidget::SaveSettings);
}

void
ProcessWidget::LoadSettings()
{
	QSettings settings;
	settings.beginGroup("Processes");
	m_processLineEdit->setText(settings.value(QString("process%1").arg(m_id)).toString());
	m_argLineEdit->setText(settings.value(QString("arguments%1").arg(m_id)).toString());
	settings.endGroup();
}

void
ProcessWidget::SaveSettings()
{
	QSettings settings;
	settings.beginGroup("Processes");
	settings.setValue(QString("process%1").arg(m_id), m_processLineEdit->text());
	settings.setValue(QString("arguments%1").arg(m_id), m_argLineEdit->text());
	settings.endGroup();
}

void
ProcessWidget::SelectFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Select Executable"), "", tr("Executable Files (*.exe)"));
    m_processLineEdit->setText(fileName);
}

void
ProcessWidget::UpdateProcess()
{
	bool run = false;
	if(m_startIdleStopButton->getState() == Run_idle)
	{
		run = m_idle;
	}
	if (m_startIdleStopButton->getState() == Run)
	{
		run = true;
	}

    if(!m_process && run)
    {
        //Start process:
        StartProcess();
    }
    
	if(m_process && !run)
    {
		QStringList strLines;
		strLines << QString("Ending process %1\n").arg(QString("0x%1").arg((quintptr)m_process->pid(), QT_POINTER_SIZE * 2, 16, QChar('0')));
		m_processLog->NewMessages(strLines, true);

        m_process->close();
		m_process->deleteLater();
		m_process = 0;
    }
}

void
ProcessWidget::ShowLog()
{
    m_processLog->show();
}

void
ProcessWidget::StartProcess()
{
    QString filePath = m_processLineEdit->text();
	QFileInfo fileInfo(filePath);
	QString workingDir = fileInfo.dir().absolutePath();

    QStringList arguments = m_argLineEdit->text().split(QRegExp("\\s+"), QString::SkipEmptyParts);

    if(m_process)
    {
        m_process->deleteLater();
    }

    m_process = new QProcess(this);
	m_process->setWorkingDirectory(workingDir);
    m_process->setReadChannel(QProcess::StandardOutput);
    
    connect (m_process, SIGNAL(readyReadStandardOutput()),
        this, SLOT(PrintOutput()));
    connect (m_process, SIGNAL(readyReadStandardError()),
        this, SLOT(PrintError()));
    
    m_process->start(filePath, arguments);
	QStringList strLines;
	strLines << QString("Starting process %1\n").arg(QString("0x%1").arg((quintptr)m_process->pid(), QT_POINTER_SIZE * 2, 16, QChar('0')));	
	m_processLog->NewMessages(strLines, true);
}

void 
ProcessWidget::PrintOutput()
{
    QByteArray byteArray = m_process->readAllStandardOutput();
    QStringList strLines = QString(byteArray).split("\n");
    
	m_processLog->NewMessages(strLines, false);
}

void ProcessWidget::PrintError()
{
    QByteArray byteArray = m_process->readAllStandardError();
    QStringList strLines = QString(byteArray).split("\n");
    
	m_processLog->NewMessages(strLines, true);
}

void ProcessWidget::Idle(bool idle)
{
	m_idle = idle;
}
