//este archivo es el fichero fuente que al compilarse produce el ejecutable PADRE

// Librería común
#include "common.h"

int main (int argc, char * argv[]) 
{
	// Validación de argumentos
	if (argc!=2) return -1;
	
	// Clave de los ipc
	if ((clave = ftok(argv[0],'X')) == -1) return -1;	
	sprintf(claveStr, "%d", clave);
	
	// Cola de mensajes
	if ((mensajes = msgget(clave, IPC_CREAT | 0666)) < 0) return -1;	
	sprintf(mensajesStr, "%d", mensajes);
	
	// Región de memoria compartida
	if ((lista = shmget(clave, atoi(argv[1])*sizeof(int), IPC_CREAT | 0666)) < 0) return -1;
	array = shmat(lista, 0, 0);	 
	sprintf(listaStr, "%d", lista);
	
	// Semáforo
	if ((sem = semget(clave, 1, IPC_CREAT | 0666)) < 0) return -1;
	sprintf(semStr, "%d", sem);
	
	init_sem(sem, 1);
	
	// Tubería sin nombre
	if ((pipe(barrera)) < 0) return -1;
	sprintf(barLec, "%d", barrera[0]);
	sprintf(barEsc, "%d", barrera[1]);
	
	// Fichero FIFO
	FILE *resultado = NULL;
	resultado = fopen("./Trabajo2/resultado", "w");
	
	// Creación de los procesos hijo
	int pid;
	
	for (int i = 0; i < atoi(argv[1]); i++) 
	{
		if ((pid = fork()) == -1) return -1;
		
		if (pid == 0) 
		{ 
			// Los hijos ejecutan su programa
			if((execl("./Trabajo2/hijo", \
			argv[1], claveStr, mensajesStr, listaStr, semStr, barLec, barEsc, NULL)) == -1) 
			return -1;	
		} 
		else 
		{
			wait_sem(sem);
				
				// El padre añade los pid a la lista 
				array[i] = pid;
			
			signal_sem(sem);
		}
	}
	
	
	// Ronda de ataques


	int restantes = atoi(argv[1]);
		
	while (restantes > 1) 
	{
		printf("\nIniciando ronda de ataques\n\n");
		fflush(stdout);		
	
		char *str;
		char ch = 'X';
		
		// Se genera el mensaje
		for (int i = 0; i < restantes; i++) 
		{
			strncpy(str, &ch, 1);
		}		
	 	
	 	// Se envia el mensaje al mismo tiempo a los hijos
		write(barrera[1], str, restantes);
		
		int derribados = 0;
	 	
		for (int i = 0; i < restantes; i++) 
		{
			if (msgrcv(mensajes, &mensaje, longitud, 2, 0) == -1) return -1;			
		
			if (strcmp(mensaje.cadena, "KO") == 0) 
			{
				if (mensaje.pid != 0) 
				{
					derribados++;
				
					kill(mensaje.pid, SIGTERM);
					waitpid(mensaje.pid, &status, 0);						
				}	
			
				for (int j = 0; j < atoi(argv[1]); j++) 
				{		
					wait_sem(sem);
					
						if (array[j] == mensaje.pid) array[j] = 0;
					
					signal_sem(sem);	
				}
			}	
		}
		restantes -= derribados;	
	}
	

	for (int i = 0; i < atoi(argv[1]); i++) 
	{
		wait_sem(sem);
			
			// Queda un algún proceso vivo
			if (array[i] != 0) 
			{	
				kill(array[i],SIGTERM);
				waitpid(array[i], &status, 0);
		
				// Resultado
				fprintf(resultado, "El hijo %d ha ganado\n\n", array[i]);
	   							
				// Limpieza
		
				msgctl(mensajes, IPC_RMID, 0);
				semctl(sem, IPC_RMID, 0);
			
				// Demostración
			 	system("ipcs -qs");

				return 0;
			}
	
		signal_sem(sem);	
	}

	// Resultado
	fprintf(resultado, "%s\n\n","Empate");

	// Limpieza
	msgctl(mensajes, IPC_RMID, 0);
	semctl(sem, IPC_RMID, 0);

	// Demostración
	system("ipcs -qs");
	
	return 0;
}

