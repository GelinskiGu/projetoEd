1 - Inicialização:
    Carrega todas as figurinhas do arquivo 'figurinhas_entrada.txt', compra um novo album, e as insere no album comprado, separando as repetidas no album de repetidas.
    Após isso, a lista encadeada que está as informações do 'figurinhas_entrada.txt' é desalocada, por isso é possivel inicializar o programa apenas uma vez.

2 - Comprar novo album:
    Compra um novo album adicionando 12 reais nos gastos. Não é necessário fazer a inicialização antes de comprar um novo álbum. Caso seja comprado um álbum antes da inicialização
    esse álbum será de número 1 e quando for feita a inicialização irá se criar um álbum de chave 2, colando as repetidas no álbum 1.

3 - Comprar pacote figurinha:
    Gera 10 números aleatórios que são guardados em um vetor dinâmico, em que os primeiros 5 números são para o código da seleção (de 1 a 32) e os 5 últimos são para o número dos jogadores (de 1 a 19), ou seja, gera 5 figurinhas e é adicionado 4 reais ao gasto. Caso seja comprado a quantidade necessária para gerar uma figurinha extra essa figurinha é gerada. É dada a opção para o usuário em qual álbum se deseja colar as figurinhas e, também é mostrado as 5 figurinhas geradas aleatóriamente. Caso no álbum exista alguma figurinha aleatória, será procurado em qual pode se colar essa figurinha, se ela já estiver presente em todos os álbuns, a figurinha repetida vai para o álbum de figurinhas repetidas, onde é possível ter figurinhas repetidas.

4 - Vender album:
    Primeiramente checa em todos os álbuns se existe algum completo e, então, mostra-se o valor de cada álbum. É dada a opção para o usuário para escolher qual álbum se quer vender (ou se não quer vender nenhum pressionando '0'). O álbum que o usuário escolher é alocado em outra cabeça, que é a cabeça de álbuns e figurinhas repetidas e, então desalocado e desencadeado da lista em que estava. O lucro é registrado ao final. A chave de novos álbuns gerados não serão mudadas e seguirão a ordem padrão. Por exemplo: o usuário tem o álbum 1, 2 e 3, ao vender o álbum 2 e comprar um novo álbum, será gerado um álbum de chave 4. Em cabeça vendidos, se manterá a mesma chave do álbum que estava na cabeça.

5 - Vender figurinhas repetidas:
    É mostrado na tela todas as figurinhas repetidas e o usuário digita o código da seleção e o número do jogador em que se quer vender. A figurinha então é encadeada no cabecaVendidos->repetidas e desalocada da lista em que estava, registrando-se o lucro.

6 - Relatório de gastos:
    É mostrado o gasto total e o gasto por cada álbum que o usuário POSSUI. Portanto, é possível que o gasto total seja diferente da soma dos gastos de cada álbum, pois o valor vendido em cada álbum não é subtraído do gasto total, e os álbuns vendidos não serão mostrados no relatório de gastos.

7 - Relatório de lucros:
    É mostrado o lucro total, os álbuns que foram vendidos e as figurinhas repetidas que foram vendidas.

8 - Imprimir todos os álbuns:
    É uma funcionalidade a mais feita para ficar mais fácil de ver o que está acontecendo com cada álbum. Imprime todos os álbuns que possui, inclusive o álbum de figurinhas repetidas.

9 - Sair:
    Primeiramente é criado um arquivo que possui o relatório de cada álbum que o usuário possui, sendo que esse arquivo é um .txt que tem seu nome mudado dinamicamente. Por exemplo, o álbum 1 irá ser salvo em 'album1.txt', o álbum 'n' irá ser salvo em 'albumn.txt'. Isso pois como o arquivo é aberto no modo "write", caso o nome não mudasse dinamicamente os arquivos iriam ser sobrescrevidos. Após isso é criado o arquivo de figurinhas repetidas, chamado de 'figurinhas_repetidas.txt' (caso o usuário possua figurinhas repetidas). Então é criado o arquivo que armazena o relatório de gastos/lucros. Ao fim, toda memória é desalocada, é desalocado todas as figurinhas de dentro dos álbuns, é desalocado todos os álbuns dentro da cabeça e, por fim, é desalocado todas as cabeças criadas juntamente com as listas encadeadas auxiliares como a lista que armazenava todas as seleções e a lista que armazenava todos os jogadores. Caso queira ver o desalocamento de memória sendo realizado, após cada chamada de função que desaloca a memória no case 9 do switch case tem um printf comentado, se descomentar será possível ver a memória sendo desalocada.

O programa funciona perfeitamente, todos os casos estão protegidos de bugs ou falhas de segmentação.
Todas as exigências pedidas no trabalho foram atendidas.