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

#ifndef polylib_trimesh_h
#define polylib_trimesh_h

#include <vector>
#include <algorithm>
#include "file_io/TriMeshIO.h"


#define M_MAX_ELEMENTS 15	/// VTreeのノードが持つ最大要素数

namespace PolylibNS {

template <typename T> class Polygons;
template <typename T> class VTree;
template <typename T> class PrivateTriangle;

////////////////////////////////////////////////////////////////////////////
///
/// クラス:TriMesh
/// 三角形ポリゴン集合を管理するクラス（KD木用に特化したクラス)。
///
////////////////////////////////////////////////////////////////////////////
template <typename T>
class TriMesh : public Polygons<T> {
public:
	///
	/// コンストラクタ。
	///
	TriMesh();

	///
	/// デストラクタ。
	///
	~TriMesh();

	///
	/// TriMeshクラスで管理する三角形ポリゴンリストを初期化し、引数で与えら
	/// れる三角形ポリゴンリストを設定する。
	/// 三角形ポリゴン用のメモリ領域は、Polylib内で新たに確保される。
	///
	///  @param[in] trias 設定する三角形ポリゴンリスト。
	///
	void init(
		const std::vector<PrivateTriangle<T>*>	*trias
	);

	///
	/// 三角形ポリゴンリストに引数で与えられる三角形の複製を追加する。
	///
	/// @param[in] trias	設定する三角形ポリゴンリスト。
	/// @attention m_idが重複するインスタンスは追加されない。
	/// @attention KD木の再構築は行わない。
	///
	void add(
		const std::vector<PrivateTriangle<T>*>  *trias
	);

	///
	/// ファイルからデータの初期化。
	///
	///  @param[in] fmap	ファイル名、ファイルフォーマット。
	///  @return PLSTAT_OK=成功/false=失敗
	///
	POLYLIB_STAT import(
		const std::map<std::string, std::string> fmap,
		T scale = 1.0
	);

	///
	/// Polygonsクラスに含まれる全ポリゴン情報からKD木を作成する。
	///
	///  @return	POLYLIB_STATで定義される値が返る。
	///
	POLYLIB_STAT build();

	///
	/// TriMeshクラスが管理している三角形ポリゴン数を返す。
	///
	int triangles_num();

	///
	/// KD木探索により、指定矩形領域に含まれる三角形ポリゴンを抽出する。
	///
	///  @param[in] bbox	検索範囲を示す矩形領域。
	///  @param[in] every	true:3頂点が全て検索領域に含まれるものを抽出。
	///						false:1頂点でも検索領域に含まれるものを抽出。
	///  @return	抽出したポリゴンリストのポインタ。
	///	 @attention	三角形ポリゴンのメモリ領域は新たにPolylib内で確保される。
	///  @attention MPIPolylib内での利用が目的なので、ユーザは使用しないこと。
	///  @attention	オーバーロードメソッドあり。
	///
	const std::vector<PrivateTriangle<T>*>* search(
		BBox<T>	*bbox, 
		bool	every
	) const;

	///
	/// KD木探索により、指定矩形領域に含まれるポリゴンを抽出する。
	///
	///  @param[in]		bbox		検索範囲を示す矩形領域
	///  @param[in]		every		true:3頂点が全て検索領域に含まれるものを抽出。
	///								false:1頂点でも検索領域に含まれるものを抽出。
	///  @param[in,out] tri_list	抽出した三角形ポリゴンリストへのポインタ。
	///  @return	POLYLIB_STATで定義される値が返る。
	///  @attention	tri_listで戻される三角形ポリゴンのポインタは、Polylib内で
	///				保持されるアドレス値なので、ユーザはdeleteしないで下さい。
	///  @attention	オーバーロードメソッドあり。
	///
	POLYLIB_STAT search(
		BBox<T>							*bbox,
		bool							every,
		std::vector<PrivateTriangle<T>*>	*tri_list
	) const;

	///
	/// 線形探索により、指定矩形領域に含まれる三角形ポリゴンを抽出する。
	///
	///  @param[in] q_bbox	検索範囲を示す矩形領域。
	///  @param[in] every	true:3頂点が全て検索領域に含まれるものを抽出。
	///						false:1頂点でも検索領域に含まれるものを抽出。
	///  @return	抽出したポリゴンリストのポインタ。
	///	 @attention	三角形ポリゴンのメモリ領域は新たにPolylib内で確保される。
	///  @attention MPIPolylib内での利用が目的なので、ユーザは使用しないこと。
	///
	const std::vector<PrivateTriangle<T>*>* linear_search(
		BBox<T>	*q_bbox, 
		bool	every
	) const;

	///
	/// 線形探索により、指定矩形領域に含まれるポリゴンを抽出する。
	///
	///  @param[in]		q_bbox		検索範囲を示す矩形領域。
	///  @param[in]		every		true:3頂点が全て検索領域に含まれるものを抽出。
	///								false:1頂点でも検索領域に含まれるものを抽出。
	///  @param[in,out] tri_list	抽出した三角形ポリゴンリストへのポインタ。
	///  @return	POLYLIB_STATで定義される値が返る。
	///  @attention	tri_listで戻される三角形ポリゴンのポインタは、Polylib内で
	///				保持されるアドレス値なので、ユーザはdeleteしないで下さい。
	///  @attention	オーバーロードメソッドあり。
	///
	POLYLIB_STAT linear_search(
		BBox<T>							*q_bbox, 
		bool							every,
		std::vector<PrivateTriangle<T>*>	*tri_list
	) const;

	///
	/// KD木探索により、指定位置に最も近いポリゴンを検索する。
	///
	///  @param[in]     pos     指定位置
	///  @return 検索されたポリゴン
	///
	const PrivateTriangle<T>* search_nearest(
		const Vec3<T>&    pos
	) const;

	///
	/// 配下の全ポリゴンのm_exid値を指定値にする。
	///
	/// @param[in]	id	指定値
	///
	POLYLIB_STAT set_all_exid(
		const int	id
	) const;

	//=======================================================================
	// Setter/Getter
	//=======================================================================
	///
	/// TriMeshクラスが管理しているBoundingBoxを返す。
	///
	BBox<T> get_bbox() const {
		return m_bbox;
	}

	///
	/// KD木クラスを取得。
	///
	/// @return KD木クラス。
	///
	VTree<T> *get_vtree() const {
		return m_vtree;
	}

private:
	///
	/// 三角形ポリゴンリストの初期化。
	///
	void init_tri_list();

	//=======================================================================
	// クラス変数
	//=======================================================================
	/// 全三角形ポリゴンを外包するBoundingBox。
	BBox<T>	m_bbox;

	/// KD木クラス。
	VTree<T>	*m_vtree;

	/// MAX要素数。
	int		m_max_elements;
};

// public /////////////////////////////////////////////////////////////////////
template <typename T>
TriMesh<T>::TriMesh()
{
	m_vtree = NULL;
	this->m_tri_list = NULL;
	m_max_elements = M_MAX_ELEMENTS;
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
TriMesh<T>::~TriMesh()
{
	delete m_vtree;
	if (this->m_tri_list != NULL) {
	  typename std::vector<PrivateTriangle<T>*>::iterator itr;
		for (itr = this->m_tri_list->begin(); itr != this->m_tri_list->end(); itr++) {
			delete *itr;
		}
		this->m_tri_list->clear();
	}
	delete this->m_tri_list;
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
void TriMesh<T>::init(const std::vector<PrivateTriangle<T>*>* trias)
{
	init_tri_list();
	typename std::vector<PrivateTriangle<T>*>::const_iterator itr;
	for (itr = trias->begin(); itr != trias->end(); itr++) {
		this->m_tri_list->push_back(
		    new PrivateTriangle<T>((*itr)->get_vertex(),	
					  (*itr)->get_normal(), 
					  (*itr)->get_area(),
					  (*itr)->get_id())
		);
	}
}

// public /////////////////////////////////////////////////////////////////////
// std::sort用ファンクタ
template <typename T>
struct PrivTriaLess{
	bool operator()( const PrivateTriangle<T> *l, const PrivateTriangle<T> *r ) const
	{
		return l->get_id() < r->get_id();
	}
};
// std::equal用ファンクタ
template <typename T>
struct PrivTriaEqual{
	bool operator()( const PrivateTriangle<T> *l, const PrivateTriangle<T> *r ) const
	{
		return l->get_id() == r->get_id();
	}
};

// public /////////////////////////////////////////////////////////////////////
template <typename T>
void
TriMesh<T>::add(
	const std::vector<PrivateTriangle<T>*> *trias
)
{
#ifdef DEBUG
	PL_DBGOSH << "TriMesh<T>::add_triangles() in." << endl;
#endif
	unsigned int i;

	if (this->m_tri_list == NULL) {
		this->m_tri_list = new std::vector<PrivateTriangle<T>*>;
	}

	// ひとまず全部追加
	for( i=0; i<trias->size(); i++ ) {
		this->m_tri_list->push_back( new PrivateTriangle<T>(*(trias->at(i))) );
	}

	// 三角形リストをID順にソート
	std::sort( this->m_tri_list->begin(), this->m_tri_list->end(), PrivTriaLess<T>() );

	// ID重複ぶんを削除
	this->m_tri_list->erase(
	std::unique(this->m_tri_list->begin(), this->m_tri_list->end(), PrivTriaEqual<T>()),
		this->m_tri_list->end());
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT TriMesh<T>::import(const std::map<std::string, std::string> fmap, T scale)
{
	init_tri_list();
	return TriMeshIO::load<T>(this->m_tri_list, fmap, scale);
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT TriMesh<T>::build()
{
	BBox<T> bbox;
	typename std::vector<PrivateTriangle<T>*>::iterator itr;

	/// TriMeshクラスに含まれる全三角形ポリゴンを外包するBoundingBoxを計算
	bbox.init();
	for (itr = this->m_tri_list->begin(); itr != this->m_tri_list->end(); itr++) {
		const Vec3<T>* vtx_arr = (*itr)->get_vertex();
		for (int i = 0; i < 3; i++) {
			bbox.add(vtx_arr[i]);
		}
	}
	m_bbox = bbox;
#define DEBUG
#ifdef DEBUG
	Vec3<T> min = m_bbox.getPoint(0);
	Vec3<T> max = m_bbox.getPoint(7);
	PL_DBGOSH << "TriMesh::build:min=(" <<min<< "),max=(" <<max<< ")" << std::endl;
#endif

	// 木構造作成
	if (m_vtree != NULL) delete m_vtree;
	m_vtree = new VTree<T>(m_max_elements, m_bbox, this->m_tri_list);
	return PLSTAT_OK;
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
int TriMesh<T>::triangles_num() {
	if (this->m_tri_list == NULL)		return 0;
	else						return this->m_tri_list->size();
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
const std::vector<PrivateTriangle<T>*> *TriMesh<T>::search(
	BBox<T>	*bbox, 
	bool	every
) const {
#ifdef DEBUG
	Vec3<T> min = bbox->getPoint(0);
	Vec3<T> max = bbox->getPoint(7);
	PL_DBGOSH << "TriMesh::search:min=(" <<min<< "),max=(" <<max<< ")" << std::endl;
#endif

	return m_vtree->search(bbox, every);
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT TriMesh<T>::search(
	BBox<T>						*bbox, 
	bool						every, 
	std::vector<PrivateTriangle<T>*>	*tri_list
) const {
	return m_vtree->search(bbox, every, tri_list);
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
const std::vector<PrivateTriangle<T>*>* TriMesh<T>::linear_search(
	BBox<T>	*q_bbox, 
	bool	every
) const {
	std::vector<PrivateTriangle<T>*>		   *tri_list = new std::vector<PrivateTriangle<T>*>;
	typename std::vector<PrivateTriangle<T>*>::iterator itr;

	for (itr = this->m_tri_list->begin(); itr != this->m_tri_list->end(); itr++) {
		BBox<T> bbox;
		bbox.init();
		const Vec3<T>* vtx_arr = (*itr)->get_vertex();
		for (int i = 0; i < 3; i++) {
			bbox.add(vtx_arr[i]);
		}
		if (every == true) {
			if (q_bbox->contain(vtx_arr[0]) == true && 
				q_bbox->contain(vtx_arr[1]) == true &&
				q_bbox->contain(vtx_arr[2]) == true)
			{
				tri_list->push_back(*itr);
			}
		}
		else {
#ifdef OLD_DEF
			if (bbox.crossed(*q_bbox) == true				||
				bbox.contain(q_bbox->getPoint(0)) == true	||
				q_bbox->crossed(bbox) == true				||
				q_bbox->contain(bbox.getPoint(0)) == true) {
#else
			if (bbox.crossed(*q_bbox) == true) {
#endif
				tri_list->push_back(*itr);
			}
		}
	}
	return tri_list;
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT TriMesh<T>::linear_search(
	BBox<T>						*q_bbox, 
	bool						every, 
	std::vector<PrivateTriangle<T>*>	*tri_list
) const {
	if (tri_list == NULL) return PLSTAT_ARGUMENT_NULL;

	typename std::vector<PrivateTriangle<T>*>::iterator itr;

	for (itr = this->m_tri_list->begin(); itr != this->m_tri_list->end(); itr++) {
		BBox<T> bbox;
		bbox.init();
		const Vec3<T>* vtx_arr = (*itr)->get_vertex();
		for (int i = 0; i < 3; i++) {
			bbox.add(vtx_arr[i]);
		}
		if (every == true) {
			if (q_bbox->contain(vtx_arr[0]) == true	&&
				q_bbox->contain(vtx_arr[1]) == true	&&
				q_bbox->contain(vtx_arr[2]) == true) {
				tri_list->push_back(*itr);
#ifdef DEBUG
			PL_DBGOSH << "TriMesh<T>::linear_search:IN TRUE" << std::endl;
			PL_DBGOSH << "     vertex 0:" << vtx_arr[0] << std::endl;
			PL_DBGOSH << "     vertex 1:" << vtx_arr[1] << std::endl;
			PL_DBGOSH << "     vertex 2:" << vtx_arr[2] << std::endl;
#endif
			}
		}
		else {
#ifdef OLD_DEF
			if (bbox.crossed(*q_bbox) == true				||
				q_bbox->crossed(bbox) == true				||
				bbox.contain(q_bbox->getPoint(0)) == true	||
				q_bbox->contain(bbox.getPoint(0)) == true) {
#else
			if (bbox.crossed(*q_bbox) == true) {
#endif
				tri_list->push_back(*itr);
#ifdef DEBUG
				PL_DBGOSH << "TriMesh::linear_search:IN FALSE" << std::endl;
#endif
			}
		}
#ifdef DEBUG
		for (int i=0; i<8; i++) {
			PL_DBGOSH << "TriMesh::linear_search:q_box[" << i << "]:" 
					  << q_bbox->getPoint(i) << std::endl;
		}
	    PL_DBGOSH << "TriMesh::linear_searc:" << " id:" << (*itr)->get_id()
				  << ",v(" << vtx_arr << ")" << std::endl;
#endif
	}
	return PLSTAT_OK;
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
const PrivateTriangle<T>* TriMesh<T>::search_nearest(
	const Vec3<T>&    pos
) const {
	return m_vtree->search_nearest(pos);
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT TriMesh<T>::set_all_exid(
	const int    id
) const {
	// 全ポリゴンのm_exidをidで上書き
  typename std::vector<PrivateTriangle<T>*>::iterator itr;
	for (itr = this->m_tri_list->begin(); itr != this->m_tri_list->end(); itr++) {
		(*itr)->set_exid( id );
	}
	return PLSTAT_OK;
}

// private ////////////////////////////////////////////////////////////////////
template <typename T>
void TriMesh<T>::init_tri_list()
{
	if (this->m_tri_list == NULL) {
		this->m_tri_list = new std::vector<PrivateTriangle<T>*>;
	}
	else {
	  typename std::vector<PrivateTriangle<T>*>::iterator itr;
		for (itr = this->m_tri_list->begin(); itr != this->m_tri_list->end(); itr++) {
			delete *itr;
		}
		this->m_tri_list->clear();
	}
}




} //namespace PolylibNS

#endif  // polylib_trimesh_h
