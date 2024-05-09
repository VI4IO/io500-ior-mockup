#include "ior.h"
#include "mdtest.h"
#include "md-workbench.h"
#include <sys/time.h>

FILE * out_logfile;
int rank;

double GetTimeStamp(void)
{
        double timeVal;
        struct timeval timer;
        gettimeofday(&timer, (struct timezone *)NULL);
        timeVal = (double)timer.tv_sec + ((double)timer.tv_usec / 1000000);

        return (timeVal);
}

ior_aiori_t dummy_aiori;
static char * current = (char*) 1;

static option_help * DUMMY_options(aiori_mod_opt_t ** init_backend_options, aiori_mod_opt_t * init_values){
  option_help h [] = {
      LAST_OPTION
  };
  option_help * help = malloc(sizeof(h));
  memcpy(help, h, sizeof(h));
  return help;
}

static aiori_fd_t *DUMMY_Create(char *testFileName, int iorflags, aiori_mod_opt_t * options)
{
  return (aiori_fd_t*) current++;
}

static aiori_fd_t *DUMMY_Open(char *testFileName, int flags, aiori_mod_opt_t * options)
{
  return (aiori_fd_t*) current++;
}

static void DUMMY_Fsync(aiori_fd_t *fd, aiori_mod_opt_t * options)
{
}


static void DUMMY_Sync(aiori_mod_opt_t * options)
{
}

static void DUMMY_Close(aiori_fd_t *fd, aiori_mod_opt_t * options)
{
}

static void DUMMY_Delete(char *testFileName, aiori_mod_opt_t * options)
{
}

static char * DUMMY_getVersion()
{
  return "0.5";
}

static IOR_offset_t DUMMY_GetFileSize(aiori_mod_opt_t * options, char *testFileName)
{
  return 0;
}

static IOR_offset_t DUMMY_Xfer(int access, aiori_fd_t *file, IOR_size_t * buffer, IOR_offset_t length, IOR_offset_t offset, aiori_mod_opt_t * options){
  return length;
}

static int DUMMY_statfs (const char * path, ior_aiori_statfs_t * stat, aiori_mod_opt_t * options){
  stat->f_bsize = 1;
  stat->f_blocks = 1;
  stat->f_bfree = 1;
  stat->f_bavail = 1;
  stat->f_files = 1;
  stat->f_ffree = 1;
  return 0;
}

static int DUMMY_mkdir (const char *path, mode_t mode, aiori_mod_opt_t * options){
  return 0;
}

static int DUMMY_rmdir (const char *path, aiori_mod_opt_t * options){
  return 0;
}

static int DUMMY_access (const char *path, int mode, aiori_mod_opt_t * options){
  return 0;
}

static int DUMMY_stat (const char *path, struct stat *buf, aiori_mod_opt_t * options){
  return 0;
}

static int DUMMY_rename (const char *path, const char *path2, aiori_mod_opt_t * options){
  return 0;
}


static int DUMMY_check_params(aiori_mod_opt_t * options){
  return 0;
}

static void DUMMY_final(aiori_mod_opt_t * options){
}

const ior_aiori_t *aiori_select (const char *api){
  static ior_aiori_t dummy_aiori = {
        .name = "DUMMY",
        .name_legacy = NULL,
        .create = DUMMY_Create,
        .open = DUMMY_Open,
        .xfer = DUMMY_Xfer,
        .close = DUMMY_Close,
        .remove = DUMMY_Delete,
        .get_version = DUMMY_getVersion,
        .fsync = DUMMY_Fsync,
        .get_file_size = DUMMY_GetFileSize,
        .statfs = DUMMY_statfs,
        .mkdir = DUMMY_mkdir,
        .rmdir = DUMMY_rmdir,
        .rename = DUMMY_rename,
        .access = DUMMY_access,
        .stat = DUMMY_stat,
        .initialize = DUMMY_final,
        .finalize = DUMMY_final,
        .get_options = DUMMY_options,
        .check_params = DUMMY_check_params,
        .sync = DUMMY_Sync,
        .enable_mdtest = true
    };
  return & dummy_aiori;
}

void * airoi_update_module_options(const ior_aiori_t * backend, options_all_t * module_defaults){
  return NULL;
}

options_all_t * airoi_create_all_module_options(option_help * global_options){
  static options_all_t all_options = {0, NULL};
  return & all_options;
}

int option_parse_str(char*val, options_all_t * opt_all){
  return 0;
}
