/*
 * Polylib - Polygon Management Library
 *
 * Copyright (c) 2010-2011 VCAD System Research Program, RIKEN.
 * All rights reserved.
 *
 * Copyright (c) 2012-2013 Advanced Institute for Computational Science, RIKEN.
 * All rights reserved.
 *
 */

#include <iostream>
#include "mpi.h"
#include "Polylib.h"
#include "MPIPolylib.h"
#include "CarGroup.h"
//#include "BladeGroup.h"
#include "MyGroupFactory.h"

using namespace std;
using namespace PolylibNS;

//#define PL_REAL double
#define PL_REAL float

template <typename T>
struct MyParallelInfo {
  T bpos[3]; //基準座標
  unsigned bbsize[3]; //number of voxel 計算領域
  unsigned gcsize[3]; //number of guidecell voxel
  T dx[3]; //size of voxel
};

static MyParallelInfo<PL_REAL> myParaInfos[4] = {
  {{-1100, -1800,-1800,}, {18,18,18,}, {1, 1,1,}, {100,100,100} },
  {{-1100,     0,-1800,}, {18,18,18,}, {1, 1,1,}, {100,100,100} },
  {{-1100, -1800,    0,}, {18,18,18,}, {1, 1,1,}, {100,100,100} },
  {{-1100,     0,    0,}, {18,18,18,}, {1, 1,1,}, {100,100,100} }
};

int main(int argc, char** argv ){
  int rank;
  unsigned int step;
  POLYLIB_STAT stat;
  PolylibMoveParams params;

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  cout << "Starting program on rank:"<<rank<<endl;

  MPIPolylib<PL_REAL>* p_polylib = MPIPolylib<PL_REAL>::get_instance();

  p_polylib->set_factory(new MyGroupFactory() );

  stat = p_polylib->init_parallel_info(MPI_COMM_WORLD,
				       myParaInfos[rank].bpos,
				       myParaInfos[rank].bbsize,
				       myParaInfos[rank].gcsize,
				       myParaInfos[rank].dx);

  if(stat !=PLSTAT_OK) return -1;

  stat=p_polylib->load_rank0("./polylib_config2.tpp");
  if(stat !=PLSTAT_OK) return -1;
  cout << "data loading finished on rank:"<<rank<<endl;
  p_polylib->show_group_hierarchy();

  string fname="";
  string stl="stl_a";
  string extend="";
  //  ID_FORMAT id_format=ID_BIN;
  ID_FORMAT id_format=ID_ASCII;
  //  p_polylib->save_rank0(&fname,stl,extend);
  p_polylib->save_parallel(&fname,stl,extend,id_format);

  MPI_Finalize();
  
  return 0;


}
