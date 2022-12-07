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
void lerSelecoes(FILE *parquivo, TSelecao **prim);
void lerFigurinhasJogadores(FILE *parquivo, TJogador **prim);
void lerFigurinhasJogadoresEntrada(FILE *parquivo, TFig **prim);
void imprimeListaSelecoes(TSelecao *p);
void imprimeListaFigurinhasJogadores(TJogador *p);
void imprimeAlbum(TFig *album);
TSelecao *buscaSelecao(TSelecao *listaselec, int selec);
TJogador *buscaJogador(TJogador *listajog, int jogador, int selecao);
void gerarNumero(int cont, int *p);
void insereFigurinhaAlbumRepetida(TAlbum **repetida, TFig *novo);
void buscaFigurinhasRepetidas(TAlbum *album, TFig **figurinha, TSelecao *listaselec, TJogador *listajog, TAlbum **repetidas, TCabeca *cabeca);
int checarAlbumCompleto(TAlbum *album);
void removeFigurinhasAlbum(TFig **figurinha);

int main()
{
    int op = 0, op2 = 0, opc;
    int selec = 32, jog = 1, i = 0, cont = 0, *p = NULL, contadorExtra = 0, contadorAlbum = 0;
    float gastos = 0.0;

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
    // imprimeListaSelecoes(primSelecao);
    while (!feof(arquivoFigurinhaJogador))
        lerFigurinhasJogadores(arquivoFigurinhaJogador, &primJogador);
    fclose(arquivoFigurinhaJogador);
    while (!feof(arquivoFigurinhaJogadorEntrada))
        lerFigurinhasJogadoresEntrada(arquivoFigurinhaJogadorEntrada, &primJogadorEntrada);
    fclose(arquivoFigurinhaJogadorEntrada);

    // imprimeListaFigurinhasJogadores(primJogador);
    TCabeca *cabeca = NULL;
    cabeca = criaCabecaAlbum();
    alocaAlbum(cabeca, 1, contadorAlbum);
    contadorAlbum++;
    gastos += 12;
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
    while (op != 8)
    {
        printf("1-Inicializacao\n2-Comprar novo album\n3-Comprar pacote figurinha\n4-Vender album\n5-Vender figurinhas repetidas\n6-relatorio de gastos\n7-Relatorio de lucros\n8-sair\n");
        scanf("%d", &op);
        switch (op)
        {
        case 1:
        {
            if (primJogadorEntrada)
            {
                TFig *aux = NULL;
                aux = primJogadorEntrada;
                while (aux->prox)
                {
                    insereFigurinhaAlbum(&(cabeca->fim), aux->cod_selecao, aux->numero_jogador, primSelecao, primJogador, &(cabeca->repetidas));
                    gastos += 0.8;
                    aux = aux->prox;
                }
                insereFigurinhaAlbum(&(cabeca->fim), aux->cod_selecao, aux->numero_jogador, primSelecao, primJogador, &(cabeca->repetidas));
                gastos += 0.8;
                printf("R$%.2f\n", gastos);
                buscaFigurinhasRepetidas(cabeca->fim, &(cabeca->repetidas->fig), primSelecao, primJogador, &(cabeca->repetidas), cabeca);
                printf("################################################################\n");
                imprimeAlbum(cabeca->fim->fig);
                printf("################################\n");
                if (cabeca->repetidas->fig)
                    imprimeAlbum(cabeca->repetidas->fig);
                removeFigurinhasAlbum(&primJogadorEntrada);
            }
            else
            {
                printf("Voce ja fez a inicializacao.\n");
            }
        }
        break;

        case 2:
        {
            alocaAlbum(cabeca, 1, contadorAlbum);
            contadorAlbum++;
            buscaFigurinhasRepetidas(cabeca->fim, &(cabeca->repetidas->fig), primSelecao, primJogador, &(cabeca->repetidas), cabeca);
            printf("##########################################\n");
            printf("Album novo:\n");
            imprimeAlbum(cabeca->fim->fig);
            printf("######################################\n");
            printf("Repetidas:\n");
            if (cabeca->repetidas->fig)
                imprimeAlbum(cabeca->repetidas->fig);
        }
        break;

        case 3:
        {
            gerarNumero(cont, p);
            printf("\n");
            for (int k = 0; k < 10; k++)
                printf("%d\n", *(p + k));
        }
        break;

        case 4:
        {
            int completo;
            completo = checarAlbumCompleto(cabeca->inicio);
            printf("Completo\n");
        }
        break;

        case 5:
        {
        }
        break;

        case 6:
        {
        }
        break;

        case 7:
        {
        }
        break;
        default:
        {
        }
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
    // printf("\n###############\n");
    // printf("%d %d %s %s\n\n", novo->cod_selecao, novo->numero_jogador, novo->nome, novo->selecao);
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
                    {
                        // printf("Breakou\n aux:%d aux2:%d novo:%d\n", aux->cod_selecao, aux2->cod_selecao, novo->cod_selecao);
                        // printf("%d %d %s %s\n\n", novo->cod_selecao, novo->numero_jogador, novo->nome, novo->selecao);
                        break;
                    }
                    aux2 = aux;
                    aux = aux->prox;
                }
                if (!aux->prox)
                {
                    // printf("Falha segmentacao\n");
                    if (novo->cod_selecao <= aux->cod_selecao)
                    {
                        if (novo->cod_selecao == aux->cod_selecao)
                        {
                            if (aux->numero_jogador <= novo->numero_jogador)
                            {
                                if (aux->numero_jogador == novo->numero_jogador)
                                {
                                    insereFigurinhaAlbumRepetida(repetidas, novo);
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
                        // printf("aqui está o bug? aux:%d aux2:%d novo:%d\n", aux->cod_selecao, aux2->cod_selecao, novo->cod_selecao);
                        // printf("%d %d %s %s\n\n", novo->cod_selecao, novo->numero_jogador, novo->nome, novo->selecao);
                        aux->prox = novo;
                        novo->prox = NULL;
                    }
                }
                else if (aux->cod_selecao > novo->cod_selecao)
                {
                    novo->prox = aux2->prox;
                    aux2->prox = novo;
                    // printf("%d %d %s %s\n\n", novo->cod_selecao, novo->numero_jogador, novo->nome, novo->selecao);
                }
                else if (aux->cod_selecao == novo->cod_selecao)
                {

                    if (aux->numero_jogador >= novo->numero_jogador)
                    {
                        if (aux->numero_jogador == novo->numero_jogador)
                        {
                            insereFigurinhaAlbumRepetida(repetidas, novo);
                            return;
                        }
                        // printf("!!aux: %d, aux2: %d, novo:%d\n", aux->numero_jogador, aux2->numero_jogador, novo->numero_jogador);
                        // printf("%d %d %s %s\n\n", novo->cod_selecao, novo->numero_jogador, novo->nome, novo->selecao);
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
                        if (aux->numero_jogador == novo->numero_jogador)
                        {
                            insereFigurinhaAlbumRepetida(repetidas, novo);
                            return;
                        }
                        if (aux->prox)
                        {
                            // printf("%d %d %s %s\n\n", novo->cod_selecao, novo->numero_jogador, novo->nome, novo->selecao);
                            novo->prox = aux->prox;
                            aux->prox = novo;
                        }
                        else
                        {
                            aux->prox = novo;
                            // printf("%d %d %s %s\n\n", novo->cod_selecao, novo->numero_jogador, novo->nome, novo->selecao);
                        }
                    }
                }
                else
                {
                    if (aux->numero_jogador == novo->numero_jogador)
                    {
                        insereFigurinhaAlbumRepetida(repetidas, novo);
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
                    return;
                }
                aux->prox = novo;
            }
        }

        else if (aux->cod_selecao == novo->cod_selecao) // Se a primeira figurinha já for a selecao correta // CONDICAO CORRETA!!!!
        {
            if (aux->numero_jogador == novo->numero_jogador)
            {
                insereFigurinhaAlbumRepetida(repetidas, novo);
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
                    return;
                }

                if (!aux->prox)
                {
                    aux->prox = novo;
                }
                else
                {
                    // printf("Aki estava o problema?\n");
                    novo->prox = aux3->prox;
                    aux3->prox = novo;
                }
            }
            else if (!aux->prox)
            {
                if (aux->numero_jogador == novo->numero_jogador)
                {
                    insereFigurinhaAlbumRepetida(repetidas, novo);
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
            // printf("Aki?\n");
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
                    {
                        // printf("Breakou\n aux:%d aux2:%d novo:%d\n", aux->cod_selecao, aux2->cod_selecao, novo->cod_selecao);
                        // printf("%d %d %s %s\n\n", novo->cod_selecao, novo->numero_jogador, novo->nome, novo->selecao);
                        break;
                    }
                    aux2 = aux;
                    aux = aux->prox;
                }
                if (!aux->prox)
                {
                    // printf("Falha segmentacao\n");
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
                        // printf("aqui está o bug? aux:%d aux2:%d novo:%d\n", aux->cod_selecao, aux2->cod_selecao, novo->cod_selecao);
                        // printf("%d %d %s %s\n\n", novo->cod_selecao, novo->numero_jogador, novo->nome, novo->selecao);
                        aux->prox = novo;
                        novo->prox = NULL;
                    }
                }
                else if (aux->cod_selecao > novo->cod_selecao)
                {
                    novo->prox = aux2->prox;
                    aux2->prox = novo;
                    // printf("%d %d %s %s\n\n", novo->cod_selecao, novo->numero_jogador, novo->nome, novo->selecao);
                }
                else if (aux->cod_selecao == novo->cod_selecao)
                {

                    if (aux->numero_jogador >= novo->numero_jogador)
                    {
                        // printf("!!aux: %d, aux2: %d, novo:%d\n", aux->numero_jogador, aux2->numero_jogador, novo->numero_jogador);
                        // printf("%d %d %s %s\n\n", novo->cod_selecao, novo->numero_jogador, novo->nome, novo->selecao);
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
                            // printf("%d %d %s %s\n\n", novo->cod_selecao, novo->numero_jogador, novo->nome, novo->selecao);
                            novo->prox = aux->prox;
                            aux->prox = novo;
                        }
                        else
                        {
                            aux->prox = novo;
                            // printf("%d %d %s %s\n\n", novo->cod_selecao, novo->numero_jogador, novo->nome, novo->selecao);
                        }
                    }
                }
                else
                    aux->prox = novo;
            }
            else
                aux->prox = novo;
        }

        else if (aux->cod_selecao == novo->cod_selecao) // Se a primeira figurinha já for a selecao correta // CONDICAO CORRETA!!!!
        {
            // printf("Chegou aki?%d %d\n", aux->cod_selecao, novo->cod_selecao);

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
                        // printf("Existe\n");

                        novo->prox = aux->prox;
                        aux->prox = novo;
                        return;
                    }
                    aux = aux->prox;
                }

                if (!aux->prox)
                {
                    aux->prox = novo;
                }
                else
                {
                    // printf("Aki estava o problema?\n");
                    novo->prox = aux3->prox;
                    aux3->prox = novo;
                }
            }
            else if (!aux->prox)
            {
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
            // printf("Aki?\n");
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
    else
    {
        printf("\nPosto vazio!\n");
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
    else
    {
        printf("\nPosto vazio!\n");
    }
}

void imprimeAlbum(TFig *album)
{
    if (album)
    {
        while (album)
        {
            printf("%d %s %d %s\n", album->cod_selecao, album->selecao, album->numero_jogador, album->nome);
            album = album->prox;
        }
    }
}

TSelecao *buscaSelecao(TSelecao *listaselec, int selec)
{ // Ata pia, nao sabia, tá certo agora né ? acho que ta
    TSelecao *tmp = NULL;
    tmp = listaselec;
    while (tmp->prox != NULL && tmp->cod_selecao != selec)
        tmp = tmp->prox;
    return tmp;
}

TJogador *buscaJogador(TJogador *listajog, int jogador, int selecao)
{ // É isso. Tem q testar na main só. Printa na main pra ver se ta td certo
    TJogador *tmp = NULL;
    tmp = listajog;
    // Sim pia
    //  A gente ta forcando que o numero seja entre 0 e 19, nunca vai chegar a tmp->prox == NULL
    while (tmp->cod_selecao != selecao)
        tmp = tmp->prox;
    while (tmp->chave != jogador)
        tmp = tmp->prox;
    return tmp;
}

void gerarNumero(int cont, int *p)
{
    int selecao, jogador;
    time_t t;
    srand((unsigned)time(&t) + cont);
    for (int i = 0; i < 5; i++)
    {
        selecao = (rand() % 32) + 1;
        *(p + i) = selecao;
    }
    for (int j = 5; j < 10; j++)
    {
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

int checarAlbumCompleto(TAlbum *album)
{
    int completo = 0, contJogador = 1;
    TFig *aux = NULL;
    TAlbum *aux2 = NULL;
    aux = album->fig;
    aux2 = album;
    printf("%d %d\n", aux->cod_selecao, aux->numero_jogador);
    if (!album->fig || !album)
        return 0;
    do
    {
        while (aux->prox)
        {
            contJogador++;
            aux = aux->prox;
        }
        if (contJogador == 640)
        {
            printf("Album %d completo\n", completo);
        }
        printf("Passou pelo if\n");
        aux2 = aux2->prox;
        completo++;
    } while (aux2);
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