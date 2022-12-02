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
TAlbum *criaAlbum();
void alocaAlbum(TCabeca *cabecaAlbum);
TSelecao *alocaSelecao(FILE *parquivo);
TFig *alocaFigurinhaAlbum(TSelecao *structSelecao, TJogador *structJogador);
void insereFigurinhaAlbum(TAlbum *album, TSelecao *structSelecao, TJogador *structJogador);
TJogador *alocaFigurinhaJogador(FILE *parquivo);
void lerSelecoes(FILE *parquivo, TSelecao **prim);
void lerFigurinhasJogadores(FILE *parquivo, TJogador **prim);
void imprimeListaSelecoes(TSelecao *p);
void imprimeListaFigurinhasJogadores(TJogador *p);
TSelecao *buscaSelecao(TSelecao *listaselec, int selec);
TJogador *buscaJogador(TJogador *listajog, int jogador, int selecao);
int gerarNumero(int k);

int main()
{
    int op = 0, op2 = 0, opc, selec, jog;

    FILE *arquivoSelecao, *arquivoFigurinhaJogador;
    arquivoSelecao = fopen("selecoes.txt", "r");
    arquivoFigurinhaJogador = fopen("figurinhas_total.txt", "r");
    TSelecao *primSelecao = NULL;
    TJogador *primJogador = NULL;
    while (!feof(arquivoSelecao))
        lerSelecoes(arquivoSelecao, &primSelecao);
    fclose(arquivoSelecao);
    // imprimeListaSelecoes(primSelecao);
    while (!feof(arquivoFigurinhaJogador))
        lerFigurinhasJogadores(arquivoFigurinhaJogador, &primJogador);
    fclose(arquivoFigurinhaJogador);
    // imprimeListaFigurinhasJogadores(primJogador);
    TCabeca *cabeca = NULL;
    cabeca = criaCabecaAlbum();
    alocaAlbum(cabeca);
    printf("\n########################\n");
    TSelecao *selecao = NULL;
    TJogador *jogador = NULL;
    selec = 2;
    jog = 2;
    for (int i = 0; i < 20; i++)
    {
        selecao = buscaSelecao(primSelecao, selec);
        jogador = buscaJogador(primJogador, jog, selecao->cod_selecao);
        insereFigurinhaAlbum(cabeca->inicio, selecao, jogador);
        free(selecao);
        free(jogador);
        jog += 1;
        if (selec >= 27)
            selec = 2;
        if (jog >= 18)
            jog = 1;
    }

    /*
    while (op != 7)
    {
        printf("7-sair\n");
        scanf("%d", &op);
        switch (op)
        {
        case 1:
        {
            jog = gerarNumero(19);
            selec = gerarNumero(32);
        }
        break;

        case 2:
        {
        }
        break;
        default:
        {
        }
        break;
        }
    } */
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

TAlbum *criaAlbum()
{
    TAlbum *novo = NULL;
    novo = (TAlbum *)malloc(sizeof(TAlbum));
    if (!novo)
        return NULL;
    novo->prox = NULL;
    novo->fig = NULL;
    return novo;
}

void alocaAlbum(TCabeca *cabecaAlbum)
{
    TAlbum *novo = NULL;
    novo = criaAlbum();
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
    printf("\n###############\n");
    printf("%d %d %s %s\n", novo->cod_selecao, novo->numero_jogador, novo->nome, novo->selecao);
    return novo;
}

void insereFigurinhaAlbum(TAlbum *album, TSelecao *structSelecao, TJogador *structJogador)
{
    TFig *novo = NULL;
    novo = alocaFigurinhaAlbum(structSelecao, structJogador);
    TFig *aux = NULL;
    aux = album->fig;
    if (!album)
        return;
    if (!(album->fig))
    {
        album->fig = novo;
        return;
    }
    else
    {
        while (aux->cod_selecao < novo->cod_selecao && aux->prox != NULL)
            aux = aux->prox;
        if (!(aux->prox))
            aux->prox = novo;
        else
        {
            TFig *aux2 = NULL;
            while (aux->numero_jogador < novo->numero_jogador && aux->prox->cod_selecao == novo->cod_selecao)
            {
                aux2 = aux;
                aux = aux->prox;
            }
            if (aux->prox)
            {
                novo->prox = aux2->prox;
                aux2->prox = novo;
            }
            else
                aux->prox = novo;
        }
    }
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

int gerarNumero(int k)
{
    int x;
    time_t t;
    srand((unsigned)time(&t));
    x = (rand() % k) + 1;
    return x;
}