/*********************************************************

   test

   --------------------------------------------------------

   generated at Tue Apr 30 17:15:52 2002

   by snns2c ( Bernward Kett 1995 )

*********************************************************/



#include <math.h>



#define Act_Logistic(sum, bias)  ( (sum+bias<10000.0) ? ( 1.0/(1.0 + exp(-sum-bias) ) ) : 0.0 )

#ifndef NULL

#define NULL (void *)0

#endif



typedef struct UT {

           float act;         /* Activation       */

           float Bias;        /* Bias of the Unit */

           int   NoOfSources; /* Number of predecessor units */

    struct UT   **sources; /* predecessor units */

           float *weights; /* weights from predecessor units */

         } UnitType, *pUnit;



   /* Forward Declaration for all unit types */

   static UnitType Units[93];

   /* Sources definition section */

   static pUnit Sources[] =  {

Units + 1, Units + 2,

Units + 1, Units + 2,

Units + 1, Units + 2,

Units + 1, Units + 2,

Units + 1, Units + 2,

Units + 1, Units + 2,

Units + 1, Units + 2,

Units + 1, Units + 2,

Units + 1, Units + 2,

Units + 1, Units + 2,

Units + 1, Units + 2,

Units + 1, Units + 2,

Units + 1, Units + 2,

Units + 1, Units + 2,

Units + 1, Units + 2,

Units + 1, Units + 2,

Units + 1, Units + 2,

Units + 1, Units + 2,

Units + 1, Units + 2,

Units + 1, Units + 2,

Units + 1, Units + 2,

Units + 1, Units + 2,

Units + 1, Units + 2,

Units + 1, Units + 2,

Units + 1, Units + 2,

Units + 1, Units + 2,

Units + 1, Units + 2,

Units + 1, Units + 2,

Units + 1, Units + 2,

Units + 1, Units + 2,

Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 18, 
Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 28, Units 
+ 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, Units + 
9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,



Units + 3, Units + 4, Units + 5, Units + 6, Units + 7, Units + 8, 
Units + 9, Units + 10, Units + 11, Units + 12,

Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units + 
18, Units + 19, Units + 20, Units + 21, Units + 22,

Units + 23, Units + 24, Units + 25, Units + 26, Units + 27, Units + 
28, Units + 29, Units + 30, Units + 31, Units + 32,





   };



   /* Weigths definition section */

   static float Weights[] =  {

-0.489670, -0.517750,

0.643540, 0.462810,

-0.098600, -0.977900,

0.421490, 0.814510,

0.722650, -0.698420,

-0.216280, -0.589160,

-0.080250, -0.259350,

-0.191260, -0.915650,

0.080340, -0.459700,

0.283790, 0.969540,

-0.364060, 0.336340,

-0.601250, -0.335730,

0.411500, 0.679550,

0.575490, 0.284950,

-0.188450, -0.643240,

0.816340, 0.451030,

0.303630, 0.017910,

-0.288920, -0.677660,

-0.740410, 0.341590,

-0.162820, 0.608870,

0.084180, -0.626580,

-0.078710, 0.243830,

-0.733210, 0.874630,

0.640310, -0.598010,

0.862180, 0.845820,

0.907280, 0.993590,

0.989320, 0.046660,

-0.718190, -0.046600,

-0.280920, -0.412580,

0.560170, -0.224460,

-1.090440, 0.786760, -0.206570, -1.093010, -0.198170, -0.179880, 
0.155650, 0.563400, -0.947680, -0.635230,

0.028890, 0.839840, -0.018320, -0.552230, -0.388790, -0.139150, 
-0.504390, 0.275910, 0.591970, -0.961970,

-0.009900, 0.500920, -0.144070, -0.649150, 0.451430, 0.683840, 
-0.992270, 0.073280, -0.806210, -0.351510,



-1.242220, -0.274410, 0.278510, -0.432380, -0.362410, -0.662420, 
0.172340, -0.416160, -0.708160, -0.655570,

0.251530, -0.381840, 0.904500, 0.349350, 0.368710, -0.075850, 0.246470, 0.726960, 
-0.146420, 0.507220,

-0.172520, 0.860770, 0.543690, -0.096780, -0.562760, 0.789020, 
-0.026400, -1.132270, -0.834770, -0.654630,



-0.437480, -0.951530, -0.788570, -1.046040, -0.530240, 0.972010, 
0.008560, -0.650420, -0.280410, -0.949080,

0.808380, 0.114210, -0.123270, 0.678870, 0.427060, 0.681750, 
0.121420, 0.148940, -0.113420, 0.681730,

0.087220, 0.699130, -1.016410, -0.825610, 0.601480, 0.794660, 
-0.498400, -0.691320, -0.413800, -0.116530,



-0.346070, 0.450060, 0.909540, 0.582440, -1.025820, 0.435940, 
0.852120, -0.590230, 0.215290, 0.324250,

-0.469040, -0.777650, -0.403250, -1.023140, 0.420690, -0.714320, 
-0.666390, 0.236090, -0.276800, -0.299580,

-0.135930, -0.518990, -0.232320, -0.896520, 0.576000, -0.235850, 
0.604830, -0.391350, -0.988430, 0.768450,



0.023070, 0.212560, 0.355730, -0.531070, 0.595080, 0.737830, 
0.459210, -0.274670, -0.105140, -0.080670,

0.457500, -0.252370, -1.052310, 0.409950, -0.094850, 0.581580, 
-1.223320, -0.013060, -0.813170, -0.461490,

0.384210, -0.051070, -0.676030, -0.689140, -1.109920, -0.998740, 
0.708310, 0.305360, -0.545590, -0.262490,



-0.236120, 0.148110, -0.059660, 0.522810, -0.560230, 0.447430, 
0.482960, -0.181680, -0.344040, 0.075590,

-0.302350, -0.613820, 0.956300, 0.207740, 0.407390, -0.230570, 
-0.779600, 0.308020, 0.427050, -0.960140,

0.532270, -0.909540, -0.839780, -0.817560, -0.698480, -0.938660, 
-0.331580, 0.655570, 0.326820, -0.918270,



-0.661430, 0.501750, -0.161960, 0.752250, -0.746580, 0.642200, 
-0.506130, 0.038610, -0.023590, 0.117830,

-0.526540, 0.069980, -0.702380, -0.990910, 0.214640, -0.613880, 
0.252970, -0.434430, 0.436140, -0.285440,

-1.010800, -0.040480, -0.134680, -0.581040, -1.099060, 0.582320, 
0.453530, 0.119050, -0.524830, 0.738880,



-0.861870, 0.521640, 0.828550, 0.357700, -0.694460, -0.349530, 
0.032970, 0.683160, 0.348240, 0.607650,

0.468060, -0.218240, -0.711910, 0.397130, -0.119720, 0.745470, 
-0.020730, -0.006260, -0.697380, -0.799920,

-0.456470, -0.481050, -0.386760, -0.003030, -0.628420, -1.094980, 
-1.067640, -0.479050, -0.103730, 0.254480,



-0.031510, 0.220170, -0.722980, -1.003100, -0.978650, 0.878420, 
-0.642840, -0.966800, -0.390360, 0.126360,

-0.080580, 0.195730, -0.963630, -0.526300, 0.568670, -0.883550, 
0.150370, 0.628640, -0.475060, 0.904870,

0.720040, -0.726190, 0.915440, -0.356990, 0.602810, 0.269060, 
-0.360770, 0.973550, -0.625450, 0.076110,



-0.157290, 0.181740, 0.774450, 0.373970, 0.052200, -0.186310, 
-0.924190, 0.060420, 0.281010, -0.782880,

-0.068990, 0.612390, -0.026830, 0.123360, -0.080300, 0.344430, 
0.299250, -0.613320, -0.431570, 0.439740,

-1.210230, 0.255610, -0.711420, -0.684780, 0.284180, -1.258480, 
-0.383850, -0.978420, -0.151260, 0.024230,



-0.503360, -1.041990, 0.019230, 0.837490, -0.759510, -0.470190, 
-0.942360, -1.003960, -0.925470, -0.185760,

0.180140, 0.820560, -0.868600, -0.389160, 0.648850, -0.275840, 
0.682930, 0.307220, -0.225200, 0.175700,

0.430990, 0.302690, -0.462900, 0.306350, -0.203140, -0.463600, 
-0.918330, 0.652460, -0.141920, -0.167160,



-0.822770, 0.778550, -0.504590, -0.886900, -0.445400, -0.605800, 
0.102290, -0.006410, 0.448910, -0.817780,

0.172510, -0.686120, -0.199610, 0.419060, 0.131910, 0.390870, 
-0.755920, 0.449040, 0.367330, -0.022810,

0.844580, -0.642050, -0.474440, -1.090310, -0.202040, -0.122120, 
-0.481970, 0.349950, 0.747200, -0.639000,



0.849540, -0.647860, 0.283630, 0.189780, 0.306380, 0.028750, 
0.073130, -0.786790, -0.845780, -0.360540,

-0.981160, -0.251510, -0.372940, -0.988660, 0.115360, -0.940260, 
0.009970, -0.453430, 0.785520, -0.950180,

0.731650, -0.548560, -0.110790, -0.606130, -0.260040, -0.594290, 
-0.097820, 0.343970, 0.190620, 0.081660,



-0.969620, -0.661520, -0.810580, -0.651390, 0.309320, 0.553220, 
0.650310, 0.543370, -0.280870, 0.212830,

0.924010, -0.273170, -0.027620, -0.088370, 0.264800, -1.138090, 
0.524860, 0.849220, -0.797120, -0.265060,

-1.071840, -0.590710, 0.846260, -0.262160, 0.474140, -1.093170, 
0.064870, -0.812990, 0.240280, 0.654960,



-0.356230, -0.861650, -0.723500, -0.909760, 0.711910, 0.441100, 
-0.804060, -0.481890, -0.508110, 0.666390,

0.545880, 0.038260, -0.140110, -0.745330, -0.117600, -0.864900, 
-0.230780, -0.570620, 0.743790, -0.617140,

0.409540, 0.803610, -0.631950, -0.579570, -0.301070, -0.184460, 
-0.113790, 0.932370, 0.124380, 0.222030,



-0.086000, -1.105320, 0.829520, -0.456890, -1.065920, -0.187800, 
-0.495100, 0.175180, -1.043230, -0.269220,

0.713030, -0.443120, 0.766490, -1.040510, 0.963230, -0.649550, 
-0.013020, -0.276380, 0.074950, -0.482450,

-0.719680, -0.674120, -1.083010, -0.289000, 0.787960, -0.507590, 
0.637080, 0.870070, -0.508390, 0.842460,



-0.267800, -0.438070, 0.190370, -0.094010, -1.036400, 0.006280, 
-0.375960, -0.778150, 0.192040, -0.143980,

-0.270250, -0.230490, -0.498940, -0.100150, 0.765430, -0.839110, 
-0.078690, -1.000060, -0.683900, -0.251480,

-0.641520, 0.501260, 0.646990, 0.337300, -0.175430, -0.723900, 
0.468950, 0.858960, 0.266480, -0.415710,



0.460390, 0.725050, -0.970410, -0.998560, -0.728840, -0.456350, 
-0.783680, -0.945110, -0.127020, 0.201830,

0.512800, -0.275770, 0.230220, -0.951190, 0.258300, 0.869560, 
-0.479410, -0.334920, 0.490380, 0.600830,

0.329180, 0.065260, -0.842110, -0.520690, -0.628350, 0.020810, 
-0.699880, 0.274380, 0.122110, -0.538070,



-0.012580, -1.263650, 0.334030, 0.082670, 0.470990, 0.527920, 
0.600470, 0.684890, -0.713350, -0.006180,

0.196610, 0.468620, -0.920010, 0.406730, -0.324530, 0.171520, 
-1.226110, 0.408680, 0.272560, -0.954170,

-1.242050, -0.181060, 0.225270, -1.149460, -0.542950, 0.208010, 
0.068130, -0.483870, -0.528190, 0.668930,



-0.875010, -0.612190, 0.960070, 0.438360, -0.812000, 0.767030, 
-0.523700, -0.902710, -0.597930, -0.505090,

-0.705340, 0.421680, -0.545150, -0.108080, 0.084770, 0.712450, 
0.409090, -0.210020, 0.410350, 0.743930,

0.663670, -0.822500, -0.250390, -0.161490, -0.762680, 0.162670, 
-0.511070, -0.861560, 0.192220, -0.454200,



0.264370, 0.938080, -0.402740, -0.774120, -0.921760, -0.023560, 
0.895570, -0.279990, 0.059810, -0.858340,

0.785610, -0.585190, 0.366730, 0.419270, -0.734530, -0.258180, 
-0.175780, -0.332910, 0.124290, -0.318060,

-0.739130, -0.587680, -0.763300, -0.235940, 0.419920, -0.814080, 
-0.287960, 0.141480, -0.743130, 0.085930,



-0.923890, 0.477960, 0.358360, -0.696470, -0.078660, -0.060420, 
-0.633010, 0.322470, -0.575020, 0.858530,

0.929320, 0.768860, -0.978850, -0.722980, 0.775520, -0.185400, 
-0.598660, -0.168590, 0.519250, -0.405180,

-0.005210, -0.821130, 0.759010, -0.768780, 0.501020, 0.825440, 
0.399750, -0.754320, -0.898500, -0.327540,



-0.307170, 0.266460, 0.280250, 0.231970, -0.439920, 0.053010, 
-0.673400, 0.012480, 0.528190, 0.477100,

0.417580, 0.978820, 0.352650, -1.265450, -0.557970, 0.207930, 
-0.005700, -0.712580, 0.318700, -0.962770,

-0.280430, 0.352120, -0.447800, -0.282080, -0.560220, 0.334210, 
-1.192510, -0.545880, -0.271890, 0.541860,



0.072910, -0.147810, 0.680030, -0.778920, 0.099690, 0.278800, 
-0.947880, 0.819500, -0.180880, -0.365830,

0.797620, 0.137640, -0.798640, 0.315970, 0.686420, -0.094650, 
-0.186690, 0.741070, -0.654510, -0.946460,

-0.588380, 0.356870, -1.080820, -0.485060, 0.578090, -0.232470, 
-1.204990, 0.444040, -0.445860, -0.876220,



0.090750, 0.804140, 0.024450, 0.005620, -1.080330, 0.077880, 
0.754800, -0.607900, -1.060830, 0.881440,

-0.837820, -0.557660, -0.499260, -0.168630, -0.808870, 0.812180, 
0.076110, -0.189350, 0.466850, 0.751430,

-0.687890, -0.839240, -0.333600, 0.643060, -0.135270, -0.917630, 
-0.633290, 0.836650, -0.514340, -0.802770,



0.692270, -0.307140, 0.014250, -0.835290, 0.066930, 0.206660, 
-0.085330, -0.923040, -1.074600, -0.231430,

-0.079550, -0.980200, 0.704470, -0.511970, 0.052240, 0.091970, 
0.335860, 0.741290, -0.816870, 0.137500,

0.533580, 0.365620, 0.233080, 0.452700, -1.094360, -0.121650, 
0.715840, -0.640260, -0.521870, -0.381650,



-0.559050, -0.561500, -0.877950, -1.077530, 0.013770, 0.424850, 
0.329570, 0.221230, -1.214580, 0.123480,

-0.316460, 0.432560, 0.486680, -0.759650, 0.333180, 0.305460, 
-0.924400, 0.812430, 0.669090, 0.002710,

-0.234400, -0.756590, 0.389500, 0.457030, 0.352990, 0.116560, 
0.069140, -0.009360, 0.293900, -0.430080,



0.630820, -0.429750, 0.703210, -0.899210, 0.253060, -0.887160, 
-0.108540, -0.661260, 0.377570, -0.987150,

0.904630, -0.977610, 0.286860, -0.458230, 0.195190, -0.737670, 
-0.476560, -0.340280, -0.714280, -0.285020,

0.034830, 0.216420, -0.717650, -0.338550, 0.734650, -0.985600, 
0.771600, -0.711180, 0.656510, -0.318580,



-0.963560, -0.928950, -0.994510, 0.324990, 0.026090, 0.072790, 
0.706530, -0.783560, 0.308330, 0.245220,

-0.926390, 0.040620, -0.348310, -0.497600, -0.810050, -0.692680, 
-0.515180, -0.164650, -0.986140, 0.489730,

0.941400, -0.453290, 0.551010, 0.550460, -0.732110, -0.634750, 
-0.267560, -0.434490, 0.532400, -0.873960,



0.281380, -0.887980, -0.146600, -0.397150, 0.857160, 0.846050, 
0.039360, -0.580440, -0.615620, 0.298860,

-0.591640, 0.042410, -0.979850, -0.715120, -0.231180, -0.788020, 
0.302120, 0.064970, 0.806120, -0.540410,

-1.020350, 0.610760, 0.463190, -0.134490, 0.475870, -0.992760, 
-0.886660, 0.544840, -0.699850, -0.176780,



0.085160, -0.092660, -0.311130, -0.423970, -0.270240, -0.945300, 
0.493190, -0.188180, -0.549680, 0.599840,

-0.569800, 0.772180, 0.176600, -0.872660, 0.228180, -0.513880, 
0.500270, -0.791650, -0.044950, 0.337150,

-0.531070, 0.148760, -0.160840, -0.944980, -0.040010, -0.550150, 
-0.410180, 0.476400, 0.196960, -0.736530,



0.486280, -0.883150, 0.354370, -0.860560, -1.096280, 0.222540, 
-0.457410, 0.245700, -1.238190, -0.523370,

0.272960, 0.436970, -0.305220, -0.242380, -0.065970, 0.431250, 
-0.118510, -0.242150, 0.251090, 0.607390,

-0.191450, 0.688300, -0.304030, 0.162790, -0.270390, -0.638840, 
0.310070, -1.094680, 0.777460, -0.000460,



-0.986510, -1.090580, -0.758900, -1.214130, -1.058140, -0.549480, 
0.605840, 0.688510, 0.279640, 0.410800,

-0.273750, 0.246370, 0.717550, -1.267440, -0.518770, -0.274810, 
-0.338750, 0.499780, -0.854560, 0.064780,

-0.352040, 0.115140, 0.480530, 0.570040, 0.620100, 0.517180, 
-0.937850, -0.710060, -0.545250, -0.645820,



-0.006970, -0.251050, -0.644830, 0.239650, 0.058050, -0.917010, 
0.165040, -0.045170, 0.204850, -0.662060,

-0.334540, 0.760330, -0.603030, -0.049740, 0.822970, -0.188590, 
0.445090, -0.893090, 0.711960, -0.903480,

-1.105450, 0.640760, 0.226780, -1.044500, -0.404510, 0.552650, 
-1.034060, 0.211840, -0.200670, -0.837150,



0.888890, 0.874810, -0.587420, -0.254730, 0.009760, 0.619950, 
0.390250, -0.298950, -0.184690, -0.762270,

0.361950, -0.980220, -0.253800, -0.540610, -0.553930, -0.581090, 
0.090910, -0.092830, -0.909820, -0.022930,

-0.593260, -0.469060, -0.179940, 0.135470, 0.124700, -0.682390, 
-0.729370, -0.080390, -0.279080, -0.477180,



0.502750, -0.373900, 0.138550, -0.876170, 0.231210, 0.673000, 
0.251110, -0.437320, 0.468340, 0.218970,

-0.828210, -0.835820, 0.043290, -1.202890, -0.315410, -0.458000, 
0.148050, -0.155160, 0.887470, 0.680810,

-1.127500, -0.586160, -0.373480, -1.252110, 0.182570, 0.762520, 
0.008590, -0.022320, 0.705850, -0.045790,



-0.404950, 0.150400, -0.876930, 0.273400, -0.822910, -0.230180, 
0.752220, -1.033570, -0.258440, 0.190400,

-0.706000, -0.134780, 0.205770, -0.749570, 0.922880, -0.483980, 
0.308910, 0.535340, -1.024610, -0.228360,

-0.429530, 0.218070, -0.381220, -0.640960, 0.085400, -0.626460, 
-0.168070, 0.106730, 0.808660, -0.226930,



0.093580, -1.107810, 0.330890, 0.794020, 0.582880, -0.303590, 
0.113650, -0.825740, 0.084160, 0.295740,

-1.014880, -0.396210, -1.084230, -0.865520, -0.228980, 0.465420, 
-0.263820, -0.355040, 0.686660, 0.572480,

0.490060, -0.773170, -0.116890, -0.859000, 0.444820, -0.405140, 
-1.007840, 0.050270, -0.418560, 0.437310,



0.477320, 0.090630, -0.154200, -0.564790, -0.491280, -0.785960, 
-0.753580, -0.436600, 0.684320, 0.081710,

0.507410, -0.898580, -1.076510, -0.425560, -0.979670, 0.291550, 
0.248180, 0.670520, -0.194210, -0.400660,

-0.550140, 0.270730, 0.870640, 0.523550, -0.292330, -0.822850, 
0.559590, 0.094170, 0.377590, -0.053930,



-0.354610, 0.630930, 0.641240, -1.114530, 0.047030, 0.495520, 
0.753540, 0.905540, 0.436590, -0.991980,

0.702900, -0.197810, -1.030300, -0.424230, 0.800920, 0.636660, 
0.580980, -0.614700, -0.592450, -0.287800,

-0.766320, -0.688130, -0.848410, -0.086850, 0.599380, -1.014140, 
-0.395690, -1.023730, -0.043430, -0.903130,



-0.412760, -0.900800, 0.733120, 0.324460, -0.827050, 0.582450, 
0.314080, 0.803340, -1.206970, 0.144680,

-0.764540, 0.010300, -1.107720, -0.700700, -0.016630, -0.071230, 
0.440260, -0.209570, -0.992840, -0.998470,

0.694040, -0.739290, -0.739590, -1.014310, 0.694930, 0.128380, 
-0.063090, 0.255680, 0.666860, -0.893200,



0.401840, 0.308250, 0.114810, 0.291080, -0.077830, -0.994360, 
-0.289940, 0.197740, -0.384290, 0.062780,

-0.444360, -0.809620, -0.341120, -1.289860, -0.280760, -0.481340, 
0.014710, -0.196210, -0.183610, -0.036940,

-1.251790, 0.325320, 0.208690, 0.426680, 0.554760, -0.299040, 
-0.328820, 0.691110, 0.315590, -0.785260,



0.896230, 0.533950, -0.743370, -0.689190, 0.329190, 0.588160, 
-1.009600, -1.043890, 0.511910, 0.040180,

-0.924950, 0.037260, 0.561270, -1.125660, 0.689060, -0.602510, 
-0.529840, -0.397230, -0.038150, 0.767820,

-0.187490, 0.268790, 0.826190, 0.098060, -0.160870, 0.275510, 
-0.922320, -0.056520, -1.051680, -0.877530,



-0.095310, 0.677980, 0.531370, -0.260020, -0.072740, 0.269730, 
0.546680, 0.522430, -1.173310, -0.539560,

0.276670, -0.367330, -1.109040, 0.266690, 0.037970, -0.154660, 
-0.953460, -0.456240, -0.187880, 0.188700,

-1.160680, 0.791330, 0.525720, -0.126670, 0.433380, -0.575890, 
0.072290, 0.164340, -0.394350, 0.559780,



0.721890, 0.561270, 0.782700, -0.768800, 0.324220, -0.193620, 
-0.368980, 0.553010, -0.127390, -0.458950,

0.617550, -0.690520, -0.864540, -0.249030, 0.010530, 0.051810, 
0.511830, -0.161240, 0.109400, 0.418820,

-0.821470, 0.254570, -0.836350, 0.257440, 0.068430, -0.613480, 
-0.176240, 0.496700, -1.076960, -0.655690,



-0.532360, 0.051080, 0.137960, -0.192090, 0.486730, -0.109160, 
0.462350, 0.543510, 0.176350, -0.283580,

-0.764200, -0.591060, -0.931200, -0.680480, 0.185520, -0.857090, 
-0.966890, -0.728980, -0.926190, 0.857230,

0.348410, -0.870080, -0.836800, 0.099270, -0.036040, 0.637650, 
-0.941110, -0.504760, -0.479410, 0.335970,



0.192870, 0.605540, 0.719520, 0.055150, -0.414960, -1.026440, 
0.431080, -0.637750, -1.365160, -0.240630,

-0.793200, 0.105980, -0.112510, 0.206440, 0.799890, -0.344530, 
-0.736680, 0.639050, 0.313540, -0.253750,

-0.212710, 0.213070, -0.694510, 0.347490, -0.301410, 0.154620, 
0.157280, 0.102940, 0.400560, -1.233480,



-0.771940, -0.249790, -0.849990, -0.676210, -0.543480, 0.625270, 
0.371320, 0.428970, -0.848770, -1.051340,

0.129430, 0.129690, 0.310930, -0.048400, 0.207060, 0.492220, 
-0.505610, 0.778730, -0.485510, -0.467790,

-1.366840, -0.393730, 0.489900, 0.136910, 0.122870, -0.796120, 
0.402080, -0.181870, 0.537430, 0.333320,



-0.472930, -0.484760, -0.101780, -0.310520, -0.760490, 0.574190, 
-0.027610, -0.723130, -0.748530, -0.827250,

-0.855200, 0.812910, 0.480700, -0.541560, 0.761070, 0.655080, 
0.211860, -0.456220, -0.171820, -0.346180,

-0.001760, 0.555380, -0.032540, 0.639460, -0.136150, -0.471470, 
-0.905650, -0.638830, -0.391430, 0.618170,



0.036940, -0.878060, 0.328050, -0.798650, -0.979440, 0.769790, 
0.526140, 0.131570, -0.934300, -0.438980,

-0.191500, 0.504920, 0.164210, -0.033150, -0.197150, -0.179340, 
-0.942870, -0.675910, -0.926000, 0.500370,

-0.320930, -0.345200, 0.500160, -0.639480, 0.473150, -0.531720, 
0.516250, 0.651230, -0.872260, 0.335850,



0.738650, 0.362770, 0.743090, -0.613930, 0.266400, -0.299780, 
0.203410, 0.025950, 0.097550, 0.040750,

-0.290890, 0.177340, -1.208040, -0.770440, -0.915060, 0.293750, 
0.188540, -0.078470, -0.609230, -0.705560,

-0.821430, 0.007790, 0.641220, -0.058660, -0.094180, -0.317900, 
-0.495140, -0.204430, -0.630490, 0.206500,



-0.043350, 0.180380, 0.070970, 0.104780, -0.119810, 0.891550, 
-0.086930, 0.791400, -0.720020, -0.360570,

-0.002680, -0.364060, 0.148270, -1.089210, -0.287060, -0.847440, 
-0.766140, 0.563040, -0.969740, 0.171710,

0.421440, -0.483240, 0.588170, -0.897000, 0.271040, -0.230600, 
0.730120, 0.514540, -0.089870, -0.403830,



-0.270550, 0.362420, 0.805110, -0.799070, -1.028260, -0.099360, 
0.656340, -0.613700, 0.451540, 0.118340,

-0.975040, 0.588890, -0.872740, -0.776140, -0.828360, -0.600850, 
-0.947620, 0.442910, -0.635940, -0.254510,

0.495660, -0.871730, 0.756210, -1.125410, -1.107270, 0.759440, 
0.153720, 0.791570, 0.307060, -0.514030,



0.690350, -0.595050, -0.098260, -0.850290, -0.121910, 0.347000, 
0.145990, 0.780920, -0.437060, -0.190500,

-0.582680, 0.562220, -0.520030, 0.576940, 0.156620, -0.066170, 
-0.373800, -0.252320, 0.214950, -0.549510,

-0.055390, 0.043010, -0.101080, -0.375420, -0.255050, -0.082900, 
0.894520, -0.775660, -0.818680, -0.792000,



0.072030, -0.698190, 0.352600, -0.672710, 0.155690, -0.193780, 
0.708760, -0.845560, 0.147910, -0.969340,

0.673960, -0.546180, 0.196040, -0.553630, 0.269150, 0.491000, 
-0.172640, -0.880380, 0.820330, 0.679140,

-0.057700, -0.158050, -0.190470, -0.922360, -1.013610, -0.684730, 
0.244800, 0.364650, 0.517540, -0.290840,



-0.059800, -0.127990, 0.147370, 0.570180, 0.121800, -0.081830, 
-0.759310, 0.795270, 0.636460, -0.975380,

0.643570, 0.332000, -0.512210, -0.097070, 0.532190, 0.429540, 
-0.954080, -0.820890, -0.545460, -0.607890,

-0.957840, -0.998950, 0.043910, -0.692840, 0.415060, -0.760660, 
-0.100120, 0.573930, -0.928750, 0.921170,



-0.394270, 0.447430, -0.242230, -0.240460, -0.895380, -0.178440, 
-0.714160, 0.829100, 0.141580, -0.591480,

-0.908690, 0.530200, 0.816830, 0.840140, -0.257910, -0.252360, 
-0.300210, 0.638360, -0.840020, 0.586540,

-0.270910, -0.834280, -0.445360, -0.355450, -0.961550, -0.733150, 
-0.873960, -0.305340, -0.353980, 0.288130,



0.708090, -1.067310, 0.215010, -0.126170, -0.197600, -0.425780, 
0.341260, -0.290830, -0.114060, -0.083520,

0.206140, -0.918520, -0.537780, -0.344690, -0.713600, -0.889920, 
0.855380, -0.732100, 0.480600, -0.442270,

0.154210, 0.631570, 0.737070, -0.435580, -0.276970, -0.846320, 
0.676940, -0.974480, 0.531590, 0.550150,



0.601100, 0.112800, 0.552030, -1.261600, -1.052420, 0.352810, 
-1.022940, 0.199060, -0.942800, -0.979010,

-0.591410, -0.568520, 0.240900, -0.466480, -0.094160, 0.250770, 
0.312600, 0.022900, 0.370090, -0.471450,

-0.938950, -0.154640, 0.761320, 0.367270, -1.493220, 0.210470, 
0.389730, 0.707100, 0.337120, 0.114540,



0.183610, -1.142960, -0.354170, -0.252150, -0.575350, 0.777110, 
-0.480750, 0.387180, -0.472360, 0.388520,

-0.274260, 0.070640, -0.390000, -1.169170, -0.680190, 0.494430, 
-0.555580, 0.248170, 0.175890, 0.416060,

0.550510, -0.521850, 0.576830, 0.380640, 0.085670, -0.017890, 
-0.294650, -0.865310, 0.806190, 0.370140,





   };



   /* unit definition section (see also UnitType) */

   static UnitType Units[93] =

   {

     { 0.0, 0.0, 0, NULL , NULL },

     { /* unit 1 (noName) */

       0.0, 0.130340, 0,

        &Sources[0] ,

        &Weights[0] ,

       },

     { /* unit 2 (noName) */

       0.0, -0.702870, 0,

        &Sources[0] ,

        &Weights[0] ,

       },

     { /* unit 3 (noName) */

       0.0, 0.793330, 2,

        &Sources[0] ,

        &Weights[0] ,

       },

     { /* unit 4 (noName) */

       0.0, -0.212250, 2,

        &Sources[2] ,

        &Weights[2] ,

       },

     { /* unit 5 (noName) */

       0.0, -0.185830, 2,

        &Sources[4] ,

        &Weights[4] ,

       },

     { /* unit 6 (noName) */

       0.0, 1.429000, 2,

        &Sources[6] ,

        &Weights[6] ,

       },

     { /* unit 7 (noName) */

       0.0, 0.879090, 2,

        &Sources[8] ,

        &Weights[8] ,

       },

     { /* unit 8 (noName) */

       0.0, -1.048060, 2,

        &Sources[10] ,

        &Weights[10] ,

       },

     { /* unit 9 (noName) */

       0.0, 0.512600, 2,

        &Sources[12] ,

        &Weights[12] ,

       },

     { /* unit 10 (noName) */

       0.0, 0.101750, 2,

        &Sources[14] ,

        &Weights[14] ,

       },

     { /* unit 11 (noName) */

       0.0, 0.823050, 2,

        &Sources[16] ,

        &Weights[16] ,

       },

     { /* unit 12 (noName) */

       0.0, 0.049100, 2,

        &Sources[18] ,

        &Weights[18] ,

       },

     { /* unit 13 (noName) */

       0.0, -0.815510, 2,

        &Sources[20] ,

        &Weights[20] ,

       },

     { /* unit 14 (noName) */

       0.0, -0.488980, 2,

        &Sources[22] ,

        &Weights[22] ,

       },

     { /* unit 15 (noName) */

       0.0, 0.675430, 2,

        &Sources[24] ,

        &Weights[24] ,

       },

     { /* unit 16 (noName) */

       0.0, 0.888900, 2,

        &Sources[26] ,

        &Weights[26] ,

       },

     { /* unit 17 (noName) */

       0.0, -1.195660, 2,

        &Sources[28] ,

        &Weights[28] ,

       },

     { /* unit 18 (noName) */

       0.0, -0.122290, 2,

        &Sources[30] ,

        &Weights[30] ,

       },

     { /* unit 19 (noName) */

       0.0, -0.245560, 2,

        &Sources[32] ,

        &Weights[32] ,

       },

     { /* unit 20 (noName) */

       0.0, 0.156240, 2,

        &Sources[34] ,

        &Weights[34] ,

       },

     { /* unit 21 (noName) */

       0.0, -0.330050, 2,

        &Sources[36] ,

        &Weights[36] ,

       },

     { /* unit 22 (noName) */

       0.0, 0.933900, 2,

        &Sources[38] ,

        &Weights[38] ,

       },

     { /* unit 23 (noName) */

       0.0, 0.229340, 2,

        &Sources[40] ,

        &Weights[40] ,

       },

     { /* unit 24 (noName) */

       0.0, -0.689280, 2,

        &Sources[42] ,

        &Weights[42] ,

       },

     { /* unit 25 (noName) */

       0.0, 0.720440, 2,

        &Sources[44] ,

        &Weights[44] ,

       },

     { /* unit 26 (noName) */

       0.0, 1.211620, 2,

        &Sources[46] ,

        &Weights[46] ,

       },

     { /* unit 27 (noName) */

       0.0, 0.822890, 2,

        &Sources[48] ,

        &Weights[48] ,

       },

     { /* unit 28 (noName) */

       0.0, 0.485940, 2,

        &Sources[50] ,

        &Weights[50] ,

       },

     { /* unit 29 (noName) */

       0.0, -0.720420, 2,

        &Sources[52] ,

        &Weights[52] ,

       },

     { /* unit 30 (noName) */

       0.0, 0.450360, 2,

        &Sources[54] ,

        &Weights[54] ,

       },

     { /* unit 31 (noName) */

       0.0, 0.384800, 2,

        &Sources[56] ,

        &Weights[56] ,

       },

     { /* unit 32 (noName) */

       0.0, -0.332590, 2,

        &Sources[58] ,

        &Weights[58] ,

       },

     { /* unit 33 (noName) */

       0.0, 0.768200, 30,

        &Sources[60] ,

        &Weights[60] ,

       },

     { /* unit 34 (noName) */

       0.0, -0.490440, 30,

        &Sources[90] ,

        &Weights[90] ,

       },

     { /* unit 35 (noName) */

       0.0, -0.255230, 30,

        &Sources[120] ,

        &Weights[120] ,

       },

     { /* unit 36 (noName) */

       0.0, -1.060800, 30,

        &Sources[150] ,

        &Weights[150] ,

       },

     { /* unit 37 (noName) */

       0.0, 0.409760, 30,

        &Sources[180] ,

        &Weights[180] ,

       },

     { /* unit 38 (noName) */

       0.0, -0.600210, 30,

        &Sources[210] ,

        &Weights[210] ,

       },

     { /* unit 39 (noName) */

       0.0, 0.149690, 30,

        &Sources[240] ,

        &Weights[240] ,

       },

     { /* unit 40 (noName) */

       0.0, -0.832160, 30,

        &Sources[270] ,

        &Weights[270] ,

       },

     { /* unit 41 (noName) */

       0.0, -0.933710, 30,

        &Sources[300] ,

        &Weights[300] ,

       },

     { /* unit 42 (noName) */

       0.0, 0.100340, 30,

        &Sources[330] ,

        &Weights[330] ,

       },

     { /* unit 43 (noName) */

       0.0, 0.158520, 30,

        &Sources[360] ,

        &Weights[360] ,

       },

     { /* unit 44 (noName) */

       0.0, -0.204610, 30,

        &Sources[390] ,

        &Weights[390] ,

       },

     { /* unit 45 (noName) */

       0.0, 0.409480, 30,

        &Sources[420] ,

        &Weights[420] ,

       },

     { /* unit 46 (noName) */

       0.0, -0.595040, 30,

        &Sources[450] ,

        &Weights[450] ,

       },

     { /* unit 47 (noName) */

       0.0, 0.570170, 30,

        &Sources[480] ,

        &Weights[480] ,

       },

     { /* unit 48 (noName) */

       0.0, 0.674070, 30,

        &Sources[510] ,

        &Weights[510] ,

       },

     { /* unit 49 (noName) */

       0.0, 0.403270, 30,

        &Sources[540] ,

        &Weights[540] ,

       },

     { /* unit 50 (noName) */

       0.0, 0.618050, 30,

        &Sources[570] ,

        &Weights[570] ,

       },

     { /* unit 51 (noName) */

       0.0, -0.304950, 30,

        &Sources[600] ,

        &Weights[600] ,

       },

     { /* unit 52 (noName) */

       0.0, -0.020710, 30,

        &Sources[630] ,

        &Weights[630] ,

       },

     { /* unit 53 (noName) */

       0.0, 0.287880, 30,

        &Sources[660] ,

        &Weights[660] ,

       },

     { /* unit 54 (noName) */

       0.0, -0.574090, 30,

        &Sources[690] ,

        &Weights[690] ,

       },

     { /* unit 55 (noName) */

       0.0, -1.113220, 30,

        &Sources[720] ,

        &Weights[720] ,

       },

     { /* unit 56 (noName) */

       0.0, 0.177130, 30,

        &Sources[750] ,

        &Weights[750] ,

       },

     { /* unit 57 (noName) */

       0.0, -0.760330, 30,

        &Sources[780] ,

        &Weights[780] ,

       },

     { /* unit 58 (noName) */

       0.0, -0.784650, 30,

        &Sources[810] ,

        &Weights[810] ,

       },

     { /* unit 59 (noName) */

       0.0, -1.262090, 30,

        &Sources[840] ,

        &Weights[840] ,

       },

     { /* unit 60 (noName) */

       0.0, -0.227450, 30,

        &Sources[870] ,

        &Weights[870] ,

       },

     { /* unit 61 (noName) */

       0.0, -0.561510, 30,

        &Sources[900] ,

        &Weights[900] ,

       },

     { /* unit 62 (noName) */

       0.0, 0.098420, 30,

        &Sources[930] ,

        &Weights[930] ,

       },

     { /* unit 63 (noName) */

       0.0, 0.298140, 30,

        &Sources[960] ,

        &Weights[960] ,

       },

     { /* unit 64 (noName) */

       0.0, 0.280480, 30,

        &Sources[990] ,

        &Weights[990] ,

       },

     { /* unit 65 (noName) */

       0.0, 0.513720, 30,

        &Sources[1020] ,

        &Weights[1020] ,

       },

     { /* unit 66 (noName) */

       0.0, 0.372470, 30,

        &Sources[1050] ,

        &Weights[1050] ,

       },

     { /* unit 67 (noName) */

       0.0, 0.312300, 30,

        &Sources[1080] ,

        &Weights[1080] ,

       },

     { /* unit 68 (noName) */

       0.0, -0.952430, 30,

        &Sources[1110] ,

        &Weights[1110] ,

       },

     { /* unit 69 (noName) */

       0.0, 0.243470, 30,

        &Sources[1140] ,

        &Weights[1140] ,

       },

     { /* unit 70 (noName) */

       0.0, -0.924990, 30,

        &Sources[1170] ,

        &Weights[1170] ,

       },

     { /* unit 71 (noName) */

       0.0, -1.021930, 30,

        &Sources[1200] ,

        &Weights[1200] ,

       },

     { /* unit 72 (noName) */

       0.0, 0.573800, 30,

        &Sources[1230] ,

        &Weights[1230] ,

       },

     { /* unit 73 (noName) */

       0.0, 0.667050, 30,

        &Sources[1260] ,

        &Weights[1260] ,

       },

     { /* unit 74 (noName) */

       0.0, -0.788530, 30,

        &Sources[1290] ,

        &Weights[1290] ,

       },

     { /* unit 75 (noName) */

       0.0, -0.688930, 30,

        &Sources[1320] ,

        &Weights[1320] ,

       },

     { /* unit 76 (noName) */

       0.0, -1.093090, 30,

        &Sources[1350] ,

        &Weights[1350] ,

       },

     { /* unit 77 (noName) */

       0.0, -0.940230, 30,

        &Sources[1380] ,

        &Weights[1380] ,

       },

     { /* unit 78 (noName) */

       0.0, 0.112250, 30,

        &Sources[1410] ,

        &Weights[1410] ,

       },

     { /* unit 79 (noName) */

       0.0, -1.146490, 30,

        &Sources[1440] ,

        &Weights[1440] ,

       },

     { /* unit 80 (noName) */

       0.0, -0.011950, 30,

        &Sources[1470] ,

        &Weights[1470] ,

       },

     { /* unit 81 (noName) */

       0.0, 0.271770, 30,

        &Sources[1500] ,

        &Weights[1500] ,

       },

     { /* unit 82 (noName) */

       0.0, 0.155200, 30,

        &Sources[1530] ,

        &Weights[1530] ,

       },

     { /* unit 83 (noName) */

       0.0, -0.439810, 30,

        &Sources[1560] ,

        &Weights[1560] ,

       },

     { /* unit 84 (noName) */

       0.0, -1.207230, 30,

        &Sources[1590] ,

        &Weights[1590] ,

       },

     { /* unit 85 (noName) */

       0.0, 0.457690, 30,

        &Sources[1620] ,

        &Weights[1620] ,

       },

     { /* unit 86 (noName) */

       0.0, -0.230410, 30,

        &Sources[1650] ,

        &Weights[1650] ,

       },

     { /* unit 87 (noName) */

       0.0, -0.329670, 30,

        &Sources[1680] ,

        &Weights[1680] ,

       },

     { /* unit 88 (noName) */

       0.0, -0.486380, 30,

        &Sources[1710] ,

        &Weights[1710] ,

       },

     { /* unit 89 (noName) */

       0.0, -0.025480, 30,

        &Sources[1740] ,

        &Weights[1740] ,

       },

     { /* unit 90 (noName) */

       0.0, -0.610370, 30,

        &Sources[1770] ,

        &Weights[1770] ,

       },

     { /* unit 91 (noName) */

       0.0, 0.364320, 30,

        &Sources[1800] ,

        &Weights[1800] ,

       },

     { /* unit 92 (noName) */

       0.0, -1.439710, 30,

        &Sources[1830] ,

        &Weights[1830] ,

       }



   };







int te(float *in, float *out, int init)

{

   int member, source;

   float sum;

   enum{OK, Error, Not_Valid};

   pUnit unit;





   /* layer definition section (names & member units) */



   static pUnit Input[2] = {Units + 1, Units + 2}; /* members */



   static pUnit Hidden1[30] = {Units + 3, Units + 4, Units + 5, Units 
+ 6, Units + 7, Units + 8, Units + 9, Units + 10, Units + 11, Units + 
12, Units + 13, Units + 14, Units + 15, Units + 16, Units + 17, Units 
+ 18, Units + 19, Units + 20, Units + 21, Units + 22, Units + 23, 
Units + 24, Units + 25, Units + 26, Units + 27, Units + 28, Units + 
29, Units + 30, Units + 31, Units + 32}; /* members */



   static pUnit Output1[60] = {Units + 33, Units + 34, Units + 35, 
Units + 36, Units + 37, Units + 38, Units + 39, Units + 40, Units + 
41, Units + 42, Units + 43, Units + 44, Units + 45, Units + 46, Units 
+ 47, Units + 48, Units + 49, Units + 50, Units + 51, Units + 52, 
Units + 53, Units + 54, Units + 55, Units + 56, Units + 57, Units + 
58, Units + 59, Units + 60, Units + 61, Units + 62, Units + 63, Units 
+ 64, Units + 65, Units + 66, Units + 67, Units + 68, Units + 69, 
Units + 70, Units + 71, Units + 72, Units + 73, Units + 74, Units + 
75, Units + 76, Units + 77, Units + 78, Units + 79, Units + 80, Units 
+ 81, Units + 82, Units + 83, Units + 84, Units + 85, Units + 86, 
Units + 87, Units + 88, Units + 89, Units + 90, Units + 91, Units + 
92}; /* members */



   static int Output[60] = {33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 
43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 
60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 
77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92};



   for(member = 0; member < 2; member++) {

     Input[member]->act = in[member];

   }



   for (member = 0; member < 30; member++) {

     unit = Hidden1[member];

     sum = 0.0;

     for (source = 0; source < unit->NoOfSources; source++) {

       sum += unit->sources[source]->act

              * unit->weights[source];

     }

     unit->act = Act_Logistic(sum, unit->Bias);

   };



   for (member = 0; member < 60; member++) {

     unit = Output1[member];

     sum = 0.0;

     for (source = 0; source < unit->NoOfSources; source++) {

       sum += unit->sources[source]->act

              * unit->weights[source];

     }

     unit->act = Act_Logistic(sum, unit->Bias);

   };



   for(member = 0; member < 60; member++) {

     out[member] = Units[Output[member]].act;

   }



   return(OK);

}

