#include <cvintwrk.h>
#include "bass.h" 
#include <analysis.h>
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "springDemo.h"

HSTREAM Music_SND;

typedef struct
{
	int width;
	int height;
	int top;
	int left;
	double k;
} spring;

typedef struct
{
	int height;
	int width;
	double mass;
	double vy;
	double ay;
	double y;
} weight;

static int panelHandle;   

spring s1,s2;
weight w1,w2;

int spring_bmp;
double *real_arr,*img_arr,*y_arr,*x_arr;
double w_arr[2];
double dt=0.000001;
double g=9.81;
int counter=0;

void update()
{
	for(int i=0;i<20000;i++)
	{
		w1.ay=g-s1.k*w1.y/w1.mass+s2.k*w2.y/w1.mass;
		w1.y=w1.y+w1.vy*dt+w1.ay*dt*dt*0.5;
		w1.vy=w1.vy+w1.ay*dt;
		
		w2.ay=g-s2.k*w2.y/w2.mass;
		w2.y=w2.y+w2.vy*dt+w2.ay*dt*dt*0.5;
		w2.vy=w2.vy+w2.ay*dt;
	}
	
	w_arr[0]=w1.y;
	w_arr[1]=w2.y;
	real_arr=(double*)realloc(real_arr,(counter+1)*sizeof(double));
	real_arr[counter]=w1.y+w2.y;
	counter++;
}

void draw()
{
	CanvasStartBatchDraw (panelHandle, PANEL_CANVAS);
	
	CanvasClear (panelHandle, PANEL_CANVAS, VAL_ENTIRE_OBJECT);
   	CanvasDrawBitmap (panelHandle, PANEL_CANVAS, spring_bmp, VAL_ENTIRE_OBJECT, MakeRect(s1.top,s1.left,s1.height+w1.y,s1.width));
	CanvasDrawRect (panelHandle, PANEL_CANVAS, MakeRect(s1.height+w1.y,s1.left,w1.height,w1.width), VAL_DRAW_FRAME_AND_INTERIOR);
	CanvasDrawBitmap (panelHandle, PANEL_CANVAS, spring_bmp, VAL_ENTIRE_OBJECT, MakeRect(s1.height+w1.y+40,s2.left,s2.height+w2.y,s2.width));
	CanvasDrawRect (panelHandle, PANEL_CANVAS, MakeRect(s1.height+w1.y+40+s2.height+w2.y,s2.left,w2.height,w2.width), VAL_DRAW_FRAME_AND_INTERIOR); 
	
	CanvasEndBatchDraw (panelHandle, PANEL_CANVAS);
}
void init()
{
	real_arr=(double*)calloc(1,sizeof(double));
	img_arr=(double*)calloc(1,sizeof(double));
 	y_arr=(double*)calloc(1,sizeof(double));
	x_arr=(double*)calloc(1,sizeof(double));
	GetBitmapFromFile ("spring.png", &spring_bmp);
	
	//Music
	BASS_Init( -1,44100, 0,0,NULL);
	Music_SND = BASS_StreamCreateFile(FALSE,"The Godfather Theme Song.mp3",0,0,0);
	BASS_ChannelPlay(Music_SND,TRUE);
	
	//Spring 1 initialization
	s1.width=40;
	s1.height=180;
	s1.left=100;
	s1.top=0;

	
	//Weight 1 initialization
	w1.width=40;
	w1.height=40;
	w1.y=0.0;
	
   	//Spring 2 initialization
	s2.width=40;
	s2.height=180;
	s2.left=100;
	s2.top=220;

	
	//Weight 2 initialization
	w2.width=40;
	w2.height=40;
	w2.y=0.0;
	
	//Set colors
	SetCtrlAttribute (panelHandle, PANEL_CANVAS, ATTR_PEN_FILL_COLOR, VAL_RED);
	//Draw initial system state
	draw();
}
void end_all()
{
	DiscardBitmap (spring_bmp);
	free(real_arr);
	free(img_arr);
	free(y_arr);
	free(x_arr);
	BASS_StreamFree(Music_SND);
}

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "springDemo.uir", PANEL)) < 0)
		return -1;
	init();
	DisplayPanel (panelHandle);
	RunUserInterface ();
	end_all();
	DiscardPanel (panelHandle);
	return 0;
}

int CVICALLBACK panelFunc (int panel, int event, void *callbackData,
						   int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface (0);
			break;
	}
	return 0;
}

int CVICALLBACK startSimulationFunc (int panel, int control, int event,
									 void *callbackData, int eventData1, int eventData2)
{
	int state,i;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panelHandle,PANEL_START,&state);
			if (state==1)
			{
				GetCtrlVal(panelHandle,PANEL_K_1,&s1.k);
				GetCtrlVal(panelHandle,PANEL_MASS_1,&w1.mass);
				GetCtrlVal(panelHandle,PANEL_K_2,&s2.k);
				GetCtrlVal(panelHandle,PANEL_MASS_2,&w2.mass);
				w1.vy=0.0;
				w1.ay=0.0;
				w2.vy=0.0;
				w2.ay=0.0;
			}
			else
			{
				img_arr=(double*)realloc(img_arr,(counter)*sizeof(double));
				y_arr=(double*)realloc(y_arr,(counter)*sizeof(double));
				x_arr=(double*)realloc(x_arr,(counter)*sizeof(double));
				if (counter>0) ReFFT (real_arr, img_arr, counter);
				for(i=0;i<counter;i++)
				{
					x_arr[i]=i/(counter*0.02);
					y_arr[i] = sqrt(pow(real_arr[i], 2)+pow(img_arr[i],2));
				}
				if (counter!=0) PlotXY (panelHandle, PANEL_GRAPH, x_arr, y_arr, counter, VAL_DOUBLE, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);
				SetAxisScalingMode (panelHandle, PANEL_GRAPH, VAL_BOTTOM_XAXIS, VAL_MANUAL, 0.0, 5.0);
			}
			SetCtrlAttribute (panelHandle, PANEL_TIMER, ATTR_ENABLED, state);
			break;
	}
	return 0;
}


int CVICALLBACK timerFunc (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
			
			update();
			draw();
			
			PlotStripChart (panelHandle, PANEL_STRIPCHART, w_arr, 2, 0, 0, VAL_DOUBLE);
			
			break;
	}
	return 0;
}



int CVICALLBACK changeY (int panel, int control, int event,
						  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panelHandle,PANEL_INIT_Y_1,&w1.y);
			GetCtrlVal(panelHandle,PANEL_INIT_Y_2,&w2.y);
			draw();
			break;
	}
	return 0;
}

int CVICALLBACK showVideoFunc (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			InetLaunchDefaultWebBrowser ("https://www.youtube.com/watch?v=tNpuTx7UQbw");
			break;
	}
	return 0;
}

int CVICALLBACK soundFunc (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	int sound;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panelHandle,PANEL_SOUND,&sound);
			if (sound==1) BASS_ChannelSetAttribute(Music_SND, BASS_ATTRIB_VOL, 1.0);
			else BASS_ChannelSetAttribute(Music_SND, BASS_ATTRIB_VOL, 0.0);
			break;
	}
	return 0;
}
