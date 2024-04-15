# Casa de Apostas
## Sistema de apostas simples em jogos de futebol ⚽

- Sistema em linguagem C, com uso de arquivos para o trabalho final da disciplina de **Algoritmos e Programação** do 2º período

## Documentação 📄

- **Atributos do Cliente:** CPF, Nome, Telefone, Renda, Rua, Número, Bairro, Cidade, Estado
- **Atributos da Partida:** Identificador da Partida, Data, Time A, Time B, Estádio, Gols Time A, Gols Time B, Terminada
- **Atributos da Aposta:** CPF do Cliente, Data, Valor da Aposta, Identificador da Partida, Time Apostado, Vencedor, Cotação
- **ChaveCliente:** CPF do Cliente, Posição no arquivo de clientes

> Aposta está associada a uma Partida através do Identificador da Partida

#
### Variáveis:

#### 🌐 Globais: 
- **_int quantidades[3]_** : É um vetor de 3 posições responsável por receber a quantidade de clientes, partidas e apostas. Onde cada posição representa a quantidade de um dos 3 acima.
  
  - _quantidade[0]_ : guarda a quantidade de clientes
  - _quantidade[1]_ : guarda a quantidade de partidas
  - _quantidade[2]_ : guarda a quantidade de apostas

Esse vetor é inicializado com zero no início do programa, e ao decorrer dele, sempre será atualizado com novos valores vindo do arquivo QUANTIDADES.bin

#### 🏠 Locais:
 > _FILE *ptarq_: É um ponteiro de arquivo, todas as funções usam esse mesmo nome para o ponteiro que recebe o arquivo através da função fopen.

#
### Funções 📌

- _void menu()_ : Exibe o menu principal do programa.

- _void controle_de_clientes()_ : Exibe um submenu para a opção 1 do menu principal.

- _void novoCliente()_ : É responsável por abrir/criar o arquivo CADASTROS.bin em modo de adição. Recebe todos os dados de um cliente em uma struct do tipo Cliente e escreve essa struct no arquivo CADASTROS.bin. Ao mesmo tempo, ela atualiza os arquivos CHAVECLIENTE.bin e QUANTIDADES.bin através de outras duas funções (void atualiza_chaveCliente() e void atualiza_quantidades()). Por fim, ela ordena os clientes do arquivo CHAVECLIENTE.bin através de mais uma função (void ordena_chaveCliente()).

- _void mostrarTodos()_ : Exibe todos os clientes do arquivo CADASTROS.bin através de um loop que percorre todo o arquivo escrevendo os dados em uma struct do tipo Cliente e exibindo-as na tela.

- _void partidas()_ : Exibe um submenu para a opção 2 do menu principal.

- _void apostas()_ : Exibe um submenu para a opção 3 do menu principal.

- _void relatorios()_ : Exibe um submenu para a opção 4 do menu principal.

- _void ler_quantidades()_ : É responsável por abrir o arquivo QUANTIDADES.bin em modo de leitura, pegar os dados dele e escrever na variável global int quantidades[]. Ela atribui um inteiro do arquivo para cada posição do vetor como especificado no tópico “Variáveis”. Caso não exista o arquivo QUANTIDADES.bin, a função não fará nada e retornará para função que a chamou.

- _void atualiza_quantidades()_ : É responsável por abrir/criar o arquivo QUANTIDADES.bin em modo de escrita, e sobrescrever os dados da variável global int quantidades[] nele, atualizando os dados no arquivo.

- _void ordena_chaveCliente()_ : //Ainda falta escrever

- _void NovaPartida()_ : //Ainda falta escrever

- _void mostra_chaveCliente()_ : É responsável por percorrer todo o arquivo CHAVECLIENTE.bin  salvando seus dados em um vetor dinamicamente e exibindo-os na tela. Essa função só serve para uso em testes durante o processo de desenvolvimento do código, para ver se o arquivo CHAVECLIENTE.bin está sendo ordenado corretamente.

- _void atualiza_chaveCliente()_ : Quando chamada na função “void novoCliente()”, recebe a struct “cliente” como parâmetro, abre o arquivo CHAVECLIENTE.bin  em modo de adição, copia o CPF e a posição do cliente no arquivo CADASTROS.bin.
#
