#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int quantidades[3]={0};

//struct para cadastro
typedef struct{
  char cpf[12];
  char nome[100];
  char telefone[14];
  float renda;
  char rua[30];
  char numero[6];
  char bairro[30];
  char cidade[30];
  char estado[2];
}Cliente;

// struct para Partida e Apostas
struct Data{
  int dia;
  int mes;
  int ano;
};

//struct para Apostas
typedef struct{
  char cpf_do_Cliente[12];
  struct Data data;
  int identificador_da_partida;
  float cotacao;
  float valor_aposta;
  char timeApostado;
  int vencedor;
} Aposta;

//struct para ChaveCliente
typedef struct{
  char cpf_do_Cliente[12];
  int posicao; // guarda a posicao em que o cliente foi inserido.
} ChaveCliente;

//struct para Partidas
typedef struct{
  int indentificador;
  struct Data dia;
  char timeA[100];
  char timeB[100];
  char estadio[200];
  int gols_timeA;
  int gols_timeB;
  int terminada;
  char timeGanhador[100];
} Partidas;

//struct para ChaveApostas
typedef struct{
    float valor;
    int posicao;
} ChaveAposta;

// PROTOTIPOS
void menu();
//opcao 1 - CLIENTES
void alterar_clientes(ChaveCliente *chave);
int buscarClientes(char *cpf, int tamanho, ChaveCliente *chave);
ChaveCliente* controle_de_clientes(ChaveCliente *chave);
void mostrarTodos();
ChaveCliente* novoCliente(ChaveCliente *chave);
//opcao 2 - PARTIDAS
void alteraVencedor(int identificador, char timeGanhador);
void finalizarPartida(int dia, int mes, int ano);
void mostrarDetalhesPartida(Partidas partida);
void novaPartida();
void partidas();
void verEspecificos();
//opcao 3 - APOSTAS
void atualiza_chaveApostas(Aposta apostas);
void inserirAposta(ChaveCliente *chave);
void ordena_chaveApostas();
int verificarPartidaDisponivel(int indentifica);
//opcao 4 - RELATORIOS
void apostasAnoEspecifico();
void apostasGanhadorVenceu(ChaveCliente *chave);
void relatorios(ChaveCliente *chave);
void todasApostas_deUmApostador(ChaveCliente *chave);
void totaisGanhosePerdidos(ChaveCliente *chave);
//Funções auxiliares
void atualiza_quantidades();
void ler_quantidades();

ChaveCliente* atualizaChaveCliente(Cliente cliente, ChaveCliente *chave);
void mostra_chaveCliente(ChaveCliente *chave);

ChaveCliente* baixaChaveCliente(ChaveCliente *chave);
void exibeDadosDeUmCliente(int posicao);
void sobeChaveCliente(ChaveCliente *Chave);


int main(){
    int op;
    ChaveCliente *chave;

    if(quantidades[0] == 0){//só aloca quando o programa inicia, Por que senao toda vez que o programa voltasse ao main, ele iria alocar novamente
        ler_quantidades();

        if(quantidades[0] == 0){ //confirma se realmente é a primeiro cliente a ser cadastrado
            chave = (ChaveCliente *)calloc(1,sizeof(ChaveCliente)); //cria o vetor dinamico para receber chavecliente
            if(!chave){
                printf("\nNao foi possivel alocar o vetor!\n");
                exit(1);
            }
        }else{ //se já existe cliente cadastrado, ele baixa os dados do CHAVECLIENTE
            chave = baixaChaveCliente(chave);
        }
    }

    do{
        menu();
        printf("Digite a opcao que deseja: ");
        scanf("%d", &op);

        switch(op){
            case 1:
                chave = controle_de_clientes(chave);
                break;
            case 2:
                partidas();
                break;
            case 3:
                inserirAposta(chave);
                break;
            case 4:
                relatorios(chave);
                break;
            case 5:
                printf("\nSaindo do programa...\n");
                break;
            default:
                printf("OPCAO INVALIDA!\n");
        }
      }while(op != 5);

      if(quantidades[0] != 0){ //caso nao exista nenhum cliente cadastrado, nao há necessidade de subir um chavecliente pro arquivo
          sobeChaveCliente(chave);
      }
    free(chave);
    return 0;
}
// menu
void menu(){
  printf("\n ______= CASA DE APOSTA =______\n");
  printf("|                              |\n");
  printf("| 1 - Controle de Clientes     |\n");
  printf("| 2 - Partidas                 |\n");
  printf("| 3 - Apostas                  |\n");
  printf("| 4 - Relatorio                |\n");
  printf("| 5 - Sair do programa         |\n|");
  printf("______________________________|\n\n");
}
//opcao 1
ChaveCliente* controle_de_clientes(ChaveCliente *chave){
  int op;
  do{
      printf("\n ______________| Menu de Controle |______________\n");
      printf("|                                                |\n");
      printf("| 1 - Cadastrar novo cliente                     |\n");
      printf("| 2 - Mostrar todos os clientes                  |\n");
      printf("| 3 - Alterar os dados de um cliente especifico  |\n");
      printf("| 4 - Sair desta aba                             |\n");
      printf("|________________________________________________|\n");
      printf("\nDigite opcao que deseja acessar: ");
      scanf("%d", &op);
      switch (op)
      {
          case 1:
              chave = novoCliente(chave);
              break;
          case 2:
              mostrarTodos();
              break;
          case 3:
              alterar_clientes(chave);
              break;
          case 4:
              printf("Saindo desta aba\n");
              break;
          default:
              printf("OPCAO INVALIDA!\n");
      }
  }while(op != 4);
  return chave;
}

ChaveCliente* novoCliente(ChaveCliente *chave){
    ler_quantidades(); //inicializa o vetor "quantidades" com os dados do arquivo QUANTIDADES.bin

    Cliente cliente;
    FILE *ptarq = fopen("CADASTROS.bin","ab"); // abre o arquivo cadastros.bin
    if(!ptarq){ //se o arquivo nao existir
        printf("\nERRO: O arquivo CADASTROS.bin não foi encontrado\n");
        exit(1);
    }

    printf("\n\n ________________| NOVO CLIENTE |________________\n");
    printf("| Cliente %d                                      |\n", quantidades[0]);
    printf("|------------------------------------------------|\n");
    printf("| CPF:       | ");
    scanf("\n%[^\n]s", cliente.cpf);

    //verificacao para nao deixar usuario digitar um cpf invalido
    if(strlen(cliente.cpf) != 11){ //se a quantidade de digitos for diferente de 11
        printf("\nCPF digitado contem um numero maior ou menor do que a quantidade oficial de numeros (11 digitos)!\n");
        return chave;
    }

    //verificação se o cpf já está cadastrado
    int existeoCPF;
    existeoCPF = buscarClientes(cliente.cpf, quantidades[0], chave); //busca o cpf digitado no vetor para ver se ele já está cadastrado, se nao tiver ele retorna -1
    if(existeoCPF != -1){//qualquer coisa diferente de -1 significa que o cpf já existe na lista
        printf("|________________________________________________|\n");
        printf("\nESSE CPF JA FOI CADASTRADO! Por favor tente novamente com outro CPF\n");
        return chave; //se o cpf ja foi cadastrado, encerra essa funcao
    }


    printf("| Nome:      | ");
        scanf("\n%[^\n]s", cliente.nome);
    printf("| Telefone:  | ");
        scanf("\n%[^\n]s", cliente.telefone);
    printf("| Renda:     | ");
        scanf("%f", &cliente.renda);
    printf("| Rua:       | ");
        scanf("\n%[^\n]s", cliente.rua);
    printf("| Numero:    | ");
        scanf("\n%[^\n]s", cliente.numero);
    printf("| Bairro:    | ");
        scanf("\n%[^\n]s", cliente.bairro);
    printf("| Cidade:    | ");
        scanf("\n%[^\n]s", cliente.cidade);
    printf("| Estado(UF):| ");
        scanf("\n%[^\n]s", cliente.estado);
    printf("|________________________________________________|\n");

    //salvando no arquivo CADASTROS.bin
    quantidades[0] += fwrite(&cliente, sizeof(Cliente), 1, ptarq); //a quantidade de clientes será acrescentada a cada vez que cadastrar um novo cliente
    fclose(ptarq); //fecha o arquivo cadastros.bin

    //atualizações de arquivos
    atualiza_quantidades(); //atualiza o arquivo QUANTIDADES.bin, com as quantidades de cliente alterada
    chave = atualizaChaveCliente(cliente, chave); //atualiza o arquivo CHAVECLIENTE.bin com os novos cpfs e posições
    return chave;
}

void mostrarTodos(){
  Cliente cliente;
  FILE *ptarq = fopen("CADASTROS.bin", "rb"); // abre o arquivo
  if(!ptarq){ //se o arquivo nao existir
    printf("ERRO: Ainda nao existe nenhum cliente cadastrado!\n");
    return;
  }

  printf("\n ______________________________| LISTA DE CLIENTES CADASTRADOS |______________________________\n|");
  //ler o uma struct por vez
  while(fread(&cliente, sizeof(Cliente), 1, ptarq)){
        printf("\n| - CPF: %s\n",cliente.cpf);
        printf("| - Nome: %s\n", cliente.nome);
        printf("| - Telefone: %s\n", cliente.telefone);
        printf("| - Renda: %.2f\n", cliente.renda);
        printf("| - Endereco: %s, %s, %s, %s, %s\n",cliente.rua,cliente.numero,cliente.bairro,cliente.cidade,cliente.estado);
  }
  printf("|_____________________________________________________________________________________________|\n");
  fclose(ptarq); // fechando o arquivo
}

int buscarClientes(char *cpf, int tamanho, ChaveCliente *chave){
    ler_quantidades();
    int inicio = 0, meio, fim = tamanho-1;
    int comparador, posicao = -1;

    //busca binaria
    while(inicio <= fim)
    {
        meio = (inicio+fim)/2;
        comparador = strcmp(cpf, chave[meio].cpf_do_Cliente); //compara o cpf procurado com o cpf do vetor na posicao meio
        if(comparador == 0){
            posicao = chave[meio].posicao;
            break;
        }else
        if(comparador < 0){//significa que o cpf procurado está depois do cpf que está na variavel "meio"
            fim = meio-1;//fim do loop será agora a posicao meio-1
        } else {//significa que o cpf procurado está depois do cpf que está na variavel "meio"
            inicio = meio+1;  //inicio do loop será agora a posicao meio+1
        }
    }
    //se o cpf nao for encontrado retorna -1
    return posicao;
}

void alterar_clientes(ChaveCliente *chave){
    ler_quantidades();
    char cpf_buscado[12];

    //recebe o cpf a ser alterado
    printf("\nDigite o CPF que deseja alterar: ");
    scanf("\n%[^\n]s", cpf_buscado);
    if(strlen(cpf_buscado) != 11){ //verificacao para que o usuario nao digite um cpf maior ou menor do que o esperado
        printf("\nVoce digitou um CPF menor ou maior do que o comum (11 numeros)!\n");
        return;
    }
    //procura o cpf no vetor
    int posicao = buscarClientes(cpf_buscado, quantidades[0], chave);
    if(posicao != -1) //se o cpf for encontrado
    {   printf("\n\n| Cliente encontrado! |");
        exibeDadosDeUmCliente(posicao);
    }else //se ele nao for encontrado
    {   printf("\nEsse CPF ainda nao esta cadastrado!\n");
        return;
    }
    //abrir arquivo CADASTROS.bin, para alocar os dados do cpf escolhido, em seguida alterar os dados que o usuario quiser e salvar novamente no arquivo
    FILE *ptarq;
    ptarq = fopen("CADASTROS.bin", "r+b");
    if(!ptarq){
        printf("\nErro ao abrir o arquivo!\n");
        exit(1);
    }

    Cliente cliente;
    fseek(ptarq, posicao*sizeof(Cliente), SEEK_SET); //posiciona o cursor do arquivo para a posicao onde está o cpf
    fread(&cliente, sizeof(Cliente), 1, ptarq); //copia os dados para struct cliente

    //alteração dos dados do vetor
    int opcao;
    printf("\n\n\n\tALTERACAO DOS DADOS DO CLIENTE\n");
    do
    {   printf(" _____________________");
        printf("\n|1 - Nome             |\n|2 - Telefone         |\n|3 - Renda            |\n|4 - Rua              |\n|5 - Numero           |\n|6 - Bairro           |\n|7 - Cidade           |\n|8 - Estado           |\n|9 - Parar de alterar |\n");
        printf("|_____________________|");
        printf("\nDigite a opcao que deseja alterar: ");
        scanf("%d", &opcao);

        switch(opcao)
        {
            case 1:
                printf("\nNome antigo: %s", cliente.nome);
                printf("\nDigite o novo nome: ");
                scanf("\n%[^\n]s", cliente.nome);
                printf("\nNOME ALTERADO!\n");
                break;
            case 2:
                printf("\nTelefone antigo: %s", cliente.telefone);
                printf("\nDigite o novo telefone: ");
                scanf("\n%[^\n]s", cliente.telefone);
                printf("\nTELEFONE ALTERADO!\n");
                break;
            case 3:
                printf("\nRenda antiga: %.2f", cliente.renda);
                printf("\nDigite a nova renda: ");
                scanf("%f", &cliente.renda);
                printf("\nRENDA ALTERADA!\n");
                break;
            case 4:
                printf("\nRua antiga: %s", cliente.rua);
                printf("\nDigite a nova rua: ");
                scanf("\n%[^\n]s", cliente.rua);
                printf("\nRUA ALTERADA!\n");
                break;
            case 5:
                printf("\nNumero antigo: %s", cliente.numero);
                printf("\nDigite o novo numero: ");
                scanf("\n%[^\n]s", cliente.numero);
                printf("\nNUMERO ALTERADO!\n");
                break;
            case 6:
                printf("\nBairro antigo: %s", cliente.bairro);
                printf("\nDigite o novo bairro: ");
                scanf("\n%[^\n]s", cliente.bairro);
                printf("\nBAIRRO ALTERADO!\n");
                break;
            case 7:
                printf("\nCidade antiga: %s", cliente.cidade);
                printf("\nDigite a nova cidade: ");
                scanf("\n%[^\n]s", cliente.cidade);
                printf("\nCIDADE ALTERADA!\n");
                break;
            case 8:
                printf("\nEstado antigo: %s", cliente.estado);
                printf("\nDigite o novo estado: ");
                scanf("\n%[^\n]s", cliente.estado);
                printf("\nESTADO ALTERADO!\n");
                break;
            case 9:
                break;
            default:
                printf("\nOPCAO INVALIDA!\n");
        }

    }while(opcao != 9);

    fseek(ptarq, posicao*sizeof(Cliente), SEEK_SET); //posiciona o cursor do arquivo para a posicao onde está o cpf
    fwrite(&cliente, sizeof(Cliente), 1, ptarq); //copia os dados da struct para o arquivo
    fclose(ptarq);
}
//opcao 2
void partidas(){
  int op;
  int dia, mes, ano;

  do{
      printf("\n\n __________| PARTIDAS |__________\n");
      printf("| 1 - Nova Partida               |\n");
      printf("| 2 - Finalizar Partida          |\n");
      printf("| 3 - Ver ao especifico          |\n");
      printf("| 4 - Sair desta aba             |\n");
      printf("|________________________________|\n");

      printf("\nDigite opcao que deseja acessar: ");
      scanf("%d", &op);
      switch (op)
      {
      case 1:
          novaPartida();
          break;
      case 2:
          printf("\n\tFINALIZAR PARTIDA\n");
          printf("\nDigite uma data para exibicao das partidas (dd mm aaaa): ");
          scanf("%d%d%d", &dia, &mes, &ano);
          finalizarPartida(dia, mes, ano);
          break;
      case 3:
          verEspecificos();
          break;
      case 4:
          printf("Saindo desta aba...\n");
          break;
      default:
          printf("OPCAO INVALIDA!\n");
      }
  }while(op != 4);
}

void novaPartida(){
    ler_quantidades();//inicializa o vetor "quantidades" com os dados do arquivo QUANTIDADES.bin
    Partidas partida;
    int qntPartidas = 0;

    FILE *ptarq = fopen("PARTIDA.bin", "ab"); // abre o vetor para escrita
    if(!ptarq){ // se o vetor for nulo, aparecerá erro
        printf("\nERRO: O arquivo PARTIDA.bin nao foi encontrado\n");
        exit(1);
    }
    printf("\n\tPARTIDA NUMERO %d\n", quantidades[1]);
    printf("- Data da Partida (DD MM AAAA): ");
        scanf("%d %d %d", &partida.dia.dia, &partida.dia.mes, &partida.dia.ano);
    printf("- Time A: ");
        scanf("\n%[^\n]s", partida.timeA);
    printf("- Time B: ");
        scanf("\n%[^\n]s", partida.timeB);
    printf("- Estadio que sera realizado: ");
        scanf("\n%[^\n]s", partida.estadio);
    printf("- Gols do time A: ");
        scanf("%d", &partida.gols_timeA );
    printf("- Gols do time B: ");
        scanf("%d", &partida.gols_timeB );
    printf("\n\tLEGENDA: \n0 - Em andamento\n1- Terminada\n");
    printf("\n- Partida terminada? ");
        scanf("%d", &partida.terminada);

    if(partida.terminada == 1){
        printf("Digite o time que ganhou a partida: ");
        scanf("\n%[^\n]s", partida.timeGanhador);
    }
    partida.indentificador = quantidades[1];

    //salvando uma struct no arquivo de partidas
    quantidades[1] += fwrite(&partida,sizeof(Partidas),1,ptarq); //a quantidade de partidas será acrescentada a cada vez que cadastrar uma nova partida
    fclose(ptarq);// ao terminar de salvar as struct, fechar o arquivo de escrita
    atualiza_quantidades(); //atualiza o arquivo QUANTIDADES.bin, com as quantidades de partidas cadastradas

    // abrir arquivo pra leitura
    ptarq = fopen("PARTIDA.bin", "rb"); // abre o vetor para escrita
    if(!ptarq){ // se o vetor for nulo, aparecerá erro
        printf("\nERRO AO ABRIR O ARQUIVO PARTIDA.bin!\n");
        exit(1);
    }

    printf("\n\n\tPARTIDAS JA CADASTRADAS\n");
    while(fread(&partida, sizeof(Partidas), 1, ptarq)){ // Mostrar as partidas já Cadastradas
        printf("\n__________________________________");
        printf("\n Partida %d\n", qntPartidas);
        printf("- Data da Partida: %d / %d / %d \n", partida.dia.dia, partida.dia.mes, partida.dia.ano);
        printf("- Time A e gols: %s  - %d\n", partida.timeA, partida.gols_timeA );
        printf("- Time B e gols: %s  - %d \n", partida.timeB, partida.gols_timeB );
        printf("- Estadio: %s\n", partida.estadio);
        if(partida.terminada == 0){
            printf("- Situacao da partida: EM ANDAMENTO\n");
        }else{
            printf("- Situacao da partida: FINALIZADA\n");
        }
        printf("__________________________________\n");
        if (partida.terminada == 0) {
            printf("- Time Ganhador: - \n");
        } else {
            printf("- Time Ganhador: %s\n", partida.timeGanhador);
        }
        qntPartidas++;
    }
    fclose(ptarq); // ao terminar de salvar as struct, fechar o arquivo
}

void verEspecificos(){
    Partidas partidas;

    FILE *ptarq = fopen("PARTIDA.bin", "rb"); // Acessa o arquivo em formato de leitura
    if(!ptarq){  // Caso o arquivo dê algum problema, entra no if indicando o ERRO
        printf("Erro: Nao foi possivel acessar o arquivo PARTIDA.bin!\n");
        exit(1);
    }

    int op, dia, mes, ano;
    char time_futebol[100];
    do {
        printf("\n\n\tEXIBICAO DE PARTIDAS\n");
        printf("-----------------------------------------\n");
        printf("| 1 - Ver Partidas por Data\n");
        printf("| 2 - Ver Partidas por Time\n");
        printf("| 3 - Ver Partidas que um Time nao Venceu\n");
        printf("| 4 - Sair desta aba\n");
        printf("-----------------------------------------\n");

        printf("\nDigite a opcao que deseja: ");
        scanf("%d", &op);

        switch (op){
            case 1:
                printf("\nDigite a data que deseja ver (dia mes ano): ");
                scanf("%d %d %d", &dia, &mes, &ano);
                printf("\n--------------------------------------------------------------------\n");
                printf("\n\tPARTIDAS CADASTRADAS NESSE DIA");

                int encontradoData = 0; //contador para verificar se foi achado ou não
                fseek(ptarq, 0, SEEK_SET); // o ponteiro dentro do arquivo sempre irá apontar para o inicio do arquivo, quando for entrar no loop
                while (fread(&partidas, sizeof(Partidas), 1, ptarq)) {
                    if (dia == partidas.dia.dia && mes == partidas.dia.mes && ano == partidas.dia.ano) {
                        mostrarDetalhesPartida(partidas); // Função auxiliar para mostrar os detalhes da partida
                        encontradoData = 1;
                    }
                }
                // O if verifica se encontraData é 0 (falso), que significa que nenhuma partida foi encontrada para o time especificado
                if (!encontradoData)
                    printf("OBS: Nenhuma partida foi cadastrada com essa data\n");
                break;

            case 2:
                printf("\nDigite o time que deseja ver: ");
                scanf(" %[^\n]s", time_futebol);
                printf("\n--------------------------------------------------------------------\n");
                printf("\n\tPARTIDAS DO TIME %s", time_futebol);

                int encontradoTime = 0;
                fseek(ptarq, 0, SEEK_SET);
                while (fread(&partidas, sizeof(Partidas), 1, ptarq)) {
                    if (strcmp(time_futebol, partidas.timeA) == 0 || strcmp(time_futebol, partidas.timeB) == 0) {
                        mostrarDetalhesPartida(partidas); // Função auxiliar para mostrar os detalhes da partida
                        encontradoTime = 1;
                    }
                }
                // O if verifica se encontraTime é 0 (falso), que significa que nenhuma partida foi encontrada para o time especificado
                if (!encontradoTime)
                    printf("OBS: Esse time nao foi cadastrado em nenhuma partida\n");
                break;

            case 3:
                printf("\nDigite o time que deseja ver: ");
                scanf(" %[^\n]s", time_futebol);
                printf("\n--------------------------------------------------------------------\n");
                printf("\n\tPARTIDAS QUE O TIME %s VENCEU", time_futebol);

                int encontradoSevenceu = 0;
                fseek(ptarq, 0, SEEK_SET);
                while (fread(&partidas, sizeof(Partidas), 1, ptarq)) {
                    //verificação se o time está cadastrado em alguma partida
                    if (strcmp(time_futebol, partidas.timeA) == 0 || strcmp(time_futebol, partidas.timeB) == 0) { // verifica se o time q procurá já foi cadastrado em alguma partida
                        if(strcmp(time_futebol, partidas.timeGanhador) != 0){ // verifica se o time que venceu a partida for diferente do que foi o time que o usuario deseja encontrar, então entra dentro do if
                            if(partidas.terminada == 1){ // verifica se a partida já foi terminada
                                mostrarDetalhesPartida(partidas); // Função auxiliar para mostrar os detalhes da partida
                                encontradoSevenceu = 1;
                            }
                        }
                    }
                }

                // O if verifica se encontraSecenceu é 0 (falso), que significa que o time não teve derrotas ou a partida que ele está jogando, ainda está em andamento
                if (!encontradoSevenceu) {
                    printf("\nOBS: Este time nao foi encontrado, porque a partida ainda esta em andamento ou nao possui derrotas\n");
                }
                break;

            case 4:
                printf("Saindo desta aba...\n");
                break;

            default:
                printf("\nOPCAO INVALIDA\n");
        }
        printf("\n--------------------------------------------------------------------\n");
    } while (op != 4);
    fclose(ptarq);
}

void mostrarDetalhesPartida(Partidas partida) {
    printf("\n_________________________________________");
    printf("\nPartida %d\n", partida.indentificador);
    printf("- Data da Partida: %d / %d / %d \n", partida.dia.dia, partida.dia.mes, partida.dia.ano);
    printf("- Time A e gols: %s - %d\n", partida.timeA, partida.gols_timeA);
    printf("- Time B e gols: %s - %d \n", partida.timeB, partida.gols_timeB);
    printf("- Estadio: %s\n", partida.estadio);
    if(partida.terminada == 0){
        printf("- Situacao da partida: EM ANDAMENTO\n");
    }else{
        printf("- Situacao da partida: TERMINADA\n");
    }

    if (partida.terminada == 0) {
        printf("- Time Ganhador: - \n");
    } else {
        printf("- Time Ganhador: %s\n", partida.timeGanhador);
    }
    printf("_________________________________________\n");
}

void finalizarPartida(int dia, int mes, int ano){
    ler_quantidades();
    FILE *ptarq_partidas = fopen("PARTIDA.bin", "rb"); //abre o arquivo
    if(!ptarq_partidas){
        printf("\nNao foi possivel abrir o arquivo PARTIDA.bin ou nao ha partidas cadastradas!\n");
        exit(1);
    }

    Partidas *partida = (Partidas *)malloc(quantidades[1]*sizeof(Partidas)); //aloca o vetor para receber os dados do arquivo
    if(!partida){
        printf("\nNAO FOI POSSIVEL ALOCAR O VETOR!\n");
        exit(1);
    }

    int cont=0;
    fread(partida, sizeof(Partidas), quantidades[1], ptarq_partidas); //baixa os dados pro vetor
    fclose(ptarq_partidas);
    //exibe as partidas
    for(int i=0; i<quantidades[1]; i++){ //enquanto tiver partida
        if(dia == partida[i].dia.dia && mes == partida[i].dia.mes && ano == partida[i].dia.ano){ //mostra somente partidas daquela data
            if(partida[i].terminada == 0){ //mostra somente as partidas em andamento
                mostrarDetalhesPartida(partida[i]);
                cont = 1;
            }
        }
    }
    if(cont == 0){
        printf("\nNAO EXISTEM PARTIDAS NESSA DATA OU JA ESTAO FINALIZADAS!\n");
        return;
    }

    int posicao;
    printf("\nDigite o numero da partida que deseja finalizar: ");
    scanf("%d", &posicao);

    //se ela ja tiver terminada ou se ela for diferente da data escolhida
    if(partida[posicao].terminada == 1 || (partida[posicao].dia.dia != dia && partida[posicao].dia.mes != mes && partida[posicao].dia.ano != ano))
    {   printf("\nEssa opcao nao esta na lista das partidas em andamento dessa data!\n");
        return;
    }
    partida[posicao].terminada = 1; //muda o status de "Em andamento" para "finalizada"

    int aux=0;
    char timeGanhador;
    while(aux == 0){ //loop para caso o usuario digite errado
        printf("\nDigite a letra do time que ganhou (A ou B): ");
        scanf("\n%c", &timeGanhador);

        if(timeGanhador == 'a' || timeGanhador == 'A'){
            timeGanhador = 'A';
            strcpy(partida[posicao].timeGanhador, partida[posicao].timeA);
            aux=1;
        }else
        if(timeGanhador == 'b' || timeGanhador == 'B'){
            timeGanhador = 'B';
            strcpy(partida[posicao].timeGanhador, partida[posicao].timeB);
            aux=1;
        }else
            printf("\nOPCAO INVALIDA!\n");
    }
    printf("\nPARTIDA FINALIZADA!\n");

    alteraVencedor(partida[posicao].indentificador, timeGanhador); //funcao pra alterar vencedor

    ptarq_partidas = fopen("PARTIDA.bin", "wb"); //abre em escrita para atualizar o arquivo com os novos valores
    fwrite(partida, sizeof(Partidas), quantidades[1], ptarq_partidas);
    fclose(ptarq_partidas);
    free(partida);
}

void alteraVencedor(int identificador, char timeGanhador){
    ler_quantidades();
    FILE *ptarq_apostas = fopen("APOSTAS.bin", "rb"); //abre o arquivo para leitura dos dados
    if(!ptarq_apostas){
        printf("\nNao foi possivel abrir o arquivo APOSTAS.bin!\n");
        exit(1);
    }

    Aposta *aposta = (Aposta *)malloc(quantidades[2]*sizeof(Aposta)); //aloca o vetor dinamico
    if(!aposta){
        printf("\nErro ao alocar o vetor!\n");
        exit(1);
    }
    fread(aposta, sizeof(Aposta), quantidades[2], ptarq_apostas); //baixa os dados para o vetor
    fclose(ptarq_apostas); //fecha o arquivo

    for(int i=0; i<quantidades[2]; i++){ //roda todo o vetor
        if(identificador == aposta[i].identificador_da_partida){ //se a aposta for sobre a partida do identificador
            if(timeGanhador == aposta[i].timeApostado){ //se o time ganhador for o mesmo que o cliente apostou
                aposta[i].vencedor = 1; //atualiza a variavel vencedor
            }
        }
    }
    ptarq_apostas = fopen("APOSTAS.bin", "wb"); //abre em formato de escrita para atualizar os dados modificados;
    fwrite(aposta, sizeof(Aposta), quantidades[2], ptarq_apostas);
    fclose(ptarq_apostas);
    free(aposta);
}
//opcao 3
void inserirAposta(ChaveCliente *chave){
    ler_quantidades();
    Aposta apostas;

    printf("Digite o CPF: ");
    scanf("\n%[^\n]s", apostas.cpf_do_Cliente);

    int encontrado_cpf = 0, encontraData = 0; // variavel que mostra se foi encontrado ou não o CPF e dia

    //percorrer o vetor inteiro até o msm CPF digitado pelo usuario ser encontrado
    for(int i=0; i<quantidades[0]; i++){
       if(strcmp(apostas.cpf_do_Cliente, chave[i].cpf_do_Cliente) == 0){ // verificando se o CPF digitado existe em alguma struct de ChaveCliente

            encontrado_cpf = 1;
            printf("\nCPF ENCONTRADO!\n");

            Partidas partida;

            FILE *ptarq_partidas;
            ptarq_partidas = fopen("PARTIDA.bin", "rb"); // arquivo que recebá as informações de aposta
            if(!ptarq_partidas){
                printf("\nNAO FOI POSSIVEL ABRIR O ARQUIVO PARTIDA.bin!\n");
                return;
            }
            printf("\nDigite a data da partida que deseja apostar (dd mm aaaa): ");
            scanf("%d %d %d", &apostas.data.dia, &apostas.data.mes, &apostas.data.ano);

            int i = 0;
            //procurar a partida no arquivo de partidas
            while(fread(&partida, sizeof(Partidas),1, ptarq_partidas)){
                if(apostas.data.dia == partida.dia.dia && apostas.data.mes == partida.dia.mes && apostas.data.ano == partida.dia.ano && partida.terminada == 0){
                    printf("\n\tPARTIDAS CADASTRADAS NESSE DIA\n");
                    mostrarDetalhesPartida(partida);
                    encontraData = 1; // Ao encontrar a Data cadastrada em alguma partida,o contado irá pra 1, simbolizando q foi encontrado
                    i++;
                }
            }
            //caso Data não seja encontrada, if acionado
            if (encontraData == 0){
                printf("\nDATA NAO FOI CADASTRADA EM NENHUMA PARTIDA!!!\n");
                return;
            }

            printf("\nDigite a partida que deseja Apostar: ");
            scanf("%d", &apostas.identificador_da_partida);

            //verifica se a partida está disponivel para apostar, caso não esteja, o if será acionado
            if(verificarPartidaDisponivel(apostas.identificador_da_partida) == 0){
                printf("A partida nao esta disponivel para apostar ou ja foi finalizada.\n");
                return;
            }
            int op; // variavel pra receber o valor da cotação

            printf("\n\n\tVALORES DA COTACAO:\n\n 1 - Valor 1.5\n 2 - Valor 2.0\n 3 - Valor 2.5\n 4 - Valor 3.0\n 5 - Valor 3.5\n 6 - Valor 4.0\n\n");
            printf("Digite a cotacao da aposta: ");
            scanf("%d", &op);

            switch (op)
            {
            case 1:
                apostas.cotacao = 1.5;
                break;
            case 2:
                apostas.cotacao = 2.0;
                break;
            case 3:
                apostas.cotacao = 2.5;
                break;
            case 4:
                apostas.cotacao = 3.0;
                break;
            case 5:
                apostas.cotacao = 3.5;
                break;
            case 6:
                apostas.cotacao = 4.0;
                break;
            default:
                printf("NAO E PERMITIDO OUTRAS COTACOES!\n");
                break;
            }
            apostas.vencedor = 0;

            printf("Digite o valor que deseja apostar: ");
            scanf("%f", &apostas.valor_aposta);

            printf("Digite em qual time que deseja apostar (A - Time A, B - Time B): ");
            scanf("\n%c", &apostas.timeApostado);
            if(apostas.timeApostado == 'a') //isso aqui é só pra nao dar conflito na hora da comparacao no finalizar partida, lá é comparado com letra maiuscula, entao caso o usuario digite letra minuscula, ele converte em maiuscula
                apostas.timeApostado = 'A';
            else if(apostas.timeApostado == 'b')
                apostas.timeApostado = 'B';

            FILE *ptarq = fopen("APOSTAS.bin", "ab"); // arquivo que recebá as informações de aposta
            if(!ptarq){
                 printf("NAO FOI POSSIVEL ACESSAR O ARQUIVO APOSTAS.bin!\n");
                 return;
            }

            //salvando no arquivo APOSTAS.bin
            quantidades[2] += fwrite(&apostas, sizeof(Aposta), 1, ptarq); //a quantidade de apostas será acrescentada a cada vez que cadastrar uma nova aposta
            fclose(ptarq_partidas);
            fclose(ptarq);

            //atualizações de arquivos
            atualiza_quantidades(); //atualiza o arquivo QUANTIDADES.bin, com as quantidades de apostas alterada
            atualiza_chaveApostas(apostas); //atualiza o arquivo CHAVEAPOSTAS.bin com os novos valores de apostas e posições
            //ordenação do arquivo chaveaposta
            ordena_chaveApostas();

            printf("\n_________________________________________\n");
            printf("\n\tDETALHES DA APOSTA\n");
            printf("\nCPF do cliente: %s", apostas.cpf_do_Cliente);
            printf("\nData da aposta: %d %d %d", apostas.data.dia, apostas.data.mes, apostas.data.ano);
            printf("\nIdentificador da partida: %d", apostas.identificador_da_partida);
            printf("\nCotacao: %.2f", apostas.cotacao);
            printf("\nValor da Aposta: %.2f", apostas.valor_aposta);
            printf("\nTime apostado: %c", apostas.timeApostado);
            printf("\nVencedor: %d\n", apostas.vencedor);
            printf("\n_________________________________________\n\n");
        }
    }
    if(encontrado_cpf == 0){
        printf("CPF nao foi cadastrado!!!\n");
        return;
    }
}

int verificarPartidaDisponivel(int indentifica){
    Partidas partidas;
    FILE *ptarq = fopen("PARTIDA.bin", "rb"); // Abrir o arquivo em modo de leitura
    if(!ptarq){
        printf("\nNAO FOI POSSIVEL ACESSAR O ARQUIVO PARTIDA.bin!\n");
        exit(1);
    }

    //Ler informações da partida de cada struct e verificar se a partida  com o indentificador fornecido está disponivel
    while(fread(&partidas, sizeof(Partidas), 1, ptarq) == 1){
        if(partidas.indentificador == indentifica){
            if(partidas.terminada == 0){
                fclose(ptarq);
                return 1; // Retorna 1, caso a partida esteja em andamento
            }else{
                fclose(ptarq);
                return 0; // Retorna 0 caso a partida já tenha terminado ou não está disponivel
            }
        }
    }
    fclose(ptarq);
    return 0; //  Retornar 0 caso a partida não foi encontrada
}

void ordena_chaveApostas(){
    ler_quantidades();
    int qtd = quantidades[2];

    FILE *ptarq = fopen("CHAVEAPOSTAS.bin", "rb"); //abre o arquivo chaveapostas.bin em modo de leitura
    if(!ptarq) //se o arquivo nao for encontrado
    {   printf("\nArquivo CHAVEAPOSTAS.bin nao foi encontrado para a ordenacao!\n");
        exit(1);
    }
    //aloca um vetor dinamico com a quantidade de apostas do arquivo
    ChaveAposta *chave = (ChaveAposta *)malloc(qtd*sizeof(ChaveAposta));
    if(chave == NULL){
        printf("\nNAO FOI POSSIVEL ALOCAR O VETOR!\n\n");
        exit(1);
    }
    //lê as apostas do arquivo e as salvam no vetor
    fread(chave, sizeof(ChaveAposta), qtd, ptarq);
    fclose(ptarq); //fecha o arquivo

    //ordena o vetor por bubble sort
    ChaveAposta aux;
    if(qtd > 1){//só ordena se tiver mais de um pra ordenar
        for(int i=0; i<qtd; i++){
            for(int j=0; j<qtd-1; j++){
                if(chave[j].valor > chave[j+1].valor)
                {
                    aux = chave[j];
                    chave[j] = chave[j+1];
                    chave[j+1] = aux;
                }
            }
        }
    }
    //escreve os dados organizados no arquivo
    ptarq = fopen("CHAVEAPOSTAS.bin", "wb"); //Abre o arquivo novamente, só que agora pra sobreescrever os dados ordenados
    if(!ptarq){ //se o arquivo nao for encontrado
        printf("\nERRO AO ABRIR O ARQUIVO CHAVEAPOSTAS.bin!\n");
        exit(1);
    }
    fwrite(chave, sizeof(ChaveAposta), qtd, ptarq); //escreve os dados no arquivo
    fclose(ptarq); //fecha o arquivo
    free(chave); //desaloca o vetor
}

void atualiza_chaveApostas(Aposta apostas){
    ler_quantidades();
    ChaveAposta chave;
    FILE *ptarq = fopen("CHAVEAPOSTAS.bin", "ab"); //abrindo arquivo em modo de adição
    if(!ptarq){
        printf("\nERRO AO ABRIR O ARQUIVO CHAVEAPOSTAS.bin!\n");
        exit(1);
    }
    //recebendo dados na struct
    chave.valor = apostas.valor_aposta; //recebe o valor da aposta
    chave.posicao = quantidades[2]-1; //recebe a posicao da aposta

    //mandando dados da struct pro arquivo ChaveApostas
    fwrite(&chave, sizeof(ChaveAposta), 1, ptarq);
    fclose(ptarq);
}
//opcao 4
void relatorios(ChaveCliente *chave){
  int op;
  do{
      printf("\n\t\tMENU DE RELATORIOS\n");
      printf(" __________________________________________________\n");
      printf("| 1 - Todas as apostas de um Apostador             |\n");
      printf("| 2 - Apostas de um ano especifico                 |\n");
      printf("| 3 - Apostas que um Apostador venceu              |\n");
      printf("| 4 - Total ganho e total perdido por um apostador |\n");
      printf("| 5 - Sair desta aba                               |\n");
      printf("|__________________________________________________|\n");

      printf("\nDigite opcao que deseja acessar: ");
      scanf("%d", &op);
    switch(op)
      {
        case 1:
            todasApostas_deUmApostador(chave);
            break;
        case 2:
            apostasAnoEspecifico();
            break;
        case 3:
            apostasGanhadorVenceu(chave);
            break;
        case 4:
            totaisGanhosePerdidos(chave);
            break;
        case 5:
            printf("\nVoltando ao menu principal...\n");
            break;
        default:
            printf("\nOPCAO INVALIDA!!\n");
            break;
      }
  }while(op != 5);
}

void todasApostas_deUmApostador(ChaveCliente *chave){
    ler_quantidades();

    Aposta aposta;
    FILE *ptarq_apostas;
    ptarq_apostas = fopen("APOSTAS.bin", "rb"); // abre apostas em modo de leitura
    if (!ptarq_apostas){
        printf("\nNAO FOI POSSIVEL ABRIR O ARQUIVO APOSTAS.bin!\n");
        exit(1);
    }

    int op;
    Aposta *vetor = (Aposta*) malloc(0 * sizeof(Aposta));

    char cpf_digitado[12];
    printf("\nDigite um CPF para exibir todas as apostas: ");
    scanf("\n%[^\n]s", cpf_digitado);
    if(strlen(cpf_digitado) != 11){ // verificacao para o usuario nao digitar um cpf maior ou menor
        printf("\nCPF INVALIDO. O CPF DEVE CONTER 11 DIGITOS!\n");
        fclose(ptarq_apostas);
        return;
    }

    int posicaoCliente;
    posicaoCliente = buscarClientes(cpf_digitado, quantidades[0], chave); // busca o cpf no arquivo chavecliente atraves da busca binaria retorna -1 se nao tiver cadastrado
    if(posicaoCliente == -1){ // se nao tiver cadastrado
        printf("\nESSE CPF NAO ESTA CADASTRADO!\n");
        fclose(ptarq_apostas);
        return;
    }

    int cpf_encontrado = 0, contador = 0;
    while (fread(&aposta, sizeof(Aposta), 1, ptarq_apostas))
    { // enquanto existir aposta no arquivo
        if (strcmp(cpf_digitado, aposta.cpf_do_Cliente) == 0){ // se o cpf digitado for igual ao cpf do cliente da aposta
            vetor = (Aposta*) realloc(vetor, (contador + 1) * sizeof(Aposta));
            if(vetor == NULL){
                printf("\nERRO AO ALOCAR MEMORIA!\n");
                fclose(ptarq_apostas);
                return;
            }
            strcpy(vetor[contador].cpf_do_Cliente, aposta.cpf_do_Cliente);
            vetor[contador].data.dia = aposta.data.dia;
            vetor[contador].data.mes = aposta.data.mes;
            vetor[contador].data.ano = aposta.data.ano;
            vetor[contador].identificador_da_partida = aposta.identificador_da_partida;
            vetor[contador].cotacao = aposta.cotacao;
            vetor[contador].valor_aposta = aposta.valor_aposta;
            vetor[contador].timeApostado = aposta.timeApostado;
            vetor[contador].vencedor = aposta.vencedor;

            cpf_encontrado = 1;
            contador++;

        }
    }
    fclose(ptarq_apostas);
    if(cpf_encontrado == 0){
        printf("\nESSE CPF NAO FEZ NENHUMA APOSTA!\n");
        free(vetor);
        return;
    }

    printf("\n----------------------------------------------------------\n");
    printf("\n\n\tO QUE VOCE DESEJA FAZER?\n\n\t 1 - Mostrar dados na tela\n\t 2 - Salvar no arquivo\n\n");
    scanf("%d", &op);

    if(op == 1){
        printf("\n\tTODAS AS APOSTAS DESSE APOSTADOR\n");
        for(int i = 0; i < contador; i++){
            printf("\n _______________________________\n");
            printf("| CPF: %s              |\n", vetor[i].cpf_do_Cliente);
            printf("| Data: %d / %d / %d          |\n", vetor[i].data.dia, vetor[i].data.mes, vetor[i].data.ano);
            printf("| Indentificador da partida: %d  |\n", vetor[i].identificador_da_partida);
            printf("| Cotacao: %.2f                 |\n", vetor[i].cotacao);
            printf("| Valor da aposta: %.2f        |\n", vetor[i].valor_aposta);
            printf("| Time Apostado: %c              |\n", vetor[i].timeApostado);
            printf("| Vencedor: %d                   |\n", vetor[i].vencedor);
            printf("|_______________________________|\n");
        }
    }else
    if(op == 2){
        FILE *ptarq;
        ptarq = fopen("RELATORIO.txt", "a");
        if(ptarq == NULL){
            printf("\nERRO AO ABRIR ARQUIVO!\n");
            return;
        }

    fprintf(ptarq, "CPF, Data, Identificador da partida, Cotacao, Valor da Aposta, Time apostado, Vencedor\n");
    for(int i = 0; i < contador; i++){
        fprintf(ptarq, "%s , %d / %d / %d , %d, %.2f, %.2f, %c, %d \n\n", vetor[i].cpf_do_Cliente,vetor[i].data.dia, vetor[i].data.mes, vetor[i].data.ano,vetor[i].identificador_da_partida, vetor[i].cotacao,vetor[i].valor_aposta,vetor[i].timeApostado,vetor[i].vencedor);
    }

    printf("\nREGISTROS SALVOS COM SUCESSO!!!\n");
        fclose(ptarq);
    }else{
        printf("\nOPCAO INVALIDA!\n");
    }
    free(vetor);
    printf("\n----------------------------------------------------------\n");
}

void apostasAnoEspecifico(){
    int ano,op;
    Aposta aposta;
    Aposta *vetor = NULL;

    printf("\nDigite um ano para exibicao: ");
    scanf("%d", &ano);

    FILE *ptarq_apostas = fopen("APOSTAS.bin", "rb");
    if(!ptarq_apostas){
        printf("\nNAO FOI POSSIVEL ABRIR O ARQUIVO APOSTAS.bin!\n");
        exit(1);
    }

    int contador = 0;
    while(fread(&aposta, sizeof(Aposta), 1, ptarq_apostas)){ //mostrando todas as aposta uma a uma
        if(ano == aposta.data.ano){ //se o ano digitado for igual o da aposta
            vetor = (Aposta*) realloc(vetor, (contador + 1) * sizeof(Aposta));
            if(!vetor){
                printf("\nERRO AO ALOCAR MEMORIA!\n");
                fclose(ptarq_apostas);
                return;
            }
            strcpy(vetor[contador].cpf_do_Cliente, aposta.cpf_do_Cliente);
            vetor[contador].data.dia = aposta.data.dia;
            vetor[contador].data.mes = aposta.data.mes;
            vetor[contador].data.ano = aposta.data.ano;
            vetor[contador].identificador_da_partida = aposta.identificador_da_partida;
            vetor[contador].cotacao = aposta.cotacao;
            vetor[contador].valor_aposta = aposta.valor_aposta;
            vetor[contador].timeApostado = aposta.timeApostado;
            vetor[contador].vencedor = aposta.vencedor;
            contador++;
        }
    }
    fclose(ptarq_apostas);

    printf("\n----------------------------------------------------------\n");
    printf("\n\n\tO QUE VOCE DESEJA FAZER?\n\n\t 1 - Mostrar dados na tela\n\t 2 - Salvar no arquivo\n\n");
    scanf("%d", &op);
    if(op == 1){
        printf("\n\tAPOSTAS DO ANO %d\n", ano);
        for(int i = 0; i < contador; i++){
            printf("\n _______________________________\n");
            printf("| CPF: %s              |\n", vetor[i].cpf_do_Cliente);
            printf("| Data: %d / %d / %d          |\n", vetor[i].data.dia, vetor[i].data.mes, vetor[i].data.ano);
            printf("| Indentificador da partida: %d  |\n", vetor[i].identificador_da_partida);
            printf("| Cotacao: %.2f                 |\n", vetor[i].cotacao);
            printf("| Valor da aposta: %.2f        |\n", vetor[i].valor_aposta);
            printf("| Time Apostado: %c              |\n", vetor[i].timeApostado);
            printf("| Vencedor: %d                   |\n", vetor[i].vencedor);
            printf("|_______________________________|\n");
        }
    }else
    if(op == 2){
        FILE *ptarq = fopen("RELATORIO.txt", "a");
        if(!ptarq){
            printf("ERRO AO ABRIR O ARQUIVO RELATORIO.bin!\n");
            return;
        }
        fprintf(ptarq, "CPF, Data, Identificador da partida, Cotacao, Valor da Aposta, Time apostado, Vencedor\n");
        for(int i = 0; i < contador; i++){
            fprintf(ptarq, "%s , %d / %d / %d , %d, %.2f, %.2f, %c, %d \n\n", vetor[i].cpf_do_Cliente,vetor[i].data.dia, vetor[i].data.mes, vetor[i].data.ano,vetor[i].identificador_da_partida, vetor[i].cotacao,vetor[i].valor_aposta,vetor[i].timeApostado,vetor[i].vencedor);
        }
        printf("\nREGISTROS SALVOS COM SUCESSO!!!\n");
        fclose(ptarq);
    }else{
        printf("OPCAO INVALIDA\n");
    }
    free(vetor);
    printf("\n----------------------------------------------------------\n");
}

void apostasGanhadorVenceu(ChaveCliente *chave){
    ler_quantidades();
    Aposta aposta;

    FILE *ptarq = fopen("APOSTAS.bin", "rb"); //abre o arquivo
    if(!ptarq){
        printf("\nNAO FOI POSSIVEL ABRIR O ARQUIVO APOSTAS.bin!\n");
        exit(1);
    }

    Aposta *vetor = NULL; //aloca o vetor

    char cpf_digitado[12];
    printf("\nDigite um CPF para exibir todas as apostas: ");
    scanf("\n%[^\n]s", cpf_digitado);
    if(strlen(cpf_digitado) != 11){ // verificacao para o usuario nao digitar um cpf maior ou menor
        printf("\nCPF INVALIDO. O CPF DEVE CONTER SOMENTE 11 DIGITOS!\n");
        fclose(ptarq);
        return;
    }

    int posicaoCliente;
    posicaoCliente = buscarClientes(cpf_digitado, quantidades[0], chave); // busca o cpf no arquivo chavecliente atraves da busca binaria retorna -1 se nao tiver cadastrado
    if (posicaoCliente == -1){ // se nao tiver cadastrado
        printf("\nESSE CPF NAO ESTA CADASTRADO!\n");
        fclose(ptarq);
        return;
    }

    int contador=0;
    while(fread(&aposta, sizeof(Aposta), 1, ptarq)){
        if(strcmp(cpf_digitado, aposta.cpf_do_Cliente) == 0){
            if(aposta.vencedor == 1){

                vetor = (Aposta *)realloc(vetor, (contador+1)*sizeof(Aposta));
                if(vetor == NULL){
                    printf("\nNAO FOI POSSIVEL ALOCAR O VETOR!\n");
                    fclose(ptarq);
                    return;
                }

                strcpy(vetor[contador].cpf_do_Cliente, aposta.cpf_do_Cliente);
                vetor[contador].data.dia = aposta.data.dia;
                vetor[contador].data.mes = aposta.data.mes;
                vetor[contador].data.ano = aposta.data.ano;
                vetor[contador].identificador_da_partida = aposta.identificador_da_partida;
                vetor[contador].cotacao = aposta.cotacao;
                vetor[contador].valor_aposta = aposta.valor_aposta;
                vetor[contador].timeApostado = aposta.timeApostado;
                vetor[contador].vencedor = aposta.vencedor;
                contador++;
            }
        }
    }
        fclose(ptarq);

        int op;
        printf("\n----------------------------------------------------------\n");
        printf("\n\n\tO QUE VOCE DESEJA FAZER?\n\n\t 1 - Mostrar dados na tela\n\t 2 - Salvar no arquivo\n\n");
        scanf("%d", &op);

        if(op == 1){
            printf("\n\tTODAS AS APOSTAS QUE O CLIENTE GANHOU\n");
            for(int i = 0; i < contador; i++){
                printf("\n _______________________________\n");
                printf("| CPF: %s              |\n", vetor[i].cpf_do_Cliente);
                printf("| Data: %d / %d / %d          |\n", vetor[i].data.dia, vetor[i].data.mes, vetor[i].data.ano);
                printf("| Indentificador da partida: %d  |\n", vetor[i].identificador_da_partida);
                printf("| Cotacao: %.2f                 |\n", vetor[i].cotacao);
                printf("| Valor da aposta: %.2f        |\n", vetor[i].valor_aposta);
                printf("| Time Apostado: %c              |\n", vetor[i].timeApostado);
                printf("| Vencedor: %d                   |\n", vetor[i].vencedor);
                printf("|_______________________________|\n");
            }
        }else
        if(op == 2){
            FILE *ptarq;
            ptarq = fopen("RELATORIO.txt", "a");
            if(!ptarq){
                printf("\nERRO AO ABRIR O ARQUIVO RELATORIO.bin!\n");
                return;
            }

        fprintf(ptarq, "CPF, Data, Identificador da partida, Cotacao, Valor da Aposta, Time apostado, Vencedor\n");
        for(int i = 0; i < contador; i++){
            fprintf(ptarq, "%s , %d / %d / %d , %d, %.2f, %.2f, %c, %d \n\n", vetor[i].cpf_do_Cliente,vetor[i].data.dia, vetor[i].data.mes, vetor[i].data.ano,vetor[i].identificador_da_partida, vetor[i].cotacao,vetor[i].valor_aposta,vetor[i].timeApostado,vetor[i].vencedor);
        }
        printf("\nREGISTROS SALVOS COM SUCESSO!!!\n");
        fclose(ptarq);

        }else{
            printf("OPCAO INVALIDA\n");
        }
        free(vetor);
        printf("\n----------------------------------------------------------\n");
}

void totaisGanhosePerdidos(ChaveCliente *chave){
    ler_quantidades();

    Aposta aposta;
    FILE *ptarq_apostas = fopen("APOSTAS.bin", "rb"); // abre apostas em modo de leitura
    if (!ptarq_apostas){
        printf("\nNAO FOI POSSIVEL ABRIR O ARQUIVO APOSTAS.bin!\n");
        exit(1);
    }

    Partidas partida;
    FILE *ptarq_partidas;
    ptarq_partidas = fopen("PARTIDA.bin", "rb");
    if (!ptarq_partidas){
        printf("\nNAO FOI POSSIVEL ABRIR O ARQUIVO PARTIDA.bin!\n");
        exit(1);
    }

    int op;
    char cpf_digitado[12];

    printf("\nDigite um CPF para calcular os ganhos e perdas: ");
    scanf("\n%[^\n]s", cpf_digitado);
    if(strlen(cpf_digitado) != 11){ // verificacao para o usuario nao digitar um cpf maior ou menor
        printf("\nCPF INVALIDO. O CPF DEVE CONTER APENAS 11 DIGITOS!\n");
        fclose(ptarq_apostas);
        fclose(ptarq_partidas);
        return;
    }

    int posicaoCliente;
    posicaoCliente = buscarClientes(cpf_digitado, quantidades[0], chave); // busca o cpf no arquivo chavecliente atraves da busca binaria retorna -1 se nao tiver cadastrado
    if (posicaoCliente == -1){ // se nao tiver cadastrado
        printf("\nESSE CPF NAO ESTA CADASTRADO!\n");
        fclose(ptarq_apostas);
        fclose(ptarq_partidas);
        return;
    }

    float ganho = 0, perdas = 0;
    int cpf_encontrado = 0;

    //somatorio dos ganhos e perdas
    while(fread(&aposta, sizeof(Aposta), 1, ptarq_apostas)){ //enquanto existir aposta
        if(strcmp(cpf_digitado, aposta.cpf_do_Cliente) == 0){//se a aposta for daquele cpf digitado

            int posicao = aposta.identificador_da_partida; //a aposta foi feita em cima de uma partida, identificador_da_partida é praticamente a posicao dela no arquivo partidas. Entao posicao recebe ela
            fseek(ptarq_partidas, posicao*sizeof(Partidas), SEEK_SET); //o cursor vai para a posicao da partida da aposta no arquivo "partidas"
            fread(&partida, sizeof(Partidas), 1, ptarq_partidas); //puxa somente essa partida para a struct partida

            if(partida.terminada == 1){//se a partida finalizou
                if(aposta.vencedor == 1){ // se ele ganhar
                    ganho += (aposta.valor_aposta*aposta.cotacao); //multiplica o valor investido pela cotacao
                }else if(aposta.vencedor == 0){ // se ele perder
                    perdas += aposta.valor_aposta; //perde o que investiu
                }
            }
        }
        cpf_encontrado = 1;
    }
    fclose(ptarq_apostas);
    fclose(ptarq_partidas);

    if (cpf_encontrado == 0){
        printf("\nESSE CPF NAO FEZ NENHUMA APOSTA!\n");
        return;
    }
    printf("\n----------------------------------------------------------\n");
    printf("\n\n\tO QUE VOCE DESEJA FAZER?\n\n\t 1 - Mostrar dados na tela\n\t 2 - Salvar no arquivo\n\n");
    scanf("%d", &op);

    if(op == 1){
        printf("\n\tGANHOS E PERDAS DO CLIENTE\n");
        printf("\n ________________________________\n");
        printf("| TOTAL GANHO   | %.2f R$\n", ganho);
        printf("| TOTAL PERDIDO | %.2f R$\n", perdas);
        printf("|_______________|________________|\n\n");
    }else
    if(op == 2){
        FILE *ptarq;
            ptarq = fopen("RELATORIO.txt", "a");
            if(!ptarq){
                printf("\nERRO AO ABRIR O ARQUIVO RELATORIO.bin!\n");
                return;
            }
        fprintf(ptarq, "CPF : %s\n", cpf_digitado);
        fprintf(ptarq, "TOTAL GANHO: %.2f\n", ganho);
        fprintf(ptarq, "TOTAL PERDIDO: %.2f\n", perdas);

        printf("\nREGISTROS SALVOS COM SUCESSO!!!\n");
        fclose(ptarq);
    }else{
        printf("\nOPCAO INVALIDA\n");
    }
    printf("\n----------------------------------------------------------\n");
}
//Funções auxiliares
ChaveCliente* baixaChaveCliente(ChaveCliente *chave){
    ler_quantidades();

    FILE *ptarq = fopen("CHAVECLIENTE.bin", "rb");
    if(ptarq){ //se o arquivo existir
        chave = (ChaveCliente *)calloc(quantidades[0],sizeof(ChaveCliente)); //cria o vetor dinamico para receber chavecliente
        fread(chave, sizeof(ChaveCliente), quantidades[0], ptarq);
        fclose(ptarq);
    }
    return chave;
}

void sobeChaveCliente(ChaveCliente *chave){
    FILE *ptarq = fopen("CHAVECLIENTE.bin", "wb");
    if(!ptarq){ //se o arquivo nao for encontrado
      printf("\n\nNAO FOI POSSIVEL ABRIR O ARQUIVO!\n\n");
      exit(1);
    }
    fwrite(chave, sizeof(ChaveCliente), quantidades[0], ptarq);
    fclose(ptarq);
}

void ler_quantidades(){
  FILE *ptarq = fopen("QUANTIDADES.bin", "rb");
  if(ptarq){ //se o arquivo existir
      fread(quantidades, sizeof(int), 3, ptarq); //copia os dados do arquivo para o vetor quantidades
      fclose(ptarq);
  }
}

void atualiza_quantidades(){
  FILE *ptarq = fopen("QUANTIDADES.bin", "wb");
  if(!ptarq){ //se o arquivo nao for encontrado
      printf("\n\nNAO FOI POSSIVEL ABRIR O ARQUIVO!\n\n");
      exit(1);
  }
  fwrite(quantidades, sizeof(int), 3, ptarq);
  fclose(ptarq);
}

void mostra_chaveCliente(ChaveCliente *chave){
    ler_quantidades();
    int qtd_clientes = quantidades[0];

    for(int i=0; i<qtd_clientes; i++){
        printf("\n __________________\n");
        printf("| CHAVE %d:         |\n", i);
        printf("| CPF: %s |\n", chave[i].cpf_do_Cliente);
        printf("| Posicao: %d       |\n", chave[i].posicao);
        printf("|__________________|\n");
    }
}

ChaveCliente* atualizaChaveCliente(Cliente cliente, ChaveCliente *chave){
    ler_quantidades();
    int posicao_insercao = -1;
    int qtdClientes = quantidades[0];

    printf("\nAntes da ordenacao:");
    mostra_chaveCliente(chave);

    if(qtdClientes == 1){//se existir aoenas um cliente cadastrado
        strcpy(chave[0].cpf_do_Cliente, cliente.cpf); //copia o cpf do cliente para o vetor chavecliente
        chave[0].posicao = 0;
    }else
    if(qtdClientes > 1){
        ChaveCliente *novaChave = (ChaveCliente*)realloc(chave, qtdClientes*sizeof(ChaveCliente)); //realoca espaco para mais um cliente
        if(!novaChave){
            printf("\nNao foi possivel alocar memoria!\n");
            exit(1);
        }

        chave = novaChave;

        int inicio = 0, meio, fim = qtdClientes-2;
        while(inicio <= fim){
            meio = (inicio+fim)/2;

            if(strcmp(cliente.cpf, chave[meio].cpf_do_Cliente) == 0){//se o cpf do cliente for igual ao cpf do vetor
                exit(1);
            }else
            if(strcmp(cliente.cpf, chave[meio].cpf_do_Cliente) > 0) {//se o cpf do cliente for maior que o cpf do vetor
                inicio = meio+1;
                posicao_insercao = inicio;
            }else {//se o cpf do cliente for menor que o cpf do vetor
                fim = meio-1;
                posicao_insercao = meio;
            }
        }
        if(posicao_insercao != -1) {//se o cpf nao existir na lista
            for(int i=qtdClientes-1; i>posicao_insercao; i--){
                chave[i] = chave[i-1];
            }
            strcpy(chave[posicao_insercao].cpf_do_Cliente, cliente.cpf); //copia o cpf do cliente para o vetor chavecliente
            chave[posicao_insercao].posicao = quantidades[0]-1;
        }
    }
    printf("\nDepois da ordenacao:");
    mostra_chaveCliente(chave);

    return chave;
}

void exibeDadosDeUmCliente(int posicao){
    Cliente clienteProcurado;

    FILE *ptarq = fopen("CADASTROS.bin", "rb"); //abrindo o arquivo CHAVECLIENTE.bin em modo de leitura
    if(!ptarq){
        printf("\nNao foi possivel abrir o arquivo!\n");
        exit(1);
    }
    fseek(ptarq, posicao*sizeof(Cliente), SEEK_SET); //Posiciona o cursor do arquivo na posicao onde estao os dados do cliente
    fread(&clienteProcurado, sizeof(Cliente), 1, ptarq); //pega os dados do arquivo e coloca na struct clientes
    fclose(ptarq); //fecha o arquivo

    printf("\n|_____________________|________________________________________________\n");
    printf("| CPF      | %s\n", clienteProcurado.cpf);
    printf("| Nome     | %s\n", clienteProcurado.nome);
    printf("| Telefone | %s\n", clienteProcurado.telefone);
    printf("| Renda    | %.2f\n", clienteProcurado.renda);
    printf("| Endereco | %s, %s, %s, %s, %s\n", clienteProcurado.rua, clienteProcurado.numero, clienteProcurado.bairro, clienteProcurado.cidade, clienteProcurado.estado);
    printf("|__________|___________________________________________________________\n");
}
