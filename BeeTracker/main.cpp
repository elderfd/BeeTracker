#include "BeeTrackerApp.h"
#include "window.h"


int main(int argc, char *argv[]) {
	BeeTrackerApp a(argc, argv);
	Window w;
	w.show();

	return a.exec();
}
