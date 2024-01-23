#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CASAS 2 //Número de casas a serem preenchidas com o número a procurar
#define LINHAS 10
#define COLUNAS 1000

main ()
{
	//Matriz
	int i, j;
	int *matriz[LINHAS];

	//Inicialização da matriz
	for (i=0; i<LINHAS; i++)
	{
		matriz[i] = (int *) malloc (sizeof (int) * COLUNAS);

		for (j=0; j<COLUNAS; j++)
			matriz[i][j] = 0;
	}

	//Preenchimento aleatório do número a procurar
	srand ( time (NULL) );
	for (int k=0; k<CASAS; k++)
	{
		i = rand() % LINHAS;
		j = rand() % COLUNAS;
		matriz[i][j] = 1;

		//stdout: informar casa
		printf ("Número colocado na casa [%d][%d]\n", i, j);
	}

	//Estrutura de dados para guardar o filho que vai procurar cada linha
	int filho[LINHAS];
	//Contador de criação de processos filho
	int spawn;
	//Estrutura de dados para guardar o resultado da procura do filho
	int resultado[LINHAS];
	//Informação acerca do resultado dos processos filho
	pid_t who;
	int status;

	//Inicializar resultado da procura e PID de cada linha
	for (i=0; i<LINHAS; i++)
	{
		filho[i] = -1;
		resultado[i] = -1;
	}

	//Ciclo de criação concurrente de processos
	for (spawn=0; spawn<LINHAS; spawn++)
	{
		filho[spawn] = fork();

		/* Processo filho */
		if ( filho[spawn] == 0 )
		{
			//stdout: Informação da procura
			printf ("PID-%d à procura na linha %d\n", getpid(), spawn);

			//Procurar o número na matriz
			for (j=0; j<COLUNAS; j++)
				if (matriz[spawn][j] == 1)
					_exit (0);

			//Não encontrado
			_exit (1);
		}
	}
	/*Processo pai*/

	//Esperar pela terminação -aleatória- dos processos filho
	for (i=0; i<spawn; i++)
	{
		who = wait (&status);
		if ( WIFEXITED(status) )
			//Procura linear pela linha do filho
			for (j=0; j<LINHAS; j++)
				if (filho[j]==who)
					//Atribuir resultado da procura
					resultado[j] = WEXITSTATUS(status);
	}

	//stdout: impressão dos resultados
	for (i=0; i<LINHAS; i++)
		if (resultado[i] == 0)
			printf ("Filho-%d, ecnontrou na linha %d\n", filho[i], i);

	//Libertação da matriz
	for (i=0; i<LINHAS; i++)
		free (matriz[i]);
}
