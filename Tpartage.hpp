#ifndef TPARTAGE_HPP
#define TPARTAGE_HPP

#include "mutex.hpp"
#include "singleton.hpp"
#include "sem.hpp"
#include <stdint.h>
#include <string.h>

class TPartage : public TSingleton<TPartage>
{
    friend class TSingleton<TPartage>;

private:
    bool etatAspi;
    double poidBalance;
    int nivA;
    int nivB;
    int nivC;
    bool echoAspi;

    bool etatAspiChange;
    bool poidBalanceChange;
    bool nivAChange;
    bool nivBChange;
    bool nivCChange;
    bool echoAspiChange;

public:
    TMutex mutex;

    TPartage();
    ~TPartage();

    void setAspi(bool etat);
    void setPoidBalance(double poid);
    void setNivA(int niv);
    void setNivB(int niv);
    void setNivC(int niv);
    void setEchoAspi(bool etat);

    bool getAspi(void);
    double getPoidBalance(void);
    int getNivA(void);
    int getNivB(void);
    int getNivC(void);
    bool getEchoAspi(void);

    bool AspiChange(void);
    bool poidChange(void);
    bool nivChange(char niv);
    bool echoChange(void);
};
;

#endif // TPARTAGE_HPP