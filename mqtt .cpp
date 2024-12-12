#include "mqtt.hpp"
#include "Tconfig.hpp"
#include "Tpartage.hpp"

using namespace std;

TMqtt::TMqtt(void *shared, const char *id, const char *host, int port) : mosqpp::mosquittopp(id)
{
	screen = (TScreen *)shared;
	partage = TPartage::getInstance();

	mqttTable["RAM/shopvac/cmd/force"] = &TMqtt::aspiEtat;
	mqttTable["RAM/balance/etats/poids"] = &TMqtt::poidBalance;
	mqttTable["RAM/melangeur/etats/recetteStatut"] = &TMqtt::recetteEtat;
	

	mosqpp::lib_init();

	connect(host, port, 120);
}

TMqtt::~TMqtt()
{
	mosqpp::lib_cleanup();
}

void TMqtt::on_connect(int rc)
{
	if (rc == 0)
	{
		screen->dispStr(1, 13, "Connected : OK  ");

		subscribe(NULL, "RAM/#", 0);
	}
	else
		screen->dispStr(1, 13, "Connected : Fail");
}

void TMqtt::on_message(const struct mosquitto_message *message)
{
	string topic = message->topic;
	string payload = (char *)message->payload;

	screen->dispStr(1, 12, (char *)message->topic);
	screen->dispStr(15, 12, "    ");
	screen->dispStr(15, 12, (char *)message->payload);

	auto it = mqttTable.find(topic);
	if (it != mqttTable.end())
		it->second(this, payload);
	/*
	#if 0
		if(topic.substr(0,7) == "module/")
			{
			if(topic.substr(7,8) == "1")
				{
				partage.setModule1( (payload == "on")? true : false );messageModule1
				}
			else if(topic.substr(7,8) == "2")
				{
				partage.setModule2( (payload == "on")? true : false );
				}
			else if(topic.substr(7,8) == "3")
				{
				partage.setModule3( (payload == "on")? true : false );
				}
			else if(topic.substr(7,8) == "4")
				{
				partage.setModule4( (payload == "on")? true : false );
				}
			else if(topic.substr(7,8) == "5")
				{
				partage.setModule5( (payload == "on")? true : false );
				}
			else if(topic.substr(7,8) == "6")bool
				{
				partage.setModule6( (payload == "on")? true : false );
				}
			}
	#endif
	*/
}

void TMqtt::on_subscribe(int mid, int qos_count, const int *granted_pos)
{
	screen->dispStr(1, 14, "on_subcribe");
}

void TMqtt::aspiEtat(string val)
{
	if(val == "GO")
		partage->setAspi(true);
	else if(val == "STOP")
		partage->setAspi(false);
}

void TMqtt::poidBalance(string val)
{
	double poids = std::stod(val);
	partage->setPoidBalance(poids);
}

void TMqtt::recetteEtat(string val)
{
	if(val == "FINISHED")
		partage->setAspi(true);
}

string TMqtt::getMqttConfig(string nameFichierConfig)
{
	TConfiguration config(nameFichierConfig);

	return config.getParametreConfiguration();
}
