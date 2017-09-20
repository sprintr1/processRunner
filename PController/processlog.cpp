/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   processlog.cpp
 * Author: ks
 * 
 * Created on 7. september 2017, 08:12
 */

#include <QDebug>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QTimer>
#include <QSettings>
#include <QSplitter>
#include <QButtonGroup>
#include <QDateTime>
#include <QRegExp>
#include <QMetaType>
#include <QScrollBar>

#include "processwidget.h"
#include "processlog.h"

Q_DECLARE_METATYPE(QRegExp::PatternSyntax);

ProcessLog::ProcessLog(QWidget *parent)
   :QDialog(parent)
    , m_plainTextEdit(0)
    , m_regExpLabel(0)
    , m_regExpEdit(0)
    , m_clearButton(0)
    , m_pauseButton(0)
{
    InitGui();
    RetranslateUi();
}


ProcessLog::~ProcessLog()
{

}


void 
ProcessLog::InitGui()
{
    this->setWindowTitle("Messages");

	m_regExpLabel = new QLabel(this);

	m_regExpEdit = new QLineEdit(this);
	connect(m_regExpEdit, SIGNAL(editingFinished()),
		this, SLOT(RegExpUpdated()));

	m_clearButton = new QPushButton(this);
	m_clearButton->setFixedSize(65, 25);
	connect(m_clearButton, SIGNAL(clicked()),
		this, SLOT(ClearLog()));

	m_pauseButton = new QPushButton(this);
	m_pauseButton->setFixedSize(65, 25);
	m_pauseButton->setCheckable(true);
	connect(m_pauseButton, SIGNAL(clicked()),
		this, SLOT(Pause()));

	QHBoxLayout *topLay = new QHBoxLayout;
	topLay->addWidget(m_clearButton);
	topLay->addWidget(m_pauseButton);
	topLay->addWidget(m_regExpLabel);
	topLay->addWidget(m_regExpEdit);

	m_plainTextEdit = new QPlainTextEdit(this);
	m_plainTextEdit->setReadOnly(true);
	m_plainTextEdit->setWordWrapMode(QTextOption::NoWrap);
	m_plainTextEdit->setMaximumBlockCount(2000);
	m_plainTextEdit->setMinimumWidth(700);

	QVBoxLayout *lay = new QVBoxLayout;
	lay->addLayout(topLay);
	lay->addWidget(m_plainTextEdit);
	
	//QWidget* top = InitTop();
    //QWidget* bottom = InitBottom();
    //lay->addWidget(top);
    //lay->addWidget(bottom);
    //lay->setContentsMargins(0, 0, 0, 0);
    this->setLayout(lay);
}

void 
ProcessLog::RetranslateUi()
{
    m_TEXT_pause = tr("Pause");
    m_TEXT_unpause = tr("Unpause");
       
    QString ss = QString("QPlainTextEdit{ background-color:  #000000; }");
    m_plainTextEdit->setStyleSheet(ss);
    
    m_regExpLabel->setText(tr("Reg Exp"));
    m_clearButton->setText(tr("Clear"));
    m_pauseButton->setText(m_TEXT_pause);
}

void 
ProcessLog::NewMessages(QStringList msgs, bool error)
{
    for(auto msg : msgs)
    {
        if (m_pauseButton->isChecked())
        {
            //Pause enabled, skip new messages
            continue;
        }

		//^(.*)(\),)(?<ss>.*)(at diff)(.*)$
		QRegularExpressionMatch match = m_regularExpression.match(msg);
		if (match.hasMatch()) {
			emit SubStringFound(match.captured("ss"));
			qDebug() << msg << " " << match.captured("ss");
		}

        //Set color:
        QTextCharFormat tf;
        tf = m_plainTextEdit->currentCharFormat();
		QColor color = QColor(Qt::white);
		if(error)
		{
			color = QColor(Qt::magenta);
		}
        tf.setForeground(QBrush(color));
        tf.setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
        m_plainTextEdit->setCurrentCharFormat(tf);

		msg.chop(1); //Remove trailing \n
        m_plainTextEdit->appendPlainText(msg);

        //Scroll to end
        QScrollBar* sb = m_plainTextEdit->verticalScrollBar();
        sb->setValue(sb->maximum());
    }
}


void
ProcessLog::RegExpUpdated()
{
	m_regularExpression.setPattern(m_regExpEdit->text());
	emit SubStringFound("");
}

void
ProcessLog::ClearLog()
{
    m_plainTextEdit->clear();
}

void
ProcessLog::Pause()
{
    if (m_pauseButton->isChecked())
    {
        m_pauseButton->setText("Unpause");
    }
    else
    {
        m_pauseButton->setText("Pause");
    }   
}
