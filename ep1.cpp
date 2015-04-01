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
        
    public:
        void inicializa(int  c_me, int c_md, int m_me, int m_md, char mar_barco,
                        Estado *predec)
        {
            canibais_me = c_me;
            canibais_md = c_md;
            monges_me = m_me;
            monges_md = m_md;
            margem_barco = mar_barco;
            predecessor = predec;
            return;
        }
        
        bool seguro()
        {
            if ((canibais_me>monges_me || monges_me == 0)
             && (canibais_md>monges_md || monges_md == 0))
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
                if (canibais_md>2)
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
                if (canibais_me>2)
                {
                    e = new Estado;
                    e->inicializa(canibais_me-2,  canibais_md+2,  monges_me,
                                    monges_md, 'd', this);
                    if (e->seguro())
                        ar[validos++] = e;
                    else
                        delete e;
                }
                if (monges_md && canibais_md)
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

class Conjunto
{
    private:
        Celula *inicio;
        int tamanho;
        Celula *getCelula(int i) //iniciando contagem do elemento 0
        {
            int cont;
            Celula *c;
            c = inicio;
            for (cont=0; cont<i; cont++)
            {
                c = c->getProx();
            }
            return c;
        }
    public:
        Conjunto()
        {
            inicio = NULL;
            tamanho = 0;
        }
        Estado *elemento(int i)
        {
            Celula *c;
            c = getCelula(i);
            return c->getConteudo();
        }
        int getTamanho()
        {
            return tamanho;
        }
        void adiciona(Estado *e)
        {
            Celula *c, *ultima;
            c = new Celula(e);
            c->setConteudo(e);
            ultima = getCelula(tamanho);
            ultima->setProx(c);
            tamanho++;
            return;
        }
        int contem(Estado *e)
        {
            int i;
            Celula *c;
            Estado *e2;
            c = inicio;
            for (i = 0; i<tamanho; i++)
            {
                e2 = c->getConteudo();
                if (e->igual_a(e2))
                    return 1;
                c = c->getProx();
                
            }
            return 0;
        }
};

class Pilha
{
    private:
        Celula *top;
        int tamanho;
    public:
        Pilha()
        {
            top = NULL;
            tamanho = 0;
        }
        bool vazia()
        {
            if (tamanho)
                return FALSE;
            return TRUE;
        }
        void empilha(Estado *e)
        {
            Celula *c;
            c = new Celula(e);
            c->setProx(top);
            top = c;
            tamanho++;
            return;
        }
        Estado *topo()
        {
            return top->getConteudo();
        }
        void desempilha()
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
        void inicializa()
        {
            top = NULL;
            tamanho = 0;
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
    while (e->anterior() != NULL)
    {
        historico->empilha(e);
        e = e->anterior();
    }
    return historico;
}

int main(){
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
    me0 = ce0 = 3;
    mb0 = 'e';
    
    mdF = cdF = 3;
    meF = ceF = 0;
    mbF = 'd';
    
    analise = new Pilha;
    estadosPassados = new Conjunto;
    
    estadoInicial = new Estado;
    estadoInicial->inicializa(ce0, cd0, me0, md0, mb0, NULL);
    estadoFinal = new Estado;
    estadoFinal->inicializa(ceF, cdF, meF, mdF, mbF, NULL);
    
    analise->empilha(estadoInicial);
    
    while (!(terminou || analise->vazia()))
    {
        s = analise->topo()->sucessores(aray);
        analise->desempilha();
        for (i=0; i<s; i++)
        {
            if (estadoFinal->igual_a(aray[i]))
            {
                estadoFinal = aray[i];
                terminou = 1;
            }
            if (!(estadosPassados->contem(aray[i])))
                analise->empilha(aray[i]);
        }
    }
    
    if (terminou == 1)
    {
        historico = estadoFinal->geraHistorico();
        while (!historico->vazia())
        {
            historico->topo()->imprime();
            historico->desempilha();
        }
    }
    else cout<<"deu certo nao";
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    return 0;
}
