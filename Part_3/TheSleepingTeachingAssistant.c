/*------------------------------------------------------------
Fichier: TheSleepingTeachingAssistant.c

Nom: Mamadou Bah
-------------------------------------------------------------*/

#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>

//Definition des constantes porur le temps de sommeill maximal et le nombre d'étudiants
#define NOMBREDETUDIANTS 5
#define TEMPSSOMMEILMAX 8

//Declaration d'une mutex pour gestion de laccès a des etudiants au TA (le TA est la ressource, et les etudians sont les "consumers")
pthread_mutex_t mutexLock;

//Creation de semaphores pour les étudiants, pour le TA et pour les 3 fauteils d'attente
sem_t semaphore_etudiants;
sem_t semaphore_assistant_enseignement;
sem_t semaphore_trois_fauteuils_dattente[3];

//Threads declaration
pthread_t AssistantEnseignementThread;
pthread_t threadEtudiants[NOMBREDETUDIANTS];

//Declaration de variable pour la gestion de la salle d'attente
int nombreDetudiantsEnAttente = 0;
int siègeActuelle = 0;

//Declaration de la signatures des fonctions (prototypes function)
void creerTousLesThreads(void);
void *EtudiantFaitDeLaProgramation(void *id);
void *TAentrainDaiderUnEtudiant();

int main(int ac, char *args[])
{
	int i;

	//Initialisation des semaphores avec les parametres correspondant
	// sem_open(&semaphore_etudiants,0,0);
	sem_open("semaphore_etudiants", 0, 0);
	sem_open("semaphore_assistant_enseignement", 0, 0);
	sem_open("semaphore_trois_fauteuils_dattente[0]", 0, 0);
	sem_open("semaphore_trois_fauteuils_dattente[1]", 0, 0);
	sem_open("semaphore_trois_fauteuils_dattente[2]", 0, 0);

	//Generation de temps au hasard pour le temps de sommeil des threads
	//Ceci permet de simuler une situation réelle au centre de mentorat
	srand(time(NULL));

	//Appel de fonction pour créer des threads pour les étudians et pour le TA
	creerTousLesThreads();

	pthread_join(AssistantEnseignementThread, NULL);

	for (i = 0; i < NOMBREDETUDIANTS; i++)
	{
		pthread_join(threadEtudiants[i], NULL);
	}

	return 0;
}

void creerTousLesThreads()
{
	int i;
	int *NumeroDesEtudiants;

	//Creation d'un thread pour l'Assistant a l'enseignement
	if (pthread_create(&AssistantEnseignementThread, NULL, TAentrainDaiderUnEtudiant, NULL))
	{
		fprintf(stderr, "Une erreur est survenu lors de la creation du thread de l'assistant a l'enseignement\n");
	}

	//Creation de threads pour les nombres d'etudiants
	NumeroDesEtudiants = (int *)malloc(sizeof(int) * NOMBREDETUDIANTS);

	for (i = 0; i < NOMBREDETUDIANTS; i++)
	{
		NumeroDesEtudiants[i] = i + 1;
		if (pthread_create(&threadEtudiants[i], NULL, EtudiantFaitDeLaProgramation, (void *)&NumeroDesEtudiants[i]))
		{
			fprintf(stderr, "Une erreur est survenu lors de la creation du thread de l'etudiant\n");
		}
	}
}

void *EtudiantFaitDeLaProgramation(void *id)
{
	int numeroEtudiant = *(int *)id;
	int variableTemporaire;

	while (1)
	{
		int TempsDeSommeilAléatoire = rand() % TEMPSSOMMEILMAX + 1;

		printf("L'etudiant avec le numero etudiant: %d va faire de la programmation pour un duree de %d s.\n", numeroEtudiant, TempsDeSommeilAléatoire);

		sleep(TempsDeSommeilAléatoire);

		//On ferme la mutex, pour blocker l'acces a la ressource (le TA)
		pthread_mutex_lock(&mutexLock); //Lock the mutex
		variableTemporaire = nombreDetudiantsEnAttente;
		//On ouvre la mutex, pour liberer l'acces a la ressource (le TA)
		pthread_mutex_unlock(&mutexLock);

		if (variableTemporaire >= 3)
		{ //Dans ce cas, il y a plus de siege disponible

			printf("Il n'y a plus de chaise disponible dans la salle d'attente, l'etudiant numero %d retournera faire de la programation.\n", numeroEtudiant);
		}
		else
		{ //Il y a des sieges disponible dans la salle d'attente

			if (variableTemporaire != 0)
			{
				printf("L'etudiant avec le numero etudiant: %d prends place dans la salle d'attente, Le nombre restant d'etudiants a servir est de: %d\n", numeroEtudiant, nombreDetudiantsEnAttente);
			}
			else
			{
				sem_post(&semaphore_assistant_enseignement);
			}

			//On ferme la mutex, pour blocker l'acces a la ressource (le TA)
			pthread_mutex_lock(&mutexLock);

			int index = (siègeActuelle + nombreDetudiantsEnAttente) % 3;
			nombreDetudiantsEnAttente++;
			//On ouvre la mutex, pour liberer l'acces a la ressource (le TA)
			pthread_mutex_unlock(&mutexLock);

			sem_wait(&semaphore_trois_fauteuils_dattente[index]);
			printf("L'etudiant avec le numero etudiant: %d se fait aider par l'assistant a l'enseignement\n", numeroEtudiant);
			sem_wait(&semaphore_etudiants);
		}
	}
}

void *TAentrainDaiderUnEtudiant()
{

	int val = 0;

	while (1)
	{
		sem_wait(&semaphore_assistant_enseignement);

		while (1)
		{
			//On ferme la mutex, pour blocker l'acces a la ressource (le TA)
			pthread_mutex_lock(&mutexLock);

			if (nombreDetudiantsEnAttente == 0 && val == 0)
			{
				printf("[Il y a 0 etudiant pour le moment donc l'assistant a l'enseignement va se coucher......]\n");
				val++;
				//On ouvre la mutex, pour liberer l'acces a la ressource (le TA)
				pthread_mutex_unlock(&mutexLock);
				break;
			}
			else
			{

				//Si il ya des etudiants dans la salle d'attente, l'assistant a l'enseignement sers le premier etudiant arrivé.
				sem_post(&semaphore_trois_fauteuils_dattente[siègeActuelle]);

				//Alors le nombre d'etudiant dans la salle d'attente se reduit de 1
				nombreDetudiantsEnAttente--;
				printf("L'assistant a l'enseignement est entrain d'aider un etudiant, Le nombre d'etudiant restant dans la salle d'attente est de: %d\n", nombreDetudiantsEnAttente);

				//Les etudiants qui attendent avance chacun de un siège
				siègeActuelle = (siègeActuelle + 1) % 3;

				//On ouvre la mutex, pour liberer l'acces a la ressource (le TA)
				pthread_mutex_unlock(&mutexLock);

				int TempsDeSommeilAléatoire = rand() % TEMPSSOMMEILMAX + 1;

				//On fait dormir les threads pour une periode de temps au hasard
				sleep(TempsDeSommeilAléatoire);

				printf("L'assistant a l'enseignement a finis d'aider l'etudiant\n");
				sem_post(&semaphore_etudiants);
			}
		}
	}
}
