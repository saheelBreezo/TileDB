/*
 * File: tiledb_array_read_sparse_1.cc
 * 
 * It shows how to read from a sparse array.
 */

#include "c_api.h"
#include <cstdio>

int main() {
  // Intialize context with the default configuration parameters
  TileDB_CTX* tiledb_ctx;
  tiledb_ctx_init(&tiledb_ctx, NULL);

  // Initialize array 
  TileDB_Array* tiledb_array;
  tiledb_array_init(
      tiledb_ctx,                                       // Context
      &tiledb_array,                                    // Array object
      "my_workspace/sparse_arrays/my_array_B",          // Array name
      TILEDB_ARRAY_READ,                                // Mode
      NULL,                                             // Whole domain
      NULL,                                             // All attributes
      0);                                               // Number of attributes

  // Prepare cell buffers 
  int buffer_a1[10];
  size_t buffer_a2[10];
  char buffer_var_a2[30];
  float buffer_a3[20];
  int64_t buffer_coords[20];
  void* buffers[] = 
      { buffer_a1, buffer_a2, buffer_var_a2, buffer_a3, buffer_coords };
  size_t buffer_sizes[] = 
  { 
      sizeof(buffer_a1),  
      sizeof(buffer_a2),
      sizeof(buffer_var_a2),
      sizeof(buffer_a3),
      sizeof(buffer_coords)
  };

  // Read from array
  tiledb_array_read(tiledb_array, buffers, buffer_sizes); 

  // Print cell values
  int64_t result_num = buffer_sizes[0] / sizeof(int);
  printf("coords\t a1\t   a2\t (a3.first, a3.second)\n");
  printf("----------------------------------------------\n");
  for(int i=0; i<result_num; ++i) { 
    printf("(%lld, %lld)", buffer_coords[2*i], buffer_coords[2*i+1]);
    printf("\t %2d", buffer_a1[i]);
    size_t var_size = (i != result_num-1) ? buffer_a2[i+1] - buffer_a2[i] 
                                          : buffer_sizes[2] - buffer_a2[i];
    printf("\t %4.*s", var_size, &buffer_var_a2[buffer_a2[i]]);
    printf("\t\t (%3.1f, %3.1f)\n", buffer_a3[2*i], buffer_a3[2*i+1]);
  }

  // Finalize the array
  tiledb_array_finalize(tiledb_array);

  // Finalize context
  tiledb_ctx_finalize(tiledb_ctx);

  return 0;
}
