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

#include <QHBoxLayout>
#include <QString>
#include <QFileDialog>
#include <QSettings>

#include "processwidget.h"

ProcessWidget::ProcessWidget(int id, QWidget *parent)
    : QWidget(parent)
	, m_id(id)
    , m_label(0)
    , m_processLineEdit(0)
	, m_argLineEdit(0)
    , m_startStopButton(0)
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
    if(m_process)
    {
        switch(m_process->state())
        {
            case QProcess::NotRunning: { m_startStopButton->setText("Start"); } break;
            case QProcess::Starting:   { m_startStopButton->setText("Starting..."); } break;
            case QProcess::Running:    { m_startStopButton->setText("Stop"); } break;
        }
    }
    else
    {
        m_startStopButton->setText("Start");
    }

	UpdateProcess();
}

void
ProcessWidget::InitGui()
{
    QHBoxLayout* lay = new QHBoxLayout();
    
    m_label = new QLabel("Process:", this);

    m_processLineEdit = new QLineEdit(this);
	m_processLineEdit->setMinimumWidth(400);

	m_argLineEdit = new QLineEdit(this);
	m_argLineEdit->setMinimumWidth(300);

    m_startStopButton = new QPushButton(this);
	m_startStopButton->setCheckable(true);
    
    m_fileButton = new QPushButton("...", this);
    
    m_showLogButton = new QPushButton("Show Log", this);
    
    lay->addWidget(m_label);
    lay->addWidget(m_processLineEdit);
	lay->addWidget(m_argLineEdit);
    lay->addWidget(m_fileButton);
    lay->addWidget(m_startStopButton);
    lay->addWidget(m_showLogButton);
    lay->addStretch();
    
    this->setLayout(lay);
}

void
ProcessWidget::ConnectStuff()
{
    connect(m_fileButton, &QPushButton::clicked,
            this, &ProcessWidget::SelectFile);
    
    connect(m_startStopButton, &QPushButton::clicked,
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
	bool run = m_idle && m_startStopButton->isChecked();
    if(!m_process && run)
    {
        //Start process:
        StartProcess();
    }
    
	if(m_process && !run)
    {
		//Stop the process if 
		qDebug() << "Ending process " << m_process->pid();
        m_process->close();
		m_process->deleteLater();
		m_process = 0;
    }
}

void
ProcessWidget::ShowLog()
{
    if(!m_processLog)
    {
        m_processLog = new ProcessLog(this);
    }
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
	qDebug() << "Started process " << m_process->pid();
}

void 
ProcessWidget::PrintOutput()
{
    QByteArray byteArray = m_process->readAllStandardOutput();
    QStringList strLines = QString(byteArray).split("\n");
    
	if (m_processLog)
	{
		m_processLog->NewMessages(strLines);
	}
    
    foreach (QString line, strLines){
		qDebug() << line;
    }
}

void ProcessWidget::PrintError()
{
    QByteArray byteArray = m_process->readAllStandardError();
    QStringList strLines = QString(byteArray).split("\n");
    
	if (m_processLog)
	{
		m_processLog->NewMessages(strLines);
	}
 
    foreach (QString line, strLines){
		qDebug() << "Error: " << line;
    }
}

void ProcessWidget::Idle(bool idle)
{
	m_idle = idle;
}