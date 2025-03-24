#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMINST "meminst.mem" //arquivo memoria de instruções
#define MEMDADOS "memdados.dat" //arquivo memoria de dados
#define INSTRUCAO 16 //instrução com 16 bits
#define DADO 0

//Funções
int contarlinhas(const char *arquivo);
char **alocMemInstr(int n, int tamanho);
void liberaMem(char **pmemn, int n);
void carregaMem(const char *arquivo, char **pmem, int n);
void printMemory(char **pmem, int n);

struct decodificacao
{
    char opcode[4];
    char rs[3];
    char rt[3];
    char rd[3];
    char funct[3];
    char imm[6];
    char addr[7];
};


int main()
{
    struct decodificacao dec;
    char **meminst = NULL, **memdados = NULL, instrucao[16];
    int op, nlinhas;

    do {
	printf("\n *** MENU *** \n");
	printf("1 - Carregar memoria de instrucoes\n");
	printf("2 - Carregar memoria de dados\n");
	printf("3 - Imprimir memorias\n");
	printf("4 - Imprimir banco de registradores\n");
	printf("5 - Imprimir todo o simulador\n");
	printf("6 - Salvar .asm\n");
	printf("7 - Salvar .dat\n");
	printf("8 - Executar programa\n");
	printf("9 - Executar instrucao\n");
	printf("10 - Volta uma instrucao\n");
	printf("11 - Sair\n\n");
    printf("Escolher opção: ");
	scanf("%d", &op);

	do {
		if (op < 1 || op > 11) {
			printf("\nOpção inválida, digite novamente: ");
			scanf("%d", &op);
		}
	} while(op < 1 || op > 11);

	switch (op) 
    {
    case 1:
        //conta número de linhas do arquivo lido
        nlinhas = contarlinhas(MEMINST);
        //aloca memoria para memoria de instruções
        meminst = alocMemInstr(nlinhas, INSTRUCAO);
        //carrega memoria com os dados do arquivo lido
        carregaMem(MEMINST, meminst, nlinhas);
		break;
	
    case 2:
        printf("Em desenvolvimento.");
        //conta número de linhas do arquivo lido
        nlinhas = contarlinhas(MEMDADOS);
        //aloca memoria para memoria de instruções
        memdados = alocMemInstr(nlinhas, DADO);
        //carrega memoria com os dados do arquivo lido
        carregaMem(MEMDADOS, memdados, nlinhas);
		break;
	
    case 3:
        //conta número de linhas do arquivo lido
        nlinhas = contarlinhas(MEMINST);
        printf("\n###### Memória de Instruções #####\n");
        //imprime memória de instruções
        printMemory(meminst, nlinhas);

        //conta número de linhas do arquivo lido
        nlinhas = contarlinhas(MEMDADOS);
        printf("\n###### Memória de Dados #####\n");
        //imprime memória de instruções
        printMemory(memdados, nlinhas);
		break;

	case 4:
        printf("Em desenvolvimento.");
        break;

	case 5:
        printf("Em desenvolvimento.");
        break;

	case 6:
        printf("Em desenvolvimento.");
		break;

	case 7:
        printf("Em desenvolvimento.");
		break;

	case 8:
        printf("Em desenvolvimento.");
		break;

	case 9:
        printf("Em desenvolvimento.");
		break;

	case 10:
        printf("Em desenvolvimento.");
		break;

	case 11:
		
		break;
	}
    } while(op != 11);

    //conta número de linhas do arquivo lido
    nlinhas = contarlinhas(MEMINST);
    //libera memória de instruções
    liberaMem(meminst, nlinhas);

    //conta número de linhas do arquivo lido
    nlinhas = contarlinhas(MEMDADOS);
    //libera memória de instruções
    liberaMem(memdados, nlinhas);

    printf("Memória desalocada!");

    return 0;
}

int contarlinhas(const char *arquivo)
{
    char c;
    int contador=0;
    // abre o arquivo em modo leitura
    FILE *arq = fopen (arquivo, "r");
    if (!arq)
    {
        perror ("Erro ao abrir arquivo");
        exit (1) ;
    }
    // Percorre o arquivo e conta as quebras de linha
    while (fread(&c, sizeof(char), 1, arq) == 1)
    {
        if (c == '\n') {
            contador++;
        }
    }
    fclose(arq);
    return contador;
}

char **alocMemInstr(int n, int tamanho)
{
    char **pmem;
    pmem = (char **) malloc(sizeof(char *) * n);
    if (pmem == NULL)
    {
        printf("Erro de alocação!");
        return 1;
    }
    for (int i = 0; i < n; i++)
    {
        pmem[i] = (char *) malloc(sizeof(char) * tamanho + 1);
        if (pmem[i] == NULL)
        {
            printf("Erro de alocação!");
            return 1;
        }
    }
    return pmem;
}

void liberaMem(char **pmem, int n)
{
    for (int i = 0; i < n; i++)
    {
        free(pmem[i]);
    }
    free(pmem);
}

void carregaMem(const char *arquivo, char **pmem, int n)
{
    // abre o arquivo em modo leitura
    FILE *arq = fopen (arquivo, "r");
    if (!arq)
    {
        perror ("Erro ao abrir arquivo") ;
        exit (1) ;
    }
    for (int i = 0; i < n; i++)
    {
        fgets(pmem[i], sizeof(char) * INSTRUCAO + 2, arq);
    }
    fclose(arq);
}

void printMemory(char **pmem, int n)
{
    if (pmem == NULL || *pmem == NULL)
    {
        printf("Memória vazia ou não alocada!");
        return;
    }
    for (int i = 0; i < n; i++)
    {
        printf("Posição %d ==> %s", i, pmem[i]);
    }
}
