/****************************************************************
 * An array with length. For storing a grid of frequencies,
 * or freq. grid.
 ***************************************************************/
typedef struct _Grid {
double *grid;
int length;
} Grid;

/****************************************************************
 * Construct an empty grid.
 ***************************************************************/

Grid* MakeGrid(int length);


/****************************************************************
 * Delete grid, and free its memory.
 ***************************************************************/

void DeleteGrid(Grid* grid);


/****************************************************************
 * Give a resonance map, and grid, and snap resonance freq's
 * to the grid points. grid is simply a list of frequencies. Return
 * a new resonance map.
 ***************************************************************/

Grid* Lock2Grid(
    Grid* res,
    Grid* grid);

/****************************************************************
 * Given a resonance map, add random values to frequencies. Provide
 * percentage of freq to alter.
 ***************************************************************/

Grid* JerkRes(
    Grid* grid1,
    double  percent);


/****************************************************************
 * Give a list of frequencies, considered to be pitches. Return 
 * a grid, a list of frequencies made from the pitch freq's and
 * N harmonics.
 ***************************************************************/

Grid* MakeHarmGrid(
    Grid* pitches,
    int num_harmonics);


/****************************************************************
 * Make lydian chromatic scale, given lydian tonic and tonal order.
 ***************************************************************/

Grid* MakeLydian(
    double tonic_freq,
    int order);


/****************************************************************
 * Find Lydian Tonic of a group of pitches 
 * according to Russell's Lydian Chromatic 
 * Concept. REturn lydian tonic, (freq) and its tonal order. Based
 * on combination of pitcnes, and not order. (all 12 pitches present
 * gives useless result.)
 *
 * ARguments:
 *    pitches    Array of frequencys
 *
 * Exit Arguments:
 *    tonic     Lydian tonic of array of pitches.
 *    order     Tonal Order of Parent Lydian Chromatic Scale of array.
 *
 ***************************************************************/

void LydianTonic(
    Grid *pitches, 
    double *lydian_tonic,
    int *tonal_order);


/****************************************************************
 * Given a lydian tonic, and another pitch, find the tonal
 * order, based on Russell's Lydian Chromatic Concept.
 ***************************************************************/

int GetTonalOrder(
    double lt, 
    double p);



/****************************************************************
 * Give a frequency, and snap it to the closest point in grid.
 * Return the snapped frequency.
 * 
 ***************************************************************/

double GridFreq(double freq, Grid *grid);
