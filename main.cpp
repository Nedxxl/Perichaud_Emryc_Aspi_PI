
//============================================================================
// Name        : TemplateThreadCom.cpp
// Author      : SG
// Version     :
// Copyright   : Your copyright notice
// Description : Template Thread Com
//============================================================================

#include "clavier.hpp"
#include "screen.hpp"
#include "Tpartage.hpp"
#include "TCom1.hpp"
#include "task1.hpp"
#include "task2.hpp"

int main(int argc, char *argv[])
{
  char car;

  // Initialisation task Principal
  TThread::initTaskMain(SCHED_FIFO, 0);

  // Création Clavier et console
  TClavier *clavier = TClavier::getInstance();
  TScreen *screen = new TScreen();
  TTask1 *task1 = new TTask1("Task1", screen, SCHED_FIFO, 80);
  TTask2 *task2 = new TTask2("Task2", screen, SCHED_FIFO, 90);
  screen->start();

  // Création tâches

  // Démarrage tâches
  task1->start();
  task2->start();
  // com1->setSignalTimeout(1);
  //  Traitement tâche principale
  screen->dispStr(1, 1, "Test Com (SG  09/09/2024)");
  do
  {
    // Traitement

    if (clavier->kbhit())
    {
      car = clavier->getch();
    }
  } while ((car != '|') && (car != '~'));

  // Destruction tâches
    if (task1)
    delete task1;

  if (task2)
    delete task2;

  if (screen)
    delete screen;

  return 0;

  // Destru    char str;
}