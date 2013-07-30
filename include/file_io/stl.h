/* -- Mode: c++ --*/
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

#ifndef stl_h
#define stl_h
#include <fstream>
#include <vector>
#include <iomanip>
#include "common/PolylibCommon.h"


#if HAVE_STRING_H 
#include <string.h>
#else
// write some code here
#endif


#define SCIENTIFIC_OUT		0
#define STL_HEAD		80		// header size for STL binary
#define STL_BUFF_LEN		256
#define TT_OTHER_ENDIAN		1
#define TT_LITTLE_ENDIAN	2
#define TT_BIG_ENDIAN		3

namespace PolylibNS {


// tempolary define uint to unsigned int
  typedef unsigned int uint;
  typedef unsigned short ushort;


static void	tt_invert_byte_order(void* _mem, int size, int n);
static int	tt_check_machine_endian();
static void	tt_read(std::istream& is, void* _data, int size, int n, int inv);
static void	tt_write(std::ostream& os, const void* _data, int size, int n, int inv);



///
/// ASCIIモードのSTLファイルを読み込み、tri_listに三角形ポリゴン情報を設定する。
///
///  @param[in,out] tri_list	三角形ポリゴンリストの領域。
///  @param[in]		fname		STLファイル名。
///  @param[in,out] total		ポリゴンIDの通番。
///  @return	POLYLIB_STATで定義される値が返る。
///
template <typename T>
POLYLIB_STAT stl_a_load(
	std::vector<PrivateTriangle<T>*>	*tri_list, 
	std::string 					fname,
	int								*total,
	T							scale=1.0
);

///
/// 三角形ポリゴン情報をASCIIモードでSTLファイルに書き出す。
///
///  @param[in] tri_list	三角形ポリゴン情報。
///  @param[in] fname		STLファイル名。
///  @return	POLYLIB_STATで定義される値が返る。
///
template <typename T>
POLYLIB_STAT stl_a_save(
			std::vector<PrivateTriangle<T>*>	*tri_list, 
	std::string 					fname
);

///
/// バイナリモードのSTLファイルを読み込み、tri_listに三角形ポリゴン情報を設定
/// する。
///
///  @param[in,out] tri_list	三角形ポリゴンリストの領域。
///  @param[in]		fname		ファイル名。
///  @param[in,out] total		ポリゴンIDの通番。
///  @return	POLYLIB_STATで定義される値が返る。
///
template <typename T>
POLYLIB_STAT stl_b_load(
	std::vector<PrivateTriangle<T>*>	*tri_list, 
	std::string						fname,
	int								*total,
	T							scale=1.0
);

///
/// 三角形ポリゴン情報をバイナリモードでSTLファイルに書き出す。
///
///  @param[in] tri_list	三角形ポリゴン情報。
///  @param[in] fname		STLファイル名。
///  @return	POLYLIB_STATで定義される値が返る。
///
template <typename T>
POLYLIB_STAT stl_b_save(
			std::vector<PrivateTriangle<T>*> *tri_list, 
			std::string fname
);

///
/// STLファイルを読み込みバイナリかアスキーかを判定する。
///
///  @param[in] STLファイルのフルパス名。
///  @return	true:アスキー形式 / false:バイナリ形式。
/// 
bool is_stl_a(
	std::string		path
);

///
/// STLファイル名から名称(拡張子を除いた部分)を取得する。
///
///  @param[in] STLファイルのフルパス名。
///  @return	拡張子を除いた名称。
///  @attention	戻り値のchar *は解放不要。
///
char *stl_get_fname(
	std::string		path
);

///
/// STLファイル名から拡張子のみを取得する。
///
///  @param[in] STLファイルのフルパス名。
///  @return	拡張子。
///  @attention	戻り値のchar *は解放不要。
///
char *stl_get_ext(
	std::string		path
);


//////////////////////////////////////////////////////////////////////////////
  template <typename T>
POLYLIB_STAT stl_a_load(
			std::vector<PrivateTriangle<T>*>	*tri_list, 
			std::string 			fname,
			int				*total,
			T		scale
) {

    std::ifstream is(fname.c_str());
	if (is.fail()) {
		PL_ERROSH << "[ERROR]stl:stl_a_load():Can't open " << fname << std::endl;
		return PLSTAT_STL_IO_ERROR;
	}

	int n_tri = *total;		// 通番の初期値をセット
	int n_vtx = 0;

	std::string token;
	Vec3<T> nml;
	Vec3<T> vtx[3];
	while (is >> token && !is.eof()) {
		if (token == "solid") {
		  std::string s;
			is >> s;
		}
		else if (token == "facet") {
			n_vtx = 0;

			std::string s;
			is >> s;
			is >> nml;
			nml.normalize();
		}
		else if (token == "vertex") {
			Vec3<T> v;
			is >> v;
			if (n_vtx < 3) {
				vtx[n_vtx] = v * scale;
			}
			n_vtx++;
		}
		else if (token == "outer") {
			std::string s;
			is >> s;
		}
		else if (token == "endloop") {
		}
		else if (token == "endfacet") {
			if (n_vtx == 3) {
				PrivateTriangle<T> *tri = new PrivateTriangle<T>(vtx, nml, n_tri);
				tri_list->push_back(tri);
				n_tri++;
			}
		}
		else if (token == "endsolid") {
			std::string s;
			is >> s;
		}
	}

	if (!is.eof() && is.fail()) {
		PL_ERROSH << "[ERROR]stl:stl_a_load():Error in loading: " << fname << std::endl;
		return PLSTAT_STL_IO_ERROR;
	}

	*total = n_tri;		// 更新した通番をセット
#ifdef DEBUG
PL_DBGOSH <<  "stl_a_load total=" << *total << std::endl;
#endif
	return PLSTAT_OK;
}

//////////////////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT stl_a_save(
	std::vector<PrivateTriangle<T>*>	*tri_list, 
	std::string 						fname
) {
    std::ofstream os(fname.c_str());

	if (os.fail()) {
		PL_ERROSH << "[ERROR]stl:stl_a_save():Can't open " << fname << std::endl;
		return PLSTAT_STL_IO_ERROR;
	}

	os << "solid " << "model1" << std::endl;

	typename std::vector<PrivateTriangle<T>*>::iterator itr;
	for (itr = tri_list->begin(); itr != tri_list->end(); itr++) {
#if SCIENTIFIC_OUT
	  os	<< "  facet " << "normal " << std::setprecision(6) << scientific 
			<< (*itr)->get_normal() << std::endl;
#else
	  os	<< "  facet " << "normal " << std::setprecision(6) 
			<< (*itr)->get_normal() << std::endl;
#endif
		os << "	outer " << "loop" << std::endl;
		for (int j = 0; j < 3; j++) {
#if SCIENTIFIC_OUT
		  os	<< "	  vertex " << std::setprecision(6) << scientific 
				<< (*itr)->get_vertex()[j] << std::endl;
#else
		  os	<< "	  vertex " << std::setprecision(6) 
				<< (*itr)->get_vertex()[j] << std::endl;
#endif
		}
		os << "	endloop" << std::endl;
		os << "  endfacet" << std::endl;
	}
	os << "endsolid " << "model1" << std::endl;

	if (!os.eof() && os.fail()) {
		PL_ERROSH << "[ERROR]stl:stl_a_save():Error in saving: " << fname << std::endl;
		return PLSTAT_STL_IO_ERROR;
	}

	return PLSTAT_OK;
}

//////////////////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT stl_b_load(
	std::vector<PrivateTriangle<T>*>	*tri_list, 
	std::string 						fname,
	int							*total,
	T						scale
) {
  std::ifstream ifs(fname.c_str(), std::ios::in | std::ios::binary);
	if (ifs.fail()) {
		PL_ERROSH << "[ERROR]stl:stl_b_load():Can't open " << fname << std::endl;
		return PLSTAT_STL_IO_ERROR;
	}

	int inv = tt_check_machine_endian() == TT_LITTLE_ENDIAN ? 0 : 1;

	int		n_tri = *total;		// 通番の初期値をセット
	uint	element = 0;

	char buf[STL_HEAD];
	for (int i = 0; i < STL_HEAD; i++) buf[i] = 0;
	tt_read(ifs, buf, sizeof(char), STL_HEAD, inv);
	tt_read(ifs, &element, sizeof(uint), 1, inv);

	ushort padding = 0;
	for (uint i = 0; i < element; i++) {
		// one plane normal
		T nml[3];
		tt_read(ifs, nml, sizeof(T), 3, inv);
		Vec3<T> normal(nml);

		// three vertices
		Vec3<T> vertex[3];
		for (int j = 0; j < 3; j++) {
			T vtx[3];
			tt_read(ifs, vtx, sizeof(T), 3, inv);
			vtx[0] = vtx[0] * scale;
			vtx[1] = vtx[1] * scale;
			vtx[2] = vtx[2] * scale;
			Vec3<T> _vertex(vtx);
			vertex[j] = _vertex;
		}

		// ２バイト予備領域
		tt_read(ifs, &padding, sizeof(ushort), 1, inv);

		PrivateTriangle<T> *tri = new PrivateTriangle<T>(vertex, normal, n_tri);
		// ２バイト予備領域をユーザ定義IDとして利用(Polylib-2.1より)
		tri->set_exid( (int)padding );
		tri_list->push_back(tri);
		n_tri++;

	}

	if (!ifs.eof() && ifs.fail()) {
		PL_ERROSH << "[ERROR]stl:stl_b_load():Error in loading: " << fname << std::endl;
		return PLSTAT_STL_IO_ERROR;
	}

	*total = n_tri;		// 更新した通番をセット
	return PLSTAT_OK;
}

//////////////////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT stl_b_save(
			std::vector<PrivateTriangle<T>*>	*tri_list, 
	std::string 						fname
) {
  std::ofstream ofs(fname.c_str(), std::ios::out | std::ios::binary);
  if (ofs.fail()) {


		PL_ERROSH << "[ERROR]stl:stl_b_save():Can't open " << fname << std::endl;
		return PLSTAT_STL_IO_ERROR;
	}

	int inv = tt_check_machine_endian() == TT_LITTLE_ENDIAN ? 0 : 1;

	uint element = tri_list->size();

	char buf[STL_HEAD];
	for (int i = 0; i < STL_HEAD; i++) buf[i] = 0;
	strcpy(buf, "default");
	tt_write(ofs, buf, 1, STL_HEAD, inv);
	tt_write(ofs, &element, sizeof(uint), 1, inv);

	typename std::vector<PrivateTriangle<T>*>::iterator itr = tri_list->begin();
	for (uint m = 0; m < element; m++,itr++) {
		// one plane normal
		tt_write(ofs, (*itr)->get_normal().ptr(), sizeof(T), 3, inv);

		// three vertices
		tt_write(ofs, (*itr)->get_vertex()[0].ptr(), sizeof(T), 3, inv);
		tt_write(ofs, (*itr)->get_vertex()[1].ptr(), sizeof(T), 3, inv);
		tt_write(ofs, (*itr)->get_vertex()[2].ptr(), sizeof(T), 3, inv);

		// ２バイト予備領域にユーザ定義IDを記録(Polylib-2.1より)
		int exid = (*itr)->get_exid();
		tt_write(ofs, &exid, sizeof(ushort), 1, inv);
	}

	if (!ofs.eof() && ofs.fail()) {
		PL_ERROSH << "[ERROR]stl:stl_b_load():Error in saving: " << fname << std::endl;
		return PLSTAT_STL_IO_ERROR;
	}

	return PLSTAT_OK;
}


//=======================================================================
// static関数
//=======================================================================
//////////////////////////////////////////////////////////////////////////////
//static void tt_invert_byte_order(void* _mem, int size, int n)
void tt_invert_byte_order(void* _mem, int size, int n)
{
	char* mem = (char*)_mem;
	char c;
	int i;

	if (size == 1)		return;

	while (n) {
		for (i = 0; i < size/2; i++) {
			c = mem[i];
			mem[i] = mem[size-1-i];
			mem[size-1-i] = c;
		}
		mem += size;
		n--;
	}
}

//////////////////////////////////////////////////////////////////////////////
//static int tt_check_machine_endian()
int tt_check_machine_endian()
{
	int v = 1;
	char* p = (char*)&v;

	if (p[0])					return TT_LITTLE_ENDIAN;
	else if (p[sizeof(int)-1])	return TT_BIG_ENDIAN;
	else						return TT_OTHER_ENDIAN;
}

//////////////////////////////////////////////////////////////////////////////
//static void tt_read(istream& is, void* _data, int size, int n, int inv)
  void tt_read(std::istream& is, void* _data, int size, int n, int inv)
{
	char* data = (char*)_data;

	is.read(data, size * n);

	if (inv) {
		tt_invert_byte_order(data, size, n);
	}
}

//////////////////////////////////////////////////////////////////////////////
//static void tt_write(ostream& os, const void* _data, int size, int n, int inv)
  void tt_write(std::ostream& os, const void* _data, int size, int n, int inv)
{
	const char* data = (const char*)_data;
	char* tmp = 0;

	if (inv) {
		int sz = size * n;
		tmp = new char[sz];
		for (int i=0; i<sz; i++) tmp[i] = data[i];
		tt_invert_byte_order(tmp, size, n);
		data = tmp;
	}

	os.write(data, size * n);

	if (inv) {
		delete [] tmp;
	}
}


} //namespace PolylibNS

#endif  // stl_h

