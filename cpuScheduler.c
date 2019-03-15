// Steven Xiong
// CSC 139

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
struct PCB
{
	int pid;
	int arrival_time;
	int burst_time;
};


void fcfs(struct PCB fcfsArray[], int size);
void rr(struct PCB fcfsArray[], int size, int quantumTime);
void srtf(struct PCB fcfsArray[], int size);
int resetFlag(int flag, int arraySize);

int main(int argc, char *argv[])
{
	char var0[7];	// will hold the input file name
	char var1[4];	// will hold the mode at which the program will execute, options are 'FCFS' 'RR' SRTF'
	char var2[2];	// will hold the quantuam time if round robin is selected
	
	int mode = 0; 	// variable to hold which mode the program will execute in
			//[mode == 1 FCFS], [mode == 2 RR], [mode == 3 SRTF]

	// CHAR ARRAYS TO BE USED TO MATCH WHICH MODE THE PROGRAM WILL BE RUNNING IN
	char fcfsMode[4] = "FCFS";	// FIRST COME FIRST SERVE (FCFS)
	char rrMode[2] = "RR";		// ROUND ROBIN (RR)
	char srtfMode[4] = "SRTF";	// SHORTEST REMAINING TIME FIRST (SRTF)
	
	char fcfsModeLow[4] = "fcfs";
	char rrModeLow[2] = "rr";
	char srtfModeLow[4] = "srtf";		
	
	// MOSTLY USED AS COUNTER'S
	int x = 0;
	int i = 0;
	int y = 0;
	
	// COUNTER THAT WILL HOLD THE VALUE OF THE QUANTUAM TIME THAT IS REQUIRED FOR ROUND ROBIN(RR)
	int quantumTimeRR = 0;

	// VARIABLES FOR OPENING FILES AND READING IN THE FILES
	FILE *fp;
	char c;
	
	char input[7] = "input.1";
	int avgLen = 0;

	int matchChar = 0; // variable to check how many letters has been matched so far
	
	// VARIABLES TO READ IN CHAR NUMBERS AND USED TO CONVERT THEM TO INT
	char doubleDigit[2]; // USED TO CHECK FOR DOUBLE DIGIT INT'S
	char holderChar;
	int convertedInt;			// USED TO HOLD INTEGER THAT HAS BEEN CONVERTED FROM CHAR
	
	struct PCB pcbRecord = {0, 0, 0};	// HOLD THE PCBRECORD THAT WILL BE TRANSFERED TO THE PCB ARRAY
	
	int PCBcount = 0;		// USED TO KNOW HOW MANY RECORDS ARE STORED IN THE PCB ARRAY
	struct PCB PCBArray[10];	// PCB ARRAY THAT WILL STORE THE PCB RECORD
	
	int pcbMode = 0; 	// USED TO KNOW WHERE TO STORE INT VALUE IN THE PCB RECORD OF EITHER 
				// 0 == PID, 1 == ARRIVAL_TIME, 2 == BURST_TIME

	// PUTS THE USER INPUT INTO 'var0', 'var1', 'vra2'
	if(argv[1] != NULL)
	{
		sscanf(argv[1], "%s", &var0);
	}
	
	if(argv[2] != NULL)
	{
		sscanf(argv[2], "%s", &var1);
	}
	
	if(argv[3] != NULL)
	{
		sscanf(argv[3], "%s", &var2);
	}

	// OPENS THE FILE TO BE READ AND PUTS IT INTO fp
	fp = fopen(var0, "r");
	
	if(fp == NULL)
	{
		printf("\nERROR! input file could not be open\n");
	}

	// IF STATEMENTS THAT WILL CHECK WHICH MODE THE PROGRAM WILL BE RUNNING IN (SRTF, RR, FCFS)
	if(argv[2] != NULL)
	{		
		// CHECKING FOR FCFS MODE
		for(x = 0; x < 4; x++)
		{
			if(var1[x] == fcfsMode[x] || var1[x] == fcfsModeLow[x])
			{
				matchChar++;
			}
		}
		
		if(matchChar == 4)
		{	// ALL LETTER MATCHES FOR 'FCFS' OR 'fcfs' AND MODE BECOMES '1'
			mode = 1;
		}
		
		
		// IF STATEMENT TO CHECK FOR RR MODE
		if(mode == 0)
		{	
			matchChar = 0;
			// CHECKS FOR RR MODE
			for(x = 0; x < 2; x++)
			{
				if(var1[x] == rrMode[x] || var1[x] == rrModeLow[x])
				{
					matchChar++;
				}
			}

			if(matchChar == 2)
			{	// ALL LETTERS MATCHES FOR 'RR' OR 'rr' AND MODE BECOMES '2'
				mode = 2;
			}			
		}
		
		// IF STATEMENT TO CHECK FOR SRTF MODE
		if(mode == 0)
		{
			matchChar = 0;
			//CHECKS FOR SRTF MODE
			for(x = 0; x < 4; x++)
			{
				if(var1[x] == srtfMode[x] || var1[x] == srtfModeLow[x])
				{
					matchChar++;
				}
			}
		
			if(matchChar == 4)
			{	// ALL LETTERS MATCHES FOR 'SRTF' or 'srtf' AND MODE BECOMES '3'
				mode = 3;
			}
		}			
	}
	
	// IF MODE == 2 THEN THE PROGRAM WILL EXECUTE ROUND-ROBIN AND WILL NEED THE QUANTUM TIME
	if(mode == 2)
	{
		quantumTimeRR = atoi(var2);	
	}

	c = getc(fp);  // SOMETHING IS HAPPENING HERE

	// READS IN THE RELEVANT NUMBERS FROM THE INPUT FILE AND STORES THE NUMBERS IN PCBARRAY
	while(c != EOF)
	{	
		// CHECKS FOR 'input.1' IN THE FILE AND READS IN EVERYTHING AFTER THAT
		if(c == input[i])
		{
			if(avgLen < 7)
			{
				i++;
				avgLen++;
			}
			
			if(avgLen == 7)
			{
				c = getc(fp);
				c = getc(fp);
			}
		}

		// IF avgLen == 7 MEANS WE FOUND 'input.1' AND WE ARE NOW INPUTING THE REST OF THE DATA INTO THE PCBARRAY
		if(avgLen == 7)
		{	
			if(c >= 48 && c <= 57 )
			{	// CHAR NUMBER FOUND
				holderChar = c;
				c = getc(fp);
				
			}
			
			// IF C DOESN'T EQUAL TAB(9) OR NEWLINE(10) THEN PROCEED TO CONVERTING THE CHAR NUMBERS TO INT
			if(c != 9 && c != 10)
			{
				// CONVERTS A DOUBLE DIGIT CHAR INTO DOUBLE DIGIT INT
				doubleDigit[0] = holderChar;
				doubleDigit[1] = c;
				sscanf(doubleDigit, "%d", &convertedInt);
				c = getc(fp);	
			}
			else
			{	// CONVERTS A SINGLE DIGIT CHAR INTO A SINGLE DIGIT INT
				doubleDigit[0] = holderChar;
				doubleDigit[1] = '\0';
				convertedInt = 	atoi(doubleDigit);
			}
			
			// CHECKS TO SEE WHICH PART OF THE PCB RECORD THE INPUT SHOULD GO INTO	
			if(pcbMode == 0)
			{	// IF '0' THEN CONVERTED INT IS A PROCESS ID (PID)
				pcbMode = 1;
				pcbRecord.pid = convertedInt;
			}
			else if(pcbMode == 1)
			{	// IF '1' THEN CONVERTED INT IS THE ARRIVAL TIME
				pcbMode = 2;
				pcbRecord.arrival_time = convertedInt;
			}
			else if(pcbMode == 2)
			{	// IF '2' THEN CONVERTED INT IS THE BURST TIME
				pcbMode = 0;
				pcbRecord.burst_time = convertedInt;
			}

			if(pcbMode == 0)
			{	// IF '0' THEN WE HAVE A COMPLETED PCB RECORD, AND STORE IT INTO THE PCB ARRAY
				PCBArray[y] = pcbRecord;
				y++;
				PCBcount++;
			}
		}	
		
		
		if(c < 48 || c > 57 )
		{	// IF C ISN'T A NUMBER CHAR GET THE NEXT CHAR
			c = getc(fp);	
		}
			
	}
	
	c = 0;
	
	// IF STATEMENT THAT WILL DECIDE WHICH MODE WE WILL RUN		
	if(mode == 1)
	{	// IF MODE IS 1 THEN RUN FIRST COME FIRST SERVE
		printf("Schdeuling algorithm: FCFS\n");
		printf("Total %d tasks are read from \"%s\". press 'enter' to start...\n", PCBcount, var0);
		while(c != '\r' && c != '\n')
		{
			c = getchar();
		}
		fcfs(PCBArray, PCBcount); // PASSES THE PCBARRAY AND HOW MANY THERE ARE IN IT
	}
	else if(mode == 2)
	{	// IF MODE IS 2 THEN RUN ROUND ROBIN
		printf("Scheduling algorithm: RR\n");
		printf("Total %d tasks are read from \"%s\". press 'enter' to start...\n", PCBcount, var0);
		while(c != '\r' && c != '\n')
		{
			c = getchar();
		}
		rr(PCBArray, PCBcount, quantumTimeRR); // PASSES THE PCBARRAY AND HOW MANY ARE IN IT AND THE QUANTUM TIME
	} 
	else if(mode == 3)
	{	// IF MODE IS 3 THEN RUN SHORTEST REMAINING TIME FIRST
		printf("Scheduling algorithm: SRTF\n");
		printf("Total %d tasks are read from \"%s\". press 'enter' to start...\n", PCBcount, var0);
		while(c != '\r' && c!= '\n')
		{
			c = getchar();
		}
		srtf(PCBArray, PCBcount); // PASSES THE PCBARRAY AND HOW MANY THERE ARE IN IT
	}
	

	return 0;
}

void srtf(struct PCB fcfsArray[], int arraySize)
{
	
	struct PCB srtfArray[10];
	struct PCB currentPCB; //CURRENT PROCESS THAT IS CURRENTLY BEING EXECUTED

	double arraySizeD;
	arraySizeD = arraySize;

	// WAITING, VARIABLES TO BE USED IN THE CALCULATIONS OF FINDING AVERAGE WAITING TIME
	int PCBwaiting[10] = {0, 0, 0, 0, 0, 0, 0};
	int PCBarrived[10] = {0, 0, 0, 0, 0, 0, 0};
	int PCBfinished[10] = {0, 0, 0, 0, 0, 0, 0};
	int totalWaitTime = 0;
	double totalWaitTimeD;
	double avgTotalWaitTime;	
	
	// RESPONSE, VARIABLES TO BE USED IN THE CALCULATIONS OF FINDING AVERAGE RESPONSE TIME
	int PCBresponse[10] = {0, 0, 0, 0, 0, 0, 0};
	int PCBhasRespond[10] = {0, 0, 0, 0, 0, 0, 0};
	int responseTime = 0;
	int totalResponse = 0;
	double totalResponseD;
	double avgResponse;
	
	// TURNAROUND, VARIABLES TO BE USED IN THE CALCULATIONS OF FINDING AVERAGE TURNAROUND TIME
	int PCBturnAround[10] = {0, 0, 0, 0, 0, 0, 0};	
	int totalTurnAround = 0;
	double totalTurnAroundD;
	double avgTotalTurnAround;	
	int turnAroundTime = 0;

	// CPU UTILIZATION VARIABLES
	double cpuUse = 0.0;
	double cpuAvgUse = 0.0;

	int time = 0;	// TIME VARIABLE TO KEEP TRACK OF SYSTEM TIME
	int currentPos = 0;	
	
	int totalBurstTime = 0; // STORES THE MAX BURST TIME OF ALL COMBINES PROCESSES THAT WAS INPUTED
	int x;
	
	int shortestTime = 0;	// KEEPS TRACK OF THE SHORTEST BURST TIME IN THE PCBARRAY
	int newInput = 0;	// SIGNAL VARIABLE THAT NOTIFYS THE PROGRAM THAT A NEW PROCESS HAS COME IN	
	
	printf("==============================================================\n");
	
	while(time <= totalBurstTime)
	{	
		
		while(time == fcfsArray[currentPos].arrival_time)
		{	// PROCESS ARRIVAL TIME == SYSTEM TIME, MEANS A PROCESS HAS ARRIVED
			srtfArray[currentPos] = fcfsArray[currentPos];
			
			// CALCULATES TOTAL BURST TIME OF ALL PROCESS THAT HAS COME IN
			totalBurstTime = totalBurstTime + srtfArray[currentPos].burst_time;		

			// CODE FOR 'WAITING' CALCULATIONS, SIGNALS THAT A CERTIAN PROCESS HAS ARRIVED
			PCBarrived[srtfArray[currentPos].pid] = 1;

			currentPos++;
			newInput = 1;	
		}
		
		
		if(newInput == 1)
		{
			
			newInput = 0;
			
			// UPDATES THE srtfArray of their current up to date burst_time
			for(x = 0; x < arraySize; x++)
			{
				if(currentPCB.pid == srtfArray[x].pid)
				{
					srtfArray[x].burst_time = currentPCB.burst_time;
				}
			}

			// ASSIGNS THE NEWEST PROCESS TO THE CURRENTPCB THAT HAS THE SHORTESTTIME	
			for(x = 0; x < currentPos; x++)
			{
				if(shortestTime == 0)
				{
					shortestTime = srtfArray[x].burst_time;
					currentPCB = srtfArray[x];
				}
				else if(shortestTime > srtfArray[x].burst_time)
				{
					shortestTime = srtfArray[x].burst_time;
					currentPCB = srtfArray[x];
				}
			}
			
		} // END OF NEWINPUT IF STATEMENT

	
		// CONTINUES EXECUTING CURRENT PROCESS THAT HAS THE SHORTEST TIME
		if(currentPCB.burst_time != 0)
		{	
			
			currentPCB.burst_time = currentPCB.burst_time - 1;
			printf("<system time	%.2d> process	%d is running\n", time, currentPCB.pid);
			shortestTime--;
			cpuUse++;	
			responseTime = currentPCB.arrival_time;			
	
			// RESPONSE TIME CODE, CALCULATES THE RESPONSE TIME OF PROCESSES
			if(PCBresponse[currentPCB.pid] == 0 && PCBhasRespond[currentPCB.pid] == 0)
			{	
				PCBhasRespond[currentPCB.pid] = 1;
				x = time - responseTime ;
				PCBresponse[currentPCB.pid] = x;
			}
			
		}	
		else
		{	// ELSE PROCESS IS FINSIHED EXECUTING AND WE NEED TO FIND A NEW PROCESS
			
			printf("<system time	%.2d> process	%d is finished ...\n", time, currentPCB.pid);
			
			//	TURNAROUND CALCULATIONS CODE
			turnAroundTime = time - currentPCB.arrival_time;
			PCBturnAround[currentPCB.pid] = turnAroundTime;
			
			//	WAITING CODE THAT TELLS THE PROGRAM THAT A CERTIAN PROCESS HAS FINISHED PROCESSING
			PCBfinished[currentPCB.pid] = 1;
			
			// UPDATES THE SRTFARRAY OF THE BURST_TIME TO SHOW THAT THE PROCESS IS DONE EXECUTING
			for(x = 0; x < arraySize; x++)
			{
				if(srtfArray[x].pid == currentPCB.pid)
				{
					srtfArray[x] = currentPCB;
				}
			}
				
			// FINDING A NEW PROCESS TO EXECTUTE WITH THE SHROTEST TIME
			for(x = 0; x < currentPos; x++)
			{
				if(shortestTime ==  0)
				{
					shortestTime = srtfArray[x].burst_time;
					currentPCB = srtfArray[x];
				}
				else if(srtfArray[x].burst_time != 0)
				{
					if(srtfArray[x].burst_time < shortestTime)
					{
						shortestTime = srtfArray[x].burst_time;
						currentPCB = srtfArray[x];
					}
				}
			} 

			// EXECUTE THE NEW PROCESS THAT WE JUST FOUND IN THE CODE RIGHT ABOVE THIS ONE 		
			if(time != totalBurstTime)
			{	
				if(currentPCB.burst_time != 0)
				{	
					currentPCB.burst_time = currentPCB.burst_time - 1;
					printf("<system time	%.2d> process	%d is running\n", time, currentPCB.pid);
					shortestTime--;
					cpuUse++;
					
					responseTime = currentPCB.arrival_time;				
	
					// RESPONSE CODE
					if(PCBresponse[currentPCB.pid] == 0 && PCBhasRespond[currentPCB.pid] == 0 )
					{	
						PCBhasRespond[currentPCB.pid] = 1;
						x = time - responseTime;
						PCBresponse[currentPCB.pid] = x;
					}
				}				
			}
			else
			{	// WE HAVE FINISH EXECUTING ALL PROCESSES
				printf("<system time	%.2d> All process finish ..............\n\n", time);
			}
			
		}
		
		// INCREMENTS TIME 	
	 	time = time + 1;
		
		
		// INCREAMENTS WAITING TIME FOR ALL PROCESSES THAT ISN'T FINSIHED AND ISN'T CURRENTLY RUNNING
		for(x = 0; x <= arraySize; x++)
		{	
			if(PCBarrived[x] == 1 && PCBfinished[x] == 0 && currentPCB.pid != x)
			{
				PCBwaiting[x]++;
			}
		}
		
	}	// end of while(time <= totalBurstTime)
	
	
	//	CALCULATES AVERAGE TURNAROUND TIME AND DISPLAYS IT	
	for(x = 0; x <= arraySize; x++)
	{
		totalTurnAround = totalTurnAround + PCBturnAround[x];
	}	

	totalTurnAroundD = totalTurnAround;
	avgTotalTurnAround = totalTurnAroundD/arraySizeD;
	
	//	CALCUALTES AVERAGE RESPONSE TIME AND DISPLAYS IT
	for(x = 0; x <= arraySize; x++)
	{	//printf("%d + %d", totalResponse, PCBresponse[x]);
		totalResponse = totalResponse + PCBresponse[x];
		//printf(" = %d\n", totalResponse);
	}
	
	totalResponseD = totalResponse;
	avgResponse = totalResponseD/arraySizeD;
		
	//	CALCULATES AVERAGE WAITING TIME AND DISPLAYERS IT
	for(x = 1; x <= arraySize; x++)
	{
		totalWaitTime = totalWaitTime + PCBwaiting[x];	
	}
	
	totalWaitTimeD = totalWaitTime;
	avgTotalWaitTime = totalWaitTimeD/arraySizeD;
	
	//	CALCULATES AVERAGE CPU USAGE
	
	cpuAvgUse = cpuUse/totalBurstTime;
	cpuAvgUse = cpuAvgUse * 100.0;
		
	//	DISPLAYS CALCULATIONS OF AVERAGE TURNAROUND, RESPONSE, AND WAITING TIME
	
	printf("==============================================================\n");	

	printf("Average cpu usage	: %.2f%%\n", cpuAvgUse);
	
	printf("Average waiting time	: %.2f\n", avgTotalWaitTime);
	
	printf("Average response time	: %.2f\n", avgResponse);

	printf("Average turnaround time	: %.2f\n", avgTotalTurnAround);	
	
	printf("==============================================================\n");
	
}



void rr(struct PCB fcfsArray[], int arraySize, int quantumTime)
{
	struct PCB rrArray[10];
	struct PCB currentPCB;

	double arraySizeD = arraySize;
	
	// WAIT TIME VARIABLES
	int PCBwaitTime[10] = {0, 0, 0, 0, 0, 0};
	int PCBfinished[10] = {0, 0, 0, 0, 0, 0};
	int PCBarrived[10] = {0, 0, 0, 0, 0, 0};
	double totalWaitTime = 0.0;
	double avgWaitTime;
		
	// RESPONSE TIME VARIABLES
	int PCBresponseTime[10] = {0, 0, 0, 0, 0, 0};
	int PCBresponse[10] = {0, 0, 0, 0, 0, 0};
	double totalResponseTime = 0.0;
	double avgResponseTime;	

	// TURNAROUND TIME VARIABLES
	int PCBturnAround[10] = {0, 0, 0, 0, 0, 0};
	double totalTurnAroundTime = 0.0;
	double avgTurnAround;

	//	CPU USAGE VARIABLES
	double cpuUse = 0.0;
	double cpuAvgUse = 0.0;

	// USED A CIRCLE ARRAY TO KEEP TRACK OF PROCESS THAT IS RUNNING IN ROUND ROBING		
	int front = 0;	// VARIABLE TO POINT TO THE PROCESS AT THE FRONT
	int rear = 0;	// VARIABLE TO POINT TO THE PROCESS AT THE REAR
	
	int time = 0;	// KEEPS TRACK OF SYSTEM TIME
	int totalBurstTime = 0;
	int currentPos = 0;	
	int opTime = quantumTime;	// HOLDS QUANTUM TIME FOR ROUND ROBIN
	
	// variables to be used in FOP loops
	int y; 
	int x;	
	
	printf("==============================================================\n");
	
	while(time <= totalBurstTime)
	{
		while(time == fcfsArray[currentPos].arrival_time)
		{
			rrArray[rear] = fcfsArray[currentPos];
			
			// CALCUALTES TOTAL BURST TIME FOR ALL PROCESSES
			totalBurstTime = totalBurstTime + fcfsArray[currentPos].burst_time;
			
			PCBarrived[currentPos] = 1;
			currentPos++;
			
			rear = resetFlag(rear, arraySize); // RECALCUALTES WHERE THE REAR POINTER WILL POINT TO
		}
		
		
		if(opTime == quantumTime)
		{	// FIND A NEW PROCESS TO RUN AND EXECUTE
			// GET PROCESS FROM FRONT OF QUE
			currentPCB = rrArray[front];
	
			currentPCB.burst_time = currentPCB.burst_time - 1;
			printf("<system time	%.2d> process	%d is running \n", time, currentPCB.pid);
			cpuUse++;
				
			// RESPONSE TIME CALCULATIONS
			if(PCBresponse[currentPCB.pid] == 0)
			{	
				PCBresponse[currentPCB.pid] = 1;
				if(PCBresponse[currentPCB.pid] == 1)
				{
					PCBresponseTime[currentPCB.pid] = time - currentPCB.arrival_time;
				}
			}
			
			front = resetFlag(front, arraySize); // RECALCULATES WHERE THE FRONT POINTER WILL POINT TO
			opTime--;
		}
		else if(currentPCB.burst_time == 0 && opTime != 0)
		{	// PROCESS HAS FINISHED RUNNING AND WE NEED TO GET NEW PROCESS TO RUN
			printf("<system time	%.2d> process	%d is finished  ...\n", time, currentPCB.pid);
			PCBturnAround[currentPCB.pid] = time - currentPCB.arrival_time; 			
					

			PCBfinished[currentPCB.pid] = 1; // marks in the array that process# is finished
	
			if(time != totalBurstTime)
			{
				// GET PROCESS FROM FRONT OF QUE
				currentPCB = rrArray[front];		
				front = resetFlag(front, arraySize);	
				currentPCB.burst_time = currentPCB.burst_time - 1;
				printf("<system time	%.2d> process	%d is running \n", time, currentPCB.pid);
				cpuUse++;			
				
				// RESPONSE TIME CALCULATIONS
				if(PCBresponse[currentPCB.pid] == 0)
				{
					PCBresponse[currentPCB.pid] = 1;
					if(PCBresponse[currentPCB.pid] == 1)
					{
						PCBresponseTime[currentPCB.pid] = time - currentPCB.arrival_time;
					}
				}				
				// RESETS OPTIME
				opTime = quantumTime;
				opTime--;
			}
			else
			{	// ALL PROCESS HAS FINSIHED RUNNING
				printf("<system time	%.2d> All processes finish ............\n", time);
			}	
		}
		else if(opTime == 0)
		{	// EXECUTING TIME HAS REACHED 0 AND WE NEED TO FIND IF THERE IS A NEW PROCESS TO RUN
			opTime = quantumTime;
			if(currentPCB.burst_time == 0)
			{	// PROCESS CURRENTLY RUNNING HAS FINSHED AND WE NEED TO RUN THE NEXT PROCESS
				printf("<system time	%.2d> process	%d is finished  ...\n", time, currentPCB.pid);
				PCBturnAround[currentPCB.pid] = time - currentPCB.arrival_time;
						
			
				PCBfinished[currentPCB.pid] = 1;
				
				currentPCB = rrArray[front];
				front = resetFlag(front, arraySize);
				currentPCB.burst_time = currentPCB.burst_time - 1;
				printf("<system time	%.2d> process	%d is running \n", time, currentPCB.pid);
				cpuUse++;				

				// RESPONSE TIME CALCULATIONS
				if(PCBresponse[currentPCB.pid] == 0)
				{
					PCBresponse[currentPCB.pid] = 1;
					if(PCBresponse[currentPCB.pid] == 1)
					{
						PCBresponseTime[currentPCB.pid] = time - currentPCB.arrival_time;
					}		
				}
				opTime--;		
			}
			else
			{	// FINDS NEXT PROCESS TO RUN THAT IS IN QUE
				rrArray[rear] = currentPCB;
				rear = resetFlag(rear, arraySize);
				currentPCB = rrArray[front];
				front = resetFlag(front, arraySize);
				currentPCB.burst_time = currentPCB.burst_time - 1;
				printf("<system time	%.2d> process	%d is running \n", time, currentPCB.pid);
				cpuUse++;
				if(PCBresponse[currentPCB.pid] == 0)
				{
					PCBresponse[currentPCB.pid] = 1;
					if(PCBresponse[currentPCB.pid] == 1)
					{
						PCBresponseTime[currentPCB.pid] = time - currentPCB.arrival_time;
					}
				}
				
				opTime--;
			}
				
		}
		else
		{	// THERE IS STILL EXECUTING TIME REMAINING SO JSUT EXECUTE PROCESS
			printf("<system time	%.2d> process	%d is running \n", time, currentPCB.pid);
			currentPCB.burst_time = currentPCB.burst_time - 1;
			
			cpuUse++;
			
			if(PCBresponse[currentPCB.pid] == 0)
			{
				PCBresponse[currentPCB.pid] = 1;
				if(PCBresponse[currentPCB.pid] == 1)
				{
					PCBresponseTime[currentPCB.pid] = time - currentPCB.arrival_time;	
				}
			}
			opTime--;
		}
		time = time + 1;		
	
		// ALL PROCESS THAT ISN'T EXECTING WILL BE INCREMENTED FOR WIATING CALUCLATIONS
		for(y = 1; y <= arraySize; y++)
		{
			
			if(PCBfinished[y] == 0 && PCBarrived[y] == 1)
			{
				PCBwaitTime[y]++;
			}
			
		}
	}
		
	// CALCULATES AND PRINTS THE AVERAGE RESPONSE TIME FOR ALL PROCESSES
	for(y = 1; y <= arraySize; y++)
	{
		
		totalResponseTime = totalResponseTime + PCBresponseTime[y];	
	}
	
	avgResponseTime = totalResponseTime/arraySizeD;
	

	// CALCULATE AND PRINTS THE AVERAGE TURNAROUND TIME OR ALL PROCESS
	for(y = 1; y <= arraySize; y++)
	{
		totalTurnAroundTime = totalTurnAroundTime + PCBturnAround[y];
	}
	
	avgTurnAround = totalTurnAroundTime/arraySizeD;

		
	// CALCULATES AND PRINTS THE AVERAGE WAIT TIME FOR ALL PROCESSES	
	for(y = 1; y <= arraySize; y++)
	{
	
		totalWaitTime = totalWaitTime + PCBwaitTime[y];	
	}
	
	avgWaitTime = totalWaitTime/arraySizeD;

	// CPU USAGE CALCULATIONS
	
	cpuAvgUse = cpuUse/totalBurstTime;
	cpuAvgUse = cpuAvgUse * 100;

	printf("========================================================\n");	
	
	printf("Average cpu usage	: %.2f%%\n",cpuAvgUse );
	
	printf("Average waiting time	: %.2f\n", avgWaitTime);
	
	printf("Average response time	: %.2f\n", avgResponseTime);
		
	printf("Average turnaround time	: %.2f\n", avgTurnAround);
	
	printf("========================================================\n");
}

int resetFlag(int flag, int arraySize)
{	// FUNCTION THAT DETERMINES THE FRONT AND REAR END OF THE CIRCLE ARRAY QUE FOR ROUND ROBIN
	if(flag ==  arraySize)
	{
		flag = 0;
	}
	else
	{
		flag++;
		if(flag == arraySize)
		{
			flag = 0;
		}
	}

	return flag;
}	

// First Come First Servce module to execute the process that comes in first
void fcfs(struct PCB fcfsArray[], int arraySize)
{	
	struct PCB fcfsArrayQue[10];
	
	double arraySizeDouble = arraySize;

	// WAIT TIME & RESPONSE TIME VARIABLE CALCULATIONS
	int responseTimeArray[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	double totalWaitTime = 0.0;
	double averageWaitTime;
	double totalWaitTimeDouble;

	// TURNAROUND TIME VARIABLE CALCULATIONS
	int turnAroundTimeArray[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	double totalTurnAroundTime = 0.0;
	double avgTurnAroundTimeD;
	
	// CPU USAGE VARIABLES
	double cpuUse = 0.0;
	double cpuAvgUse = 0.0;	
		
	// variables to used in for loops
	int y = 0;
	int z = 0;
	int x = 0;

	int totalBurstTime = 0;
		
	int time = 0;
	int currentArraySize = 0;
	int currentPosition = 0;

	printf("==============================================================\n");

	while(time <= totalBurstTime)
	{
	 	while(time == fcfsArray[currentArraySize].arrival_time)
		{	// this module assigns the process to the que base on their arrival time
			fcfsArrayQue[currentArraySize] = fcfsArray[currentArraySize];	
			
			// CALCULATES TOTAL BURST TIME OF ALL PROCESSES
			totalBurstTime = totalBurstTime + fcfsArray[currentArraySize].burst_time;			

			currentArraySize++;  
		}
		
		if(fcfsArrayQue[currentPosition].burst_time > 0)
		{	// executes currently running process	
			printf("<system time	%.2d> process	%d is running\n", time, fcfsArrayQue[currentPosition].pid);
			cpuUse++;
			fcfsArrayQue[currentPosition].burst_time--;
			
		}		
		else
		{
					
			if(currentPosition < arraySize)
			{	// code for turnaround time calculations
				turnAroundTimeArray[currentPosition] = time - fcfsArrayQue[currentPosition].arrival_time;
			}			

			printf("<system time	%.2d> process	%d is finished.......\n", time, fcfsArrayQue[currentPosition].pid);
			currentPosition++;			
			
			if(currentPosition < arraySize)
			{	// code for response time calculations
				responseTimeArray[currentPosition] = time - fcfsArrayQue[currentPosition].arrival_time;
			}		
	
			if(currentPosition < arraySize)
			{	
				fcfsArrayQue[currentPosition].burst_time--;
				printf("<system time	%.2d> process	%d is running\n", time, fcfsArrayQue[currentPosition].pid);
				cpuUse++;
			}
		}
		
		if(time == totalBurstTime)
		{
			printf("<system time	%.2d> All processes finish....................\n", time);
		}
		
		time = time + 1;		
	}
		
	//===============================================
	//	CALCULATES AVERAGE WAIT TIME AND RESPONSE TIME	
	for(y = 0; y < arraySize; y++)
	{
		totalWaitTime = totalWaitTime + responseTimeArray[y];	

	}
		
	averageWaitTime = totalWaitTime/arraySizeDouble;
	
	//==================================================
	//	CALCULATES AVERAGE TURNAROUND TIME
	for(z = 0; z < arraySize; z++)
	{
		totalTurnAroundTime = totalTurnAroundTime + turnAroundTimeArray[z];	
	}	

	avgTurnAroundTimeD = totalTurnAroundTime/arraySizeDouble;
	
	//==================================================
	//	CALCULATES AVERAGE CPU USAGES
	cpuAvgUse = cpuUse/totalBurstTime;
	cpuAvgUse = cpuAvgUse * 100;
		
	//==================================================
	//	DISPLAYS THE AVERAGE WAITING TIME, RESPONSE TIME, AND TURNAROUND TIME
	
	printf("================================================================\n");
	
	printf("Average cpu useage	: %.2f%%\n", cpuAvgUse);
	
	printf("Average waiting time	: %.2f\n", averageWaitTime);
	
	printf("Average response time	: %.2f\n", averageWaitTime);	

	printf("Average turnaround time	: %.2f\n", avgTurnAroundTimeD);

	printf("=================================================================\n");
		
};

