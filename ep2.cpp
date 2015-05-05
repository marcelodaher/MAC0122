#include <fstream>
#include <stdexcept>
#include <iostream>

#define TRUE (1==1)
#define FALSE (!TRUE)

using namespace std;

class Pilha;

class Estado
{
    private:
        int canibais_me, canibais_md, monges_me, monges_md;
        Estado *predecessor;
        char margem_barco;
        int valor;
    public:
        Estado(bool barcoEstaDoLadoEquerdo, int mongesDoLadoEsquerdo,
               int canibaisDoLadoEsquerdo, Estado *predec)
        {
            canibais_me = canibaisDoLadoEsquerdo;
            canibais_md = n - canibaisDoLadoEsquerdo;
            monges_me = mongesDoLadoEsquerdo;
            monges_md = m - mongesDoLadoEsquerdo;
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
            if (margem_barco == 'd')
            {
                if (monges_md)
                {
                    e = new Estado;
                    e->inicializa(canibais_me,  canibais_md,  monges_me+1,
                                    monges_md-1, 'e', this);
                    if (e->seguro())
                        ar[validos++] = e;
                    else
                        delete e;
                }
                if (canibais_md)
                {
                    e = new Estado;
                    e->inicializa(canibais_me+1,  canibais_md-1,  monges_me,
                                    monges_md, 'e', this);
                    if (e->seguro())
                        ar[validos++] = e;
                    else
                        delete e;
                }
                if (monges_md>=2)
                {
                    e = new Estado;
                    e->inicializa(canibais_me,  canibais_md,  monges_me+2,
                                    monges_md-2, 'e', this);
                    if (e->seguro())
                        ar[validos++] = e;
                    else
                        delete e;
                }
                if (canibais_md>=2)
                {
                    e = new Estado;
                    e->inicializa(canibais_me+2,  canibais_md-2,  monges_me,
                                    monges_md, 'e', this);
                    if (e->seguro())
                        ar[validos++] = e;
                    else
                        delete e;
                }
                if (monges_md && canibais_md)
                {
                    e = new Estado;
                    e->inicializa(canibais_me+1,  canibais_md-1,  monges_me+1,
                                    monges_md-1, 'e', this);
                    if (e->seguro())
                        ar[validos++] = e;
                    else
                        delete e;
                }
            }
            if (margem_barco == 'e')
            {
                if (monges_me)
                {
                    e = new Estado;
                    e->inicializa(canibais_me,  canibais_md,  monges_me-1,
                                    monges_md+1, 'd', this);
                    if (e->seguro())
                        ar[validos++] = e;
                    else
                        delete e;
                }
                if (canibais_me)
                {
                    e = new Estado;
                    e->inicializa(canibais_me-1,  canibais_md+1,  monges_me,
                                    monges_md, 'd', this);
                    if (e->seguro())
                        ar[validos++] = e;
                    else
                        delete e;
                }
                if (monges_me>=2)
                {
                    e = new Estado;
                    e->inicializa(canibais_me,  canibais_md,  monges_me-2,
                                    monges_md+2, 'd', this);
                    if (e->seguro())
                        ar[validos++] = e;
                    else
                        delete e;
                }
                if (canibais_me>=2)
                {
                    e = new Estado;
                    e->inicializa(canibais_me-2,  canibais_md+2,  monges_me,
                                    monges_md, 'd', this);
                    if (e->seguro())
                        ar[validos++] = e;
                    else
                        delete e;
                }
                if (monges_me && canibais_me)
                {
                    e = new Estado;
                    e->inicializa(canibais_me-1,  canibais_md+1,  monges_me-1,
                                    monges_md+1, 'd', this);
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
            if (margem_barco == 'd') valor += (m+1)*(n=1);
            return;
        }
        
        bool igual_a(Estado *e2)
        {
            if (canibais_me == e2->canibais_me
                 && canibais_md == e2->canibais_md
                 && monges_me == e2->monges_me
                 && monges_md == e2->monges_md
                 && margem_barco == e2->margem_barco)
                return TRUE;
            return FALSE;
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
    private:
        Estado *conteudo;
        No *dir, *esq;
    public:
        No(Estado *e)
        {
            setConteudo(e);
            setDir(NULL);
            setEsq(NULL);
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
};

class ConjuntoOrdenado
{
    protected:
        int tamanho;
    public:
        bool vazio()
        {
            if (tamanho==0) return TRUE;
            return FALSE;
        }
        void adiciona(Estado *e);
        Estado *proximo();
        void remove();
};

class Pilha: public ConjuntoOrdenado
{
    protected:
        celula *top;
    public:
        Pilha()
        {
            top = NULL;
            tamanho = 0;
        }
        bool vazia()
        {
            return vazio();
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
        int tamanho;
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
        bool vazia()
        {
            if (tamanho == 0)
                return TRUE;
            return FALSE;
        }
        void adiciona(Estado *e)
        {
            Celula *c;
            c = new Celula(e);
            if (vazia())
                inicio = c;
            else
                fim->setProx(c);
            fim = c;
            tamanho ++;
            return;
        }
        Estado *proximo()
        {
            return fim->getConteudo();
        }
        void remove()
        {
            inicio = inicio->getProx();
            tamanho --;
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
        void adiciona(Estado *e);
        int contem(Estado *e);
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
        Estado *elemento(int i)
        {
            Celula *c;
            c = getCelula(i);
            return c->getConteudo();
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
            Celula *c;
            Estado *e2;
            itContem++;
            c = inicio;
            for (i = 0; i<getTamanho(); i++)
            {
                e2 = c->getConteudo();
                if (e->igual_a(e2))
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
                if (e == n1->getConteudo())
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
            return bool;
        }
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
    
    static int m, n;
    
    int md0, me0, cd0, ce0; //monges e canibais por margem (inicial)
    int mdF, meF, cdF, ceF; //monges e canibais por margem (final)
    char mb0, mbF; //margem do barco inicial e final
    
    int s, i;
    int terminou = 0; //flag
    
    Estado *estadoInicial;
    Estado *estadoFinal;
    Estado *aray[5];
    
    Pilha *analise, *historico;
    Conjunto *estadosPassados;
    
    //VALORES TESTE
    md0 = cd0 = 0;
    me0 = m;
    ce0 = n;
    mb0 = 'e';
    
    mdF = m;
    cdF = n;
    meF = ceF = 0;
    mbF = 'd';
    
    analise = new Pilha;
    estadosPassados = new Conjunto;
    
    estadoInicial = new Estado;
    estadoInicial->inicializa(ce0, cd0, me0, md0, mb0, NULL);
    estadoFinal = new Estado;
    estadoFinal->inicializa(ceF, cdF, meF, mdF, mbF, NULL);
    
    analise->empilha(estadoInicial);
    estadosPassados->adiciona(estadoInicial);
    
    while (!(terminou || analise->vazia()))
    {
        s = analise->topo()->sucessores(aray);
        analise->desempilha();
        for (i=0; i<s; i++)
        {
                //aray[i]->imprime();
            if (estadosPassados->contem(aray[i]))
                delete aray[i];
            else
            {
                estadosPassados->adiciona(aray[i]);
                analise->empilha(aray[i]);
                if (estadoFinal->igual_a(aray[i]))
                    {
                    estadoFinal = aray[i];
                    terminou = 1;
                }
            }
        }
    }
    
    cout<<"aieee\n";
    if (terminou == 1)
    {
        historico = estadoFinal->geraHistorico();
        while (!historico->vazia())
        {
            historico->topo()->imprime();
            historico->desempilha();
        }
    }
    else cout<<"deu certo nao\n";
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    return 0;
}
