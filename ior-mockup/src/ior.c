#include "ior.h"
#include <string.h>
#include <assert.h>

double GetTimeStamp(void);
extern FILE * out_logfile;
extern int rank;

enum IOR_phase { // must be synchronized with IO500 phases
  PHASE_EASY_WRITE = 0,
  PHASE_HARD_WRITE,
  PHASE_EASY_READ,
  PHASE_HARD_READ
};

static void easy_write(IOR_test_t * test){
  double tstart = GetTimeStamp();
  IOR_point_t * p = & test->results->write;
  
  sleep(1);

  p->time = GetTimeStamp() - tstart;
  p->aggFileSizeForBW = 1;
  // for write
  p->stonewall_avg_data_accessed = 10000;
  p->stonewall_time = p->time;
}

static void hard_write(IOR_test_t * test){
  double tstart = GetTimeStamp();
  IOR_point_t * p = & test->results->write;
  
  sleep(1);

  p->time = GetTimeStamp() - tstart;
  p->aggFileSizeForBW = 2;
  // for write
  p->stonewall_avg_data_accessed = 10000;
  p->stonewall_time = p->time;
}

static void easy_read(IOR_test_t * test){
  double tstart = GetTimeStamp();
  IOR_point_t * p = & test->results->read;

  p->time = GetTimeStamp() - tstart;
  p->aggFileSizeForBW = 3;
}

static void hard_read(IOR_test_t * test){
  double tstart = GetTimeStamp();
  IOR_point_t * p = & test->results->read;

  p->time = GetTimeStamp() - tstart;  
  p->aggFileSizeForBW = 4;
}


IOR_test_t * ior_run(int argc, char **argv, MPI_Comm world_com, FILE * logfile){
  out_logfile = logfile;
  MPI_Comm_rank(world_com, & rank);

  IOR_test_t * res = malloc(sizeof(IOR_test_t));
  memset(res, 0, sizeof(IOR_test_t));
  res->results = malloc(sizeof(IOR_results_t));
  memset(res->results, 0, sizeof(IOR_results_t));
    
  // detect the phase we run
  static enum IOR_phase phase = PHASE_EASY_WRITE;
  // printf("PHASE %d\n", phase);
  switch(phase){
    case(PHASE_EASY_WRITE):
    easy_write(res);
    break;    
    case(PHASE_HARD_WRITE):
    hard_write(res);
    break;
    case(PHASE_EASY_READ):
    easy_read(res);
    break;
    case(PHASE_HARD_READ):
    hard_read(res);
    break;
    default:
    assert(phase == 0);
  }
  phase++;
  return res;
}
