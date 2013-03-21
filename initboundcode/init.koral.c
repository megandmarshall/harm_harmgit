
#include "decs.h"


// NOTE on units:
// Many things below are so far in code units, not physical units, so they don't need conversion.  This includes:
//
// Rin, Rout, tf, DTdumpgen


int BEAMNO,FLATBACKGROUND; // global for bounds.koral.c


//FTYPE RADBEAMFLAT_FRATIO=0.95;
FTYPE RADBEAMFLAT_FRATIO;
FTYPE RADBEAMFLAT_ERAD;
FTYPE RADBEAMFLAT_RHO;
FTYPE RADBEAMFLAT_UU;

FTYPE RADATM_MDOTEDD;
FTYPE RADATM_LUMEDD;
int RADATM_THINRADATM;
FTYPE RADATM_FERATIO;
FTYPE RADATM_FRATIO;
FTYPE RADATM_RHOAMB;
FTYPE RADATM_TAMB;


int RADWAVE_NWAVE;
int RADWAVE_NUMERO;
FTYPE RADWAVE_PP;
FTYPE RADWAVE_CC;
FTYPE RADWAVE_KAPPA;
FTYPE RADWAVE_RHOFAC;
FTYPE RADWAVE_DRRE;
FTYPE RADWAVE_DRIM;
FTYPE RADWAVE_DVRE;
FTYPE RADWAVE_DVIM;
FTYPE RADWAVE_DURE;
FTYPE RADWAVE_DUIM;
FTYPE RADWAVE_DERE;
FTYPE RADWAVE_DEIM;
FTYPE RADWAVE_DFRE;
FTYPE RADWAVE_DFIM;
FTYPE RADWAVE_OMRE;
FTYPE RADWAVE_OMIM;
FTYPE RADWAVE_DTOUT1;
FTYPE RADWAVE_RHOZERO;
FTYPE RADWAVE_AAA;
FTYPE RADWAVE_KK;
FTYPE RADWAVE_UINT;
FTYPE RADWAVE_TEMP;
FTYPE RADWAVE_ERAD;
FTYPE RADWAVE_VX;
FTYPE RADWAVE_KAPPAES;
FTYPE RADWAVE_ERADFACTOR;
FTYPE RADWAVE_GASFACTOR;



FTYPE RADBONDI_TESTNO;
FTYPE RADBONDI_PRADGAS;
FTYPE RADBONDI_TGAS0;
FTYPE RADBONDI_MDOTPEREDD;
FTYPE RADBONDI_MDOTEDD;
FTYPE RADBONDI_RHOAMB;
FTYPE RADBONDI_TAMB;
FTYPE RADBONDI_MUGAS;
FTYPE RADBONDI_MINX;
FTYPE RADBONDI_MAXX;









FTYPE normglobal;

int prepre_init_specific_init(void)
{
  int funreturn;


  funreturn=user1_prepre_init_specific_init();
  if(funreturn!=0) return(funreturn);

  periodicx1=periodicx2=periodicx3=1;

  // Also: SET USEROMIO to 0 or 1 in mympi.definit.h (needs to be 0 for TEXTOUTPUT)
  binaryoutput=TEXTOUTPUT;

  return(0);

}


int pre_init_specific_init(void)
{

  UTOPRIMVERSION = UTOPRIMJONNONRELCOMPAT;

  return(0);
}

int set_fieldfrompotential(int *fieldfrompotential)
{
  int pl,pliter;

  // default (assume all fields are from potential)
  PLOOPBONLY(pl) fieldfrompotential[pl-B1+1]=1;


  return(0);
}


int init_conservatives(FTYPE (*prim)[NSTORE2][NSTORE3][NPR],FTYPE (*pstag)[NSTORE2][NSTORE3][NPR], FTYPE (*Utemp)[NSTORE2][NSTORE3][NPR], FTYPE (*U)[NSTORE2][NSTORE3][NPR])
{
  int funreturn;
  int fieldfrompotential[NDIM];

  set_fieldfrompotential(fieldfrompotential);

  funreturn=user1_init_conservatives(fieldfrompotential, prim,pstag, Utemp, U);
  if(funreturn!=0) return(funreturn);


  return(0);
 
}


int post_init_specific_init(void)
{
  int funreturn;

  funreturn=user1_post_init_specific_init();
  if(funreturn!=0) return(funreturn);

  // print out units and some constants
  trifprintf("Constants\n");
  trifprintf("LBAR=%g TBAR=%g VBAR=%g RHOBAR=%g MBAR=%g UBAR=%g TEMPBAR=%g\n",LBAR,TBAR,VBAR,RHOBAR,MBAR,UBAR,TEMPBAR); 
  trifprintf("ARAD_CODE=%26.20g OPACITYBAR=%g KAPPA_ES_CODE(1,1)=%g KAPPA_FF_CODE(1,1)=%g KAPPA_BF_CODE(1,1)=%g\n",ARAD_CODE,OPACITYBAR,KAPPA_ES_CODE(1,1),KAPPA_FF_CODE(1,1),KAPPA_BF_CODE(1,1));
  trifprintf("ARAD_CODE_DEF=%g\n",ARAD_CODE_DEF);


  //  cour=0.8;
  

  return(0);
}



int init_consts(void)
{
  //  Lunit=Tunit=Munit=1.0;

  // units can be used for user to read in data, but otherwise for rest of code all that matters is Mfactor and Jfactor
  Mfactor=Jfactor=1.0;

  return(0);

}





int init_global(void)
{
  int pl,pliter;
  int funreturn;

  funreturn=user1_init_global();
  if(funreturn!=0) return(funreturn);


  // default
  ARAD_CODE=ARAD_CODE_DEF;


  //  ERADLIMIT=UUMIN; // set same for now
  ERADLIMIT=UUMINLIMIT; // seems fine.
  // maximum radiation frame lorentz factor
  GAMMAMAXRAD=10000.0;
  //  GAMMAMAXRAD=100.0;



  //////////////////
  // overrides for more detailed problem dependence
  TIMEORDER=2; // no need for 4 unless higher-order or cold collapse problem.
  //  FLUXB=FLUXCTTOTH;
  FLUXB=FLUXCTSTAG;
  

  //  rescaletype=1;
  rescaletype=4;
  BSQORHOLIMIT=1E2; // was 1E2 but latest BC test had 1E3 // CHANGINGMARK
  BSQOULIMIT=1E3; // was 1E3 but latest BC test had 1E4
  UORHOLIMIT=1E3;
  RHOMIN = 1E-4;
  UUMIN = 1E-6;
  //OSMARK: where is DTr1 defined? what is DTfake?
  DTfake=MAX(1,DTr/10); 




  /*************************************************/
  /*************************************************/
  /*************************************************/

  if(WHICHPROBLEM==FLATNESS){
	
	//  lim[1]=lim[2]=lim[3]=MINM;
	cour=0.8;
	gam=gamideal=5.0/3.0;
	cooling=KORAL;

	BCtype[X1UP]=PERIODIC; // OUTFLOW;
	BCtype[X1DN]=PERIODIC;
	BCtype[X2UP]=PERIODIC; // OUTFLOW;
	BCtype[X2DN]=PERIODIC;
	BCtype[X3UP]=PERIODIC; // OUTFLOW;
	BCtype[X3DN]=PERIODIC;

	int idt;
	for(idt=0;idt<NUMDUMPTYPES;idt++) DTdumpgen[idt]=0.05;   // default dumping period

	DTr = 100; //number of time steps for restart dumps
	tf = 10.0; //final time
  }

  /*************************************************/
  /*************************************************/
  /*************************************************/


  if(WHICHPROBLEM==RADPULSE || WHICHPROBLEM==RADPULSEPLANAR || WHICHPROBLEM==RADPULSE3D){

    lim[1]=lim[2]=lim[3]=MINM;
	//  cour=0.5;
	cour=0.8;
	gam=gamideal=5.0/3.0;
	cooling=KORAL;

	BCtype[X1UP]=OUTFLOW;
	BCtype[X1DN]=OUTFLOW;
	BCtype[X2UP]=OUTFLOW;
	BCtype[X2DN]=OUTFLOW;
	BCtype[X3UP]=OUTFLOW; 
	BCtype[X3DN]=OUTFLOW;

    // sigmarad = 1.56E-64
    // arad=4*sigmarad/c
    // NOTE: Koral code has different values than paper
	ARAD_CODE=4.0*1.56E-64*(TEMPBAR*TEMPBAR*TEMPBAR*TEMPBAR); // to match koral and avoiding real units


	int idt;
	//  for(idt=0;idt<NUMDUMPTYPES;idt++) DTdumpgen[idt]=1E3;

	DTr = 100; //number of time steps for restart dumps
    if(WHICHPROBLEM==RADPULSEPLANAR){
      tf=1E5;
      for(idt=0;idt<NUMDUMPTYPES;idt++) DTdumpgen[idt]=100.0; // Koral output steps
      //      for(idt=0;idt<NUMDUMPTYPES;idt++) DTdumpgen[idt]=0.1; // testing
    }
    else if(WHICHPROBLEM==RADPULSE){
      tf = 35; //final time
      for(idt=0;idt<NUMDUMPTYPES;idt++) DTdumpgen[idt]=0.1;
    }
	else if(WHICHPROBLEM==RADPULSE3D){
      tf = 70; //final time
      for(idt=0;idt<NUMDUMPTYPES;idt++) DTdumpgen[idt]=0.1;
    }
  }

  /*************************************************/
  /*************************************************/
  /*************************************************/

  if(WHICHPROBLEM==RADBEAMFLAT){
	cour=0.8;
	gam=gamideal=5.0/3.0;
	cooling=KORAL;

    // RADBEAMFLAT_FRATIO=0.95;
    RADBEAMFLAT_FRATIO=0.99995; // vradx
    RADBEAMFLAT_ERAD=1.; // 1g/cm^3 worth of energy density in radiation
    RADBEAMFLAT_RHO=1.; // 1g/cm^3
    RADBEAMFLAT_UU=0.1; // 0.1g/cm^3 worth of energy density in fluid


	BCtype[X1UP]=OUTFLOW;
	BCtype[X1DN]=RADBEAMFLATINFLOW;
	BCtype[X2UP]=OUTFLOW;
	BCtype[X2DN]=OUTFLOW;
	BCtype[X3UP]=PERIODIC; 
	BCtype[X3DN]=PERIODIC;

	int idt;
	//  for(idt=0;idt<NUMDUMPTYPES;idt++) DTdumpgen[idt]=0.05;   // default dumping period
	for(idt=0;idt<NUMDUMPTYPES;idt++) DTdumpgen[idt]=0.1;   // like problem24 in koral

	DTr = 100; //number of time steps for restart dumps
	tf = 10.0; //final time
  }

  /*************************************************/
  /*************************************************/
  /*************************************************/

  if(WHICHPROBLEM==RADTUBE){

// 1,2,3,31,4,41,5
//#define NTUBE 1
#define NTUBE 31 // harder near t=0 at discontinuity
//#define NTUBE 5
//#define NTUBE 3

	lim[1]=lim[2]=lim[3]=MINM; // NTUBE=1 has issues near cusp, so use MINM
	// should have PARA(LINE) not oscillate so much at cusp
	// Also should eliminate PARA's zig-zag steps in internal energy density in other tests.
	cour=0.8;
	cooling=KORAL;

	// arad = 4*sigmarad/c (so removed /4. from koral sigma setup).
	// Note, sigmarad or arad is not arbitrary -- value chosen to IC in radiative-hydro balance for each separately the left and right states.
	if(NTUBE==1){
	  gam=gamideal=5./3.;
	  ARAD_CODE=(1e-8/pow(calc_PEQ_Tfromurho(3.e-5/(gam-1.),1.),4.));
	}
	else if(NTUBE==2){
	  gam=gamideal=5./3.;
	  ARAD_CODE=(2e-5/pow(calc_PEQ_Tfromurho(4.e-3/(gam-1.),1.),4.));
	}
	else if(NTUBE==3){
	  gam=gamideal=2.;
	  ARAD_CODE=(2./pow(calc_PEQ_Tfromurho(60./(gam-1.),1.),4.));
	}
	else if(NTUBE==31){
	  gam=gamideal=2.;
	  ARAD_CODE=(2./pow(calc_PEQ_Tfromurho(60./(gam-1.),1.),4.));
	}
	else if(NTUBE==4){
	  gam=gamideal=5./3.;
	  ARAD_CODE=(.18/pow(calc_PEQ_Tfromurho(6.e-3/(gam-1.),1.),4.));
	}
	else if(NTUBE==41){
	  gam=gamideal=5./3.;
	  ARAD_CODE=(.18/pow(calc_PEQ_Tfromurho(6.e-3/(gam-1.),1.),4.));
	}
	else if(NTUBE==5){
	  gam=gamideal=2.;
	  ARAD_CODE=(2./pow(calc_PEQ_Tfromurho(60./(gam-1.),1.),4.));
	}


	trifprintf("RADTUBE NTUBE=%d ARAD_CODE=%g SIGMARAD_CODE=%g\n",NTUBE,ARAD_CODE,ARAD_CODE/4.0);


	BCtype[X1UP]=FREEOUTFLOW;
	BCtype[X1DN]=FREEOUTFLOW;
	BCtype[X2UP]=OUTFLOW; // NOTEMARK: Koral sets fixed BCs.  We can do that following the IC choices, but not necessary.
	BCtype[X2DN]=OUTFLOW;
	BCtype[X3UP]=PERIODIC; 
	BCtype[X3DN]=PERIODIC;

	int idt;
	if(NTUBE==5){
	  for(idt=0;idt<NUMDUMPTYPES;idt++) DTdumpgen[idt]=1.0;
	}
	else{
	  for(idt=0;idt<NUMDUMPTYPES;idt++) DTdumpgen[idt]=2.0;
	}

	DTr = 100; //number of time steps for restart dumps
	//  tf = 100.0; //final time (seems almost good enough to get quasi-steady solution for these steady tube tests)
	if(NTUBE==5) tf=15.0; // koral paper shows about t=13, and code has no problem going much further than 15.
    else tf = 3E2; //final time (good enough to see any evolution and errored evolution)
  }

  /*************************************************/
  /*************************************************/
  /*************************************************/

  if(WHICHPROBLEM==RADSHADOW){

	lim[1]=lim[2]=lim[3]=MINM; // NTUBE=1 has issues near cusp, so use MINM
	// should have PARA(LINE) not oscillate so much at cusp
	// Also should eliminate PARA's zig-zag steps in internal energy density in other tests.
	cour=0.8;
	gam=gamideal=1.4;
	cooling=KORAL;
	ARAD_CODE=1E7*1E-5*(2.5E-9/7.115025791e-10); // tuned so radiation energy flux puts in something much higher than ambient, while initial ambient radiation energy density lower than ambient gas internal energy.

	BCtype[X1UP]=FREEOUTFLOW;
	BCtype[X1DN]=RADSHADOWINFLOW;
	BCtype[X2UP]=PERIODIC;
	BCtype[X2DN]=PERIODIC;
	BCtype[X3UP]=PERIODIC; 
	BCtype[X3DN]=PERIODIC;

	int idt;
	for(idt=0;idt<NUMDUMPTYPES;idt++) DTdumpgen[idt]=0.5;

	DTr = 100; //number of time steps for restart dumps
	//  tf = 100.0; //final time (seems almost good enough to get quasi-steady solution for these steady tube tests)
	tf = 10.0; //final time
  }




  /*************************************************/
  /*************************************************/
  /*************************************************/

  if(WHICHPROBLEM==RADDBLSHADOW){

	lim[1]=lim[2]=lim[3]=MINM; // NTUBE=1 has issues near cusp, so use MINM
	//	lim[1]=lim[2]=lim[3]=MC; // MC gets totally bonkers answer with NLEFT=0.99999
	//lim[1]=lim[2]=lim[3]=PARALINE; // bonkers answer for NLEFT=0.99999, ok for NLEFT=0.99
	// should have PARA(LINE) not oscillate so much at cusp
	// Also should eliminate PARA's zig-zag steps in internal energy density in other tests.
	cour=0.8;
	//	cour=0.49; // doesn't help oscillations for NLEFT=0.99999 with MINM
	gam=gamideal=1.4;
	cooling=KORAL;
    //	ARAD_CODE=1E-30;
	ARAD_CODE=1E7*1E-5*(2.5E-9/7.115025791e-10); // tuned so radiation energy flux puts in something much higher than ambient, while initial ambient radiation energy density lower than ambient gas internal energy.

	BCtype[X1UP]=FREEOUTFLOW;
	BCtype[X1DN]=RADSHADOWINFLOW;
	BCtype[X2UP]=RADSHADOWINFLOWX2UP;
	BCtype[X2DN]=ASYMM;
	BCtype[X3UP]=PERIODIC; 
	BCtype[X3DN]=PERIODIC;

	int idt;
	for(idt=0;idt<NUMDUMPTYPES;idt++) DTdumpgen[idt]=2E-1;

	DTr = 100; //number of time steps for restart dumps
	//  tf = 100.0; //final time (seems almost good enough to get quasi-steady solution for these steady tube tests)
	tf = 200.0; //final time (far past plot so see evolves or stationary).
  }


  /*************************************************/
  /*************************************************/
  /*************************************************/

  if(WHICHPROBLEM==RADBEAM2D || WHICHPROBLEM==RADBEAM2DKS){

    BEAMNO=1; // 1-4
// whether constant or radially varying background
// ==0 doesn't make much sense for Minkowski without gravity, because flow reverses due to chosen high density
    FLATBACKGROUND=1;


	lim[1]=lim[2]=lim[3]=MINM; // NTUBE=1 has issues near cusp, so use MINM
	a=0.0; // no spin in case use MCOORD=KSCOORDS

	if(!(ISSPCMCOORDNATIVE(MCOORD))){
	  dualfprintf(fail_file,"Must choose MCOORD (currently %d) to be spherical polar grid type for RADBEAM2D\n",MCOORD);
	  myexit(3434628752);
	}

	cour=0.8;
	gam=gamideal=1.4;
	cooling=KORAL;
	//	ARAD_CODE=ARAD_CODE_DEF*1E5; // tuned so radiation energy flux puts in something much higher than ambient, while initial ambient radiation energy density lower than ambient gas internal energy.

	BCtype[X1UP]=RADBEAM2DFLOWINFLOW;
    BCtype[X1DN]=OUTFLOW;
    //	BCtype[X1DN]=HORIZONOUTFLOW;
	BCtype[X2UP]=PERIODIC;
	BCtype[X2DN]=PERIODIC;
	//	BCtype[X3UP]=FREEOUTFLOW;
	BCtype[X3UP]=OUTFLOW;
	BCtype[X3DN]=RADBEAM2DBEAMINFLOW;


	FTYPE DTOUT1;
	if (BEAMNO==1){
	  DTOUT1=.1; //dt for basic output
	}
	else if (BEAMNO==2){
	  DTOUT1=.4; //dt for basic output
	}
	else if (BEAMNO==3){
	  DTOUT1=1.; //dt for basic output
	}
	else if (BEAMNO==4){
	  DTOUT1=.25; //dt for basic output
	}

	int idt;
    //	for(idt=0;idt<NUMDUMPTYPES;idt++) DTdumpgen[idt]=DTOUT1;
	for(idt=0;idt<NUMDUMPTYPES;idt++) DTdumpgen[idt]=0.001;

	DTr = 100; //number of time steps for restart dumps
	tf = 20.0; //final time

    //    DODIAGEVERYSUBSTEP = 1;

  }

  /*************************************************/
  /*************************************************/
  /*************************************************/

  if(WHICHPROBLEM==ATMSTATIC){

    lim[1]=lim[2]=lim[3]=MINM; // NTUBE=1 has issues near cusp, so use MINM
	a=0.0; // no spin in case use MCOORD=KSCOORDS

	if(!(ISSPCMCOORDNATIVE(MCOORD))){
	  dualfprintf(fail_file,"Must choose MCOORD (currently %d) to be spherical polar grid type for ATMSTATIC\n",MCOORD);
	  myexit(3434628752);
	}

	cour=0.8;
	gam=gamideal=1.4;
	cooling=KORAL;
    ARAD_CODE=0.0;

	BCtype[X1UP]=OUTFLOW;
    BCtype[X1DN]=HORIZONOUTFLOW; // leads to little bit more static solution near inner radial boundary due to higher-order interpolation.  Could also fix values as in Koral, but odd to fix values for incoming flow.
	BCtype[X2UP]=PERIODIC;
	BCtype[X2DN]=PERIODIC;
	BCtype[X3UP]=PERIODIC;
	BCtype[X3DN]=PERIODIC;


	int idt;
	for(idt=0;idt<NUMDUMPTYPES;idt++) DTdumpgen[idt]=1E2;

	DTr = 100; //number of time steps for restart dumps
	tf = 1E7; //final time

    //    DODIAGEVERYSUBSTEP = 1;

  }


  /*************************************************/
  /*************************************************/
  /*************************************************/

  if(WHICHPROBLEM==RADATM){


    lim[1]=lim[2]=lim[3]=MINM; // MINM gets larger error and jump in v1 at outer edge
	a=0.0; // no spin in case use MCOORD=KSCOORDS

	if(!(ISSPCMCOORDNATIVE(MCOORD))){
	  dualfprintf(fail_file,"Must choose MCOORD (currently %d) to be spherical polar grid type for RADATM\n",MCOORD);
	  myexit(3434628753);
	}

	cour=0.8;
	gam=gamideal=1.4;
	cooling=KORAL;
    //    ARAD_CODE=0.0;

	BCtype[X1UP]=RADATMBEAMINFLOW;
    BCtype[X1DN]=RADATMBEAMINFLOW;

    //    BCtype[X1UP]=OUTFLOW;
    //    BCtype[X1DN]=HORIZONOUTFLOW;
    //BCtype[X1DN]=OUTFLOW;

	BCtype[X2UP]=PERIODIC;
	BCtype[X2DN]=PERIODIC;
	BCtype[X3UP]=PERIODIC;
	BCtype[X3DN]=PERIODIC;


	int idt;
    for(idt=0;idt<NUMDUMPTYPES;idt++) DTdumpgen[idt]=1E6;
    //for(idt=0;idt<NUMDUMPTYPES;idt++) DTdumpgen[idt]=1E5; // DEBUG

	DTr = 100; //number of time steps for restart dumps
	tf = 2E9; //final time
    
    tf=1E8; // profiling

    //    DODIAGEVERYSUBSTEP = 1;

  }

  /*************************************************/
  /*************************************************/
  /*************************************************/

  if(WHICHPROBLEM==RADWALL){

    lim[1]=lim[2]=lim[3]=MINM; // Messy with PARALINE

	cour=0.8;
	gam=gamideal=5.0/3.0;
	cooling=KORAL;

	BCtype[X1UP]=OUTFLOW;
	BCtype[X1DN]=RADWALLINFLOW;
	BCtype[X2UP]=RADWALLINFLOW;
	BCtype[X2DN]=ASYMM;
	BCtype[X3UP]=PERIODIC; 
	BCtype[X3DN]=PERIODIC;

	int idt;
	for(idt=0;idt<NUMDUMPTYPES;idt++) DTdumpgen[idt]=0.5;

	DTr = 100; //number of time steps for restart dumps
	tf = 50.0; //final time
  }

  /*************************************************/
  /*************************************************/
  /*************************************************/

  if(WHICHPROBLEM==RADWAVE){

    lim[1]=lim[2]=lim[3]=MINM; // generates glitch at extrema in prad0
    //lim[1]=lim[2]=lim[3]=MC; // less of a glitch near extrema in prad0

	cour=0.8;
	cooling=KORAL;
    gam=gamideal=5./3.;


    // KORALTODO: See Jiang, Stone, Davis (2012) for S6.1.2 for linear MHD-radiation compressible wave tests
    
    RADWAVE_NWAVE=5; // 1,2,3,4,5 .  And for 5 can choose NUMERO=41,11,1
    //    RADWAVE_NWAVE=1; // GOOD
    //    RADWAVE_NWAVE=2; // GOOD
    //    RADWAVE_NWAVE=3; // GOOD
    //    RADWAVE_NWAVE=4; // gets noisy in prad1 by t~30 with MINM or MC  -- check koral when Olek makes it work.  KORALTODO
    //RADWAVE_NUMERO=11; // GOOD
    RADWAVE_NUMERO=41; // OK if don't use check if can do explicit.  So use this to show how should more generally improve the tau based suppression check!  But, DAMPS significantly! Smaller IMPCONV doesn't help.  Check with koral KORALTODO.  MC doesn't help/change much.
    //RADWAVE_NUMERO=1; // wierd jello oscillations in prad0, and no wave motion -- like in koral though.  KORALTODO.  With only implicit, jello is different (smaller IMPCONV doesn't help and larger IMPEPS doesn't help).

    // NUMERO=41 corresponds to Jiang et al. (2002) PP=100, sigma=10 (2nd row, 2nd column in Table B1) 11 to PP=0.01, sigma=0.01 (1st row, 1st column).

    //NUMERO 1 was supposed to be his original test (1st row, 1st column) but, as you mention, it turned out to be jelly. The reason is that the initial conditions from the table were not precise enough to hit the acoustic mode and much faster radiation mode quickly dominates causing the jelly behavior. I had difficult time with that and decided to derive the numbers by myself (PROBLEMS/RADWAVE/disp107.nb). They were a bit different and made the difference so that one sees only the acoustic mode. The reason is most likely the fact that my dispersion relation and the code are somewhat relativistic.




    // defaults
    RADWAVE_KAPPA=RADWAVE_KAPPAES=0.0;


	if(RADWAVE_NWAVE==5){ //sound wave with radiation set up according to Jiang+12

      if(RADWAVE_NUMERO==41){
        RADWAVE_PP=100.;
        RADWAVE_CC=1.e2;
        RADWAVE_KAPPA=10.;
        RADWAVE_RHOFAC=0.01;
        RADWAVE_DRRE=(1.e-3*RADWAVE_RHOFAC);
        RADWAVE_DRIM=0.;
        RADWAVE_DVRE=(4.06372e-6*RADWAVE_RHOFAC);
        RADWAVE_DVIM=(6.90937e-6*RADWAVE_RHOFAC);
        RADWAVE_DURE=(9.88671e-4*RADWAVE_RHOFAC);
        RADWAVE_DUIM=(6.97077e-6*RADWAVE_RHOFAC);
        RADWAVE_DERE=(-4.52724e-5*RADWAVE_RHOFAC);
        RADWAVE_DEIM=(2.78566e-5*RADWAVE_RHOFAC);
        RADWAVE_DFRE=(-5.83678e-6*RADWAVE_RHOFAC);
        RADWAVE_DFIM=(-9.48194e-6*RADWAVE_RHOFAC);
        RADWAVE_OMRE=0.0255331;
        RADWAVE_OMIM=0.0434128;
        RADWAVE_DTOUT1=1.e-0;
      }

      if(RADWAVE_NUMERO==11){
        RADWAVE_PP=0.01;
        RADWAVE_CC=1.e2;
        RADWAVE_KAPPA=0.01;
        RADWAVE_RHOFAC=0.01;
        RADWAVE_DRRE=(1.e-3*RADWAVE_RHOFAC);
        RADWAVE_DRIM=0.;
        RADWAVE_DVRE=(9.99998e-6*RADWAVE_RHOFAC);
        RADWAVE_DVIM=(8.48878e-9*RADWAVE_RHOFAC);
        RADWAVE_DURE=(1.66666e-3*RADWAVE_RHOFAC);
        RADWAVE_DUIM=(2.82938e-6*RADWAVE_RHOFAC);
        RADWAVE_DERE=(1.95853e-8*RADWAVE_RHOFAC);
        RADWAVE_DEIM=(1.91123e-7*RADWAVE_RHOFAC);
        RADWAVE_DFRE=(-1.33508e-5*RADWAVE_RHOFAC);
        RADWAVE_DFIM=(4.23463e-6*RADWAVE_RHOFAC);
        RADWAVE_OMRE=6.28317e-2;
        RADWAVE_OMIM=5.33366e-5;
        RADWAVE_DTOUT1=1.e-0;
      }


      if(RADWAVE_NUMERO==1){
        RADWAVE_PP=0.01;
        RADWAVE_CC=1.e4;
        RADWAVE_KAPPA=0.01;
        RADWAVE_DRRE=1.e-3;
        RADWAVE_DRIM=0.;
        RADWAVE_DVRE=9.7548e-8;
        RADWAVE_DVIM=7.92788e-9;
        //RADWAVE_DPRE=1.61075e-3;
        //RADWAVE_DPIM=2.07402e-4;
        RADWAVE_DURE=1.57546e-3;
        RADWAVE_DUIM=2.57783e-4;
        RADWAVE_DERE=1.6874e-8; // 1.79137e-8
        RADWAVE_DEIM=9.48966e-9; // 8.56498e-9
        RADWAVE_DFRE=-1.77115e-6; // -1.32035e-6
        RADWAVE_DFIM=3.65291e-6; // 3.88814e-6
        RADWAVE_OMRE=6.12912e-4; // 7.99077
        RADWAVE_OMIM=4.98123e-5; // 0.512336
        RADWAVE_DTOUT1=1.e-2;
      }

      RADWAVE_RHOZERO=1.;
      RADWAVE_KK=2.*Pi;
      RADWAVE_UINT=((1./RADWAVE_CC/RADWAVE_CC)*RADWAVE_RHOZERO/gam/(gam-1.-1./RADWAVE_CC/RADWAVE_CC)) ; // to get proper sound speed
      RADWAVE_TEMP=(calc_PEQ_Tfromurho(RADWAVE_UINT,RADWAVE_RHOZERO)) ; // temperature from rho and uint
      ARAD_CODE=((RADWAVE_PP*(gam-1.)*RADWAVE_UINT/RADWAVE_TEMP/RADWAVE_TEMP/RADWAVE_TEMP/RADWAVE_TEMP)); //to get the proper radiation to gas pressure ratio, PP=4 sig T^4 / P
      RADWAVE_ERAD=(calc_LTE_EfromT(RADWAVE_TEMP)) ; // to get thermal equilibrium, E=4 sig T^4
    }


	if(RADWAVE_NWAVE==1){ //density wave advected with the gas
      // NO RADIATION
      RADWAVE_PP=0.1;
      RADWAVE_CC=1.e6;
      RADWAVE_VX=1.e-3;
      RADWAVE_DTOUT1=(.05/RADWAVE_VX);
      RADWAVE_RHOZERO=1.;
      RADWAVE_AAA=1.e-5;
      RADWAVE_ERAD=1.;
      RADWAVE_KK=2.*Pi;
      RADWAVE_UINT=(1./RADWAVE_CC/RADWAVE_CC)*RADWAVE_RHOZERO/gam/(gam-1.-1./RADWAVE_CC/RADWAVE_CC);
      RADWAVE_TEMP=calc_PEQ_Tfromurho(RADWAVE_UINT,RADWAVE_RHOZERO);
      ARAD_CODE=(RADWAVE_PP*(gam-1.)*RADWAVE_UINT/RADWAVE_TEMP/RADWAVE_TEMP/RADWAVE_TEMP/RADWAVE_TEMP);
    }

	if(RADWAVE_NWAVE==2){ //hydro sound wave
      // NO RADIATION
      RADWAVE_PP=0.01;
      RADWAVE_CC=1.e6;
      RADWAVE_DTOUT1=(.05*RADWAVE_CC);
      RADWAVE_VX=0.;
      RADWAVE_RHOZERO=1.;
      RADWAVE_AAA=1.e-5;
      RADWAVE_ERAD=1.;
      RADWAVE_KK=2.*Pi;
      RADWAVE_UINT=(1./RADWAVE_CC/RADWAVE_CC)*RADWAVE_RHOZERO/gam/(gam-1.-1./RADWAVE_CC/RADWAVE_CC);
      RADWAVE_TEMP=calc_PEQ_Tfromurho(RADWAVE_UINT,RADWAVE_RHOZERO);
      ARAD_CODE=(RADWAVE_PP*(gam-1.)*RADWAVE_UINT/RADWAVE_TEMP/RADWAVE_TEMP/RADWAVE_TEMP/RADWAVE_TEMP);
    }

	if(RADWAVE_NWAVE==3){ //radiative density wave advected with the gas
      FLUXDISSIPATION=(0.0);
      RADWAVE_PP=10.;
      RADWAVE_CC=1.e6;
      RADWAVE_VX=1.e-2;
      RADWAVE_DTOUT1=(.0005/RADWAVE_VX);
      RADWAVE_RHOZERO=1.;
      RADWAVE_AAA=1.e-5;
      RADWAVE_KK=2.*Pi;
      RADWAVE_UINT=(1./RADWAVE_CC/RADWAVE_CC)*RADWAVE_RHOZERO/gam/(gam-1.-1./RADWAVE_CC/RADWAVE_CC);
      RADWAVE_TEMP=calc_PEQ_Tfromurho(RADWAVE_UINT,RADWAVE_RHOZERO);
      ARAD_CODE=(RADWAVE_PP*(gam-1.)*RADWAVE_UINT/RADWAVE_TEMP/RADWAVE_TEMP/RADWAVE_TEMP/RADWAVE_TEMP);
      RADWAVE_ERAD=calc_LTE_EfromT(RADWAVE_TEMP);
      RADWAVE_KAPPAES=10.;
    }


	if(RADWAVE_NWAVE==4){ //sound wave with radiation, set up without the phase shifts etc.
      FLUXDISSIPATION=(0.0);
      RADWAVE_PP=1.;
      RADWAVE_CC=1.e2;
      RADWAVE_DTOUT1=(.005*RADWAVE_CC);
      RADWAVE_VX=0.;
      RADWAVE_RHOZERO=1.;
      RADWAVE_AAA=1.e-1;
      RADWAVE_KK=2.*Pi;
      RADWAVE_UINT=(1./RADWAVE_CC/RADWAVE_CC)*RADWAVE_RHOZERO/gam/(gam-1.-1./RADWAVE_CC/RADWAVE_CC);
      RADWAVE_TEMP=calc_PEQ_Tfromurho(RADWAVE_UINT,RADWAVE_RHOZERO);
      ARAD_CODE=(RADWAVE_PP*(gam-1.)*RADWAVE_UINT/RADWAVE_TEMP/RADWAVE_TEMP/RADWAVE_TEMP/RADWAVE_TEMP);
      RADWAVE_ERAD=calc_LTE_EfromT(RADWAVE_TEMP);
      RADWAVE_KAPPA=100.;
      RADWAVE_ERADFACTOR=.5;
      RADWAVE_GASFACTOR=.5;
    }


	BCtype[X1UP]=PERIODIC;
	BCtype[X1DN]=PERIODIC;
	BCtype[X2UP]=PERIODIC;
	BCtype[X2DN]=PERIODIC;
	BCtype[X3UP]=PERIODIC; 
	BCtype[X3DN]=PERIODIC;

	int idt;
	for(idt=0;idt<NUMDUMPTYPES;idt++) DTdumpgen[idt]=RADWAVE_DTOUT1;

	DTr = 100; //number of time steps for restart dumps
	if(RADWAVE_VX==0.0) tf = MAX(100.0*RADWAVE_DTOUT1,5.0/RADWAVE_CC);
    else tf = MAX(100.0*RADWAVE_DTOUT1,5.0/MIN(RADWAVE_VX,RADWAVE_CC));


    //    DODIAGEVERYSUBSTEP = 1;

  }


  /*************************************************/
  /*************************************************/
  /*************************************************/

  if(WHICHPROBLEM==RADBONDI){

    //	lim[1]=lim[2]=lim[3]=MINM; // too low order for ~100 points
    lim[1]=lim[2]=lim[3]=PARALINE;
	a=0.0; // no spin in case use MCOORD=KSCOORDS

	if(!(ISSPCMCOORDNATIVE(MCOORD))){
	  dualfprintf(fail_file,"Must choose MCOORD (currently %d) to be spherical polar grid type for RADBONDI\n",MCOORD);
	  myexit(3434628752);
	}

	cour=0.8;
	cooling=KORAL;
	//	ARAD_CODE=ARAD_CODE_DEF*1E5; // tuned so radiation energy flux puts in something much higher than ambient, while initial ambient radiation energy density lower than ambient gas internal energy.

	BCtype[X1UP]=RADBONDIINFLOW;
    //    BCtype[X1DN]=OUTFLOW;
    BCtype[X1DN]=HORIZONOUTFLOW; // although more specific extrapolation based upon solution might work better
	BCtype[X2UP]=PERIODIC;
	BCtype[X2DN]=PERIODIC;
	//	BCtype[X3UP]=FREEOUTFLOW;
	BCtype[X3UP]=OUTFLOW;
	BCtype[X3DN]=OUTFLOW;



    RADBONDI_TESTNO=2;

	if(RADBONDI_TESTNO==1){
      RADBONDI_PRADGAS=1.2e-7/RHOBAR;
      RADBONDI_TGAS0=1e5/TEMPBAR;
      RADBONDI_MDOTPEREDD=10.;
    }

	if(RADBONDI_TESTNO==2){
      RADBONDI_PRADGAS=1.2e-4/RHOBAR;
      RADBONDI_TGAS0=1.e6/TEMPBAR;
      RADBONDI_MDOTPEREDD=10.;
    }

	if(RADBONDI_TESTNO==3){
      RADBONDI_PRADGAS=1.2e-1/RHOBAR;
      RADBONDI_TGAS0=1e7/TEMPBAR;
      RADBONDI_MDOTPEREDD=10.;
    }

	if(RADBONDI_TESTNO==4){
      RADBONDI_PRADGAS=1.2e-5/RHOBAR;
      RADBONDI_TGAS0=1e6/TEMPBAR;
      RADBONDI_MDOTPEREDD=100.;
    } 

    RADBONDI_MDOTEDD=(2.23/16.*1e18*MPERSUN)/(MBAR/TBAR); //Mdot converted to code units
    RADBONDI_RHOAMB=1.e-25/RHOBAR;
    RADBONDI_TAMB=1.e5/TEMPBAR;
    gam=gamideal=(1.+1./3.*((1.+RADBONDI_PRADGAS)/(.5+RADBONDI_PRADGAS)));
    RADBONDI_MUGAS=.5;

    trifprintf("RADBONDI: %g %g %g %g %g %g %g %g\n",RADBONDI_PRADGAS,RADBONDI_TGAS0,RADBONDI_MDOTPEREDD,RADBONDI_MDOTEDD,RADBONDI_RHOAMB,RADBONDI_TAMB,gam,RADBONDI_MUGAS);

	int idt;
	for(idt=0;idt<NUMDUMPTYPES;idt++) DTdumpgen[idt]=10.0;

	DTr = 100; //number of time steps for restart dumps
	tf = 100*DTdumpgen[0]; // 100 dumps(?)

    //    DODIAGEVERYSUBSTEP = 1;

  }


  /*************************************************/
  /*************************************************/
  /*************************************************/


  return(0);

}


int init_defcoord(void)
{


  /*************************************************/
  /*************************************************/
  /*************************************************/
  if(WHICHPROBLEM==FLATNESS){


	defcoord = UNIFORMCOORDS;
	Rin_array[1]=0;
	Rin_array[2]=0;
	Rin_array[3]=0;

	Rout_array[1]=1.0;
	Rout_array[2]=1.0;
	Rout_array[3]=1.0;
  }


  /*************************************************/
  /*************************************************/
  /*************************************************/

  if(WHICHPROBLEM==RADPULSE || WHICHPROBLEM==RADPULSEPLANAR){

	defcoord = UNIFORMCOORDS;
	Rin_array[1]=-50.0; 
	Rin_array[2]=-1.0;
	Rin_array[3]=-1.0;

	Rout_array[1]=50.0; 
	Rout_array[2]=1.0;
	Rout_array[3]=1.0;

  }

  /*************************************************/
  /*************************************************/
  /*************************************************/

  if(WHICHPROBLEM==RADPULSE3D){

	defcoord = UNIFORMCOORDS;
	Rin_array[1]=-50.0; 
	Rin_array[2]=-50.0; 
	Rin_array[3]=-50.0; 

	Rout_array[1]=50.0; 
	Rout_array[2]=50.0; 
	Rout_array[3]=50.0; 
  }

  /*************************************************/
  /*************************************************/
  /*************************************************/
  if(WHICHPROBLEM==RADBEAMFLAT){


	defcoord = UNIFORMCOORDS;
	Rin_array[1]=0;
	Rin_array[2]=0;
	Rin_array[3]=0;

	Rout_array[1]=1.0;
	Rout_array[2]=1.0;
	Rout_array[3]=1.0;
  }

  /*************************************************/
  /*************************************************/
  /*************************************************/
  if(WHICHPROBLEM==RADTUBE){



	if(NTUBE==5){
	  defcoord = UNIFORMCOORDS;
	  Rin_array[1]=-20.0;
	  Rin_array[2]=-1.0;
	  Rin_array[3]=-1.0;

	  Rout_array[1]=20.0;
	  Rout_array[2]=1.0;
	  Rout_array[3]=1.0;
	}
	else{
	  defcoord = UNIFORMCOORDS;
	  Rin_array[1]=-15.0;
	  Rin_array[2]=-1.0;
	  Rin_array[3]=-1.0;

	  Rout_array[1]=15.0;
	  Rout_array[2]=1.0;
	  Rout_array[3]=1.0;
	}
  }
  /*************************************************/
  /*************************************************/
  /*************************************************/
  if(WHICHPROBLEM==RADSHADOW){

	defcoord = UNIFORMCOORDS;
	Rin_array[1]=-1.0;
	Rin_array[2]=-1.0;
	Rin_array[3]=-1.0;
	
	Rout_array[1]=3.0;
	Rout_array[2]=1.0;
	Rout_array[3]=1.0;
  }

  /*************************************************/
  /*************************************************/
  /*************************************************/
  if(WHICHPROBLEM==RADDBLSHADOW){

	defcoord = UNIFORMCOORDS;
	Rin_array[1]=-6.0;
	Rin_array[2]=0.0;
	Rin_array[3]=-1.0;
	
	Rout_array[1]=3.0;
	Rout_array[2]=1.5;
	Rout_array[3]=1.0;
  }

  /*************************************************/
  /*************************************************/
  /*************************************************/
  if(WHICHPROBLEM==RADBEAM2D || WHICHPROBLEM==RADBEAM2DKS){


	defcoord = UNIFORMCOORDS;
	Rin_array[1]=0;
	Rin_array[2]=0;
	Rin_array[3]=0;

	Rout_array[1]=1.0;
	Rout_array[2]=1.0;
	Rout_array[3]=1.0;
  


	if (BEAMNO==1){
	  Rin_array[1]=2.6;
	  Rout_array[1]=3.5;
	}
	else if (BEAMNO==2){
	  Rin_array[1]=5.5;
	  Rout_array[1]=7.5;
	}
	else if (BEAMNO==3){
	  Rin_array[1]=14.5;
	  Rout_array[1]=20.5;
	}
	else if (BEAMNO==4){
	  Rin_array[1]=30;
	  Rout_array[1]=50;
	}

	Rin_array[2]=0.99*M_PI*0.5;
	Rout_array[2]=1.01*M_PI*0.5;

	Rin_array[3]=0.0;
	Rout_array[3]=M_PI*0.25;

	
  }
  /*************************************************/
  /*************************************************/
  /*************************************************/
  if(WHICHPROBLEM==ATMSTATIC){


	defcoord = UNIFORMCOORDS;
 
    Rin_array[1]=1E6;
    Rout_array[1]=2E6;

    Rin_array[2]=0.99*M_PI*0.5;
    Rout_array[2]=1.01*M_PI*0.5;

	Rin_array[3]=-1.0;
	Rout_array[3]=1.0;
	
  }
  /*************************************************/
  /*************************************************/
  /*************************************************/
  if(WHICHPROBLEM==RADATM){


	defcoord = UNIFORMCOORDS;
 
    Rin_array[1]=1E6;
    Rout_array[1]=1.4E6;

    Rin_array[2]=0.99*M_PI*0.5;
    Rout_array[2]=1.01*M_PI*0.5;

	Rin_array[3]=-1.0;
	Rout_array[3]=1.0;
	
  }

  /*************************************************/
  /*************************************************/
  /*************************************************/
  if(WHICHPROBLEM==RADWALL){


	defcoord = UNIFORMCOORDS;
	Rin_array[1]=-6;
	Rin_array[2]=0;
	Rin_array[3]=0;

	Rout_array[1]=3.0;
	Rout_array[2]=1.5;
	Rout_array[3]=1.0;
  }

  /*************************************************/
  /*************************************************/
  /*************************************************/
  if(WHICHPROBLEM==RADWAVE){

	defcoord = UNIFORMCOORDS;
	Rin_array[1]=0;
	Rin_array[2]=0;
	Rin_array[3]=0;

	Rout_array[1]=1.0;
	Rout_array[2]=1.0;
	Rout_array[3]=1.0;

  }

  /*************************************************/
  /*************************************************/
  /*************************************************/
  if(WHICHPROBLEM==RADBONDI){

    // KORALTODO: 2.5 to 2E4 in paper
    RADBONDI_MINX=3.5;
    RADBONDI_MAXX=2e3;

    //#define LOGXGRID
    //    FTYPE LOGPAR1=2.2;
    //    FTYPE LOGPAR2=2.;

    //	defcoord = UNIFORMCOORDS;
    defcoord = LOGRUNITH; // Uses R0, Rin, Rout
    R0=0.0;
	Rin=RADBONDI_MINX;
	Rout=RADBONDI_MAXX;

  }




  return(0);
}


int init_grid(void)
{
  
  init_defcoord(); // just avoids splitting function call, here sets R0,Rin,Rout

  return(0);
}



// assumes normalized density
int init_atmosphere(int *whichvel, int*whichcoord,int i, int j, int k, FTYPE *pr)
{
  int funreturn;

  // NO atmosphere
  //  funreturn=user1_init_atmosphere(whichvel, whichcoord,i, j, k, pr);
  //  if(funreturn!=0) return(funreturn);
  //  return(0);

  // tells to do no coordinate transformations
  *whichvel=WHICHVEL;
  *whichcoord=PRIMECOORDS;
  //  return(0);

  return(-1); // no atmosphere set, so don't do anything at all

}

int init_grid_post_set_grid(FTYPE (*prim)[NSTORE2][NSTORE3][NPR], FTYPE (*pstag)[NSTORE2][NSTORE3][NPR], FTYPE (*ucons)[NSTORE2][NSTORE3][NPR], FTYPE (*vpot)[NSTORE1+SHIFTSTORE1][NSTORE2+SHIFTSTORE2][NSTORE3+SHIFTSTORE3], FTYPE (*Bhat)[NSTORE2][NSTORE3][NPR], FTYPE (*panalytic)[NSTORE2][NSTORE3][NPR], FTYPE (*pstaganalytic)[NSTORE2][NSTORE3][NPR], FTYPE (*vpotanalytic)[NSTORE1+SHIFTSTORE1][NSTORE2+SHIFTSTORE2][NSTORE3+SHIFTSTORE3], FTYPE (*Bhatanalytic)[NSTORE2][NSTORE3][NPR], FTYPE (*F1)[NSTORE2][NSTORE3][NPR], FTYPE (*F2)[NSTORE2][NSTORE3][NPR], FTYPE (*F3)[NSTORE2][NSTORE3][NPR], FTYPE (*Atemp)[NSTORE1+SHIFTSTORE1][NSTORE2+SHIFTSTORE2][NSTORE3+SHIFTSTORE3])
{
  int i,j,k;
  FTYPE X[NDIM],V[NDIM],r,th;
  extern void check_spc_singularities_user(void);



  trifprintf("BEGIN check_rmin\n");
  // check rmin
  check_rmin();
  trifprintf("END check_rmin\n");


  // check that singularities are properly represented by code
  trifprintf("BEGIN check_spc_singularities_user\n");
  // SUPERGODMARK: Goes very slowly sometimes randomly for unknown reasons.
  dualfprintf(fail_file,"WARNING: check_spc_singularities_user() oddly stalls sometimes...\n");
  check_spc_singularities_user();
  trifprintf("END check_spc_singularities_user\n");
  dualfprintf(fail_file,"WARNING: done with check_spc_singularities_user(), but it sometimes stalls or goes very very slow for no apparently good reason.  E.g., on NAUTILUS with -O0, very slow checks.  But just putting dualfprintf before and after the above call leads to quick finish.\n");

  
  return(0);

}



//****************************************//
//****************************************//
//****************************************//
// Problem setup constants that only modifies things in init.c (not init.h)
//****************************************//
//****************************************//

#if(WHICHPROBLEM==FLATNESS)

#define KAPPA 0.
#define KAPPAES 0.

// assume KAPPA defines fraction of FF opacity
#define KAPPAUSER(rho,T) (rho*KAPPA*KAPPA_FF_CODE(rho,T))
// assume KAPPAES defines fractoin of ES opacity
#define KAPPAESUSER(rho,T) (rho*KAPPAES*KAPPA_ES_CODE(rho,T))


#endif

//****************************************//
//****************************************//


#if(WHICHPROBLEM==RADPULSE || WHICHPROBLEM==RADPULSEPLANAR || WHICHPROBLEM==RADPULSE3D)




#if(WHICHPROBLEM==RADPULSEPLANAR)

#define KAPPA 0.
//#define KAPPAES (0.0)
//#define KAPPAES (1E-7)
//#define KAPPAES (1E-4*1.09713E-18*1E6)
//#define KAPPAES (1E-4*1.09713E-18*1E3)
//#define KAPPAES (1E-4*1.09713E-18*1E-0)
//#define KAPPAES (1E-4*1.09713E-18*0.2)
//#define KAPPAES (1E-4*1.09713E-18*1E-1)
//#define KAPPAES (1E-4*1.09713E-18*1E-2)
//#define KAPPAES (1E-4*1.09713E-18*1E-3)
//#define KAPPAES (1E-4*1.09713E-18*1E-3*1E-10)

// assume KAPPA defines fraction of FF opacity
//#define KAPPAUSER(rho,T) (rho*KAPPA*KAPPA_FF_CODE(rho,T))
// assume KAPPAES defines fractoin of ES opacity
//#define KAPPAESUSER(rho,T) (rho*KAPPAES*KAPPA_ES_CODE(rho,T))

#define KAPPAES (1E3)
//#define KAPPAES (10.0)
//#define KAPPAES (1E-1)

#define KAPPAUSER(rho,T) (rho*KAPPA)
#define KAPPAESUSER(rho,T) (rho*KAPPAES)


#else // PULSE and PULSE3D

// KAPPAs are fraction of physical FF and ES opacities
#define KAPPA 0.
#define KAPPAES 1.e-30

// assume KAPPA defines fraction of FF opacity
#define KAPPAUSER(rho,T) (rho*KAPPA*KAPPA_FF_CODE(rho,T))
// assume KAPPAES defines fractoin of ES opacity
#define KAPPAESUSER(rho,T) (rho*KAPPAES*KAPPA_ES_CODE(rho,T))


#endif


#endif


//****************************************//
//****************************************//


#if(WHICHPROBLEM==RADBEAMFLAT)


#define KAPPA 0.
#define KAPPAES 0.

// assume KAPPA defines fraction of FF opacity
#define KAPPAUSER(rho,T) (rho*KAPPA*KAPPA_FF_CODE(rho,T))
// assume KAPPAES defines fractoin of ES opacity
#define KAPPAESUSER(rho,T) (rho*KAPPAES*KAPPA_ES_CODE(rho,T))


#endif

//****************************************//
//****************************************//


#if(WHICHPROBLEM==RADTUBE)

#define KAPPAESUSER(rho,T) (0.0)

#if(NTUBE==1)
#define KAPPAUSER(rho,T) (0.4*rho)
#elif(NTUBE==2)
#define KAPPAUSER(rho,T) (0.2*rho)
#elif(NTUBE==3)
#define KAPPAUSER(rho,T) (0.3*rho)
#elif(NTUBE==31)
#define KAPPAUSER(rho,T) (25*rho)
#elif(NTUBE==4)
#define KAPPAUSER(rho,T) (0.08*rho)
#elif(NTUBE==41)
#define KAPPAUSER(rho,T) (0.7*rho)
#elif(NTUBE==5)
#define KAPPAUSER(rho,T) (1000*rho)
#endif


#endif

//****************************************//
//****************************************//


#if(WHICHPROBLEM==RADSHADOW || WHICHPROBLEM==RADDBLSHADOW)

#define KAPPAUSER(rho,T) (rho*1E2)
//#define KAPPAUSER(rho,T) (rho*1E0)
#define KAPPAESUSER(rho,T) (rho*0.0)


#endif


//****************************************//
//****************************************//


//****************************************//
//****************************************//


#if(WHICHPROBLEM==RADBEAM2D || WHICHPROBLEM==RADBEAM2DKS)


#define KAPPA 0.
#define KAPPAES 0.

// assume KAPPA defines fraction of FF opacity
#define KAPPAUSER(rho,T) (rho*KAPPA*KAPPA_FF_CODE(rho,T))
// assume KAPPAES defines fractoin of ES opacity
#define KAPPAESUSER(rho,T) (rho*KAPPAES*KAPPA_ES_CODE(rho,T))


#endif


#if(WHICHPROBLEM==ATMSTATIC)


#define KAPPA 0.
#define KAPPAES 0.

// assume KAPPA defines fraction of FF opacity
#define KAPPAUSER(rho,T) (rho*KAPPA*KAPPA_FF_CODE(rho,T))
// assume KAPPAES defines fractoin of ES opacity
#define KAPPAESUSER(rho,T) (rho*KAPPAES*KAPPA_ES_CODE(rho,T))


#endif


#if(WHICHPROBLEM==RADATM)


#define KAPPA 0.
#define KAPPAES 1. // only scattering

// assume KAPPA defines fraction of FF opacity
#define KAPPAUSER(rho,T) (rho*KAPPA*KAPPA_FF_CODE(rho,T))
// assume KAPPAES defines fractoin of ES opacity
#define KAPPAESUSER(rho,T) (rho*KAPPAES*KAPPA_ES_CODE(rho,T))


#endif


#if(WHICHPROBLEM==RADWALL)


#define KAPPA 0.
#define KAPPAES 0.

// assume KAPPA defines fraction of FF opacity
#define KAPPAUSER(rho,T) (rho*KAPPA*KAPPA_FF_CODE(rho,T))
// assume KAPPAES defines fractoin of ES opacity
#define KAPPAESUSER(rho,T) (rho*KAPPAES*KAPPA_ES_CODE(rho,T))


#endif




#if(WHICHPROBLEM==RADWAVE)

#define KAPPAUSER(rho,T) (rho*RADWAVE_KAPPA)
#define KAPPAESUSER(rho,T) (rho*RADWAVE_KAPPAES)

#endif


#if(WHICHPROBLEM==RADBONDI)


#define KAPPA 1.0
#define KAPPAES 1.0

// assume KAPPA defines fraction of FF opacity
#define KAPPAUSER(rho,T) (rho*KAPPA*KAPPA_FF_CODE(rho,T))
// assume KAPPAES defines fractoin of ES opacity
#define KAPPAESUSER(rho,T) (rho*KAPPAES*KAPPA_ES_CODE(rho,T))


#endif


int init_primitives(FTYPE (*prim)[NSTORE2][NSTORE3][NPR], FTYPE (*pstag)[NSTORE2][NSTORE3][NPR], FTYPE (*ucons)[NSTORE2][NSTORE3][NPR], FTYPE (*vpot)[NSTORE1+SHIFTSTORE1][NSTORE2+SHIFTSTORE2][NSTORE3+SHIFTSTORE3], FTYPE (*Bhat)[NSTORE2][NSTORE3][NPR], FTYPE (*panalytic)[NSTORE2][NSTORE3][NPR], FTYPE (*pstaganalytic)[NSTORE2][NSTORE3][NPR], FTYPE (*vpotanalytic)[NSTORE1+SHIFTSTORE1][NSTORE2+SHIFTSTORE2][NSTORE3+SHIFTSTORE3], FTYPE (*Bhatanalytic)[NSTORE2][NSTORE3][NPR], FTYPE (*F1)[NSTORE2][NSTORE3][NPR], FTYPE (*F2)[NSTORE2][NSTORE3][NPR], FTYPE (*F3)[NSTORE2][NSTORE3][NPR], FTYPE (*Atemp)[NSTORE1+SHIFTSTORE1][NSTORE2+SHIFTSTORE2][NSTORE3+SHIFTSTORE3])
{
  int funreturn;
  int inittype;


  THETAROT = 0.0; // define rho,u,v,B as if no rotation


  inittype=1;

  funreturn=user1_init_primitives(inittype, prim, pstag, ucons, vpot, Bhat, panalytic, pstaganalytic, vpotanalytic, Bhatanalytic, F1, F2, F3,Atemp);
  if(funreturn!=0) return(funreturn);



  return(0);


}



int init_dsandvels(int inittype, int pos, int *whichvel, int*whichcoord, SFTYPE time, int i, int j, int k, FTYPE *pr, FTYPE *pstag)
{
  int init_dsandvels_koral(int *whichvel, int*whichcoord, int i, int j, int k, FTYPE *pr, FTYPE *pstag);

  // assume inittype not used, pos==CENT, and time doesn't matter (e.g. only used at t=0)

  init_dsandvels_koral(whichvel, whichcoord,  i,  j,  k, pr, pstag);

  return(0);
}


// unnormalized density
int init_dsandvels_koral(int *whichvel, int*whichcoord, int i, int j, int k, FTYPE *pr, FTYPE *pstag)
{
  FTYPE X[NDIM],V[NDIM];
  int pl,pliter;

  //  coord(i, j, k, CENT, X);
  //  bl_coord(X, V);
  //  r=V[1];
  //  th=V[2];

  /*************************************************/
  /*************************************************/
  /*************************************************/
  if(WHICHPROBLEM==FLATNESS){

    
	pr[RHO] = 1./RHOBAR ; // i.e. 1g/cm^3
	pr[UU] = 0.1/RHOBAR; // i.e. c^2 * 1g/cm^3 of energy density
	pr[U1] = 0 ;
	pr[U2] = 0 ;    
	pr[U3] = 0 ;

	// just define some field
	pr[B1]=0.0;
	pr[B2]=0.0;
	pr[B3]=0.0;

	if(FLUXB==FLUXCTSTAG){
	  // assume pstag later defined really using vector potential or directly assignment of B3 in axisymmetry
	  PLOOPBONLY(pl) pstag[pl]=pr[pl];
	}


	pr[URAD0] = 1./RHOBAR; // i.e. c^2 * 1g/cm^3 of energy density
	pr[URAD1] = 0 ;
	pr[URAD2] = 0 ;    
	pr[URAD3] = 0 ;

	*whichvel=WHICHVEL;
	*whichcoord=CARTMINKMETRIC2;
	return(0);
  }





  /*************************************************/
  /*************************************************/
  if(WHICHPROBLEM==RADBEAMFLAT){

    
	pr[RHO] = RADBEAMFLAT_RHO/RHOBAR ;
	pr[UU] = RADBEAMFLAT_UU/RHOBAR; // RADBEAMFLAT_UU was set in per c^2 units
	pr[U1] = 0 ;
	pr[U2] = 0 ;    
	pr[U3] = 0 ;

	// just define some field
	pr[B1]=0.0;
	pr[B2]=0.0;
	pr[B3]=0.0;

	if(FLUXB==FLUXCTSTAG){
	  // assume pstag later defined really using vector potential or directly assignment of B3 in axisymmetry
	  PLOOPBONLY(pl) pstag[pl]=pr[pl];
	}


	pr[PRAD0] = RADBEAMFLAT_ERAD/RHOBAR; // RADBEAMFLAT_ERAD was set in per c^2 units
	pr[PRAD1] = 0 ;
	pr[PRAD2] = 0 ;    
	pr[PRAD3] = 0 ;

	// no transformations required since only setting radiation frame E that is PRAD0 itself.

	*whichvel=WHICHVEL;
	*whichcoord=CARTMINKMETRIC2;
	return(0);
  }




  /*************************************************/
  /*************************************************/
  if(WHICHPROBLEM==RADPULSE || WHICHPROBLEM==RADPULSEPLANAR || WHICHPROBLEM==RADPULSE3D){
    // now avoids use of real units as in Koral paper (unlike Koral code)

	FTYPE Trad,Tgas,ERAD,uint;
	FTYPE xx,yy,zz,rsq;
	coord(i, j, k, CENT, X);
	bl_coord(X, V);
	xx=V[1];
	yy=V[2];
	zz=V[3];


	if(WHICHPROBLEM==RADPULSE || WHICHPROBLEM==RADPULSE3D){
	  rsq=(xx)*(xx)+(yy)*(yy)+(zz)*(zz);
	}
	else if(WHICHPROBLEM==RADPULSEPLANAR){
	  rsq=(xx)*(xx);

	}

	//FTYPE RHO_AMB (1.e0) // in grams per cm^3
    //	FTYPE RHO_AMB=(MPERSUN*MSUN/(LBAR*LBAR*LBAR)); // in grams per cm^3 to match koral's units with rho=1
    FTYPE RHO_AMB=1.0;
	FTYPE T_AMB=1.0E6/TEMPBAR;

	FTYPE BLOBP=100.;
	FTYPE BLOBW=5.;

	// radiation temperature is distributed
	Trad=T_AMB*(1.+BLOBP*exp(-rsq/(BLOBW*BLOBW)));
	ERAD=calc_LTE_EfromT(Trad);

	//flat gas profiles
	Tgas=T_AMB;
	FTYPE rho;
	rho=RHO_AMB;
	uint=calc_PEQ_ufromTrho(Tgas,rho);

	//	dualfprintf(fail_file,"IC i=%d Trad=%g ERAD=%g Tgas=%g rho=%g uint=%g\n",i,Trad,ERAD,Tgas,rho,uint);

   
	pr[RHO] = rho;
	pr[UU] = uint;
	pr[U1] = 0 ;
	pr[U2] = 0 ;    
	pr[U3] = 0 ;

	// just define some field
	pr[B1]=0.0;
	pr[B2]=0.0;
	pr[B3]=0.0;

	if(FLUXB==FLUXCTSTAG){
	  // assume pstag later defined really using vector potential or directly assignment of B3 in axisymmetry
	  PLOOPBONLY(pl) pstag[pl]=pr[pl];
	}

	FTYPE Fx,Fy,Fz;
	Fx=Fy=Fz=0.0;

	pr[URAD0] = ERAD ;
	pr[URAD1] = Fx ;
	pr[URAD2] = Fy ;    
	pr[URAD3] = Fz ;

    // no transformation, but only because tuned units to be like koral and so ERAD gives same value and also because no Flux.

	*whichvel=WHICHVEL;
	*whichcoord=CARTMINKMETRIC2;
	return(0);
  }



  /*************************************************/
  /*************************************************/
  if(WHICHPROBLEM==RADTUBE){

	FTYPE rho,mx,my,mz,m,ERAD,uint,E0,Fx,Fy,Fz,pLTE;  
	FTYPE rho0,Tgas0,ur,Tgas,Trad,r,rcm,prad,pgas,vx,ut,ux;
	FTYPE xx,yy,zz;
	coord(i, j, k, CENT, X);
	bl_coord(X, V);
	xx=V[1];
	yy=V[2];
	zz=V[3];
  

	// set fluid values.  Also set radiation ff primitives (E,F^i), which are set in fluid frame orthonormal basis
	if(xx<(Rout_array[1]+Rin_array[1])/2.){
	  rho=1.;
	  if(NTUBE==1) {uint = 3.e-5 / (gamideal - 1.); ERAD=1.e-8; Fx=1.e-2*ERAD;ux=0.015;}
	  if(NTUBE==2) {uint = 4.e-3 / (gamideal - 1.);ERAD=2.e-5; Fx=1.e-2*ERAD;ux=0.25;}
	  if(NTUBE==3 || NTUBE==31) {uint = 60. / (gamideal - 1.);ERAD=2.; Fx=1.e-2*ERAD;ux=10.;}
	  if(NTUBE==4 || NTUBE==41) {uint = 6.e-3 / (gamideal - 1.);ERAD=0.18; Fx=1.e-2*ERAD;ux=0.69;}	  
	  if(NTUBE==5) {uint = 60. / (gamideal - 1.);ERAD=2.; Fx=1.e-2*ERAD;ux=1.25;}
	}
	else{
	  if(NTUBE==1) {rho=2.4;uint = 1.61e-4/ (gamideal - 1.); ERAD=2.51e-7; Fx=1.e-2*ERAD;ux=6.25e-3;}
	  if(NTUBE==2) {rho=3.11;uint = 0.04512 / (gamideal - 1.);ERAD=3.46e-3; Fx=1.e-2*ERAD;ux=0.0804;}
	  if(NTUBE==3 || NTUBE==31) {rho=8.0;uint = 2.34e3 / (gamideal - 1.);ERAD=1.14e3; Fx=1.e-2*ERAD;ux=1.25;}
	  if(NTUBE==4 || NTUBE==41) {rho=3.65;uint =3.59e-2 / (gamideal - 1.);ERAD=1.30; Fx=1.e-2*ERAD;ux=0.189;}	  
	  if(NTUBE==5) {rho=1.0;uint = 60. / (gamideal - 1.);ERAD=2.; Fx=1.e-2*ERAD;ux=1.10;}
	}
    Fy=Fz=0.0;

  
	pr[RHO] = rho;
	pr[UU] = uint;
	pr[U1] = ux ; // ux is 4-velocity
	pr[U2] = 0 ;    
	pr[U3] = 0 ;

	// just define some field
	pr[B1]=0.0;
	pr[B2]=0.0;
	pr[B3]=0.0;
  
	if(FLUXB==FLUXCTSTAG){
	  // assume pstag later defined really using vector potential or directly assignment of B3 in axisymmetry
	  PLOOPBONLY(pl) pstag[pl]=pr[pl];
	}

	pr[PRAD0] = 0 ; // so triggers failure if used
	pr[PRAD1] = 0 ;
	pr[PRAD2] = 0 ;    
	pr[PRAD3] = 0 ;


    //E, F^i in orthonormal fluid frame
    FTYPE pradffortho[NPR];
    pradffortho[PRAD0] = ERAD;
    pradffortho[PRAD1] = Fx;
    pradffortho[PRAD2] = Fy;
    pradffortho[PRAD3] = Fz;


	// Transform these fluid frame E,F^i to lab frame coordinate basis primitives
	*whichvel=VEL4;
	*whichcoord=CARTMINKMETRIC2;
	prad_fforlab(whichvel, whichcoord, FF2LAB, i,j,k,CENT,NULL,pradffortho,pr, pr);

	//  PLOOPRADONLY(pl) dualfprintf(fail_file,"FOO1: i=%d pl=%d pr=%g\n",ptrgeomreal->i,pl,pr[pl]);

  
	return(0);
  

  }


  /*************************************************/
  /*************************************************/
  if(WHICHPROBLEM==RADSHADOW || WHICHPROBLEM==RADDBLSHADOW){

	//	FTYPE MASS=10.0;
	FTYPE TAMB=1.e7/TEMPBAR;
	FTYPE RHOAMB=1.e-4;
	FTYPE RHOBLOB=1.e3;
	FTYPE BLOBW=0.22;

	FTYPE Trad,Tgas,ERAD;
	FTYPE xx,yy,zz,rsq;
	coord(i, j, k, CENT, X);
	bl_coord(X, V);
	xx=V[1];
	yy=V[2];
	zz=V[3];

	FTYPE rho,uint,Fx,Fy,Fz,pLTE;  

	/*****************************/
	
	//	FTYPE pamb=calc_PEQ_ufromTrho(TAMB,RHOAMB);
	rsq=xx*xx+yy*yy+zz*zz;
	rho=(RHOBLOB-RHOAMB)*exp(-sqrt(rsq)/(BLOBW*BLOBW))+RHOAMB;      
	Tgas=TAMB*RHOAMB/rho;
	// Paper says T = T0*rho/RHOAMB
	//	Tgas = TAMB*rho/RHOAMB;
	// for constant gas pressure, P=\rho T implies rho T = constant so that T\propto 1/rho
	uint=calc_PEQ_ufromTrho(Tgas,rho);

	Trad=TAMB;
	ERAD=calc_LTE_EfromT(Trad);

	//	dualfprintf(fail_file,"i=%d j=%d rho=%g Trad=%g uint=%g ERAD=%g\n",i,j,rho,Trad,uint,ERAD);

	Fx=0.;
	Fy=0.;
	Fz=0.;	      	      

	FTYPE VV=0.; // assumed orthonormal 4-velocity

	pr[RHO] = rho;
	pr[UU] = uint;
	pr[U1] = -VV ;
	pr[U2] = 0 ;    
	pr[U3] = 0 ;

	// just define some field
	pr[B1]=0.0;
	pr[B2]=0.0;
	pr[B3]=0.0;

	if(FLUXB==FLUXCTSTAG){
	  // assume pstag later defined really using vector potential or directly assignment of B3 in axisymmetry
	  PLOOPBONLY(pl) pstag[pl]=pr[pl];
	}


	pr[PRAD0] = 0 ; // so triggers failure if used
	pr[PRAD1] = 0 ;
	pr[PRAD2] = 0 ;    
	pr[PRAD3] = 0 ;

    //E, F^i in orthonormal fluid frame
    FTYPE pradffortho[NPR];
    pradffortho[PRAD0] = ERAD;
    pradffortho[PRAD1] = Fx;
    pradffortho[PRAD2] = Fy;
    pradffortho[PRAD3] = Fz;

	// Transform these fluid frame E,F^i to lab frame coordinate basis primitives
	*whichvel=VEL4;
	*whichcoord=CARTMINKMETRIC2;
	prad_fforlab(whichvel, whichcoord, FF2LAB, i,j,k,CENT,NULL,pradffortho,pr, pr);
   
	//	PLOOP(pliter,pl) dualfprintf(fail_file,"pl=%d pr=%g\n",pl,pr[pl]);


	return(0);
  }



  /*************************************************/
  /*************************************************/
  if(WHICHPROBLEM==RADBEAM2D || WHICHPROBLEM==RADBEAM2DKS){


	FTYPE RHOAMB=1.e0/RHOBAR;
	FTYPE TAMB=1e7/TEMPBAR;
	int BLOB=0; // whether to put blob in way of beam
	FTYPE BLOBW=.1;
	FTYPE BLOBP=100000.;
	FTYPE BLOBX=10.;
	FTYPE BLOBZ=Pi/20.;
	FTYPE PAR_D=1./RHOBAR;
	FTYPE PAR_E=1e-4/RHOBAR;
    FTYPE Fx,Fy,Fz;

	FTYPE xx,yy,zz,rsq;
	coord(i, j, k, CENT, X);
	bl_coord(X, V);
	xx=V[1];
	yy=V[2];
	zz=V[3];

	*whichvel=VEL4;
	*whichcoord=MCOORD;

	// if BLOB, override density with blob density
	FTYPE rhoblob;
	rhoblob=RHOAMB*(1.+BLOBP*exp(-((xx-BLOBX)*(xx-BLOBX)+(yy)*(yy)+(zz-BLOBZ)*(zz-BLOBZ))/BLOBW/BLOBW));

	//zaczynam jednak od profilu analitycznego:   
	FTYPE ERADAMB;
	FTYPE rho,uint,Vr;
	if(FLATBACKGROUND){
	  Vr=0.;
	  rho=RHOAMB;
	  if(BLOB) rho=rhoblob;
	  uint=calc_PEQ_ufromTrho(TAMB,rho);
	  ERADAMB=calc_LTE_EfromT(TAMB);
	  //	  ERADAMB=calc_LTE_Efromurho(uint,rho);
	}
	else{
	  FTYPE r=V[1];
	  FTYPE mD=PAR_D/(r*r*sqrt(2./r*(1.-2./r)));
	  FTYPE mE=PAR_E/(pow(r*r*sqrt(2./r),gamideal)*pow(1.-2./r,(gamideal+1.)/4.));
	  Vr=sqrt(2./r)*(1.-2./r);

	  // get metric grid geometry for these ICs
	  int getprim=0;
	  struct of_geom geomrealdontuse;
	  struct of_geom *ptrgeomreal=&geomrealdontuse;
	  gset(getprim,*whichcoord,i,j,k,ptrgeomreal);

	  FTYPE W=1./sqrt(1.-Vr*Vr*ptrgeomreal->gcov[GIND(1,1)]);
	  rho=PAR_D/(r*r*sqrt(2./r));
	  if(BLOB) rho += rhoblob;
	  FTYPE T=TAMB;
	  //			FTYPE ERAD=calc_LTE_EfromT(T);
	  uint=mE/W;
	  ERADAMB=calc_LTE_Efromurho(uint,rho);
	}

    Fx=Fy=Fz=0;

	//test!
	//Vr=0.7;

	pr[RHO] = rho ;
	pr[UU]  = uint;
	pr[U1]  = -Vr; // VEL4
	pr[U2]  = 0 ;  // static in VEL4
	pr[U3]  = 0 ;  // static in VEL4

	// just define some field
	pr[B1]=0.0;
	pr[B2]=0.0;
	pr[B3]=0.0;

	if(FLUXB==FLUXCTSTAG){
	  // assume pstag later defined really using vector potential or directly assignment of B3 in axisymmetry
	  PLOOPBONLY(pl) pstag[pl]=pr[pl];
	}

	pr[PRAD0] = ERADAMB;
	pr[PRAD1] = 0.0 ; // static in VEL4
	pr[PRAD2] = 0.0 ;    
	pr[PRAD3] = 0.0 ;

    //E, F^i in orthonormal fluid frame
    FTYPE pradffortho[NPR];
    pradffortho[PRAD0] = ERADAMB;
    pradffortho[PRAD1] = Fx;
    pradffortho[PRAD2] = Fy;
    pradffortho[PRAD3] = Fz;

	// Transform these fluid frame E,F^i to lab frame coordinate basis primitives
	prad_fforlab(whichvel, whichcoord, FF2LAB, i,j,k,CENT,NULL,pradffortho,pr, pr);

	return(0);
  }

  /*************************************************/
  /*************************************************/
  if(WHICHPROBLEM==ATMSTATIC){

	FTYPE xx,yy,zz,rsq;
	coord(i, j, k, CENT, X);
	bl_coord(X, V);
	xx=V[1];
	yy=V[2];
	zz=V[3];

	*whichvel=VEL4;
	*whichcoord=MCOORD;


    FTYPE rho0=1.;
    FTYPE r0=2.e6;
    FTYPE u0=0.0001;
    FTYPE r=xx;

    FTYPE rho,uint;
    rho=rho0*r0/r;
    uint=u0*r0*r0/r/r;
    
    //    FTYPE E=exp(1.);
    
    uint=(4*r*u0 - 4*r*u0*gamideal - 2*r*rho0 + 2*r0*rho0 + r*r0*rho0*Log(-2 + r) - r*r0*rho0*Log(r) - r*r0*rho0*Log(-2 + r0) + r*r0*rho0*Log(r0))/(4*r - 4*r*gamideal);

	FTYPE uradx,urady,uradz;
	uradx=urady=uradz=0.;
    
	pr[RHO] = rho ;
	pr[UU]  = uint;
	pr[U1]  = 0 ;
	pr[U2]  = 0 ;    
	pr[U3]  = 0 ;

	// just define some field
	pr[B1]=0.0;
	pr[B2]=0.0;
	pr[B3]=0.0;

	if(FLUXB==FLUXCTSTAG){
	  // assume pstag later defined really using vector potential or directly assignment of B3 in axisymmetry
	  PLOOPBONLY(pl) pstag[pl]=pr[pl];
	}


    //	pr[PRAD0] = ERADLIMIT;
	pr[PRAD0] = uint*1E-10;
	pr[PRAD1] = uradx ;
	pr[PRAD2] = urady ;    
	pr[PRAD3] = uradz ;

	// no transformations required since only setting fluid-frame E that is PRAD0 itself. (i.e. urad(xyz)=0)

	//	*whichvel=WHICHVEL;
	*whichvel=VEL4;
	*whichcoord=MCOORD;

	return(0);
  }


  /*************************************************/
  /*************************************************/
  if(WHICHPROBLEM==RADATM){


    // KORALTODO: t=0 R^t_t [lab] R^t_x [lab] or prad0 (E rad frame] prad1 [rad 4-vel] are off by about 16% compared to koral

    RADATM_MDOTEDD=(2.23/16.*1e18*MPERSUN)/(MBAR/TBAR);
    RADATM_LUMEDD=(1.25e38*MPERSUN)/(ENBAR/TBAR);
    RADATM_THINRADATM=1;
    RADATM_FERATIO=.99999; // koral code
    //    RADATM_FERATIO=.99; // koral paper
    RADATM_FRATIO=.1; // 1 = edd limit.  They ran 1E-10, 0.1, 0.5, 1.0.
    RADATM_RHOAMB=1E-15/RHOBAR;
    RADATM_TAMB=1.e6/TEMPBAR;


    FTYPE MINX=Rin_array[1];
    FTYPE kappaesperrho=calc_kappaes_user(1,0, 0,0,0);
    FTYPE FLUXLEFT=RADATM_FRATIO/kappaesperrho/pow(MINX,2.0);



	FTYPE xx,yy,zz,rsq;
	coord(i, j, k, CENT, X);
	bl_coord(X, V);
	xx=V[1];
	yy=V[2];
	zz=V[3];

	*whichvel=VEL4;
	*whichcoord=MCOORD;

    //at outern boundary
    FTYPE f = (FTYPE)kappaesperrho*FLUXLEFT*MINX*MINX;

    FTYPE p0=RADATM_RHOAMB*RADATM_TAMB;
    FTYPE KKK=p0/pow(RADATM_RHOAMB,gamideal);
    FTYPE C3=gamideal*KKK/(gamideal-1.)*pow(RADATM_RHOAMB,gamideal-1.)-(1.-f)*(1./MINX+0.*1./MINX/MINX+0.*4./3./MINX/MINX/MINX);

    FTYPE rho=pow((gamideal-1.0)/gamideal/KKK*(C3+(1.-f)*(1./xx + 0.*1./xx/xx + 0.*4./3./xx/xx/xx)),1./(gamideal-1.0));

    FTYPE pre=KKK*pow(rho,gamideal);

    FTYPE uint=pre/(gamideal-1.0);

    FTYPE Fz=0;
    FTYPE Fy=0.;
    FTYPE Fx=FLUXLEFT*(MINX/xx)*(MINX/xx);

    FTYPE ERAD;
    if(RADATM_THINRADATM){
      ERAD=Fx/RADATM_FERATIO;
    }
    else{
      ERAD=calc_LTE_EfromT(calc_PEQ_Tfromurho(uint,rho));
    }

    dualfprintf(fail_file,"i=%d f=%g p0=%g KKK=%g C3=%g rho=%g uint=%g Fx=%g ERAD=%g : kappaesperrho=%g \n",i,f,p0,KKK,C3,rho,uint,Fx,ERAD , kappaesperrho);

    pr[RHO] = rho ;
	pr[UU]  = uint;
	pr[U1]  = 0 ;
	pr[U2]  = 0 ;    
	pr[U3]  = 0 ;

	// just define some field
	pr[B1]=0.0;
	pr[B2]=0.0;
	pr[B3]=0.0;

	if(FLUXB==FLUXCTSTAG){
	  // assume pstag later defined really using vector potential or directly assignment of B3 in axisymmetry
	  PLOOPBONLY(pl) pstag[pl]=pr[pl];
	}


	pr[PRAD0] = 0 ; // so triggers failure if used
	pr[PRAD1] = 0 ;
	pr[PRAD2] = 0 ;    
	pr[PRAD3] = 0 ;

    //E, F^i in orthonormal fluid frame
    FTYPE pradffortho[NPR];
    pradffortho[PRAD0] = ERAD;
    pradffortho[PRAD1] = Fx;
    pradffortho[PRAD2] = Fy;
    pradffortho[PRAD3] = Fz;

	// Transform these fluid frame E,F^i to lab frame coordinate basis primitives
	*whichvel=VEL4;
	*whichcoord=MCOORD;
	prad_fforlab(whichvel, whichcoord, FF2LAB, i,j,k,CENT,NULL,pradffortho, pr, pr);

    dualfprintf(fail_file,"AFTER: i=%d rho=%g uint=%g vx=%g uradx=%g ERAD=%g\n",i,pr[RHO],pr[UU],pr[U1],pr[URAD1],pr[URAD0]);

	return(0);
  }



  /*************************************************/
  /*************************************************/
  if(WHICHPROBLEM==RADWALL){

    
    // direct assignments since simple
	pr[RHO] = 1.0 ;
	pr[UU] = 1.0;
	pr[U1] = 0 ;
	pr[U2] = 0 ;    
	pr[U3] = 0 ;

	// just define some field
	pr[B1]=0.0;
	pr[B2]=0.0;
	pr[B3]=0.0;

	if(FLUXB==FLUXCTSTAG){
	  // assume pstag later defined really using vector potential or directly assignment of B3 in axisymmetry
	  PLOOPBONLY(pl) pstag[pl]=pr[pl];
	}


    // direct assignments since simple
	pr[PRAD0] = 1.0;
	pr[PRAD1] = 0 ;
	pr[PRAD2] = 0 ;    
	pr[PRAD3] = 0 ;

	// no transformations required since only setting fluid-frame E that is PRAD0 itself.

	*whichvel=WHICHVEL;
	*whichcoord=CARTMINKMETRIC2;
	return(0);
  }




  /*************************************************/
  /*************************************************/
  if(WHICHPROBLEM==RADWAVE){

	FTYPE rho,ERAD,uint,Fx,Fy,Fz;
	FTYPE vx;
	FTYPE xx,yy,zz;
	coord(i, j, k, CENT, X);
	bl_coord(X, V);
	xx=V[1];
	yy=V[2];
	zz=V[3];


    // default
	Fx=Fy=Fz=0.0;


    FTYPE time=0.;

    //Jiang+12 waves
    if(RADWAVE_NWAVE==5){

      //printf("RHOZERO = %g\nUINT = %g\nT = %g\nERAD = %g\nARAD = %g\n",RADWAVE_RHOZERO,RADWAVE_UINT,RADWAVE_TEMP,RADWAVE_ERAD,ARAD_RAD_CODE);getchar();


      rho=RADWAVE_RHOZERO*(1+RADWAVE_DRRE*exp(-RADWAVE_OMIM*time)*(cos(RADWAVE_OMRE*time-RADWAVE_KK*xx)-RADWAVE_DRIM/RADWAVE_DRRE*sin(RADWAVE_OMRE*time-RADWAVE_KK*xx)));
      //FTYPE RADWAVE_DURE=RADWAVE_DPRE/(gam-1.); FTYPE RADWAVE_DUIM=RADWAVE_DPIM/(gam-1.);
      uint=RADWAVE_UINT*(1.+RADWAVE_DURE*exp(-RADWAVE_OMIM*time)*(cos(RADWAVE_OMRE*time-RADWAVE_KK*xx)-RADWAVE_DUIM/RADWAVE_DURE*sin(RADWAVE_OMRE*time-RADWAVE_KK*xx))) ;
      FTYPE cs=1/RADWAVE_CC;
      vx=0. + RADWAVE_DVRE*exp(-RADWAVE_OMIM*time)*(cos(RADWAVE_OMRE*time-RADWAVE_KK*xx)-RADWAVE_DVIM/RADWAVE_DVRE*sin(RADWAVE_OMRE*time-RADWAVE_KK*xx)) ; //RADWAVE_DVRE absolute!
      ERAD=RADWAVE_ERAD*(1+RADWAVE_DERE*exp(-RADWAVE_OMIM*time)*(cos(RADWAVE_OMRE*time-RADWAVE_KK*xx)-RADWAVE_DEIM/RADWAVE_DERE*sin(RADWAVE_OMRE*time-RADWAVE_KK*xx)));
      Fx=0. + RADWAVE_ERAD*RADWAVE_DFRE*exp(-RADWAVE_OMIM*time)*(cos(RADWAVE_OMRE*time-RADWAVE_KK*xx)-RADWAVE_DFIM/RADWAVE_DFRE*sin(RADWAVE_OMRE*time-RADWAVE_KK*xx));
      Fz=Fy=0.;

      //rho=RADWAVE_RHOZERO;
      //uint=RADWAVE_UINT;
      //ERAD=RADWAVE_ERAD;
      //vx=0;
      //Fx=0.;
    }

    //hydro density wave
    if(RADWAVE_NWAVE==1){
      rho=RADWAVE_RHOZERO*(1.+RADWAVE_AAA*cos(RADWAVE_KK*xx));
      uint=RADWAVE_UINT;
      vx=RADWAVE_VX;
      ERAD=1E-10*uint; // no radiation
      Fx=Fz=Fy=0.;
    }

    //hydro sound wave
	if(RADWAVE_NWAVE==2){
      rho=RADWAVE_RHOZERO*(1.+RADWAVE_AAA*cos(RADWAVE_KK*xx));
      uint=RADWAVE_UINT*(1.+gam*RADWAVE_AAA*cos(RADWAVE_KK*xx));
      FTYPE cs=1./RADWAVE_CC;
      vx=RADWAVE_AAA*cos(RADWAVE_KK*xx)*cs;
      ERAD=RADWAVE_ERAD; // KORALTODO: Why does koral not set #define RADIATION for this test?
      Fx=Fz=Fy=0.;
    }

    //radiative hydro density wave
	if(RADWAVE_NWAVE==3){
      rho=RADWAVE_RHOZERO*(1.+RADWAVE_AAA*cos(RADWAVE_KK*xx));
      uint=RADWAVE_UINT;
      vx=RADWAVE_VX;
      ERAD=RADWAVE_ERAD;
      Fx=Fz=Fy=0.;
    }

    //radiative sound wave
	if(RADWAVE_NWAVE==4){
      rho=RADWAVE_RHOZERO*(1.+RADWAVE_GASFACTOR*RADWAVE_AAA*cos(RADWAVE_KK*xx));
      uint=RADWAVE_UINT*(1.+RADWAVE_GASFACTOR*gam*RADWAVE_AAA*cos(RADWAVE_KK*xx));
      FTYPE cs=1./RADWAVE_CC;
      vx=RADWAVE_GASFACTOR*RADWAVE_AAA*cos(RADWAVE_KK*xx)*cs;
      ERAD=RADWAVE_ERAD*(1.+RADWAVE_ERADFACTOR*RADWAVE_AAA*cos(RADWAVE_KK*xx));
      Fx=Fz=Fy=0.;
    }



	pr[RHO] = rho;
	pr[UU] = uint;
	pr[U1] = vx ; // vx is 3-velocity
	pr[U2] = 0 ;    
	pr[U3] = 0 ;

	// just define some field
	pr[B1]=0.0;
	pr[B2]=0.0;
	pr[B3]=0.0;
  
	if(FLUXB==FLUXCTSTAG){
	  // assume pstag later defined really using vector potential or directly assignment of B3 in axisymmetry
	  PLOOPBONLY(pl) pstag[pl]=pr[pl];
	}



	pr[PRAD0] = 0 ; // so triggers failure if used
	pr[PRAD1] = 0 ;
	pr[PRAD2] = 0 ;    
	pr[PRAD3] = 0 ;

    //E, F^i in orthonormal fluid frame
    FTYPE pradffortho[NPR];
    pradffortho[PRAD0] = ERAD;
    pradffortho[PRAD1] = Fx;
    pradffortho[PRAD2] = Fy;
    pradffortho[PRAD3] = Fz;


	// Transform these fluid frame E,F^i to lab frame coordinate basis primitives
	*whichvel=VEL3;
	*whichcoord=CARTMINKMETRIC2;
    prad_fforlab(whichvel, whichcoord, FF2LAB, i,j,k,CENT,NULL, pradffortho, pr, pr);

    //  PLOOPRADONLY(pl) dualfprintf(fail_file,"FOO1: i=%d pl=%d pr=%g\n",ptrgeomreal->i,pl,pr[pl]);

 
	return(0);
  

  }








  /*************************************************/
  /*************************************************/
  if(WHICHPROBLEM==RADBONDI){


	FTYPE xx,yy,zz,rsq;
	coord(i, j, k, CENT, X);
	bl_coord(X, V);
	xx=V[1];
	yy=V[2];
	zz=V[3];

	*whichcoord=MCOORD;
    // get metric grid geometry for these ICs
    int getprim=0;
    struct of_geom geomrealdontuse;
    struct of_geom *ptrgeomreal=&geomrealdontuse;
    gset(getprim,*whichcoord,i,j,k,ptrgeomreal);

    
    FTYPE rho,ERAD,uint;
    FTYPE rho0,Tgas0,ur,Tgas,Trad,r,rcm,prad,pgas,vx,ut;

    FTYPE Fx,Fy,Fz;
    Fx=Fy=Fz=0;

    //at outern boundary
    r=RADBONDI_MAXX;
    ur=-sqrt(2./r);
    rho0=-RADBONDI_MDOTPEREDD*RADBONDI_MDOTEDD/(4.*Pi*r*r*ur);
    Tgas0=RADBONDI_TGAS0;
            
    //at given cell
    r=xx;
    ur=-sqrt(2./r);    
    ut=sqrt((-1.-ur*ur*ptrgeomreal->gcov[GIND(1,1)])/ptrgeomreal->gcov[GIND(0,0)]);
    vx=ur/ut;  
    rho=-RADBONDI_MDOTPEREDD*RADBONDI_MDOTEDD/(4.*Pi*r*r*ur);
    Tgas=Tgas0*pow(rho/rho0,gam-1.);      

    uint=calc_PEQ_ufromTrho(Tgas,rho);

    pgas=rho*Tgas;
    prad=RADBONDI_PRADGAS*pgas;
    ERAD=prad*3.;
    
	pr[RHO] = rho ;
	pr[UU]  = uint;
	pr[U1]  = vx;
	pr[U2]  = 0 ;    
	pr[U3]  = 0 ;

	// just define some field
	pr[B1]=0.0;
	pr[B2]=0.0;
	pr[B3]=0.0;

	if(FLUXB==FLUXCTSTAG){
	  // assume pstag later defined really using vector potential or directly assignment of B3 in axisymmetry
	  PLOOPBONLY(pl) pstag[pl]=pr[pl];
	}


	pr[PRAD0] = ERAD;
	pr[PRAD1] = 0 ;
	pr[PRAD2] = 0 ;    
	pr[PRAD3] = 0 ;


	pr[PRAD0] = 0 ; // so triggers failure if used
	pr[PRAD1] = 0 ;
	pr[PRAD2] = 0 ;    
	pr[PRAD3] = 0 ;

    //E, F^i in orthonormal fluid frame
    FTYPE pradffortho[NPR];
    pradffortho[PRAD0] = ERAD;
    pradffortho[PRAD1] = Fx;
    pradffortho[PRAD2] = Fy;
    pradffortho[PRAD3] = Fz;


	// Transform these fluid frame E,F^i to lab frame coordinate basis primitives
	*whichvel=VEL3;
    prad_fforlab(whichvel, whichcoord, FF2LAB, i,j,k,CENT,ptrgeomreal, pradffortho, pr, pr);

	return(0);
  }



}











#define NOFIELD -1
#define DISK1FIELD 0
#define DISK2FIELD 1
#define VERTFIELD 2
#define DISK1VERT 3
#define DISK2VERT 4
#define BLANDFORDQUAD 5
#define TOROIDALFIELD 6

#define FIELDTYPE NOFIELD


// assumes normal field in pr
// SUPERNOTE: A_i must be computed consistently across all CPUs.  So, for example, cannot use randomization of vector potential here.
int init_vpot_user(int *whichcoord, int l, SFTYPE time, int i, int j, int k, int loc, FTYPE (*prim)[NSTORE2][NSTORE3][NPR], FTYPE *V, FTYPE *A)
{
  SFTYPE rho_av, u_av,q;
  FTYPE r,th,ph;
  FTYPE vpot;

  vpot=0.0;


  // since init_vpot() is called for all i,j,k, can't use
  // non-existence values, so limit averaging:
  if((i==-N1BND)&&(j==-N2BND)){
    rho_av = MACP0A1(prim,i,j,k,RHO);
    u_av = MACP0A1(prim,i,j,k,UU);
  }
  else if(i==-N1BND){
    rho_av = AVGN_2(prim,i,j,k,RHO);
    u_av = AVGN_2(prim,i,j,k,UU);
  }
  else if(j==-N2BND){
    rho_av = AVGN_1(prim,i,j,k,RHO);
    u_av = AVGN_1(prim,i,j,k,UU);
  }
  else{ // normal cells
    rho_av = AVGN_for3(prim,i,j,k,RHO);
    u_av = AVGN_for3(prim,i,j,k,UU);
  }





  if(FIELDTYPE==TOROIDALFIELD){
    
    if(l==2){// A_\theta (MCOORD)
      
      r=V[1];
      th=V[2];


      vpot += 0;
      
    }
  }


  //////////////////////////////////
  //
  // finally assign what's returned
  //
  //////////////////////////////////
  *A = vpot;
  *whichcoord = MCOORD;



  return(0);

}



int init_vpot2field_user(SFTYPE time, FTYPE (*A)[NSTORE1+SHIFTSTORE1][NSTORE2+SHIFTSTORE2][NSTORE3+SHIFTSTORE3],FTYPE (*prim)[NSTORE2][NSTORE3][NPR], FTYPE (*pstag)[NSTORE2][NSTORE3][NPR], FTYPE (*ucons)[NSTORE2][NSTORE3][NPR], FTYPE (*Bhat)[NSTORE2][NSTORE3][NPR])
{
  int funreturn;
  int fieldfrompotential[NDIM];

  funreturn=user1_init_vpot2field_user(time, fieldfrompotential, A, prim, pstag, ucons, Bhat);
  if(funreturn!=0) return(funreturn);
 

  return(0);


}



// assumes we are fed the true densities
int normalize_densities(FTYPE (*prim)[NSTORE2][NSTORE3][NPR])
{

  return(0);
}



// assumes we are fed the true densities
int getmax_densities(FTYPE (*prim)[NSTORE2][NSTORE3][NPR],SFTYPE *rhomax, SFTYPE *umax)
{
  int funreturn;

  funreturn=user1_getmax_densities(prim,rhomax, umax);
  if(funreturn!=0) return(funreturn);
 
  return(0);
}



// get maximum b^2 and p_g
int get_maxes(FTYPE (*prim)[NSTORE2][NSTORE3][NPR], FTYPE *bsq_max, FTYPE *pg_max, FTYPE *beta_min)
{
   return(0);
}


// assumes normal field definition
int normalize_field(FTYPE (*prim)[NSTORE2][NSTORE3][NPR], FTYPE (*pstag)[NSTORE2][NSTORE3][NPR], FTYPE (*ucons)[NSTORE2][NSTORE3][NPR], FTYPE (*vpot)[NSTORE1+SHIFTSTORE1][NSTORE2+SHIFTSTORE2][NSTORE3+SHIFTSTORE3], FTYPE (*Bhat)[NSTORE2][NSTORE3][NPR])
{
  int funreturn;

 
  //  funreturn=user1_normalize_field(beta, prim, pstag, ucons, vpot, Bhat);
  //  if(funreturn!=0) return(funreturn);
 
  return(0);

}



#undef SLOWFAC


// UUMIN/RHOMIN used for atmosphere

// for each WHICHVEL possibility, set atmosphere state for any coordinate system
// which=0 : initial condition
// which=1 : evolution condition (might also include a specific angular momentum or whatever)
// which==1 assumes pr set to something locally reasonable, and we adjust to that slowly

#define TAUADJUSTATM (10.0) // timescale for boundary to adjust to using preset inflow
int set_atmosphere(int whichcond, int whichvel, struct of_geom *ptrgeom, FTYPE *pr)
{
  int funreturn;
  int atmospheretype;

  atmospheretype=1; // default
 
  funreturn=user1_set_atmosphere(atmospheretype, whichcond, whichvel, ptrgeom, pr);
  if(funreturn!=0) return(funreturn);
 
  return(0);

}



int set_density_floors(struct of_geom *ptrgeom, FTYPE *pr, FTYPE *prfloor)
{
  int funreturn;
  
  // default is for spherical flow near BH
  //  funreturn=set_density_floors_default(ptrgeom, pr, prfloor);

  int pliter,pl;
  PLOOP(pliter,pl){
    prfloor[RHO]=RHOMINLIMIT;
    prfloor[UU]=UUMINLIMIT;

    prfloor[PRAD0]=ERADLIMIT;
  }

  //  if(funreturn!=0) return(funreturn);

  return(0);
}






// Setup problem-dependent grid sectioning
int theproblem_set_enerregiondef(int forceupdate, int timeorder, int numtimeorders, long int thenstep, FTYPE thetime, int (*enerregiondef)[NDIM] )
{

  // Torus problem
  //  torus_set_enerregiondef(forceupdate, timeorder, numtimeorders, thenstep, thetime, enerregiondef);
  //jet_set_enerregiondef(forceupdate, timeorder, numtimeorders, thenstep, thetime, enerregiondef);

  if(1){
	enerregiondef[POINTDOWN][1]=0;
	enerregiondef[POINTUP][1]=totalsize[1]-1;
	enerregiondef[POINTDOWN][2]=0;
	enerregiondef[POINTUP][2]=totalsize[2]-1;
	enerregiondef[POINTDOWN][3]=0;
	enerregiondef[POINTUP][3]=totalsize[3]-1;
  }

  return(0);
}


int theproblem_set_enerregionupdate(int forceupdate, int timeorder, int numtimeorders, long int thenstep, FTYPE thetime, int *updateeverynumsteps, int *everynumsteps)
{

  ////////////////////
  //
  // Setup update period
  //
  ///////////////////

  ////////
  //
  // number of steps after which position/size of active section is updated
  //
  ////////
  if(N3==1){
	//  *updateeverynumsteps=100;
	*updateeverynumsteps=1; // update every step since otherwise flow runs into wall at outer boundary
  }
  else{
	//  *updateeverynumsteps=10;
	*updateeverynumsteps=1; // update every step since otherwise flow runs into wall at outer boundary
  }

  ////////
  //
  //number of steps after which position/size of active section is reported to file
  //
  ////////
  *everynumsteps = *updateeverynumsteps*100;

  return(0);
}


// specify MPI task rank ordering
// example user-dependent code
int theproblem_set_myid(void)
{
  int retval;
 
  // default is to do nothing
  //  retval=jet_set_myid();
  retval=0;

  // do other things?

  return(retval);

}



void adjust_fluxctstag_vpot(SFTYPE fluxtime, FTYPE (*prim)[NSTORE2][NSTORE3][NPR], int *Nvec, FTYPE (*vpot)[NSTORE1+SHIFTSTORE1][NSTORE2+SHIFTSTORE2][NSTORE3+SHIFTSTORE3])
{
  // not used
}

void adjust_fluxcttoth_vpot(SFTYPE fluxtime, FTYPE (*prim)[NSTORE2][NSTORE3][NPR], int *Nvec, FTYPE (*vpot)[NSTORE1+SHIFTSTORE1][NSTORE2+SHIFTSTORE2][NSTORE3+SHIFTSTORE3])
{
  // not used
}


void adjust_fluxcttoth_emfs(SFTYPE fluxtime, FTYPE (*prim)[NSTORE2][NSTORE3][NPR], FTYPE (*emf)[NSTORE1+SHIFTSTORE1][NSTORE2+SHIFTSTORE2][NSTORE3+SHIFTSTORE3] )
{
  // not used
}

void adjust_fluxctstag_emfs(SFTYPE fluxtime, FTYPE (*prim)[NSTORE2][NSTORE3][NPR], int *Nvec, FTYPE (*fluxvec[NDIM])[NSTORE2][NSTORE3][NPR])
{
  // not used
}


//**********************************************************************
//******* user opacities ****************************************************
//**********************************************************************

// \kappa is optical depth per unit length per unit rest-mass energy density

//absorption
FTYPE calc_kappa_user(FTYPE rho, FTYPE T,FTYPE x,FTYPE y,FTYPE z)
{
  return(KAPPAUSER(rho,T));
}

//scattering
FTYPE calc_kappaes_user(FTYPE rho, FTYPE T,FTYPE x,FTYPE y,FTYPE z)
{  
  return(KAPPAESUSER(rho,T));

}

