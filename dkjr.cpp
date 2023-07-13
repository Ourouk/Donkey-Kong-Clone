#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <SDL/SDL.h>
#include "./presentation/presentation.h"


//Define the different object of the game
#define VIDE        		0
#define DKJR       		1
#define CROCO       		2
#define CORBEAU     		3
#define CLE 			4

//Define different type of events
#define AUCUN_EVENEMENT    	0

#define LIBRE_BAS		1
#define LIANE_BAS		2
#define DOUBLE_LIANE_BAS	3
#define LIBRE_HAUT		4
#define LIANE_HAUT		5

#define DROITE  	 	1
#define GAUCHE   		2

void* FctThreadEvenements(void *);
void* FctThreadCle(void *);
void* FctThreadDK(void *);
void* FctThreadDKJr(void *);
void* FctThreadScore(void *);
void* FctThreadEnnemis(void *);
void* FctThreadCorbeau(void *);
void* FctThreadCroco(void *);

void initGrilleJeu();
void setGrilleJeu(int l, int c, int type = VIDE, pthread_t tid = 0);
void afficherGrilleJeu();

void HandlerSIGUSR1(int);
void HandlerSIGUSR2(int);
void HandlerSIGALRM(int);
void HandlerSIGINT(int);
void HandlerSIGQUIT(int);
void HandlerSIGCHLD(int);
void HandlerSIGHUP(int);

void DestructeurVS(void *p);

pthread_t threadCle;
pthread_t threadDK;
pthread_t threadDKJr;
pthread_t threadEvenements;
pthread_t threadScore;
pthread_t threadEnnemis;

pthread_cond_t condDK;
pthread_cond_t condScore;

pthread_mutex_t mutexGrilleJeu;
pthread_mutex_t mutexDK;
pthread_mutex_t mutexEvenement;
pthread_mutex_t mutexScore;

pthread_key_t keySpec;

bool MAJDK = false;
int  score = 0;
bool MAJScore = true;
int  delaiEnnemis = 4000;
int  positionDKJr = 1;
int  evenement = AUCUN_EVENEMENT;
int etatDKJr;

typedef struct
{
  int type;
  pthread_t tid;
} S_CASE;

S_CASE grilleJeu[4][8];

typedef struct
{
  bool haut;
  int position;
} S_CROCO;

// ------------------------------------------------------------------------

int main(int argc, char* argv[])
{
	int evt;

	ouvrirFenetreGraphique();

	afficherCage(2);
	afficherCage(3);
	afficherCage(4);

	afficherRireDK();

	afficherCle(3);

	afficherCroco(11, 2);
	afficherCroco(17, 1);
	afficherCroco(0, 3);
	afficherCroco(12, 5);
	afficherCroco(18, 4);

	afficherDKJr(11, 9, 1);
	afficherDKJr(6, 19, 7);
	afficherDKJr(0, 0, 9);
	
	afficherCorbeau(10, 2);
	afficherCorbeau(16, 1);
	
	effacerCarres(9, 10, 2, 1);

	afficherEchec(1);
	afficherScore(1999);

	// Initialise mutex
	int res = NULL;
	res = pthread_mutex_init(&mutexGrilleJeu, NULL);
	if(res != 0)
	{
		printf("Erreur lors de l'initialisation du mutexGrilleJeu\n");
		exit(1);
	}
	res = pthread_mutex_init(&mutexDK, NULL);
	if(res != 0)
	{
		printf("Erreur lors de l'initialisation du mutexDK\n");
		exit(1);
	}
	res = pthread_mutex_init(&mutexEvenement, NULL);
	if(res != 0)
	{
		printf("Erreur lors de l'initialisation du mutexEvenement\n");
		exit(1);
	}
	res = pthread_mutex_init(&mutexScore, NULL);
	if(res != 0)
	{
		printf("Erreur lors de l'initialisation du mutexScore\n");
		exit(1);
	}

	pthread_create(&threadCle,NULL,FctThreadCle,NULL);

	while (1)
	{
	    evt = lireEvenement();
		pthread_create(&threadEvenements, NULL, FctThreadEvenements, NULL);
		pthread_create(&threadCle, NULL, FctThreadCle, NULL);

	    switch (evt)
	    {
		case SDL_QUIT:
			exit(0);
		case SDLK_UP:
			printf("KEY_UP\n");
			break;
		case SDLK_DOWN:
			printf("KEY_DOWN\n");
			break;
		case SDLK_LEFT:
			printf("KEY_LEFT\n");
			break;
		case SDLK_RIGHT:
			printf("KEY_RIGHT\n");
	    }
	}
}

// -------------------------------------

void initGrilleJeu()
{
  int i, j;   
  
  pthread_mutex_lock(&mutexGrilleJeu);

  for(i = 0; i < 4; i++)
    for(j = 0; j < 7; j++)
      setGrilleJeu(i, j);

  pthread_mutex_unlock(&mutexGrilleJeu);
}

// -------------------------------------

void setGrilleJeu(int l, int c, int type, pthread_t tid)
{
  grilleJeu[l][c].type = type;
  grilleJeu[l][c].tid = tid;
}

// -------------------------------------

void afficherGrilleJeu()
{   
   for(int j = 0; j < 4; j++)
   {
       for(int k = 0; k < 8; k++)
          printf("%d  ", grilleJeu[j][k].type);
       printf("\n");
   }

   printf("\n");   
}


void* FctThreadEvenements(void *){
	pthread_mutex_lock(&mutexEvenement);
	switch(evenement)

	pthread_mutex_unlock(&mutexEvenement);

}
void* FctThreadCle(void *){
	char posKey = 0;
	while(1)
	{
		pthread_mutex_lock(&mutexGrilleJeu); //lock the rest of the program during this call
			if(posKey == 0) //if the key is at the end of the line
			{
				grilleJeu[0][1].type = CLE; //add the key
			}else
			{
				grilleJeu[0][1].type = VIDE; //remove the key

			}
			posKey = (posKey+1)%4; //must be lower than 4
			afficherCle(posKey+1); //permit to change the sprite of the key
			nanosleep((const struct timespec[]){{0, 700000000L}}, NULL); //wait 0.7s
			effacerCarres(3,12,2,3); //remove the sprite of the key
		pthread_mutex_unlock(&mutexGrilleJeu);
	}
}
void* FctThreadDK(void *){

}
void* FctThreadDKJr(void *){

}
void* FctThreadScore(void *){

}
void* FctThreadEnnemis(void *){

}
void* FctThreadCorbeau(void *){

}
void* FctThreadCroco(void *){

}