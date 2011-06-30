#include "decs.h"

/////////////////////
//
// At this point all grid type parameters should be set as if done with init()
//
// Perform some extra checks to ensure restart file read-in is reasonable
//
// OPENMPOPTMARK: Don't optimize since many critical regions
//
/////////////////////
int restart_init_checks(int which, FTYPE (*prim)[NSTORE2][NSTORE3][NPR], FTYPE (*pstag)[NSTORE2][NSTORE3][NPR], FTYPE (*ucons)[NSTORE2][NSTORE3][NPR])
{
  char ans[100];
  struct of_geom geomdontuse;
  struct of_geom *ptrgeom=&geomdontuse;
  int failreturn;
  FTYPE ucon[NDIM];
  FTYPE utmax=0.0;
  int i,j,k,pl,pliter;
  int failflag=0;
  extern int set_dt(FTYPE (*prim)[NSTORE2][NSTORE3][NPR], SFTYPE *dt);
  int gotnan;
  int finalstepbackup;



  ////////////////////////////////////////////////////////////////
  //
  // NOW have all parameters and data.
  //
  /////////




  //////////
  // NOW CHECK THE DATA and apply "fixups" if necessary
  //
  ////////////////////////////////////////////////////////////////

#if(CHECKRHONEGZERORESTART)

  // report if data has rho<0 or u<0
  ZLOOP{
    if(STEPOVERNEGRHO!=-1){
      if(MACP0A1(prim,i,j,k,RHO)<=0.0){
	dualfprintf(fail_file,"restart data has negative mass density at i=%d j=%d k=%d\n",startpos[1]+i,startpos[2]+j,startpos[3]+k);
	failflag++;
      }
    }
    if(STEPOVERNEGU!=-1){
      if(MACP0A1(prim,i,j,k,UU)<=0.0){
	dualfprintf(fail_file,"restart data has negative ie density at i=%d j=%d k=%d\n",startpos[1]+i,startpos[2]+j,startpos[3]+k);
	failflag++;
      }
    }

    // check 3-velocity
#if(WHICHVEL==VEL3)
    if(jonchecks){
      get_geometry(i,j,k,CENT,ptrgeom);
      failreturn=check_pr(MAC(prim,i,j,k),MAC(prim,i,j,k),MAC(ucons,i,j,k), ptrgeom,-2,-1);
      if(failreturn){
	dualfprintf(fail_file,"restart data has large or imaginary u^t=%21.15g at i=%d j=%d k=%d.  I will attempt to correct.\n",1.0/sqrt(uttdiscr),startpos[1]+i,startpos[2]+j,startpos[3]+k);
      }
      if(1.0/sqrt(uttdiscr)>utmax) utmax=1.0/sqrt(uttdiscr);
      // need to settle over limit u^t's
      failreturn=check_pr(MAC(prim,i,j,k),MAC(prim,i,j,k),MAC(ucons,i,j,k), ptrgeom,-1,-1);
      if(failreturn){
	dualfprintf(fail_file,"restart data has imaginary u^t at i=%d j=%d k=%d.  Unable to correct.\n",startpos[1]+i,startpos[2]+j,startpos[3]+k);
	return(1);
      }
    }
#endif    
  }
  if(WHICHVEL==VEL3){
    if(jonchecks){
      dualfprintf(fail_file,"max u^t of restart data=%21.15g\n",utmax);
    }
  }
#endif

  if(failflag>0){
    dualfprintf(fail_file,"Restart data has at least %d failures, please correct\n",failflag);
    return(1);
  }




  /////////////////////
  //
  // fixup() during restart
  //
  /////////////////////
#if(FIXUPAFTERRESTART)
  if(fixup(STAGEM1,prim,ucons,-1)>=1)
    FAILSTATEMENT("restart.c:restart_init()", "fixup()", 1);

  trifprintf( "proc: %d fixup restart completed: failed=%d\n", myid,failed);
#endif





  /////////////////////
  //
  // BOUND during restart
  //
  /////////////////////
  finalstepbackup=finalstepglobal;
  finalstepglobal=1;
  if (bound_allprim(STAGEM1,t,prim,pstag,ucons, 1, USEMPI) >= 1) {
    fprintf(fail_file, "restart_init:bound_allprim: failure\n");
    fflush(fail_file);
    return (1);
  }
  finalstepglobal=finalstepbackup;

  trifprintf( "proc: %d bound restart completed: failed=%d\n", myid,failed);


  //////////////
  //
  // make sure all zones are not nan after bound
  //
  //////////////
  // make sure all zones are not nan
  gotnan=0;
  FULLLOOP{// OPENMPOPTMARK: Don't optimize since critical region
    PDUMPLOOP(pliter,pl){
      if(!finite(MACP0A1(prim,i,j,k,pl))){
	dualfprintf(fail_file,"restart data has NaN at i=%d j=%d k=%d ti=%d tj=%d tk=%d :: pl=%d\n",i,j,k,startpos[1]+i,startpos[2]+j,startpos[3]+k,pl);
	//	myexit(24968346);
	gotnan=1;
      }
    }
  }
  if(gotnan) myexit(24968346);

  /////////////////////
  //
  // pre_fixup() during restart
  //
  /////////////////////
  
  if(pre_fixup(STAGEM1,prim)>=1)
    FAILSTATEMENT("init.c:init()", "postbc_fixup()", 1);



  /////////////////////
  //
  // make sure all zones are good now
  //
  /////////////////////
#if(CHECKRHONEGZERORESTART)
  if(STEPOVERNEGRHO!=-1){
    FULLLOOP{// OPENMPOPTMARK: Don't optimize since return
      if(MACP0A1(prim,i,j,k,RHO)<=0.0){
	dualfprintf(fail_file,"restart data has negative mass density at i=%d j=%d k=%d\n",startpos[1]+i,startpos[2]+j,startpos[3]+k);
	return(1);
	
      }
    }
    if(STEPOVERNEGU!=-1){
      if(MACP0A1(prim,i,j,k,UU)<=0.0){
	dualfprintf(fail_file,"restart data has negative ie density at i=%d j=%d k=%d\n",startpos[1]+i,startpos[2]+j,startpos[3]+k);
	return(1);
	
      }
    }
  }
#endif



  ////////////////////////
  //
  // test read by looking at images
  //
  ////////////////////////
  if(image_dump(-3)>=1) return(1);



  ////////////////////////
  //
  // Now adjust timestep since may be dt=0 or whatever if last run ended on tf
  // NOW done whether restarting or not at end of init()
  //
  ///////////////////////
  // set_dt(prim,&dt);


  trifprintf("end restart_init_checks\n");

  /* done! */
  return (0);

}
