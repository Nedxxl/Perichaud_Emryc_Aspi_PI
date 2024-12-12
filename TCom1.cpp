#include "TCom1.hpp"

TCom1::TCom1(const char *name, void *shared, int priority, baudrate_t baudRate, parity_t parity, dimData_t dimData, int32_t timeoutRxMs)
	: TCom(name, shared, priority, baudRate, parity, dimData, timeoutRxMs), screen(nullptr)
{
	screen = (TScreen *)shared;
	screen->dispStr(1, 2, name);
	screen->dispStr(1, 3, com);

}

TCom1::~TCom1()
{
}

void TCom1::rxCar(unsigned char car)
{
	static enum { WAIT_START,
				  RECEIVE_DATA,
				  PERCENT_RECIVED,
				  WAIT_END } state = WAIT_START;

	static char nivA[4] = {' ', ' ', ' ', '\0'};
	static char nivB[4] = {' ', ' ', ' ', '\0'};
	static char nivC[4] = {' ', ' ', ' ', '\0'};
	static int indexBuffer = 0;
	static int indexNiv = 0;

	switch (state)
	{
	case WAIT_START:
		if (car == '<')
		{
			state = RECEIVE_DATA;
		}
		indexBuffer = 0;
		indexNiv = 0;
		memset(nivA, ' ', 3);
		memset(nivB, ' ', 3);
		memset(nivC, ' ', 3);
		break;

	case RECEIVE_DATA:
		switch (car)
		{
		case 'M':
			if (partage->getEchoAspi() == false)
			{
				partage->setEchoAspi(true);
				screen->dispStr(1, 5, "Aspi : ON ");
			}
			state = WAIT_END;
			break;

		case 'P':
			state = PERCENT_RECIVED;
			break;

		case 'A':
			if (partage->getEchoAspi() == true)
			{
				partage->setEchoAspi(false);
				screen->dispStr(1, 5, "Aspi : OFF");
			}
			state = WAIT_END;
			break;

		case 'D':
			if (partage->getEchoAspi() == true)
			{
				partage->setEchoAspi(false);
				screen->dispStr(1, 5, "Aspi : OFF");
			}
			state = WAIT_END;
			break;

		default:
			screen->dispStr(1, 8, std::to_string(car).c_str());
			state = WAIT_END;
			break;
		}

		break;
	case PERCENT_RECIVED:
		if ((car == '.') || (indexBuffer >= 3))
		{
			indexNiv++;
			indexBuffer = 0;
		}
		else if (car == '>')
		{
			if (partage->getNivA() != atoi(nivA))
			{
				partage->setNivA(atoi(nivA));
				screen->dispStr(1, 7, nivA);
			}

			if (partage->getNivB() != atoi(nivB))
			{
				partage->setNivB(atoi(nivB));
				screen->dispStr(1, 8, nivB);
			}

			if (partage->getNivC() != atoi(nivC))
			{
				partage->setNivC(atoi(nivC));
				screen->dispStr(1, 9, nivC);
			}
			state = WAIT_START;
		}
		else
		{
			switch (indexNiv)
			{
			case 0:
				nivA[indexBuffer++] = car;
				break;
			case 1:
				nivB[indexBuffer++] = car;
				break;
			case 2:
				nivC[indexBuffer++] = car;
				break;
			default:
				screen->dispStr(1, 10, std::to_string(car).c_str());
				break;
			}
		}
		break;

	case WAIT_END:
		if (car == '>')
		{
			state = WAIT_START;
		}
		break;
	}
}

void TCom1::rxTimeout(void)
{
	if (screen)
	{
		screen->dispStr(1, 4, "Receive timeout.");
	}
}

std::string TCom1::getComConfig(std::string nameFichierConfig)
{
	TConfiguration config(nameFichierConfig);
	return config.getParametreConfiguration();
}