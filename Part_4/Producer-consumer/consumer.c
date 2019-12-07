/*
 * Author: Mamadou Bah
 * Purpose: The consumer process read and output the sequence from shared memory.
 * Language:  C
 * Reference: http://www.cse.cuhk.edu.hk/~ericlo/teaching/os/lab/7-IPC2/sync-pro.html
 *            http://www.cse.cuhk.edu.hk/~ericlo/teaching/os/lab/7-IPC2/share-mem.html
 */

#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/ipc.h>

int main(int ac, char *args[])
{

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
    * Lecture et affichage de la séquence se trouvant dans la mémoire
    * partagée, à partir du processus Consumer
    */
    printf("Consumer reading data from shared memory: %s\n", str);

    /* 
    * Détache le segment de mémoire partagée situé à l'adresse spécifiée par str
    * à partir de l'espace d'adressage du processus appelant. 
    */
    shmdt(str);

    /* 
    * Supprime du système lidentificateur de mémoire partagée spécifié par shmid et 
    * détruis le segment de mémoire partagée et la structure de données shmid_ds qui lui est associée. 
    */
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
