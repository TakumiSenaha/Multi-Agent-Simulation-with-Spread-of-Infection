#define _USE_MATH_DEFINES
#include <iostream>
#include <opencv2\opencv.hpp>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <chrono>
#include <thread>
#include "Random.hpp"
#include <iostream>
#include <fstream>

const static int N = 800;
const static int SIZE = N;
const static double SPEED = 4.0;
const static int R = 2;//点の半径
const static double state1[4] = {50,40,10,0};//良い
const static double state2[4] = {40,50,10,0};//ふつう
const static double state3[4] = {10,30,50,10};//病気
const static double state4[4] = {0,10,10,80};//病気
int sx, sy, ax, ay;

class Agent {
private:
	int ID;
	int x;
	int y;
	int v;
	double radian;
	int x_v;
	int y_v;
	int flg_x;
	int flg_y;
	int state;//体調指数
	    /*3:良い
		  2:普通
		  1:悪い
		  0:病気
		 */
	cv::Scalar bgr_color;
	std::vector<int> contacted;
public:
	Agent(int ID_, int x_, int y_) : ID(ID_), x(x_), y(y_) {
		v = SPEED;
		flg_x = 1;
		flg_y = 1;
		bgr_color = cv::Scalar(0, 0, 255);
		std::srand(unsigned int(time(NULL)));
		radian = double(rnd(1, 360)) * (M_PI / 180);
		x_v = int(cos(radian) * v);
		y_v = int(sin(radian) * v);
		state = 2;
		/*for (int i = 0; i <= 3; i++) {
			int r = rnd(0, 100);
			int sum = 0;
			sum += state1[i];
			if (r <= sum) {
				state = i;
				break;
			}
		}
		*/
	}
	~Agent() {}

	int return_ID() {
		return ID;
	}
	int return_x() {
		return x;
	}
	int return_y() {
		return y;
	}
	int return_state() {
		return state;
	}
	void set_v() {//速度を定義
		//std::srand(unsigned int(time(NULL)));
		//radian = double(rnd(1, 360)) * (M_PI / 180);
		x_v = int(cos(this->radian) * v ) * flg_x;
		y_v = int(sin(this->radian) * v ) * flg_y;
	}
	void check_condition() {
		double r = rnd(0, 100);
		double sum = 0;
		switch (state) {
		case 3:
			v = SPEED;
			set_v();
			for (int i = 0; i <= 3; i++) {
				sum += state1[i];
				if (r <= sum) {
					state = 3-i;
					break;
				}
			}
			bgr_color = cv::Scalar(0, 255, 0);
			break;
		case 2:
			v = SPEED;
			set_v();
			for (int i = 0; i <= 3; i++) {
				sum += state1[i];
				if (r <= sum) {
					state = 3-i;
					break;
				}
				bgr_color = cv::Scalar(255, 255, 255);
			}
			break;
		case 1:
			v = SPEED / 2;
			set_v();
			for (int i = 0; i <= 3; i++) {
				sum += state2[i];
				if (r <= sum) {
					state = 3-i;
					break;
				}
				bgr_color = cv::Scalar(255, 0, 0);
			}
			break;
		case 0:
			v = 0;
			set_v();
			for (int i = 0; i <= 2; i++) {
				sum += state3[i];
				if (r <= sum) {
					state = 3-i;
					break;
				}
				bgr_color = cv::Scalar(255, 0, 255);
			}
			break;
		}
	}
	void update_xy() {//エージェントのxy座標を更新する
		check_condition();
		if (x + x_v < 0 or SIZE*1.5 < x + x_v) {
			x_v *= -1;
			flg_x *= -1;
		}
		if (y + y_v < 0 or SIZE < y + y_v) {
			y_v *= -1;
			flg_y *= -1;
		}
		x += x_v;
		y += y_v;
		
	}
	void draw_circle(cv::Mat img_) {
		cv::circle(img_, cv::Point(x, y), R, bgr_color, -1);
	}
	void chenge_color(int b, int g, int r) {
		bgr_color = cv::Scalar(b, g, r);
	}

	void add_list(Agent* encountered) {
		contacted.push_back(encountered->return_ID());

	}
	void change_direction() {
		std::srand(unsigned int(time(NULL)));
		radian = double(rnd(1, 360)) * (M_PI / 180);
		x_v = int(cos(radian) * v);
		y_v = int(sin(radian) * v);
	}
	void confirmation_infection() {
		double r = rnd(0, 100);
		double sum = 0;
		for (int i = 0; i <= 3; i++) {
			sum += state4[i];
			if (r <= sum) {
				state = 3-i;
				break;
			}
		}

	}
	void collision(Agent* self, std::vector<Agent*> agent) {//衝突確認
		sx = self->x;
		sy = self->y;//自分の座標
		for (int i = 0; i < agent.size(); i++) {
			ax = agent[i]->return_x();
			ay = agent[i]->return_y();
			if ((((sx - ax) * (sx - ax)) + ((sy - ay) * (sy - ay))) < R*R ){
				if (self->return_ID() != agent[i]->return_ID()) {
					std::cout << "collision" << sx << "," << sy << std::endl;
					if (agent[i]->return_state() == 0) {
						self->add_list(agent[i]);//衝突した感染状態のエージェントのIDを保存
					}
					self->change_direction();//衝突するため新しく速度の方向を定義
					self->chenge_color(0, 0, 255);
					//どの状態の人と接触したか
					if (agent[i]->return_state() == 0) {
						self->confirmation_infection();
					}
				}
			}
		}
	}

	int print_contacted_agents(int i_) {//衝突したIDを出力
		return (contacted[i_]);
	}
	int return_contacted_agents_size() {//衝突したIDを出力
		return (int(contacted.size()));
	}
};

int main() {
	using namespace std;
	using namespace cv;
	int cnt_infection[4] = { 0 };
	ofstream cnt("count.csv");
	vector<Agent*> Agents;//エージェントを格納する可変配列
	namedWindow("test", WINDOW_AUTOSIZE);
	for (int i = 1; i < 101; i++) {
		Agent* agent = new Agent(i, rnd(1, int(N*1.5)), rnd(1, N));
		Agents.push_back(agent);
	}
	this_thread::sleep_for(std::chrono::seconds(3));
	for (int i = 0; i < 1001; ++i) {
		Mat img(Size(int(SIZE*1.5), SIZE), CV_8UC3, Scalar(0, 0, 0));
		for (int j = 0; j < Agents.size(); j++) {
			Agents[j]->draw_circle(img);
			Agents[j]->update_xy();
			Agents[j]->collision(Agents[j], Agents);
			cnt_infection[Agents[j]->return_state()] += 1;
		}
		/*for (int k = 0; k < 4; k++) {
			cnt << "state" << k << ", ";
			cnt << cnt_infection[k] << ", ";
		}
		cnt << endl;
		*/
		cnt << cnt_infection[0] << ", ";
		for (int k = 0; k < 4; k++) {
			cnt_infection[k] = 0;
		}
		//cv::resize(img, img, cv::Size(), 0.1, 0.1);
		imshow("test", img);
		const int c = waitKey(20);
		if (c == 'q') {
			//break;
		}

	}
	destroyWindow("test");
	ofstream ofs("data.csv");  // ファイルパスを指定する(home/user/...など)
	ofs << "Agent ID" << ", " << "contacted Agent ID→" << endl;
	for (int j = 0; j < Agents.size(); j++) {
		ofs << Agents[j]->return_ID() << ", ";
		for (int k = 0; k < Agents[j]->return_contacted_agents_size(); k++) {
			ofs << Agents[j]->print_contacted_agents(k) << ", ";
		}
		ofs << endl;
	}
	return(0);
}