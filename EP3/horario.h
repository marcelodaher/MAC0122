//horario.h

#include "evento.h"

class Horario
{
    private:
        
    public:
       bool novoSlot(Slot *s, String nomeDoEvento);
       void removeSlot(Slot *s, String nomeDoEvento);
       
};
