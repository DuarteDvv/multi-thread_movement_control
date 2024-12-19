#ifndef THREAD
#define THREAD

#include <vector>

using namespace std;

struct Thread_data // Estrutura com os dados das threads
{
   int id;
   int tempo_init;

   vector<pair<int, int>> rooms;

   Thread_data(int id, int tempo_init, vector<pair<int, int>> rooms) : id(id), tempo_init(tempo_init), rooms(rooms) {}
};

#endif