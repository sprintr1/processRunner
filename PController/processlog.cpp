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
   :QDialog(parent),
   fPlainTextEditLabel(0),
   fPlainTextEdit(0),
   fRegExpLabel(0),
   fRegExpCombo(0),
   fRegExpEdit(0),
   fClearButton(0),
   fPauseButton(0)
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
   QSplitter *splitter = new QSplitter(this);
   splitter->setOrientation(Qt::Vertical);
   QWidget* top = InitTop();
   QWidget* bottom = InitBottom();
   splitter->addWidget(top);
   splitter->addWidget(bottom);
   QList<int> sizes;
   sizes.append(top->sizeHint().width());
   sizes.append(100000);
   splitter->setSizes(sizes);
   QVBoxLayout *lay = new QVBoxLayout;
   lay->setContentsMargins(0, 0, 0, 0);
   lay->addWidget(splitter);
   this->setLayout(lay);
}


QWidget* 
ProcessLog::InitTop()
{
   fRegExpLabel = new QLabel(this);
   fRegExpCombo = new QComboBox(this);

   int re_index = 0;
   fRegExpCombo->addItem("RegExp", qVariantFromValue(QRegExp::RegExp));
   fRegExpCombo->setItemData(re_index++, "A rich Perl - like pattern matching syntax.This is the default.", Qt::ToolTipRole);
   fRegExpCombo->addItem("RegExp2", qVariantFromValue(QRegExp::RegExp2));
   fRegExpCombo->setItemData(re_index++, "Like RegExp, but with greedy quantifiers. (Introduced in Qt 4.2.)", Qt::ToolTipRole);
   fRegExpCombo->addItem("Wildcard", qVariantFromValue(QRegExp::Wildcard));
   fRegExpCombo->setItemData(re_index++, "This provides a simple pattern matching syntax similar to that used by shells(command interpreters) for \"file globbing\".See QRegExp wildcard matching.", Qt::ToolTipRole);
   fRegExpCombo->addItem("WildcardUnix", qVariantFromValue(QRegExp::WildcardUnix));
   fRegExpCombo->setItemData(re_index++, "This is similar to Wildcard but with the behavior of a Unix shell.The wildcard characters can be escaped with the character \"\\\".", Qt::ToolTipRole);
   fRegExpCombo->addItem("FixedString", qVariantFromValue(QRegExp::FixedString));
   fRegExpCombo->setItemData(re_index++, "The pattern is a fixed string.This is equivalent to using the RegExp pattern on a string in which all metacharacters are escaped using escape().", Qt::ToolTipRole);
   fRegExpCombo->addItem("W3CXmlSchema11", qVariantFromValue(QRegExp::W3CXmlSchema11));
   fRegExpCombo->setItemData(re_index++, "The pattern is a regular expression as defined by the W3C XML Schema 1.1 specification.", Qt::ToolTipRole);

   connect(fRegExpCombo, SIGNAL(currentIndexChanged(int)),
       this, SLOT(RegExpSyntaxUpdated()));

   fRegExpEdit = new QLineEdit(this);
   connect(fRegExpEdit, SIGNAL(editingFinished()),
       this, SLOT(RegExpUpdated()));

   QHBoxLayout *regExpLay = new QHBoxLayout;
   regExpLay->addWidget(fRegExpEdit);
   regExpLay->addWidget(fRegExpCombo);

   fClearButton = new QPushButton(this);
   fClearButton->setFixedSize(65,25);
   connect(fClearButton, SIGNAL(clicked()),
       this, SLOT(ClearLog()));

   fPauseButton = new QPushButton(this);
   fPauseButton->setFixedSize(65, 25);
   fPauseButton->setCheckable(true);
   connect(fPauseButton, SIGNAL(clicked()),
       this, SLOT(Pause()));

   QHBoxLayout *buttonLay = new QHBoxLayout;
   buttonLay->addWidget(fClearButton);
   buttonLay->addWidget(fPauseButton);
   buttonLay->addStretch();

   QVBoxLayout *lay = new QVBoxLayout;
   lay->addStretch();
   lay->addWidget(fRegExpLabel);
   lay->addLayout(regExpLay);
   lay->addLayout(buttonLay);

   QWidget *w = new QWidget(this);
   w->setLayout(lay);
   return w;
}


QWidget* 
ProcessLog::InitBottom()
{
   fPlainTextEditLabel = new QLabel(this);
   QHBoxLayout *llay = new QHBoxLayout;
   llay->addWidget( fPlainTextEditLabel);
   llay->addStretch();
   fPlainTextEdit = new QPlainTextEdit(this);
   fPlainTextEdit->setReadOnly(true);
   fPlainTextEdit->setWordWrapMode(QTextOption::NoWrap);
   fPlainTextEdit->setMaximumBlockCount(2000);
   fPlainTextEdit->setMinimumWidth(700);

   QVBoxLayout *lay = new QVBoxLayout;
   lay->addLayout(llay);
   lay->addWidget( fPlainTextEdit );
   QWidget *w = new QWidget(this);
   w->setLayout(lay);
   return w;
}


void 
ProcessLog::RetranslateUi()
{
   fTEXT_messages = tr("Messages");
   fTEXT_regExp = tr("Reg Exp");
   fTEXT_configure = tr("Configure");
   fTEXT_clear = tr("Clear");
   fTEXT_pause = tr("Pause");
   fTEXT_unpause = tr("Unpause");
      
   QString ss = QString("QPlainTextEdit{ background-color:  #000000; }");
   fPlainTextEdit->setStyleSheet(ss);

   fPlainTextEditLabel->setText(fTEXT_messages);
   fRegExpLabel->setText(fTEXT_regExp);
   fClearButton->setText(fTEXT_clear);
   fPauseButton->setText(fTEXT_pause);
}

void 
ProcessLog::NewMessages(QStringList msgs, bool error)
{
    for(auto msg : msgs)
    {
        if (fPauseButton->isChecked())
        {
            //Pause enabled, skip new messages
            continue;
        }

        //Filter regexp
        if (fRegExp.pattern().size() && !fRegExp.exactMatch(msg)) {
            //Reg exp does not match, skip msg
            continue;
        }

        //Set color:
        QTextCharFormat tf;
        tf = fPlainTextEdit->currentCharFormat();
		QColor color = QColor(Qt::white);
		if(error)
		{
			color = QColor(Qt::magenta);
		}
        tf.setForeground(QBrush(color));
        tf.setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
        fPlainTextEdit->setCurrentCharFormat(tf);

		msg.chop(1); //Remove trailing \n
        fPlainTextEdit->appendPlainText(msg);

        //Scroll to end
        QScrollBar* sb = fPlainTextEdit->verticalScrollBar();
        sb->setValue(sb->maximum());
    }
}


void
ProcessLog::RegExpUpdated()
{
    fRegExp.setPattern(fRegExpEdit->text());
}

void
ProcessLog::RegExpSyntaxUpdated()
{
    fRegExp.setPatternSyntax(fRegExpCombo->currentData().value<QRegExp::PatternSyntax>());
}

void
ProcessLog::ClearLog()
{
    fPlainTextEdit->clear();
}

void
ProcessLog::Pause()
{
    if (fPauseButton->isChecked())
    {
        fPauseButton->setText(fTEXT_unpause);
    }
    else
    {
        fPauseButton->setText(fTEXT_pause);
    }   
}
