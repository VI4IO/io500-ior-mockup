#include "mdtest.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <mpi.h>


extern FILE * out_logfile;
extern int rank;
double GetTimeStamp(void);

// change to 300 to achieve a "valid" run
#define RUNTIME 3 

enum mdtest_phase { // must be synchronized with IO500 phases
  PHASE_EASY_WRITE = 0,
  PHASE_HARD_WRITE,
  PHASE_EASY_STAT,
  PHASE_HARD_STAT,
  PHASE_EASY_DELETE,
  PHASE_HARD_READ,
  PHASE_HARD_DELETE
};

//static uint64_t easy_stonewall_create_items;
static uint64_t hard_stonewall_create_items;


/*
 This access pattern creates empty files in a process-specific/local folder
 */
void easy_write(mdtest_results_t * res){
  int counter = MDTEST_FILE_CREATE_NUM;
  double tstart = GetTimeStamp();

  sleep(1);

  res->time[counter] =  GetTimeStamp() - tstart;
  res->items[counter] = 1;
  res->rate[counter] = 1;
  // calculated for write
  res->stonewall_time[counter] = 1;
  res->stonewall_item_sum[counter] = 1;
}

/*
 This access pattern creates 3901 byte files in a *shared* folder
 */
void hard_write(mdtest_results_t * res){
  // Some sampel code how to implement a write pattern
  int counter = MDTEST_FILE_CREATE_NUM;
  double runtime;
  int rank;
  int size;
  char rankstr[10];
  char itemstr[22];

  MPI_Comm_rank(MPI_COMM_WORLD, & rank);
  MPI_Comm_size(MPI_COMM_WORLD, & size);
  sprintf(rankstr, "%d", rank);

  char * buf = malloc(3901);
  memset(buf, '1', 3901);

  MPI_Barrier(MPI_COMM_WORLD);
  double tstart = GetTimeStamp();

  double rate;
  /* 
   * in order to run for 300s, try to create batches of 10k, then approximate...
   */
  uint64_t items = 0;
  uint64_t cur_items = 10000;

  while(1){
    double time_iter_start = GetTimeStamp();

    for(uint64_t i=0; i < cur_items; i++){
      sprintf(itemstr, "%llu", (long long unsigned) (i + items));
      // TODO create this item
    }
    items += cur_items;
    
    // calculate current creation rate    
    double time_end = GetTimeStamp();
    runtime = time_end - tstart; 
    if(runtime > RUNTIME) break;
    
    double time_iter = (time_end - time_iter_start);
    // current creation rate
    rate = cur_items / time_iter;

    cur_items = rate * (RUNTIME - runtime);
    if(cur_items < 10) cur_items = 10;
    // printf("%d - %llu\n", rank, (long long unsigned) cur_items);
  }

  // stonewalling, bring all processes to the same number of items ...
  uint64_t global_max;
  MPI_Allreduce(& items, & global_max, 1, MPI_UINT64_T, MPI_MAX, MPI_COMM_WORLD);
  if (items < global_max){
    cur_items = global_max - items;

    for(uint64_t i=0; i < cur_items; i++){
      sprintf(itemstr, "%llu", (long long unsigned) (i + items));
      // create the item 
    }
  }

  MPI_Barrier(MPI_COMM_WORLD);
  runtime = GetTimeStamp() - tstart; 

  items = size * global_max;
  // remember the number globally for read phase!
  hard_stonewall_create_items = global_max; 

  res->time[counter] = runtime;
  res->items[counter] = items;
  res->rate[counter] = items / runtime;
  // calculated for write
  res->stonewall_time[counter] = runtime;
  res->stonewall_item_sum[counter] = items;
  free(buf);
}

void easy_stat(mdtest_results_t * res){
  int counter = MDTEST_FILE_STAT_NUM;
  double tstart = GetTimeStamp();

  res->time[counter] =  GetTimeStamp() - tstart;
  res->items[counter] = 3;
  res->rate[counter] = 3;
}

void hard_stat(mdtest_results_t * res){
  int counter = MDTEST_FILE_STAT_NUM;
  double tstart = GetTimeStamp();

  res->time[counter] =  GetTimeStamp() - tstart;
  res->items[counter] = 4;
  res->rate[counter] = 4;
}

void easy_delete(mdtest_results_t * res){
  int counter = MDTEST_FILE_REMOVE_NUM;
  double tstart = GetTimeStamp();

  res->time[counter] =  GetTimeStamp() - tstart;
  res->items[counter] = 5;
  res->rate[counter] = 5;
}

void hard_read(mdtest_results_t * res){
  int counter = MDTEST_FILE_READ_NUM;
  double tstart = GetTimeStamp();

  res->time[counter] =  GetTimeStamp() - tstart;
  res->items[counter] = 6;
  res->rate[counter] = 6;
}

void hard_delete(mdtest_results_t * res){
  int counter = MDTEST_FILE_REMOVE_NUM;
  double tstart = GetTimeStamp();

  res->time[counter] =  GetTimeStamp() - tstart;
  res->items[counter] = 7;
  res->rate[counter] = 7;
}

mdtest_results_t * mdtest_run(int argc, char **argv, MPI_Comm world_com, FILE * logfile){
  out_logfile = logfile;
  MPI_Comm_rank(world_com, & rank);

  mdtest_results_t * res = malloc(sizeof(mdtest_results_t));
  memset(res, 0, sizeof(mdtest_results_t));

  static enum mdtest_phase phase = PHASE_EASY_WRITE;
  // printf("PHASE %d\n", phase);
  switch(phase){
    case(PHASE_EASY_WRITE):
    easy_write(res);
    break;
    case(PHASE_HARD_WRITE):
    hard_write(res);
    break;
    case(PHASE_EASY_STAT):
    easy_stat(res);
    break;
    case(PHASE_HARD_STAT):
    hard_stat(res);
    break;
    case(PHASE_EASY_DELETE):
    easy_delete(res);
    break;
    case(PHASE_HARD_READ):
    hard_read(res);
    break;
    case(PHASE_HARD_DELETE):
    hard_delete(res);
    break;
    default:
      assert(phase == 0);
  }
  phase++;

  return res;
}
