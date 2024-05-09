#include <string.h>
#include <mpi.h>
#include <stdlib.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>

#include "pfind-options.h"

double GetTimeStamp(void); // prototype

pfind_find_results_t * pfind_find(pfind_options_t * opt){
  pfind_find_results_t * res = malloc(sizeof(pfind_find_results_t));
  memset(res, 0, sizeof(pfind_find_results_t));
  double tstart = GetTimeStamp();

  sleep(1);

  res->found_files = 10000;
  res->total_files = 10000;
  res->runtime = GetTimeStamp() - tstart;
  res->rate = res->total_files / res->runtime;

  return res;
}

pfind_find_results_t * pfind_aggregrate_results(pfind_find_results_t * local){
  return local;
}

pfind_options_t * pfind_parse_args(int argc, char ** argv, int force_print_help, MPI_Comm com){
  static pfind_options_t options = {
  };

  return & options;
}