#include <iostream>
#include <chrono>

using namespace std;

#define RED 1
#define BLACK 0

struct No{
    int info;
    No *esq;
    No *dir;
    int cor;
};

typedef No* Arvore;


void cria_ArvRb(Arvore &);
void libera_ArvRb(Arvore* raiz);
void insere_ArvRb(Arvore* raiz,int valor);
void remove_ArvRb(Arvore *raiz, int valor);
int estaVazia_ArvRb(Arvore *raiz);
int altura_ArvRb(Arvore *raiz);
void preOrdem_ArvRb(Arvore);
void emOrdem_ArvRb(Arvore);
void posOrdem_ArvRb(Arvore);


void cria_ArvRb(Arvore &raiz) {
    raiz = nullptr;
}

void libera_NO(Arvore no){
    if(no == NULL)
        return;
    libera_NO(no->esq);
    libera_NO(no->dir);
    free(no);
    no = NULL;
}

void libera_ArvRb(Arvore* raiz){
    if(raiz == NULL)
        return;
    libera_NO(*raiz);
    free(raiz);
}

void encontraValor(Arvore raiz, int x, int *cont) {
    *cont += 1;
    if(raiz->info == x) return;

    if(x < raiz->info) encontraValor(raiz->esq, x, cont);
    else encontraValor(raiz->dir, x, cont);
}

Arvore rotacionaEsquerda(Arvore A){
    Arvore B = A->dir;
    A->dir = B->esq;
    B->esq = A;
    B->cor = A->cor;
    A->cor = RED;
    return B;
}

Arvore rotacionaDireita(Arvore A){
    Arvore B = A->esq;
    A->esq = B->dir;
    B->dir = A;
    B->cor = A->cor;
    A->cor = RED;
    return B;
}

int cor(Arvore H){
    if(H == NULL)
        return BLACK;
    else
        return H->cor;
}

void trocaCor(Arvore H){
    H->cor = !H->cor;
    if(H->esq != NULL)
        H->esq->cor = !H->esq->cor;
    if(H->dir != NULL)
        H->dir->cor = !H->dir->cor;
}

Arvore insereNO(Arvore H, int valor){
    if(H == NULL){
        Arvore novo;
        novo = (struct No*)malloc(sizeof(struct No));
        if(novo == NULL){
            return NULL;
        }

        novo->info = valor;
        novo->cor = RED;
        novo->dir = NULL;
        novo->esq = NULL;
        return novo;
    }

    if(valor < H->info)
        H->esq = insereNO(H->esq, valor);
    else
        H->dir = insereNO(H->dir, valor);

    //nó Vermelho é sempre filho à esquerda
    if(cor(H->dir) == RED && cor(H->esq) == BLACK)
        H = rotacionaEsquerda(H);

    //Filho e Neto são vermelhos
    //Filho vira pai de 2 n�s vermelhos
    if(cor(H->esq) == RED && cor(H->esq->esq) == RED)
        H = rotacionaDireita(H);

    //2 filhos Vermelhos: troca cor!
    if(cor(H->esq) == RED && cor(H->dir) == RED)
        trocaCor(H);

    return H;
}

void insere_ArvRb(Arvore* raiz, int valor){
    *raiz = insereNO(*raiz, valor);
    if((*raiz) != NULL)
        (*raiz)->cor = BLACK;

}

Arvore balancear(Arvore H){
    //nó Vermelho é sempre filho à esquerda
    if(cor(H->dir) == RED)
        H = rotacionaEsquerda(H);

    //Filho da esquerda e neto da esquerda são vermelhos
    if(H->esq != NULL && cor(H->esq) == RED && cor(H->esq->esq) == RED)
        H = rotacionaDireita(H);

    //2 filhos Vermelhos: troca cor!
    if(cor(H->esq) == RED && cor(H->dir) == RED)
        trocaCor(H);

    return H;
}

Arvore move2EsqRED(Arvore H){
    trocaCor(H);
    if(cor(H->dir->esq) == RED){
        H->dir = rotacionaDireita(H->dir);
        H = rotacionaEsquerda(H);
        trocaCor(H);
    }
    return H;
}

Arvore move2DirRED(Arvore H){
    trocaCor(H);
    if(cor(H->esq->esq) == RED){
        H = rotacionaDireita(H);
        trocaCor(H);
    }
    return H;
}

Arvore removerMenor(Arvore H){
    if(H->esq == NULL){
        free(H);
        return NULL;
    }
    if(cor(H->esq) == BLACK && cor(H->esq->esq) == BLACK)
        H = move2EsqRED(H);

    H->esq = removerMenor(H->esq);
    return balancear(H);
}


Arvore procuraMenor(Arvore atual){
    Arvore no1 = atual;
    Arvore no2 = atual->esq;
    while(no2 != NULL){
        no1 = no2;
        no2 = no2->esq;
    }
    return no1;
}

Arvore remove_NO(Arvore H, int valor){
    if(valor < H->info){
        if(cor(H->esq) == BLACK && cor(H->esq->esq) == BLACK)
            H = move2EsqRED(H);

        H->esq = remove_NO(H->esq, valor);
    }else{
        if(cor(H->esq) == RED)
            H = rotacionaDireita(H);

        if(valor == H->info && (H->dir == NULL)){
            free(H);
            return NULL;
        }

        if(cor(H->dir) == BLACK && cor(H->dir->esq) == BLACK)
            H = move2DirRED(H);

        if(valor == H->info){
            Arvore x = procuraMenor(H->dir);
            H->info = x->info;
            H->dir = removerMenor(H->dir);
        }else
            H->dir = remove_NO(H->dir, valor);
    }
    return balancear(H);
}

void remove_ArvRb(Arvore *raiz, int valor){
    Arvore h = *raiz;
    *raiz = remove_NO(h,valor);
    if(*raiz != NULL)
        (*raiz)->cor = BLACK;
}

int estaVazia_ArvRb(Arvore *raiz){
    if(raiz == NULL)
        return 1;
    if(*raiz == NULL)
        return 1;
    return 0;
}

int altura_ArvRb(Arvore *raiz){
    if (raiz == NULL)
        return 0;
    if (*raiz == NULL)
        return 0;
    int alt_esq = altura_ArvRb(&((*raiz)->esq));
    int alt_dir = altura_ArvRb(&((*raiz)->dir));
    if (alt_esq > alt_dir)
        return (alt_esq + 1);
    else
        return(alt_dir + 1);
}


void posOrdem_ArvRb(Arvore raiz){
    if(raiz == NULL)
        return;

    posOrdem_ArvRb(raiz->esq);
    posOrdem_ArvRb(raiz->dir);

    if(raiz->cor == RED) printf("%dR\n", raiz->info);
    else printf("%dB\n", raiz->info);
}

void emOrdem_ArvRb(Arvore raiz){
    if(raiz == NULL)
        return;

    emOrdem_ArvRb(raiz->esq);

    if(raiz->cor == RED) printf("%dR\n", raiz->info);
    else printf("%dB\n", raiz->info);

    emOrdem_ArvRb(raiz->dir);
}

void preOrdem_ArvRb(Arvore raiz){
    if(raiz == NULL)
        return;

    if(raiz->cor == RED) printf("%dR\n", raiz->info);
    else printf("%dB\n", raiz->info);

    preOrdem_ArvRb(raiz->esq);
    preOrdem_ArvRb(raiz->dir);
}

int main(){
    Arvore *raiz;
    cria_ArvRb(*raiz);

    int N;

    cout << "Digite a quantidade de elementos para teste: ";
    cin >> N;

    cout << "Digite os elementos: ";
    for (int i = 0; i < N; i++) {
        int aux;
        cin >> aux;
        insere_ArvRb(raiz, aux);
    }

    cout << "Percurso em ordem: ";
    emOrdem_ArvRb(*raiz);
    cout << endl << endl;

    return 0;
}
