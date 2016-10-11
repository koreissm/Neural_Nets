#include <stdio.h>
#include <stdlib.h>

#define LIG 5
#define COL 4
#define EPSILON 0.000001
#define NBSIMUL 100000

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

void printTab (int *tab) {
	int i, j;
	for (i = 0; i < LIG; i++) {
		for (j = 0; j < COL; j++) {
			printf ("%d ", tab[i*COL+j]);
		}
		printf("\n");
	}
}

void printTab2 (double *tab) {
	int i, j;
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

int decision (double somme) {
	if (somme < 0.5)
		return 0;
	else 
		return 1;
}

double propagate (int* pixels, double* couts) { //d : sortie désirée
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
	double erreurs[NBSIMUL];
	double erreurs2[NBSIMUL];
	double propagation;
	int i = 0;

	pixels = readFile("input");
	couts = initTab ();

	//printTab (pixels); //Affichage de la matrice
	couts = initTab();
	//printTab2 (couts);
	int d, y;

	//Apprentisssage de A
	int appris = 0;
	while (appris < NBSIMUL) { //Tant que le réseau de neurone n'a pas appris (sortie neurone != sortie désirée)
		//Apprentisssage de C
		pixels = readFile("input");
		d = 1;
		propagation = propagate (pixels, couts);
		//printf ("Propagation : %f\n", propagation);
		y = decision (propagation);
		//printf ("Y : %d\n", y);
		couts = majPoids (couts, pixels, d, y);
		//printTab2 (couts);

		erreurs[i] = d - propagation;

		//Apprentisssage de C
		pixels = readFile("inputC");
		d = 0;
		propagation = propagate (pixels, couts);
		//printf ("Propagation : %f\n", propagation);
		y = decision (propagation);
		//printf ("Y : %d\n", y);
		
		couts = majPoids (couts, pixels, d, y);
		//printTab2 (couts);
		appris += 1;

		erreurs2[i] = d - propagation;

		i++;
	}

	//Test
	pixels = readFile("input"); //A
	double propA = propagate (pixels, couts);
	int yA = decision (propA);
	pixels = readFile("inputC"); //C
	double propC = propagate (pixels, couts);
	int yC = decision (propC);

	//printf("PropA : %f, yA : %d\nPropC : %f, yC : %d\n", propA, yA, propC, yC);

	for (i = 0; i < NBSIMUL; i++) {
		printf("%d %f %f\n", i, erreurs[i], erreurs2[i]);
	}

	return 0;
}
