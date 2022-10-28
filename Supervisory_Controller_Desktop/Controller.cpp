#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include "supervisorycontroller_time.h"

int main(void){
	// Time Counter
    clock_t t;

	// Controller Class
    supervisorycontroller fsm;

	// Serial Port Communication Variables
    DWORD dwCommEvent;
    DWORD dwRead;
    DWORD lpEvtMask;
    int bufsize = 20;
    char chRead[bufsize], chWrite[bufsize]; memset(chRead, 0, bufsize);
    char  ComPortName[] = "\\\\.\\COM8";  // Name of the Serial port(May Change) to be opened,

    HANDLE hComm;
    hComm = CreateFile( ComPortName,  
                        GENERIC_READ | GENERIC_WRITE, 
                        0, 
                        0, 
                        OPEN_EXISTING,
                        0,
                        0);
    if (hComm == INVALID_HANDLE_VALUE)
       printf("Error opening port.\n");
    
    DCB dcb;

    FillMemory(&dcb, sizeof(dcb), 0);
    if (!GetCommState(hComm, &dcb))     // get current DCB
       printf("Error GetCommState.\n");

    // Update DCB rate.
    dcb.BaudRate = 115200 ;
    dcb.ByteSize = 8;             // Setting ByteSize = 8
    dcb.StopBits = 1;    // Setting StopBits = 1
    dcb.Parity = 0;        // Setting Parity = None 
    dcb.DCBlength = sizeof(dcb);
     // Set new state.
    if (!SetCommState(hComm, &dcb))
        printf("Error SetCommState.\n");
        // Error in SetCommState. Possibly a problem with the communications 
         // port handle or a problem with the DCB structure itself.

	// Set Connection Timeout Parameters
    COMMTIMEOUTS timeouts;

    timeouts.ReadIntervalTimeout = MAXDWORD; 
    timeouts.ReadTotalTimeoutMultiplier = 0;
    timeouts.ReadTotalTimeoutConstant = 0;
    timeouts.WriteTotalTimeoutMultiplier = 0;
    timeouts.WriteTotalTimeoutConstant = 0;

    if (!SetCommTimeouts(hComm, &timeouts))
       printf("Error timeouts.\n");

    if(!PurgeComm(hComm,PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT))
        printf("Error PurgeComm.\n");


	// Start Communication with Serial Port
    for ( ; ; ) {
        if (!SetCommMask(hComm, 0))
            printf("Error CommMask.\n");
    
        if (!SetCommMask(hComm, EV_RXCHAR))
            printf("Error CommMask.\n");
    
        while (WaitCommEvent(hComm, &dwCommEvent, NULL)) {
           do {
                if (ReadFile(hComm, chRead, bufsize, &dwRead, NULL)){
                    if(dwRead!=0)
                        {
							// Start Time Counting
                            t = clock();

                            // Read Feedback and Feed the Feedbacks into the FSM
							// Data Format: [door pose],[button state],[human sensor state],
                            char* token;
                            token = strtok(chRead, ",");
                            int pose = atoi(token);
                            token = strtok(NULL, ",");
                            int vel = atoi(token);
                            token = strtok(NULL, ",");
                            int but = atoi(token);
                            token = strtok(NULL, ",");
                            int sense = atoi(token);
                            fsm.setFeedback(pose/10000., vel/10000., but, sense);

							// Print Feedback from Serial Port
                            printf(" pose*10000:%d, vel*10000:%d, button state:%d, human sensor state:%d, ", pose, vel, but, sense);
                            printf("state:%d, control effort:%f, target_vel:%f, ", fsm.getState(), fsm.getControlEffort(), fsm.getTarget());
							
							// Clear Communication Buffer to Prevent Buffer Overflow
                            memset(chRead, 0, bufsize);

                            // Compute Command and Pass it to the Serial Port
							// Data Format: [command]
                            float command = fsm.computeControl();
                            itoa((int)(command*10000), chWrite, 10);
                            WriteFile(hComm, chWrite, bufsize, NULL, NULL);

							// Clear Communication Buffer for the Next Data Transmission
                            memset(chWrite,0,bufsize);
                            
							// Wait for the Next Computation Sample
                            while ((double)(clock() - t)/CLOCKS_PER_SEC < 0.007) {}
                            t = clock()-t;
                            double time_taken = ((double)t)/CLOCKS_PER_SEC;
                            printf("Time elapsed = %lf s \n", time_taken);

                        }
                }
                else{
                    printf("ErrorReadFile.\n");
                    break;
                }
            }
            while (dwRead);
       }
    }
}