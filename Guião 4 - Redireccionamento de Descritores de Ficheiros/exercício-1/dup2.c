#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

//STDIN_FILENO <=> 0
//STDOUT_FILENO <=> 1
//STDERR_FILENO <=> 2

//Ficheiro de leitura
#define IN "dup2.c"
//Ficheiro de escrita
#define OUT "saida.txt"
//Ficheiro de erros
#define ERR "erros.txt"

main ()
{
	//Descritor dos ficheiros a serem abertos
	int fd_fileIN, fd_fileOUT, fd_fileERR;
	//Descritor do stdin -0-, do stdout -1-, e do stderr -2-
	int fd_stdin, fd_stdout, fd_stderr;
	//Buffer de leitura
	char buf[1];

	//Abrir ficheiro de leitura
	fd_fileIN = open (IN, O_RDONLY);

	//Ficheiro de leitura aberto
	if (fd_fileIN != -1)
	{
		//Abrir ficheiro de escrita
		fd_fileOUT = open (OUT, O_WRONLY | O_CREAT, 0666);
		
		//Ficheiro de escrita aberto
		if (fd_fileOUT != -1)
		{
			//Abrir ficheiro de erros
			fd_fileERR = open (ERR, O_WRONLY | O_APPEND | O_CREAT, 0666);

			//Ficheiro de erros aberto
			if (fd_fileERR != -1)
			{
				//Duplicar ficheiro descritor do ficheiro de leitura para o stdin -0-
				fd_stdin = dup2 (fd_fileIN, STDIN_FILENO); 
		
				//Fechar ficheiro descritor (antigo) de leitura
				close (fd_fileIN);

				//Duplicação -0- bem sucedida
				if (fd_stdin == STDIN_FILENO)
				{
						//Duplicar ficheiro descritor do ficheiro de escrita para o stdout -1-
						fd_stdout = dup2 (fd_fileOUT, STDOUT_FILENO); 

						//Fechar ficheiro descritor (antigo) de escrita
						close (fd_fileOUT);

						//Duplicação -1- bem sucedida
						if (fd_stdout == STDOUT_FILENO)
						{
							//Duplicar ficheiro descritor do ficheiro de erros para o stderr -2-
							fd_stderr = dup2 (fd_fileERR, STDERR_FILENO); 

							//Fechar ficheiro descritor (antigo) do ficheiro de erros
							close (fd_fileERR);

							//Duplicação -2- bem sucedida
							if (fd_stderr == STDERR_FILENO)
							{
								//Enquanto existir ficheiro, lêr 1 caractér
								while ( (read (STDIN_FILENO, buf, 1)) > 0)
									//Escrever no -stdout- 
									write (STDOUT_FILENO, buf, 1);

								//Fechar ficheiro descritor -0-
								close (fd_stdin);
								//Fechar ficheiro descritor -1-
								close (fd_stdout);
								//Fechar ficheiro descritor -2-
								close (fd_stderr);
							}

						}
						//Duplicação -2- mal sucedida
						else
						{
							//Fechar ficheiro descritor -0-
							close (fd_stdin);
							//Fechar ficheiro descritor -1-
							close (fd_stdout);
						}
				}
				//Duplicação -1- mal sucedida
				else
				{
					//Fechar ficheiro descritor -0-
					close (fd_stdin);
					//Fechar ficheiro  de erros
					close (fd_fileERR)
				}
			}
			//Ficheiro de erros não foi aberto
			else
			{
				//Fechar ficheiro de leitura
				close (fd_fileIN);
				//Fechar ficheiro de escrita
				close (fd_fileOUT);
			}
		}
		//Ficheiro de escrita não foi aberto
		else
		{
			//Fechar ficheiro de leitura
			close (fd_fileIN);
		}

	}
}
