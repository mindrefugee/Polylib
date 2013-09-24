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

#ifndef polylib_polygongroup_h
#define polylib_polygongroup_h


#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <map>
#include <sstream>

#include "polygons/Triangle.h"
#include "common/PolylibStat.h"
#include "common/PolylibCommon.h"
#include "TextParser.h"
#include "polygons/TriMesh.h"
#include "file_io/TriMeshIO.h"
#include "file_io/triangle_id.h"


namespace PolylibNS {

template <typename T> class TryMesh;

template <typename T> class Polylib;

template <typename T> class Polygons;
class PolylibMoveParams;

////////////////////////////////////////////////////////////////////////////
///
/// クラス:PolygonGroup
/// ポリゴングループを管理するクラスです。
///
////////////////////////////////////////////////////////////////////////////
template <typename T>
class PolygonGroup {
public:
	///
	/// コンストラクタ
	///
	PolygonGroup();

	///
	/// コンストラクタ
	///
	/// @param[in] tolerance 頂点同一性チェックの基準値（距離）
	PolygonGroup(T tolerance);

	///
 	/// デストラクタ
	///
	virtual ~PolygonGroup();

	///
	/// 引数で与えられる三角形ポリゴンリストを複製し、KD木の生成を行う。
	///
	///  @param[in] tri_list	設定する三角形ポリゴンリスト。
	///  @param[in] clear		true:ポリゴン複製、面積計算、KD木生成を行う。
	///  						false:面積計算、KD木生成だけを行う。
	///  @return	POLYLIB_STATで定義される値が返る。
	///  @attention TriMeshクラスのinit()参照。オーバーロードメソッドあり。
	///
	POLYLIB_STAT init(
		const std::vector<PrivateTriangle<T>*>		*tri_list, 
		bool									clear = true
	);


	///
	/// 引数で与えられる三角形ポリゴンリストを複製し、KD木の生成を行う。
	///
	///  @param[in] vertlist 設定する三角形ポリゴン頂点リスト。
	///  @param[in] idlist 三角形のid。
	///  @param[in] n_start_tri vertlistの頂点開始位置
	///  @param[in] n_start_id idlistのid開始位置
	///  @param[in] n_tri 加える三角形の数
	///  @return	POLYLIB_STATで定義される値が返る。
	///  @attention TriMeshクラスのinit()参照。オーバーロードメソッドあり。
	
	POLYLIB_STAT init(const T* vertlist,
			  const int* idlist,
			  const int n_start_tri,
			  const int n_start_id,
			  const unsigned int n_tri);




	///
	/// PolygonGroupツリーの作成。
	/// 設定ファイルの内容を再帰的に呼び出し、PolygonGroupツリーを作成する。
	///
	///  @param[in] polylib		Polygonクラスのインスタンス
	///  @param[in] parent		親グループ
	///  @param[in] tp 　　　　　　　　　　　　TextParser のインスタンス
	///  @return	POLYLIB_STATで定義される値が返る。
	///
	virtual POLYLIB_STAT build_group_tree(
		Polylib<T>					*polylib,
		PolygonGroup<T>			*parent,
		TextParser* tp
	);

	///
	/// 三角形ポリゴンの法線ベクトルの計算、面積の計算、KD木の生成を行う。
	/// 三角形ポリゴンはTriMeshクラスが管理している。
	///
	///  @return	POLYLIB_STATで定義される値が返る。
	///  @attention	TriMeshクラスのbuild()参照。
	///
	POLYLIB_STAT build_polygon_tree();

	///
	/// STLファイルからポリゴン情報を読み込み、TriMeshクラスに登録する。
	///
	///  @return POLYLIB_STATで定義される値が返る。
	///  @attention TriMeshクラスのimport()参照。
	///
	POLYLIB_STAT load_stl_file(T scale=1.0);

	///
	/// 三角形ポリゴンIDファイルからポリゴンIDを読み込み、m_internal_idに登録する。
	///
	///  @param[in] id_format	三角形IDファイルの入力形式。
	///  @return	POLYLIB_STATで定義される値が返る。
	///
	POLYLIB_STAT load_id_file(
		ID_FORMAT		id_format
	);

	///
	/// TriMeshクラスが管理しているポリゴン情報をSTLファイルに出力する。
	/// TextParser 対応版
	///  @param[in] rank_no	ファイル名に付加するランク番号。
	///  @param[in] extend	ファイル名に付加する自由文字列。
	///  @param[in] format	STLファイルフォーマット。
	///  @param[in,out] stl_fname_map stl ファイル名とポリゴングループのパス
	///  @return	POLYLIB_STATで定義される値が返る。
	///  @attention TriMeshIOクラスのsave()参照。オーバーロードメソッドあり。
	///
	POLYLIB_STAT save_stl_file(
		std::string		rank_no,
		std::string		extend,
		std::string		format,
		std::map<std::string,std::string>& stl_fname_map
	);

	///
	/// 三角形ポリゴンIDファイルにポリゴンIDを出力する。IDファイル名は、
	/// 階層化されたグループ名_ランク番号_自由文字列.id。
	///
	///  @param[in] rank_no		ファイル名に付加するランク番号。
	///  @param[in] extend		ファイル名に付加する自由文字列。
	///  @param[in] id_format	三角形IDファイルの出力形式。
	///  @return	POLYLIB_STATで定義される値が返る。
	/// 
	POLYLIB_STAT save_id_file(
		std::string 	rank_no,
		std::string		extend,
		ID_FORMAT		id_format
	);

	
	///
	/// 設定ファイルに出力するTextParserのリーフを編集する.
	/// デフォルトでは何もしない。
	/// CarGroup.cxx の例を参照.
	///
	///  @param[in] pointer to TextParser 
	///  @param[in] rank_no	ファイル名に付加するランク番号。
	///  @param[in] extend	ファイル名に付加する自由文字列。
	///  @param[in] format	STLファイルフォーマット。
	///  @return	POLYLIB_STATで定義される値が返る。
	///  @attention  do nothing by default
	///

	virtual POLYLIB_STAT mk_param_tag(
					  TextParser* pt,
		std::string		rank_no,
		std::string		extend,
		std::string		format
	);


	///
	/// 三角形ポリゴン移動メソッド。virtual用の関数なので処理はない。
	///
	///  @param[in] params	Polylib.hで宣言しているパラメタセットクラス。
	///  @return	POLYLIB_STATで定義される値が返る。
	///
	virtual POLYLIB_STAT move(
		PolylibMoveParams	&params
	);

	///
	/// KD木探索により、指定矩形領域に含まれるポリゴンを抽出する。
	///
	///  @param[in] bbox	矩形領域。
	///  @param[in]	every	true:3頂点が全て検索領域に含まれるものを抽出。
	///  					false:1頂点でも検索領域に含まれるものを抽出。
	///  @return	抽出したポリゴンリストのポインタ。
	///  @attention オーバーロードメソッドあり。
	///
	const std::vector<PrivateTriangle<T>*>* search(
		BBox<T>	*bbox, 
		bool	every
	) const;

	///
	/// KD木探索により、指定矩形領域に含まれるポリゴンを抽出する。
	///
	///  @param[in]		bbox		矩形領域。
	///  @param[in]		every		true:3頂点が全て検索領域に含まれるものを抽出。
	///  							false:1頂点でも検索領域に含まれるものを抽出。
	///  @param[in,out] tri_list	抽出した三角形ポリゴンリストのポインタ。
	///  @return	POLYLIB_STATで定義される値が返る。
	///  @attention オーバーロードメソッドあり。
	///
	POLYLIB_STAT search(
		BBox<T>							*bbox, 
		bool							every, 
		std::vector<PrivateTriangle<T>*>	*tri_list
	) const;

	///
	/// 線形探索により、指定矩形領域に含まれるポリゴンを抽出する。
	///
	///  @param[in] bbox	矩形領域。
	///  @param[in]	every	true:3頂点が全て検索領域に含まれるものを抽出。
	///  					false:1頂点でも検索領域に含まれるものを抽出。
	///  @return	抽出したポリゴンリストのポインタ。
	///  @attention	オーバーロードメソッドあり。
	///
	const std::vector<PrivateTriangle<T>*>* linear_search(
		BBox<T>	*bbox, 
		bool	every
	) const;

	///
	/// 線形探索により、指定矩形領域に含まれるポリゴンを抽出する。
	///
	///  @param[in]		bbox		 矩形領域。
	///  @param[in]		every	 	true:3頂点が全て検索領域に含まれるものを抽出。
	///  						 	false:1頂点でも検索領域に含まれるものを抽出。
	///  @param[in,out] tri_list	抽出した三角形ポリゴンリストのポインタ。
	///  @return	POLYLIB_STATで定義される値が返る。
	///  @attention	オーバーロードメソッドあり。
	///
	POLYLIB_STAT linear_search(
		BBox<T>							*bbox, 
		bool							every, 
		std::vector<PrivateTriangle<T>*>	*tri_list
	) const;

	/// 
	/// KD木探索により、指定位置に最も近いポリゴンを検索する。
	///
	///  @param[in]     pos     指定位置
	///  @return    検索されたポリゴン
	///
	const PrivateTriangle<T>* search_nearest(
		const Vec3<T>&    pos
	) const;

	///
	/// PolygonGroupのフルパス名を取得する。
	///
	///  @return フルパス名。
	///
	std::string acq_fullpath();

	///
	/// カンマ区切りでSTLファイル名リストを取得。
	///
	///  @return ファイル名リスト。
	///
	std::string acq_file_name();

	///
	/// PE領域間移動する三角形ポリゴンリストの取得。
	///
	///  @param[in]	neibour_bbox		隣接PE領域バウンディングボックス。
	///  @param[in]	exclude_tria_ids	領域移動対象外三角形IDリスト。
	///  @return	検索結果三角形リスト。
	///
	const std::vector<PrivateTriangle<T>*>* search_outbounded(
		BBox<T>				neibour_bbox,
		std::vector<int>	*exclude_tria_ids
	);

	///
	/// 三角形リストの追加。
	///
	///  @param[in]	tri_list	三角形ポリゴンリストのポインタ。
	///  @return	POLYLIB_STATで定義される値が返る。
	///  @attention	三角形IDが重複した三角形は追加しない。KD木の再構築はしない。
	///
	POLYLIB_STAT add_triangles(
	   std::vector<PrivateTriangle<T>*>	*tri_list
	);



	///
	/// 三角形リストの追加。
	///
	///  @param[in] vertlist 設定する三角形ポリゴン頂点リスト。
	///  @param[in] idlist 三角形のid。
	///  @param[in] n_start_tri vertlistの頂点開始位置
	///  @param[in] n_start_id idlistのid開始位置
	///  @param[in] n_tri 加える三角形の数
	///  @return	POLYLIB_STATで定義される値が返る。
	///  @attention TriMeshクラスのadd()参照。オーバーロードメソッドあり。
	POLYLIB_STAT add_triangles(	
				   const T* vertlist,
				   const int* idlist,
				   const int n_start_tri,
				   const int n_start_id,
				   const unsigned int n_tri);
	

	///
	/// ポリゴン情報を再構築する。（KD木の再構築をおこなう）
	///
	///  @return	POLYLIB_STATで定義される値が返る。
	///
	POLYLIB_STAT rebuild_polygons();

	///
	/// グループ情報（ランク番号、親グループ名、自分のグループ名、ファイル名、
	/// 頂点数、各頂点のXYZ座標値、法線ベクトルのXYZ座標値、面積）を出力する。
	///
	///  @param[in] irank ランク数。
	///  @return	POLYLIB_STATで定義される値が返る。
	///
	POLYLIB_STAT show_group_info(
		int irank = -1
	);

  // add keno 20120331
  /// ポリゴングループの要素数を返す
  int get_group_num_tria( void );
  
  /// ポリゴンの面積を積算して返す
  T get_group_area( void );

  /// ポリゴンの縮尺変換＆KD木再構築
  POLYLIB_STAT rescale_polygons( T scale );

	///
	/// グループ配下の全Triangleオブジェクトのm_exidを更新する。
	///
	/// @param[in] id 更新するID番号。
	/// @return    ステータスコード。
	///
	POLYLIB_STAT set_all_exid_of_trias(
		int id
	);

	//=======================================================================
	// Setter/Getter
	//=======================================================================
	///
	/// クラス名を取得。
	///
	/// @return		クラス名。
	/// @attention	本クラスを継承する場合、継承後のクラス名を返すように変更す
	/// 			ることる。
	///
	static std::string get_class_name() {return "PolygonGroup";};

	///
	/// クラス名を取得。
	///
	/// @return		クラス名。
	/// @attention	継承するクラスのクラス名取得関数get_class_name()を呼び出す。
	///
	virtual std::string whoami()		{return get_class_name();};

	///
	/// STLファイル名とファイルフォーマットを設定。
	///
	///  @param[in] fname STLファイル名とファイルフォーマットの対応マップ。
	///
	void set_file_name(std::map<std::string, std::string> fname) {
		m_file_name = fname;
	}

	///
	/// STLファイル名とファイルフォーマットの対応マップ取得。
	///
	///  @return STLファイル名とファイルフォーマットの対応マップ。
	///
	std::map<std::string, std::string> get_file_name() const {
		return m_file_name;
	}

	///
	/// グループ名を設定。
	///
	/// @param[in] name グループ名。
	///
	void set_name(std::string name){
		m_name = name;
	}

	///
	/// グループ名を取得。
	///
	/// @return グループ名。
	///
	std::string get_name(void){
		return m_name;
	}

	///
	/// 親グループのフルパス名を設定。
	///
	/// @param[in] ppath 親グループのフルパス名。
	///
	void set_parent_path(std::string ppath){
		m_parent_path = ppath;
	}

	///
	/// 親グループのフルパス名を取得。
	///
	/// @return 親グループのフルパス名。
	///
	std::string get_parent_path(void){
		return m_parent_path;
	}

	///
	/// 親グループを設定。
	///
	/// @param[in] p 親グループのポインタ。
	///
	void set_parent(PolygonGroup<T>* p) {
		m_parent = p;
	}

	///
	/// 親グループを取得。
	///
	/// @return 親グループのポインタ。
	///
	PolygonGroup<T>* get_parent(void) {
		return m_parent;
	}

	///
	/// 子グループを設定。
	///
	/// @param[in] p	子グループのリスト。
	///
	void set_children(std::vector<PolygonGroup<T>*>& p) {
		m_children = p;
	}

	///
	/// 子グループを取得。
	///
	/// @return 子グループのリスト。
	///
	std::vector<PolygonGroup<T>*>& get_children(void) {
		return m_children;
	}

	///
	/// 子グループを追加。
	///
	/// @param[in] p	子グループ。
	///
	void add_children(PolygonGroup<T>* p) {
		m_children.push_back(p);
	}

	///
	/// Polygonクラスが管理する頂点リストを取得。
	///
	/// @return  頂点リスト
	///
	VertexList<T>* get_vertexlist() {
		return m_polygons->get_vtx_list();
	}

	///
	/// Polygonクラスが管理するKD木クラスを取得。
	///
	/// @return KD木ポリゴンリスト。
	///
	VertKDT<T> *get_vertkdt() {
		return m_polygons->get_vertkdt();
	}
	///
	/// Polygonクラスが管理する三角形ポリゴンリストを取得。
	///
	/// @return 三角形ポリゴンリスト。
	///
	std::vector<PrivateTriangle<T>*>* get_triangles() {
		return m_polygons->get_tri_list();
	}

	///
	/// Polygonクラスが管理するKD木クラスを取得。
	///
	/// @return KD木ポリゴンリスト。
	///
	VTree<T> *get_vtree() {
		return m_polygons->get_vtree();
	}

	///
	/// ポリゴングループIDを取得。
	/// メンバー名修正( m_id -> m_internal_id) 2010.10.20
	///
	///  @return ポリゴングループID。
	///
	int get_internal_id() {
		return m_internal_id;
	}

	///
	/// ユーザ定義ラベルを取得。
	/// 追加 2012.08.31
	///
	///  @return ユーザ定義ラベル。
	///
	std::string get_label() {
		return m_label;
	}

	///
	/// ユーザ定義タイプを取得。
	/// 追加 2013.07.17
	///
	///  @return ユーザ定義タイプ。
	///
	std::string get_type() {
		return m_type;
	}

	///
	/// ユーザ定義IDを取得。
	/// 追加 2010.10.20
	///
	///  @return ユーザ定義ID。
	///
	int get_id() {
		return m_id;
	}

	///
	/// 移動対象フラグを取得。
	///
	///  @return 移動対象フラグ。
	///
	int get_movable() {
		return m_movable;
	}

	///
	/// move()による移動前三角形一時保存リストの個数を取得。
	///
	///  @return 一時保存リストサイズ。
	///
	size_t get_num_of_trias_before_move() {
		if (m_trias_before_move == NULL)	return 0;
		else	return m_trias_before_move->size();
	}


	///
	/// test function for Vertex test
	/// 
	void print_vertex() const{
	  m_polygons->print_vertex();
	}

	void show_bbox() const{
	  BBox<T> bbox=m_polygons->get_bbox();
	  Vec3<T> min=bbox.getPoint(0);
	  Vec3<T> max=bbox.getPoint(7);
	  std::cout<< __func__ << " min " <<min<<std::endl;
	  std::cout<< __func__ << " max " <<max<<std::endl;

	}

	///
	/// configファイルに記述するParamタグのクラス名(value="...")。
	///
	//	static const char *ATT_NAME_config;
		static const char *ATT_NAME_CLASS;

	///
	/// 頂点同一性の基準値　configファイルに記述するParamタグ(value="...")。
	///
	static const char *ATT_NAME_TOLERANCE;



protected:

	///
	/// 設定ファイルから取得したPolygonGroupの情報をインスタンスにセットする。
	/// 
	/// "filepath" に関して、先にfilepathが複数　(filepath[0])が存在するかどうか
	///  をチェックして、複数ならばその処理を行い、filepath の処理は終了する。
	///  複数でないことが分かったら、filepath が単体で存在するかをチェックして、
	///  存在するならば、処理を行う。
	///
	///  @param[in] polylib		Polygonクラスのインスタンス。
	///  @param[in] parent		親グループ。
	///  @param[in] tp              TextParserクラスのインスタンス
	///  @return	POLYLIB_STATで定義される値が返る。
	///
	POLYLIB_STAT setup_attribute (
		Polylib<T>					*polylib,
		PolygonGroup<T>			*parent,
		TextParser *tp
	);

	///
	/// move()メソッド実行により、頂点が隣接セルよりも遠くへ移動した三角形情報
	/// を報告（前処理）。
	///
	///  @return	POLYLIB_STATで定義される値が返る。
	///  @attention 本メソッドはデバッグ用です。
	///  			派生クラスでオーバーライドしたmove()メソッド内で、座標移動
	///				処理前に呼ぶこと。
	///
	POLYLIB_STAT init_check_leaped();

	///
	/// move()メソッド実行により、頂点が隣接セルよりも遠くへ移動した三角形情報
	/// を報告（後処理）。該当する三角形について、以下の情報をcerrへ出力する。
	///		・ポリゴングループID
	///		・三角形ID
	///		・移動前/後の頂点座標
	///
	///  @param[in] origin		計算領域起点座標
	///  @param[in] cell_size	ボクセルサイズ
	///  @return	POLYLIB_STATで定義される値が返る。
	///  @attention	本メソッドはデバッグ用です。
	///  			派生クラスでオーバーライドしたmove()メソッド内で、座標移動
	///				処理後に呼ぶこと。
	///
	POLYLIB_STAT check_leaped(
		Vec3<T> origin,
		Vec3<T> cell_size
	);

	///
	/// 2点が隣接ボクセルよりも離れているか？
	///
	///  @param[in] origin		計算領域起点座標。
	///  @param[in] cell_size	ボクセルサイズ。
	///  @param[in] pos1			点(1)。
	///  @param[in] pos2			点(2)。
	///  @return	true:2点が隣接ボクセルよりも離れている。
	///
	bool is_far(
		Vec3<T> origin,
		Vec3<T> cell_size,
		Vec3<T> pos1,
		Vec3<T> pos2
	);




private:
	///
	/// STLファイル名を作成。ファイル名は、以下の通り。
	/// グループ名のフルパス_ランク番号_自由文字列.フォーマット文字列。
	///
	///  @param[in] rank_no	ファイル名に付加するランク番号。
	///  @param[in] extend	ファイル名に付加する自由文字列。
	///  @param[in] format	STLファイルフォーマット。
	///  @return	POLYLIB_STATで定義される値が返る。
	///
	char *mk_stl_fname(
		std::string		rank_no,
		std::string		extend,
		std::string		format
	);


	///
	/// STLファイル名を作成。ファイル名は、以下の通り。
	/// グループ名のフルパス_ランク番号_自由文字列.フォーマット文字列。
	/// TextParser 対応版
	///
	///  @param[in] rank_no	ファイル名に付加するランク番号。
	///  @param[in] extend	ファイル名に付加する自由文字列。
	///  @param[in] format	STLファイルフォーマット。
	///  @param[in,out] stl_fname_map stl ファイル名とポリゴングループのパス
	///  @return	POLYLIB_STATで定義される値が返る。
	///
	char *mk_stl_fname(
		std::string		rank_no,
		std::string		extend,
		std::string		format,
		std::map<std::string,std::string>& stl_fname_map
	);

	///
	/// 三角形ポリゴンIDファイル名を作成。ファイル名は、以下の通り。
	/// グループ名のフルパス_ランク番号_自由文字列.id。
	///
	///  @param[in] rank_no	ファイル名に付加するランク番号。
	///  @param[in] extend	ファイル名に付加する自由文字列。
	///  @return	POLYLIB_STATで定義される値が返る。
	///
	char *mk_id_fname(
		std::string		extend,
		std::string		rank_no
	);

	///
	/// 全PolygonGroupに一意のグループIDを作成する。
	///
	///  @return	グループID。
	///
	int create_global_id();
 

protected:
	//=======================================================================
	// クラス変数
	//=======================================================================
	/// グループID。
	// メンバー名修正( m_id -> m_internal_id) 2010.10.20
	int					m_internal_id;

	/// 自グループ名。
	std::string			m_name;

	/// 親グループのパス名。
	std::string			m_parent_path;

	/// 親グループへのポインタ。
	PolygonGroup<T>			*m_parent;

	/// 子グループへのポインタリスト。
	std::vector<PolygonGroup<T>*>	m_children;

	/// STLファイル名とファイル形式。
	std::map<std::string, std::string>	m_file_name;

	/// 三角形Polygonsクラス。
	Polygons<T>				*m_polygons;

	/// moveメソッドにより移動するグループか？
	bool					m_movable;

	/// KD木の再構築が必要か？
	bool					m_need_rebuild;

	/// move()による移動前三角形一時保存リスト。
	std::vector<PrivateTriangle<T>*>		*m_trias_before_move;
	
	/// ユーザ定義ラベル : (追加 2012.08.31)
	std::string							m_label;

	/// ユーザ定義タイプ : (追加 2013.07.17)
	std::string							m_type;

private:
	/// ユーザ定義id : (追加 2010.10.20)
	int							m_id;

	/// ユーザ定義IDが設定されたか？：（追加 2013.06.17)
	bool						m_id_defined;

	///  頂点同一性チェックの判定基準 (追加 2013.09.03)
	T m_tolerance;
};


///
/// 他クラスでも使用するXMLタグ
///
template <typename T>
const char* PolygonGroup<T>::ATT_NAME_CLASS = "class_name";

template <typename T>
const char* PolygonGroup<T>::ATT_NAME_TOLERANCE = "tolerance";

///
/// 本クラス内でのみ使用するXMLタグ
///
//#define ATT_NAME_NAME		"name"
#define ATT_NAME_PATH		"filepath"
#define ATT_NAME_MOVABLE	"movable"
// ユーザ定義ID追加 2010.10.20
#define ATT_NAME_ID			"id"
// ユーザ定義ラベル追加 2012.08.31
#define ATT_NAME_LABEL		"label"
// ユーザ定義タイプ追加 2013.07.17
#define ATT_NAME_TYPE		"type"

 
// public /////////////////////////////////////////////////////////////////////
template <typename T>
PolygonGroup<T>::PolygonGroup() {
	m_parent	= 0;
	m_polygons	= new TriMesh<T>();
	m_movable	= false;
	m_need_rebuild = false;
	m_trias_before_move = NULL;
}
// public /////////////////////////////////////////////////////////////////////
template <typename T>
PolygonGroup<T>::PolygonGroup(T tolerance) {
	m_parent	= 0;
	m_polygons	= new TriMesh<T>(tolerance);
	m_movable	= false;
	m_need_rebuild = false;
	m_trias_before_move = NULL;
	m_tolerance=tolerance;
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
PolygonGroup<T>::~PolygonGroup()
{
#ifdef DEBUG
	PL_DBGOSH << "Delete PolygonGroup" << std::endl;
#endif
	delete m_polygons;
	if( m_trias_before_move ) {
		for( unsigned int i=0; i<m_trias_before_move->size(); i++ ) {
			delete m_trias_before_move->at(i);
		}
		delete m_trias_before_move;
	}
}
// public /////////////////////////////////////////////////////////////////////
template <typename T>
  POLYLIB_STAT PolygonGroup<T>::init(const T* vertlist,
				     const int* idlist,
				     const int n_start_tri,
				     const int n_start_id,
				     const unsigned int n_tri){

  //#define DEBUG

#ifdef DEBUG
	PL_DBGOSH <<"PolygonGroup::" << __func__<<" start of Polygons::init." << std::endl;
#endif

  
  m_polygons->init(vertlist,idlist,n_start_tri,n_start_id,n_tri);
#ifdef DEBUG
	PL_DBGOSH <<"PolygonGroup::" << __func__<<" end of Polygons::init."  <<std::endl;
#endif

  return build_polygon_tree();
  //#undef DEBUG
}




// public /////////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT PolygonGroup<T>::init(
	const std::vector<PrivateTriangle<T>*>	*tri_list, 
	bool clear
) {
  //#define DEBUG
#ifdef DEBUG
	PL_DBGOSH <<"PolygonGroup::init :clear=" << clear << std::endl;
#endif
	if (clear == true) {
		m_polygons->init(tri_list);

#ifdef DEBUG
	PL_DBGOSH <<"PolygonGroup::init :back from TriMesh::init" <<  std::endl;
#endif

	}
	return build_polygon_tree();
	//#undef DEBUG
}

//TextParser version
// public /////////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT PolygonGroup<T>::build_group_tree(
	Polylib<T>					*polylib,
	PolygonGroup			*parent,
	TextParser* tp
) {
#ifdef DEBUG
	PL_DBGOSH << "PolygonGroup::build_group_tree() in." << std::endl;
#endif

	//current で出来ているPolygonGroupに属性をつける。
	POLYLIB_STAT 	ret = setup_attribute(polylib, parent, tp);
	if (ret != PLSTAT_OK)		return ret;

	// 元コードの説明
	// PolylibCfgElem がなくなるまでループ
	// すでにelem には、情報が読み込まれていて、それを使用する。
	//

	std::vector<std::string> nodes;

	TextParserError error=TP_NO_ERROR;
	error=tp->getNodes(nodes);


	// tp で　情報を読みながら、ループ
	for(std::vector<std::string>::iterator nodes_iter=nodes.begin();
	    nodes_iter != nodes.end();
	    nodes_iter++){
	  
	  error=tp->changeNode(*nodes_iter);
	  if(error!=TP_NO_ERROR){
	    PL_ERROSH << "[ERROR]PolygonGroup::build_group_tree():"
		      << " TextParser error " 
		      << tp->TextParserErrorHandler(error,"can not move to ") 
		      << (*nodes_iter) << std::endl;
	    return PLSTAT_CONFIG_ERROR;
	  }
	  //属性を読む。
	  std::vector<std::string> leaves;
	  error=tp->getLeaves(leaves,1);
	  if(error!=TP_NO_ERROR){
	    PL_ERROSH << "[ERROR]PolygonGroup::build_group_tree():"
		      << " TextParser error " 
		      << tp->TextParserErrorHandler(error,"can not get leaves ") 
		      << (*nodes_iter) << std::endl;
	    return PLSTAT_CONFIG_ERROR;
	  }

	  // class_name をチェック
	  std::string class_name = "PolygonGroup";
	  std::vector<std::string>::iterator leaf_iter=find(leaves.begin(),
						  leaves.end(),
						  ATT_NAME_CLASS);
	  
	  if(leaf_iter!=leaves.end()){
	    std::string value;
	    error=tp->getValue((*leaf_iter),value);
	    class_name=value;
	  }

	  T tolerance=m_tolerance;
	  //TOLERANCE
	  leaf_iter=find(leaves.begin(),
			 leaves.end(),
			 PolygonGroup<T>::ATT_NAME_TOLERANCE);
	    
	  if(leaf_iter != leaves.end()){
	    //     class_name = *leaf_iter;
	    std::string value;
	    //TextPraserError  status=tp->getValue((*leaf_iter),value);
	    tp->getValue((*leaf_iter),value);
	    int ierror;
	    tolerance=tp->convertDouble(value,&ierror);
	  }


	  PolygonGroup* pg;
	  pg = polylib->create_polygon_group(class_name,tolerance);
	  polylib->add_pg_list(pg);	
	  if (pg == NULL) {
	    PL_ERROSH << "[ERROR]PolygonGroup::build_group_tree():"
		      << "Unknown Class name:" << class_name << "." << std::endl;
	    return PLSTAT_CONFIG_ERROR;
	  }

	  ret = pg->build_group_tree(polylib, this, tp);

	  // go up and next
	  tp->changeNode("..");

	}

	return PLSTAT_OK;
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT PolygonGroup<T>::build_polygon_tree()
{
#ifdef BENCHMARK
	double	st1, st2, ut1, ut2, tt1, tt2;
	bool	ret1, ret2;
	ret1 = getrusage_sec(&ut1, &st1, &tt1);
#endif

	//#define DEBUG
#ifdef DEBUG
	PL_DBGOSH << "PolygonGroup::build_polygon_tree() in.:" << m_name << std::endl;
#endif

	//木構造の生成
	POLYLIB_STAT ret = m_polygons->build();
#ifdef DEBUG
	PL_DBGOSH << "PolygonGroup::build_polygon_tree() out."<< ret << std::endl;
#endif


	if (ret != PLSTAT_OK) return ret;

#ifdef DEBUG
	PL_DBGOSH << "PolygonGroup::build_polygon_tree() out." << std::endl;
#endif
#ifdef BENCHMARK
	ret2 = getrusage_sec(&ut2,&st2,&tt2);
	if (ret1 == false || ret2 == false) {
		PL_DBGOSH << "Reading STL SYS Time Error" << std::endl;
		PL_DBGOSH << "Reading STL CPU Time Error" << std::endl;
		PL_DBGOSH << "Reading STL Total Time Error" << std::endl;
	}
	else{
		cerr.setf(ios::scientific, ios::floatfield);
		PL_DBGOSH << "Reading STL SYS   Time:" << st2 - st1 << std::endl;
		PL_DBGOSH << "Reading STL CPU   Time:" << ut2 - ut1 << std::endl;
		PL_DBGOSH << "Reading STL Total Time:" << tt2 - tt1 << std::endl;
		cerr.unsetf(ios::scientific);
	}
#endif
	//#undef DEBUG
	return PLSTAT_OK;
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT PolygonGroup<T>::load_stl_file( T scale )
{
  //#define DEBUG
#ifdef DEBUG
PL_DBGOSH << "PolygonGroup:load_stl_file():IN" << std::endl;
#endif
	POLYLIB_STAT ret = m_polygons->import(m_file_name, scale);

#ifdef DEBUG
PL_DBGOSH << "PolygonGroup:load_stl_file():import finished" << std::endl;
#endif

	if (ret != PLSTAT_OK) return ret;

	// m_idが指定されていたら、その値で全三角形のm_exidを更新
	// added by tkawanab 2013.06.17
	if( m_id_defined ) {
		ret = m_polygons->set_all_exid( m_id );
	}
	if (ret != PLSTAT_OK) return ret;
	
	POLYLIB_STAT result= build_polygon_tree();


#ifdef DEBUG
PL_DBGOSH << "PolygonGroup:load_stl_file():import build_finished" << std::endl;
	m_polygons->print_memory_size();
#endif

	return result;
	//#undef DEBUG
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT PolygonGroup<T>::load_id_file(
	ID_FORMAT	id_format
) {
#ifdef DEBUG
  PL_DBGOSH << "PolygonGroup:load_id_file():IN" << std::endl;
#endif

  // no stl file, no id file.
  if(m_file_name.size() == 0 ){
    return PLSTAT_OK;	
  }

	// IDはsave_id()関数で一括して出力されるので、ファイル数は必ず1個

	if (m_file_name.size() != 1) {
		PL_ERROSH << "[ERROR]PolygonGroup::load_id_file():Num of files mismatch:" 
				  << m_file_name.size() << std::endl;
		return PLSTAT_NG;	
	}
	std::string	fname	= m_file_name.begin()->first;
	int		pos		= fname.find_last_of(".");

	fname.replace(pos + 1, fname.length(), "id");
#ifdef DEBUG
PL_DBGOSH << "load_id_file:" << fname.c_str() << std::endl;
#endif
// ?? return load_id<T>(m_polygons->get_tri_list(), fname, id_format);
 return load_id(m_polygons->get_tri_list(), fname, id_format);
}

// TextParser でのsaveの為、save した stl ファイルを記憶しておく
/////////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT PolygonGroup<T>::save_stl_file(
	std::string	rank_no,
	std::string	extend,
	std::string	format,
	std::map<std::string,std::string>& stl_fname_map
) {
  char	*fname = mk_stl_fname(rank_no, extend, format,stl_fname_map);
  //	return TriMeshIO::save(m_polygons->get_tri_list(), fname, format);
  return TriMeshIO::save<T>( m_polygons->get_vtx_list(),
			   m_polygons->get_tri_list(),
			   fname, format);
}
 
// public /////////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT PolygonGroup<T>::save_id_file(
	std::string		rank_no,
	std::string		extend,
	ID_FORMAT	id_format
) {
	char	*fname = mk_id_fname(rank_no, extend);
#ifdef DEBUG
PL_DBGOSH <<  "save_id_file:" << fname << std::endl;
#endif
	return save_id(m_polygons->get_tri_list(), fname, id_format);
}


//TextParser 版
template <typename T>
POLYLIB_STAT PolygonGroup<T>::mk_param_tag(
					TextParser* tp,
					std::string	rank_no,
					std::string	extend,
					std::string	format
) {

	// virtual用の関数なので中身はない
	return PLSTAT_OK;
}


// public /////////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT PolygonGroup<T>::move(
	PolylibMoveParams	&params
){
#ifdef DEBUG
PL_DBGOSH <<  "PolygonGroup::move() in." << std::endl;
#endif
	// virtual用の関数なので中身はない
	return PLSTAT_OK;
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
const std::vector<PrivateTriangle<T>*>* PolygonGroup<T>::search(
	BBox<T>	*bbox, 
	bool	every
) const {
	return m_polygons->search(bbox, every);
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT PolygonGroup<T>::search(
	BBox<T>						*bbox, 
	bool						every, 
	std::vector<PrivateTriangle<T>*>	*tri_list
) const {
	return m_polygons->search(bbox, every, tri_list);
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
const std::vector<PrivateTriangle<T>*>* PolygonGroup<T>::linear_search(
	BBox<T>	*bbox, 
	bool	every
) const {
	return m_polygons->linear_search(bbox, every);
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT PolygonGroup<T>::linear_search(
	BBox<T>						*bbox, 
	bool						every, 
	std::vector<PrivateTriangle<T>*>	*tri_list
) const {
	return m_polygons->linear_search(bbox, every, tri_list);
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
std::string PolygonGroup<T>::acq_fullpath() {
	if (m_parent_path.empty() == true)	return m_name;
	else								return m_parent_path + "/" + m_name;
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
std::string PolygonGroup<T>::acq_file_name() {
  std::string	fnames;
  std::map<std::string, std::string>::iterator	it;
	for (it = m_file_name.begin(); it != m_file_name.end(); it++) {
		if (it == m_file_name.begin()) {
			fnames = it->first;
		}
		else {
			fnames.append(",");
			fnames.append(it->first);
		}
	}
	return fnames;
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
const std::vector<PrivateTriangle<T>*>*
PolygonGroup<T>::search_outbounded(
	BBox<T> neibour_bbox,
	std::vector<int> *exclude_tria_ids
)
{
#ifdef DEBUG
	PL_DBGOSH << "PolygonGroup::search_outbounded() in. " << std::endl;
#endif
	std::vector<PrivateTriangle<T>*> *p_trias;

	// 除外IDリストを昇順ソート
	std::sort( exclude_tria_ids->begin(), exclude_tria_ids->end() );

	// 隣接PE領域(ガイドセル含)に懸かる三角形を検索
	p_trias = (std::vector<PrivateTriangle<T>*>*)search( &neibour_bbox, false );
#ifdef DEBUG
PL_DBGOSH << "p_trias org num:" << p_trias->size() << std::endl;
#endif

	// 検索結果から除外対象を除く
	typename std::vector<PrivateTriangle<T>*>::iterator itr;
	for( itr=p_trias->begin(); itr!=p_trias->end(); ) {
		int id = (*itr)->get_id();
		if( std::binary_search(exclude_tria_ids->begin(),
							   exclude_tria_ids->end(), id) ) {
			itr = p_trias->erase(itr);
		}
		else {
			itr++;
		}
	}
#ifdef DEBUG
PL_DBGOSH << "p_trias ret num:" << p_trias->size() << std::endl;
#endif
	return p_trias;
}
// public /////////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT
PolygonGroup<T>::add_triangles(
				   const T* vertlist,
				   const int* idlist,
				   const int n_start_tri,
				   const int n_start_id,
				   const unsigned int n_tri){

  if( n_tri==0)	  return PLSTAT_OK;

	m_polygons->add(vertlist, idlist, n_start_tri, n_start_id,n_tri);


	// KD木要再構築フラグを立てる
	m_need_rebuild = true;

	return PLSTAT_OK;
}




// public /////////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT
PolygonGroup<T>::add_triangles(
	std::vector<PrivateTriangle<T>*> *tri_list
)
{
#ifdef DEBUG
	PL_DBGOSH << "PolygonGroup::add_triangles() in. " << std::endl;
#endif
	if( tri_list==NULL || tri_list->size()==0 ) {
		return PLSTAT_OK;
	}

	m_polygons->add( tri_list );

	// KD木要再構築フラグを立てる
	m_need_rebuild = true;

	return PLSTAT_OK;
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT
PolygonGroup<T>::rebuild_polygons()
{
#ifdef DEBUG
	PL_DBGOSH << "PolygonGroup::rebuild_polygons() in. " << std::endl;
#endif
	// 不要な再構築は行わない
	if( !m_need_rebuild ) {
#ifdef DEBUG
		PL_DBGOSH << "PolygonGroup::rebuild_polygons() didnot need rebuild." << std::endl;
#endif
		return PLSTAT_OK;
	}

	POLYLIB_STAT ret = build_polygon_tree();
	m_need_rebuild = false;
	return ret;
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT PolygonGroup<T>::show_group_info(
	int irank
) {
  std::ostringstream	oss;
	std::string			rank;

	if (irank < 0) {
		rank = "";
	}
	else{
	  oss << std::setw(3) << std::setfill('0') << "rank(" << irank << "): ";
		rank = oss.str();
	}
	PL_DBGOSH << "PolygonGroup::show_group_info::rank:" << rank << std::endl;

	if (m_name.empty() == false) {
		PL_DBGOSH << "  polygon group name: " << m_name << std::endl;
	}
	else {
		PL_DBGOSH << "  polygon group name: empty." << std::endl;
	}

	if (m_parent_path.empty() == false) {
		PL_DBGOSH << "  parent polygon group name: " << m_parent_path << std::endl;
	}
	else {
		PL_DBGOSH << "  parent polygon group name: empty." << std::endl;
	}

	if (m_file_name.size() > 0) {
	  typename std::map<std::string, std::string>::iterator it = m_file_name.begin();
		for (; it != m_file_name.end(); it++) 
			PL_DBGOSH << "  file name: " << (*it).first << std::endl;
	}
	else {
		PL_DBGOSH << "  file name: empty." << std::endl;
	}

	if (m_polygons == NULL) {
		PL_ERROSH << "[ERROR]PolygonGroup::show_group_info():Polygon is nothing:"
				  << std::endl;
		return PLSTAT_POLYGON_NOT_EXIST;
	}
	if (m_polygons->get_tri_list() == 0) {
		PL_ERROSH << "[ERROR]PolygonGroup::show_group_info():Triangle is nothing:"
				  << std::endl;
		return PLSTAT_TRIANGLE_NOT_EXIST;
	}

	std::vector<PrivateTriangle<T>*>* tmp_list = m_polygons->get_tri_list();

	PL_DBGOSH << "  triangle list size: " << tmp_list->size() << std::endl;
	PL_DBGOSH << "  vertex vector list: " << std::endl;
	typename std::vector<PrivateTriangle<T>*>::iterator it;
	for (it = tmp_list->begin(); it != tmp_list->end(); it++) {
	  //Vec3<T>* vtx[3] = (*it)->get_vertex();
	  Vertex<T>** vtx = (*it)->get_vertex();
	  for (int i=0; i<3; i++) {
	    //	    PL_DBGOSH << "    id:" << i		   << " x:" << (*vtx)[i][0]
	    //<< " y:"	  << (*vtx)[i][1] << " z:" << (*vtx)[i][2] << std::endl;
	    PL_DBGOSH << "    id:" << i		   << " x:" << (*(vtx[i]))[0]
		      << " y:"	  << (*(vtx[i]))[1] << " z:" << (*(vtx[i]))[2] << std::endl;

	  }

		// Vec3<T> *vtx = (*it)->get_vertex();
		// for (int i=0; i<3; i++) {
		//   PL_DBGOSH << "    id:" << i		   << " x:" << vtx[i][0]
		// 	    << " y:"	  << vtx[i][1] << " z:" << vtx[i][2] << std::endl;
		// }
	}

	PL_DBGOSH << "  normal vector list: " << std::endl;
	for (it = tmp_list->begin(); it != tmp_list->end(); it++) {
		Vec3<T> vtx = (*it)->get_normal();
		PL_DBGOSH << "    x:" << vtx[0] << " y:" << vtx[1] << " z:" << vtx[2] <<std::endl;
	}

	PL_DBGOSH << "  triangle area list: " << std::endl;
	for (it = tmp_list->begin(); it != tmp_list->end(); it++) {
		PL_DBGOSH << "    area:" << (*it)->get_area() << std::endl;
	}
	return PLSTAT_OK;
}

//// public? ///////////////////////
// add keno 20120331
template <typename T>
T PolygonGroup<T>::get_group_area( void ) {
  
  T m_area=0.0, a;
  
	std::vector<PrivateTriangle<T>*>* tmp_list = m_polygons->get_tri_list();
  
	typename std::vector<PrivateTriangle<T>*>::iterator it;
  
	for (it = tmp_list->begin(); it != tmp_list->end(); it++) {
    a = (*it)->get_area();
		m_area += a;
	}
	return m_area;
}
//// public? ///////////////////////
template <typename T>
int PolygonGroup<T>::get_group_num_tria( void ) {
  
	std::vector<PrivateTriangle<T>*>* tmp_list = m_polygons->get_tri_list();
  
	return (int)tmp_list->size();
}// add keno 20120331

//// public? ///////////////////////
template <typename T>
POLYLIB_STAT PolygonGroup<T>::rescale_polygons( T scale )
{
	std::vector<PrivateTriangle<T>*>* tmp_list = m_polygons->get_tri_list();
	typename std::vector<PrivateTriangle<T>*>::iterator it;
	for (it = tmp_list->begin(); it != tmp_list->end(); it++) {

	  Vertex<T>** original = (*it)->get_vertex();
	  Vec3<T> org[3];
	  org[0]=*(original[0]);
	  org[1]=*(original[1]);
	  org[2]=*(original[2]);

	  Vec3<T>  scaled[3];
	  scaled[0][0] = org[0][0] * scale;
	  scaled[0][1] = org[0][1] * scale;
	  scaled[0][2] = org[0][2] * scale;
	  scaled[1][0] = org[1][0] * scale;
	  scaled[1][1] = org[1][1] * scale;
	  scaled[1][2] = org[1][2] * scale;
	  scaled[2][0] = org[2][0] * scale;
	  scaled[2][1] = org[2][1] * scale;
	  scaled[2][2] = org[2][2] * scale;
	  (*it)->set_vertexes( scaled, true, true );

	  // Vec3<T>* org = (*it)->get_vertex();
	  // Vec3<T>  scaled[3];
	  // scaled[0][0] = org[0][0] * scale;
	  // scaled[0][1] = org[0][1] * scale;
	  // scaled[0][2] = org[0][2] * scale;
	  // scaled[1][0] = org[1][0] * scale;
	  // scaled[1][1] = org[1][1] * scale;
	  // scaled[1][2] = org[1][2] * scale;
	  // scaled[2][0] = org[2][0] * scale;
	  // scaled[2][1] = org[2][1] * scale;
	  // scaled[2][2] = org[2][2] * scale;
	  // (*it)->set_vertexes( scaled, true, true );
	}
	m_need_rebuild = true;
	return rebuild_polygons();
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT PolygonGroup<T>::set_all_exid_of_trias( int id )
{
  m_id = id;           // keno 2013-07-20
  m_id_defined = true; // keno 2013-07-20
	return m_polygons->set_all_exid( id );
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
const PrivateTriangle<T>* PolygonGroup<T>::search_nearest(
	const Vec3<T>&    pos
) const {
	return m_polygons->search_nearest(pos);
}

// TextParser Version
// protected //////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT PolygonGroup<T>::setup_attribute (
	Polylib<T>					*polylib,
	PolygonGroup			*parent, 
	TextParser* tp
) {
#ifdef DEBUG
  PL_DBGOS << __func__ << " in"  <<std::endl;
#endif
  TextParserError tp_error = TP_NO_ERROR;
  int ierror;


  //  vector<std::string> nodes,leaves;
  std::vector<std::string> leaves;
  //  tp_error=tp->getNodes(nodes);
  tp_error=tp->getLeaves(leaves,1);


  //  class_name search first
  std::vector<std::string>::iterator leaf_iter = find(leaves.begin(),leaves.end(),ATT_NAME_CLASS);

  //  PL_DBGOS << __func__ << " # of nodes: " << nodes.size()
  //<< " # of leaves: " << leaves.size() << std::endl;

  std::string class_name = "";
  if(leaf_iter!=leaves.end()) {
    tp_error=tp->getValue((*leaf_iter),class_name);
    //PL_DBGOS << __func__ << " there is a class_name "<< class_name<<std::endl;
  }

  // id 
  std::string id_string = "";
  leaf_iter = find(leaves.begin(),leaves.end(),ATT_NAME_ID);

  if(leaf_iter!=leaves.end()) {
    tp_error=tp->getValue((*leaf_iter),id_string);
    //PL_DBGOS << __func__ << " there is a id number. "<< id_string
    //<<std::endl;
  }

  // label (2012.08.31 追加)
  std::string label_string = "";
  leaf_iter = find(leaves.begin(),leaves.end(),ATT_NAME_LABEL);

  if(leaf_iter!=leaves.end()) {
    tp_error=tp->getValue((*leaf_iter),label_string);
    //PL_DBGOS << __func__ << " there is a label. "<< label_string
    //<<std::endl;
  }

  // type (2013.07.17 追加)
  std::string type_string = "";
  leaf_iter = find(leaves.begin(),leaves.end(),ATT_NAME_TYPE);

  if(leaf_iter!=leaves.end()) {
    tp_error=tp->getValue((*leaf_iter),type_string);
    //PL_DBGOS << __func__ << " there is a type. "<< type_string
    //<<std::endl;
  }

  // moveメソッドにより移動するグループか?
  if (this->whoami() == this->get_class_name()) {
    // 基本クラスの場合はmovableの設定は不要
  }
  else {
    std::string movable_string;
    leaf_iter = find(leaves.begin(),leaves.end(),ATT_NAME_MOVABLE);

    if(leaf_iter!=leaves.end()) {
      tp_error=tp->getValue((*leaf_iter),movable_string);

      m_movable = tp->convertBool(movable_string,&ierror);		
      //PL_DBGOS << __func__ << " is movavle ? true or false  "
      //<< m_movable <<std::endl;
    }
  }

  // グループ名が重複していないか確認
  // for tp
  std::string current_node;
  tp->currentNode(current_node);

  //cout << __func__ << " current_node = "  << current_node <<std::endl;
  std::string pg_name = current_node;

  std::string	parent_path = "";
  if (parent != NULL)		parent_path = parent->acq_fullpath();
  POLYLIB_STAT ret = polylib->check_group_name(pg_name, parent_path);
  if (ret != PLSTAT_OK)		return ret;

  std::string fname = "";

  leaf_iter = find(leaves.begin(),leaves.end(),"filepath[0]");
  if(leaf_iter != leaves.end()){
    //filepath が複数の場合

    int index=0;
 
    while(leaf_iter != leaves.end()){ //end　にいかなければ。
      std::stringstream ss;
      std::string tmpstring=ATT_NAME_PATH;

      ss << tmpstring <<"["<<index<<"]";
      ss >> tmpstring;
#ifdef DEBUG      
      PL_DBGOS << __func__<< " multi stl files "<< tmpstring << " "<<*leaf_iter<<std::endl;
#endif //DEBUG      

      leaf_iter = find(leaf_iter,leaves.end(),tmpstring);
      if(leaf_iter == leaves.end()) break;
      tp_error=tp->getValue((*leaf_iter),fname);

#ifdef DEBUG      
      PL_DBGOS << __func__ << " STLfiles " << index <<" " << fname <<std::endl;
#endif //DEBUG      

      std::string format = TriMeshIO::input_file_format(fname);
      if (format.empty()) {
	PL_ERROSH << "[ERROR]PolygonGroup::setup_attribute():Unknown"
		  << "extention: fname[]=" << fname << std::endl;
	return PLSTAT_UNKNOWN_STL_FORMAT;
      }             
    
      m_file_name.insert(std::map<std::string, std::string>::value_type(fname, format));
      index++;
      leaf_iter++;
    }
   
  } else { //filepath が単数の場合
      leaf_iter = find(leaves.begin(),leaves.end(),ATT_NAME_PATH);
      if(leaf_iter!=leaves.end()) {
	tp_error=tp->getValue((*leaf_iter),fname);

#ifdef DEBUG
	PL_DBGOS << __func__ << " STLfile "  << fname <<std::endl;
#endif // DEBUG
	std::string format = TriMeshIO::input_file_format(fname);
	if (format.empty()) {
	  PL_ERROSH << "[ERROR]PolygonGroup::setup_attribute():Unknown"
		    << "extention: fname=" << fname << std::endl;
	  return PLSTAT_UNKNOWN_STL_FORMAT;
	}             
    
	m_file_name.insert(std::map<std::string, std::string>::value_type(fname, format));
      }
    }


	// 親を設定
	if (parent != NULL)	{
	  m_parent		= parent;
	  m_parent_path	= parent->acq_fullpath();
	  parent->add_children(this);
	}

	// その他の属性を設定
	// for tp
	m_name = pg_name;

	m_internal_id = create_global_id();

	// ユーザ定義ID追加 2010.10.20
	// m_id_defined追加 2013.06.17
	if (id_string == "") {
		m_id = 0;
		m_id_defined = false;
	}
	else {
		m_id = tp->convertInt(id_string,&ierror);
		m_id_defined = true;
	}

	// ユーザ定義ラベル追加 2012.08.31
	m_label = label_string;

	// ユーザ定義タイプ追加 2013.07.17
	m_type = type_string;

	return PLSTAT_OK;

}

// protected //////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT
PolygonGroup<T>::init_check_leaped()
{
#ifdef DEBUG
	PL_DBGOSH << "PolygonGroup<T>::init_check_leaped() in. " << std::endl;
#endif

	std::vector<PrivateTriangle<T>*>* p_trias = get_triangles();

	// 動かないポリゴングループならば何もしないで終了
	if( !m_movable || p_trias==NULL || p_trias->size()==0 ) return PLSTAT_OK;

	// move後と比較するために三角形ポリゴンリストのディープコピーを保存
	m_trias_before_move = new std::vector<PrivateTriangle<T>*>;
	for( unsigned int i=0; i<p_trias->size(); i++ ) {
		m_trias_before_move->push_back( new PrivateTriangle<T>(*(p_trias->at(i))) );
	}
	return PLSTAT_OK;
}

// protected //////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT
PolygonGroup<T>::check_leaped(
	Vec3<T> origin,
	Vec3<T> cell_size
)
{
#ifdef DEBUG
	PL_DBGOSH << "PolygonGroup::check_leaped() in. " << std::endl;
#endif
	unsigned int i, j;
	std::vector<PrivateTriangle<T>*>* p_trias = get_triangles();

	// 動かないポリゴングループならば何もしないで終了
	if( !m_movable || p_trias==NULL || p_trias->size()==0 ) return PLSTAT_OK;

	// move前の三角形と座標を比較。
	for( i=0; i<p_trias->size(); i++ ) {

		for( j=0; j<3; j++ ) {
			// 隣接セルより遠方へ移動してたらcerrにメッセージを出力。
			if( is_far( origin, cell_size, p_trias->at(i)->get_vertex()[j],
						m_trias_before_move->at(i)->get_vertex()[j]        ) ) {
				PL_ERROSH << "[ERROR]PolygonGroup::check_leaped():Leaped Vertex"
					  << " Detected. GroupID:" << m_internal_id
					  << " TriaID:" << p_trias->at(i)->get_id()
					  << " before:(" << m_trias_before_move->at(i)->get_vertex()[j]
					  << ") after:(" << p_trias->at(i)->get_vertex()[j]
					  << ")" << std::endl;
			}
		}
		// 移動前三角形インスタンスはもう不要なので削除
		delete m_trias_before_move->at(i);
	}

	// あとしまつ
	delete m_trias_before_move;

	return PLSTAT_OK;
}

// protected //////////////////////////////////////////////////////////////////
template <typename T>
bool
PolygonGroup<T>::is_far(
	Vec3<T> origin,
	Vec3<T> cell_size,
	Vec3<T> pos1,
	Vec3<T> pos2
)
{
	for( int i=0; i<3; i++ ) {
		// pos1所属ボクセルの起点座標を求める
		T p;
		T dist = pos1[i] - origin[i];
		if( dist >= 0 ) {
			p = origin[i] + ((int(dist / cell_size[i])) * cell_size[i]);
		}
		else {
			if( fmodf(dist,cell_size[i]) == 0 ) {
				p = origin[i] + ((int(dist / cell_size[i])) * cell_size[i]);
			} else {
				p = origin[i] + ((int(dist / cell_size[i]) - 1) * cell_size[i]);
			}
		}

		// 隣接ボクセルまで含んだ領域のmin/max
		T min = p - cell_size[i];
		T max = p + cell_size[i] * 2;

		// pos2がmin-max間に含まれなければ真
		if( pos2[i] < min || pos2[i] > max ) return true;
	}
	return false;
}
// protected //////////////////////////////////////////////////////////////////

// protected //////////////////////////////////////////////////////////////////
template <typename T>
char *PolygonGroup<T>::mk_stl_fname(
	std::string		rank_no,
	std::string		extend,
	std::string		format
) {
	char		fname1[1024];
	std::string		prefix;
	static char	fname2[1024];

	// グループ名のフルパスを取得して、/を_に置き換え
	strcpy(fname1, acq_fullpath().c_str());

	//cout << __func__ << " acq_fullpath() " <<acq_fullpath()<<std::endl;
	
	for (int i = 0; i < (int)strlen(fname1); i++) {
		if (fname1[i] == '/')	fname1[i] = '_';
	}
#ifdef DEBUG
	PL_DBGOS << __func__ << " fname1 " <<fname1<<std::endl;
#endif //  DEBUG
	if (format == TriMeshIO::FMT_STL_A || format == TriMeshIO::FMT_STL_AA) {
	  //		prefix = "stla";
	  		prefix = "stl";
	}
	else if (format == TriMeshIO::FMT_OBJ_A || format == TriMeshIO::FMT_OBJ_AA) {
	  //		prefix = "stla";
	  		prefix = "obj";
	}
	else if (format == TriMeshIO::FMT_OBJ_B || format == TriMeshIO::FMT_OBJ_BB) {
	  //		prefix = "stla";
	  		prefix = "obj";
	}

	else {
		prefix = "stl";
	}

	if (rank_no == "") {
		sprintf(fname2, "%s_%s.%s", fname1, extend.c_str(), prefix.c_str());
	}
	else {
		sprintf(fname2, "%s_%s_%s.%s", fname1, rank_no.c_str(), extend.c_str(), 
																prefix.c_str());
	}
	//#define DEBUG
#ifdef DEBUG
	PL_DBGOS << __func__ << " prefix " <<prefix<<std::endl;
#endif //  DEBUG
	//#undef DEBUG

#ifdef DEBUG
	PL_DBGOS << __func__ << " fname2 " <<fname2<<std::endl;
#endif //DEBUG

	return fname2;
}

// protected //////////////////////////////////////////////////////////////////
template <typename T>
char *PolygonGroup<T>::mk_stl_fname(
	std::string		rank_no,
	std::string		extend,
	std::string		format,
	std::map<std::string,std::string>& stl_fname_map
) {
	char		fname1[1024];
	std::string		prefix;
	static char	fname2[1024];

	// グループ名のフルパスを取得して、/を_に置き換え
	strcpy(fname1, acq_fullpath().c_str());

	

	//cout << __func__ << " acq_fullpath() " <<acq_fullpath()<<std::endl;
	
	for (int i = 0; i < (int)strlen(fname1); i++) {
		if (fname1[i] == '/')	fname1[i] = '_';
	}

	//cout << __func__ << " fname1 " <<fname1<<std::endl;

	if (format == TriMeshIO::FMT_STL_A || format == TriMeshIO::FMT_STL_AA) {
	  		prefix = "stl";
	}
	else if (format == TriMeshIO::FMT_OBJ_A || format == TriMeshIO::FMT_OBJ_AA) {
	  		prefix = "obj";
	}
	else if (format == TriMeshIO::FMT_OBJ_B || format == TriMeshIO::FMT_OBJ_BB) {
	  		prefix = "obj";
	}

	else {
		prefix = "stl";
	}

	if (rank_no == "") {
		sprintf(fname2, "%s_%s.%s", fname1, extend.c_str(), prefix.c_str());
	}
	else {
		sprintf(fname2, "%s_%s_%s.%s", fname1, rank_no.c_str(), extend.c_str(), 
																prefix.c_str());
	}


	//#define DEBUG
#ifdef DEBUG
	PL_DBGOS << __func__ << " prefix " <<prefix<<std::endl;
#endif //  DEBUG


#ifdef DEBUG
	PL_DBGOS << __func__ << " fname2 " <<fname2<<std::endl;
#endif //  DEBUG
	//#undef DEBUG
	std::string tmp_fname = fname2;
	stl_fname_map.insert(std::map<std::string,std::string>::value_type(acq_fullpath(),tmp_fname));

	return fname2;
}

// protected //////////////////////////////////////////////////////////////////
template <typename T>
char *PolygonGroup<T>::mk_id_fname(
	std::string		rank_no,
	std::string		extend
) {
	char		fname1[1024];
	static char	fname2[1024];

	// グループ名のフルパスを取得して、/を_に置き換え
	strcpy(fname1, acq_fullpath().c_str());
	for (int i = 0; i < (int)strlen(fname1); i++) {
		if (fname1[i] == '/')	fname1[i] = '_';
	}

	if (rank_no == "") {
		sprintf(fname2, "%s_%s.id", fname1, extend.c_str());
	}
	else {
		sprintf(fname2, "%s_%s_%s.id", fname1, rank_no.c_str(), extend.c_str());
	}
	return fname2;
}

// protected //////////////////////////////////////////////////////////////////
template <typename T>
int PolygonGroup<T>::create_global_id() {
	static int global_id = 0;
	return global_id++;
}





} //namespace PolylibNS

#endif //polylib_polygongroup_h
