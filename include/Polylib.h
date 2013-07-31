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

#ifndef polylib_h
#define polylib_h
#include <string.h>
#include <vector>
#include <iostream>
#include "polygons/Polygons.h"
#include "polygons/TriMesh.h"
#include "polygons/Triangle.h"
#include "groups/PolygonGroup.h"
#include "groups/PolygonGroupFactory.h"
#include "common/PolylibStat.h"
#include "common/PolylibCommon.h"
#include "common/BBox.h"
#include "common/Vec3.h"

#include "TextParser.h"
#include "Version.h"

 
namespace PolylibNS {

////////////////////////////////////////////////////////////////////////////
///
/// クラス:CalcAreaInfo
/// 計算領域情報。
///
////////////////////////////////////////////////////////////////////////////
template <typename T>
struct CalcAreaInfo {
	/// 基点座標
	Vec3<T> m_bpos;

	/// 計算領域のボクセル数
	Vec3<T> m_bbsize;

	/// ガイドセルのボクセル数
	Vec3<T> m_gcsize;

	/// ボクセル１辺の長さ
	Vec3<T> m_dx;

	/// ガイドセルを含めた担当領域の最小位置
	Vec3<T> m_gcell_min;

  /// ガイドセルを含めた担当領域の最大位置
	Vec3<T> m_gcell_max;

	/// ガイドセルを含めたBounding Box
	BBox<T> m_gcell_bbox;
};

////////////////////////////////////////////////////////////////////////////
///
/// クラス:PolylibMoveParams
/// Polylib::move()の引数として利用するパラメタセットクラスです。
/// 本クラスメンバ変数ではパラメタが不足する場合は、継承クラスをユーザ定義
/// してください。
///
////////////////////////////////////////////////////////////////////////////
class PolylibMoveParams {
public:
	/// 現在の計算ステップ番号
	int	m_current_step;

	/// 移動後の計算ステップ番号
	int m_next_step;

	/// １計算ステップあたりの時間変異
	double m_delta_t;
};

////////////////////////////////////////////////////////////////////////////
///
/// クラス:Polylib
///	ポリゴンを管理する為のクラスライブラリです。
///
////////////////////////////////////////////////////////////////////////////
template <typename T>
class Polylib
{
public:
	///
	/// singletonのPolylibインスタンス取得。
	/// デフォルトのFactoryクラスであるPolygonGroupFactoryを使用してインスタンス
	/// を生成する。
	///
	///  @return	Polylibクラスのインスタンス。
	///  @attention	呼び出し側でdeleteはできません。
	///
	static Polylib<T>* get_instance();

	///
	/// PolygonGroupクラスを生成するためのFactoryクラスを登録。
	/// 本メソッドは、独自のFactoryクラスを登録しない限り、呼び出し不要である。
	/// コンストラクタで生成したFactoryクラスを破棄し、代わりに引数で指定された
	/// Factoryクラスを登録する。
	///
	///  @param[in] factory	Factoryクラス。
	///  @attention	PolygonGroupを拡張した場合、拡張後のPolygonGroupのFactory
	///				クラスを登録する。
	///
	void set_factory(
		PolygonGroupFactory<T>		*factory = NULL
	);


	///
	/// PolygoGroup、三角形ポリゴン情報の読み込み。
	/// 引数で指定された設定ファイル (TextParser 形式) を読み込み、グループツリーを作成する。
	/// 続いて設定ファイルで指定されたSTLファイルを読み込み、KD木を作成する。
	///
	///  @param[in] config_name 設定ファイル名。
	///  @return	POLYLIB_STATで定義される値が返る。
	///
	POLYLIB_STAT load(
           std::string			config_name = "polylib_config.tpp",
			T				scale = 1.0
	);

	///
	/// PolygoGroupツリー、三角形ポリゴン情報の保存。
	/// グループツリーの情報を設定ファイルへ出力。三角形ポリゴン情報をSTL
	///	ファイルへ出力。
	///
	///	 @param[out] p_config_name	保存した設定ファイル名の返却用。
	///  @param[in]	 stl_format		TriMeshIOクラスで定義されているSTLファイルの
	///								フォーマット。
	///  @param[in]	 extend			ファイル名に付加する文字列。省略可。省略した
	///								場合は、付加文字列として本メソッド呼び出し時
	///								の年月日時分秒(YYYYMMDD24hhmmss)を用いる。
	///  @return	POLYLIB_STATで定義される値が返る。
	///  @attention	ファイル名命名規約は次の通り。
	///			定義ファイル : polylib_config_ランク番号_付加文字.xml。
	///			STLファイル  : ポリゴングループ名_ランク番号_付加文字.拡張子。
	///
	POLYLIB_STAT save(
		std::string			*p_config_name,
		std::string			stl_format,
		std::string			extend = ""
	);

	///
	/// 三角形ポリゴン座標の移動。
	/// 本クラスインスタンス配下の全PolygonGroupのmoveメソッドが呼び出される。
	/// moveメソッドは、PolygonGroupクラスを拡張したクラスに利用者が記述する。
	///
	///  @param[in] params	Polylib.hで宣言された移動計算パラメータセット。
	///  @return	POLYLIB_STATで定義される値が返る。
	///
	POLYLIB_STAT move(
		PolylibMoveParams	&params
	);

	///
	/// PolygoGroupツリーの最上位ノードの取得。
	///
	///  @return	最上位ノードのvector。
	///  @attention 返却したPolygonGroupは、削除不可。vectorは要削除。
	///
	std::vector<PolygonGroup<T> *> *get_root_groups() const;

	///
	/// 三角形ポリゴンの検索。
	/// 位置ベクトルmin_posとmax_posにより特定される矩形領域に含まれる、
	/// 三角形ポリゴンをgroup_nameで指定されたグループの下から探索する。
	///
	///  @param[in] group_name	抽出グループ名。
	///  @param[in] min_pos		抽出する矩形領域の最小値。
	///  @param[in] max_pos		抽出する矩形領域の最大値。
	///  @param[in] every		true:3頂点が全て検索領域に含まれるものを抽出。
	///   						false:3頂点の一部でも検索領域と重なるものを抽出。
	///  @return	抽出した三角形ポリゴンのvector。
	///  @attention 返却した三角形ポリゴンは、削除不可。vectorは要削除。
	///
	std::vector<Triangle<T>*>* search_polygons(
		std::string		group_name, 
		Vec3<T>			min_pos, 
		Vec3<T>			max_pos, 
		bool			every
	) const;

	///
	/// 指定した点に最も近い三角形ポリゴンの検索。
	///
	///  @param[in] group_name	抽出グループ名。
	///  @param[in] pos			指定した点。
	///  @return    検索されたポリゴン
	///
	const Triangle<T>* search_nearest_polygon(
		std::string group_name,
		const Vec3<T>&    pos
	) const;

	///
	/// 引数のグループ名が既存グループと重複しないかチェック。
	///
	///  @param[in] pg_name		グループ名
	///  @param[in] parent_path	親グループまでのフルパス
	///  @return	POLYLIB_STATで定義される値が返る。
	///  @attention	Polylib内部で使用する関数であり、通常は利用者が用いるもの
	///				ではない。
	///
	POLYLIB_STAT check_group_name(
		const std::string	&pg_name, 
		const std::string	&parent_path
	);

	///
	/// PolygonGroupのインスタンスの生成。
	/// 本クラスが管理しているFactoryクラスを利用して、引数で渡されたクラス名
	/// に応じたPolygonGroupのインスタンスを生成する。
	///
	///  @param[in] class_name		クラス名
	///  @return	生成したPolygonGroup
	///  @attention	Polylib内部で使用する関数であり、通常は利用者が用いるもの
	///				ではない。
	///
	PolygonGroup<T> *create_polygon_group(
		std::string		class_name
	);

	///
	/// PolygonGroupの追加。
	/// 本クラスが管理しているPolygonGroupのリストにPolygonGroupを追加する。
	///
	///  @param[in] pg		PolygonGroup
	///  @attention	Polylib内部で使用する関数であり、通常は利用者が用いるもの
	///				ではない。
	///
	void add_pg_list(
		PolygonGroup<T>	*pg
	);

	///
	/// グループ階層構造を標準出力に出力。
	/// 2010.10.20 引数FILE *追加。
	///  @param[in] fp	出力先ファイル。指定されて行ければ、標準出力へ出力する。
	///
	///  @attention	テスト用の関数であり、通常は利用者が用いるものではない。
	///
	void show_group_hierarchy(
		FILE	*fp	= NULL
	);

	///
	/// グループの情報と配下の三角形ポリゴン情報を標準出力に出力。
	/// 	親グループ名、自身の名前、STLファイル名、登録三角形数、3頂点ベクト
	///		ルの座標、法線ベクトルの座標、面積。
	///
	///  @param[in] group_name グループ名。
	///  @return	POLYLIB_STATで定義される値が返る。
	///  @attention	テスト用の関数であり、通常は利用者が用いるものではない。
	///
	POLYLIB_STAT show_group_info(
		std::string		group_name
	);

	///
	/// Polylibが利用中の概算メモリ量を返す
	///
	/// @return 利用中のメモリ量(byte)
	///
	unsigned int used_memory_size();

	///
	/// グループの取得。
	/// nameで与えられた名前のPolygonGroupを返す。
	///  @param[in] name グループ名
	///  @return	ポリゴングループクラスのポインタ。エラー時はNULLが返る。
	///  @attention オーバーロードメソッドあり。
	///
	PolygonGroup<T>* get_group(
		std::string		name
	) const;
  
  /**
   * @brief バージョン番号の文字列を返す
   */
  std::string getVersionInfo()
  {
    std::string str(PL_VERSION_NO);
    return str;
  }
  

protected:
	///
	/// コンストラクタ
	///
	/// @attention
	///   singletonのため、子クラス以外からの呼び出し不可とする
	///
	Polylib();//PolygonGroupFactory* factory = 0);

	///
	/// デストラクタ
	///
	~Polylib();

	///
	/// グループツリー作成。
	/// TextParser クラスを使い、
	/// PolygonGroupを作成し、グループツリーに登録する。
	///
	///  @param[in] TextParser のインスタンス
	///  @return	POLYLIB_STATで定義される値が返る。
	///  @attention	オーバーロードメソッドあり。
	///
	///

  POLYLIB_STAT make_group_tree(
       TextParser *  tp_ptr
		       );

	///
	/// 引数の内容でグループ階層構造を構築。
	///
	///  @param[in] config_contents	設定ファイルの内容(XML形式)。
	///  @return	POLYLIB_STATで定義される値が返る。
	///  @attention	MPIPolylibクラスがMPI環境で利用することを想定している。
	///  @attention	オーバーロードメソッドあり。
	///
	POLYLIB_STAT make_group_tree(
		std::string		config_contents
	);

	///
	/// 設定ファイルを読み込み、内容をcontentsに設定。
	///
	///  @param[out] contents	設定ファイルの内容(XML形式)。
	///  @param[in]  fname		設定ファイル名。
	///  @return	POLYLIB_STATで定義される値が返る。
	///  @attention	MPIPolylibクラスがMPI環境で利用することを想定している。
	///
	POLYLIB_STAT load_config_file(
		std::string			*contents,
		std::string			fname = "" 
	);

	///
	/// 三角形IDファイルの存在が必須なload関数。
	/// loadと同様の動作を行う。但し読み込み時には、三角形IDファイルが必要で
	/// あり、このファイルに記述されているIDを用いてm_idを設定する。
	///
	///  @param[in] config_name	設定ファイル名。
	///  @param[in]	id_format	三角形IDファイルの入力形式。
	///  @return	POLYLIB_STATで定義される値が返る。
	///  @attention	MPIPolylibクラスがMPI環境で利用することを想定している。
	///
	POLYLIB_STAT load_with_idfile(
		std::string		config_name,
		ID_FORMAT		id_format,
		T			scale = 1.0
	);

	///
	/// STLファイルの読み込み。
	/// グループツリーの全リーフについて、設定されているSTLファイルから
	/// ポリゴン情報を読み込む。読み込んだ後、KD木の生成、法線の計算、面積の
	/// 計算を行う。
	///
	///  @param[in] with_id_file	trueならば、三角形ポリゴンIDファイルを読み
	///				込んでm_idを設定する。
	///				falseならば、STL読み込み時にm_idを自動生成。
	///  @param[in]	id_format		三角形IDファイルの入力形式。
	///  @return	POLYLIB_STATで定義される値が返る。
	///
	POLYLIB_STAT load_polygons(
		bool		with_id_file,
		ID_FORMAT	id_format,
		T		scale = 1.0
	);

	///
	/// 設定ファイルの保存。
	/// メモリに展開しているグループツリー情報から設定ファイルを生成する。
	///
	///  @param[in] rank_no	ランク番号
	///  @param[in] extend	ファイル名に付加する文字列
	///  @param[in] format	TriMeshIOクラスで定義されているSTLファイルのフォー
	///						マット。
	///  @return	作成した設定ファイルの名称。エラー時はNULLが返る。
	///
#if 1
	char *save_config_file(
		std::string	rank_no,
		std::string	extend,
		std::string	format
	);
#endif
	
	/// TextParser 内部データから　"filepath" "filepath[*]" というリーフを
	/// すべて削除する.
	///
	/// recursiveの動作の為、引数にtp_ptrが必要
	///
	/// @param[in] tp_ptr TextParser　へのポインタ.
	///  @return	POLYLIB_STATで定義される値が返る。


	POLYLIB_STAT clearfilepath(TextParser* tp_ptr);

	/// TextParser 内部データに　saveしたstl ファイルの　"filepath"を書き込む。
	///
	///　saveしたSTLファイルとPolygonGroupの階層は、save_stl_file に
	///　map を渡し保持してもらう。その　map の内容に基づき、TextParser内部のデータを
	///　変更する.
	///
	/// @param[in] stl_fname_map saveしたSTLファイルとその階層のmap型データ
	/// @return	POLYLIB_STATで定義される値が返る。

	POLYLIB_STAT setfilepath( std::map<std::string,std::string>& stl_fname_map);


	///
	/// 設定ファイルの保存。 PolylibConfig 内部にあったものをここへ。
	//  暫定措置
	//  file name を作ってsave
	///
	///  @param[in] rank_no	ランク番号
	///  @param[in] extend	ファイル名に付加する文字列
	///  @param[in] format	TriMeshIOクラスで定義されているSTLファイルのフォー
	///						マット。
	///  @return	作成した設定ファイルの名称。エラー時はNULLが返る。
	///
#if 1
	char * polylib_config_save_file(
		std::string	rank_no,
		std::string	extend
	);
#endif

	/// PolygoGroupツリー、三角形ポリゴン情報の保存。
	/// グループツリー情報を設定ファイルへ出力。三角形ポリゴン情報をSTLファイル
	/// へ出力。ID情報をIDファイルへ出力。ファイル名にランク番号を付加する。
	///
	///	 @param[out] p_config_name	保存した設定ファイル名の返却用。
	///	 @param[in]  myrank			自ランク番号。
	///	 @param[in]	 maxrank		最大ランク番号。
	///	 @param[in]	 extend			ファイ名に付加される文字列。
	///	 @param[in]	 stl_format		STLファイルフォーマット指定。
       	///  @param[in]	 id_format		三角形IDファイルの出力形式。
	///  @return	POLYLIB_STATで定義される値が返る。
	///  @attention	ファイル名命名規約は次の通り。
	///			定義ファイル : polylib_config_ランク番号_付加文字.xml。
	///			STLファイル  : ポリゴングループ名_ランク番号_付加文字.拡張子。
	///			IDファイル   : ポリゴングループ名_ランク番号_付加文字.ID。
	///  @attention	MPIPolylibクラスがMPI環境で利用することを想定している。
	POLYLIB_STAT save_with_rankno(
		std::string		*p_config_name,
		int				myrank,
		int				maxrank,
		std::string		extend,
		std::string		stl_format,
		ID_FORMAT		id_format
	);

	///
	/// グループ名の表示。
	/// 指定されたグループ以下の階層構造をツリー形式で標準出力に出力する。
	/// 2010.10.20 引数FILE *追加。
	///
	///  @param[in] p	検索の基点となるPolygonGroupのポインタ
	///  @param[in] tab 階層の深さを示すスペース
	///  @param[in] fp	出力先ファイル。指定されて行ければ、標準出力へ出力する。
	///
	void show_group_name(
		PolygonGroup<T>	*p, 
		std::string		tab,
		FILE			*fp
	);

	///
	/// グループの取得。
	/// internal_idで与えられたm_internal_idを持つPolygonGroupを返す。
	///  @param[in] internal_id ポリゴングループID
	///  @return	ポリゴングループクラスのポインタ。エラー時はNULLが返る。
	///  @attention オーバーロードメソッドあり。
	///
	PolygonGroup<T>* get_group(
		int	internal_id
	) const;

private:
	///
	/// 三角形ポリゴンの検索。
	/// 位置ベクトルmin_posとmax_posにより特定される矩形領域に含まれる、
	/// 三角形ポリゴンをgroup_nameで指定されたグループの下から探索する。
	/// 
	///  @param[in]  group_name	抽出グループ名
	///  @param[in]  min_pos	抽出する矩形領域の最小値
	///  @param[in]  max_pos	抽出する矩形領域の最大値
	///  @param[in]  every		true:3頂点が全て検索領域に含まれるものを抽出。
	///   						false:3頂点のBBoxが一部でも検索領域と重なるものを抽出。
	///  @param[in]	 linear		true:線形探索を行う。
	///							false:KD木探索を行う。
	///  @param[out] ret		POLYLIB_STATで定義される値(デバック用)。
	///  @return	抽出した三角形ポリゴンリスト
	///  @attention	publicなsearch_polygons()は内部で本関数を利用している。
	///
	std::vector<PrivateTriangle<T>*> *search_polygons(
		std::string		group_name, 
		Vec3<T>			min_pos, 
		Vec3<T>			max_pos,
		bool			every,
		bool			linear, 
		POLYLIB_STAT	*ret
	) const;

	///
	/// グループの検索。
	/// 基点となるポリゴングループに連なる子孫ポリゴングループを全て抽出する。
	///  @param[in]  p	探索の基点となるポリゴングループへのポインタ
	///  @param[out] pg	抽出した子孫ポリゴングループのリスト
	///
	void search_group(
		PolygonGroup<T>				*p, 
		std::vector<PolygonGroup<T>*>	*pg
	) const;


protected:
	//=======================================================================
	// クラス変数
	//=======================================================================
	/// 自クラスのインスタンス(singleton)
  //static Polylib<T>		*m_instance;

	/// PolygonGroupのファクトリークラス
	PolygonGroupFactory<T>			*m_factory;

	/// ポリゴングループリスト
	std::vector<PolygonGroup<T>*>	m_pg_list;


	// TextParser へのポインタ
	TextParser* tp;

};



// public /////////////////////////////////////////////////////////////////////
template <typename T>
Polylib<T>* Polylib<T>::get_instance() {
	// この方法ならば、デストラクタを呼び出さなくてもクラスインスタンスの領域
	// が解放できるし、もし本関数が複数回呼び出されても、クラスインスタンスが
	// 複数作成されることはない(=singletonになる)
	static Polylib<T> m_instance;
	return &m_instance;
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
void Polylib<T>::set_factory(
	PolygonGroupFactory<T>		*factory
) {
	if (factory == NULL) return;

	// 明示的に指定された場合は、そのFactoryクラスを使用する
	delete m_factory;
	m_factory = factory;
}

/// TextParser
// public /////////////////////////////////////////////////////////////////////
// 
template <typename T>
POLYLIB_STAT Polylib<T>::load(
	std::string	config_name,
	T	scale
) {

#ifdef DEBUG
	PL_DBGOSH << "Polylib<T>::load_test() in." << std::endl;
#endif

	// 設定ファイル読み込み
	try {
	  //PolylibConfig base(config_name);

	  tp->read(config_name);
	  //  tp->write("tmp.tpp");
	  // グループツリー作成
	  POLYLIB_STAT stat = make_group_tree(tp);
	  if (stat != PLSTAT_OK)	return stat;

	  // STLファイル読み込み (三角形IDファイルは不要なので、第二引数はダミー)
	  return load_polygons(false, ID_BIN, scale);
	}
	catch (POLYLIB_STAT e) {
		return e;
	}
}

// public /////////////////////////////////////////////////////////////////////
//TextParser 版
template <typename T>
POLYLIB_STAT Polylib<T>::save(
	std::string	*p_config_name,
	std::string	stl_format,
	std::string	extend
) {
  //#ifdef DEBUG
	PL_DBGOSH << "Polylib::save() in." << std::endl;
	//#endif
	char	my_extend[128];
	POLYLIB_STAT stat=PLSTAT_OK;


	// 拡張文字列がカラであれば、現在時刻から作成
	if (extend == "") {
	  time_t timer = time(NULL);
	  struct tm	*date = localtime(&timer);
	  sprintf(my_extend, "%04d%02d%02d%02d%02d%02d",
		  date->tm_year+1900,date->tm_mon+1,date->tm_mday,
		  date->tm_hour,date->tm_min,date->tm_sec);
	}
	else {
	  sprintf(my_extend, "%s", extend.c_str());
	}
	
	//PL_DBGOSH << __FUNCTION__ << " extend "<< my_extend << std::endl;

	std::map<std::string,std::string> stl_fname_map;
	typename std::vector<PolygonGroup<T>*>::iterator it;
	for (it = m_pg_list.begin(); it != m_pg_list.end(); it++) {
	  //リーフのみがポリゴン情報を持っている
	  if ((*it)->get_children().empty() == false)	continue;

	  // ポリゴン数が0ならばファイル出力不要 2010.10.19
	  if ((*it)->get_triangles()->size() == 0)	continue;

	  // STLファイル保存 (第一引数のランク番号は不要)
	  stat = (*it)->save_stl_file("", my_extend, stl_format,
						   stl_fname_map);
	  if (stat != PLSTAT_OK) return stat;

	  stat = (*it)->mk_param_tag(tp, "", "", "");
	  if (stat != PLSTAT_OK) return stat;
	}


	// update stl filepath 
	// clear file path first
	stat=clearfilepath(tp);
	//tp->write("tmp2.tpp");
	// set filepath
	stat=setfilepath(stl_fname_map);

	//	std::string tmp_extend = my_extend;
	//	char	*config_name = save_config_file("", tmp_extend, stl_format);
	char	*config_name = save_config_file("", my_extend, stl_format);
	//	PL_DBGOSH << __FUNCTION__ << " config_name "<< config_name << std::endl;

	if (config_name == NULL)	return PLSTAT_NG;
	else	*p_config_name = std::string(config_name);
	return PLSTAT_OK;
	
}


// public /////////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT Polylib<T>::move(
	PolylibMoveParams	&params
) {
#ifdef DEBUG
	PL_DBGOSH << "Polylib::move() in." << std::endl;
#endif
	POLYLIB_STAT ret;
	typename std::vector<PolygonGroup<T>*>::iterator it;

	for (it = m_pg_list.begin(); it != m_pg_list.end(); it++) {

		// リーフグループで、movableフラグONのポリゴンを移動
		if ((*it)->get_children().empty() == true && (*it)->get_movable() ) {
			ret = (*it)->move(params);
			if (ret != PLSTAT_OK)	return ret;

			// 座標移動したのでKD木の再構築
			ret = (*it)->rebuild_polygons();
			if( ret != PLSTAT_OK ) return ret;
		}

	}
	return PLSTAT_OK;
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
std::vector<PolygonGroup<T> *> *Polylib<T>::get_root_groups() const {
#ifdef DEBUG
	PL_DBGOSH << "Polylib::get_root_groups() in." << std::endl;
#endif
	std::vector<PolygonGroup<T>*>	*root = new std::vector<PolygonGroup<T>*>;
	typename std::vector<PolygonGroup<T>*>::const_iterator	it;

	for (it = m_pg_list.begin(); it != m_pg_list.end(); it++) {
		if (((*it)->get_parent()) == NULL) {
			root->push_back(*it);
		}
	}
	return root;
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
std::vector<Triangle<T>*>* Polylib<T>::search_polygons(
	std::string		group_name, 
	Vec3<T>		min_pos, 
	Vec3<T>		max_pos, 
	bool		every
) const {
#ifdef DEBUG
	PL_DBGOSH << "Polylib::search_polygons() in." << std::endl;
#endif
	POLYLIB_STAT ret;
	return (std::vector<Triangle<T>*>*)
		search_polygons(group_name, min_pos, max_pos,every, false, &ret);
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT Polylib<T>::check_group_name(
	const std::string	&name, 
	const std::string	&path
) {
#ifdef DEBUG
	PL_DBGOSH << "Polylib::check_group_name() in." << std::endl;
#endif
	if (name.empty() == true) {
		PL_ERROSH << "[ERROR]Polylib::check_group_name():Group name is empty." 
				  << std::endl;
		return PLSTAT_GROUP_NAME_EMPTY;
	}

	typename std::vector<PolygonGroup<T>*>::iterator it;
	for (it = m_pg_list.begin(); it != m_pg_list.end(); it++) {
		if ((*it)->get_name() == name && (*it)->get_parent_path() == path) {
			PL_ERROSH << "[ERROR]Polylib::check_group_name():Group name is "
				<< "duplicate:name:" << name << "," << "path:" << path 
				<< std::endl;
			return PLSTAT_GROUP_NAME_DUP;
		}
	}
	return PLSTAT_OK;
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
PolygonGroup<T> *Polylib<T>::create_polygon_group(std::string class_name)
{
#ifdef DEBUG
	PL_DBGOSH << "Polylib::create_polygon_group() in." << std::endl;
#endif
	return m_factory->create_instance(class_name);
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
void Polylib<T>::add_pg_list(PolygonGroup<T> *pg)
{
#ifdef DEBUG
	PL_DBGOSH << "Polylib::add_pg_list() in." << std::endl;
#endif
	m_pg_list.push_back(pg);
}

// public /////////////////////////////////////////////////////////////////////
/// 2010.10.20 引数FILE *追加。
template <typename T>
void Polylib<T>::show_group_hierarchy(FILE	*fp)
{
#ifdef DEBUG
	PL_DBGOSH << "Polylib::show_group_hierarchy() in." << std::endl;
#endif
	std::string		tab;
	typename std::vector<PolygonGroup<T>*>::iterator	it;
	for (it = m_pg_list.begin(); it != m_pg_list.end(); it++) {
		if ((*it)->get_parent() != NULL) {
			// Not Use
		}
		else{
			show_group_name(*it, tab, fp);
		}
	}
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT Polylib<T>::show_group_info(std::string group_name)
{
#ifdef DEBUG
	PL_DBGOSH << "Polylib::show_group_info() in." << std::endl;
#endif
	PolygonGroup<T> *p = get_group(group_name);
	if (p == NULL) {
		PL_ERROSH << "[ERROR]Polylib<T>::show_group_info():Group not found:"
			 << group_name << std::endl;
		return PLSTAT_GROUP_NOT_FOUND;
	}

	return p->show_group_info();
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
unsigned int Polylib<T>::used_memory_size()
{
	unsigned int size;
	typename std::vector<PolygonGroup<T>*>::iterator pg;
	typename std::vector<PrivateTriangle<T>*>::iterator pt;

	// 自クラスとFactoryクラス
	size = sizeof(Polylib) + sizeof(PolygonGroupFactory<T>);

	// ポリゴングループ
#ifdef DEBUG
PL_DBGOSH << "Polylib::used_memory_size:PolygonGroup num=" << m_pg_list.size() << std::endl;
#endif
	for (pg = m_pg_list.begin(); pg != m_pg_list.end(); pg++) {

		// PolygonGroupクラス
		size += sizeof(PolygonGroup<T>);

		// TriMeshクラス
		size += sizeof(TriMesh<T>);

		// 三角形移動前一時リスト
		size += (*pg)->get_num_of_trias_before_move() * sizeof(std::vector<void> *);

		// リーフにはポリゴンがある
		if ((*pg)->get_children().empty()) {

			// 三角形ポリゴン
			std::vector<PrivateTriangle<T>*>	*tri_list = (*pg)->get_triangles();
#ifdef DEBUG
PL_DBGOSH << "Polylib::used_memory_size:PrivateTriangle num=" << tri_list->size() << std::endl;
#endif
			size += tri_list->size() * sizeof(PrivateTriangle<T>);

			// KD木
			VTree<T> *vtree = (*pg)->get_vtree();
			size += vtree->memory_size();
		}

	}

	return size;
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
PolygonGroup<T>* Polylib<T>::get_group(std::string name) const
{
#ifdef DEBUG
	PL_DBGOSH << "Polylib<T>::get_group(" << name << ") in." << std::endl;
#endif
	typename std::vector<PolygonGroup<T>*>::const_iterator it;
	for (it = m_pg_list.begin(); it != m_pg_list.end(); it++) {
		if (name == (*it)->acq_fullpath()) {
#ifdef DEBUG
			PL_DBGOS	<< "get_group: " << (*it)->get_parent_path()
						<< " name: " << (*it)->get_name()
						<< " size: " << (*it)->get_children().size() << std::endl;
#endif
			return *it;
		}
	}
	for (it = m_pg_list.begin(); it != m_pg_list.end(); it++) {
		if (name == (*it)->get_name()) {
			return *it;
		}
	}
#ifdef DEBUG
	PL_DBGOS << "Polylib::get_group(" << name << ") returns NULL" << std::endl;
#endif
	return NULL;
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
const Triangle<T>* Polylib<T>::search_nearest_polygon(
	std::string	 group_name, 
	const Vec3<T>&	pos
) const {

	PolygonGroup<T>* pg = get_group(group_name);
	if (pg == 0) {
		PL_ERROSH << "[ERROR]Polylib::search_polygons():Group not found: " 
				  << group_name << std::endl;
		return 0;
	}

	std::vector<PolygonGroup<T>*>* pg_list2 = new std::vector<PolygonGroup<T>*>;

	//子孫を検索
	search_group(pg, pg_list2);

	//自身を追加
	pg_list2->push_back(pg);

		const PrivateTriangle<T>* tri_min = 0;
		T dist2_min = 0.0;

	//対象ポリゴングループ毎に検索
	typename std::vector<PolygonGroup<T>*>::iterator it;
	for (it = pg_list2->begin(); it != pg_list2->end(); it++) {
		//リーフポリゴングループからのみ検索を行う
		if ((*it)->get_children().size()==0) {
				const PrivateTriangle<T>* tri = (*it)->search_nearest(pos);
				if (tri) {
					Vec3<T>* v = tri->get_vertex();
					Vec3<T> c((v[0][0]+v[1][0]+v[2][0])/3.0,
							(v[0][1]+v[1][1]+v[2][1])/3.0,
							(v[0][2]+v[1][2]+v[2][2])/3.0);
					T dist2 = (c - pos).lengthSquared();
					if (tri_min == 0 || dist2 < dist2_min) {
						tri_min = tri;
						dist2_min = dist2;
					}
				}
			}
		}

	delete pg_list2;

	return (const Triangle<T>*)tri_min;
}

// protected //////////////////////////////////////////////////////////////////
template <typename T>
Polylib<T>::Polylib()
{
	gs_rankno = "";
	// デフォルトのファクトリークラスを登録する 2010.08.16
	m_factory = new PolygonGroupFactory<T>();

	
	//Polylib にTextParser クラスを持たせる。
	tp = new TextParser;
	
	
	//PL_DBGOS<< __FUNCTION__ <<" m_factory "<< m_factory << " tp " << tp<<std::std::endl;

}

// protected //////////////////////////////////////////////////////////////////
template <typename T>
Polylib<T>::~Polylib()
{
  typename std::vector<PolygonGroup<T>*>::iterator it;
	for (it = m_pg_list.begin(); it != m_pg_list.end();) {
#ifdef DEBUG
PL_DBGOSH << "~Polylib():" << (*it)->get_name() << std::endl;
#endif
		(*it)->get_children().clear();
		delete *it;
		it = m_pg_list.erase(it);
	}
	if(tp !=0) delete tp;

}

// protected //////////////////////////////////////////////////////////////////
// TextParser 版
template <typename T>
POLYLIB_STAT Polylib<T>::make_group_tree(
    TextParser* tp
) {
#ifdef DEBUG
	PL_DBGOSH << "Polylib::make_group_tree(TextParser) in." << std::endl;
#endif
	TextParserError status = TP_NO_ERROR;

	// 念のため階層構造の最上位へ
	std::string cur;	
	tp->currentNode(cur);
	if(cur != "/Polylib") {
	  status=tp->changeNode("/Polylib");
	  tp->currentNode(cur);

	  if(status!=TP_NO_ERROR){
	    PL_ERROSH << 
	      "[ERROR]Polylib::make_group_tree(TextParser):Root node not found."
		      << std::endl;
	    return PLSTAT_CONFIG_ERROR;
	  }

	}
#if 0 
	if(cur != "/") {
	  status=tp->changeNode("/");
	  tp->currentNode(cur);

	  if(status!=TP_NO_ERROR){
	    PL_ERROSH << 
	      "[ERROR]Polylib::make_group_tree(TextParser):Root node not found."
		      << std::endl;
	    return PLSTAT_CONFIG_ERROR;
	  }

	}
#endif

	// ノードとリーフのリストを取る

	//ノードを取得
	std::vector<std::string> nodes;
	tp->getNodes(nodes);
	std::string current_node;


	//	tp->currentNode(current_node);
	// std::vector<std::string> leaves;
	// tp->getLabels(leaves);
	// if(nodes.size()==0 && leaves.size()==0){ return PLSTAT_CONFIG_ERROR;}

	// std::string class_name = "PolygonGroup";
	// if(leaves.size()!=0){
	//   std::vector<std::string>::iterator leaf_iter=find(leaves.begin(),
	// 					  leaves.end(),
	// 					  PolygonGroup::ATT_NAME_CLASS);
	//   if(leaf_iter != leaves.end()){
	//     //	    class_name = *leaf_iter;
	//     std::string value;
	//     status=tp->getValue((*leaf_iter),value);
	//     class_name=value;

	//   }
	// }


	// loop over node recurcively.
	PolygonGroup<T> *pg;
	for(typename std::vector<std::string>::iterator nodes_iter = nodes.begin(); 
	    nodes_iter != nodes.end();
	    nodes_iter++){

	  status = tp->changeNode(*nodes_iter);
	  tp->currentNode(current_node);
#ifdef DEBUG	  
	  PL_DBGOS<<"current_node "<< current_node <<  std::endl;
#endif // DEBUG	  
	  std::vector<std::string> nodes;
	  std::vector<std::string> leaves;

	  tp->getNodes(nodes);
	  tp->getLabels(leaves);

	  // comment out by T. Suzuki Hulinks Inc. 
	  // To fix examples/test_mpi2. Please check it later. 2013/7/31
	  //if(nodes.size()==0 && leaves.size()==0){ return PLSTAT_CONFIG_ERROR;}

	  std::string class_name = "PolygonGroup"; //default

	  if(leaves.size()!=0){
	    typename std::vector<std::string>::iterator leaf_iter=find(leaves.begin(),
						    leaves.end(),
						    PolygonGroup<T>::ATT_NAME_CLASS);
	    if(leaf_iter != leaves.end()){
	      //     class_name = *leaf_iter;
	      std::string value;
	      status=tp->getValue((*leaf_iter),value);
	      class_name=value;
	    }
	  }
	  pg = m_factory->create_instance(class_name);
	  add_pg_list(pg);
	  if (pg == NULL) {
	    PL_ERROSH << "[ERROR]Polylib::make_group_tree():Class name not found."
		      << class_name
		      << std::endl;
	    return PLSTAT_CONFIG_ERROR;
	  }


	  // 配下のタグを取得して、PolygonGroupツリーを作成
	  //	  POLYLIB_STAT res = pg->build_group_tree(this, NULL, elem);
	  //  
	  POLYLIB_STAT res = pg->build_group_tree(this, NULL, tp);
	  if (res != PLSTAT_OK) return res;
	  status = tp->changeNode("..");

	} 
	

	return PLSTAT_OK;
}

//TextParser Version
// protected //////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT Polylib<T>::make_group_tree(
	std::string		config_contents
) {
#ifdef DEBUG
	PL_DBGOSH << "Polylib::make_group_tree() in." << std::endl;
#endif
	try {
	 tp->read(config_contents);
	  return make_group_tree(tp);
	}
	catch (POLYLIB_STAT e) {
		return e;
	}
	return PLSTAT_NG;
}

// TextParser 版
// protected //////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT Polylib<T>::load_config_file(
	std::string		*contents,
	std::string		fname
) {
#ifdef DEBUG
 	PL_DBGOSH << "Polylib::load_config_file() in." << std::endl;
#endif
	// 設定ファイルを読み込みstring型で返す
	//	return PolylibConfig::load_config_file(contents, fname);
	//	return PolylibConfig::load_config_file(contents, fname);

	// right now just return no error
	return PLSTAT_OK;

}
/// textparser 版
// protected //////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT Polylib<T>::load_with_idfile(
	std::string		config_name,
	ID_FORMAT	id_format,
	T		scale
) {
#ifdef DEBUG
	PL_DBGOSH << "Polylib::load_with_idfile() in." << std::endl;
#endif
	// 設定ファイル読み込み
	tp->read_local(config_name);

	// グループツリー作成
	
	POLYLIB_STAT stat = make_group_tree(tp);
	if (stat != PLSTAT_OK)	return stat;

	// STLファイルとIDファイル読み込み
	return load_polygons(true, id_format, scale);
}


// protected //////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT Polylib<T>::load_polygons(
	bool		with_id_file,
	ID_FORMAT	id_format,
	T		scale
)
{
#ifdef DEBUG
	PL_DBGOSH << "Polylib::load_polygons() in." << std::endl;
#endif
	typename std::vector<PolygonGroup<T>*>::iterator it;
	for (it = m_pg_list.begin(); it != m_pg_list.end(); it++) {
		// リーフの場合
		if ((*it)->get_children().empty() == true) {

			//STLファイルを読み込む
			POLYLIB_STAT ret = (*it)->load_stl_file(scale);
			if (ret != PLSTAT_OK)		return ret;

			// 必要であればIDファイルを読み込んでm_idを設定
			if (with_id_file == true) {
				POLYLIB_STAT ret = (*it)->load_id_file(id_format);
				if (ret != PLSTAT_OK)		return ret;
			}
		}
	}
	return PLSTAT_OK;
}


// protected //////////////////////////////////////////////////////////////////
//TextPArser 版
template <typename T>
char *Polylib<T>::save_config_file(
	std::string	rank_no,
	std::string	extend,
	std::string	format
){
#ifdef DEBUG
  //  PL_DBGOSH << "Polylib::save_config_file() in. " << std::endl;
#endif

  typename std::vector<PolygonGroup<T>*>::iterator	it;
	//POLYLIB_STAT					stat;

	// ファイル出力
	char	*config_name;

	if (rank_no == "") {
		// ランク番号不要
	  config_name = polylib_config_save_file("", extend);
	  //config_name = PolylibConfig::save_file(doc, "", extend);
	}
	else {
	  // 要ランク番号(MPI版)
	  //	config_name = PolylibConfig::save_file(doc, rank_no, extend);
	  config_name = polylib_config_save_file(rank_no, extend);
	}

#ifdef DEBUG
	//PL_DBGOSH << "save_config_file(): " << config_name << std::endl;
#endif
//	xmlFreeDoc(doc);

	return config_name;
}

/////////////////////////////////////////　　
// 追加　２０１２ー０８
//////////////////////////////////
template <typename T>
char* Polylib<T>::polylib_config_save_file(
					std::string rank_no,
					std::string extend){

#define POLYLIB_CONFIG_NAME		"polylib_config"  
#define POLYLIB_CONFIG_EXT		"tpp"

  //  cout <<__FUNCTION__<<" in "<< rank_no <<" "<< extend << std::endl;

  static char fname[1024];
  if (rank_no == ""){
    sprintf(fname,"%s_%s.%s",POLYLIB_CONFIG_NAME,extend.c_str(),POLYLIB_CONFIG_EXT);
  } else {
    sprintf(fname, "%s_%s_%s.%s", POLYLIB_CONFIG_NAME, rank_no.c_str(),
	    extend.c_str(), POLYLIB_CONFIG_EXT);
  }
  std::string fname_string = fname;

  //config_file の書き込み

  tp->write(fname_string,1);

  return fname;

}


//TextParser version 
// protected //////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT Polylib<T>::save_with_rankno(
	std::string		*p_config_name,
	int			myrank,
	int 		maxrank,
	std::string		extend,
	std::string		stl_format,
	ID_FORMAT	id_format
){
#ifdef DEBUG
	PL_DBGOSH << "Polylib::save_with_rankno() in. " << std::endl;
#endif
	char	my_extend[128];

	// 拡張文字列がカラであれば、現在時刻から作成
	if (extend == "") {
		time_t		timer = time(NULL);
		struct tm	*date = localtime(&timer);
		sprintf(my_extend, "%04d%02d%02d%02d%02d%02d",
			date->tm_year+1900, date->tm_mon+1, date->tm_mday,
			date->tm_hour,      date->tm_min,   date->tm_sec);
	}
	else {
		sprintf(my_extend, "%s", extend.c_str());
	}

	// ランク番号の整形
	char	rank_no[16];
	int		fig = (int)log10((double)maxrank) + 1;
	sprintf(rank_no, "%0*d", fig, myrank);

#ifdef DEBUG
	PL_DBGOSH << "Polylib::save_with_rankno() rank_no" << rank_no<< std::endl;
#endif

	std::map<std::string,std::string> stl_fname_map;
	// STLファイルとIDファイルの保存
	typename std::vector<PolygonGroup<T>*>::iterator	it;
	for (it = m_pg_list.begin(); it != m_pg_list.end(); it++) {
		POLYLIB_STAT	stat;
		//リーフのみがポリゴン情報を持っている
		if ((*it)->get_children().empty() == false) continue;

		// ポリゴン数が0ならばファイル出力不要 2010.10.19
		if ((*it)->get_triangles()->size() == 0) continue;

		//stat = (*it)->save_stl_file(rank_no, my_extend, stl_format);
		stat = (*it)->save_stl_file(rank_no, my_extend, stl_format,stl_fname_map);
		if (stat != PLSTAT_OK)	return stat;
		stat = (*it)->save_id_file(rank_no, my_extend, id_format);
		if (stat != PLSTAT_OK)	return stat;
		std::string rank_string,my_extend_string;
		rank_string=rank_no;
		my_extend_string = my_extend;
		stat = (*it)->mk_param_tag(tp, "", "", "");
		if (stat != PLSTAT_OK) return stat;
	  
	}
	tp->changeNode("/"); //
	//	cout << "before cleanfilepath" <<std::endl;
	clearfilepath(tp); //clear whole file path
	//	cout << "after cleanfilepath" <<std::endl;

	//	cout << "before setfilepath" <<std::endl;
	setfilepath(stl_fname_map);
	//cout << "after setfilepath" <<std::endl;

	// 定義ファイルの保存	


	char	*config_name = save_config_file(rank_no, my_extend, stl_format);
#ifdef DEBUG	
	PL_DBGOSH << __FUNCTION__ << " config_name "<< config_name << std::endl;
#endif 
#ifdef DEBUG
	PL_DBGOSH << "Polylib::save_with_rankno() config_name " << config_name << std::endl;
#endif
	if (config_name == NULL)	return PLSTAT_NG;
	else	*p_config_name = std::string(config_name);
	return PLSTAT_OK;

}

/////////////////////////////////////////　　
// 追加　２０１２ー０８
//////////////////////////////////
template <typename T>
POLYLIB_STAT Polylib<T>::setfilepath(std::map<std::string,std::string>& stl_fname_map){

#ifdef DEBUG
  PL_DBGOS << "stl_map size " <<  stl_fname_map.size()<<std::endl;
#endif //DEBUG
  //  tp->changeNode("/");
  tp->changeNode("/Polylib");
  for(std::map<std::string,std::string>::iterator map_iter=stl_fname_map.begin();
      map_iter != stl_fname_map.end();
      map_iter++){
#ifdef DEBUG
    PL_DBGOS << "stl_map " <<  map_iter->first <<" "<< map_iter->second<<std::endl;
#endif // DEBUG
    tp->changeNode(map_iter->first); //
    std::string cur;
    tp->currentNode(cur);
    
    //    cout << __FUNCTION__ << " " <<  cur << std::endl;
    std::string value = "\"" + map_iter->second + "\"";
    //    cout << "before leaf createion" <<std::endl;
    tp->createLeaf("filepath",value);
    //    cout << "after leaf createion" <<std::endl;
    //check
    //    std::vector<std::string> leaves;
    //   tp->getLabels(leaves);  //label　の取り直し
    //    for(std::vector<std::string>::iterator leaf_iter=leaves.begin();
    //	leaf_iter!=leaves.end();
    //	leaf_iter++){
    //      cout << __FUNCTION__ << " "  << *leaf_iter << std::endl;
    //    }
    //  	  std::string value;
    //    tp->getValue("filepath",value);
    //    cout << __FUNCTION__ << " " << cur << " "<< value <<std::endl;

    //    tp->changeNode("/");

    tp->changeNode("/Polylib");
  }
  return PLSTAT_OK;
}
/////////////////////////////////////////　　
// 追加　２０１２ー０８
//////////////////////////////////
template <typename T>
POLYLIB_STAT Polylib<T>::clearfilepath(TextParser* tp_ptr){

  // recursive にするため、TextParserのポインタを引数に取る。

  std::vector<std::string> leaves;
  tp_ptr->getLabels(leaves,1);

  std::vector<std::string>::iterator leaf_iter=find(leaves.begin(),leaves.end(),"filepath");
  if(leaf_iter!=leaves.end()){ // 見つかったら
    tp_ptr->deleteLeaf("filepath");
  } 
  leaves.clear();
  tp_ptr->getLabels(leaves);


  leaf_iter=find(leaves.begin(),leaves.end(),"filepath[0]");
  if(leaf_iter!=leaves.end()){ // 見つかったら

    int index=0;
    leaf_iter = leaves.begin();
    while(leaf_iter != leaves.end()){
      std::stringstream ss;
      std::string tmpstring="filepath";
      ss << tmpstring <<"["<<index<<"]";
      ss >> tmpstring;
      leaf_iter = find(leaf_iter,leaves.end(),tmpstring);
      if(leaf_iter == leaves.end()) break;
      TextParserError tp_error=tp_ptr -> deleteLeaf(tmpstring);
      if (tp_error!=TP_NO_ERROR) {
	PL_ERROSH << "[ERROR]Polylib::save() "
		  << "can not remove leaf = " << tmpstring << std::endl;
	return PLSTAT_NG;
      }
      index++;
      leaf_iter++;
    }
  }

  std::vector<std::string> nodes;
  tp_ptr->getNodes(nodes);
  for( std::vector<std::string>::iterator node_iter=nodes.begin();
       node_iter!=nodes.end();
       node_iter++){
    tp_ptr->changeNode(*node_iter);
    clearfilepath(tp_ptr);
    tp_ptr->changeNode("..");
  }

  return PLSTAT_OK;

}



// protected //////////////////////////////////////////////////////////////////
// 2010.10.20 引数FILE *追加。
template <typename T>
void Polylib<T>::show_group_name(
	PolygonGroup<T>	*pg, 
	std::string			tab,
	FILE			*fp
){
  typename std::vector<PolygonGroup<T>*>::iterator it;

	// ユーザ定義id出力 2010.10.20
	// ユーザ定義ラベル出力 2012.08.31
	// ユーザ定義タイプ出力 2013.07.17
	if (fp == NULL) {
		PL_DBGOSH << "Polylib::show_group_name: " << tab ;
		if (pg->get_parent_path().empty() == true)  PL_DBGOS << "+";
		PL_DBGOS << pg->get_name() << ":" << pg->acq_file_name() << ":"
				 << pg->get_id() << ":" << pg->get_label() << ":"
				 << pg->get_type() << std::endl;

	}
	else {
		// 出力先が指定されていれば、そちらに出力
		fprintf(fp, "%sPolylib::show_group_name:%s%s%s:%s:%d:%s:%s\n", 
			gs_rankno.c_str(),		tab.c_str(),
			(pg->get_parent_path().empty() == true) ? "+" : "",
			pg->get_name().c_str(),	pg->acq_file_name().c_str(),
			pg->get_id(), pg->get_label().c_str(), pg->get_type().c_str());
	}

	tab = tab + "    ";
	for (it = pg->get_children().begin(); it != pg->get_children().end(); it++){
		show_group_name(*it, tab, fp);
	}
}

// protected //////////////////////////////////////////////////////////////////
template <typename T>
PolygonGroup<T>* Polylib<T>::get_group(int internal_id) const
{
#ifdef DEBUG
	PL_DBGOSH << "Polylib::get_group(" << internal_id << ") in." << std::endl;
#endif
	typename std::vector<PolygonGroup<T>*>::const_iterator it;
	for (it = m_pg_list.begin(); it != m_pg_list.end(); it++) {
		if (internal_id == (*it)->get_internal_id()) {
			return *it;
		}
	}
#ifdef DEBUG
	PL_DBGOS << "Polylib::get_group(" << internal_id << ") returns NULL" << std::endl;
#endif
	return NULL;
}

// private ////////////////////////////////////////////////////////////////////
template <typename T>
std::vector<PrivateTriangle<T>*>* Polylib<T>::search_polygons(
	std::string			group_name, 
	Vec3<T>			min_pos, 
	Vec3<T>			max_pos,
	bool			every,
	bool			linear, 
	POLYLIB_STAT	*ret
) const {
#ifdef DEBUG
	PL_DBGOSH << "Polylib::search_polygons() in." << std::endl;
#endif
	std::vector<PrivateTriangle<T>*>* tri_list = new std::vector<PrivateTriangle<T>*>;
	PolygonGroup<T>* pg = get_group(group_name);

	if (pg == 0) {
		PL_ERROSH << "[ERROR]Polylib::search_polygons():Group not found: " 
				  << group_name << std::endl;
		*ret = PLSTAT_GROUP_NOT_FOUND;
		return tri_list;
	}
	std::vector<PolygonGroup<T>*>* pg_list2 = new std::vector<PolygonGroup<T>*>;

#ifdef BENCHMARK
	double st1, st2, ut1, ut2, tt1, tt2;
	bool ret1, ret2;
	ret1 = getrusage_sec(&ut1, &st1, &tt1);
#endif

	//子孫を検索
	search_group(pg, pg_list2);

	//自身を追加
	pg_list2->push_back(pg);

	// 検索範囲
	BBox<T> bbox;
	bbox.init();
	bbox.add(min_pos);
	bbox.add(max_pos);

	//全ポリゴングループを検索
	typename std::vector<PolygonGroup<T>*>::iterator it;
	for (it = pg_list2->begin(); it != pg_list2->end(); it++) {

		//リーフ構造からのみ検索を行う
		if ((*it)->get_children().size()==0) {
			POLYLIB_STAT ret2;
#ifdef DEBUG
			if (linear == false) {
#endif
				ret2 = (*it)->search (&bbox,every,tri_list);
				if (ret2 != PLSTAT_OK) {
					*ret = ret2;
					return tri_list;
				}
#ifdef DEBUG
			}
			else{
				ret2 = (*it)->linear_search (&bbox,every,tri_list);
				if (ret2 != PLSTAT_OK) {
					*ret = ret2;
					return tri_list;
				}
			}
#endif
		}
	}
#ifdef BENCHMARK
	ret2 = getrusage_sec(&ut2,&st2,&tt2);
	if (ret1 == false || ret2 == false) {
		PL_DBGOSH << "Search SYS   Time Error" << std::endl;
		PL_DBGOSH << "Search CPU   Time Error" << std::endl;
		PL_DBGOSH << "Search Total Time Error" << std::endl;
	}
	else{
		cout.setf(ios::scientific, ios::floatfield);
		PL_DBGOSH << "Search SYS   Time:" << st2 - st1 << std::endl;
		PL_DBGOSH << "Search CPU   Time:" << ut2 - ut1 << std::endl;
		PL_DBGOSH << "Search Total Time:" << tt2 - tt1 << std::endl;
		std::cout.unsetf(ios::scientific);
	}
#endif

	delete pg_list2;
	*ret = PLSTAT_OK;
	return tri_list;
}

// private ////////////////////////////////////////////////////////////////////
template <typename T>
void Polylib<T>::search_group(
	PolygonGroup<T>			*p, 
	std::vector<PolygonGroup<T>*>	*pg
) const {
#ifdef DEBUG
	PL_DBGOSH << "Polylib::search_group() in." << std::endl;
#endif
	typename std::vector<PolygonGroup<T>*>::iterator it;
	for (it = p->get_children().begin(); it != p->get_children().end(); it++) {
		pg->push_back(*it);
		search_group(*it, pg);
	}
}




} //namespace PolylibNS

#endif // polylib_h
