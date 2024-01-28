#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

//STDIN_FILENO <=> 0
//STDOUT_FILENO <=> 1

main ()
{
	//Descritor do ficheiro a ser aberto
	int fd_file;
	//Descritor do stdin -0-
	int fd_stdin;
	//Buffer de leitura
	char buf[1];

	//Abrir ficheiro
	fd_file = open ("dup_stdin.c", O_RDONLY);
	//Ficheiro aberto
	if (fd_file != -1)
	{
		//Duplicar ficheiro descritor para o stdin -0-
		fd_stdin = dup2 (fd_file, STDIN_FILENO); 
		
		//Fechar ficheiro descritor (antigo)
		close (fd_file);
		
		//Duplicação bem sucedida
		if (fd_stdin == STDIN_FILENO)
		{	
			//Enquanto existir ficheiro, lêr 1 caractér
			while ( (read (STDIN_FILENO, buf, 1)) > 0)
				//Escrever no terminal -stdout- 
				write (STDOUT_FILENO, buf, 1); 
			
			//Fechar ficheiro descritor
			close (fd_stdin);
		}
	}
}
