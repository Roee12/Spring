/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1       /* callback function: panelFunc */
#define  PANEL_START                      2       /* control type: textButton, callback function: startSimulationFunc */
#define  PANEL_CANVAS                     3       /* control type: canvas, callback function: (none) */
#define  PANEL_INIT_Y_2                   4       /* control type: numeric, callback function: changeY */
#define  PANEL_INIT_Y_1                   5       /* control type: numeric, callback function: changeY */
#define  PANEL_K_2                        6       /* control type: numeric, callback function: (none) */
#define  PANEL_K_1                        7       /* control type: numeric, callback function: (none) */
#define  PANEL_TIMER                      8       /* control type: timer, callback function: timerFunc */
#define  PANEL_MASS_2                     9       /* control type: numeric, callback function: (none) */
#define  PANEL_MASS_1                     10      /* control type: numeric, callback function: (none) */
#define  PANEL_STRIPCHART                 11      /* control type: strip, callback function: (none) */
#define  PANEL_GRAPH                      12      /* control type: graph, callback function: (none) */
#define  PANEL_LINK                       13      /* control type: command, callback function: showVideoFunc */
#define  PANEL_SOUND                      14      /* control type: binary, callback function: soundFunc */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK changeY(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK panelFunc(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK showVideoFunc(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK soundFunc(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK startSimulationFunc(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK timerFunc(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
