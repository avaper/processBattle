
// Para printf(), fflush()
#include <stdio.h>

// Para atoi(), srand(), system()
#include <stdlib.h>

// Para ftok(), msgget(), semget(), shmget()
#include <sys/types.h>
#include <sys/ipc.h>

// Para msgget()
#include <sys/msg.h>

// Para shmget(), shmat()
#include <sys/shm.h>

// Para semget()
#include <sys/sem.h>

// Para wait()
#include <sys/wait.h>

// Para pipe(), getpid()
#include <unistd.h>

// Para strcpy(), strcmp()
#include <string.h>

int init_sem (int semid, ushort valor) 
{
	ushort sem_array[1];
	sem_array[0] = valor;
	if (semctl(semid, 0, SETALL, sem_array) == -1) return -1;
	     
    return 0; 
}

int wait_sem (int semid) 
{  
	struct sembuf op[1];    
	op[0].sem_num =  0;    
	op[0].sem_op  = -1;
    op[0].sem_flg =  0;
      
    if (semop(semid, op, 1) == -1) return -1;

    return 0; 
} 

int signal_sem (int semid) 
{ 
	struct sembuf op[1];    
	op[0].sem_num =  0;    
	op[0].sem_op  =  1;    
	op[0].sem_flg =  0;    
	
	if (semop(semid, op, 1) == -1) return -1; 

	return 0; 
}

key_t clave = 0;
char  claveStr[32] = {};

int   mensajes = 0;
char  mensajesStr[10] = {};

int   lista = 0;
int   *array = NULL;
char  listaStr[8] = {};

int   sem = 0;
int   status = 0;
char  semStr[10] = {};

int   barrera[2] = {};
char  barLec[10] = {};
char  barEsc[10] = {};

struct 
{
	long 	tipo;
	int 	pid;
	char 	cadena[3];
	
} mensaje;

int longitud = sizeof(mensaje) - sizeof(mensaje.tipo);


