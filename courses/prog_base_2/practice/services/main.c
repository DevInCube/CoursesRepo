#include <stdio.h>
#include <stdlib.h>

#include <windows.h>

#define SLEEP_TIME 2000
#define LOGFILE "C:\\__TestService\\log.txt"

SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE hStatus;

void ServiceMain(int argc, char ** argv);
void ControlHandler(DWORD request);
int InitService(void);

int WriteToLog(char * str);

int main(void) {
    SERVICE_TABLE_ENTRY ServiceTable[2];
    ServiceTable[0].lpServiceName = "MemoryStatus";
    ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;

    ServiceTable[1].lpServiceName = NULL;
    ServiceTable[1].lpServiceProc = NULL;
    // Start the control dispatcher thread for our service
    StartServiceCtrlDispatcher(ServiceTable);
    return 0;
}

// Service initialization
int InitService(void) {
    int result;
    result = WriteToLog("Monitoring started.");
    return(result);
}

void ServiceMain(int argc, char ** argv) {
    int error;

    ServiceStatus.dwServiceType        = SERVICE_WIN32;
    ServiceStatus.dwCurrentState       = SERVICE_START_PENDING;
    ServiceStatus.dwControlsAccepted   = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
    ServiceStatus.dwWin32ExitCode      = 0;
    ServiceStatus.dwServiceSpecificExitCode = 0;
    ServiceStatus.dwCheckPoint         = 0;
    ServiceStatus.dwWaitHint           = 0;

    hStatus = RegisterServiceCtrlHandler(
		"MemoryStatus",
		(LPHANDLER_FUNCTION)ControlHandler);

    if (hStatus == (SERVICE_STATUS_HANDLE)0) {
        // Registering Control Handler failed
        return;
    }
    // Initialize Service
    error = InitService();
    if (error) {
		// Initialization failed
        ServiceStatus.dwCurrentState       = SERVICE_STOPPED;
        ServiceStatus.dwWin32ExitCode      = -1;
        SetServiceStatus(hStatus, &ServiceStatus);
        return;
    }
    // We report the running status to SCM.
    ServiceStatus.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus (hStatus, &ServiceStatus);

    MEMORYSTATUS memory;
    // The worker loop of a service
    while (ServiceStatus.dwCurrentState == SERVICE_RUNNING) {
		char buffer[16];
		GlobalMemoryStatus(&memory);
		sprintf(buffer, "%d", memory.dwAvailPhys);
		int result = WriteToLog(buffer);
		if (result) {
			ServiceStatus.dwCurrentState       = SERVICE_STOPPED;
			ServiceStatus.dwWin32ExitCode      = -1;
			SetServiceStatus(hStatus, &ServiceStatus);
			return;
		}
		Sleep(SLEEP_TIME);
	}
    return;
}

// Control handler function
void ControlHandler(DWORD request) {
    switch(request) {
        case SERVICE_CONTROL_STOP:
            WriteToLog("Monitoring stopped.");

            ServiceStatus.dwWin32ExitCode = 0;
            ServiceStatus.dwCurrentState  = SERVICE_STOPPED;
            SetServiceStatus (hStatus, &ServiceStatus);
            return;

        case SERVICE_CONTROL_SHUTDOWN:
            WriteToLog("Monitoring stopped.");

            ServiceStatus.dwWin32ExitCode = 0;
            ServiceStatus.dwCurrentState  = SERVICE_STOPPED;
            SetServiceStatus (hStatus, &ServiceStatus);
            return;

        default:
            break;
    }
    // Report current status
    SetServiceStatus (hStatus,  &ServiceStatus);
    return;
}

int WriteToLog(char * str) {
    FILE * log;
    log = fopen(LOGFILE, "a+");
    if (log == NULL) {
        return -1;
    }
    fprintf(log, "%s\n", str);
    fclose(log);
    return 0;
}
