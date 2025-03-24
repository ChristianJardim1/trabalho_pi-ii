#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMINST "meminst.mem" //arquivo memoria de instruções
#define MEMDADOS "memdados.dat" //arquivo memoria de dados
#define INSTRUCAO 16 //instrução com 16 bits

typedef struct {
    int regis[8];
}Reg;

typedef struct {
    char opcode[4];
    char rs[4];
    char rt[3];
    char rd[3];
    char funct[3];
    char imm[6];
    char addr[7];
}Deco;

//Funções
int contarlinhas(const char *arquivo);
char **alocMemInstr(int n);
void liberaMem(char **pmem);
void carregaMemInst(const char *arquivo, char **pmem, int n);
void printMemory(char **pmem, int n);
void printReg(Reg reg);
int executaInstrucao(char **pmem, int pc, Deco dec);
int menu(int op);

int main()
{
    Reg reg;
    Deco dec;
    char **meminst = NULL, instrucao[16];
    int op, nlinhas;

do {
	op = menu(op);

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
        meminst = alocMemInstr(nlinhas);
        //carrega memoria com os dados do arquivo lido
        carregaMemInst(MEMINST, meminst, nlinhas);
		break;
	
    case 2:
        printf("Em desenvolvimento.");
		break;
	
    case 3:
        //conta número de linhas do arquivo lido
        nlinhas = contarlinhas(MEMINST);
        //imprime memória de instruções
        printMemory(meminst, nlinhas);
		break;

	case 4:
        printReg(reg);
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
        int pc = 0;
        pc = executaInstrucao(meminst, pc, dec);
		break;

	case 9:
        printf("Em desenvolvimento.");
		break;

	case 10:
        printf("Em desenvolvimento.");
		break;

	case 11:
		liberaMem(meminst);
		return 0;
		break;
   } while(op != 11);
}while(op != 0);
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

char **alocMemInstr(int n)
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
        pmem[i] = (char *) malloc(sizeof(char) * INSTRUCAO + 1);
        if (pmem[i] == NULL)
        {
            printf("Erro de alocação!");
            return 1;
        }
    }
    return pmem;
}

void liberaMem(char **pmem)
{
    for (int i = 0; i < 16; i++)
    {
        free(pmem[i]);
    }
    free(pmem);
}

void carregaMemInst(const char *arquivo, char **pmem, int n)
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
        printf("%s", pmem[i]);
    }
}

int menu(int op){
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
	return op;
}

void printReg(Reg reg) {
  for(int i=0; i<8; i++){  
		printf("%d\n",reg.regis[i]);
	}
}

int executaInstrucao(char **pmen, int pc, Deco dec) {
		for(int j=0; j<4; j++){
				dec.rs[j] = pmen[pc][j];
    }
    printf("%s",dec.rs);
    pc++;
    return pc;
}