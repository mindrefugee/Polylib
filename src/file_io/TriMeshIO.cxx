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
#include <string>
#include "polygons/Triangle.h"
#include "file_io/TriMeshIO.h"
#include "file_io/stl.h"

namespace PolylibNS {

using namespace std;

const string TriMeshIO::FMT_STL_A  = "stl_a";
const string TriMeshIO::FMT_STL_AA = "stl_aa";
const string TriMeshIO::FMT_STL_B  = "stl_b";
const string TriMeshIO::FMT_STL_BB = "stl_bb";
const string TriMeshIO::FMT_OBJ_A  = "obj_a";
const string TriMeshIO::FMT_OBJ_AA = "obj_aa";
const string TriMeshIO::FMT_OBJ_B  = "obj_b";
const string TriMeshIO::FMT_OBJ_BB = "obj_bb";
const string TriMeshIO::DEFAULT_FMT = TriMeshIO::FMT_STL_B;

/************************************************************************
 *
 * TriMeshIOクラス
 *
 ***********************************************************************/
// // public /////////////////////////////////////////////////////////////////////
// POLYLIB_STAT TriMeshIO::load(
// 	vector<PrivateTriangle*>	*tri_list, 
// 	const map<string, string>	&fmap,
// 	float scale
// ) {
// 	map<string, string>::const_iterator	it;
// 	int									total;
// 	POLYLIB_STAT						ret = PLSTAT_OK;

// 	if (tri_list == NULL) {
// 		PL_ERROSH << "[ERROR]TriMeshIO::load():tri_list is NULL." << endl;
// 		return PLSTAT_NG;
// 	}

// 	total = 0;	// 通算番号に初期値をセット
// 	for (it = fmap.begin(); it != fmap.end(); it++) {
// 		string fname	= it->first;
// 		string fmt		= it->second;

// 		if (fmt == "") {
// 			PL_ERROSH << "[ERROR]:TTriMeshIO::load():Unknown stl format." << endl;
// 			ret = PLSTAT_NG;
// 		}
// 		else if (fmt == FMT_STL_A || fmt == FMT_STL_AA) {
// 			ret = stl_a_load(tri_list, fname, &total, scale);
// 		}
// 		else if (fmt == FMT_STL_B || fmt == FMT_STL_BB) {
// 			ret = stl_b_load(tri_list, fname, &total, scale);
// 		}

// 		// 一ファイルでも読み込みに失敗したら戻る
// 		if (ret != PLSTAT_OK)		return ret;
// 	}

// 	return ret;
// }

// // public /////////////////////////////////////////////////////////////////////
// POLYLIB_STAT TriMeshIO::save(
// 	vector<PrivateTriangle*>	*tri_list, 
// 	string						fname, 
// 	string						fmt
// ) {
// #ifdef DEBUG
//   //  PL_DBGOS<<__FUNCTION__ << " saving stl file..."<<endl;
// #endif
// 	if (tri_list == NULL) {
// 		PL_ERROSH << "[ERROR]:TriMeshIO::save():tri_list is NULL." << endl;
// 		return PLSTAT_NG;
// 	}

// 	if (fmt == FMT_STL_A || fmt == FMT_STL_AA) {
// 		return stl_a_save(tri_list, fname);
// 	}
// 	else if (fmt == FMT_STL_B || fmt == FMT_STL_BB) {
// 		return stl_b_save(tri_list, fname);
// 	}
// 	else{
// 		return PLSTAT_UNKNOWN_STL_FORMAT;
// 	}
// }

// public /////////////////////////////////////////////////////////////////////
string TriMeshIO::input_file_format(
	const string &filename
)
{
  

	//書式の決定
	char	*ext = stl_get_ext(filename);
	//#define DEBUG
#ifdef DEBUG
	PL_DBGOS << __func__ << " file " << filename <<" ext " << ext <<std::endl;
#endif
	//#undef DEBUG


	if (!strcmp(ext, "stla") || !strcmp(ext, "STLA")) {
		 return FMT_STL_A;
	}
	else if (!strcmp(ext, "stlb") || !strcmp(ext, "STLB")) {
		 return FMT_STL_B;
	}

	else if (!strcmp(ext, "obja") || !strcmp(ext, "OBJA")) {
		 return FMT_STL_A;
	}
	else if (!strcmp(ext, "objb") || !strcmp(ext, "OBJB")) {
		 return FMT_STL_B;
	}

	else if (!strcmp(ext, "stl") || !strcmp(ext, "STL")) {
		//読み込んで書式を判定する
		if(is_stl_a(filename) == true)	return FMT_STL_A;
		else	return FMT_STL_B;
	}
	else if (!strcmp(ext, "obj") || !strcmp(ext, "OBJ")) {
		if(is_obj_a(filename) == true)	  return FMT_OBJ_A;
		else	return FMT_OBJ_B;

	}



	return "";
}

} //namespace PolylibNS
