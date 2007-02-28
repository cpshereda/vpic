#ifndef _emitter_h_
#define _emitter_h_

#include <common.h>
#include <mtrand.h>
#include <grid.h>
#include <field.h>
#include <particle.h>
#include <species.h>

/* Every local cell has 27 components associated with it (6 faces, 12 edges, 8
   corners and cell body). All components in a local grid simulation can be
   uniquely enumerated by 32*local_cell_id + component_type. 32 is used instead
   of 27 for the following reasons:
   - Trivial to compute the component id by bit ops
   - Trivial to compute cell and component by bit ops
   - Regions 27-31 can be used for future expansion.
   The face, edge and corner, body components types are enumerated by a
   (-1:1,-1:1,-1:1) FORTRAN style indexing calculation. Note that this
   allows distinctions like which side of a cell a face is on. */

#define COMPONENT_ID(local_cell,component_type) (((local_cell)<<5) | (component_type))
#define EXTRACT_LOCAL_CELL(component_id)        ((component_id)>>5)
#define EXTRACT_COMPONENT_TYPE(component_id)    ((component_id)&31)


#define MAX_EMISSION_MODEL_SIZE 1024

typedef struct emitter_t {
  int * ALIGNED component;
  int n_component, max_component;
  species_t * sp;                  /* Species to emit */
  void (*emission_model)( struct emitter_t * RESTRICT e,
                          const interpolator_t * RESTRICT ALIGNED fi,
                          field_t * RESTRICT ALIGNED f, 
                          accumulator_t * RESTRICT ALIGNED a, 
                          grid_t * RESTRICT g, 
                          mt_handle rng );
  char model_parameters[MAX_EMISSION_MODEL_SIZE]; 
  struct emitter_t * next;
  char name[1];                    /* Terminal zero of string */
} emitter_t;

typedef void (*emission_model_t)( struct emitter_t * RESTRICT e,
                                  const interpolator_t * RESTRICT ALIGNED fi,
                                  field_t * RESTRICT ALIGNED f, 
                                  accumulator_t * RESTRICT ALIGNED a, 
                                  grid_t * RESTRICT g, 
                                  mt_handle rng );

BEGIN_C_DECLS

/* In structors.c */

extern emitter_t * new_emitter( const char * name,
                                species_t * sp,
                                emission_model_t emission_model,
                                int max_component,
                                emitter_t ** e_list );
extern void delete_emitter_list( emitter_t ** e );
extern emitter_t * find_emitter_name( const char * name, emitter_t * e_list );

/* In child-langmuir.c */

typedef struct child_langmuir_t {
  int   n_emit_per_face;              /* How many particles to emit per face */
  float ut_perp;                      /* Perpendicular normalized thermal momentum */
  float ut_para;                      /* Parallel normalized thermal momentum */
} child_langmuir_t;

extern void
child_langmuir( struct emitter_t * RESTRICT e,
                const interpolator_t * RESTRICT ALIGNED fi, /* For interpolation of fields */
                field_t * RESTRICT ALIGNED f,             /* For accumulation of rhob */
                accumulator_t * RESTRICT ALIGNED a,       /* injected current accumulation */
                grid_t * RESTRICT g,                      /* Grid corresponding to above */
                mt_handle rng );

/* In ccube.c */

typedef struct ccube_t {
  int   n_emit_per_face;              /* How many particles to emit per face */
  float ut_perp;                      /* Perpendicular normalized thermal momentum */
  float ut_para;                      /* Parallel normalized thermal momentum */
  float thresh_e_norm;                /* Only emit particles if |E| > thresh_e_norm. */
} ccube_t;

extern void
ccube( struct emitter_t * RESTRICT e,
       const interpolator_t * RESTRICT ALIGNED fi, /* For interpolation of fields */
       field_t * RESTRICT ALIGNED f,             /* For accumulation of rhob */
       accumulator_t * RESTRICT ALIGNED a,       /* injected current accumulation */
       grid_t * RESTRICT g,                      /* Grid corresponding to above */
       mt_handle rng );

/* In ivory.c */

typedef struct ivory_t {
  int   n_emit_per_face;              /* How many particles to emit per face */
  float ut_perp;                      /* Perpendicular normalized thermal momentum */
  float ut_para;                      /* Parallel normalized thermal momentum */
  float thresh_e_norm;                /* Only emit particles if |E| > thresh_e_norm. */
} ivory_t;

extern void
ivory( struct emitter_t * RESTRICT e,
       const interpolator_t * RESTRICT ALIGNED fi, /* For interpolation of fields */
       field_t * RESTRICT ALIGNED f,             /* For accumulation of rhob */
       accumulator_t * RESTRICT ALIGNED a,       /* injected current accumulation */
       grid_t * RESTRICT g,                      /* Grid corresponding to above */
       mt_handle rng );


END_C_DECLS

#endif

