#define SIMULNB 10000000
#define NBSAMPLES 20
#define NBNEURON 20
#define NBELEM 2
#define EPSILON 0.1
#define DVP 1
#define DVN -2
#define LIG 5
#define COL 4
#define VECTOR_VISIBILITY 50
#define NB_NEIGHBOUR 4
#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821480865132823066470938446095505822317253594081284811174502841027019385211055596446229489549303819644288109756659334461284756482337867831652712019091456485669234603486104543266482133936072602491412737245870066063155881748815209209628292540917153643678925903600113305305488204665213841469519415116094330572703657595919530921861173819326117931051185480744623799627495673518857527248912279381830119491

typedef struct {
	double x, y;
} VectEntree;

typedef struct {
	double activite;
	double potentiel;
	double w1;
	double w2;
} Neuron;

Neuron neuronSet[NBNEURON]; //Création de 20 neurones
VectEntree dataSet[NBSAMPLES];
double potentiel[NBNEURON];
int winner;

//double dog[NB_NEIGHBOUR] = {1, 0.2, 0.1, -0.01, 0.0, 0.0, 0.0, 0.0};
//double dog[NB_NEIGHBOUR] = {1.0, 0.1, 0.02, -0.005, 0.005};
double dog[NB_NEIGHBOUR] = {1.0, 0.4, -0.005, 0};
//double dog[NB_NEIGHBOUR] = {1.0, 0.4, 0.1, -0.005, 0.005};

int setTab[NBSAMPLES] = {0};

double estimation = 0;

void initialiseSet () ;

void initPoids () ;

VectEntree sortData () ;

void calculePotentiel (VectEntree vect) ;

void calculeActivite () ;

int calculeGagnant () ;

void majPoids (VectEntree vect, int k, int winner) ;

int cmp(const void *x, const void *y);

void trace (char *filename, double estimation, int iteration);

double estimateLearning(Neuron *neuronSet, VectEntree * v);

double distance(Neuron neuron, VectEntree city);