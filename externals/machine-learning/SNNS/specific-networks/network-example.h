/*********************************************************

   tesh

   --------------------------------------------------------

   generated at Tue Apr 30 17:15:52 2002

   by snns2c ( Bernward Kett 1995 )

*********************************************************/



extern int te(float *in, float *out, int init);



static struct {

   int NoOfInput;    /* Number of Input Units  */

   int NoOfOutput;   /* Number of Output Units */

   int(* propFunc)(float *, float*, int);

} teREC = {2,60,te};

