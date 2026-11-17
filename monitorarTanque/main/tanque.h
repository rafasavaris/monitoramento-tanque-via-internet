#ifndef __tanque__
#define __tanque__

class Tanque {
    private:
        float alturaTanque;
        float areaBase;
         
    public:
        Tanque(float altura, float area);
        float getAlturaColuna(float distancia);
        float getLitros(float distancia);
};

#endif