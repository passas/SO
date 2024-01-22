#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//Nome do ficheiro "pessoas"
#define PESSOAS "pessoas.log"

//Struct "pessoas"
typedef struct pessoa
{
	int idade;
	char nome[100];
} *Pessoa;


//main
int main (int argc, char *argv[])
{
	//Ficheiro descritor de "pessoas"
	int fd_pessoas;
	//Entidade computacional "pessoa"
	Pessoa p;
	//Offset do ficheiro
	off_t indice;

	//Teste e processamento do comando '-cat'
	if (argc == 2 && strcmp (argv[1], "-cat") == 0)
	{
		//Index
		int index;

		//Abrir pessoas
		fd_pessoas = open (PESSOAS, O_RDONLY);
		if ( fd_pessoas == -1 )
			return 303;

		//Alocar buffer
		p = malloc (sizeof (struct pessoa));

		index = 1;
		//Enquanto existir ficheiro
		while ( read (fd_pessoas, p, sizeof (struct pessoa)) > 0 )
		{
			printf ("%d %s %d\n", index, p->nome, p->idade);
			index++;
		}

		//Fechar ficheiro
		close (fd_pessoas);

		//Libertar buffer
		free (p);
	}
	//Teste e processamento da inserção de um registo 
	else if (argc == 4 && strcmp (argv[1], "-i") == 0 )
	{
		//Validar argumento da idade
		if ( atoi (argv[3]) == 0 )
			return 202;

		//Criar Pessoa
		p = (Pessoa) malloc (sizeof (struct pessoa));
		strcpy (p->nome, argv[2]);
		p->idade = atoi (argv[3]);

		//Abrir ficheiro
		fd_pessoas = open (PESSOAS, O_CREAT | O_WRONLY | O_APPEND, 0666);
		if ( fd_pessoas == -1 )
			return 303;

		//Escrever Pessoa
		write (fd_pessoas, p, sizeof (struct pessoa));

		//Calcular índice de inserção
		indice =  ( lseek (fd_pessoas, 0, SEEK_END) ); //número de bytes
		indice /= sizeof (struct pessoa); //número de registos

		//Fechar ficheiro
		close (fd_pessoas);

		//Imprimir índice de inserção
		printf ("registo %ld\n", indice);
	}
	//Teste e processamento da atualização de um registo
	else if (argc == 4 &&  strcmp (argv[1], "-u") == 0 )
	{
		//Validar argumento da idade
		if ( atoi (argv[3]) == 0 )
			return 202;

		//Testar e processar atualização por nome
		if ( atoi (argv[2]) == 0 )
		{
			//Abrir "pessoas"
			fd_pessoas = open (PESSOAS, O_RDWR);
			if ( fd_pessoas == -1 )
				return 303;

			//Alocar buffer
			p = malloc (sizeof (struct pessoa));
			//Procurar registo
			while ( read (fd_pessoas, p, sizeof (struct pessoa)) > 0 )
			{
				if ( strcmp (p->nome, argv[2]) == 0 )
					break;
			}

			//Atualizar registo
			if (p && strcmp(p->nome, argv[2]) == 0)
			{
				//Retroceder
				lseek (fd_pessoas, - sizeof (struct pessoa), SEEK_CUR);
				//Alterar Idade
				p->idade = atoi (argv[3]);
				//Escrever registo
				write (fd_pessoas, p, sizeof (struct pessoa));
			}

			//Fechar "pessoas"
			close (fd_pessoas);

			//Libertar buffer
			free (p);
		}
		//Testar e processar atualização por índice
		else //atoi(argv[2]) != 0
		{
			//Abrir "pessoas"
			fd_pessoas = open (PESSOAS, O_RDWR);
			if ( fd_pessoas == -1 )
				return 303;

			//Alocar buffer
			p = malloc (sizeof (struct pessoa));

			//Posicionar ficheiro descritor
			lseek (fd_pessoas, (atoi(argv[2]) - 1) * sizeof (struct pessoa), SEEK_SET);
			//Ler registo
			read (fd_pessoas, p, sizeof (struct pessoa));
			//Atualizar Idade
			p->idade = atoi (argv[3]);
			//Posicionar ficheiro descritor
			lseek (fd_pessoas, - sizeof (struct pessoa), SEEK_CUR);
			//Escrever registo
			write (fd_pessoas, p, sizeof (struct pessoa));

			//Fechar ficheiro
			close (fd_pessoas);

			//Desalocar buffer
			free (p);
		}
	}
	else
	{
		return 101;
	}

	return 0;
}