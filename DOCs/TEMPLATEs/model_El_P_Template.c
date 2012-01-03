/*******************************************************************************
*
*  model_<FILL element name>_P_<FILL model name>
*
*  <FILL model name> pair potential model for <FILL element name>
*
*  Reference: <FILL>
*
*  Author: <FILL>
*  Date  : <FILL>
*
*  Language: C
*
*  Release: This file is part of the openkim-api.git repository.
*
*  Copyright 2011 Ellad B. Tadmor, Ryan S. Elliott, and James P. Sethna
*  All rights reserved.
*
*******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "KIMserviceC.h"
#include "KIMstatus.h"

/******************************************************************************
* Below are the definitions and values of all Model parameters
*******************************************************************************/
#define DIM 3       /* dimensionality of space */
#define SPECCODE 1  /* internal species code */
#define MODEL_CUTOFF <FILL cutoff value> /* cutoff radius in angstroms */
#define MODEL_CUTSQ  (MODEL_CUTOFF * MODEL_CUTOFF)
#define <FILL parameter name> <FILL parameter value>

/* Define prototypes for model init */
/* must be all lowercase to be compatible with the KIM API (to support Fortran Tests) */
/**/
void model_<FILL (lowercase) element name>_p_<FILL (lowercase) model name>_init_(void* km);

/* Define prototypes for model reinit, compute, and destroy */
/* defined as static to avoid namespace clashes with other Models */
/**/
static void compute(void* km, int* ier);
/**/
static void calc_phi(double r, double* phi);
static void calc_phi_dphi(double r, double* phi, double* dphi);


/* Calculate pair potential phi(r) */
static void calc_phi(double r, double* phi)
{
   /* local variables */
   /*<FILL place any local variable definitions here>*/

   if (r > MODEL_CUTOFF)
   {
      /* Argument exceeds cutoff radius */
      *phi = 0.0;
   }
   else
   {
      *phi = <FILL functional form of phi(r)>;
   }

   return;
}

/* Calculate pair potential phi(r) and its derivative dphi(r) */
static void calc_phi_dphi(double r, double* phi, double* dphi)
{
   /* local variables */
   /*<FILL place any local variable definitions here>*/

   if (r > MODEL_CUTOFF)
   {
      /* Argument exceeds cutoff radius */
      *phi  = 0.0;
      *dphi = 0.0;
   }
   else
   {
      *phi  = <FILL functional form of phi(r)>;
      *dphi = <FILL functional form of dphi(r)>;
   }

   return;
}

/* compute function */
static void compute(void* km, int* ier)
{
   /* local static parameters */
   const static double cutsq = MODEL_CUTOFF * MODEL_CUTOFF;
   
   /* local variables */
   intptr_t* pkim = *((intptr_t**) km);
   double R;
   double Rsqij;
   double phi;
   double dphi;
   double dEidr;
   double Rij[DIM];
   int i;
   int j;
   int jj;
   int k;
   int numOfAtomNeigh;
   int currentAtom;
   int comp_energy;
   int comp_force;
   int comp_energyPerAtom;
   int comp_virial;
   int IterOrLoca;
   int HalfOrFull;
   int NBC;
   char* NBCstr;
   int numberContrib;

   int* nAtoms;
   int* atomTypes;
   double* Rij_list;
   double* coords;
   double* energy;
   double* force;
   double* energyPerAtom;
   double* virialGlobal;
   int* neighListOfCurrentAtom;
   double* boxlength;
   int* numContrib;
   
   
   /* Determine neighbor list boundary condition (NBC) */
   /* and half versus full mode: */
   /*****************************
    * HalfOrFull = 1 -- Half
    *            = 2 -- Full
    *****************************/
   NBCstr = (char*) KIM_API_get_NBC_method(pkim, ier);
   if (KIM_STATUS_OK > *ier)
   {
      KIM_API_report_error(__LINE__, __FILE__, "KIM_API_get_NBC_method", *ier);
      return;
   }
   if (!strcmp("CLUSTER",NBCstr))
   {	   
      NBC = 0;
      HalfOrFull = 1;
   }
   else if (!strcmp("MI-OPBC-H",NBCstr))
   {	   
      NBC = 1;
      HalfOrFull = 1;
   }
   else if (!strcmp("MI-OPBC-F",NBCstr))
   {	   
      NBC = 1;
      HalfOrFull = 2;
   }

   else if (!strcmp("NEIGH-PURE-H",NBCstr))
   {	   
      NBC = 2;
      HalfOrFull = 1;
   }
   else if (!strcmp("NEIGH-PURE-F",NBCstr))
   {	   
      NBC = 2;
      HalfOrFull = 2;
   }
   else if (!strcmp("NEIGH-RVEC-F",NBCstr))
   {	   
      NBC = 3;
      HalfOrFull = 2;
   }
   else
   {
      *ier = KIM_STATUS_FAIL;
      KIM_API_report_error(__LINE__, __FILE__, "Unknown NBC method", *ier);
      return;
   }
   free(NBCstr); /* don't forget to release the memory... */

   /* determine neighbor list handling mode */
   if (NBC != 0)
   {
      /*****************************
       * IterOrLoca = 1 -- Iterator
       *            = 2 -- Locator
       *****************************/
      IterOrLoca = KIM_API_get_neigh_mode(pkim, ier);
      if (KIM_STATUS_OK > *ier)
      {
         KIM_API_report_error(__LINE__, __FILE__, "KIM_API_get_neigh_mode", *ier);
         return;
      }
      if ((IterOrLoca != 1) && (IterOrLoca != 2))
      {
         printf("* ERROR: Unsupported IterOrLoca mode = %i\n", IterOrLoca);
         exit(-1);
      }
   }
   else
   {
      IterOrLoca = 2;   /* for CLUSTER NBC */
   }

   /* check to see if we have been asked to compute the forces, energyPerAtom, energy and virial */
   KIM_API_get_compute_multiple(pkim, ier, 4*3,
                                "energy",        &comp_energy,        1,
                                "forces",        &comp_force,         1,
                                "energyPerAtom", &comp_energyPerAtom, 1,
                                "virialGlobal",  &comp_virial,        1);
   if (KIM_STATUS_OK > *ier)
   {
      KIM_API_report_error(__LINE__, __FILE__, "KIM_API_get_compute_multiple", *ier);
      return;
   }

   /* unpack data from KIM object */
   KIM_API_get_data_multiple(pkim, ier, 9*3,
                             "numberOfAtoms",           &nAtoms,        1,
                             "atomTypes",               &atomTypes,     1,
                             "coordinates",             &coords,        1,
                             "numberContributingAtoms", &numContrib,    (HalfOrFull==1),
                             "boxlength",               &boxlength,     (NBC==1),
                             "energy",                  &energy,        (comp_energy==1),
                             "forces",                  &force,         (comp_force==1),
                             "energyPerAtom",           &energyPerAtom, (comp_energyPerAtom==1),
                             "virialGlobal",            &virialGlobal,  (comp_virial==1));
   if (KIM_STATUS_OK > *ier)
   {
      KIM_API_report_error(__LINE__, __FILE__, "KIM_API_get_data_multiple", *ier);
      return;
   }

   if (HalfOrFull == 1)
   {
      if (0 != NBC) /* non-CLUSTER cases */
      {
         numberContrib = *numContrib;
      }
      else
      {
         numberContrib = *nAtoms;
      }
   }

   /* Check to be sure that the atom types are correct */
   /**/
   *ier = KIM_STATUS_FAIL; /* assume an error */
   for (i = 0; i < *nAtoms; ++i)
   {
      if ( SPECCODE != atomTypes[i])
      {
         KIM_API_report_error(__LINE__, __FILE__, "Unexpected species type detected", *ier);
         return;
      }
   }
   *ier = KIM_STATUS_OK; /* everything is ok */

   /* initialize potential energies, forces, and virial term */
   if (comp_energyPerAtom)
   {
      for (i = 0; i < *nAtoms; ++i)
      {
         energyPerAtom[i] = 0.0;
      }
   }
   else if (comp_energy)
   {
      *energy = 0.0;
   }

   if (comp_force)
   {
      for (i = 0; i < *nAtoms; ++i)
      {
         for (k = 0; k < DIM; ++k)
         {
            force[i*DIM + k] = 0.0;
         }
      }
   }

   if (comp_virial)
   {
      for (i = 0; i < 6; ++i)
      {
         virialGlobal[i] = 0.0;
      }
   }

   /* Initialize neighbor handling for CLUSTER NBC */
   if (0 == NBC) /* CLUSTER */
   {
      neighListOfCurrentAtom = (int *) malloc((*nAtoms)*sizeof(int));
   }

   /* Initialize neighbor handling for Iterator mode */

   if (1 == IterOrLoca)
   {
      if (1 == HalfOrFull) /* HALF list */
      {
         *ier = KIM_API_get_half_neigh(pkim, 0, 0, &currentAtom, &numOfAtomNeigh,
                                       &neighListOfCurrentAtom, &Rij_list);
      }
      else                 /* FULL list */
      {
         *ier = KIM_API_get_full_neigh(pkim, 0, 0, &currentAtom, &numOfAtomNeigh,
                                       &neighListOfCurrentAtom, &Rij_list);
      }
      /* check for successful initialization */
      if (KIM_STATUS_NEIGH_ITER_INIT_OK != *ier)
      {
         if (1 == HalfOrFull)
         {
            KIM_API_report_error(__LINE__, __FILE__, "KIM_API_get_half_neigh", *ier);
         }
         else
         {
            KIM_API_report_error(__LINE__, __FILE__, "KIM_API_get_full_neigh", *ier);
         }
         *ier = KIM_STATUS_FAIL;
         return;
      }
   }

   /* Compute enery and forces */

   /* loop over particles and compute enregy and forces */
   i = -1;
   while( 1 )
   {

      /* Set up neighbor list for next atom for all NBC methods */
      if (1 == IterOrLoca) /* ITERATOR mode */
      {
         if (1 == HalfOrFull) /* HALF list */
         {
            *ier = KIM_API_get_half_neigh(pkim, 0, 1, &currentAtom, &numOfAtomNeigh,
                                          &neighListOfCurrentAtom, &Rij_list);  
         }
         else
         {
            *ier = KIM_API_get_full_neigh(pkim, 0, 1, &currentAtom, &numOfAtomNeigh,
                                          &neighListOfCurrentAtom, &Rij_list);
         }
         if (KIM_STATUS_NEIGH_ITER_PAST_END == *ier) /* the end of the list, terminate loop */
         {
            break;
         }
         if (KIM_STATUS_OK > *ier) /* some sort of problem, exit */
         {
            if (1 == HalfOrFull)
            {
               KIM_API_report_error(__LINE__, __FILE__, "KIM_API_get_half_neigh", *ier);
            }
            else
            {
               KIM_API_report_error(__LINE__, __FILE__, "KIM_API_get_full_neigh", *ier);
            }
            return;
         }

         i = currentAtom;
      }
      else
      {
         i++;
         if (*nAtoms <= i) /* incremented past end of list, terminate loop */
         {
            break;
         }

         if (1 == HalfOrFull) /* HALF list */
         {
            if (0 == NBC)     /* CLUSTER NBC method */
            {
               numOfAtomNeigh = *nAtoms - (i + 1);
               for (k = 0; k < numOfAtomNeigh; ++k)
               {
                  neighListOfCurrentAtom[k] = i + k + 1;
               }
               *ier = KIM_STATUS_OK;
            }
            else
            {
               *ier = KIM_API_get_half_neigh(pkim, 1, i, &currentAtom, &numOfAtomNeigh,
                                             &neighListOfCurrentAtom, &Rij_list);
            }
         }
         else                 /* FULL list */
         {
            *ier = KIM_API_get_full_neigh(pkim, 1, i, &currentAtom, &numOfAtomNeigh,
                                          &neighListOfCurrentAtom, &Rij_list);
         }
      }
      if (KIM_STATUS_OK != *ier) /* some sort of problem, exit */
      {
         if (1 == HalfOrFull)
         {
            KIM_API_report_error(__LINE__, __FILE__, "KIM_API_get_half_neigh", *ier);
         }
         else
         {
            KIM_API_report_error(__LINE__, __FILE__, "KIM_API_get_full_neigh", *ier);
         }
         *ier = KIM_STATUS_FAIL;
         return;
      }
            
      /* loop over the neighbors of atom i */
      for (jj = 0; jj < numOfAtomNeigh; ++ jj)
      {

         j = neighListOfCurrentAtom[jj]; /* get neighbor ID */

         /* compute relative position vector and squared distance */
         Rsqij = 0.0;
         for (k = 0; k < DIM; ++k)
         {
            if (3 != NBC) /* all methods except NEIGH-RVEC-F */
            {
               Rij[k] = coords[j*DIM + k] - coords[i*DIM + k];
            }
            else          /* NEIGH-RVEC-F method */
            {
               Rij[k] = Rij_list[jj*DIM + k];
            }

            /* apply periodic boundary conditions if required */
            if (1 == NBC)
            {
               if (abs(Rij[k]) > 0.5*boxlength[k])
               {
                  Rij[k] -= (Rij[k]/fabs(Rij[k]))*boxlength[k];
               }
            }
            
            /* compute squared distance */
            Rsqij += Rij[k]*Rij[k];
         }
         
         /* compute energy and force */
         if (Rsqij < MODEL_CUTSQ) /* particles are interacting ? */
         {
            R = sqrt(Rsqij);
            if (comp_force || comp_virial)
            {
               /* compute pair potential and its derivative */
               calc_phi_dphi(R, &phi, &dphi);

               /* compute dEidr */
               if ((1 == HalfOrFull) && (j < numberContrib))
               {
                  /* HALF mode -- double contribution */
                  dEidr = dphi;
               }
               else
               {
                  /* FULL mode -- regular contribution */
                  dEidr = 0.5*dphi;
               }
            }
            else
            {
               /* compute just pair potential */
               calc_phi(R, &phi);
            }
            
            /* contribution to energy */
            if (comp_energyPerAtom)
            {
               energyPerAtom[i] += 0.5*phi;
               /* if half list add energy for the other atom in the pair */
               if ((1 == HalfOrFull) && (j < numberContrib)) energyPerAtom[j] += 0.5*phi;
            }
            else if (comp_energy)
            {
               if ((1 == HalfOrFull) && (j < numberContrib))
               {
                  /* Half mode -- add v to total energy */
                  *energy += phi;
               }
               else
               {
                  /* Full mode -- add half v to total energy */
                  *energy += 0.5*phi;
               }
            }
            
            /* contribution to virial tensor */
            if (comp_virial)
            {
               /* virial(i,j) = r(i)*r(j)*(dV/dr)/r */
	       virialGlobal[0] += Rij[0]*Rij[0]*dEidr/R;
	       virialGlobal[1] += Rij[1]*Rij[1]*dEidr/R;
	       virialGlobal[2] += Rij[2]*Rij[2]*dEidr/R;
	       virialGlobal[3] += Rij[1]*Rij[2]*dEidr/R;
	       virialGlobal[4] += Rij[0]*Rij[2]*dEidr/R;
	       virialGlobal[5] += Rij[0]*Rij[1]*dEidr/R;
            }
            
            /* contribution to forces */
            if (comp_force)
            {
               for (k = 0; k < DIM; ++k)
               {
                  force[i*DIM + k] += dEidr*Rij[k]/R; /* accumulate force on atom i */
                  force[j*DIM + k] -= dEidr*Rij[k]/R; /* accumulate force on atom j */
               }
            }
         }
      } /* loop on jj */
   }    /* infinite while loop (terminated by break statements above */
   

   /* perform final tasks */
   
   if (comp_energyPerAtom && comp_energy)
   {
      *energy = 0.0;
      for (k = 0; k < *nAtoms; ++k)
      {
         *energy += energyPerAtom[k];
      }
   }

   /* Free temporary storage */
   if (0 == NBC) 
   {
      free(neighListOfCurrentAtom);
   }

   /* everything is great */
   *ier = KIM_STATUS_OK;
   return;
}

/* Initialization function */
void model_<FILL (lowercase) elemenet name>_p_<FILL (lowercase) model name>_init_(void *km)
{
   /* Local variables */
   intptr_t* pkim = *((intptr_t**) km);
   double* model_cutoff;
   int ier;

   /* store pointer to compute function in KIM object */
   ier = KIM_API_set_data(pkim, "compute", 1, (void*) &compute);
   if (KIM_STATUS_OK > ier)
   {
      KIM_API_report_error(__LINE__, __FILE__, "KIM_API_set_data", ier);
      exit(1);
   }

   /* store model cutoff in KIM object */
   model_cutoff = (double*) KIM_API_get_data(pkim, "cutoff", &ier);
   if (KIM_STATUS_OK > ier)
   {
      KIM_API_report_error(__LINE__, __FILE__, "KIM_API_get_data", ier);
      exit(1);
   }
   *model_cutoff = MODEL_CUTOFF; /* cutoff distance in angstroms */

   return;
}
