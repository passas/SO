#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

#define MAX 10

main ()
{
	//Identificadores dos processos
	pid_t pai, filho;
	//Contador
	int spawn;
	//Informação acerca da espera
	pid_t who;
	int status;

	spawn = 1;
	//Ciclo de criação
	for (int i=0; i<MAX; i++)
	{
		//Criar processo
		filho = fork ();
	
		/* Processo filho */
		if ( filho == 0 )
		{
			//Identificador do pai
			pai = getppid();
	
			//Identificador do processo
			filho = getpid();
	
			//stdout: quem sou eu, quem é o meu pai
			printf ("Eu sou o PID-%d, proveniente do meu pai-%d\n", filho, pai);
	
			//Terminar processo e devolver o código herdado
			_exit (spawn);
		}

		/* Processo pai */
	
		//Identificador do processo
		pai = getpid();
	
		//stdout: quem sou eu, quem é o meu filho
		printf ("Eu sou o PID-%d, aguardo pelo meu filho-%d\n", pai, filho);

		//Incrementar contador
		spawn++;
	}

	//Ciclo de espera
	for (int i=0; i<MAX; i++)
	{
		//Esperar pelo desfecho de um dos meus filhos
		who = wait (&status);
		if ( WIFEXITED (status) )
			//stdout: pid de quem me devolveu o desfecho, e o código do desfecho
			printf ("PID-%d devolveu código: %d\n", who, WEXITSTATUS(status));
	}
	
}