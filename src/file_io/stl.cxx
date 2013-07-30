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


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#if HAVE_STRING_H
#include <string.h>
#else
//alternative codes here
#endif

#include <fstream>
#include <vector>
#include <iomanip>
#include "common/tt.h"
#include "polygons/Triangle.h"
#include "file_io/TriMeshIO.h"
#include "file_io/stl.h"

namespace PolylibNS {

using namespace std;

// #define SCIENTIFIC_OUT		0
// #define STL_HEAD			80		// header size for STL binary
// #define STL_BUFF_LEN		256
// #define TT_OTHER_ENDIAN		1
// #define TT_LITTLE_ENDIAN	2
// #define TT_BIG_ENDIAN		3

 // static void	tt_invert_byte_order(void* _mem, int size, int n);
 // static int	tt_check_machine_endian();
 // static void	tt_read(istream& is, void* _data, int size, int n, int inv);
 // static void	tt_write(ostream& os, const void* _data, int size, int n, int inv);

// //////////////////////////////////////////////////////////////////////////////
// POLYLIB_STAT stl_a_load(
// 	vector<PrivateTriangle*>	*tri_list, 
// 	string 						fname,
// 	int							*total,
// 	float						scale
// ) {

// 	ifstream is(fname.c_str());
// 	if (is.fail()) {
// 		PL_ERROSH << "[ERROR]stl:stl_a_load():Can't open " << fname << std::endl;
// 		return PLSTAT_STL_IO_ERROR;
// 	}

// 	int n_tri = *total;		// 通番の初期値をセット
// 	int n_vtx = 0;

// 	string token;
// 	Vec3f nml;
// 	Vec3f vtx[3];
// 	while (is >> token && !is.eof()) {
// 		if (token == "solid") {
// 			string s;
// 			is >> s;
// 		}
// 		else if (token == "facet") {
// 			n_vtx = 0;

// 			string s;
// 			is >> s;
// 			is >> nml;
// 			nml.normalize();
// 		}
// 		else if (token == "vertex") {
// 			Vec3f v;
// 			is >> v;
// 			if (n_vtx < 3) {
// 				vtx[n_vtx] = v * scale;
// 			}
// 			n_vtx++;
// 		}
// 		else if (token == "outer") {
// 			string s;
// 			is >> s;
// 		}
// 		else if (token == "endloop") {
// 		}
// 		else if (token == "endfacet") {
// 			if (n_vtx == 3) {
// 				PrivateTriangle *tri = new PrivateTriangle(vtx, nml, n_tri);
// 				tri_list->push_back(tri);
// 				n_tri++;
// 			}
// 		}
// 		else if (token == "endsolid") {
// 			string s;
// 			is >> s;
// 		}
// 	}

// 	if (!is.eof() && is.fail()) {
// 		PL_ERROSH << "[ERROR]stl:stl_a_load():Error in loading: " << fname << std::endl;
// 		return PLSTAT_STL_IO_ERROR;
// 	}

// 	*total = n_tri;		// 更新した通番をセット
// #ifdef DEBUG
// PL_DBGOSH <<  "stl_a_load total=" << *total << std::endl;
// #endif
// 	return PLSTAT_OK;
// }

// //////////////////////////////////////////////////////////////////////////////
// POLYLIB_STAT stl_a_save(
// 	vector<PrivateTriangle*>	*tri_list, 
// 	string 						fname
// ) {
// 	ofstream os(fname.c_str());

// 	if (os.fail()) {
// 		PL_ERROSH << "[ERROR]stl:stl_a_save():Can't open " << fname << std::endl;
// 		return PLSTAT_STL_IO_ERROR;
// 	}

// 	os << "solid " << "model1" << std::endl;

// 	vector<PrivateTriangle*>::iterator itr;
// 	for (itr = tri_list->begin(); itr != tri_list->end(); itr++) {
// #if SCIENTIFIC_OUT
// 		os	<< "  facet " << "normal " << setprecision(6) << scientific 
// 			<< (*itr)->get_normal() << endl;
// #else
// 		os	<< "  facet " << "normal " << setprecision(6) 
// 			<< (*itr)->get_normal() << endl;
// #endif
// 		os << "	outer " << "loop" << endl;
// 		for (int j = 0; j < 3; j++) {
// #if SCIENTIFIC_OUT
// 			os	<< "	  vertex " << setprecision(6) << scientific 
// 				<< (*itr)->get_vertex()[j] << endl;
// #else
// 			os	<< "	  vertex " << setprecision(6) 
// 				<< (*itr)->get_vertex()[j] << endl;
// #endif
// 		}
// 		os << "	endloop" << endl;
// 		os << "  endfacet" << endl;
// 	}
// 	os << "endsolid " << "model1" << endl;

// 	if (!os.eof() && os.fail()) {
// 		PL_ERROSH << "[ERROR]stl:stl_a_save():Error in saving: " << fname << endl;
// 		return PLSTAT_STL_IO_ERROR;
// 	}

// 	return PLSTAT_OK;
// }

// //////////////////////////////////////////////////////////////////////////////
// POLYLIB_STAT stl_b_load(
// 	vector<PrivateTriangle*>	*tri_list, 
// 	string 						fname,
// 	int							*total,
// 	float						scale
// ) {
// 	ifstream ifs(fname.c_str(), ios::in | ios::binary);
// 	if (ifs.fail()) {
// 		PL_ERROSH << "[ERROR]stl:stl_b_load():Can't open " << fname << endl;
// 		return PLSTAT_STL_IO_ERROR;
// 	}

// 	int inv = tt_check_machine_endian() == TT_LITTLE_ENDIAN ? 0 : 1;

// 	int		n_tri = *total;		// 通番の初期値をセット
// 	uint	element = 0;

// 	char buf[STL_HEAD];
// 	for (int i = 0; i < STL_HEAD; i++) buf[i] = 0;
// 	tt_read(ifs, buf, sizeof(char), STL_HEAD, inv);
// 	tt_read(ifs, &element, sizeof(uint), 1, inv);

// 	ushort padding = 0;
// 	for (uint i = 0; i < element; i++) {
// 		// one plane normal
// 		float nml[3];
// 		tt_read(ifs, nml, sizeof(float), 3, inv);
// 		Vec3f normal(nml);

// 		// three vertices
// 		Vec3f vertex[3];
// 		for (int j = 0; j < 3; j++) {
// 			float vtx[3];
// 			tt_read(ifs, vtx, sizeof(float), 3, inv);
// 			vtx[0] = vtx[0] * scale;
// 			vtx[1] = vtx[1] * scale;
// 			vtx[2] = vtx[2] * scale;
// 			Vec3f _vertex(vtx);
// 			vertex[j] = _vertex;
// 		}

// 		// ２バイト予備領域
// 		tt_read(ifs, &padding, sizeof(ushort), 1, inv);

// 		PrivateTriangle *tri = new PrivateTriangle(vertex, normal, n_tri);
// 		// ２バイト予備領域をユーザ定義IDとして利用(Polylib-2.1より)
// 		tri->set_exid( (int)padding );
// 		tri_list->push_back(tri);
// 		n_tri++;

// 	}

// 	if (!ifs.eof() && ifs.fail()) {
// 		PL_ERROSH << "[ERROR]stl:stl_b_load():Error in loading: " << fname << endl;
// 		return PLSTAT_STL_IO_ERROR;
// 	}

// 	*total = n_tri;		// 更新した通番をセット
// 	return PLSTAT_OK;
// }

// //////////////////////////////////////////////////////////////////////////////
// POLYLIB_STAT stl_b_save(
// 	vector<PrivateTriangle*>	*tri_list, 
// 	string 						fname
// ) {
// 	ofstream ofs(fname.c_str(), ios::out | ios::binary);
// 	if (ofs.fail()) {
// 		PL_ERROSH << "[ERROR]stl:stl_b_save():Can't open " << fname << endl;
// 		return PLSTAT_STL_IO_ERROR;
// 	}

// 	int inv = tt_check_machine_endian() == TT_LITTLE_ENDIAN ? 0 : 1;

// 	uint element = tri_list->size();

// 	char buf[STL_HEAD];
// 	for (int i = 0; i < STL_HEAD; i++) buf[i] = 0;
// 	strcpy(buf, "default");
// 	tt_write(ofs, buf, 1, STL_HEAD, inv);
// 	tt_write(ofs, &element, sizeof(uint), 1, inv);

// 	vector<PrivateTriangle*>::iterator itr = tri_list->begin();
// 	for (uint m = 0; m < element; m++,itr++) {
// 		// one plane normal
// 		tt_write(ofs, (*itr)->get_normal().ptr(), sizeof(float), 3, inv);

// 		// three vertices
// 		tt_write(ofs, (*itr)->get_vertex()[0].ptr(), sizeof(float), 3, inv);
// 		tt_write(ofs, (*itr)->get_vertex()[1].ptr(), sizeof(float), 3, inv);
// 		tt_write(ofs, (*itr)->get_vertex()[2].ptr(), sizeof(float), 3, inv);

// 		// ２バイト予備領域にユーザ定義IDを記録(Polylib-2.1より)
// 		int exid = (*itr)->get_exid();
// 		tt_write(ofs, &exid, sizeof(ushort), 1, inv);
// 	}

// 	if (!ofs.eof() && ofs.fail()) {
// 		PL_ERROSH << "[ERROR]stl:stl_b_load():Error in saving: " << fname << endl;
// 		return PLSTAT_STL_IO_ERROR;
// 	}

// 	return PLSTAT_OK;
// }

//////////////////////////////////////////////////////////////////////////////
bool is_stl_a(string path)
{
	const char  dcs[] = "\n\r";
	char		buff[STL_BUFF_LEN];
	size_t		i = 0;
	char		c;

	ifstream ifs(path.c_str());
	if (!ifs)				   return false;

	// ファイル内容の一部を読み込み
	memset(buff, 0, STL_BUFF_LEN);
	while (i < STL_BUFF_LEN - 1 && ifs.get(c) && !strchr(dcs, c)) {
		buff[i++] = c;
	}
	while (ifs.get(c) && strchr(dcs, c))
		;
	if (ifs.good()) ifs.putback(c);

	if (ifs.eof())				return false;

	// ASCII判定
	if (strstr(buff, "solid"))	return true;
	else						return false;
}

//////////////////////////////////////////////////////////////////////////////
char *stl_get_fname(const string path) {
	static char	fname[256];

	int pos = path.find_last_of("."); // 拡張子の手前までの位置
	memset(fname, 0, sizeof(fname));
	path.copy(fname, pos, 0);
	return fname;
}

//////////////////////////////////////////////////////////////////////////////
char *stl_get_ext(const string path) {
	static char ext[16];

	int pos = path.find_last_of("."); // 拡張子の手前までの位置
	memset(ext, 0, sizeof(ext));
	path.copy(ext, path.length() - pos, pos + 1);
	return ext;
}






} //namespace PolylibNS
