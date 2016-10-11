/* structure de coordonnée 2D pour affichage*/
typedef struct {
  double x,y;
} Point;

int cpt=0;
char presse,calc;
Point ville;
int anglex,angley,x,y,xold,yold;


void draw_text(float x, float y, const char *fmt, ...);


/* Prototypes */
void affichage();
void clavier(unsigned char touche,int x,int y);
void reshape(int x,int y);
void mouse(int bouton,int etat,int x,int y);
void mousemotion(int x,int y);
void idle();

