#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

int main (int argc, char *argv[])
{
	if (argc != 3)
		return 101;

	//Fonte e destino da cópia
	char *fonte, *destino;
	//Ficheiros descritores da fonta e do destino
	int fd_fonte, fd_destino;
	//Controladores de leituras e escritas
	ssize_t byte_rd, byte_rw;
	//Buffer de leitura
	char b[1];

	//Definir fonte e destino
	fonte = argv[1];
	destino = argv[2];

	//Abrir fonte para leitura
	fd_fonte = open (fonte, O_RDONLY);
	//Testar abertura
	if (fd_fonte == -1)
		return 202;

	//Abrir destino para escrita
	fd_destino = open (destino, O_WRONLY | O_CREAT, 0644);
	//Testar abertura
	if (fd_destino == -1)
		return 202;

	//Iniciar processo de transcrição
	while ( (byte_rd = read (fd_fonte, b, 1)) > 0 )
	{
		byte_rw = write (fd_destino, b, 1);
	}

	//Fechar fonte e destino
	close (fd_fonte); close (fd_destino);

	return 0;
}