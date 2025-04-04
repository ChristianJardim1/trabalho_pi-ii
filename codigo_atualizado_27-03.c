#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMINST "meminst.mem" //arquivo memoria de instrucoeses
#define MEMDADOS "memdados.dat" //arquivo memoria de dados
#define INSTRUCAO 16 //instrucao com 16 bits

//STRUCTS e ENUMS
typedef enum {
	Tipo_R=0,
	Tipo_I=1,
	Tipo_J=2,
	Tipo_OUTROS=3
} Tipo_Instrucao;

typedef struct {
	int regis[8];
} Reg;

typedef struct {
	int opcode;
	int rs;
	int rt;
	int rd;
	int funct;
	int imm;
	int addr;
	Tipo_Instrucao tipo;
} Deco;

//NOMES DAS FUNCOES
int contarlinhas(const char *arquivo);
char **alocMemInstr(int n);
void liberaMem(char **pmem);
void carregaMemInst(const char *arquivo, char **pmem, int n);
void printMemory(char **pmem, int n);
void printReg(Reg reg);
int binario_para_inteiro(const char *instrucao, int inicio, int tamanho);
void decodificar_uma_instrucao (const char *instrucao, Deco *dec);
void decodificar_tudo (char **meminst, int tamanho, Deco *memoriainst);
void menu();
void programULA(Deco *inst, Reg reg);

//PROGRAMA PRINCIPAL
int main() {

	menu();

	return 0;
}

//MENU
void menu() {
	Reg reg;
	Deco dec;
	char **meminst = NULL, instrucao[16];
	int op, nlinhas, resul;

	do {
		printf("\n\n *** MENU *** \n");
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
		printf("Sua escolha: ");
		scanf("%d", &op);

		switch (op) {
		case 1:
			//conta nC:mero de linhas do arquivo lido
			nlinhas = contarlinhas(MEMINST);
			//aloca memoria para memoria de instruC'C5es
			meminst = alocMemInstr(nlinhas);
			//carrega memoria com os dados do arquivo lido
			carregaMemInst(MEMINST, meminst, nlinhas);
			break;

		case 2:
			printf("Em desenvolvimento.");
			break;

		case 3:
			//conta nC:mero de linhas do arquivo lido
			nlinhas = contarlinhas(MEMINST);
			//imprime memC3ria de instruC'C5es
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
			Deco *memoriainst = malloc(nlinhas * sizeof(Deco));
			decodificar_tudo(meminst, nlinhas, memoriainst);

			int pc = 0;

			while (pc < nlinhas) {
				printf("\nExecutando instruC'C#o [%d] -> %s\n", pc, meminst[pc]);

				Deco *inst = &memoriainst[pc];

				if(inst->tipo == Tipo_R) {
					printf("Tipo: R | Opcode: %d | RS: %d | RT: %d | RD: %d | Funct: %d\n\n", inst->opcode, inst->rs, inst->rt, inst->rd, inst->funct);
				}
				if(inst->tipo == Tipo_I) {
					printf("Tipo: I | Opcode: %d | RS: %d | RT: %d | IMM: %d\n\n", inst->opcode, inst->rs, inst->rt, inst->imm);
				}
				if(inst->tipo == Tipo_J) {
					printf("Tipo J | Opcode: %d | ADDR: %d\n\n", inst->opcode, inst->addr);
				}
				if(inst->tipo == Tipo_OUTROS) {
					printf("Tipo OUTROS\n\n");
				}
				programULA(inst, reg);
				printf("O resultado do programa e: %d\n", reg.regis[inst->rd]);

				pc++;
			}
			free(memoriainst);
			break;

		case 9:
			printf("Em desenvolvimento.");
			break;

		case 10:
			printf("Em desenvolvimento.");
			break;

		case 11:
			liberaMem(meminst);
			printf("VOCE SAIU!!!");
			break;
		}

	} while(op != 11);
}

void printReg(Reg reg) {
	for(int i=0; i<8; i++) {
		printf("REGISTRADOR [%d] - %d\n", i, reg.regis[i]);
	}
}




//FUNCOES IMPLEMENTADAS
int contarlinhas(const char *arquivo)
{
	char c;
	int contador=1;
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
		printf("Erro de alocaC'C#o!");
		return NULL;
	}
	for (int i = 0; i < n; i++)
	{
		pmem[i] = (char *) malloc(sizeof(char) * INSTRUCAO + 1);
		if (pmem[i] == NULL)
		{
			printf("Erro de alocaC'C#o!");
			return NULL;
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
		pmem[i][strcspn(pmem[i], "\n")] = '\0';
	}

	fclose(arq);
}

void printMemory(char **pmem, int n)
{
	if (pmem == NULL || *pmem == NULL)
	{
		printf("MemC3ria vazia ou nC#o alocada!");
		return;
	}
	for (int i = 0; i < n; i++)
	{
		printf("\n%s", pmem[i]);
	}
}



int binario_para_inteiro(const char *instrucao, int inicio, int tamanho) {

	int valor=0;
	char binario[tamanho];

	strncpy(binario, &instrucao[inicio], tamanho);

	char *resto_string;
	valor = strtol(binario, &resto_string, 2);

	return valor;
}


void decodificar_uma_instrucao (const char *instrucao, Deco *dec) {

	dec->opcode = binario_para_inteiro(instrucao, 0, 4);

	if(dec->opcode == 0) {
		dec->tipo = Tipo_R;
		dec->rs = binario_para_inteiro(instrucao, 4, 3);
		dec->rt = binario_para_inteiro(instrucao, 7, 3);
		dec->rd = binario_para_inteiro(instrucao, 10, 3);
		dec->funct = binario_para_inteiro(instrucao, 13, 3);
	}
	if(dec->opcode == 4 || dec->opcode == 8 || dec->opcode == 11 || dec->opcode == 15) {
		dec->tipo = Tipo_I;
		dec->rs = binario_para_inteiro(instrucao, 4, 3);
		dec->rt = binario_para_inteiro(instrucao, 7, 3);
		dec->imm = binario_para_inteiro(instrucao, 10, 6);
	}

	if(dec->opcode == 2) {
		dec->tipo = Tipo_J;
		dec->addr = binario_para_inteiro(instrucao, 4, 12);
	}

	if(dec->opcode != 0 && dec->opcode != 4 && dec->opcode != 8 && dec->opcode != 11 && dec->opcode != 15 && dec->opcode != 2)
		dec->tipo = Tipo_OUTROS;
}


void decodificar_tudo (char **meminst, int tamanho, Deco *memoriainst) {

	for(int x=0; x<tamanho; x++) {
		decodificar_uma_instrucao (meminst[x], &memoriainst[x]);
	}
}

void programULA(Deco *inst, Reg reg) {
	int resul;
	if(inst->opcode == 0) {
		if(inst->funct == 0) {
			reg.regis[inst->rd] = inst->rs  + inst->rt;
		}
		if(inst->funct == 1) {
			reg.regis[inst->rd] = inst->rs  - inst->rt;
		}
	}
}
