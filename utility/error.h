typedef enum error {
  DEFAULT,
  MEMORY_SUCCESS,  
  MEMORY_FAILURE,
  LIST_SUCCESS,
  LIST_INVALID,
  LIST_OUT_OF_BOUNDS,
} error;

static error errorcode = DEFAULT;