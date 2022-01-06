/*
 * var.h
 *
 * Created: 16/12/2021 08:34:07 م
 *  Author: ahmed essam
 */ 


#ifndef VAR_H_
#define VAR_H_

char g = 0;
char trash;
char ov_num = 0;
int cpuls_num = 0;
int puls_num = 0;                  //number of pulses of one signal
//int pulse_num[10];               //array of number of pulses for 11 signal
char buffer1[20],buffer2[5];
int  puls_time[100];                  //array of time per pulses for one signal
int signal_num = 0;                //number of received signal
long recieved_ir = 0;               //array of row data for 11 signal

/*typedef struct
{
	int fir[100];
	int sec[100];
	int thir[100];
	int four[100];
	int fifth[100];
	int sexth[100];
	int seventh[100];
	int eighth[100];
	int nighnth[100];
	int tenth[100];
}puls_tim;*/

//puls_tim puls_time_psignal;



#endif /* VAR_H_ */