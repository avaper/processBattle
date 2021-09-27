//este archivo es el fichero fuente que al compilarse produce el ejecutable HIJO

// Librería común
#include "common.h"

// Para ftime()
#include <sys/timeb.h>

char estado[3];

void defensa(int sig) 
{
	strcpy(estado, "OK");
	printf ("\tEl hijo %d ha repelido un ataque\n", getpid());
	fflush(stdout);
}

void indefenso(int sig) 
{
	if (strcmp(estado,"KO")!=0 ) 
	{
		strcpy(estado, "KO");
		printf ("\tEl hijo %d ha sido emboscado mientras realizaba un ataque\n", getpid());
		fflush(stdout);
	}
}

int main (int argc, char * argv[]) 
{
	// Validación de argumentos
	if (argc!=7) return -1;
	
	// Clave de los ipc
	clave = atoi(argv[1]);

	// Cola de mensajes
	mensajes = atoi(argv[2]);

	// Región de memoria compartida
	lista = atoi(argv[3]);
	array = shmat(lista, 0, 0);
	
	// Semáforo 
	sem = atoi(argv[4]);

	// Tubería sin nombre
	barrera[0] = atoi(argv[5]);
	barrera[1] = atoi(argv[6]);


	// Ronda de ataques
	
	
	mensaje.tipo=2;
	
	strcpy(estado, "OK");

	while(1) 
	{
		char caracter;
		read(barrera[0], &caracter, 1);
     
        struct timeb hora;
        ftime(&hora);
		srand(hora.millitm);
		int decision = rand()%2;
		
		if (decision == 0) 
		{
			if (signal(SIGUSR1, defensa) == SIG_ERR) return -1;
			
			usleep(200000);
		} 
		else 
		{
			if (signal(SIGUSR1, indefenso) == SIG_ERR) return -1;
			
			usleep(100000);
			
			struct timeb hora2;
        	ftime(&hora2);
			srand(hora2.millitm);
		
			int victima = rand() % atoi(argv[0]);
			
			wait_sem(sem);
			
				while ((array[victima] == getpid()) || (array[victima] == 0)) 
				{
					victima = rand() % atoi(argv[0]);			
				}
			
				kill(array[victima], SIGUSR1);
				
				printf ("\tAtacando al proceso %d\n", array[victima]);
				fflush(stdout);
		
			signal_sem(sem);
			
			usleep(100000);
		}
		
		mensaje.pid = getpid();
		
		strcpy(mensaje.cadena, estado);

		if(msgsnd(mensajes, &mensaje, longitud, 0) == -1) return -1;	
	}
	
	return 0;
}

