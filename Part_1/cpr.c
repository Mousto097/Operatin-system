/*------------------------------------------------------------
Fichier: cpr.c

Author: Mamadou Bah

Description: Ce programme contient le code pour la creation
	d'un processus enfant et y attacher un tuyau.
	L'enfant envoyera des messages par le tuyau qui seront 
	ensuite envoyes a la sortie standard.
-------------------------------------------------------------*/
#include <stdio.h>
#include <sys/select.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

/* Prototype */
void creerEnfantEtLire(int);

/*-------------------------------------------------------------
Function: main
Arguments: 
	int ac	- nombre d'arguments de la commande
	char **av - tableau de pointeurs aux arguments de commande
Description:
	Extrait le nombre de processus a creer de la ligne de
	commande. Si une erreur a lieu, le processus termine.
	Appel creerEnfantEtLire pour creer un enfant, et lire
	les donnees de l'enfant.
-------------------------------------------------------------*/

int main(int ac, char **av)
{
	int numeroProcessus;

	if (ac == 2)
	{
		if (sscanf(av[1], "%d", &numeroProcessus) == 1)
		{
			creerEnfantEtLire(numeroProcessus);
		}
		else
			fprintf(stderr, "Ne peut pas traduire argument\n");
	}
	else
		fprintf(stderr, "Arguments pas valide\n");
	return (0);
}

/*-------------------------------------------------------------
Function: creerEnfantEtLire
Arguments: 
	int prcNum - le numero de processus
Description:
	Cree l'enfant, en y passant prcNum-1. Utilise prcNum
	comme identificateur de ce processus. Aussi, lit les
	messages du bout de lecture du tuyau et l'envoie a 
	la sortie standard (df 1). Lorsqu'aucune donnee peut
	etre lue du tuyau, termine.
-------------------------------------------------------------*/

void creerEnfantEtLire(int prcNum)
{
	printf("Processus %d commence \n", prcNum);
	fflush(stdout);

	if (prcNum > 1)
	{

		//Creation du tuyeau pour l'attacher a la sortie standard de l'enfant
		int tuyau[2];

		//Erreur lors de la creation du tuyau
		if (pipe(tuyau) == -1)
		{
			fprintf(stderr, "Pipe error");
			exit(1);
		}

		char arg[10];
		sprintf(arg, "%d", prcNum - 1); //Changing prcNum int to string type
		pid_t currpid;

		//creation du processus enfant
		pid_t pid = fork();

		//Verifie s'il ya une erreur lors de la création d'un enfant
		if (pid < 0)
		{
			fprintf(stderr, "Erreur Fork");
			exit(1);
		}

		//pid = 0, lorsqu'on execute un processus enfant
		else if (pid == 0)
		{

			//Fermeture du bout de lecture du tuyau
			close(tuyau[0]);
			//Puis on copie la sortie standard au bout d'écriture du tuyau
			dup2(tuyau[1], 1);
			//Remplace l'image de l'enfant par le pogramme "cpr.c" et décremente le paramètre "prcNum-1"
			execlp("./cpr", "cpr", arg, (char *)NULL);
		}
		//pid > 0, lorsqu'on execute un processus parent
		else if (pid > 0)
		{
			//Fermeture du bout d'ecriture du tuyau
			close(tuyau[1]);
			char buf[1];
			//Boucle while, qui lit du bout d'ecriture du tuyau et ecrit au bout d'ecriture du tuyau
			while (read(tuyau[0], buf, 1) > 0)
			{
				write(1, buf, 1);
			}
			//Fermeture du bout de lecture du tuyau
			close(tuyau[0]);
			//Attendre que tous les processus enfant finissent
			wait(NULL);
		}
	}
	else
	{
		sleep(10);
	}

	printf("Processus %d termine \n", prcNum);
	fflush(stdout);
}
