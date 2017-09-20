#pragma once

#include <QPushButton>

enum State
{
	Run_idle = 0,
	Run,
	Stop
};

class StartIdleStopButton : public QPushButton
{
	Q_OBJECT

public:
	explicit StartIdleStopButton(QWidget *parent = Q_NULLPTR);
	~StartIdleStopButton();

	void  setState(State state);
	State getState() { return m_state; };

protected:
	void nextCheckState();

private:
	State m_state;
};
