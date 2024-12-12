#include "task1.hpp"

TTask1::TTask1(const char *name, void *shared, int32_t policy, int32_t priority, int32_t cpu) : TThread(name, shared, policy, priority, cpu)
{
	std::string nameMqtt;

	screen = (TScreen *)shared;

	partage = TPartage::getInstance();

	nameMqtt = TMqtt::getMqttConfig();
	mqtt = new TMqtt(screen, "MqttAspi", nameMqtt.c_str());

	com1 = new TCom1(TCom1::getComConfig("com.def").c_str(), screen, 10, TCom::b9600, TCom::pNONE, TCom::dS8, 10000);
	com1->start();

	screen->dispStr(1, 10, "Task MQTT (CPU :  ) :");
	screen->dispStr(30, 10, nameMqtt.c_str());
}

TTask1::~TTask1()
{
	if (mqtt)
		delete mqtt;
}

void TTask1::task(void)
{
	// variable locale
	char strCar[2] = {'-', '\0'};
	bool aspiChange = false;
	bool poidChange = false;
	bool nivAChange = false;
	bool nivBChange = false;
	bool nivCChange = false;
	bool echoAspiChange = false;
	char sendMarche[3] = {'<', 'M', '>'};
	char sendArret[3] = {'<', 'A', '>'};
	char sendPoid[10];
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

		// lecture des changements
		aspiChange = partage->AspiChange();
		poidChange = partage->poidChange();
		nivAChange = partage->nivChange('A');
		nivBChange = partage->nivChange('B');
		nivCChange = partage->nivChange('C');
		echoAspiChange = partage->echoChange();

		// envoi des données
		if (aspiChange)
		{
			if (partage->getAspi())
			{
				com1->sendTx(sendMarche, sizeof(sendMarche));
			}
			else
			{
				com1->sendTx(sendArret, sizeof(sendArret));
			}
		}
		else
		{
			com1->sendTx(sendPoid, sizeof(sendPoid));
		}

		if (echoAspiChange == true)
		{
			if (partage->getEchoAspi() == true)
			{
				mqtt->publish(NULL, "RAM/shopvac/etats/sequence", 10, "PROCESSING", 0, false);
			}
			else
			{
				mqtt->publish(NULL, "RAM/shopvac/etats/sequence", 8, "FINISHED", 0, false);
			}
		}

		if (poidChange)
		{
			poid = partage->getPoidBalance();
			sprintf(sendPoid, "<P%07.1f>", poid);
			screen->dispStr(1, 6, sendPoid);
		}

		if (nivAChange)
		{
			mqtt->publish(NULL, "RAM/shopvac/etats/NivA", sizeof(partage->getNivA()), std::to_string(partage->getNivA()).c_str(), 0, false);
		}

		if (nivBChange)
		{
			mqtt->publish(NULL, "RAM/shopvac/etats/NivB", sizeof(partage->getNivB()), std::to_string(partage->getNivB()).c_str(), 0, false);
		}

		if (nivCChange)
		{
			mqtt->publish(NULL, "RAM/shopvac/etats/NivC", sizeof(partage->getNivC()), std::to_string(partage->getNivC()).c_str(), 0, false);
		}

		if (mqtt->loop(0) != 0)
			mqtt->reconnect();

		usleep(250000); // 250 ms
	}
}
