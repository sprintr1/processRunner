#include "startidlestopbutton.h"

StartIdleStopButton::StartIdleStopButton(QWidget *parent)
	: QPushButton(parent)
{
	m_state = Run_idle;
	setText("Run Idle");
}

StartIdleStopButton::~StartIdleStopButton()
{
}

void
StartIdleStopButton::setState(State state)
{
	switch (state)
	{
	case Run_idle:
	{
		m_state = Run_idle;
		setText("Run Idle");
		break;
	}
	case Run:
	{
		m_state = Run;
		setText("Run");
		break;
	}
	case Stop:
	{
		m_state = Stop;
		setText("Stop");
		break;
	}
	}
}

void
StartIdleStopButton::nextCheckState()
{
	switch (m_state)
	{
		case Run_idle:	{ setState(Run); break; }
		case Run: { setState(Stop); break; }
		case Stop: { setState(Run_idle); break; }
	}
}