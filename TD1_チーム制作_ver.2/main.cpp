#include <Novice.h>
#include <ctime>

enum types {
	ONE,
	TWO,
	THREE,
	FOUR,
	FIVE,
};

enum gamescene {
	gamestart,
	game,
	result,
	gamemiss
};

const char kWindowTitle[] = "LC1B_06_オノザワ_カナト_タイトル";

// 定数

const int kObjCount = 5;
const int kMaxObjAlive = 3;

// 構造体

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
int WINAPI WinMain(_In_ HINSTANCE,_In_opt_ HINSTANCE,_In_ LPSTR,_In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	//乱数の初期化
	srand(static_cast<unsigned int>(time(nullptr)));
	// 変数

	int scene = gamestart;

	int frame = 0;
	int timer = 0;

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

	int aliveCount = 0;
	int availableCount = 0;
	int randomIndex = 0;
	int spawnIndex = 0;

	int availableObj[kObjCount] = {};

	int lastSpawnIndex = -1;
	int obj0Consecutive = 0;

	float gravity = 0.5f;
	float baund = -1.0f;

	float spawnTimer = 0.0f;

	bool spawnWaiting = true;

	// オブジェクト

	Obj obj[kObjCount] = {

		//obj[0]
		{{0.0f, 0.0f},
		{0.0f, 0.0f},
		{0.0f, 0.0f},
		30.0f,
		WHITE,
		false,
		0.0f,
		ONE,
		1},

		//obj[1]
		{{0.0f, 0.0f},
		{0.0f, 0.0f},
		{0.0f, 0.0f},
		30.0f,
		RED,
		false,
		0.0f,
		TWO,
		2},

		//obj[2]
		{{0.0f, 0.0f},
		{0.0f, 0.0f},
		{0.0f, 0.0f},
		30.0f,
		BLACK,
		false,
		0.0f,
		THREE,
		1},

		//obj[3]
		{{0.0f, 0.0f},
		{0.0f, 0.0f},
		{0.0f, 0.0f},
		30.0f,
		GREEN,
		false,
		0.0f,
		FOUR,
		1},
		//obj[4]
		{{0.0f, 0.0f},
		{0.0f, 0.0f},
		{0.0f, 0.0f},
		30.0f,
		BLUE,
		false,
		0.0f,
		FIVE,
		1},
	};

	// ゲージ
	Gauge gauge{
		{1200, 640},
		60,
		20
	};

	// キー入力
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// メインループ
	while (Novice::ProcessMessage() == 0) {

		//フレームの開始
		Novice::BeginFrame();

		// キー入力
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		// タイマーとフレーム

		timer++;

		if (timer > 10) {
			timer = 0;
			frame++;
		}

		if (frame > 10) {
			frame = 0;
		}

		//ゲージが満タンを超えたらリセット
		if (gauge.high > 540) {
			gauge.high = 20;
		}

		///
		/// ↓更新処理ここから
		///

		// gamestart

		if (scene == gamestart) {

		}

		// game

		if (scene == game) {
			// オブジェクトの物理演算
			for (int i = 0; i < kObjCount; i++) {
				//生きているオブジェクトだけ処理
				if (obj[i].isAlive) {
					//重力
					obj[i].velocity.y += gravity;
					//移動
					obj[i].position.y += obj[i].velocity.y;
					obj[i].position.x += obj[i].velocity.x;
					//加速度
					obj[i].velocity.y += obj[i].acceleration.y;
					//画面下まで行ったら消す
					if (obj[i].position.y >=
						720 + obj[i].radius + 20) {
						obj[i].isAlive = false;
						//0.2～0.5秒待つ
						spawnTimer =
							12.0f +
							static_cast<float>(rand() % 19);
						spawnWaiting = true;
					}
				}
			}

			// オブジェクトの出現

			if (spawnWaiting) {
				//出現タイマーを減らす
				spawnTimer--;
				if (spawnTimer <= 0.0f) {
					//現在生きている数を数える
					aliveCount = 0;
					for (int i = 0; i < kObjCount; i++) {
						if (obj[i].isAlive) {
							aliveCount++;
						}
					}
					//3体未満なら出現
					if (aliveCount < kMaxObjAlive) {
						//出現候補をリセット
						availableCount = 0;
						//出現できるobjを探す
						for (int i = 0; i < kObjCount; i++) {
							// obj[0]
							if (i == 0) {
								//3回連続にならないようにする
								if (obj0Consecutive >= 2) {
									continue;
								}
								//obj[0]は複数存在可能
								availableObj[availableCount] = i;
								availableCount++;
							}

							// obj[1]～obj[4]

							else {
								//すでに存在していたら出さない
								if (obj[i].isAlive) {
									continue;
								}
								//直前と同じ種類なら出さない
								if (i == lastSpawnIndex) {
									continue;
								}
								availableObj[availableCount] = i;
								availableCount++;
							}
						}

						//出現できるobjがあった場合
						if (availableCount > 0) {

							//ランダムに選ぶ
							randomIndex = rand() % availableCount;
							spawnIndex = availableObj[randomIndex];

							// オブジェクトを出現

							obj[spawnIndex].isAlive = true;
							obj[spawnIndex].position.x = 200;
							obj[spawnIndex].position.y = 200;
							obj[spawnIndex].velocity.x = 13;
							obj[spawnIndex].velocity.y = -10.0f;

							// HP設定

							if (obj[spawnIndex].type == ONE) {
								obj[spawnIndex].hp = objONEhp;
							}
							else if (obj[spawnIndex].type == TWO) {
								obj[spawnIndex].hp = objTWOhp;
							}

							// 連続出現回数

							if (spawnIndex == 0) {
								obj0Consecutive++;
							}
							else {
								obj0Consecutive = 0;
							}
							//今回出現したobjを記録
							lastSpawnIndex = spawnIndex;

							//出現完了
							spawnWaiting = false;
						}
					}
				}
			}
			// スペースキー
			if (keys[DIK_SPACE] &&
				!preKeys[DIK_SPACE]) {
				for (int i = 0; i < kObjCount; i++) {
					//生きていないobjは無視
					if (!obj[i].isAlive) {
						continue;
					}
					// PERFECT
					if (obj[i].position.y > pAreaY + 50 &&
						obj[i].position.y < pAreaY + 100) {
						obj[i].hp -= 1;
						gauge.high += 20;

						if (obj[i].hp <= 0) {
							obj[i].isAlive = false;
							//出現まで0.2～0.5秒待つ
							spawnTimer = 12.0f + static_cast<float>(rand() % 19);
							spawnWaiting = true;
							//スコア
							score += perfectScore;
							//HPリセット
							if (obj[i].type == ONE) {
								obj[i].hp = objONEhp;
							}
							else if (obj[i].type == TWO) {
								obj[i].hp = objTWOhp;
							}
						}
						else {
							//HPが残っていたらバウンド
							obj[i].velocity.y *= baund;
							obj[i].velocity.x = 0;
						}
					}
					// GREAT
					else if (obj[i].position.y > pAreaY + 10 &&
						obj[i].position.y < pAreaY + 110) {
						obj[i].hp -= 1;
						obj[i].velocity.y *= baund;
						obj[i].velocity.x = 0;
						if (obj[i].hp <= 0) {
							obj[i].isAlive = false;
							//出現まで0.2～0.5秒待つ
							spawnTimer = 12.0f + static_cast<float>(rand() % 19);
							spawnWaiting = true;
							//スコア
							score += greatScore;
							//HPリセット
							if (obj[i].type == ONE) {
								obj[i].hp = objONEhp;
							}
							else if (obj[i].type == TWO) {
								obj[i].hp = objTWOhp;
							}
						}
					}
					// GOOD
					else if (obj[i].position.y > pAreaY - 20 &&
						obj[i].position.y < pAreaY + 120) {
						obj[i].hp -= 1;
						obj[i].velocity.y *= baund;
						obj[i].velocity.x = 0;
						if (obj[i].hp <= 0) {
							obj[i].isAlive = false;
							//出現まで0.2～0.5秒待つ
							spawnTimer = 12.0f + static_cast<float>(rand() % 19);
							spawnWaiting = true;
							//スコア
							score += goodScore;
							//HPリセット
							if (obj[i].type == ONE) {
								obj[i].hp = objONEhp;
							}
							else if (obj[i].type == TWO) {
								obj[i].hp = objTWOhp;
							}
						}
					}
					// 範囲外
					else {
						score -= 50;
					}
				}
			}
		}

		// result
		if (scene == result) {
		}

		// gamemiss
		if (scene == gamemiss) {
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// gamestart

		if (scene == gamestart) {
			Novice::ScreenPrintf(
				500,
				400,
				"title");
			if (keys[DIK_SPACE] &&
				!preKeys[DIK_SPACE]) {

				scene = game;
			}
		}

		// game

		if (scene == game) {

			//判定エリア
			Novice::DrawBox(
				pAreaX - 40,
				pAreaY - 20,
				goodAreaRadius,
				goodAreaRadius,
				0.0f,
				BLUE,
				kFillModeSolid);

			Novice::DrawBox(
				pAreaX - 10,
				pAreaY + 10,
				greatAreaRadius,
				greatAreaRadius,
				0.0f,
				GREEN,
				kFillModeSolid);

			Novice::DrawBox(
				pAreaX + 30,
				pAreaY + 50,
				perfectAreaRadius,
				perfectAreaRadius,
				0.0f,
				RED,
				kFillModeSolid);

			//obj描画
			for (int i = 0; i < kObjCount; i++) {
				if (obj[i].isAlive) {
					Novice::DrawEllipse(
						static_cast<int>(obj[i].position.x),
						static_cast<int>(obj[i].position.y),
						static_cast<int>(obj[i].radius),
						static_cast<int>(obj[i].radius),
						0.0f,
						obj[i].color,
						kFillModeSolid);
				}
			}

			//スコア
			Novice::ScreenPrintf(
				10,
				10,
				"%d",
				score);
			//ゲージ
			Novice::DrawBox(
				static_cast<int>(gauge.position.x),
				static_cast<int>(gauge.position.y),
				gauge.wide,
				gauge.high,
				109.95f,
				WHITE,
				kFillModeSolid);
		}

		// result

		if (scene == result) {

		}

		// gamemiss

		if (scene == gamemiss) {

		}

		///
		/// ↑描画処理ここまで
		///

		// シーン

		switch (scene) {

		case gamestart:
			break;

		case game:
			break;

		case result:
			break;

		case gamemiss:
			break;
		}

		//フレームの終了
		Novice::EndFrame();

		//ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 &&
			keys[DIK_ESCAPE] != 0) {

			break;
		}
	}

	// ライブラリの終了

	Novice::Finalize();

	return 0;
}
