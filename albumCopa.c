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
    struct figurinha *ant;
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
void insereFigurinhaAlbum(TAlbum **album, int selecao, int jogador, TSelecao *listaselec, TJogador *listajog);
TJogador *alocaFigurinhaJogador(FILE *parquivo);
void lerSelecoes(FILE *parquivo, TSelecao **prim);
void lerFigurinhasJogadores(FILE *parquivo, TJogador **prim);
void imprimeListaSelecoes(TSelecao *p);
void imprimeListaFigurinhasJogadores(TJogador *p);
void imprimeAlbum(TFig *album);
TSelecao *buscaSelecao(TSelecao *listaselec, int selec);
TJogador *buscaJogador(TJogador *listajog, int jogador, int selecao);
int gerarNumero(int k, int cont);

int main()
{
    int op = 0, op2 = 0, opc, selec = 32, jog = 1, i = 0, cont = 0;
    TSelecao *selecao = NULL, *primSelecao = NULL;
    TJogador *jogador = NULL, *primJogador = NULL;
    FILE *arquivoSelecao, *arquivoFigurinhaJogador;
    TCabeca *cabeca = NULL;
    cabeca = criaCabecaAlbum();
    alocaAlbum(cabeca);

    arquivoSelecao = fopen("selecoes.txt", "r");
    arquivoFigurinhaJogador = fopen("figurinhas_total.txt", "r");
    while (!feof(arquivoSelecao))
        lerSelecoes(arquivoSelecao, &primSelecao);
    fclose(arquivoSelecao);
    // imprimeListaSelecoes(primSelecao);
    while (!feof(arquivoFigurinhaJogador))
        lerFigurinhasJogadores(arquivoFigurinhaJogador, &primJogador);
    fclose(arquivoFigurinhaJogador);
    // imprimeListaFigurinhasJogadores(primJogador);
    printf("\n########################\n");
    imprimeListaSelecoes(primSelecao);
    printf("#######################################\n");
    selec = 2;
    jog = 20;
    while (i < 50)
    {
        selec = gerarNumero(31, cont);
        cont += 1000;
        jog = gerarNumero(19, cont);
        cont += 1000;
        insereFigurinhaAlbum(&(cabeca->inicio), selec, jog, primSelecao, primJogador);
        printf("#######################################\n");
        imprimeAlbum(cabeca->inicio->fig);
        i++;
    }

    /*
    while (op != 7)
    {
        printf("1-Cria album\n2-Insere album\n3-\n7-sair\n");
        scanf("%d", &op);
        switch (op)
        {
        case 1:
        {
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
    novo->ant = NULL;
    printf("\n###############\n");
    printf("%d %d %s %s\n\n", novo->cod_selecao, novo->numero_jogador, novo->nome, novo->selecao);
    return novo;
}

void insereFigurinhaAlbum(TAlbum **album, int selecao, int jogador, TSelecao *listaselec, TJogador *listajog)
{
    TFig *novo = NULL;
    TSelecao *structSelecao = NULL;
    TJogador *structJogador = NULL;
    structSelecao = buscaSelecao(listaselec, selecao);
    structJogador = buscaJogador(listajog, jogador, selecao);
    novo = alocaFigurinhaAlbum(structSelecao, structJogador);
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
                while ((aux->prox != NULL) && (aux->prox->cod_selecao <= novo->cod_selecao))
                {
                    aux2 = aux;
                    aux->ant = aux;
                    aux = aux->prox;
                    aux->ant = aux2;
                }
                if (aux->cod_selecao == novo->cod_selecao)
                {
                    if (aux->prox && aux->numero_jogador >= novo->numero_jogador) // ULTIMA mudanca aki
                    {
                        if (aux->cod_selecao == novo->cod_selecao) // Arrumar o anterior que tá dando merda, só isso que tá dando errado >:(
                        {
                            aux->ant = aux2->ant->ant;
                            printf("desgraca %d %d\n", aux->numero_jogador, aux->ant->numero_jogador);
                        }
                        // # TODO: AKI PRECISA ARRUMAR.
                        if (aux->numero_jogador == aux->ant->numero_jogador)
                            return;
                        if (!(novo->numero_jogador < aux->numero_jogador && aux->ant->numero_jogador < novo->numero_jogador))
                        {
                            novo->prox = aux->ant;
                            aux->ant->ant = novo;
                            aux->ant->ant->prox = novo;
                            novo->ant = aux->ant->ant;
                        }
                        TFig *anterior = NULL;
                        anterior = aux->ant;
                        printf("ENTRA AKI %d %d %d\n", aux->numero_jogador, aux->ant->numero_jogador, novo->numero_jogador);
                        printf("Entrou 1\n");
                        novo->prox = aux;
                        aux->ant = novo;
                        anterior->prox = novo;
                        novo->ant = anterior;
                    }
                    else if (aux->prox)
                    {
                        printf("Entrou 2\n");
                        /*
                        if (aux->numero_jogador > novo->numero_jogador)
                        {
                            novo->prox = aux;
                            aux2->prox = novo;
                            return;
                        }
                        */
                        // else
                        if (aux->prox->cod_selecao != novo->cod_selecao) // Ultimo nome da selecao, prox muda de selecao
                        {
                            printf("Entrou aki, gloria!\n");
                            if (aux->numero_jogador > novo->numero_jogador)
                            {
                                printf("Capeta4\n");
                                novo->prox = aux;
                                aux->ant = novo;
                                aux->ant->prox = novo;
                                novo->ant = aux->ant;
                            }
                            else
                            {
                                printf("Capeta3\n");
                                novo->prox = aux->prox;
                                aux->prox->ant = novo;
                                aux->prox = novo;
                                novo->ant = aux;
                            }
                        }
                        else // entra se for a mesma seleção
                        {
                            while (aux->prox != NULL && aux->prox->cod_selecao == novo->cod_selecao)
                            {
                                printf("Ta entrando dentro do while\n");
                                if ((aux->numero_jogador < novo->numero_jogador && aux->prox->numero_jogador >= novo->numero_jogador)) // possivelmente a desgraça está aqui pra baixo
                                {
                                    printf("Chegou aki\n");
                                    novo->prox = aux->prox;
                                    aux->prox->ant = novo;
                                    aux->prox = novo;
                                    novo->ant = aux;
                                    return;
                                }
                                aux->ant = aux;
                                aux = aux->prox;
                            }
                            if (aux->prox)
                            {
                                printf("Chegou aki 2 %d %d\n", aux->numero_jogador, novo->numero_jogador);
                                novo->prox = aux->prox;
                                aux->prox->ant = novo;
                                aux->prox = novo;
                                novo->ant = aux;
                            }
                            else
                            {
                                printf("entrou depois do aki 2\n");
                                aux->prox = novo;
                                novo->ant = aux;
                            }
                        }
                    }
                    else
                    {
                        if (aux->numero_jogador >= novo->numero_jogador)
                        {
                            if (aux->numero_jogador == aux->ant->numero_jogador)
                                return;
                            printf("Entrou certo 234901283 %d %d %d\n", aux->numero_jogador, aux->ant->numero_jogador, novo->numero_jogador);
                            TFig *anterior = NULL;
                            anterior = aux->ant;
                            novo->prox = aux;
                            aux->ant = novo;
                            anterior->prox = novo;
                            novo->ant = anterior;
                        }
                        else
                        {
                            printf("Entrou 3\n");
                            aux->prox = novo;
                            novo->ant = aux;
                        }
                    }

                    /*
                    else
                    {
                        printf("Entrou 3\n");
                        aux->prox = novo;
                    }
                    */
                }
                else
                {
                    if (aux->prox)
                    {
                        printf("Nao ta entrando aki %d %d\n", aux->cod_selecao, novo->cod_selecao);
                        novo->prox = aux->prox;
                        aux->prox->ant = novo;
                        aux->prox = novo;
                        novo->ant = aux;
                    }
                    else
                    {
                        printf("Capeta\n");
                        aux->prox = novo;
                        novo->ant = aux;
                    }
                }
            }
            else
            {
                printf("Capeta2\n");
                aux->prox = novo;
                novo->ant = aux;
            }
        }
        else if (aux->cod_selecao == novo->cod_selecao) // Se a primeira figurinha já for a selecao correta // CONDICAO CORRETA!!!!
        {
            printf("Chegou aki?%d %d\n", aux->cod_selecao, novo->cod_selecao);
            if (aux->numero_jogador > novo->numero_jogador)
            {
                novo->prox = (*album)->fig;
                novo->prox->ant = novo;
                (*album)->fig = novo;
            }
            else if (aux->prox)
            {
                printf("Entrou na desgraca\n");
                while (aux->prox->cod_selecao == novo->cod_selecao && aux->prox != NULL)
                {
                    if ((aux->prox->numero_jogador > novo->numero_jogador))
                    {
                        printf("Existe\n");
                        novo->prox = aux->prox;
                        aux->prox->ant = novo;
                        aux->prox = novo;
                        novo->ant = aux;
                        return;
                    }
                    printf("entrou na merda do while\n");
                    aux->ant = aux;
                    aux = aux->prox;
                }
                if (!aux->prox)
                {
                    printf("capeta7\n");
                    aux->prox = novo;
                    novo->ant = aux;
                }
                else
                {
                    aux = aux->prox;
                    printf("Aki estava o problema?\n");
                    novo->prox = aux->prox;
                    aux->prox->ant = novo;
                    aux->prox = novo;
                    novo->ant = aux;
                }
            }
            else if (!aux->prox)
            {
                printf("capeta5\n");
                aux->prox = novo;
                novo->ant = aux;
            }
            else
            {
                printf("capeta6\n");
                novo->prox = aux->prox;
                aux->prox->ant = novo;
                aux->prox = novo;
                novo->ant = aux;
            }
        }
        else // Insere no inicio
        {
            printf("Aki?\n");
            novo->prox = (*album)->fig;
            novo->prox->ant = novo;
            (*album)->fig = novo;
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

int gerarNumero(int k, int cont)
{
    int x;
    time_t t;
    srand((unsigned)time(&t) + cont);
    x = (rand() % k) + 1;
    return x;
}