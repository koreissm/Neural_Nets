#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdarg.h>
#include <GLUT/glut.h>
#include "kohonen.h" //My prototypes
#include "base_opengl.h" //OpenGL features

void initialiseSet () {
	int i = 0;

	for (i = 0; i < NBSAMPLES; i++) {
		dataSet[i].x = rand()%200;
		dataSet[i].y = rand()%200;
		//printf("%f %f\n", dataSet[i].x, dataSet[i].y);
		//printf("xix : %f, xiy : %f\n", neuronSet[i].vect.x, neuronSet[i].vect.y);
	}
	//printf("\n");
}

VectEntree sortData () {
	int i;

	while (1) {
		int n = rand()%NBSAMPLES;
		//printf("n : %d\n", n);
		if (setTab[n] == 0) {
			setTab[n] = 1;
			return dataSet[n];
		}
	}
}

void reinitData () {
	int i;
	for (i = 0; i < NBSAMPLES; i++) {
		setTab[i] = 0;
	}
}

void initPoids () {
	int i;
	for (i = 0; i < NBNEURON; i++) {
		neuronSet[i].w1 = rand()%200;
		neuronSet[i].w2 = rand()%200;
		neuronSet[i].activite = 0.0;
		neuronSet[i].potentiel = 0.0;
	}
}

void calculePotentiel (VectEntree vect) {
	int i;

	for (i = 0; i < NBNEURON; i++) {
		neuronSet[i].potentiel = sqrt(pow(neuronSet[i].w1 - vect.x, 2) + pow(neuronSet[i].w2 - vect.y, 2));
		//potentiel[i] = sqrt(pow(neuronSet[i].w1 - vect.x, 2) + pow(neuronSet[i].w2 - vect.y, 2));
		//printf("%f\n", potentiel[i]);
	}
}

void calculeActivite () {
	int i;

	for (i = 0; i < NBNEURON; i++) {
		neuronSet[i].activite = fabs(1 / (1 + neuronSet[i].potentiel));
		//printf("%f\n", potentiel[i]);
	}
}

int calculeGagnant () {
	double activiteGagnant = 201;
	int i, indice;

	for (i = 0; i < NBNEURON; i++) {
		if (neuronSet[i].potentiel < activiteGagnant) {
			activiteGagnant = neuronSet[i].potentiel;
			indice = i;
		}
	}
	//printf("%d\n", indice);
	return indice;
}

void majPoids (VectEntree vect, int k, int winner) {
	int i;
	
	double phi;

	//qsort(neuronSet, sizeof(neuronSet)/sizeof(neuronSet[0]), sizeof(neuronSet[0]), cmp);
	int neighboor_min = winner - NB_NEIGHBOUR;
	int neighboor_max = winner + NB_NEIGHBOUR;

	// neuronSet[winner].w1 += EPSILON * (vect.x - neuronSet[winner].w1);
	// neuronSet[winner].w2 += EPSILON * (vect.y - neuronSet[winner].w2);

	if (neighboor_min < 0) neighboor_min = 0;
	if (neighboor_max > NBNEURON) neighboor_max = NBNEURON;

	for (i = neighboor_min; i < neighboor_max; i++) {
		phi = dog[abs(winner - i)]; //Fonction de voisinage
		//printf("%f ", phi);
		neuronSet[i].w1 += EPSILON * phi * (vect.x - neuronSet[i].w1);
		neuronSet[i].w2 += EPSILON * phi * (vect.y - neuronSet[i].w2);
		//if (k == 0 || k == SIMULNB-1)
			//printf("%f %f\n", neuronSet[i].w1, neuronSet[i].w2);
	}
	//printf("\n");
	//if (k == 0 || k == SIMULNB-1)
	//	printf("\n");
}

/* affiche la chaine fmt a partir des coordonnées x,y*/
void draw_text(float x, float y, const char *fmt, ...)
{
	char		buf[1024];			//Holds Our String
	char		*text = buf;
	va_list		ap;				// Pointer To List Of Arguments

	if (fmt == NULL)				// If There's No Text
		return;					// Do Nothing

	va_start(ap, fmt);				// Parses The String For Variables
	    vsprintf(text, fmt, ap);			// And Converts Symbols To Actual Numbers
	va_end(ap);					// Results Are Stored In Text

	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glRasterPos2f(x, y);
	while (*text)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *text++);

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

/* *************************************************** */
/* main */
int main(int argc,char **argv) {

 
/* initilisation de glut et creation de la fenetre */
glutInit(&argc,argv);
glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
glutInitWindowPosition(200,200);
glutInitWindowSize(530,530);
glutCreateWindow("Kohonen");

/* initialisation OpenGL */
glClearColor(0,0,0,0);
glColor3f(1,1,1);
glPointSize(4);
glEnable(GL_DEPTH_TEST);

/* enregistrement des fonctions de rappel */
glutDisplayFunc(affichage);
glutKeyboardFunc(clavier);
glutReshapeFunc(reshape);
glutIdleFunc(idle);
glutMouseFunc(mouse);
glutMotionFunc(mousemotion);

srand(0);
initialiseSet();
initPoids ();

/* Entree dans la boucle principale */
glutMainLoop();
return 0;
}


/* *************************************************** */

void drawCircle(float x, float y, float radius) {
	glBegin(GL_LINE_LOOP);
	int i;
	for (i = 1; i < 360; i++) {
		float degInRad = i*PI/180;
		glVertex2f(cos(degInRad)*radius+x,sin(degInRad)*radius+y);
	}
	glEnd();
}

/* fonction d'affichage appelée a chaque refresh*/

void affichage() {
	int i,j;
	//printf("affichage\n");
	/* effacement de l'image avec la couleur de fond */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glRotatef(-angley,1,0,0); 
	glRotatef(-anglex,0,1,0);

	glColor3f(1.0,1.0,1.0);
	draw_text(-0.9,0.9,"Iteration : %d", cpt);
 	draw_text(-0.9,0.85,"Poids : (%.2f, %.2f) (%.2f, %.2f) (%.2f, %.2f) (%.2f, %.2f) (%.2f, %.2f) (%.2f, %.2f) (%.2f, %.2f) (%.2f, %.2f) (%.2f, %.2f) (%.2f, %.2f) (%.2f, %.2f) (%.2f, %.2f) (%.2f, %.2f) (%.2f, %.2f) (%.2f, %.2f) (%.2f, %.2f) (%.2f, %.2f) (%.2f, %.2f) (%.2f, %.2f) (%.2f, %.2f)", neuronSet[0].w1, neuronSet[0].w2,
 						neuronSet[1].w1, neuronSet[1].w2,neuronSet[2].w1, neuronSet[2].w2,neuronSet[3].w1, neuronSet[3].w2,neuronSet[4].w1, neuronSet[4].w2,
 						neuronSet[5].w1, neuronSet[5].w2,neuronSet[6].w1, neuronSet[6].w2,neuronSet[7].w1, neuronSet[7].w2,neuronSet[8].w1, neuronSet[8].w2,
 						neuronSet[9].w1, neuronSet[9].w2,neuronSet[10].w1, neuronSet[10].w2,neuronSet[11].w1, neuronSet[11].w2,neuronSet[12].w1, neuronSet[12].w2,
 						neuronSet[13].w1, neuronSet[13].w2,neuronSet[14].w1, neuronSet[14].w2,neuronSet[15].w1, neuronSet[15].w2,neuronSet[16].w1, neuronSet[16].w2,
 						neuronSet[17].w1, neuronSet[17].w2,neuronSet[18].w1, neuronSet[18].w2,neuronSet[19].w1, neuronSet[19].w2);
 	draw_text(-0.9,0.8,"Gagnant : %d", winner);
 	draw_text(-0.9,0.75,"DOG : %f %f %f %f %f", dog[0], dog[1], dog[2], dog[3], dog[4]);
 	glColor3f(1.0,0.0,0.0);
 	draw_text(-0.9,0.70,"Distance moyenne : %.2f", estimation);


	glBegin(GL_POINTS);
	glColor3f(1.0,0.0,0.0);
 	glColor3f(0.0,0.0,1.0);
	for (i = 0; i < NBSAMPLES; i++) {
		glVertex2f(dataSet[i].x/200.0-0.5, dataSet[i].y/200.0-0.5);
	}
	glEnd();


	glBegin(GL_LINE_LOOP); 
	glColor3f(1.0,1.0,0.0);
   	for (i = 0; i < NBNEURON; i++) {
		glVertex2f(neuronSet[i].w1/200.0-0.5, neuronSet[i].w2/200.0-0.5);
	}
	glEnd();

	glBegin(GL_POINTS); 
	glColor3f(1.0,1.0,0.0);
   	for (i = 0; i < NBNEURON; i++) {
		glVertex2f(neuronSet[i].w1/200.0-0.5, neuronSet[i].w2/200.0-0.5);
	}
	glEnd();

	// WINNER
	glBegin(GL_LINE_LOOP); 
  	glColor3f(1.0,0.0,0.0);
	drawCircle(neuronSet[winner].w1/200.0-0.5, neuronSet[winner].w2/200.0-0.5, 0.05);
	glEnd();
	glBegin(GL_LINES);
		glVertex2i(-0.5,-0.5);
		glVertex2i(0.7,0.5);
	glEnd();
	
	// BOTTOM LEGEND
	glColor3f(1.0,1.0,0.0);
 	draw_text(-0.9,-0.9,"weights vector : ");
 	glBegin(GL_LINE_LOOP); 
 	glVertex2f(-0.35, -0.89);
 	glVertex2f(-0.25, -0.89);
 	glEnd();
 	glColor3f(1.0,0.0,0.0);
 	drawCircle(-0.33,-0.84, 0.02);
 	draw_text(-0.69,-0.85,"Gagnant : ");
	glColor3f(0.0,0.0,1.0);
 	drawCircle(-0.33,-0.84, 0.02);
 	draw_text(-0.66,-0.95,"Entrées : ");
 	glBegin(GL_POINTS);
 	glVertex2f(-0.34, -0.95);
 	glEnd();

	glFlush();


	/* On echange les buffers */ 
	glutSwapBuffers();
} 


void idle(){
	int i;
	if(calc){
		cpt++;
		//printf("mettre les calculs ici\n");

		int nb = 0;
	
		while (nb < NBSAMPLES) {
			reinitData();
			VectEntree v = sortData();
			//printf("Iteration : %d\n", nb);
			calculePotentiel (v);
			//calculeActivite ();
			winner = calculeGagnant ();
			majPoids (v, nb, winner);
			nb++;
		}

		estimation = estimateLearning(neuronSet, dataSet);
		trace("./simulResults/weightsConvergence.data", estimation, cpt);

		glutPostRedisplay();
	}
}


void clavier(unsigned char touche,int x,int y) {
  int i;
 switch(touche) {
	case 'p': 
		calc=!calc;
		//if(calc) calc=0; else calc=1;
	break;

	case 'q': /* la touche 'q' permet de quitter le programme */
		exit(0);
 } /* switch */

} /* clavier */




void reshape(int x,int y) {
 if (x<y)
  glViewport(0,(y-x)/2,x,x);
 else
  glViewport((x-y)/2,0,y,y);  
} /* reshape */



/* getion des boutons de  la souris*/
void mouse(int bouton,int etat,int x,int y) {
 /* si on appuie sur la bouton de gauche */
 if (bouton==GLUT_LEFT_BUTTON && etat ==GLUT_DOWN) {
  presse=1; // vrai 
  xold=x;   // sauvegarde de la position de la souris
  yold=y;
 }
 
 /* si on relache la souris */
 if (bouton==GLUT_LEFT_BUTTON && etat ==GLUT_UP) {
  presse=0; // faux
 }
} /* mouse */



/*gestion des mouvements de la souris */
void mousemotion(int x,int y) {
 if (presse) { /* si le bouton gauche est presse */
  /* on mofifie les angles de rotation de l'objet en fonction de la position actuelle de la souris et de la derniere position sauvegard?e */ 
  anglex=anglex+(x-xold);
  angley=angley+(y-yold);
  glutPostRedisplay();
 }

 xold=x; /* sauvegarde des valeurs courante des positions de la souris */
 yold=y;
} /* mousemotion */

int cmp(const void *x, const void *y) {
  Neuron xx = *(Neuron*)x, yy = *(Neuron*)y;
  if (xx.potentiel < yy.potentiel) return -1;
  if (xx.potentiel > yy.potentiel) return  1;
  return 0;
}

double distance(Neuron neuron, VectEntree city) {
	return sqrt(pow(neuron.w1 - city.x, 2) + pow(neuron.w2 - city.y, 2));
}

double estimateLearning(Neuron *neuronSet, VectEntree * v) {
	double estimation = 0.0;
	int * isCityTaken = malloc(NBSAMPLES * sizeof(int));
	int i;
	int j;
	double * minDistance = malloc(NBNEURON * sizeof(double));
	double testedDistance = 999999.9;
	int nearestCityJ = 0;

	for (j = 0; j < NBSAMPLES; j++) {
		isCityTaken[j] = 0;
	}

	for (i = 0; i < NBNEURON; i++) {
		minDistance[i] = 999999.9;
	}

	for (i = 0; i < NBNEURON; i++) {
		for (j = 0; j < NBSAMPLES; j++) {
			if (isCityTaken[j] == 0) {
				testedDistance = distance(neuronSet[i], v[j]);
				if (testedDistance < minDistance[i]) {
					minDistance[i] = testedDistance;
					nearestCityJ = j;
				}
			}
		}
		isCityTaken[nearestCityJ] = 1;
	}

	free(isCityTaken);

	for (i = 0; i < NBNEURON; i++) {
		estimation += minDistance[i];
	}
	estimation = estimation / ((double) NBNEURON);

	return estimation;
}

void trace (char *filename, double estimation, int iteration) {
	//fp = fopen("./simulResults/neurone1.data", "w+");
	FILE *fp = fopen(filename, "a");
	fprintf(fp, "%f %d\n", estimation, iteration);
	fclose(fp);
}