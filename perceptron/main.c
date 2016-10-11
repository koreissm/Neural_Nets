#include <stdio.h>
#include <stdlib.h>

#define LIG 5
#define COL 4
#define EPSILON 0.000001
#define NBSIMUL 100000
#define NBLETTER 26

int setTab[NBLETTER] = {0};
char letters[NBLETTER] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

int* readFile (char* filename) {
	int *tab = malloc(sizeof (int) * LIG * COL);
	int k;
	FILE *file;

   	file = fopen(filename, "r");
	for (k = 0; k < LIG; k++) {
		fscanf(file, "%d %d %d %d\n", &tab[k*COL+0], &tab[k*COL+1], &tab[k*COL+2], &tab[k*COL+3]);
	}

	fclose(file);
	return tab;
}

char sortData () {
	int i;

	while (1) {
		int n = rand()%NBLETTER;
		//printf("n : %d\n", n);
		if (setTab[n] == 0) {
			setTab[n] = 1;
			return letters[n];
		}
	}
}

void reinitData () {
	int i;
	for (i = 0; i < NBLETTER; i++) {
		setTab[i] = 0;
	}
}

void printTab (double *tab, char c) {
	int i, j;
	if (c == 'i') //Si on doit afficher des Int
		tab = (int*)tab;
	for (i = 0; i < LIG; i++) {
		for (j = 0; j < COL; j++) {
			printf ("%f ", tab[i*COL+j]);
		}
		printf("\n");
	}
}

double* initTab () {
	double *tab = malloc(sizeof (double) * LIG * COL);
	int i, j;
	for (i = 0; i < LIG; i++) {
		for (j = 0; j < COL; j++) {
			tab[i*COL+j] = EPSILON;
		}
	}
	return tab;
}

//Update function
double* majPoids (double* couts, int* pixels, int d, int y) {
	int i, j;
	double delta;
	for (i = 0; i < LIG; i++) {
		for (j = 0; j < COL; j++) {
			couts[i*COL+j] = couts[i*COL+j] + EPSILON * (d - y) * pixels[i*COL+j];
		}
	}

	return couts;
}

//Heavyside function
int signe (double somme) {
	if (somme < 0.5)
		return 0;
	else 
		return 1;
}

//Function that calculate the result of the network
double calculSortie (int* pixels, double* couts) { //d : sortie désirée
	int i, j;
	int y;
	double somme = 0.0;

	for (i = 0; i < LIG; i++) {
		for (j = 0; j < COL; j++) {
			somme += pixels[i*COL+j] * couts[i*COL+j];
		}
	}
	
	return somme;
}

int main () {
	
	int *pixels = malloc(sizeof (int) * LIG * COL);
	double *couts = malloc(sizeof (int) * LIG * COL);
	//double erreurs[NBSIMUL];
	//double erreurs2[NBSIMUL];
	double propagation;
	int i = 0;

	//pixels = readFile("./data/A");
	couts = initTab ();

	int d, y;

	//Apprentisssage de A
	int appris = 0;
	while (appris < NBSIMUL) { //Tant que le réseau de neurone n'a pas appris (sortie neurone != sortie désirée)

		for (i = 0; i < 26; i++) {
			//Apprentisssage de C
			char inputLetter = sortData ();

			pixels = readFile("./data/A");
			d = 1;
			y = signe (calculSortie (pixels, couts));
			couts = majPoids (couts, pixels, d, y);

			//erreurs[i] = d - propagation;

			//Apprentisssage de C
			pixels = readFile("inputC");
			d = 0;
			propagation = calculSortie (pixels, couts);
			y = signe (propagation);
			
			couts = majPoids (couts, pixels, d, y);
			appris += 1;
		}
		reinitData ();
		//erreurs2[i] = d - propagation;
	}

	//Test
	pixels = readFile("input"); //A
	double propA = calculSortie (pixels, couts);
	int yA = signe (propA);
	pixels = readFile("inputC"); //C
	double propC = calculSortie (pixels, couts);
	int yC = signe (propC);

	// for (i = 0; i < NBSIMUL; i++) {
	// 	printf("%d %f %f\n", i, erreurs[i], erreurs2[i]);
	// }

	return 0;
}
