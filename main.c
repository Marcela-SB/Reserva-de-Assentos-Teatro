#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>


char alfabeto[26] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";  

typedef int boolean;
#define true 1
#define false 0

typedef struct{
  boolean reservado;
  int fileira; //linha
  int numero; //coluna
  char nome[100];
} Assento;

typedef struct{
  Assento** Assento;
  int linhas;
  int colunas;
} Teatro;




//######################################################################################

//-------------------------Funções relativas a Teatro e Assento-------------------------
Assento ** inicializarMatriz(Teatro t, int numLinhas, int numColunas){
  //Criando e alocando matriz
  t.Assento = malloc(numLinhas * sizeof(Assento*));
  for(int i = 0; i < numLinhas; i++) {
    t.Assento[i] = malloc(numColunas * sizeof(Assento));
  }

  //Definindo valores da struct da matriz
  for(int x = 0; x < numLinhas; x++){
    for(int y = 0; y < numColunas; y++){
      t.Assento[x][y].reservado = false;
      t.Assento[x][y].fileira = x;
      t.Assento[x][y].numero = y+1;
      t.Assento[x][y].nome[0] = '\0';
    }
  }

  return t.Assento;
}


Teatro criarTeatro(int numLinhas, int numColunas){
  Teatro matriz;
  matriz.linhas = numLinhas;
  matriz.colunas = numColunas;
  
  matriz.Assento = inicializarMatriz(matriz, numLinhas, numColunas);

  printf("\nTeatro criado!!!\n");
  return matriz;
}


void exibirTeatro(Teatro teatro){ //❎😎
  printf("\nLEGENDA:\
          \n\t💺 - Assento disponível\
          \n\t❌ - Assento reservado\
        \n\nASSENTOS DO TEATRO:\n");
  //Índice de Assentos de 1 a N
  printf("    ");
  for(int i = 0; i < teatro.colunas; i++){
    printf("%d", i+1);
    if(i!=teatro.colunas-1){
      printf("   ");
      if(i<9){
        printf(" ");
      }
    }
  }
  printf("\n");  
  for(int i=0; i<(teatro.colunas*5); i++){
    if(i<1){
      printf("   ");
    }
    else{
      printf("-");
    }
  }
  //Índice de Assentos de A a ? e os Assentos
  for(int y=0; y<teatro.linhas; y++){
    printf("\n%c | ", alfabeto[y]);
    for(int x=0; x<teatro.colunas; x++){
      if(teatro.Assento[y][x].reservado == true){
        printf("❌");
      }
      else{
        printf("💺");
      }

      if(x!=teatro.colunas-1){
        printf(" - ");
      }
    }
  }
}


void liberarMatriz(Assento **matriz, int linhas){
  //Desaloca a memória da matriz de assentos.
  for(int i=0; i<linhas; i++){
    free(matriz[i]);
  }
}



//--------------------------------Reservas e Cancelamentos--------------------------------

int reservarAssento(Teatro t, int linha, int coluna, char *nome){
  if(t.Assento[linha][coluna-1].reservado == 0){
    t.Assento[linha][coluna-1].reservado = 1;
    strcpy(t.Assento[linha][coluna-1].nome, nome);
    return 1;
  }
  else{
    return 0;
  }
}


int reservarAssentosConsecutivos(Teatro t, int linha, int coluna, int n, char *nome){
  int livres = 0;
  if(n>t.colunas){
    return -1;
  }
  else{
    //Verifica se existem N assentos consecutivos disponíveis
    for(int i=coluna-1; i<(coluna+n-1); i++){
      if(t.Assento[linha][i].reservado == 0){
        livres++;
      }
    }
    //Tomada de decisão
    if(livres == n){
      for(int i=coluna-1; i<(coluna+n-1); i++){
        t.Assento[linha][i].reservado = 1;
        strcpy(t.Assento[linha][i].nome, nome);  
      }
      return 1;
    }
    else{
      return 0;
    }
  }   
}


Assento * reservarAssentoPeloSistema(Teatro t, char *nome){
  int reserva=0;
  for(int y=0; y<t.linhas; y++){
    for(int x=0; x<t.colunas; x++){
      if(t.Assento[y][x].reservado == 0){
        t.Assento[y][x].reservado = 1;
        printf("Reserva realizada com sucesso! Assento %c%d reservado.", alfabeto[y], x+1);
        strcpy(t.Assento[y][x].nome, nome);
        reserva=1;
        return &t.Assento[y][x];
      }
    }
  }
  if(reserva==0){
    return NULL;
  }
}

  
Assento ** reservarAssentosConsecutivosPeloSistema(Teatro t, int n, char *nome){
  int livres=0, inicial=-1, aFinal;
  
  if(n>t.colunas){ //Não há assentos suficientes
    return NULL;
  }
  else{
    //Verifica se existem N assentos consecutivos disponíveis
    for(int y=0; y<t.linhas; y++){
      for(int x=0; x<t.colunas; x++){
        if(t.Assento[y][x].reservado==0){
          livres++;
          if(livres == 1){
            inicial = x;
          }
          if(livres == n){ //Tem N?
            aFinal = x+1;
            printf("\nReserva realizada dos assentos ");
            printf("%c%d a %c%d ", alfabeto[y], inicial + 1, alfabeto[y], aFinal);

            //Reservando eles
            for(int i=inicial; i<aFinal; i++){
              t.Assento[y][i].reservado = 1;
              strcpy(t.Assento[y][i].nome, nome);
            }
            return t.Assento;
          }
        }
        else {
          livres = 0;
        }
      }
      livres = 0;
    }
  }

  if(livres != n){
    return NULL;
  }
  
}

  
int cancelarReserva(Teatro t, int linha, int coluna){
  if(t.Assento[linha][coluna-1].reservado == 1){
    t.Assento[linha][coluna-1].reservado = 0;
    t.Assento[linha][coluna-1].nome[0] = '\0';
    return 1;
  }
  else{
    return 0;
  }
}


int cancelarReservas(Teatro t, char *nome){
  int canceladas = 0;
  for(int y=0; y<t.linhas; y++){
    for(int x=0; x<t.colunas; x++){
      if(strcmp(t.Assento[y][x].nome, nome) == 0){
        t.Assento[y][x].nome[0] = '\0';
        t.Assento[y][x].reservado = 0;
        canceladas++;
      }
    }
  }
  return canceladas;
}


int cancelarTodasAsReservas(Teatro t){
  int canceladas=0;
  for(int y=0; y<t.linhas; y++){
    for(int x=0; x<t.colunas; x++){
      if(t.Assento[y][x].reservado == 1){
        t.Assento[y][x].reservado = 0;
        t.Assento[y][x].nome[0] = '\0';
        canceladas++;
      }
    }
  }
  return canceladas;
}



//--------------------------------Consultas e Estatísticas--------------------------------

int verificarDisponibilidade(Teatro t, int linha, int coluna){
  if(t.Assento[linha][coluna-1].reservado == 0){
    return 0;
  }
  else{
    return 1;
  }
}


void consultarAssentosConsecutivos(Teatro t, int n){
  int livres=0, intervalos=0, inicial=0, aFinal=0, encontrado=0;
  
  if(n > t.colunas){
    printf("\nNão existe fileira com %d assentos!\n", n);
    encontrado=-1;
  }
  else{
    for(int y = 0; y < t.linhas; y++){
      for(int x = 0; x <= t.colunas; x++){ // Para incluir o último assento
        if(x < t.colunas && t.Assento[y][x].reservado == 0){
          livres++;
          if(livres == 1){
            inicial = x;
          }
        }
        else {
          if(livres >= n){
            aFinal = x;
            // Imprimindo o maior intervalo possível
            if(intervalos == 0 || (aFinal - inicial) > (t.Assento[y][aFinal-1].reservado - t.Assento[y][inicial].reservado)){
              if(encontrado == 0){
                printf("\nOs intervalos de (no mínimo) %d assentos consecutivos são:\n", n);
              }
              encontrado = 1;
              printf("[%c%d-%c%d] ", alfabeto[y], inicial + 1, alfabeto[y], aFinal);
              intervalos++;
            }
          }
          livres = 0;
        }
      }
    }
  }
  if(encontrado == 0){
    printf("\nNão existe %d assentos consecutivos disponíveis!\n", n);
  }
}


void exibirInformacoes(Teatro t){
  int ocupados=0, disponiveis=0, sequencia=0, maior=-1;

  for(int y=0; y<t.linhas; y++){
    for(int x=0; x<t.colunas; x++){
      //assentos reservados e disponiveis
      if(t.Assento[y][x].reservado == 1){
        ocupados++;
      }
      else{
        disponiveis++; 
      }
      
      //maior sequencia de assentos livres
      if(x!=0){
        if(t.Assento[y][x].reservado==0 && t.Assento[y][x-1].reservado==0){
          sequencia++;
          if(sequencia>maior){
            maior = sequencia;
          }
        }
        else{
          sequencia = 0;
        }
      }
    }
    sequencia = 0;
  }

  printf("Existem %d assentos disponíveis e %d ocupados.\
    \nA maior sequência de assentos disponíveis consecutivamente é de %d assentos.", disponiveis, ocupados, maior+1);
}


//--------------------------------Armazenamento--------------------------------

int salvarEstado(Teatro t, char *nomeArquivo){
  // Adicionando nome do arquivo em lista (se ainda não estiver lá)
  FILE *listaTeatros = fopen("listaTeatros.txt", "a+");
  
  // Verificando se nome do arquivo já existe
  char linhasLista[100];
  int existe=0;
  fseek(listaTeatros, 0, SEEK_SET);
  while(fgets(linhasLista, sizeof(linhasLista), listaTeatros) != NULL){
    linhasLista[strcspn(linhasLista, "\n")] = '\0';
    if(strcmp(linhasLista, nomeArquivo) == 0){
      existe = 1;
    }
  }
  if(existe==0){
    fprintf(listaTeatros, "%s\n", nomeArquivo);
  }
  fclose(listaTeatros);

  // Criando arquivo do Teatro e chamando função para inserir informações
  FILE *arquivoTeatro = fopen(nomeArquivo, "w");
  if(arquivoTeatro == NULL){
    printf("\nErro na abertura do arquivo!\n");
    return 0;
  }
  else{
    //Editando Arquivo
    fprintf(arquivoTeatro, "Matriz %d * %d\n", t.linhas, t.colunas);
    for(int y=0; y<t.linhas; y++){
      for(int x=0; x<t.colunas; x++){
        fprintf(arquivoTeatro, "%d,%d,%d,", t.Assento[y][x].fileira, t.Assento[y][x].numero, t.Assento[y][x].reservado);
        if(strcmp(t.Assento[y][x].nome,"\0")==0){
          fprintf(arquivoTeatro, "NULL\n");
        }
        else{
          fprintf(arquivoTeatro, "%s\n", t.Assento[y][x].nome);
        }
      }
    }
    fclose(arquivoTeatro); //fecha o arquivo
    return 1;
  }
  
}


Teatro carregarEstado(char *nomeArquivo){
  int lin = -1, col = -1;
  Teatro t;
  FILE *arquivoTeatro = fopen(nomeArquivo, "r");

  if(arquivoTeatro == NULL){
    printf("\nArquivo não encontrado!\n");
    t.colunas = -1;
    t.linhas = -1;
    return t;
  }
  else{
    fscanf(arquivoTeatro, "Matriz %d * %d\n", &lin, &col);

    //Alocando espaço para a matriz
    t.Assento = inicializarMatriz(t, lin, col);
    t.linhas = lin;
    t.colunas = col;

    //Transferindo dados para a matriz atual
    for(int y = 0; y <t.linhas; y++){
      for(int x = 0; x < t.colunas; x++){
        fscanf(arquivoTeatro, "%d,%d,%d,%99[^\n]\n", &t.Assento[y][x].fileira, &t.Assento[y][x].numero, &t.Assento[y][x].reservado, t.Assento[y][x].nome);
        
        if(strcmp(t.Assento[y][x].nome,"NULL")==0){
          strcpy(t.Assento[y][x].nome,"\0");
        }
      }
    }

    fclose(arquivoTeatro); 
    printf("\nTeatro carregado com sucesso!\n");
    return t;
  }
}



//#################################################################################
//Transformar a letra da fileira em um número para funções na matriz
int letraFileiraNumero(char* letraFileira, int reservarFileira){
  for(int i=0; i<26; i++){
    if(toupper(letraFileira[0]) == alfabeto[i]){
      reservarFileira = i;
      break;
    }
  }
  return reservarFileira;
}

int main(void) {
  //variáveis
  int opcao=-1, numLinhas=0, numColunas=0, reservarFileira, reservarCadeira, assentoFinal, reserva=-1, numAssentos=0, totalCanceladas=0, estado;
  char quemReserva[51], letraFileira[1], nomeTeatro[51], nomeArquivo[51];

// Inserir linhas e colunas da matriz
  while((numLinhas>26 || numLinhas<2) || (numColunas>99 || numColunas<2)){
    printf("Para criar um Teatro digite a quantidade de fileiras (mínimo 2 e máximo 26) e de assentos por fileira (mínimo 2 e máximo 99) reespectivamente: (Ex: 2 3)   ");
    scanf("%d %d", &numLinhas, &numColunas);

    if((numLinhas>26 || numLinhas<2) || (numColunas>99 || numColunas<2)){
      printf("\nQuantidade de linhas ou colunas não aceita! Por favor insira novamente!\n\n");
    }
  }

  Teatro t1, teatro = criarTeatro(numLinhas, numColunas);
  
  // criarTeatro(numLinhas, numColunas);

// ------------- OPERAÇÕES ------------- 
  while(opcao != 0){
    
    // Opções de operações
    {printf("\n\n#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-\n\n");

    printf("0- Sair do programa\
        \n1- Salvar estado \
        \n2- Carregar estado \
        \n3- Visualizar \
        \n4- Reservar assento específico \
        \n5- Reservar assentos consecutivos específicos \
        \n6- Reservar assento pelo sistema \
        \n7- Reservar assentos consecutivos pelo sistema \
        \n8- Cancelar reserva de um assento \
        \n9- Cancelar reservas de uma pessoa \
        \n10- Cancelar todas as reservas \
        \n11- Verificar disponibilidade \
        \n12- Consultar assentos consecutivos \
        \n13- Exibir informações\n");

    printf("\nDigite o número do que deseja fazer: ");
    scanf("%d", &opcao);
    printf("\n\n#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-\n\n");}

    //Realizando operações e chamando funções
    switch(opcao){
      case 0:
        {system("clear");
        printf("Saindo do programa...");
        return 0;
        break;}
      
      case 1: //Salvar estado
        {system("clear");
        printf("Qual o nome do Teatro a ter um arquivo criado? \n");
        scanf("%50s", nomeTeatro); 
        strcpy(nomeArquivo, "TEATROS/");
        strcat(nomeArquivo, nomeTeatro);
        strcat(nomeArquivo, ".txt");
        estado = salvarEstado(teatro, nomeArquivo);
        if(estado==0){
          printf("Salvamento cancelado.");
        }
        else{
          printf("Estado salvo!");
        }
        break;}
      
      case 2: //2- Carregar estado
        {system("clear");
        char  opcaoListaTeatros[63];
        int cont=1, arq;
        //Lista de arquivos
        FILE *listaTeatros = fopen("listaTeatros.txt", "r");
        if(listaTeatros == NULL){
          printf("\nNão há arquivos!\n");
        }
        else{
          printf("Arquivos no sistema:\n");
           while (fgets(opcaoListaTeatros, sizeof(opcaoListaTeatros), listaTeatros) != NULL) {
              printf("%d- %s", cont, opcaoListaTeatros);
              cont++;
           }
        }
        fclose(listaTeatros);
        listaTeatros = fopen("listaTeatros.txt", "r");

        //Escolhendo arquivo
        printf("Qual o número do arquivo a ser carregado?  ");
        scanf("%d", &arq);
        cont = 1;
        while(fgets(opcaoListaTeatros, sizeof(opcaoListaTeatros), listaTeatros) != NULL) {
          if(cont==arq){
            sscanf(opcaoListaTeatros, "%s\n", nomeArquivo);
          }
          cont++;
         }
         
        t1 = carregarEstado(nomeArquivo);
        if(t1.colunas!=-1 && t1.linhas!=-1){
          liberarMatriz(teatro.Assento, teatro.linhas);
          teatro = t1;
        }
        break;}
      
      case 3: //3- Visualizar
        {system("clear");
        exibirTeatro(teatro);
        break;}
      
      case 4: //4- Reservar assento específico
        {system("clear");
        printf("Qual assento de A1 a %c%d deseja reservar? (Ex: A1)  ", alfabeto[teatro.linhas-1], teatro.colunas);
        scanf(" %c%d", letraFileira, &reservarCadeira);
        reservarFileira = letraFileiraNumero(letraFileira, reservarFileira);
        printf("No nome de quem deseja reservar?  ");
        scanf("%50s", quemReserva);

        reserva = reservarAssento(teatro, reservarFileira, reservarCadeira, quemReserva);
        if(reserva==0){
          printf("\nAssento já ocupado! Reserva não realizada.\n\n");
        }
        else if(reserva==1){
          printf("\nAssento reservado com sucesso!\n\n");
        }
        reserva = -1;
        break;}
      
      case 5: //5- Reservar assentos consecutivos específicos
        {system("clear");
        printf("Qual o assento Inicial de A1 a %c%d deseja reservar? (Ex: A1)  ", alfabeto[teatro.linhas-1], teatro.colunas);
         scanf(" %c%d", letraFileira, &reservarCadeira);
        reservarFileira = letraFileiraNumero(letraFileira, reservarFileira);
        printf("Qual o número do último Assento entre %c%d e %c%d que deseja reservar? (Ex: 3)  ", toupper(letraFileira[0]), reservarCadeira+1, toupper(letraFileira[0]), teatro.colunas);
        scanf("%d", &assentoFinal);
        numAssentos = (assentoFinal - reservarCadeira) + 1;        
        printf("No nome de quem deseja reservar?  ");
        scanf("%50s", quemReserva);

        reserva = reservarAssentosConsecutivos(teatro, reservarFileira, reservarCadeira, numAssentos, quemReserva);
        if(reserva==0){
          printf("\nAssentos já ocupados! Reserva não realizada.\n\n");
        }
        else if(reserva==1){
          printf("\nAssentos reservados com sucesso!");
        }
        else if(reserva==-1){
          printf("\nReserva não realizada: número de Assentos escolhido maior do que a quantidade existente em uma fileira ou Assento não existente!");
        }
        reserva = -1;
        break;}
      
      case 6: //6- Reservar assento pelo sistema
        {system("clear");
        printf("No nome de quem deseja reservar?  ");
        scanf("%50s", quemReserva);
        Assento *ptr;
        ptr = reservarAssentoPeloSistema(teatro, quemReserva);
        if(ptr == NULL){
          printf("Não há assentos disponíveis para reserva!");
        }
        break;}
      
      case 7: //7- Reservar assentos consecutivos pelo sistema
        {system("clear");
        printf("Qual o número de Assentos consecutivos deseja reservar?  ");
        scanf("%d", &numAssentos);
        printf("No nome de quem deseja reservar?  ");
        scanf("%50s", quemReserva);
        Assento **ptr;
        ptr = reservarAssentosConsecutivosPeloSistema(teatro, numAssentos, quemReserva);
        if(ptr == NULL){
          printf("Não há %d assentos disponíveis para reserva!", numAssentos);
        }
        break;}
      
      case 8: //8- Cancelar reserva de um assento
        {system("clear");
        printf("Qual assento de A1 a %c%d deseja cancelar a reserva? (Ex: A1)  ", alfabeto[teatro.linhas-1], teatro.colunas);
        scanf(" %c%d", letraFileira, &reservarCadeira);
        reservarFileira = letraFileiraNumero(letraFileira, reservarFileira);
        reserva = cancelarReserva(teatro, reservarFileira, reservarCadeira);
        if(reserva==0){
          printf("\nAssento não tinha reserva!\n\n");
        }
        else if(reserva==1){
          printf("\nReserva cancelada com sucesso!");
        }
        reserva = -1;
        break;}
      
      case 9: //9- Cancelar reservas de uma pessoa
        {system("clear");
        printf("Qual o nome da pessoa que deseja cancelar as reservas?  ");
        scanf("%50s", quemReserva);
        totalCanceladas = cancelarReservas(teatro, &quemReserva[0]);
        if(totalCanceladas==0){
          printf("Reservas no nome de %s não encontradas ou não existentes!", quemReserva);
        }
        else{
          printf("Foram um total de %d reservas canceladas no nome de %s.", totalCanceladas, quemReserva);
        }
        break;}
      
      case 10: //10- Cancelar todas as reservas
        {system("clear");
        totalCanceladas = cancelarTodasAsReservas(teatro);
        printf("Todas as reservas foram canceladas!\nForam canceladas um total de %d reservas.\n", totalCanceladas);
        break;}
      
      case 11: //11- Verificar disponibilidade
        {system("clear");
        printf("Qual Assento deseja verificar a disponibilidade? (Ex: A1)  ");
        scanf("%c%d", letraFileira, &reservarCadeira);
        reservarFileira = letraFileiraNumero(letraFileira, reservarFileira);
        reserva = verificarDisponibilidade(teatro, reservarFileira, reservarCadeira);
        if(reserva==0){
          printf("\nAssento já ocupado!\n\n");
        }
        else if(reserva==1){
          printf("\nAssento disponível!\n\n");
        }
        reserva = -1;
        break;}
      
      case 12: //12- Consultar assentos consecutivos
        {system("clear");
        printf("Qual o número de assentos consecutivos que deseja consultar?  ");
        scanf("%d", &numAssentos);
        consultarAssentosConsecutivos(teatro, numAssentos);
        break;}
      
      case 13: //13- Exibir informações
        {system("clear");
        exibirInformacoes(teatro);
        break;}
      
      default:
        printf("OPÇÃO INDISPONÍVEL!");
    }
  } 
}
