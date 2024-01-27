#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main (int argc, char *argv[])
{	
	//Variável que guarda o número gerado aleatorimente
	int error;

	//Sementar com base nos segundos decorridos desde 1970 
	srandom (time(NULL));
	
	//Gerar um número entre [0, 2]
	error = random () % 3;

	//stdout: informar número gerado aleatorimente
	printf ("%s: %d\n", argv[0], error);

	//Devovler código de erro para o exterior
	return ( error );
}
