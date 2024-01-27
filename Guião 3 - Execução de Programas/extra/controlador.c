#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

main (int argc, char *argv[])
{
	//Informções acerca da espera: pid + código de fecho
	pid_t who;
	int status;
	//Estrutura de dados auxiliar à contagem de execuções dos programas argumento
	int contador[argc];
	//Estrutura de dados auxiliar à associação dos processos que estão a executar os programas argumento
	int filho[argc];
	//Contador de processos (filho) ativos
	int ativos;

	//Iniciar contadores e PID's
	for (int i=1; i<argc; i++) //argv[0] => executável currente
	{
		contador[i] = 0;
		filho[i] = -1; //-1 significa falta de processo
	}

	//Iniciar contador de processos ativos
	ativos = 0;

	//Ciclo de lançamento de processos
	for (int i=1; i<argc; i++)
	{
		//Criar processo
		filho[i] = fork ();

		//Verificar criação do processo
		if (filho[i] != -1)
			//Incrementar número de processos ativos
			ativos++;

		/* Processo filho */
		if ( filho[i] == 0 )
		{
			//Variável de suporte ao controlo de erros
			int erro;

			//Lançar um processo -independente-
			erro = execl (argv[i], argv[i], NULL);

			//Controlar a gama de valores suportada pela diretiva _exit(): [0,255] -1 byte-
			if (erro == -1)
				erro = 1;

			//Lançar código de erro
			_exit ( erro );
		}

	}

	/* Processo pai */
	//Enquanto existirem processos ativos
	while (ativos>0)
	{
		//Esperar pelo término de um processo
		who = wait (&status);
		//Processo terminou com código de insucesso
		if (WIFEXITED(status) && WEXITSTATUS(status)!=0)
		{
			//Decrementar o número de processos ativos
			ativos--;

			//Procurar re-lançar o programa -i-
			for (int i=1; i<argc; i++)
			{
				//Encontrar o programa -i- através de quem lançou o programa -i-
				if (filho[i]==who)
				{
					//Incrementar o número de execuções do programa -i-
					contador[i]++;

					//Desatribuir processo à execução do programa -i-
					filho[i] = -1;

					//Criar processo
					filho[i] = fork ();

					//Verificar a criação do processo
					if (filho[i] != -1)
						//Incrementar número de processos ativos
						ativos++;

					/* Processo filho */
					if ( filho[i] == 0 )
					{
						//Controlador do código de erro
						int erro;
				
						//Executar um novo processo -independente-
						erro = execl (argv[i], argv[i], NULL);
				
						//Controlar a representação de 8 bits (1 byte) do código de erro
						if (erro == -1)
							erro = 1;
				
						//Devolver código de erro
						_exit ( erro );
					}
					/* Processo pai */
					//Relancei o programa -i-, paro a procura
					break;
				}
			}
		}
		//Processo terminou com código de sucesso
		else if (WIFEXITED(status) && WEXITSTATUS(status)==0)
		{
			//Decrementar o número de processos ativos
			ativos --;

			//Procurar o programa -i-
			for (int i=1; i<argc; i++)
			{
				//Encontrar o programa -i- através de quem lançou o programa -i-
				if (filho[i]==who)
				{
					//Incrementar o número de execuções do programa -i-
					contador[i]++;
					//Parar procura
					break;
				}
			}
		}
	}

	//stdout: informar quantas vezes exxecutou cada programa
	for (int i=1; i<argc; i++)
		printf ("%s %d\n", argv[i], contador[i]);

}
