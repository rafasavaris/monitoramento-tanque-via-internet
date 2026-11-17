#include "tanque.h"

Tanque::Tanque(float altura, float area)
{
    alturaTanque = altura;
    areaBase = area;
}

float Tanque::getAlturaColuna(float distancia)
{
    if (distancia < 0) { return -1.0f; }

    float alturaAgua = alturaTanque - distancia;

    if (alturaAgua < 0) {
        alturaAgua = 0;
    }

    if (alturaAgua > alturaTanque) {
        alturaAgua = alturaTanque;
    }

    return alturaAgua;
}

float Tanque::getLitros(float distancia)
{
    float alturaAgua = getAlturaColuna(distancia);

    if (alturaAgua < 0) { return -1.0f; }

    return (areaBase * alturaAgua) / 1000.0f;
}