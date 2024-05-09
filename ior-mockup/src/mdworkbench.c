#include "ior.h"
#include "md-workbench.h"

extern FILE * out_logfile;
extern int rank;

mdworkbench_results_t* md_workbench_run(int argc, char ** argv, MPI_Comm world_com, FILE * logfile){
  out_logfile = logfile;
  MPI_Comm_rank(world_com, & rank);
  return NULL;
}