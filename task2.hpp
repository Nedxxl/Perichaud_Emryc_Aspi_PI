#ifndef TTask2_HPP
#define TTask2_HPP

#include "thread.hpp"
#include "Tpartage.hpp"
#include "screen.hpp"
#include "TCom1.hpp"

class TTask2 : public TThread
{
private:
	TScreen *screen;
	TPartage *partage;
	TCom1 *com1;
	
public : TTask2(const char *name, void *shared, int32_t policy, int32_t priority, int32_t cpu = -1);
	~TTask2();

	void task(void);
};

#endif // TTask2_HPP
