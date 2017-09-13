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
  
public slots:
	void NewMessages(QStringList msgs);

private slots :
  void RegExpUpdated();
  void RegExpSyntaxUpdated();
  void ClearLog();
  void Pause();

private: // functions
  void InitGui();
  void RetranslateUi();
  QWidget* InitLeft();
  QWidget* InitRight();

private: // member objects
  QLabel *fPlainTextEditLabel;
  QPlainTextEdit *fPlainTextEdit;

  QLabel* fRegExpLabel;
  QComboBox *fRegExpCombo;
  QLineEdit* fRegExpEdit;
  QRegExp fRegExp;

  QPushButton* fClearButton;
  QPushButton* fPauseButton;

  QString fTEXT_messages;
  QString fTEXT_regExp;
  QString fTEXT_configure;
  QString fTEXT_clear;
  QString fTEXT_pause;
  QString fTEXT_unpause;
};

#endif