#include "iostream"
#include "pthread.h"
#include "../include/passa_tempo.h"
#include "../include/Thread_data.h"

#define MAX_SALAS 10 // Maximo de salas definido pelo enunciado (fixo)

using namespace std;

pthread_mutex_t mutex_salas[MAX_SALAS];    // Mutex individual de cada sala
pthread_cond_t cond_trios[MAX_SALAS];      // Variavel condicional que monitora a formação de trios
pthread_cond_t cond_sala_vazia[MAX_SALAS]; // Variavel condicional que monitora o estado da sala que a Thread vai entrar

int ocupacao_salas[MAX_SALAS] = {0}; // Monitora quantas threads estão dentro das salas
int esperando_sala[MAX_SALAS] = {0}; // Monitora quantas threads estão esperando para entrar na sala

/*
A lógica da sincronização é baseada em tratar separadamente os gargalos das salas deixando um mutex e duas variaveis de condição para cada sala uma vez que
os processos feitos em salas diferentes não interferem entre si a não ser no momento de saida ou entrada, além disso dois arrays de inteiros são usados para monitorar
quantas threads estão esperando para entrar ou dentro das salas. O processo é para cada thread iteramos por cada sala da rota, executamos a função
entra que verifica se a sala esta vazia e caso não esteja espera até a notificação da função sai que lança um broadcast caso o numero de elementos dentro da sala chegue a zero.
Após passar pela verificação de vazio, ele espera até 3 threads estarem esperando e acorda duas para que possam entrar juntas. Em seguida ambas saem da thread repetindo o processo.
*/

void entra(int sala, int tid)
{
   pthread_mutex_lock(&mutex_salas[sala]);

   if (ocupacao_salas[sala] != 0) // Se a sala não esta vazia espere
   {
      pthread_cond_wait(&cond_sala_vazia[sala], &mutex_salas[sala]);
   }

   esperando_sala[sala]++;

   if (esperando_sala[sala] < 3) // Se não existe threads suficientes para um trio espera
   {
      pthread_cond_wait(&cond_trios[sala], &mutex_salas[sala]);
   }
   else // Avisa 2 threads para terem exatamente 3 acordadas
   {
      pthread_cond_signal(&cond_trios[sala]);
      pthread_cond_signal(&cond_trios[sala]);
   }

   ocupacao_salas[sala]++;
   esperando_sala[sala]--;

   pthread_mutex_unlock(&mutex_salas[sala]);
}

void sai(int sala, int tid)
{
   pthread_mutex_lock(&mutex_salas[sala]);

   ocupacao_salas[sala]--;

   if (ocupacao_salas[sala] == 0) // Se a sala esta vazia, avisa todas as threads esperando
   {
      pthread_cond_broadcast(&cond_sala_vazia[sala]);
   }

   pthread_mutex_unlock(&mutex_salas[sala]);
}

void *task(void *arg) // Taréfa a ser executada por cada thread
{
   Thread_data *td = (Thread_data *)arg;

   passa_tempo(td->id, 0, td->tempo_init);

   for (size_t sala = 0; sala < td->rooms.size(); sala++) // Para cada sala em rota
   {
      entra(td->rooms[sala].first, td->id); // Espera sala ficar vazia, espera trio, entra

      if (sala != 0)
      {
         sai(td->rooms[sala - 1].first, td->id); // Se a sala não for a primeira, sai e avisa threads quando estiver vazia
      }

      passa_tempo(td->id, td->rooms[sala].first, td->rooms[sala].second);
   }

   sai(td->rooms[td->rooms.size() - 1].first, td->id); // sai da ultima sala

   return NULL;
}

int main()
{
   int n_salas, n_threads;

   cin >> n_salas >> n_threads;

   vector<pthread_t> threads(n_threads);              // Vetor de Threads
   vector<Thread_data *> thread_data_list(n_threads); // Vetor de dados da Thread

   for (int i = 0; i < n_salas; i++) // Inicialização
   {
      pthread_mutex_init(&mutex_salas[i], NULL);
      pthread_cond_init(&cond_trios[i], NULL);
      pthread_cond_init(&cond_sala_vazia[i], NULL);
   }

   for (int i = 0; i < n_threads; i++) // Lê e armazena os dados de cada Thread
   {
      int id_thread, tempo_init, n_visitas;
      cin >> id_thread >> tempo_init >> n_visitas;

      vector<pair<int, int>> quartos; // Vetor que armazena a rota da thread sequencialmente (0-N) par id-sala e tempo que fica na sala

      for (int j = 0; j < n_visitas; j++)
      {
         int id_sala, tempo_min;
         cin >> id_sala >> tempo_min;
         quartos.push_back({id_sala, tempo_min});
      }

      thread_data_list[i] = new Thread_data(id_thread, tempo_init, quartos);

      pthread_create(&threads[i], NULL, task, (void *)thread_data_list[i]); // Lança a thread
   }

   for (int i = 0; i < n_threads; i++) // Desalocando daddos
   {
      pthread_join(threads[i], NULL);
      delete thread_data_list[i];
   }

   for (int i = 0; i < n_salas; i++) // Desalocando mutex e condições
   {
      pthread_mutex_destroy(&mutex_salas[i]);
      pthread_cond_destroy(&cond_trios[i]);
      pthread_cond_destroy(&cond_sala_vazia[i]);
   }

   return 0;
}
