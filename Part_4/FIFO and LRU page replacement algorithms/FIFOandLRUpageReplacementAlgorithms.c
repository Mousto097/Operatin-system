/*
 * Author: Mamadou Bah
 * Purpose: Program that implements the FIFO and LRU page-replacement algorithms.
 * Language:  C
 * Reference: https://web2.clarkson.edu/class/cs444/cs444.sp2010/hw/paging/
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

void remplacerPage(int *, int);
bool estInclus(int *, int);
int trouverIndex(int *, int);
void genererLaChaineDeCaractereAleatoireDeReference(int *);

void algorithmeLRU(int *);
void algorithmeFIFO(int *);

int nbrFauteDePagesPourLRU = 0;
int nbrFauteDePagesPourFIFO = 0;

int nbrCadreDePages = -1;
int nbrDePages = -1;

int main(int ac, char *args[])
{

	/* La fonction srand initialise le générateur de nombres pseudo-aléatoire */
	time_t temps;
	srand((unsigned)time(&temps));

	/* Obliger l'utilisateur à rentrer le nombre de cadre de pages desirées */
	while (nbrCadreDePages < 1 || nbrCadreDePages > 7)
	{
		printf("SVP, entrez un nombre compris entre 1 et 7 pour le nombre de cadre-pages desirées:\n");
		scanf("%d", &nbrCadreDePages);
	}

	/* Obliger l'utilisateur à rentrer le nombre de pageS desiréeS */
	while (nbrDePages < 0)
	{
		printf("SVP, entrez le nombre de page desiré pour commencer la distribution:\n");
		scanf("%d", &nbrDePages);
	}

	/* Création d'un tableau pour contenir la chaine de caractere aleatoire pour le referencement de pages */
	int tableauDeNbrAleatoire[nbrDePages];

	/* Generation de nombre aleatoire compris entre 0 et 9 */
	genererLaChaineDeCaractereAleatoireDeReference(tableauDeNbrAleatoire);

	/* Application de l'algorithme FIFO et LRU sur la sequence aleatoire pour referencement de pages */
	algorithmeFIFO(tableauDeNbrAleatoire);
	algorithmeLRU(tableauDeNbrAleatoire);

	return 0;
}

/*
 * Fonction:  algorithmeLRU 
 * --------------------
 * Cette methode implemente la strategie LRU de remplacement de page
 * 
 * 
 *  *tableauDeNbrAleatoire: est le pointeur vers le tableau qui contient la chaine de caractere 
 * 							aleatoire pour le referencement de pages
 *
 *  returns: Cette methode ne renvoie rien
 */

void algorithmeLRU(int *tableauDeNbrAleatoire)
{
	int taille = 0;
	int framesLRU[nbrCadreDePages];

	for (int i = 0; i < nbrCadreDePages; i++)
	{
		framesLRU[i] = -1;
	}

	for (int i = 0; i < nbrDePages; i++)
	{
		if (taille == nbrCadreDePages || estInclus(framesLRU, tableauDeNbrAleatoire[i]))
		{

			if (estInclus(framesLRU, tableauDeNbrAleatoire[i]))
			{
				//We place the element just accessed at the end of the array
				int index = trouverIndex(framesLRU, tableauDeNbrAleatoire[i]);
				remplacerPage(framesLRU, index);
				taille--;
				framesLRU[taille] = tableauDeNbrAleatoire[i];
				taille++;
			}
			else
			{
				//We remove the least used element (index 0)
				remplacerPage(framesLRU, 0);
				taille--;
				framesLRU[taille] = tableauDeNbrAleatoire[i];
				taille++;
				nbrFauteDePagesPourLRU++;
			}
		}
		else
		{
			nbrFauteDePagesPourLRU++;
			framesLRU[taille] = tableauDeNbrAleatoire[i];
			taille++;
		}
	}
	/* Affichage du nombre de faute de pages pour l'algorithme de LRU */
	printf("\nLe nombre de faute de page pour l'algorithme de LRU est de: %d\n", nbrFauteDePagesPourLRU);
}

/*
 * Fonction:  algorithmeFIFO 
 * --------------------
 * Cette methode implemente la strategie FIFO de remplacement de page
 * 
 * 
 *  *tableauDeNbrAleatoire: est le pointeur vers le tableau qui contient la chaine de caractere 
 * 							aleatoire pour le referencement de pages
 *
 *  returns: Cette methode ne renvoie rien
 */

void algorithmeFIFO(int *tableauDeNbrAleatoire)
{
	int taille = 0;

	int cadreDePagesFIFO[nbrCadreDePages];

	for (int i = 0; i < nbrCadreDePages; i++)
	{
		cadreDePagesFIFO[i] = -1;
	}

	for (int i = 0; i < nbrDePages; i++)
	{
		if (taille == nbrCadreDePages || estInclus(cadreDePagesFIFO, tableauDeNbrAleatoire[i]))
		{

			if (!estInclus(cadreDePagesFIFO, tableauDeNbrAleatoire[i]))
			{
				nbrFauteDePagesPourFIFO++;
				remplacerPage(cadreDePagesFIFO, 0);
				taille--;
				cadreDePagesFIFO[taille] = tableauDeNbrAleatoire[i];
				taille++;
			}
		}
		else
		{
			nbrFauteDePagesPourFIFO++;
			cadreDePagesFIFO[taille] = tableauDeNbrAleatoire[i];
			taille++;
		}
	}
	/* Affichage du nombre de faute de pages pour l'algorithme de FIFO */
	printf("\nLe nombre de faute de page pour l'algorithme de FIFO est de: %d\n", nbrFauteDePagesPourFIFO);
}

/*
 * Function:  remplacerPage 
 * --------------------
 * Cette methode enleve la page specifiée d'un cadre de page
 * 
 *  *tableau: Tableau du cadre de page selectionné
 *  index: La position de la page a enlevé
 *
 *  returns: Cette methode ne renvoie rien.
 */

void remplacerPage(int *tableau, int index)
{
	int i;
	tableau[index] = -1;

	for (i = index; i < sizeof(tableau) - 1; i++)
	{
		tableau[i] = tableau[i + 1];
	}
}

/*
 * Function:  estInclus 
 * --------------------
 * Cette methode chercher l'element specifié dans un tableau donnée
 * 
 *  *tableau: Tableau dans lequelle on doit effectuer la recherche
 *  element: L'element a rechercher
 *
 *  returns: Cette methode renvoie vrai (TRUE) si l'element specifié se trouve
 * 				dans le tableau, Sinon elle renvoie faux (FALSE)
 */
bool estInclus(int *tableau, int element)
{

	for (int i = 0; i < sizeof(tableau); i++)
	{
		if (tableau[i] == element)
		{
			return true;
		}
	}

	return false;
}

/*
 * Function:  trouverIndex 
 * --------------------
 * Cette methode trouve la position d'un element specifié dans un tableau donnée
 * 
 *  *tableau: Tableau dans lequelle on doit effectuer la recherche
 *  element: L'element pour lequelle on cherche la position
 *
 *  returns: Cette methode renvoie la position l'element specifié se trouvant
 * 				dans le tableau.
 */
int trouverIndex(int *tableau, int element)
{
	int index = -1;

	for (int i = 0; i < sizeof(tableau); i++)
	{
		if (tableau[i] == element)
		{
			index = i;
			break;
		}
	}

	return index;
}

/*
 * Function:  genererLaChaineDeCaractereAleatoireDeReference 
 * --------------------
 * Cette methode genere une sequence aleatoire de nombre compris entre 0 et 9 
 * 	puis remplis le tableau donnée
 * 
 *  *tableau: Tableau dans lequelle on met la sequence aleatoire generer
 *
 *  returns: Cette methode ne renvoie rien.
 */
void genererLaChaineDeCaractereAleatoireDeReference(int *tableau)
{

	printf("\nLa sequence de nombre aléatoire génèrée est: ");
	for (int i = 0; i < nbrDePages; i++)
	{
		int nombreAleatoire = rand() % 10;
		tableau[i] = nombreAleatoire;
		printf("%d,", nombreAleatoire);
	}
}