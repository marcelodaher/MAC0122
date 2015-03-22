#DEFINE MAX_ESTADOS 1000

#include <fstream>
#include <stdexcept>
#include <iostream>

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
        
        int seguro()
        {
            if ((canibais_me>monges_me || monges_me == 0)
             && (canibais_md>monges_md || monges_md == 0))
                return 0;
            return 1;
        }
        
        int sucessores(Estados *ar)
        {
            int validos = 0;
            Estado *e;
            //m c mm cc mc
            if (margem_barco == "d")
            {
                if (monges_md)
                {
                    e = new Estado;
                    e->inicializa(canibais_me,  canibais_md,  monges_me+1,
                                    montes_md-1, "e", this);
                    if (e->seguro())
                        ar[validos++] = e;
                    else
                        delete e;
                }
                if (canibais_md)
                {
                    e = new Estado;
                    e->inicializa(canibais_me+1,  canibais_md-1,  monges_me,
                                    montes_md, "e", this);
                    if (e->seguro())
                        ar[validos++] = e;
                    else
                        delete e;
                }
                if (monges_md>=2)
                {
                    e = new Estado;
                    e->inicializa(canibais_me,  canibais_md,  monges_me+2,
                                    montes_md-2, "e", this);
                    if (e->seguro())
                        ar[validos++] = e;
                    else
                        delete e;
                }
                if (canibais_md>2)
                {
                    e = new Estado;
                    e->inicializa(canibais_me+2,  canibais_md-2,  monges_me,
                                    montes_md, "e", this);
                    if (e->seguro())
                        ar[validos++] = e;
                    else
                        delete e;
                }
                if (monges_md && canibais_md)
                {
                    e = new Estado;
                    e->inicializa(canibais_me+1,  canibais_md-1,  monges_me+1,
                                    montes_md-1, "e", this);
                    if (e->seguro())
                        ar[validos++] = e;
                    else
                        delete e;
                }
            }
            if (margem_barco == "e")
            {
                if (monges_me)
                {
                    e = new Estado;
                    e->inicializa(canibais_me,  canibais_md,  monges_me-1,
                                    montes_md+1, "d", this);
                    if (e->seguro())
                        ar[validos++] = e;
                    else
                        delete e;
                }
                if (canibais_me)
                {
                    e = new Estado;
                    e->inicializa(canibais_me-1,  canibais_md+1,  monges_me,
                                    montes_md, "d", this);
                    if (e->seguro())
                        ar[validos++] = e;
                    else
                        delete e;
                }
                if (monges_me>=2)
                {
                    e = new Estado;
                    e->inicializa(canibais_me,  canibais_md,  monges_me-2,
                                    montes_md+2, "d", this);
                    if (e->seguro())
                        ar[validos++] = e;
                    else
                        delete e;
                }
                if (canibais_me>2)
                {
                    e = new Estado;
                    e->inicializa(canibais_me-2,  canibais_md+2,  monges_me,
                                    montes_md, "d", this);
                    if (e->seguro())
                        ar[validos++] = e;
                    else
                        delete e;
                }
                if (monges_md && canibais_md)
                {
                    e = new Estado;
                    e->inicializa(canibais_me-1,  canibais_md+1,  monges_me-1,
                                    montes_md+1, "d", this);
                    if (e->seguro())
                        ar[validos++] = e;
                    else
                        delete e;
                }
            }
            return validos;
        }
        
        Estado predecessor()
        {
            return predecessor;
        }
}

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
        Estado getConteudo()
        {
            return conteudo;
        }
        Estado getProx()
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
}

class Lista
{
    private:
        Celula *inicio;
        int tamanho;
        Celula getCelula(int i) //iniciando contagem do elemento 0
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
        Lista()
        {
            inicio = NULL
            tamanho = 0;
        }
        Estado *elemento(int i)
        {
            Celula *c;
            c = getCelula(i);
            return c->getConteudo();
        }
        int tamanho()
        {
            return tamanho;
        }
        int adiciona(Estado *e)
        {
            c = new Celula(e);
            c->setConteudo(e);
            ultima = getCelula(tamanho);
            ultima->setProx(c);
            return tamanho++;
        }
        int pertence(Estado *e)
        {
            Celula c;
            c = inicio;
            for (i = 0; i<tamanho, i++)
            {
                e2 = c.getConteudo();
                if (*e.canibais_me == e2->canibais_me
                 && *e.canibais_md == e2->canibais_md
                 && *e.monges_me == e2->monges_me
                 && *e.monges_md == e2->monges_md
                 && *e.margem_barco == e2->margem_barco)
                    return 1;
                c = c->getProx();
                
            }
            return 0;
        }
}

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
        int vazia()
        {
            if (tamanho)
                return 0;
            return 1;
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
        void Desempilha()
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
}

int main(){
    Estado estados[MAX_ESTADOS]
    


    return 0;
}
