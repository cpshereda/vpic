#include <particle_pipelines.h>

double
energy_p( const particle_t     * ALIGNED p,
          const int                      n,
          const float                    q_m,
          const interpolator_t * ALIGNED f,
          const grid_t         *         g ) {
  energy_p_pipeline_args_t args[1];
  double local, global;
  int rank;

  if( n<0     ) ERROR(("Bad number of particles"));
  if( f==NULL ) ERROR(("Bad interpolator"));
  if( g==NULL ) ERROR(("Bad grid"));

  /* Have the pipelines do the bulk of particles in quads and
     have the host do the final incomplete quad. */

  args->p   = p;
  args->n   = n;
  args->q_m = q_m;
  args->f   = f;
  args->g   = g;

  dispatch_pipelines( energy_p_pipeline_v4, args, 0 );
  energy_p_pipeline( args, _n_pipeline, _n_pipeline );
  wait_for_pipelines();

  local = 0;
  for( rank=0; rank<=_n_pipeline; rank++ ) local += args->en[rank];
  mp_allsum_d( &local, &global, 1, g->mp );
  return (double)g->cvac*(double)g->cvac*global/(double)q_m;
}

