#include <Windows.h>
//#include <stdio.h>


enum SignalType {
	Interrupt = CTRL_C_EVENT,
	Break = CTRL_BREAK_EVENT,
};

static void sendSignal(DWORD pid, DWORD signal = Interrupt) {
	FreeConsole();
	if (AttachConsole(pid)) { // this does not require the console window to be visible.
		SetConsoleCtrlHandler(NULL, true); // disable Ctrl-C handling for ourselves.

		GenerateConsoleCtrlEvent(signal, 0);
		//FreeConsole(); // avoid terminating ourselves if `proc` is killed by others.
		//Sleep(1000); // avoid terminating ourselves.

		//SetConsoleCtrlHandler(NULL, false); // re-enable Ctrl-C handling or any subsequently started programs will inherit the disabled state.
	//} else {
	//	printf("failed to attach");
	}
}


int main(int argc, char* argv[]) {
	if (argc > 1) {
		DWORD signal = (argc < 3) ? Interrupt : atoi(argv[2]);
		sendSignal(atoi(argv[1]), signal);
	//} else {
	//	printf("SendSignal PID [SIGNAL=0|1]");
	}
	return 0;
}
