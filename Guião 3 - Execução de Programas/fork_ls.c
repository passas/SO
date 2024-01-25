#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

main ()
{
	//Identificador do processo filho
	pid_t filho;
	//Informação acerca da espera
	pid_t who;
	int status;

	//Criar processo
	filho = fork ();

	/* Processo filho */
	if ( filho == 0 )
	{
		//Controlador do código de erro
		int erro;

		//Executar um novo processo -independente-
		erro = execlp ("ls", "ls", "-l", NULL);

		//Controlar a representação de 8 bits (1 byte) do código de erro
		if (erro == -1)
			erro = 1;

		//Devolver código de erro
		_exit ( erro );
	}

	/* Processo pai */

	//Esperar pelo filho
	who = wait (&status);
	if (WIFEXITED(status))
		printf("PID-%d fechou com o código %d\n", who, WEXITSTATUS(status));
}
