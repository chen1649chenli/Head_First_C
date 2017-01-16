#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

int beers = 2000000;
pthread_mutex_t beers_lock = PTHREAD_MUTEX_INITIALIZER;
void error(char *msg){
  fprintf(stderr, "%s: %s\n", msg, strerror(errno));
  exit(1);
}

void* drink_lots(){
  int i;
  for (i = 0;i < 100000; i++){
    pthread_mutex_lock(&beers_lock);
    beers = beers-1;
    pthread_mutex_unlock(&beers_lock);
  }
  printf("beers = %i\n",beers);
  return NULL;
}

int main(){
  pthread_t threads[20];
  int t;
  int s;
  printf("%i bottles of beer on the wall\n%i bottles of beer\n", beers, beers);
  for (t=0;t<20;t++){
    if(pthread_create(&threads[t],NULL,drink_lots,NULL) == -1){
      error("Can't create all the threads!");
    }
  }

  void * result;
  for (s=0;s<20;s++){
    if(pthread_join(threads[s],&result) == -1){
      error("Can't join all the threads!");
    }
  }
  printf("There are now %i bottles of beer on the wall\n",beers);
  return 0;
}
