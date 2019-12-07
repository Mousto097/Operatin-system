/*
 * Author: Mamadou Bah
 * Purpose: The producer process will generate the Catalan sequence and write it to a shared memory object.
 * Language:  C
 * Reference: http://www.cse.cuhk.edu.hk/~ericlo/teaching/os/lab/7-IPC2/sync-pro.html
 *            http://www.cse.cuhk.edu.hk/~ericlo/teaching/os/lab/7-IPC2/share-mem.html
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

void generateCatalanSequence(long *);
long generateCatalanNumber(long);
long factorielle(long);
int nbrDeNombresCatalan = -1;

int main(int ac, char *args[])
{

	/* Obliger l'utilisateur à rentrer le nombre de nombre Catalan à generer */
	while (nbrDeNombresCatalan < 1)
	{
		printf("SVP, entrer le nombre de nombres Catalan à génèrer: ");
		scanf("%d", &nbrDeNombresCatalan);
	}

	/* Création d'un tableau pour contenir la chaine de caractere aleatoire pour le referencement de pages */
	long catalanSequenceArray[nbrDeNombresCatalan];
	generateCatalanSequence(catalanSequenceArray);

	/* 
    * Lecture et affichage de la séquence se trouvant dans la mémoire 
	* partagée, à partir du processus Producer
    */
	for (int i = 0; i < nbrDeNombresCatalan; i++)
	{
		printf("%li\n", catalanSequenceArray[i]);
	}

	char strCatalanNumbers[sizeof(long) * nbrDeNombresCatalan];
	int size = 0;
	for (int i = 0; i < nbrDeNombresCatalan; i++)
	{
		size += sprintf(&strCatalanNumbers[size], "%li,", catalanSequenceArray[i]);
	}

	/* 
    * Renvoie une clé basée sur le chemin et l'id. La fonction renvoie la même clé pour 
    * tous les chemins qui pointent vers le même fichier lorsqu'ils sont appelés avec la même valeur id. 
    */
	key_t key = ftok("shmfile", 65);

	/* 
    * Renvoie l'identifiant de la mémoire partagée associé à la clé. 
    */
	int shmid = shmget(key, 1024, 0666 | IPC_CREAT);

	/* 
    * Attache le segment de mémoire partagée identifié par shmid à  
    * l'espace d'adressage du processus appelant.
    */
	char *str = (char *)shmat(shmid, (void *)0, 0);

	/* 
    * Écriture de la séquence se trouvant dans la mémoire 
	* partagée, à partir du processus Producer
    */
	strcpy(str, strCatalanNumbers);

	/* 
    * Lecture et affichage de la séquence se trouvant dans la mémoire 
	* partagée, à partir du processus Producer
    */
	printf("Producer writting data in shared memory: %s\n", str);

	/* 
    * Détache le segment de mémoire partagée situé à l'adresse spécifiée par str
    * à partir de l'espace d'adressage du processus appelant. 
    */
	shmdt(str);

	return 0;
}

/*
 * fonction:  generateCatalanSequence
 * --------------------
 * computes the value of the next Catalan number using:
 *  	Cn = (2n)!/(n+1)!n!
 * 
 *  *tableauOfCatalaNumbers: pointer to the table of 
 *
 *  returns: does not return a value
 */
void generateCatalanSequence(long *tableauOfCatalaNumbers)
{

	for (long i = 0; i < nbrDeNombresCatalan; i++)
	{
		tableauOfCatalaNumbers[i] = generateCatalanNumber(i + 1);
	}
}

/*
 * Fonction:  generateCatalanNumber 
 * --------------------
 * computes the value of the next Catalan number using:
 *  	Cn = (2n)!/(n+1)!n!
 *
 *  nombre: number of terms in the series to sum
 *
 *  returns: the value of next catalan number
 *          
 */
long generateCatalanNumber(long nombre)
{

	/* compute the value of the next Catalan number Cn = (2n)!/(n+1)!n! */
	long result = ((factorielle(2 * nombre)) / (factorielle(nombre) * factorielle(1 + nombre)));

	return result;
}

/*
 * Fonction:  factorielle 
 * --------------------
 * computes an approximation of pi using:
 *    n! = n×(n-1)!
 *
 *  n: number of terms in the series to multiply
 *
 *  returns: the value of factorial obtained by by the product 
 * 				of integer numbers from 1 to n.
 *         
 */
long factorielle(long n)
{

	long factorielle = 1;

	for (long i = 1; i <= n; ++i)
	{
		factorielle = factorielle * i;
	}

	return factorielle;
}