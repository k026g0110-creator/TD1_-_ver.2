#include <Novice.h>
#include<stdio.h>
#include<time.h>

enum types {
	ONE,
	TWO,
};

enum gamescene {
	gamestart,
	game,
	result,
	gamemiss
};

const char kWindowTitle[] = "LC1B_06_オノザワ_カナト_タイトル";

struct Vector2 {
	float x;
	float y;
};

struct Obj {
	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;
	float radius;
	unsigned int color;
	bool isAlive;
	float respawntime;
	int type;
	int hp;
};

struct Gauge {
	Vector2 position;
	int wide;
	int high;
};

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	int dp = Novice::LoadTexture("./images/dp.png");

	int scene = gamestart;
	int frame = 0;
	int timer = 0;
	float gravity = 0.5;
	float baund = -1.0;

	Obj obj[2] = {
		{{0.0f,0.0f},{0.0f,0.0f},{0.0f,0.0f},30.0f,WHITE,false,0.0f,ONE,1},
		{{0.0f,0.0f},{0.0f,0.0f},{0.0f,0.0f},30.0f,RED,false,10.0f,TWO,2},
	};

	Gauge gauge{
		{1200,640},60,20
	};

	int pAreaX = 900;
	int pAreaY = 400;

	int perfectAreaRadius = 50;
	int greatAreaRadius = 100;
	int goodAreaRadius = 140;

	int score = 0;
	int perfectScore = 1000;
	int greatScore = 750;
	int goodScore = 500;

	int objONEhp = 1;
	int objTWOhp = 2;

	bool feaverFlag = false;

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);
		//タイマーとフレーム
		timer++;
		if (timer > 10) {
			timer = 0;
			frame++;
		}
		if (frame > 10) {
			frame = 0;
		}
		if (gauge.high > 540) {
			gauge.high = 20;
		}
		///
		/// ↓更新処理ここから
		///

		//sceneがgamestartのとき
		if (scene == gamestart) {
			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				scene = game;
			}
		}
		//sceneがgameのとき
		if (scene == game) {

			//オブジェクトが生きていない
			for (int i = 0;i < 2;i++) {
				//死んでいる弾
				if (!obj[i].isAlive) {

					//リスポーンタイマーが対象秒数になったら
					if (obj[i].respawntime > 0.0f) {
						obj[i].respawntime--;
					}
					else {
						//オブジェクトを出現させる
						obj[i].isAlive = true;
						//切る奴の初期化
						obj[i].position.x = 200;
						obj[i].position.y = 200;
						obj[i].velocity.x = static_cast<float>(rand() % 5 + 11);
						obj[i].velocity.y = -10.0f;
						if (obj[i].type == ONE) {
							obj[0].hp = objONEhp;
						}
						else if (obj[i].type == TWO) {
							obj[1].hp = objTWOhp;
						}
					}
				}
				else {
					//オブジェクトの物理演算
					obj[i].velocity.y += gravity;
					obj[i].position.y += obj[i].velocity.y;
					obj[i].position.x += obj[i].velocity.x;
					obj[i].velocity.y += obj[i].acceleration.y;
				}
				if (obj[i].position.y >= 720 + obj[i].radius + 20) {
					obj[i].isAlive = false;
				}
			}

			//スペースを押したとき範囲内なら消す判定
			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {

				for (int i = 0;i < 2;i++) {
					if (!obj[i].isAlive) {
						continue;
					}
					//パーフェクトの範囲内判定
					if (obj[i].position.y > pAreaY + 50 &&
						obj[i].position.y < pAreaY + 100)
					{
						//切断回数の計測
						obj[i].hp -= 1;
						//フィーバー加算
						gauge.high += 20;
						//切断ノルマ達成
						if (obj[i].hp <= 0) {
							if(obj[i].position.y)
							//オブジェクトを消す
							obj[i].isAlive = false;

							//リスポーンタイムのリセット
							obj[i].respawntime = 60.0f;
							//スコアを加える
							score += perfectScore;
							//種類の計測
							if (obj[i].type == ONE) {
								//hpリセット
								obj[0].hp = objONEhp;
							}
							else if (obj[i].type == TWO) {
								//hpリセット
								obj[1].hp = objTWOhp;
							}
						}
						else {
							// まだHPが残っている場合だけバウンド
							obj[i].velocity.y *= baund;
							obj[i].velocity.x = 0;
						}
					}
					//グレイトの範囲内判定
					else if (obj[i].position.y > pAreaY + 10 &&
						obj[i].position.y < pAreaY + 110)
					{
						//切断回数の計測
						obj[i].hp -= 1;
						//フィーバー加算
						gauge.high += 20;
						//上に跳ねる
						obj[i].velocity.y *= baund;
						obj[i].velocity.x = 0;
						//切断ノルマ達成
						if (obj[i].hp <= 0) {
							//オブジェクトを消す
							obj[i].isAlive = false;
							//リスポーンタイムのリセット
							obj[i].respawntime = 60.0f;
							//スコアを加える
							score += greatScore;
							//種類の計測
							if (obj[i].type == ONE) {
								//hpリセット
								obj[0].hp = objONEhp;
							}
							else if (obj[i].type == TWO) {
								//hpリセット
								obj[1].hp = objTWOhp;
							}
						}
					}
					else if (obj[i].position.y > pAreaY - 20 &&
						obj[i].position.y < pAreaY + 120)
					{
						//切断回数の計測
						obj[i].hp -= 1;
						//フィーバー加算
						gauge.high += 20;
						//上に跳ねる
						obj[i].velocity.y *= baund;
						obj[i].velocity.x = 0;
						//切断ノルマ達成
						if (obj[i].hp <= 0) {
							//オブジェクトを消す
							obj[i].isAlive = false;
							//リスポーンタイムのリセット
							obj[i].respawntime = 60.0f;
							//スコアを加える
							score += goodScore;
							//種類の計測
							if (obj[i].type == ONE) {
								//hpリセット
								obj[0].hp = objONEhp;
							}
							else if (obj[i].type == TWO) {
								//hpリセット
								obj[1].hp = objTWOhp;
							}
						}
					}
					else {
						score -= 50;
					}
				}
			}
		}

		//sceneがgamecreaのとき
		if (scene == result) {

		}
		//sceneがgamemissのとき
		if (scene == gamemiss) {

		}
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		//sceneがgamestartのとき
		if (scene == gamestart) {
			Novice::ScreenPrintf(500, 400, "title");

		}
		//sceneがgameのとき
		if (scene == game) {
			if (feaverFlag == true) {
				Novice::DrawSprite(0, 0, dp, 1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
			}
			Novice::DrawBox(pAreaX - 40, pAreaY - 20, goodAreaRadius, goodAreaRadius, 0.0f, BLUE, kFillModeSolid);
			Novice::DrawBox(pAreaX - 10, pAreaY + 10, greatAreaRadius, greatAreaRadius, 0.0f, GREEN, kFillModeSolid);
			Novice::DrawBox(pAreaX + 30, pAreaY + 50, perfectAreaRadius, perfectAreaRadius, 0.0f, RED, kFillModeSolid);
			for (int i = 0;i < 2;i++) {
				if (obj[i].isAlive) {
					Novice::DrawEllipse(static_cast<int>(obj[i].position.x), static_cast<int>(obj[i].position.y), static_cast<int>(obj[i].radius), static_cast<int>(obj[0].radius), 0.0f, obj[i].color, kFillModeSolid);
				}
				Novice::ScreenPrintf(10, 10, "%d", score);
			}
			Novice::DrawBox(static_cast<int>(gauge.position.x), static_cast<int>(gauge.position.y), gauge.wide, gauge.high, 109.95f, WHITE, kFillModeSolid);
		}
		//sceneがgamecreaのとき
		if (scene == result) {

		}
		//sceneがgamemissのとき
		if (scene == gamemiss) {

		}
		///
		/// ↑描画処理ここまで
		///
		switch (scene) {
		case gamestart:
			break;
		case game:
			break;
		case result:
			break;
		}
		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
