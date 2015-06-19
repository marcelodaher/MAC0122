//slot.h

class Slot
{
    private:
        int inicio, duracao;
        int dia; //1 = domingo, 7 = sabado
        int getFim();
    public:
        Slot (int in, int du, int di);
        bool interfere (Slot *s);
        bool antes (Slot *s);
};
