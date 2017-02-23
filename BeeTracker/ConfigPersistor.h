#pragma once
#include <QString>


class ConfigPersistor {
public:
	ConfigPersistor();
	~ConfigPersistor();

private:
	static const QString fileName;
};

