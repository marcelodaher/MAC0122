//slot.cpp

#include "slot.h"

Slot::Slot (int in, int du, int di)
{
    inicio = in;
    duracao = du;
    dia = di;
    return;
}

int Slot::getFim()
{
    return inicio + duracao;
}

bool Slot::interfere(Slot *s)
{
    if (dia == s->dia && s->inicio < getFim() && s->getFim() > inicio) return true;
    else return false;
}

bool Slot::antes(Slot *s)
{
    if (dia < s-> dia) return true;
    if (dia == s-> dia && inicio < s-> inicio) return true;
    return false;
}
