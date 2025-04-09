/*	0000/000 => ADD
	0000/010 => SUB
	0000/100 => AND
	0000/101 => OR
	0100	 => AADi
	1011	 => LW
	1111	 => SW
	1000	 => BEQ
	0010	 => J		*/

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    #define MEMINST char meminst[256][17] = {{'\0'}}
    #define MEMDADOS int memdados[256] = {0}
    #define REGISTRADOR int registrador[8] = {0}
        
    //STRUCTS e ENUMS
    typedef enum {
        Tipo_R=0,
        Tipo_I=1,
        Tipo_J=2,
        Tipo_OUTROS=3
    } Tipo_Instrucao;
    
    struct instrucao
    {
        char opcode[5];
        char rs[4];
        char rt[4];
        char rd[4];
        char funct[4];
        char imm[7];
        char addr[8];
    };
    
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
    void menu();
    int contarlinhas(const char *arquivo);
    int binarioParaDecimal(const char *bin);
    int extensaobits(int valor, int bits);
    void carregaMemInst(char mem[256][17]);
    void printMemory(char mem[256][17], struct instrucao *inst, Deco *dec);
    void printReg(int *reg);
    void menu();
    void controle(Deco *inst, int *reg, int *memdado, int *pc);
    void copiarBits(const char *instrucao, char *destino, int inicio, int tamanho);
    void decodificarInstrucao(const char *bin, struct instrucao *inst, Deco *dec);
    void printInstrucao(Deco *dec);
    void printmemory(int *memdado);
    void carregarMemoriaDados(int mem[256]);
    int ULA(int op1, int op2, int opULA);
    void saveDAT(int mem[256]);
    void executaP(char );
    
    //PROGRAMA PRINCIPAL
    int main() 
    {
        menu();
    
        return 0;
    }
    
    //MENU
    void menu() 
    {
        Deco dec;
        MEMINST;
        MEMDADOS;
        REGISTRADOR;
        struct instrucao instrucao;
        int op, nlinhas, resul, pc = 0;
        
        do {
	    menu();
            scanf("%d", &op);
            switch (op) {
            case 1:
                //carrega memoria com os dados do arquivo lido
                carregaMemInst(meminst);
                break;
            case 2:
                //carrega memória de dados "arquivo.dat"
                carregarMemoriaDados(memdados);
                break;
            case 3:
                //imprime memoria de instrucoes
                printMemory(meminst, &instrucao, &dec);
                
                //imprime memoria de dados
                printmemory(memdados);
                break;
    
            case 4:
                //imprime banco de registradores
                printReg(registrador);
                break;
    
            case 5:
                printReg(registrador);
                printMemory(meminst, &instrucao, &dec);
                printmemory(memdados);
                break;
    
            case 6:
                printf("Em desenvolvimento.");
                break;
    
            case 7:
                saveDAT(memdados);
                break;
    
            case 8:
                executaP();
    
            case 9:
                executaI(dec, );
                break;
    
            case 10:
                printf("Em desenvolvimento.");
                break;
    
            case 11:
                printf("VOCE SAIU!!!");
                break;
            }
    
        } while(op != 11);
    }
    
    //FUNCOES IMPLEMENTADAS
    void menu() {
	printf("\n\n *** MENU *** \n");
        printf("1 - Carregar memoria de instrucoes\n");
        printf("2 - Carregar memoria de dados\n");
        printf("3 - Imprimir memorias\n")
        printf("4 - Imprimir banco de registradores\n");
        printf("5 - Imprimir todo o simulador\n");
        printf("6 - Salvar .asm\n");
        printf("7 - Salvar .dat\n");
        printf("8 - Executar programa\n");
        printf("9 - Executar instrucao\n");
        printf("10 - Volta uma instrucao\n");
        printf("11 - Sair\n\n");
        printf("Sua escolha: ");
    }   

    void printReg(int *reg) {
        for(int i=0; i<8; i++) {
            printf("REGISTRADOR [%d] - %d\n", i, reg[i]);
        }
    }
    
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
    
    void carregaMemInst(char mem[256][17])
    {
        char arquivo[20];
        // abre o arquivo em modo leitura
        printf("Nome do arquivo: ");
        scanf("%s", arquivo);
        FILE *arq = fopen (arquivo, "r");
        if (!arq)
        {
            perror ("Erro ao abrir arquivo") ;
            exit (1) ;
        }
        int i = 0;
        char linha[20]; // Buffer para leitura
        while (i < 256 && fgets(linha, sizeof(linha), arq)) {
            // Remover quebras de linha e caracteres extras
            linha[strcspn(linha, "\r\n")] = '\0';
    
            // Ignorar linhas vazias
            if (strlen(linha) == 0) {
                continue;
            }
    
            strncpy(mem[i], linha, 16); // Copia até 16 caracteres
            mem[i][16] = '\0'; // Garante terminação de string
            i++; // Avança corretamente para a próxima posição
        }
        fclose(arq);
    }
    
    void printMemory(char mem[256][17], struct instrucao *inst, Deco *dec)
    {
        printf("\n############## MEMÓRIA DE INSTRUÇÕES ##############\n");
        for (int i = 0; i < 256; i++)
        {
            if (mem[i][0] == '\0')
                continue;
            printf("\nIntrucao: %s\n", mem[i]);
            printf("[%d].  ", i);
            decodificarInstrucao(mem[i], inst, dec);
            printInstrucao(dec);
            printf("\n");
        }
    }
    
    void controle(Deco *dec, int *reg, int *memdado, int *pc)
    {
        if (dec->opcode == 11)
        {
            reg[dec->rt] = dec->imm;
        }
        else if (dec->opcode == 15)
        {
            reg[dec->rt] = ULA(reg[dec->rs], memdado[dec->imm], 0);
        }
        else if (dec->opcode == 11)
        {
            memdado[ULA(dec->rs, dec->imm, 0)] = reg[dec->rt];
        }  
        else if (dec->opcode == 4)
        {
            reg[dec->rt] = ULA(reg[dec->rs], dec->imm, 0);
        }
        else if (dec->opcode == 0 && dec->funct == 0)
        {
            reg[dec->rd] = ULA(reg[dec->rs], reg[dec->rt], 0);
        }
        else if (dec->opcode == 0 && dec->funct == 2)
        {
            reg[dec->rd] = ULA(reg[dec->rs], reg[dec->rt], 2);
        }
        else if (dec->opcode == 0 && dec->funct == 4)
        {
            reg[dec->rd] = ULA(reg[dec->rs], reg[dec->rt], 4);
        }
         else if (dec->opcode == 0 && dec->funct == 5)
        {
            reg[dec->rd] = ULA(reg[dec->rs], reg[dec->rt], 5);
        }
        else if (dec->opcode == 8) {
            if (reg[dec->rs] == reg[dec->rt]) {
                *pc = ULA(*pc, dec->imm, 0);
            }
        }
        else if (dec->opcode == 2){
            *pc = dec->addr;
        }
    }
    
    int ULA(int op1, int op2, int opULA) {
        if(opULA == 0) {
            return op1 + op2;
        }
        else if(opULA == 2) {
            return op1 - op2;
        }
        else if(opULA == 4) {
            return op1 & op2;
        }
        else if(opULA == 5) {
            return op1 | op2;
        }
    } 

    // copia os bits da instrução para cada campo da struct instrucao
    void copiarBits(const char *instrucao, char *destino, int inicio, int tamanho)
    {
        strncpy(destino, instrucao + inicio, tamanho); // Copia os bits desejados
        destino[tamanho] = '\0';  // Adiciona o terminador de string
    }
    
    // decodifica a instrucao e armazena os valores na struct do tipo Deco ja no formato int
    void decodificarInstrucao(const char *bin, struct instrucao *inst, Deco *dec)
    {
      copiarBits(bin, inst->opcode, 0, 4);    // Copia os 4 bits do opcode (4 bits)
      dec->opcode = binarioParaDecimal(inst->opcode);
        copiarBits(bin, inst->rs, 4, 3);        // Copia os 3 bits do rs
      dec->rs = binarioParaDecimal(inst->rs);
        copiarBits(bin, inst->rt, 7, 3);        // Copia os 3 bits do rt
      dec->rt = binarioParaDecimal(inst->rt);
        copiarBits(bin, inst->rd, 10, 3);       // Copia os 3 bits do rd
      dec->rd = binarioParaDecimal(inst->rd);
        copiarBits(bin, inst->funct, 13, 3);    // Copia os 3 bits do funct
      dec->funct = binarioParaDecimal(inst->funct);
        copiarBits(bin, inst->imm, 10, 6);      // Copia os 6 bits do imm
      dec->imm = binarioParaDecimal(inst->imm);
      dec->imm = extensaobits(dec->imm, 6);
        copiarBits(bin, inst->addr, 9, 7);     // Copia os 7 bits do addr
      dec->addr = binarioParaDecimal(inst->addr);
      dec->addr = extensaobits(dec->addr, 7);
    }
    
    // Função para imprimir a instrução
    void printInstrucao(Deco *dec)
    {
        printf("opcode: %d  ", dec->opcode);
        printf("rs: %d  ", dec->rs);
        printf("rt: %d  ", dec->rt);
        printf("rd: %d  ", dec->rd);
        printf("funct: %d  ", dec->funct);
        printf("imm: %d  ", dec->imm);
        printf("addr: %d", dec->addr);
    }
    
    // converte de binario para decimal
    int binarioParaDecimal(const char *bin){
        int valor = (int)strtol(bin, NULL, 2);
        int bits = strlen(bin);
        
        if(bits > 4){
            if (valor & (1 << (bits - 1))) {
                valor = valor - (1 << bits);
            }
        }
        
        return valor;
    }
    
    void printmemory(int *memdado) {
        printf("\n############## MEMÓRIA DE DADOS ##############\n\n");
        for(int i=0; i<256; i++) {
            printf("[%d]. %d   ", i, memdado[i]);
            if (i % 8 == 7)
            {
                printf("\n");
            }
        }       
    }
    
    // faz a extensão de 6 bits para 8 bits
    int extensaobits(int valor, int bits){
       
        int bitsinal = (valor >> (bits - 1)) & 1;
        
        if (bitsinal == 0){
            return valor;
        }
        
        else {
            int bitsfaltando = 8 - bits;
            int adicionabits = (1 << bitsfaltando) - 1;
            adicionabits = adicionabits << bits;
            return valor | adicionabits;
        }
    }

void carregarMemoriaDados(int mem[256])
{
    char arquivo[20];
    printf("Nome do arquivo: ");
    scanf("%s", arquivo);
    FILE *arq = fopen(arquivo, "r");
    if (!arq)
    {
    perror ("Erro ao abrir arquivo") ;
    exit (1) ;
    }
    int i = 0;
    while (fscanf(arq, "%d", &mem[i]) != EOF)
    {
        i++;
    }
}

  void saveDAT(int mem[256]) {

    char arquivo[20];
    printf("Nome para o arquivo de saida: ");
    scanf("%s", arquivo);
    FILE *arq = fopen(arquivo, "w");
    if (!arq)
    {
    perror ("Erro ao abrir arquivo") ;
    exit (1) ;
    }
    for(int i=0; i<256; i++) {
      fprintf(arq,"%d\n",mem[i]);
    }
    fclose(arq);
  }

void executaI(Deco *dec, char *meminst[256][17], int *pc, int *memdados[256]) {
	decodificarInstrucao(meminst[pc], &instrucao, &dec);
        int pc_antes = pc;
        printInstrucao(&dec);
        controle(&dec, registrador, memdados, &pc);
	if(pc == pc_antes){
       		pc++;
	}
}
