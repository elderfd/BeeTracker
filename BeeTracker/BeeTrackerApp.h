#pragma once
#include "ConfigPersistor.h"
#include <QApplication>
#include "DesignManager.h"
#include "ProgramState.h"
#include "FileOutputManager.h"


class BeeTrackerApp :
	public QApplication {
public:
	BeeTrackerApp(int argc, char* argv[]);
	~BeeTrackerApp();

	ProgramState state;
	DesignManager designManager;
	FileOutputManager fileOutputManager;
	ConfigPersistor configPersistor;
};


// Redefine the qApp global pointer macro with the right cast
#undef qApp
#define qApp (static_cast<BeeTrackerApp *>(QCoreApplication::instance()))


