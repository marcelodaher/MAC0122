#include <fstream>
#include <stdexcept>
#include <iostream>
#include <cmath>

#define TRUE (1==1)
#define FALSE (!TRUE)

int m, n;

using namespace std;

class Pilha;

class Estado
{
    private:
        int canibais_me, canibais_md, monges_me, monges_md;
        Estado *predecessor;
        char margem_barco;
        bool barcoEsquerdo;
        int valor;
    public:
        Estado(bool barcoEstaDoLadoEquerdo, int mongesDoLadoEsquerdo,
               int canibaisDoLadoEsquerdo, Estado *predec)
        {
            canibais_me = canibaisDoLadoEsquerdo;
            canibais_md = n - canibaisDoLadoEsquerdo;
            monges_me = mongesDoLadoEsquerdo;
            monges_md = m - mongesDoLadoEsquerdo;
            barcoEsquerdo = barcoEstaDoLadoEquerdo;
            if (barcoEstaDoLadoEquerdo)
                margem_barco = 'e';
            else margem_barco = 'd';
            predecessor = predec;
            calculaValor();
            return;
        }
        
        bool seguro()
        {
            if ((canibais_me>monges_me && monges_me != 0)
             || (canibais_md>monges_md && monges_md != 0))
                return FALSE;
            return TRUE;
        }
        
        int sucessores(Estado *ar[5])
        {
            int validos = 0;
            Estado *e;
            //m c mm cc mc
            if (!barcoEsquerdo)
            {
                if (monges_md)
                {
                    e = new Estado(!barcoEsquerdo, monges_me+1, canibais_me, this);
                    if (e->seguro())
                        ar[validos++] = e;
                    else
                        delete e;
                }
                if (canibais_md)
                {
                    e = new Estado(!barcoEsquerdo, monges_me, canibais_me+1, this);
                    if (e->seguro())
                        ar[validos++] = e;
                    else
                        delete e;
                }
                if (monges_md>=2)
                {
                    e = new Estado(!barcoEsquerdo, monges_me+2, canibais_me, this);
                    if (e->seguro())
                        ar[validos++] = e;
                    else
                        delete e;
                }
                if (canibais_md>=2)
                {
                    e = new Estado(!barcoEsquerdo, monges_me, canibais_me+2, this);
                    if (e->seguro())
                        ar[validos++] = e;
                    else
                        delete e;
                }
                if (monges_md && canibais_md)
                {
                    e = new Estado(!barcoEsquerdo, monges_me+1, canibais_me+1, this);
                    if (e->seguro())
                        ar[validos++] = e;
                    else
                        delete e;
                }
            }
            if (barcoEsquerdo)
            {
                if (monges_me)
                {
                    e = new Estado(!barcoEsquerdo, monges_me-1, canibais_me, this);
                    if (e->seguro())
                        ar[validos++] = e;
                    else
                        delete e;
                }
                if (canibais_me)
                {
                    e = new Estado(!barcoEsquerdo, monges_me, canibais_me-1, this);
                    if (e->seguro())
                        ar[validos++] = e;
                    else
                        delete e;
                }
                if (monges_me>=2)
                {
                    e = new Estado(!barcoEsquerdo, monges_me-2, canibais_me, this);
                    if (e->seguro())
                        ar[validos++] = e;
                    else
                        delete e;
                }
                if (canibais_me>=2)
                {
                    e = new Estado(!barcoEsquerdo, monges_me, canibais_me-2, this);
                    if (e->seguro())
                        ar[validos++] = e;
                    else
                        delete e;
                }
                if (monges_me && canibais_me)
                {
                    e = new Estado(!barcoEsquerdo, monges_me-1, canibais_me-1, this);
                    if (e->seguro())
                        ar[validos++] = e;
                    else
                        delete e;
                }
            }
            return validos;
        }
        
        Estado *anterior()
        {
            return predecessor;
        }
        
        void calculaValor()
        {
            valor = canibais_me + (n+1)*monges_me;
            if (margem_barco == 'd') valor += (m+1)*(n+1);
            return;
        }
        
        int getValor()
        {
            return valor;
        }
        
        bool operator==(Estado *e2)
        {
            return (getValor() == e2->getValor());
        }
        
        bool operator!=(Estado *e2)
        {
            return !(this == e2);
        }
        
        bool operator>(Estado *e2)
        {
            return (getValor()>e2->getValor());
        }
        
        bool operator<(Estado *e2)
        {
            return (e2>this);
        }
                
        Pilha *geraHistorico();
        void imprime();
};

class Celula
{
    private:
        Estado *conteudo;
        Celula *prox;
    public:
        Celula(Estado *e)
        {
            setConteudo(e);
            setProx(NULL);
            return;
        }
        Estado *getConteudo()
        {
            return conteudo;
        }
        Celula *getProx()
        {
            return prox;
        }
        void setConteudo(Estado *e)
        {
            conteudo = e;
            return;
        }
        void setProx(Celula *c)
        {
            prox = c;
            return;
        }
};

class No
{
    protected:
        Estado *conteudo;
        No *dir, *esq, *pai;
        int hDir, hEsq; //altura da arvore pra direita e esquerda
        int balance; //o quanto pesa a arvore para direita (+) ou esquerda (-)
    public:
        No(Estado *e)
        {
            setConteudo(e);
            setDir(NULL);
            setEsq(NULL);
            setPai(NULL);
            return;
        }
        Estado *getConteudo()
        {
            return conteudo;
        }
        No *getDir()
        {
            return dir;
        }
        No *getEsq()
        {
            return esq;
        }
        No *getPai()
        {
            return pai;
        }
        void setConteudo(Estado *e)
        {
            conteudo = e;
            return;
        }
        void setDir(No *direito)
        {
            dir = direito;
            return;
        }
        void setEsq(No *esquerdo)
        {
            esq = esquerdo;
            return;
        }
        void setPai(No *pai1)
        {
            pai = pai1;
            return;
        }
        int getBalance()
        {
            return balance;
        }
        int calcHDir()
        {
            if (dir == NULL)
            {
                hDir == 0;
            }
            else
            {
                hDir = 1 + max(dir->calcHEsq(), dir->calcHDir());
            }
            return hDir;
        }
        int calcHEsq()
        {
            if (esq == NULL)
            {
                hEsq == 0;
            }
            else
            {
                hEsq = 1 + max(esq->calcHEsq(), esq->calcHDir());
            }
            return hEsq;
        }
        int calcBalance()
        {
            setBalance(calcHDir() - calcHEsq());
            return getBalance();
        }
        void addBalance()
        {
            balance++;
            return;
        }
        void subBalance()
        {
            balance--;
            return;
        }
        void setBalance(int b)
        {
            balance = b;
            return;
        }
};

class ConjuntoOrdenado
{
    protected:
        int tamanho;
    public:
        int getTamanho()
        {
            return tamanho;
        };
        bool vazio()
        {
            if (tamanho==0) return TRUE;
            return FALSE;
        }
        virtual void adiciona(Estado *e){}
        virtual Estado *proximo(){}
        virtual void remove(){}
};

class Pilha: public ConjuntoOrdenado
{
    protected:
        Celula *top;
    public:
        Pilha()
        {
            top = NULL;
            tamanho = 0;
        }
        void adiciona(Estado *e)
        {
            Celula *c;
            c = new Celula(e);
            c->setProx(top);
            top = c;
            tamanho++;
            return;
        }
        Estado *proximo()
        {
            return top->getConteudo();
        }
        void remove()
        {
            if (tamanho == 0)
                {
                throw std::invalid_argument( "pilha secou" );
                return;
                }
            top = top->getProx();
            tamanho--;
            return;
        }
};

class Fila: public ConjuntoOrdenado
{
    protected:
        Celula *inicio, *fim;
        Celula *getCelula(int i) //iniciando contagem do elemento 0
        {
            int cont;
            Celula *c;
            c = inicio;
            for (cont=1; cont<i; cont++)
            {
                c = c->getProx();
            }
            return c;
        }
    public:
        Fila()
        {
            inicio = fim = NULL;
            tamanho = 0;
        }
        void adiciona(Estado *e)
        {
            Celula *c;
            c = new Celula(e);
            if (vazio())
                inicio = c;
            else
                fim->setProx(c);
            fim = c;
            tamanho++;
            return;
        }
        Estado *proximo()
        {
            return fim->getConteudo();
        }
        void remove()
        {
            inicio = inicio->getProx();
            tamanho--;
            return;
        }
};

class Conjunto
{
    protected:
        int tamanho;
        int itContem, itAdiciona, itTamanho;
    public:
        int getTamanho()
        {
            itTamanho++;
            return tamanho;
        }
        int iteracoesContem()
        {
            return itContem;
        }
        int iteracoesAdiciona()
        {
            return itAdiciona;
        }
        int iteracoesTamanho()
        {
            return itTamanho;
        }
        virtual void adiciona(Estado *e){}
        virtual bool contem(Estado *e){}
};

class ConjuntoLista: public Conjunto
{
    protected:
        Celula *inicio, *fim;
    public:
        ConjuntoLista()
        {
            inicio = NULL;
            tamanho = 0;
            itContem = itAdiciona = itTamanho = 0;
        }
        void adiciona(Estado *e)
        {
            itAdiciona++;
            Celula *c, *ultima;
            c = new Celula(e);
            if (getTamanho() == 0)
                inicio = c;
            else
                fim->setProx(c);
            fim = c;
            tamanho++;
            return;
        }
        bool contem(Estado *e)
        {
            int i;
            int t = getTamanho();
            Celula *c;
            Estado *e2;
            itContem++;
            c = inicio;
            for (i = 0; i<t; i++)
            {
                e2 = c->getConteudo();
                if (*e==e2)
                    return TRUE;
                c = c->getProx();
            }
            return FALSE;
        }
};

class ArvoreBinaria: public Conjunto
{
    private:
        No *inicio; //era o verbo (Joao 1:1)
    public:
        ArvoreBinaria()
        {
            inicio = NULL;
            tamanho = 0;
            itContem = itAdiciona = itTamanho = 0;
        }
        void adiciona(Estado *e)
        {
            No *node, *n1, *n2;
            bool dir;
            node = new No(e);
            if (!getTamanho())
                inicio = node;
            else
            {
                n1 = inicio;
                while (n1 != NULL)
                {
                    n2 = n1;
                    if (e>n1->getConteudo())
                        {
                        n1 = n1->getDir();
                        dir = TRUE;
                        }
                    else
                        {
                        n1 = n1->getEsq();
                        dir = FALSE;
                        }
                }
                if (dir)
                    n2->setDir(node);
                else
                    n2->setEsq(node);
            }
            tamanho++;
            itAdiciona++;
            return;
        }
        bool contem(Estado *e)
        {
            No *n1;
            bool contem = FALSE;
            n1 = inicio;
            while (n1 != NULL && contem == FALSE)
            {
                if (*e == n1->getConteudo())
                    contem = TRUE;
                else
                {
                    if (e > n1->getConteudo())
                        n1 = n1->getDir();
                    else
                        n1 = n1->getEsq();
                }
            }
            itContem++;
            return contem;
        }
};

class ArvoreAVL: public Conjunto
{
    private:
        No *inicio;
    public:
        ArvoreAVL()
        {
            inicio = NULL;
            tamanho = 0;
            itContem = itAdiciona = itTamanho = 0;
        }
        void adiciona(Estado *e)
        {
            No *node, *n1, *n2;
            bool dir;
            int bal;
            node = new No(e);
            if (!getTamanho())
                inicio = node;
            else
            {
                n1 = inicio;
                while (n1 != NULL)
                {
                    n2 = n1;
                    if (e>n1->getConteudo())
                        {
                        n1 = n1->getDir();
                        dir = TRUE;
                        }
                    else
                        {
                        n1 = n1->getEsq();
                        dir = FALSE;
                        }
                }
                if (dir)
                    n2->setDir(node);
                else
                    n2->setEsq(node);
                node->setPai(n2);
                while (n1 != NULL)
                {
                    n1 = node->getPai();
                    if (abs(n1->calcBalance()) > 1)
                    {
                        fixBalance(n1);
                    }
                }
            }
            tamanho++;
            itAdiciona++;
            return;
        }
        bool contem(Estado *e)
        {
            No *n1;
            bool contem = FALSE;
            n1 = inicio;
            while (n1 != NULL && contem == FALSE)
            {
                if (*e == n1->getConteudo())
                    contem = TRUE;
                else
                {
                    if (e > n1->getConteudo())
                        n1 = n1->getDir();
                    else
                        n1 = n1->getEsq();
                }
            }
            itContem++;
            return contem;
        }
        void rotateL(No *n)
        {
            No *temp;
            temp = n->getDir();
            if (temp->getEsq() != NULL)
                temp->getEsq()->setPai(n);
            if (n->getPai() == NULL)
                {
                    inicio = temp;
                    temp->setPai(NULL);
                }
            else if (n->getPai()->getEsq() == n)
                n->getPai()->setEsq(temp);
            else
                n->getPai()->setDir(temp);
            temp->setEsq(n);
            n->setPai(temp);
            return;
        }
        void rotateR(No *n)
        {
            No *temp;
            temp = n->getEsq();
            if (temp->getDir() != NULL)
                temp->getDir()->setPai(n);
            if (n->getPai() == NULL)
                {
                    inicio = temp;
                    temp->setPai(NULL);
                }
            else if (n->getPai()->getDir() == n)
                n->getPai()->setDir(temp);
            else
                n->getPai()->setEsq(temp);
            temp->setDir(n);
            n->setPai(temp);
            return;
        }
        void rotateLR(No *n)
        {
            rotateR(n->getEsq());
            rotateL(n);
            return;
        }
        void rotateRL(No *n)
        {
            rotateL(n->getDir());
            rotateR(n);
            return;
        }
        void fixBalance(No *n)
        {
            switch (n->getBalance())
            {
                case 2:
                    if (n->getDir()->getBalance()<0)
                            rotateLR(n);
                    else
                        rotateL(n);
                    break;
                case -2:
                    if (n->getEsq()->getBalance()>0)
                            rotateRL(n);
                    else
                        rotateR(n);
                    break;
            }
            n->calcBalance();
            return;
        }
};

class ConjuntoHash: public Conjunto
{

};

void Estado::imprime()
{
    int i;
    cout<<"\n";
    for (i=0; i<canibais_me; i++) cout<<"c";
    for (i=0; i<monges_me; i++) cout<<"m";
    if (margem_barco == 'e') cout<<"\\_/";
    cout<<" |~~| ";
    if (margem_barco == 'd') cout<<"\\_/";
    for (i=0; i<canibais_md; i++) cout<<"c";
    for (i=0; i<monges_md; i++) cout<<"m";
    cout<<"\n";
    return;
}


Pilha *Estado::geraHistorico()
{
    Pilha *historico;
    Estado *e;
    historico = new Pilha;
    e=this;
    while (e->anterior() != NULL)
    {
        historico->adiciona(e);
        e = e->anterior();
    }
    historico->adiciona(e);
    e = e->anterior();
    return historico;
}

int main(){
        
    int md0, me0, cd0, ce0; //monges e canibais por margem (inicial)
    int mdF, meF, cdF, ceF; //monges e canibais por margem (final)
    bool be0, beF; //barco esta na esquerda - inicial e final
    
    int s, i;
    int terminou = 0; //flag
    int tipoConjuntoOrdenado, tipoConjunto;
    
    Estado *estadoInicial;
    Estado *estadoFinal;
    Estado *aray[5];
    
    Pilha *historico;
        
    //interface
    cout << "\nQual tipo de conjunto ordenado deve ser utilizado?";
    cout << "\n1 - Pilha\n2-Fila\n";
    cin >> tipoConjuntoOrdenado;
    
    cout << "\nQual tipo de conjunto deve ser utilizado?";
    cout << "\n1 - ConjuntoLista\n2-ArvoreBinaria\n3-ArvoreAVL\n4-ConjuntoHash\n";
    cin >> tipoConjunto;
    
    cout << "\n\nQual o numero de monges?   ";
    cin >> m;
    cout << "\nQual o numero de canibais?    ";
    cin >> n;
    
    ConjuntoOrdenado *analise;
    switch(tipoConjuntoOrdenado) //define analise
    {
        case 1:
            analise = new Pilha;
            break;
        case 2:
            analise = new Fila;
            break;
    }
    
    Conjunto *estadosPassados;
    switch(tipoConjunto) //define estadosPassados
    {
        case 1:
            estadosPassados = new ConjuntoLista;
            break;
        case 2:
            estadosPassados = new ArvoreBinaria;
            break;
        case 3:
            estadosPassados = new ArvoreAVL;
            break;
        case 4:
            estadosPassados = new ConjuntoHash;
            break;
    }
        
    //VALORES TESTE
    md0 = cd0 = 0;
    me0 = m;
    ce0 = n;
    be0 = TRUE;

    mdF = m;
    cdF = n;
    meF = ceF = 0;
    beF = FALSE;

    estadoInicial = new Estado(be0, me0, ce0, NULL);
    estadoFinal = new Estado(beF, meF, ceF, NULL);

    analise->adiciona(estadoInicial);
    estadosPassados->adiciona(estadoInicial);
            
    while (!(terminou || analise->vazio()))
    {
        s = analise->proximo()->sucessores(aray);
        analise->remove();
        for (i=0; i<s; i++)
        {
            if (estadosPassados->contem(aray[i]))
                delete aray[i];
            else
            {
                estadosPassados->adiciona(aray[i]);
                analise->adiciona(aray[i]);
                if (*estadoFinal==aray[i])
                    {
                    estadoFinal = aray[i];
                    terminou = 1;
                }
            }
        }
    }
    
    if (terminou == 1)
    {
        historico = estadoFinal->geraHistorico();
        while (!historico->vazio())
        {
            historico->proximo()->imprime();
            historico->remove();
        }
    }
    else cout<<"deu certo nao\n";
    
    cout << "\nIteracoes Contem:    " << estadosPassados->iteracoesContem();
    cout << "\nIteracoes Adiciona:  " << estadosPassados->iteracoesAdiciona();
    cout << "\nIteracoes Tamanho:   " << estadosPassados->iteracoesTamanho();
    
    cout << "\n";
    
    
    
    
    
    
    
    
    return 0;
}
