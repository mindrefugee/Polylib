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

#ifndef polylib_polygons_h
#define polylib_polygons_h

#include <vector>
#include <map>
#include "polygons/VTree.h"
#include "common/tt.h"
#include "common/Vec3.h"
#include "common/BBox.h"
#include "common/PolylibStat.h"
#include "common/PolylibCommon.h"

namespace PolylibNS {

template <typename T> class Triangle;
template <typename T> class PrivateTriangle;

////////////////////////////////////////////////////////////////////////////
///
/// クラス:Polygons
/// 三角形ポリゴン集合を管理する純粋仮想クラスです。
///
////////////////////////////////////////////////////////////////////////////
template <typename T>
class Polygons {
public:
	///
	/// コンストラクタ。
	///
	Polygons(){};

	///
	/// デストラクタ。
	///
	virtual ~Polygons() = 0;

	///
	/// 引数で与えられる三角形ポリゴンリストの複製を設定する。
	/// @param[in] trias 設定する三角形ポリゴンリスト。
	/// @attention オーバーロードメソッドあり。
	///
	virtual void init(
		const std::vector<PrivateTriangle<T>*>	*trias
	) = 0;

	///
	/// 三角形ポリゴンリストに引数で与えられる三角形を追加する。
	///
	///  @param[in] trias 設定する三角形ポリゴンリスト。
	///
	virtual void add(
		const std::vector<PrivateTriangle<T>*>		*trias
	) = 0;

	///
	/// STLファイルを読み込みデータの初期化。
	///
	///  @param[in] fname	ファイル名とファイルフォーマットのmap。
	///  @return	POLYLIB_STATで定義される値が返る。
	///
	virtual POLYLIB_STAT import(
		const std::map<std::string, std::string>	fname,
		T scale = 1.0
	) = 0;

	///
	/// Polygonsクラスに含まれる全ポリゴン情報からKD木を作成する。
	///
	///  @return	POLYLIB_STATで定義される値が返る。
	///
	virtual POLYLIB_STAT build() = 0;

	///
	/// Polygonsクラスで保持する三角形ポリゴンの総数を返す。
	/// 
	///  @return 三角形ポリゴンの総数。
	///
	virtual int triangles_num() = 0;

	///
	/// KD木探索により、指定矩形領域に含まれる三角形ポリゴンを抽出する。
	///
	///  @param[in] bbox	検索範囲を示す矩形領域。
	///  @param[in] every	true:3頂点が全て検索領域に含まれるものを抽出。
	///						false:1頂点でも検索領域に含まれるものを抽出。
	///  @return	抽出したポリゴンリストのポインタ。
	///  @attention MPIPolylib内でのみ利用するため、ユーザは使用しないで下さい。
	///  @attention オーバーロードメソッドあり。
	///
	virtual const std::vector<PrivateTriangle<T>*>* search(
	       BBox<T>	*bbox, 
		bool	every
	) const = 0;

	///
	/// KD木探索により、指定矩形領域に含まれるポリゴンを抽出する。
	///
	///  @param[in]  bbox			検索範囲を示す矩形領域。
	///  @param[in]  every			true:3頂点が全て検索領域に含まれるものを抽出。
	///								false:1頂点でも検索領域に含まれるものを抽出。
	///  @param[in,out] tri_list	抽出した三角形ポリゴンリストへのポインタ。
	///  @return	POLYLIB_STATで定義される値が返る。
	///  @attention オーバーロードメソッドあり。
	///
	virtual POLYLIB_STAT search(
		BBox<T>							*bbox, 
		bool							every, 
		std::vector<PrivateTriangle<T>*>	*tri_list
	) const = 0;

	///
	/// 線形探索により、指定矩形領域に含まれる三角形ポリゴンを抽出する。
	///
	///  @param[in] bbox	検索範囲を示す矩形領域。
	///  @param[in] every	true:3頂点が全て検索領域に含まれるものを抽出。
	///						false:1頂点でも検索領域に含まれるものを抽出。
	///  @return	抽出したポリゴンリストのポインタ。
	///  @attention MPIPolylib内でのみ利用するため、ユーザは使用しないで下さい。
	///  @attention オーバーロードメソッドあり。
	///
	virtual const std::vector<PrivateTriangle<T>*>* linear_search(
		BBox<T>	*bbox, 
		bool	every
	) const = 0;

	///
	/// 線形探索により、指定矩形領域に含まれるポリゴンを抽出する。
	///
	///  @param[in]  bbox			検索範囲を示す矩形領域。
	///  @param[in]  every			true:3頂点が全て検索領域に含まれるものを抽出。
	///								false:1頂点でも検索領域に含まれるものを抽出。
	///  @param[in,out] tri_list	抽出した三角形ポリゴンリストのポインタ。
	///  @return	POLYLIB_STATで定義される値が返る。
	///  @attention オーバーロードメソッドあり。
	///
	virtual POLYLIB_STAT linear_search(
		BBox<T>							*bbox, 
		bool							every, 
		std::vector<PrivateTriangle<T>*>	*tri_list
	) const = 0;

	///
	/// KD木探索により、指定位置に最も近いポリゴンを検索する。
	///
	///  @param[in]     pos     指定位置
	///  @return    検索されたポリゴン
	///
	virtual const PrivateTriangle<T>* search_nearest(
		const Vec3<T>&    pos
	) const = 0;

	///
	/// 配下の全ポリゴンのm_exid値を指定値にする。
	///
	/// @param[in]	id	指定値
	///
	virtual POLYLIB_STAT set_all_exid(
		const int id
	) const = 0;

	//=======================================================================
	// Setter/Getter
	//=======================================================================
	///
	/// 三角形ポリゴンのリストを取得。
	///
	/// @return 三角形ポリゴンのリスト。
	///
	std::vector<PrivateTriangle<T>*> *get_tri_list() const {
		return m_tri_list;
	}

	///
	/// KD木クラスを取得。
	///
	/// @return KD木クラス。
	///
	virtual VTree<T> *get_vtree() const = 0;

private:
	///
	/// 三角形ポリゴンリストの初期化。
	///
	virtual void init_tri_list() = 0;

protected:
	//=======================================================================
	// クラス変数
	//=======================================================================
	/// 三角形ポリゴンのリスト。
	std::vector<PrivateTriangle<T>*>	*m_tri_list;
};

///
/// デストラクタ
///  @attention 継承しているクラスから呼び出されるために必要。
///
template <typename T>
Polygons<T>::~Polygons() {}


} //namespace PolylibNS

#endif //polylib_polygons_h
