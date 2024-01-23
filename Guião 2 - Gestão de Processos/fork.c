#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

main ()
{
	//Identificadores dos processos
	pid_t pai, filho;
	//Informação acerca da espera
	pid_t who;
	int status;

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

		//Terminar processo e devolver um código [0,255] de término
		_exit (256);
	}

	/* Processo pai */

	//Identificador do processo
	pai = getpid();

	//stdout: quem sou eu, quem é o meu filho
	printf ("Eu sou o PID-%d, aguardo pelo meu filho-%d\n", pai, filho);

	//Esperar pelo desfecho do meu filho
	who = wait (&status);
	if ( WIFEXITED (status) )
		//stdout: pid de quem me devolveu o desfecho, e o código do desfecho
		printf ("PID-%d devolveu código: %d\n", who, WEXITSTATUS(status));

	return (0);
}
