#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct figurinha // Essa vai ser a struct que vai entrar nos albuns
{
    int cod_selecao;
    char selecao[20]; // pais
    int numero_jogador;
    char nome[20];
    struct figurinha *prox;
} TFig;

typedef struct selecoes // Struct para leitura de selecoes do arquivo selecoes.txt
{
    int cod_selecao;
    char selecao[20];
    struct selecoes *prox;
} TSelecao;

typedef struct figurinhaJogador // Struct para leitura de jogadores do arquivo figurinhas_total.txt
{
    int cod_selecao;
    int chave;
    char nome[20];
    struct figurinhaJogador *prox;
} TJogador;

typedef struct album // Struct para fazer lista de albuns, e cada album ter suas figurinhas
{
    int chave;
    float gasto;
    struct album *prox;
    TFig *fig;
} TAlbum;

typedef struct cabecaAlbum // Struct cabeca para albuns e um album para figurinhas repetidas
{
    TAlbum *inicio;
    TAlbum *fim;
    TAlbum *repetidas;
} TCabeca;

TCabeca *criaCabecaAlbum();
TAlbum *criaAlbum(int k);
void alocaAlbum(TCabeca *cabecaAlbum, int op, int k);
TSelecao *alocaSelecao(FILE *parquivo);
TFig *alocaFigurinhaAlbum(TSelecao *structSelecao, TJogador *structJogador);
void insereFigurinhaAlbum(TAlbum **album, int selecao, int jogador, TSelecao *listaselec, TJogador *listajog, TAlbum **repetidas);
TJogador *alocaFigurinhaJogador(FILE *parquivo);
TFig *alocaFigurinhaJogadorEntrada(FILE *parquivo);
void lerSelecoes(FILE *parquivo, TSelecao **prim);
void lerFigurinhasJogadores(FILE *parquivo, TJogador **prim);
void lerFigurinhasJogadoresEntrada(FILE *parquivo, TFig **prim);
void imprimeListaSelecoes(TSelecao *p);
void imprimeListaFigurinhasJogadores(TJogador *p);
void imprimeAlbum(TFig *album);
TSelecao *buscaSelecao(TSelecao *listaselec, int selec);
TJogador *buscaJogador(TJogador *listajog, int jogador, int selecao);
TAlbum *buscaAlbum(TAlbum *album, int k);
void gerarNumero(int cont, int *p);
void insereFigurinhaAlbumRepetida(TAlbum **repetida, TFig *novo);
void buscaFigurinhasRepetidas(TAlbum *album, TFig **figurinha, TSelecao *listaselec, TJogador *listajog, TAlbum **repetidas, TCabeca *cabeca);
void venderAlbum(TAlbum *album, TCabeca **cabeca);
int checarAlbumCompleto(TAlbum *album);
void removeFigurinhasAlbum(TFig **figurinha);
void removeFigurinha(TAlbum **figurinha, int cod, int jogador);
void removeCabeca(TCabeca **cabeca);
void desalocaTSelecao(TSelecao **selecao);
void desalocaTJogador(TJogador **jogador);
void criaArquivoAlbum(TAlbum *album);
void criaArquivoAlbumRepetidas(TAlbum *album);
void criaArquivoGastosELucros(float gastos, float lucros);

int main()
{
    int op = 0, op2 = 0, opc;
    int selec = 32, jog = 1, i = 0, cont = 0, *p = NULL, contadorExtra = 1, contadorAlbum = 1;
    float gastos = 0.0, lucros = 0.0;

    FILE *arquivoSelecao, *arquivoFigurinhaJogador, *arquivoFigurinhaJogadorEntrada;
    arquivoSelecao = fopen("selecoes.txt", "r");
    arquivoFigurinhaJogador = fopen("figurinhas_total.txt", "r");
    arquivoFigurinhaJogadorEntrada = fopen("figurinhas_entrada.txt", "r");
    TSelecao *primSelecao = NULL;
    TJogador *primJogador = NULL;
    TFig *primJogadorEntrada = NULL;
    while (!feof(arquivoSelecao))
        lerSelecoes(arquivoSelecao, &primSelecao);
    fclose(arquivoSelecao);
    while (!feof(arquivoFigurinhaJogador))
        lerFigurinhasJogadores(arquivoFigurinhaJogador, &primJogador);
    fclose(arquivoFigurinhaJogador);
    while (!feof(arquivoFigurinhaJogadorEntrada))
        lerFigurinhasJogadoresEntrada(arquivoFigurinhaJogadorEntrada, &primJogadorEntrada);
    fclose(arquivoFigurinhaJogadorEntrada);

    TCabeca *cabeca = NULL;
    TCabeca *cabecaVendidos = NULL;
    cabeca = criaCabecaAlbum();
    cabecaVendidos = criaCabecaAlbum();

    // gastos += 12;
    alocaAlbum(cabeca, 0, -1);
    p = (int *)malloc(sizeof(int) * 10);
    /*
    for (int i = 1; i <= 32; i++)
    {
        for (int j = 1; j <= 20; j++)
        {
            insereFigurinhaAlbum(&(cabeca->fim), i, j, primSelecao, primJogador, &(cabeca->repetidas));

        }
    }
    */
    while (op != 9)
    {
        printf("1-Inicializacao\n2-Comprar novo album\n3-Comprar pacote figurinha\n4-Vender album\n5-Vender figurinhas repetidas\n6-Relatorio de gastos\n7-Relatorio de lucros\n8-Imprimir todos os albuns\n9-Sair\nDigite a opcao que deseja: ");
        scanf("%d", &op);
        switch (op)
        {
        case 1:
        {
            printf("\n");
            if (primJogadorEntrada)
            {
                alocaAlbum(cabeca, 1, contadorAlbum);
                gastos += 12;
                contadorAlbum++;
                TFig *aux = NULL;
                aux = primJogadorEntrada;
                while (aux)
                {
                    insereFigurinhaAlbum(&(cabeca->fim), aux->cod_selecao, aux->numero_jogador, primSelecao, primJogador, &(cabeca->repetidas));
                    gastos += 0.8;
                    aux = aux->prox;
                }
                aux = NULL;
                removeFigurinhasAlbum(&primJogadorEntrada);
                printf("Figurinhas coladas!\n");
                printf("\n");
            }
            else
            {
                printf("Voce ja colou as figurinhas que possuia.\n");
                printf("\n");
            }
        }
        break;

        case 2:
        {
            printf("\n");
            alocaAlbum(cabeca, 1, contadorAlbum);
            printf("Voce comprou o album %d!\n", cabeca->fim->chave);
            gastos += 12;
            contadorAlbum++;
            buscaFigurinhasRepetidas(cabeca->fim, &(cabeca->repetidas->fig), primSelecao, primJogador, &(cabeca->repetidas), cabeca);
            printf("\n");
        }
        break;

        case 3:
        {
            printf("\n");
            if (cabeca->inicio)
            {
                int op3;
                printf("Voce quer comprar figurinha para qual album?\n");
                printf("Quero o album:");
                scanf("%d", &op3);
                printf("\n");
                if (op3 <= 0)
                    printf("Insira um numero valido.\n");
                else
                {
                    TAlbum *auxAlbum3 = NULL;
                    TSelecao *auxSelecao3 = NULL;
                    TJogador *auxJogador3 = NULL;
                    auxAlbum3 = cabeca->inicio;
                    while (auxAlbum3->chave != op3 && auxAlbum3->prox != NULL)
                        auxAlbum3 = auxAlbum3->prox;
                    if (auxAlbum3->chave == op3 && auxAlbum3 != NULL)
                    {
                        gerarNumero(cont, p);
                        printf("\n");
                        printf("Voce comprou as seguintes figurinhas para o album %d:\n", auxAlbum3->chave);
                        for (int k = 0; k < 5; k++)
                        {
                            if ((contadorExtra % 190) == 0 || (contadorExtra % 317) == 0 || (contadorExtra % 950) == 0 || (contadorExtra % 1900) == 0)
                            {
                                insereFigurinhaAlbum(&(auxAlbum3), *(p + k), 20, primSelecao, primJogador, &(cabeca->repetidas));
                                printf("Parabens, voce conseguiu uma figurinha extra!\n");
                            }
                            else
                            {
                                insereFigurinhaAlbum(&(auxAlbum3), *(p + k), *(p + k + 5), primSelecao, primJogador, &(cabeca->repetidas));
                                auxSelecao3 = buscaSelecao(primSelecao, *(p + k));
                                auxJogador3 = buscaJogador(primJogador, *(p + k + 5), *(p + k));
                                printf("\t%d %s %d %s\n", auxJogador3->cod_selecao, auxSelecao3->selecao, auxJogador3->chave, auxJogador3->nome);
                            }
                        }
                        gastos += 4;
                        contadorExtra++;
                        auxSelecao3 = NULL;
                        auxJogador3 = NULL;
                        auxAlbum3 = NULL;
                        auxAlbum3 = cabeca->inicio;
                        while (auxAlbum3)
                        {
                            if (auxAlbum3)
                                buscaFigurinhasRepetidas(auxAlbum3, &(cabeca->repetidas->fig), primSelecao, primJogador, &(cabeca->repetidas), cabeca);
                            auxAlbum3 = auxAlbum3->prox;
                        }
                    }
                    else
                        printf("Voce nao possui um album com chave %d.\n", op3);
                }
            }
            else
            {
                printf("Voce precisa ter pelo menos 1 album para poder comprar figurinhas.\n");
            }
            printf("\n");
        }
        break;

        case 4:
        {
            printf("\n");
            if (!cabeca->inicio)
                printf("Voce ainda nao possui nenhum album.\n");
            else
            {
                int completo, album;
                TAlbum *auxAlbum4 = NULL;
                auxAlbum4 = cabeca->inicio;
                while (auxAlbum4)
                {
                    completo = checarAlbumCompleto(auxAlbum4);
                    if (completo)
                        printf("Album %d esta completo!\n", auxAlbum4->chave);
                    auxAlbum4 = auxAlbum4->prox;
                }
                auxAlbum4 = NULL;
                printf("Preco de cada album:\n");
                auxAlbum4 = cabeca->inicio;
                while (auxAlbum4)
                {
                    if (auxAlbum4)
                    {
                        printf("Album %d:\n", auxAlbum4->chave);
                        printf("\tR$%.2f\n", auxAlbum4->gasto);
                    }
                    auxAlbum4 = auxAlbum4->prox;
                }
                auxAlbum4 = NULL;
                printf("\nVoce quer vender qual album? (Pressione 0 caso nao queira vender nenhum)\n");
                printf("Vender album:");
                scanf("%d", &album);
                if (album)
                {
                    auxAlbum4 = cabeca->inicio;

                    while (auxAlbum4->chave != album && auxAlbum4->prox != NULL)
                        auxAlbum4 = auxAlbum4->prox;
                    if (auxAlbum4->chave == album && auxAlbum4 != NULL)
                    {
                        lucros += auxAlbum4->gasto;
                        alocaAlbum(cabecaVendidos, 1, auxAlbum4->chave);
                        cabecaVendidos->fim->gasto = auxAlbum4->gasto;
                        venderAlbum(auxAlbum4, &cabeca);
                        printf("Album %d vendido!\n", album);
                    }
                    else
                    {
                        printf("Voce nao possui um album com chave %d.\n", album);
                    }
                }
            }
            printf("\n");
        }
        break;

        case 5:
        {
            printf("\n");
            if (cabeca->repetidas->fig)
            {
                int codigoSelecao, numeroJogador, op2 = 0;
                if (!cabecaVendidos->repetidas)
                    alocaAlbum(cabecaVendidos, 0, -1);
                TFig *auxAlbumRepetido5 = NULL;
                auxAlbumRepetido5 = cabeca->repetidas->fig;
                printf("As figurinhas repetidas disponiveis sao:\n");
                imprimeAlbum(cabeca->repetidas->fig);
                printf("\n");
                printf("Digite o codigo da selecao da figurinha que voce deseja comprar: ");
                scanf("%d", &codigoSelecao);
                printf("Digite o numero do jogador que deseja comprar: ");
                scanf("%d", &numeroJogador);
                TFig *aux2 = NULL;
                while (auxAlbumRepetido5)
                {
                    aux2 = auxAlbumRepetido5;
                    if (auxAlbumRepetido5 != NULL && (auxAlbumRepetido5->cod_selecao == codigoSelecao) && (auxAlbumRepetido5->numero_jogador == numeroJogador))
                    {
                        op2 = 1;
                        removeFigurinha(&(cabeca->repetidas), codigoSelecao, numeroJogador);
                        insereFigurinhaAlbum(&(cabecaVendidos->repetidas), codigoSelecao, numeroJogador, primSelecao, primJogador, &(cabecaVendidos->repetidas));
                        break;
                    }
                    auxAlbumRepetido5 = auxAlbumRepetido5->prox;
                }
                auxAlbumRepetido5 = NULL;
                if (op2)
                {
                    cabecaVendidos->repetidas->gasto += 0.8;
                    lucros += 0.8;
                    printf("\nFigurinha vendida com sucesso!\n");
                    imprimeAlbum(cabecaVendidos->repetidas->fig);
                }
                else
                    printf("Nao existe essa figurinha repetida.\n");
            }
            else
                printf("Voce nao possui nenhuma figurinha repetida.\n");
            printf("\n");
        }
        break;

        case 6:
        {
            printf("\n");
            if (cabeca->inicio)
            {
                TAlbum *auxAlbum6 = NULL;
                auxAlbum6 = cabeca->inicio;
                printf("Gasto total: R$%.2f\n\n", gastos);
                printf("Gasto por album:\n");
                while (auxAlbum6)
                {
                    if (auxAlbum6)
                    {
                        printf("Album %d:\n", auxAlbum6->chave);
                        printf("\tR$%.2f\n", auxAlbum6->gasto);
                    }
                    auxAlbum6 = auxAlbum6->prox;
                }
                printf("Repetidas:\n");
                printf("\tR$%.2f\n", cabeca->repetidas->gasto);
            }
            else
            {
                if (gastos)
                    printf("Gasto total: R$%.2f\n\n", gastos);
                printf("Voce nao possui nenhum album.\n");
            }
            printf("\n");
        }
        break;

        case 7:
        {
            printf("\n");
            printf("Lucro: R$%.2f\n", lucros);
            if (cabecaVendidos->inicio || cabecaVendidos->repetidas)
            {
                TAlbum *auxAlbum7 = NULL;
                auxAlbum7 = cabecaVendidos->inicio;
                printf("Lucro por album:\n");
                while (auxAlbum7)
                {
                    if (auxAlbum7)
                    {
                        printf("Album %d:\n", auxAlbum7->chave);
                        printf("\tR$%.2f\n", auxAlbum7->gasto);
                    }
                    auxAlbum7 = auxAlbum7->prox;
                }
                if (cabecaVendidos->repetidas)
                {
                    if (cabecaVendidos->repetidas->fig)
                    {
                        printf("Figurinhas repetidas vendidas:\n");
                        imprimeAlbum(cabecaVendidos->repetidas->fig);
                    }
                    else
                        printf("Voce nao vendeu nenhuma figurinha.\n");
                }
            }
            else
                printf("Voce ainda nao vendeu nada.\n");
            printf("\n");
        }
        break;
        case 8:
        {
            printf("\n");
            if (!cabeca->inicio && !cabeca->repetidas)
                printf("Voce nao possui album.\n");
            else
            {
                TAlbum *auxAlbum8 = NULL;
                auxAlbum8 = cabeca->inicio;
                while (auxAlbum8)
                {
                    if (auxAlbum8)
                    {
                        printf("Album %d:\n", auxAlbum8->chave);
                        if (auxAlbum8->fig)
                            imprimeAlbum(auxAlbum8->fig);
                        else
                            printf("\tAlbum vazio!\n");
                    }
                    auxAlbum8 = auxAlbum8->prox;
                    printf("\n");
                }
                printf("Repetidas:\n");
                if (cabeca->repetidas->fig)
                    imprimeAlbum(cabeca->repetidas->fig);
                else
                    printf("\tNao existe nenhuma figurinha repetida ainda.\n");
            }
            printf("\n");
        }
        break;
        case 9:
        {
            printf("\n");
            if (cabeca->fim)
            {
                TAlbum *auxAlbum9 = NULL;
                auxAlbum9 = cabeca->inicio;
                while (auxAlbum9)
                {
                    if (auxAlbum9)
                        criaArquivoAlbum(auxAlbum9);
                    auxAlbum9 = auxAlbum9->prox;
                }
                printf("Todos os albuns foram salvos.\n");
                if (cabeca->repetidas->fig)
                {
                    criaArquivoAlbumRepetidas(cabeca->repetidas);
                    printf("As figurinhas repetidas foram salvas em: 'figurinhas_repetidas.txt'.\n");
                }
                printf("Seus gastos e lucros foram salvos em 'custos_e_lucros_obtidos.txt'\n");
                criaArquivoGastosELucros(gastos, lucros);
                auxAlbum9 = NULL;
                if (primSelecao)
                {
                    desalocaTSelecao(&primSelecao);
                    // printf("primSelecao desalocado.\n");
                }
                if (primJogador)
                {
                    desalocaTJogador(&primJogador);
                    // printf("primJogador desalocado.\n");
                }
                if (cabeca)
                {
                    removeCabeca(&cabeca);
                    cabeca = NULL;
                    // printf("Cabeca desalocado.\n");
                }
                if (cabecaVendidos)
                {
                    removeCabeca(&cabecaVendidos);
                    cabecaVendidos = NULL;
                    // printf("Cabeca vendidos desalocado.\n");
                }
                printf("Toda memoria alocada dinamicamente foi desalocada.\n");
                printf("Encerrando programa.\n");
            }
            else
            {
                if (cabeca->repetidas->fig)
                {
                    criaArquivoAlbumRepetidas(cabeca->repetidas);
                    printf("As figurinhas repetidas foram salvas em: 'figurinhas_repetidas.txt'.\n");
                }
                printf("Seus gastos e lucros foram salvos em 'custos_e_lucros_obtidos.txt'\n");
                criaArquivoGastosELucros(gastos, lucros);
                if (primSelecao)
                {
                    desalocaTSelecao(&primSelecao);
                    // printf("primSelecao desalocado.\n");
                }
                if (primJogador)
                {
                    desalocaTJogador(&primJogador);
                    // printf("primJogador desalocado.\n");
                }
                if (cabeca)
                {
                    removeCabeca(&cabeca);
                    cabeca = NULL;
                    // printf("Cabeca desalocado.\n");
                }
                if (cabecaVendidos)
                {
                    removeCabeca(&cabecaVendidos);
                    cabecaVendidos = NULL;
                    // printf("Cabeca vendidos desalocado.\n");
                }
                printf("Toda memoria alocada dinamicamente foi desalocada.\n");
                printf("Encerrando programa.\n");
            }
            printf("\n");
        }
        break;
        default:
            break;
        }
    }
}

TCabeca *criaCabecaAlbum()
{
    TCabeca *novo = NULL;
    novo = (TCabeca *)malloc(sizeof(TCabeca));
    if (!novo)
        return NULL;
    novo->inicio = NULL;
    novo->fim = NULL;
    novo->repetidas = NULL;
    return novo;
}

TAlbum *criaAlbum(int k)
{
    TAlbum *novo = NULL;
    novo = (TAlbum *)malloc(sizeof(TAlbum));
    if (!novo)
        return NULL;
    novo->prox = NULL;
    novo->fig = NULL;
    novo->chave = k;
    novo->gasto = 12;
    return novo;
}

void alocaAlbum(TCabeca *cabecaAlbum, int op, int k)
{
    TAlbum *novo = NULL;
    novo = criaAlbum(k);
    if (!op)
    {
        if (!novo)
            return;
        novo->gasto = 0;
        cabecaAlbum->repetidas = novo;
    }
    else
    {
        if (!novo)
            return;
        if (!(cabecaAlbum->inicio))
        {
            cabecaAlbum->inicio = novo;
            cabecaAlbum->fim = novo;
        }
        else
        {
            cabecaAlbum->fim->prox = novo;
            cabecaAlbum->fim = novo;
        }
    }
}

TSelecao *alocaSelecao(FILE *parquivo) // Cria e retorna um nó TSelecao
{
    TSelecao *novo = NULL;
    novo = (TSelecao *)malloc(sizeof(TSelecao));
    if (!novo)
        return NULL;
    fscanf(parquivo, "%d %s\n", &novo->cod_selecao, novo->selecao);
    novo->prox = NULL;
    return novo;
}

TFig *alocaFigurinhaAlbum(TSelecao *structSelecao, TJogador *structJogador)
{
    TFig *novo = NULL;
    novo = (TFig *)malloc(sizeof(TFig));
    if (!novo)
        return NULL;
    novo->cod_selecao = structSelecao->cod_selecao;
    novo->numero_jogador = structJogador->chave;
    strcpy(novo->nome, structJogador->nome);
    strcpy(novo->selecao, structSelecao->selecao);
    novo->prox = NULL;
    return novo;
}

void insereFigurinhaAlbum(TAlbum **album, int selecao, int jogador, TSelecao *listaselec, TJogador *listajog, TAlbum **repetidas)
{
    TFig *novo = NULL;
    TSelecao *structSelecao = NULL;
    TJogador *structJogador = NULL;
    structSelecao = buscaSelecao(listaselec, selecao);
    structJogador = buscaJogador(listajog, jogador, selecao);
    novo = alocaFigurinhaAlbum(structSelecao, structJogador);
    (*album)->gasto += 0.8;
    TFig *aux = NULL;
    aux = (*album)->fig;
    if (!*album)
        return;
    if (!(*album)->fig)
    {
        (*album)->fig = novo;
    }
    else
    {
        if (aux->cod_selecao < novo->cod_selecao) // Insere no fim
        {

            if (aux->prox)
            {

                TFig *aux2 = NULL;
                while ((aux->prox != NULL))
                {
                    if (aux->cod_selecao >= novo->cod_selecao)
                        break;
                    aux2 = aux;
                    aux = aux->prox;
                }
                if (!aux->prox)
                {
                    if (novo->cod_selecao <= aux->cod_selecao)
                    {
                        if (novo->cod_selecao == aux->cod_selecao)
                        {
                            if (aux->numero_jogador <= novo->numero_jogador)
                            {
                                if (aux->numero_jogador == novo->numero_jogador)
                                {
                                    insereFigurinhaAlbumRepetida(repetidas, novo);
                                    (*album)->gasto -= 0.8;
                                    return;
                                }
                                aux->prox = novo;
                                novo->prox = NULL;
                            }
                            else
                            {
                                if (aux->numero_jogador == novo->numero_jogador)
                                {
                                    insereFigurinhaAlbumRepetida(repetidas, novo);
                                    (*album)->gasto -= 0.8;
                                    return;
                                }
                                novo->prox = aux;
                                aux2->prox = novo;
                                aux->prox = NULL;
                            }
                        }
                        else
                        {
                            novo->prox = aux;
                            aux2->prox = novo;
                            aux->prox = NULL;
                        }
                    }
                    else
                    {
                        aux->prox = novo;
                        novo->prox = NULL;
                    }
                }
                else if (aux->cod_selecao > novo->cod_selecao)
                {
                    novo->prox = aux2->prox;
                    aux2->prox = novo;
                }
                else if (aux->cod_selecao == novo->cod_selecao)
                {

                    if (aux->numero_jogador >= novo->numero_jogador)
                    {
                        if (aux->numero_jogador == novo->numero_jogador)
                        {
                            insereFigurinhaAlbumRepetida(repetidas, novo);
                            (*album)->gasto -= 0.8;
                            return;
                        }
                        novo->prox = aux;
                        aux2->prox = novo;
                    }
                    else
                    {
                        if (aux->prox->numero_jogador > novo->numero_jogador && aux->numero_jogador < novo->numero_jogador)
                        {
                            novo->prox = aux->prox;
                            aux->prox = novo;
                            return;
                        }
                        while (aux->prox != NULL && aux->prox->cod_selecao == novo->cod_selecao && aux->prox->numero_jogador <= novo->numero_jogador)
                            aux = aux->prox;
                        if (aux->numero_jogador == novo->numero_jogador)
                        {
                            insereFigurinhaAlbumRepetida(repetidas, novo);
                            (*album)->gasto -= 0.8;
                            return;
                        }
                        if (aux->prox)
                        {
                            novo->prox = aux->prox;
                            aux->prox = novo;
                        }
                        else
                            aux->prox = novo;
                    }
                }
                else
                {
                    if (aux->numero_jogador == novo->numero_jogador)
                    {
                        insereFigurinhaAlbumRepetida(repetidas, novo);
                        (*album)->gasto -= 0.8;
                        return;
                    }
                    aux->prox = novo;
                }
            }
            else
            {
                if (aux->numero_jogador == novo->numero_jogador)
                {
                    insereFigurinhaAlbumRepetida(repetidas, novo);
                    (*album)->gasto -= 0.8;
                    return;
                }
                aux->prox = novo;
            }
        }

        else if (aux->cod_selecao == novo->cod_selecao) // Se a primeira figurinha já for a selecao correta
        {
            if (aux->numero_jogador == novo->numero_jogador)
            {
                insereFigurinhaAlbumRepetida(repetidas, novo);
                (*album)->gasto -= 0.8;
                return;
            }
            if (aux->numero_jogador > novo->numero_jogador)
            {
                novo->prox = (*album)->fig;
                (*album)->fig = novo;
            }
            else if (aux->prox)
            {
                TFig *aux3 = NULL;
                while (aux->cod_selecao == novo->cod_selecao && aux->prox != NULL)
                {
                    aux3 = aux;
                    if ((aux->prox->numero_jogador > novo->numero_jogador))
                    {
                        if (aux->numero_jogador == novo->numero_jogador)
                        {
                            insereFigurinhaAlbumRepetida(repetidas, novo);
                            (*album)->gasto -= 0.8;
                            return;
                        }
                        novo->prox = aux->prox;
                        aux->prox = novo;
                        return;
                    }
                    aux = aux->prox;
                }
                if (aux->numero_jogador == novo->numero_jogador)
                {
                    insereFigurinhaAlbumRepetida(repetidas, novo);
                    (*album)->gasto -= 0.8;
                    return;
                }

                if (!aux->prox)
                {
                    aux->prox = novo;
                }
                else
                {
                    novo->prox = aux3->prox;
                    aux3->prox = novo;
                }
            }
            else if (!aux->prox)
            {
                if (aux->numero_jogador == novo->numero_jogador)
                {
                    insereFigurinhaAlbumRepetida(repetidas, novo);
                    (*album)->gasto -= 0.8;
                    return;
                }
                aux->prox = novo;
            }
            else
            {
                novo->prox = aux->prox;
                aux->prox = novo;
            }
        }
        else // Insere no inicio
        {
            novo->prox = (*album)->fig;
            (*album)->fig = novo;
            return;
        }
    }
    return;
}

void insereFigurinhaAlbumRepetida(TAlbum **repetida, TFig *novo)
{
    TFig *aux = NULL;
    (*repetida)->gasto += 0.8;
    aux = (*repetida)->fig;
    if (!*repetida)
        return;
    if (!(*repetida)->fig)
        (*repetida)->fig = novo;
    else
    {
        if (aux->cod_selecao < novo->cod_selecao) // Insere no fim
        {

            if (aux->prox)
            {

                TFig *aux2 = NULL;
                while ((aux->prox != NULL))
                {
                    if (aux->cod_selecao >= novo->cod_selecao)
                        break;
                    aux2 = aux;
                    aux = aux->prox;
                }
                if (!aux->prox)
                {
                    if (novo->cod_selecao <= aux->cod_selecao)
                    {
                        if (novo->cod_selecao == aux->cod_selecao)
                        {
                            if (aux->numero_jogador <= novo->numero_jogador)
                            {
                                aux->prox = novo;
                                novo->prox = NULL;
                            }
                            else
                            {
                                novo->prox = aux;
                                aux2->prox = novo;
                                aux->prox = NULL;
                            }
                        }
                        else
                        {
                            novo->prox = aux;
                            aux2->prox = novo;
                            aux->prox = NULL;
                        }
                    }
                    else
                    {
                        aux->prox = novo;
                        novo->prox = NULL;
                    }
                }
                else if (aux->cod_selecao > novo->cod_selecao)
                {
                    novo->prox = aux2->prox;
                    aux2->prox = novo;
                }
                else if (aux->cod_selecao == novo->cod_selecao)
                {

                    if (aux->numero_jogador >= novo->numero_jogador)
                    {
                        novo->prox = aux;
                        aux2->prox = novo;
                    }
                    else
                    {
                        if (aux->prox->numero_jogador > novo->numero_jogador && aux->numero_jogador < novo->numero_jogador)
                        {
                            novo->prox = aux->prox;
                            aux->prox = novo;
                            return;
                        }
                        while (aux->prox != NULL && aux->prox->cod_selecao == novo->cod_selecao && aux->prox->numero_jogador <= novo->numero_jogador)
                        {
                            aux = aux->prox;
                        }
                        if (aux->prox)
                        {
                            novo->prox = aux->prox;
                            aux->prox = novo;
                        }
                        else
                            aux->prox = novo;
                    }
                }
                else
                    aux->prox = novo;
            }
            else
                aux->prox = novo;
        }

        else if (aux->cod_selecao == novo->cod_selecao) // Se a primeira figurinha já for a selecao correta
        {

            if (aux->numero_jogador > novo->numero_jogador)
            {
                novo->prox = (*repetida)->fig;
                (*repetida)->fig = novo;
            }
            else if (aux->prox)
            {
                TFig *aux3 = NULL;
                while (aux->cod_selecao == novo->cod_selecao && aux->prox != NULL)
                {
                    aux3 = aux;
                    if ((aux->prox->numero_jogador > novo->numero_jogador))
                    {
                        novo->prox = aux->prox;
                        aux->prox = novo;
                        return;
                    }
                    aux = aux->prox;
                }

                if (!aux->prox)
                    aux->prox = novo;
                else
                {
                    novo->prox = aux3->prox;
                    aux3->prox = novo;
                }
            }
            else if (!aux->prox)
                aux->prox = novo;
            else
            {
                novo->prox = aux->prox;
                aux->prox = novo;
            }
        }
        else // Insere no inicio
        {
            novo->prox = (*repetida)->fig;
            (*repetida)->fig = novo;
            return;
        }
    }
    return;
}

TJogador *alocaFigurinhaJogador(FILE *parquivo) // Cria e retorna um nó TJogador
{
    TJogador *novo = NULL;
    novo = (TJogador *)malloc(sizeof(TJogador));
    if (!novo)
        return NULL;
    fscanf(parquivo, "%d %d %s\n", &novo->cod_selecao, &novo->chave, novo->nome);
    novo->prox = NULL;
    return novo;
}

TFig *alocaFigurinhaJogadorEntrada(FILE *parquivo) // Cria e retorna um nó TFig
{
    TFig *novo = NULL;
    novo = (TFig *)malloc(sizeof(TFig));
    if (!novo)
        return NULL;
    fscanf(parquivo, "%d %s %d %s\n", &novo->cod_selecao, novo->selecao, &novo->numero_jogador, novo->nome);
    novo->prox = NULL;
    return novo;
}

void lerSelecoes(FILE *parquivo, TSelecao **prim) // Insere no final um TSelecao na lista encadeada
{
    if (!(*prim))
        *prim = alocaSelecao(parquivo);
    else
    {
        TSelecao *novo = NULL, *aux = NULL;
        aux = *prim;
        while (aux->prox)
            aux = aux->prox;
        novo = alocaSelecao(parquivo);
        if (!novo)
            return;
        aux->prox = novo;
    }
}

void lerFigurinhasJogadores(FILE *parquivo, TJogador **prim) // Insere no final um TJogador na lista encadeada
{
    if (!(*prim))
        *prim = alocaFigurinhaJogador(parquivo);
    else
    {
        TJogador *novo = NULL, *aux = NULL;
        aux = *prim;
        while (aux->prox)
            aux = aux->prox;
        novo = alocaFigurinhaJogador(parquivo);
        if (!novo)
            return;
        aux->prox = novo;
    }
}

void lerFigurinhasJogadoresEntrada(FILE *parquivo, TFig **prim)
{
    if (!(*prim))
        *prim = alocaFigurinhaJogadorEntrada(parquivo);
    else
    {
        TFig *novo = NULL, *aux = NULL;
        aux = *prim;
        while (aux->prox)
            aux = aux->prox;
        novo = alocaFigurinhaJogadorEntrada(parquivo);
        if (!novo)
            return;
        aux->prox = novo;
    }
}
void imprimeListaSelecoes(TSelecao *p) // Printa a lista encadeada de 'seleções'
{
    if (p)
    {
        while (p)
        {
            printf("%d %s\n", p->cod_selecao, p->selecao);
            p = p->prox;
        }
    }
}

void imprimeListaFigurinhasJogadores(TJogador *p) // Printa a lista encadeada de 'figurinhas_total'
{
    if (p)
    {
        while (p)
        {
            printf("%d %d %s\n", p->cod_selecao, p->chave, p->nome);
            p = p->prox;
        }
    }
}

void imprimeAlbum(TFig *album)
{
    if (album)
    {
        while (album)
        {
            printf("\t%d %s %d %s\n", album->cod_selecao, album->selecao, album->numero_jogador, album->nome);
            album = album->prox;
        }
    }
}

TSelecao *buscaSelecao(TSelecao *listaselec, int selec)
{
    TSelecao *tmp = NULL;
    tmp = listaselec;
    while (tmp->prox != NULL && tmp->cod_selecao != selec)
        tmp = tmp->prox;
    return tmp;
}

TJogador *buscaJogador(TJogador *listajog, int jogador, int selecao)
{
    TJogador *tmp = NULL;
    tmp = listajog;
    while (tmp->cod_selecao != selecao)
        tmp = tmp->prox;
    while (tmp->chave != jogador)
        tmp = tmp->prox;
    return tmp;
}

TAlbum *buscaAlbum(TAlbum *album, int k)
{
    TAlbum *aux = NULL;
    aux = album;
    while (aux->chave != k && aux != NULL)
        aux = aux->prox;
    return aux;
}

void gerarNumero(int cont, int *p)
{
    int selecao, jogador;
    time_t t;
    srand((unsigned)time(&t) + cont);
    for (int i = 0; i < 5; i++)
    {
        cont += 1000;
        selecao = (rand() % 32) + 1;
        *(p + i) = selecao;
    }
    for (int j = 5; j < 10; j++)
    {
        cont += 1000;
        jogador = (rand() % 19) + 1;
        *(p + j) = jogador;
    }
}

void buscaFigurinhasRepetidas(TAlbum *album, TFig **figurinha, TSelecao *listaselec, TJogador *listajog, TAlbum **repetidas, TCabeca *cabeca)
{
    TFig *aux = NULL;
    aux = *figurinha;
    alocaAlbum(cabeca, 0, -1);
    while (aux)
    {
        insereFigurinhaAlbum(&album, (*figurinha)->cod_selecao, (*figurinha)->numero_jogador, listaselec, listajog, repetidas);
        (*figurinha) = (*figurinha)->prox;
        free(aux);
        aux = *figurinha;
    }
    aux = NULL;
}

void venderAlbum(TAlbum *album, TCabeca **cabeca)
{
    TAlbum *aux = NULL, *aux2 = NULL;

    removeFigurinhasAlbum(&album->fig);

    aux = (*cabeca)->inicio;
    aux2 = aux;
    if (aux->chave == album->chave)
    {

        if (aux->prox)
        {
            (*cabeca)->inicio = aux->prox;
            free(aux);
            aux = NULL;
        }
        else
        {
            (*cabeca)->inicio = NULL;
            (*cabeca)->fim = NULL;
            free(aux);
            aux = NULL;
        }
    }
    else
    {

        while ((album->chave != aux->chave) && (aux->prox != NULL))
        {
            aux2 = aux;
            aux = aux->prox;
        }
        if (!aux)
            return;
        if (aux->prox)
        {
            aux2->prox = aux->prox;
            free(aux);
            aux = NULL;
        }
        else
        {
            (*cabeca)->fim = aux2;
            free(album);
            aux2->prox = NULL;
            album = NULL;
        }
    }
}
int checarAlbumCompleto(TAlbum *album)
{
    int completo = 0, contJogador = 0;
    TFig *aux = NULL;
    aux = album->fig;
    if (album->fig == NULL || album == NULL)
        return completo;
    while (aux)
    {
        contJogador++;
        aux = aux->prox;
    }
    if (contJogador == 640)
        completo++;
    return completo;
}

void removeFigurinhasAlbum(TFig **figurinha)
{
    if (*figurinha == NULL)
        return;
    removeFigurinhasAlbum(&(*figurinha)->prox);
    free(*figurinha);
    *figurinha = NULL;
}

void removeFigurinha(TAlbum **figurinha, int cod, int jogador)
{
    if (!(*figurinha))
        return;
    TFig *aux = NULL;
    aux = (*figurinha)->fig;
    if (((*figurinha)->fig->cod_selecao == cod) && ((*figurinha)->fig->numero_jogador == jogador))
    {
        (*figurinha)->fig = aux->prox;
        free(aux);
        aux = NULL;
    }
    else
    {
        TFig *aux2 = NULL;
        while (aux != NULL)
        {
            if (aux)
            {
                if ((aux->cod_selecao == cod) && (aux->numero_jogador == jogador))
                {
                    if (aux->prox)
                    {
                        aux2->prox = aux->prox;
                        printf("%d %d\n", aux->cod_selecao, aux->numero_jogador);
                        free(aux);
                        return;
                    }
                    else
                    {
                        free(aux);
                        aux2->prox = NULL;
                        return;
                    }
                }
            }
            aux2 = aux;
            aux = aux->prox;
        }
    }
}

void removeCabeca(TCabeca **cabeca)
{
    if (!(*cabeca))
        return;
    if (!(*cabeca)->inicio)
    {
        if ((*cabeca)->repetidas)
        {
            free((*cabeca)->repetidas);
            (*cabeca)->repetidas = NULL;
        }
        free(*cabeca);
        *cabeca = NULL;
        return;
    }
    if ((*cabeca)->inicio->fig)
    {
        removeFigurinhasAlbum(&(*cabeca)->inicio->fig);
    }
    TAlbum *aux = (*cabeca)->inicio;
    if (aux->prox)
    {
        (*cabeca)->inicio = aux->prox;
        free(aux);
        aux = NULL;
    }
    else
    {
        free((*cabeca)->inicio);
        if ((*cabeca)->repetidas)
        {
            free((*cabeca)->repetidas);
            (*cabeca)->repetidas = NULL;
        }
        (*cabeca)->inicio = NULL;
        (*cabeca)->fim = NULL;
        free(*cabeca);
        *cabeca = NULL;
    }
}

void desalocaTSelecao(TSelecao **selecao)
{
    if (*selecao == NULL)
        return;
    free(*selecao);
    *selecao = NULL;
}

void desalocaTJogador(TJogador **jogador)
{
    if (*jogador == NULL)
        return;
    free(*jogador);
    *jogador = NULL;
}

void criaArquivoAlbum(TAlbum *album)
{
    FILE *arquivo;
    char nomeArquivo[15] = "album";
    char txt[5] = ".txt";
    char numero[1];
    numero[0] = (album->chave) + '0';
    strcat(nomeArquivo, numero);
    setbuf(stdin, NULL);
    arquivo = fopen(nomeArquivo, "w");
    while (album->fig)
    {
        fprintf(arquivo, "%d %s %d %s\n", album->fig->cod_selecao, album->fig->selecao, album->fig->numero_jogador, album->fig->nome);
        album->fig = album->fig->prox;
    }
    fclose(arquivo);
}

void criaArquivoAlbumRepetidas(TAlbum *album)
{
    FILE *arquivo;
    char nomeArquivo[30] = "figurinhas_repetidas.txt";
    setbuf(stdin, NULL);
    arquivo = fopen(nomeArquivo, "w");
    while (album->fig)
    {
        fprintf(arquivo, "%d %s %d %s\n", album->fig->cod_selecao, album->fig->selecao, album->fig->numero_jogador, album->fig->nome);
        album->fig = album->fig->prox;
    }
    fclose(arquivo);
}

void criaArquivoGastosELucros(float gastos, float lucros)
{
    FILE *arquivo;
    char nomeArquivo[30] = "custos_e_lucros_obtidos.txt";
    arquivo = fopen(nomeArquivo, "w");
    fprintf(arquivo, "%s %s %s%.2f\n%s %s %s%.2f\n", "Gasto", "Total:", "R$", gastos, "Lucro", "Total:", "R$", lucros);
    fclose(arquivo);
}
