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

#ifndef CARGROUP_H
#define CARGROUP_H

#include "common/BBox.h"
#include "polygons/VTree.h"
#include "polygons/Polygons.h"
#include "groups/PolygonGroup.h"
#include <vector>

#include "Polylib.h"
using namespace std;
using namespace PolylibNS;

#define PL_REAL double

class CarGroup:public PolygonGroup<PL_REAL>{

 public:
  static string get_class_name() {return "CarGroup";}
  virtual string whoami(){return get_class_name();}

 protected:
  double m_velocity;
  POLYLIB_STAT move(PolylibMoveParams& params);
  POLYLIB_STAT build_group_tree(Polylib<PL_REAL>* polylib,PolygonGroup<PL_REAL>* parent,TextParser* tp);


  POLYLIB_STAT mk_param_tag(
				      TextParser* tp,
				      string rank_no,
				      string extend,
				      string format);

};
#endif //CARGROUP_H
