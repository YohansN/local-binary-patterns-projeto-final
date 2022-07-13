#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <dirent.h>
struct pgm
{
	int tipo;
	int c;
	int r;
	int mv;
	unsigned char *pData; // <- Depois do readPGMImage os dados que utilizaremos estaram salvos nesse ponteiro.
};

void readPGMImage(struct pgm *, char *);
void viewPGMImage(struct pgm *);
void writePGMImage(struct pgm *, char *);
int main(){
    struct pgm img;
	DIR *d;
    struct dirent *dir;
	char base[50];


    d = opendir("./imgs");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            strcpy(base, "./imgs/");
			char *caminho = strcat(base, dir->d_name);
            char primeiro = dir->d_name[0];
            printf("%s", caminho);
            
            if(strstr(caminho, ".pgm")) {
            	readPGMImage(&img, caminho);
				
				
				
            }

						// Leitura da Imagem -PGM

						// Processar - Medir o tempo apenas nesta etapa.
						
						// Saída.
        }
        closedir(d);
    }
	writePGMImage(&img, "nome");
	viewPGMImage(&img);
}

void readPGMImage(struct pgm *pio, char *filename)
{
	FILE *fp;
	char ch;

	if (!(fp = fopen(filename, "r")))
	{
		perror("Erro.");
        printf("CARALHO");
		exit(1);
	}

	if ((ch = getc(fp)) != 'P')
	{
		puts("A imagem fornecida não está no formato pgm");
		exit(2);
	}

	pio->tipo = getc(fp) - 48;

	fseek(fp, 1, SEEK_CUR);

	while ((ch = getc(fp)) == '#')
	{
		while ((ch = getc(fp)) != '\n');
	}

	fseek(fp, -1, SEEK_CUR);

	fscanf(fp, "%d %d", &pio->c, &pio->r);
	if (ferror(fp))
	{
		perror(NULL);
		exit(3);
	}
	fscanf(fp, "%d", &pio->mv);
	fseek(fp, 1, SEEK_CUR);

	pio->pData = (unsigned char *)malloc(pio->r * pio->c * sizeof(unsigned char));

	if (!pio->pData)
	{
		puts("A memoria nao foi alocada corretamente!");
		exit(1);
	}

	switch (pio->tipo)
	{
	case 2:
		puts("Lendo imagem PGM (dados em texto)");
		for (int k = 0; k < (pio->r * pio->c); k++)
		{
			fscanf(fp, "%hhu", pio->pData + k);
		}
		break;
	case 5:
		puts("Lendo imagem PGM (dados em binário)");
		fread(pio->pData, sizeof(unsigned char), pio->r * pio->c, fp);
		break;
	default:
		puts("Não está implementado");
	}

	fclose(fp);
}

void writePGMImage(struct pgm *pio, char *filename)
{
	FILE *fp;
	char ch;

	if (!(fp = fopen(filename, "wb")))
	{
		perror("Erro.");
		exit(1);
	}

	fprintf(fp, "%s\n", "P5");
	fprintf(fp, "%d %d\n", pio->c, pio->r);
	fprintf(fp, "%d\n", 255);

	fwrite(pio->pData, sizeof(unsigned char), pio->c * pio->r, fp);

	fclose(fp);
}

void viewPGMImage(struct pgm *pio)
{
	printf("Tipo: %d\n", pio->tipo);
	printf("Dimensões: [%d %d]\n", pio->c, pio->r);
	printf("Max: %d\n", pio->mv);

	for (int k = 0; k < (pio->r * pio->c); k++)
	{
		if (!(k % pio->c))
			printf("\n");
		printf("%2hhu ", *(pio->pData + k));
	}
	printf("\n");
}