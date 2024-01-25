#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Função que analisa uma linha e: (1) monta uma matriz -heap- de argumentos, e (2) informa da quantidade de argumentos.
char **monta_argumentos (char buf[], size_t bytes, int *n_argumentos);
//Função responsável por libertar o espaço -heap- ocupado pela matriz de argumentos.
void liberta_argumentos (char **argumentos, int n_argumentos);


main (int argc, char *argv[])
{
	//Identificador do processo filho
	pid_t filho;
	//Informação acerca da espera
	pid_t who;
	int status;

	//Buffer de suporte ao stdin
	size_t bytes_r;
	char buffer[64]; //aguenta 63 caractéres + '\n' do stdin

	//Variáveis de suporte à interpretação de comandos
	int n_argumentos;
	char **argumentos;


	//Lêr linha
	while ( (bytes_r = read (STDIN_FILENO, buffer, 63)) > 0 )
	{
		//Interpretar comandos
		argumentos = monta_argumentos (buffer, bytes_r, &n_argumentos);

		//Criar processo
		filho = fork ();

		//Lançar processo
		/* Processo filho */
		if ( filho == 0 )
		{
			//Variável que controla o tratamento do erro
			int erro;

			//Lançar um novo processo -independente-
			erro = execvp (argumentos[0], argumentos);
			
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
		{
			//stdout: informar condições de término
			printf("PID-%d fechou com o código %d\n", who, WEXITSTATUS(status));

			//Libertar memória -heap-
			liberta_argumentos (argumentos, n_argumentos);
		}
	}
}

//Função auxiliar à montagem da matriz de argumentos proveninetes de uma linha
int conta_argumentos (char buf[], size_t bytes)
{
	//Variável de suporte à percepção do parsing de uma palavra
	int in_palavra;
	//Variável de suporte à contagem do número de palavras
	int total;
	
	//Estar fora da palavra
	in_palavra = 0;
	//Total de palavras encontradas
	total = 0;

	//Percorrer a linha
	for (int i=0; i<bytes; i++)
	{
		//Encontrar um caractér de mudança de linha
		if (buf[i]=='\n')
		{
			//Saír do parse de uma palavra
			in_palavra = 0;
		}
		//Encontrar um caractér de espaço
		else if (buf[i]==' ')
		{
			//Saír do parse de uma palavra
			in_palavra = 0;
		}
		//Encontrar um caractér
		else if (in_palavra == 0)
		{
			//Entrar no parse de uma palavra
			in_palavra = 1;
			//Incrementar o contador de palavras
			total++;
		}
	}

	//Devolver o número de palavras encontradas
	return total;
}
//Função que cria na -heap- uma matriz de argumentos
char **monta_argumentos (char buf[], size_t bytes, int *n_argumentos)
{
	//Estrutura de dados que alberga -em heap- a matriz de argumentos
	char **argumentos;
	//Variável que armazena o número total de argumentos
	int n_args;
	//Variável que suporta a cópia dos argumentos
	int k;

	//Contar o número total de argumentos presentes na linha
	n_args = conta_argumentos (buf, bytes);

	//Alocar tantas casas quanto o número total de argumentos presentes na linha -mais uma casa para o valor NULL requerido pelo exec()-
	argumentos = malloc (sizeof (char *) * (n_args+1));


	//Variável que controla o índice onde começa o argumento na linha
	int desde;
	//Variável de suporte à percepção do parsing de uma palavra
	int in_palavra;

	//Estar fora da palavra
	in_palavra = 0;
	//Preparar as cópias das palavras -argumentos-
	k = 0;
	
	//Percorrer linha
	for (int i=0; i<bytes; i++)
	{
		//Estar sobre uma palavra e encontrar um caractér de mudança de linha
		if (in_palavra==1 && buf[i]=='\n')
		{
			//Marcar a casa com a terminação de string
			buf[i] = '\0';
			//Ir para fora de uma palavra
			in_palavra = 0;
			//Copiar a palavra -argumento-
			argumentos[k++] = strdup (buf+desde);
		}
		//Estar sobre uma palavra e encontrar um caractér de espaço
		else if (in_palavra==1 && buf[i]==' ')
		{
			//Marcar a casa com a terminação de string
			buf[i] = '\0';
			//Ir para fora de uma palavra
			in_palavra = 0;
			//Copiar a palavra -argumento-
			argumentos[k++] = strdup (buf+desde);
		}
		//Encontrar o início de uma palavra -argumento-
		else if (in_palavra==0)
		{
			//Marcar posição de onde a palavra começa na linha
			desde = i;
			//Ir para dentro de uma palavra
			in_palavra=1;
		}
	}

	//Marcar o término da lista de argumentos -exigência do exec()-
	argumentos[k] = NULL;


	//Devolver o número total de argumentos presentes na matriz -incluindo NULL-
	(*n_argumentos) = n_args + 1;
	//Devolver o apontador -heap- da matriz que contém os argumentos
	return argumentos;
}

//Função que liberta -heap- a matriz de argumentos
void liberta_argumentos (char **args, int n_args)
{
	//Percorrer os argumentos
	for (int i=0; i<n_args; i++)
		//Libertar a memória alocada -heap- correspondente ao tamanho da palavra
		free (args[i]);
	//Libertar a memória dos apontadores para as palavras -argumentos-
	free (args);
}
