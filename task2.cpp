#include "task2.hpp"

TTask2::TTask2(const char *name, void *shared, int32_t policy, int32_t priority, int32_t cpu) : TThread(name, shared, policy, priority, cpu)
{
	std::string nameMqtt;

	screen = (TScreen *)shared;

	partage = TPartage::getInstance();

	com1 = new TCom1(TCom1::getComConfig("com.def").c_str(), screen, 10, TCom::b9600, TCom::pNONE, TCom::dS8, 10000);
	com1->start();

	screen->dispStr(1, 10, "Task MQTT (CPU :  ) :");
	screen->dispStr(30, 10, nameMqtt.c_str());
}

TTask2::~TTask2()
{
}

void TTask2::task(void)
{
	// variable locale
	char strCar[2] = {'-', '\0'};
	char sendMarche[4] = {'<', 'M', '>', '\0'};
	char sendArret[4] = {'<', 'A', '>', '\0'};
	char sendPoid[11];
	double poid;

	// synchronisation démarrage tâche
	signalStart();

	sleep(2);

	while (1)
	{
		// traitement
		if (strCar[0] == '|')
			strCar[0] = '-';
		else
			strCar[0] = '|';
		screen->dispStr(23, 11, strCar);

		// envoi des données
		if (partage->getAspi() && !partage->getEchoAspi())
		{
			com1->sendTx(sendMarche, strlen(sendMarche));
		}
		else if (!partage->getAspi() && partage->getEchoAspi())
		{
			com1->sendTx(sendArret, strlen(sendArret));
		}

		else
		{
			poid = partage->getPoidBalance();
			sprintf(sendPoid, "<P%07.1f>", poid);
			com1->sendTx(sendPoid, strlen(sendPoid));
			screen->dispStr(1, 6, sendPoid);
		}

		sleep(2); // 1s
	}
}
