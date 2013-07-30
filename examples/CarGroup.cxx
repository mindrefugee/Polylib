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


#include <string.h>
using namespace std;

#include "CarGroup.h"

POLYLIB_STAT CarGroup::build_group_tree(Polylib<PL_REAL>* polylib,
					PolygonGroup<PL_REAL>* parent,
					TextParser* tp)
{
  TextParserError status = TP_NO_ERROR;
  m_velocity = 0;
  vector<string> leaves;
  tp->getLabels(leaves);
  vector<string>::iterator leaf_iter=find(leaves.begin(),leaves.end(),"velocity");

  if(leaf_iter!=leaves.end()){
    string value;
    status=tp->getValue(*leaf_iter,value);
    if(status!=TP_NO_ERROR){
      tp->TextParserErrorHandler(status," can not read velocity.");
      return PLSTAT_CONFIG_ERROR;
    }else{
      int error;
      m_velocity = tp->convertDouble(value,&error);
    }
  }

  cout << whoami() <<" "<<__FUNCTION__<< " "<<  m_velocity << endl;

  POLYLIB_STAT stat = PolygonGroup<PL_REAL>::build_group_tree(polylib,parent,tp);

  return stat;
}

POLYLIB_STAT CarGroup::move(PolylibMoveParams& params){

  //   write code here


  return PLSTAT_OK;
}

POLYLIB_STAT CarGroup::mk_param_tag(
				    TextParser* tp,
				    string rank_no,
				    string extend,
				    string format
) {

  cout << "CarGroup::"<<__FUNCTION__ << endl;
  stringstream ss;
  POLYLIB_STAT	stat;
  //goto root
  tp->changeNode("/");
  tp->changeNode(acq_fullpath());
  //this is test. change m_velocity forcely
  m_velocity = -500.;
  ss<<m_velocity;
  string value;
  ss>>value;
  tp->updateValue("velocity",value);

  return PolygonGroup<PL_REAL>::mk_param_tag(tp,rank_no,extend,format);
}
