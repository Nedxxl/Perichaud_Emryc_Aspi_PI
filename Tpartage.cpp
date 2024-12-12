#include "Tpartage.hpp"

TPartage::TPartage()
{
    etatAspi = false;
    poidBalance = 0;
    nivA = 0;
    nivB = 0;
    nivC = 0;
    echoAspi = false;

    etatAspiChange = false;
    poidBalanceChange = false;
    nivAChange = false;
    nivBChange = false;
    nivCChange = false;
    echoAspiChange = false;
}

TPartage::~TPartage()
{
}

void TPartage::setAspi(bool etat)
{
    mutex.take();
    if(etatAspi != etat)
    {
        etatAspi = etat;
        etatAspiChange = true;
    }
    mutex.release();
}

void TPartage::setPoidBalance(double poid)
{
    mutex.take();
    if(poidBalance != poid)
    {
        poidBalance = poid;
        poidBalanceChange = true;
    }
    mutex.release();
}

void TPartage::setNivA(int niv)
{
    mutex.take();
    nivA = niv;
    nivAChange = true;
    mutex.release();
}

void TPartage::setNivB(int niv)
{
    mutex.take();
    nivB = niv;
    nivBChange = true;
    mutex.release();
}

void TPartage::setNivC(int niv)
{
    mutex.take();
    nivC = niv;
    nivCChange = true;
    mutex.release();
}

void TPartage::setEchoAspi(bool etat)
{
    mutex.take();
    echoAspi = etat;
    echoAspiChange = true;
    mutex.release();
}

bool TPartage::getAspi(void)
{
    bool etat;

    mutex.take();
    etat = etatAspi;
    mutex.release();

    return etat;
}

double TPartage::getPoidBalance(void)
{
    double poid;

    mutex.take();
    poid = poidBalance;
    mutex.release();

    return poid;
}

int TPartage::getNivA(void)
{
    int niv;

    mutex.take();
    niv = nivA;
    mutex.release();

    return niv;
}

int TPartage::getNivB(void)
{
    int niv;

    mutex.take();
    niv = nivB;
    mutex.release();

    return niv;
}

int TPartage::getNivC(void)
{
    int niv;

    mutex.take();
    niv = nivC;
    mutex.release();

    return niv;
}

bool TPartage::getEchoAspi(void)
{
    bool etat;

    mutex.take();
    etat = echoAspi;
    mutex.release();

    return etat;
}

bool TPartage::nivChange(char niv)
{
    bool change;

    mutex.take();
    switch (niv)
    {
    case 'A':
        change = nivAChange;
        nivAChange = false;
        break;

    case 'B':
        change = nivBChange;
        nivBChange = false;
        break;

    case 'C':
        change = nivCChange;
        nivCChange = false;
        break;

    default:
        change = false;
        break;
    }
    mutex.release();

    return change;
}

bool TPartage::AspiChange(void)
{
    bool change;

    mutex.take();
    change = etatAspiChange;
    etatAspiChange = false;
    mutex.release();

    return change;
}

bool TPartage::poidChange(void)
{
    bool change;

    mutex.take();
    change = poidBalanceChange;
    poidBalanceChange = false;
    mutex.release();

    return change;
}

bool TPartage::echoChange(void)
{
    bool change;

    mutex.take();
    change = echoAspiChange;
    echoAspiChange = false;
    mutex.release();

    return change;
}
