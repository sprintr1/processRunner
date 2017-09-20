/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   processlog.h
 * Author: ks
 *
 * Created on 7. september 2017, 08:12
 */

#ifndef _PROCESSLOG_H_
#define _PROCESSLOG_H_


#include <QWidget>
#include <QDialog>
#include <QComboBox>
#include <QPlainTextEdit>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

class ProcessLog : public QDialog
{
    Q_OBJECT

public:
    explicit ProcessLog(QWidget *parent = 0);
    ~ProcessLog();

signals:
    void SubStringFound(QString);
  
public slots:
	void NewMessages(QStringList msgs, bool error);

private slots :
    void RegExpUpdated();
    void ClearLog();
    void Pause();

private: // functions
    void InitGui();
    void RetranslateUi();

private: // member objects
    QPlainTextEdit *m_plainTextEdit;

    QLabel* m_regExpLabel;
    QLineEdit* m_regExpEdit;
    QRegularExpression m_regularExpression;

    QPushButton* m_clearButton;
    QPushButton* m_pauseButton;

    QString m_TEXT_pause;
    QString m_TEXT_unpause;
};

#endif