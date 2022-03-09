#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <Windows.h>

int cell_next[30][30];
int cell_type[30][30];
int sleep_time = 200, basic_speed = 23000, score1 = 0, score2 = 0;
sf::Color color_border(50, 140, 100), color_base(110, 210, 180), color_block(66, 180, 160), color_thead(200, 150, 150), color_two_heads(150, 100, 150);
sf::Color color_snake(100, 100, 100), color_apple(180, 40, 40), color_head(150, 150, 150), color_tsnake(200, 60, 100);
sf::Font font;
int record_stay, record_run, record_enemy;

void play_basic_snake(sf::RenderWindow& window, bool is_apple_run);
void play_two_players(sf::RenderWindow& window);
void play_with_enemy(sf::RenderWindow& window);
void make_homescreen(sf::RenderWindow& window, int pos);
void show_homescreen(sf::RenderWindow& window);
void clear_cell();
void make_start(sf::RenderWindow& window);
void make_end(sf::RenderWindow& window);

enum cell_type {
	base,
	block,
	snake_tail,
	snake_head,
	apple_,
	snaket_tail,
	snaket_head,
	two_heads
};

enum direction {
	up,
	down,
	left,
	right,
	nothing
};

enum game_type {
	basic_snake,
	with_enemy,
	fast_apple,
	two_players
};

struct Apple {
	int x = int(rand()) % 18;
	int y = int(rand()) % 17;

	void change_place() {
		while (cell_type[x][y] != base) {
			x = int(rand()) % 18;
			y = int(rand()) % 17;
		}
		cell_type[x][y] = cell_type::apple_;
	}
	void insert_in_map() {
		this->change_place();
	}

	bool can_move(int next) {
		int nx = x, ny = y;
		if (next == right) {
			++nx;
		}
		else if (next == left) {
			--nx;
		}
		else if (next == up) {
			--ny;
		}
		else if (next == down) {
			++ny;
		}
		if (nx < 0 || ny < 0 || nx > 17 || ny > 16) {
			return 0;
		}
		if (cell_type[nx][ny] == base) {
			return 1;
		}
		return 0;
	}

	void move() {
		for (int i = 0; i < 100; ++i) {
			int next = int(rand()) % 4;
			if (this->can_move(next)) {
				cell_type[x][y] = base;
				if (next == right) {
					x = (x + 1) % 18;
				}
				else if (next == left) {
					x = (x + 17) % 18;
				}
				else if (next == up) {
					y = (y + 16) % 17;
				}
				else if (next == down) {
					y = (y + 1) % 17;
				}
				cell_type[x][y] = apple_;
				return;
			}
		}
	}
};

struct Snake {
	int x_beg;
	int y_beg;
	int x_end;
	int y_end;
	int len;
	int next;
	bool is_snaket = false;

	void basic_build() {
		x_beg = 9;
		y_beg = 7;
		x_end = 7;
		y_end = 7;
		len = 3;
		cell_type[x_beg][y_beg] = snake_head;
		cell_type[x_beg - 1][y_beg] = snake_tail;
		cell_type[x_beg - 2][y_beg] = snake_tail;
		cell_next[x_beg][y_beg] = right;
		cell_next[x_beg - 1][y_beg] = right;
		cell_next[x_beg - 2][y_beg] = right;
		next = right;
	}

	void build1() {
		x_beg = 2;
		y_beg = 7;
		x_end = 0;
		y_end = 7;
		len = 3;
		cell_type[x_beg][y_beg] = snaket_head;
		cell_type[x_beg - 1][y_beg] = snaket_tail;
		cell_type[x_beg - 2][y_beg] = snaket_tail;
		cell_next[x_beg][y_beg] = right;
		cell_next[x_beg - 1][y_beg] = right;
		cell_next[x_beg - 2][y_beg] = right;
		next = right;
		is_snaket = true;
	}

	void build2() {
		x_beg = 15;
		y_beg = 7;
		x_end = 17;
		y_end = 7;
		len = 3;
		cell_type[x_beg][y_beg] = snake_head;
		cell_type[x_beg + 1][y_beg] = snake_tail;
		cell_type[x_beg + 2][y_beg] = snake_tail;
		cell_next[x_beg][y_beg] = left;
		cell_next[x_beg + 1][y_beg] = left;
		cell_next[x_beg + 2][y_beg] = left;
		next = left;
	}

	bool can_move() {
		int x = x_beg;
		int y = y_beg;
		next = cell_next[x_beg][y_beg];
		if (next == right) {
			x = (x + 1) % 18;
		}
		else if (next == left) {
			x = (x + 17) % 18;
		}
		else if (next == up) {
			y = (y + 16) % 17;
		}
		else if (next == down) {
			y = (y + 1) % 17;
		}
		if (cell_type[x][y] != base && cell_type[x][y] != apple_ && cell_type[x][y] != snaket_head && cell_type[x][y] != snake_head) {
			return false;
		}
		return true;
	}

	bool move() {
		if (!(this->can_move())) {
			return 0;
		}
		char next = cell_next[x_beg][y_beg];
		cell_type[x_beg][y_beg] = (!is_snaket ? snake_tail : snaket_tail);
		if (next == right) {
			x_beg = (x_beg + 1) % 18;
		}
		else if (next == left) {
			x_beg = (x_beg + 17) % 18;
		}
		else if (next == up) {
			y_beg = (y_beg + 16) % 17;
		}
		else if (next == down) {
			y_beg = (y_beg + 1) % 17;
		}
		if (cell_type[x_beg][y_beg] == apple_) {
			++len;
			cell_type[x_beg][y_beg] = (!is_snaket ? snake_head : snaket_head);
			cell_next[x_beg][y_beg] = next;
			return 1;
		}
		if (cell_type[x_beg][y_beg] == snaket_head || cell_type[x_beg][y_beg] == snake_head) {
			cell_type[x_beg][y_beg] = two_heads;
		}
		else {
			cell_type[x_beg][y_beg] = (!is_snaket ? snake_head : snaket_head);
		}
		cell_next[x_beg][y_beg] = next;
		next = cell_next[x_end][y_end];
		cell_type[x_end][y_end] = base;
		cell_next[x_end][y_end] = nothing;
		if (next == right) {
			x_end = (x_end + 1) % 18;
		}
		else if (next == left) {
			x_end = (x_end + 17) % 18;
		}
		else if (next == up) {
			y_end = (y_end + 16) % 17;
		}
		else if (next == down) {
			y_end = (y_end + 1) % 17;
		}
		return 1;
	}

	int type_next() {
		int x = x_beg;
		int y = y_beg;
		next = cell_next[x_beg][y_beg];
		if (next == right) {
			x = (x + 1) % 18;
		}
		else if (next == left) {
			x = (x + 17) % 18;
		}
		else if (next == up) {
			y = (y + 16) % 17;
		}
		else if (next == down) {
			y = (y + 1) % 17;
		}
		return cell_type[x][y];
	}
};

struct Enemy {
	int x_beg;
	int y_beg;
	int x_end;
	int y_end;
	bool is_alive = true;

	void build() {
		x_beg = 2;
		y_beg = 7;
		x_end = 0;
		y_end = 7;
		cell_type[x_beg][y_beg] = snaket_head;
		cell_type[x_beg - 1][y_beg] = snaket_tail;
		cell_type[x_beg - 2][y_beg] = snaket_tail;
		cell_next[x_beg - 1][y_beg] = right;
		cell_next[x_beg - 2][y_beg] = right;
	}

	bool can_move(int next) {
		int nx = x_beg, ny = y_beg;
		if (next == right) {
			++nx;
		}
		else if (next == left) {
			--nx;
		}
		else if (next == up) {
			--ny;
		}
		else if (next == down) {
			++ny;
		}
		if (nx < 0 || ny < 0 || nx > 17 || ny > 16) {
			return 0;
		}
		if (cell_type[nx][ny] == base || cell_type[nx][ny] == apple_) {
			return 1;
		}
		return 0;
	}

	void move() {
		bool is_move = false;
		int next = nothing;

		if (this->can_move(up) && cell_type[x_beg][y_beg - 1] == apple_) {
			cell_type[x_beg][y_beg] = snaket_tail;
			cell_next[x_beg][y_beg] = up;
			--y_beg;
			cell_type[x_beg][y_beg] = snaket_head;
			return;
		}
		else if (this->can_move(down) && cell_type[x_beg][y_beg + 1] == apple_) {
			cell_type[x_beg][y_beg] = snaket_tail;
			cell_next[x_beg][y_beg] = down;
			++y_beg;
			cell_type[x_beg][y_beg] = snaket_head;
			return;
		}
		else if (this->can_move(left) && cell_type[x_beg - 1][y_beg] == apple_) {
			cell_type[x_beg][y_beg] = snaket_tail;
			cell_next[x_beg][y_beg] = left;
			--x_beg;
			cell_type[x_beg][y_beg] = snaket_head;
			return;
		}
		else if (this->can_move(right) && cell_type[x_beg + 1][y_beg] == apple_) {
			cell_type[x_beg][y_beg] = snaket_tail;
			cell_next[x_beg][y_beg] = right;
			++x_beg;
			cell_type[x_beg][y_beg] = snaket_head;
			return;
		}
		for (int i = 0; i < 100 && !is_move; ++i) {
			next = int(rand()) % 4;
			if (this->can_move(next)) {
				is_move = true;
				cell_type[x_beg][y_beg] = snaket_tail;
				cell_next[x_beg][y_beg] = next;
				if (next == right) {
					++x_beg;
				}
				else if (next == left) {
					--x_beg;
				}
				else if (next == up) {
					--y_beg;
				}
				else if (next == down) {
					++y_beg;
				}
				cell_type[x_beg][y_beg] = snaket_head;
			}
		}
		if (!is_move) {
			is_alive = false;
			for (int i = 0; i < 18; ++i) {
				for (int j = 0; j < 17; ++j) {
					if (cell_type[i][j] == snaket_head || cell_type[i][j] == snaket_tail) {
						cell_type[i][j] = base;
						cell_next[i][j] = nothing;
					}
				}
			}
			return;
		}
		next = cell_next[x_end][y_end];
		cell_type[x_end][y_end] = base;
		cell_next[x_end][y_end] = nothing;
		if (next == right) {
			x_end = (x_end + 1) % 18;
		}
		else if (next == left) {
			x_end = (x_end + 17) % 18;
		}
		else if (next == up) {
			y_end = (y_end + 16) % 17;
		}
		else if (next == down) {
			y_end = (y_end + 1) % 17;
		}
	}

};

std::string int_to_str(int a) {
	std::string res = "";
	if (a == 0) {
		return "0";
	}
	while (a) {
		res += '0' + a % 10;
		a /= 10;
	}
	std::reverse(res.begin(), res.end());
	return res;
}

void print_cells(sf::RenderWindow& window) {
	for (int i = 0; i < 18; ++i) {
		for (int j = 0; j < 17; ++j) {
			sf::RectangleShape cell(sf::Vector2f(46, 46));
			cell.setPosition(52 + i * 50, 102 + j * 50);
			cell.setFillColor(color_base);
			if (cell_type[i][j] == block) {
				cell.setFillColor(color_block);
			}
			else if (cell_type[i][j] == snake_tail) {
				cell.setFillColor(color_snake);
			}
			else if (cell_type[i][j] == snake_head) {
				cell.setFillColor(color_head);
			}
			else if (cell_type[i][j] == apple_) {
				cell.setFillColor(color_apple);
			}
			else if (cell_type[i][j] == snaket_tail) {
				cell.setFillColor(color_tsnake);
			}
			else if (cell_type[i][j] == snaket_head) {
				cell.setFillColor(color_thead);
			}
			else if (cell_type[i][j] == two_heads) {
				cell.setFillColor(color_two_heads);
			}
			window.draw(cell);
		}
	}
}

void make_map(sf::RenderWindow& window, int score, int record, bool need_start, bool need_end) {
	window.clear(color_border);
	sf::Text text("Score: " + int_to_str(score), font);
	text.setCharacterSize(30);
	text.setStyle(sf::Text::Bold);
	text.setFillColor(sf::Color::Black);
	text.setPosition(10, 0);
	window.draw(text);
	text.setString("Best score: " + int_to_str(record));
	text.setPosition(10, 50);
	window.draw(text);
	sf::RectangleShape rectangle(sf::Vector2f(900, 850));
	rectangle.setFillColor(color_base);
	rectangle.setPosition(50, 100);
	window.draw(rectangle);
	print_cells(window);
	if (need_start) {
		make_start(window);
	}
	else if (need_end) {
		make_end(window);
	}
	window.display();
}

void make_map_2(sf::RenderWindow& window, int score1, int score2, int winner, bool need_start, bool need_end) {
	window.clear(color_border);
	sf::Text text("Score 1: " + int_to_str(score1), font);
	text.setCharacterSize(30);
	text.setStyle(sf::Text::Bold);
	text.setFillColor(sf::Color::Black);
	text.setPosition(10, 0);
	window.draw(text);
	text.setString("Score 2: " + int_to_str(score2));
	text.move(600, 0);
	window.draw(text);
	if (winner == 1) {
		text.setString("WINNER");
		text.setFillColor(sf::Color::Red);
		text.setPosition(10, 30);
		window.draw(text);
	}
	else if (winner == 2) {
		text.setString("WINNER");
		text.setFillColor(sf::Color::Red);
		text.setPosition(610, 30);
		window.draw(text);
	}
	sf::RectangleShape rectangle(sf::Vector2f(900, 850));
	rectangle.setFillColor(color_base);
	rectangle.setPosition(50, 100);
	window.draw(rectangle);
	print_cells(window);
	if (need_start) {
		make_start(window);
	}
	else if (need_end) {
		make_end(window);
	}
	window.display();
}

void add_block() {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			cell_type[i][j] = block;
			cell_type[i + 14][j] = block;
			cell_type[i][j + 13] = block;
			cell_type[i + 14][j + 13] = block;
		}
	}
}

void print_pictures(sf::RenderWindow& window) {
	cell_type[1][1] = snake_tail;
	cell_type[1][2] = snake_tail;
	cell_type[1][3] = snake_tail;
	cell_type[2][3] = snake_tail;
	cell_type[3][3] = snake_tail;
	cell_type[3][2] = snake_head;
	cell_type[15][5] = apple_;
	cell_type[13][14] = snaket_tail;
	cell_type[13][13] = snaket_tail;
	cell_type[13][12] = snaket_head;
	print_cells(window);
}

void clear_cell() {
	for (int i = 0; i < 18; ++i) {
		for (int j = 0; j < 17; ++j) {
			cell_type[i][j] = base;
			cell_next[i][j] = nothing;
		}
	}
}

void make_homescreen(sf::RenderWindow& window, int pos) {
	window.clear(color_border);
	clear_cell();
	print_pictures(window);
	for (int i = 0; i < 4; ++i) {
		sf::RectangleShape rectangle(sf::Vector2f(300, 100));
		rectangle.setPosition(350, 300 + i * 150);
		if (pos == i) {
			rectangle.setFillColor(color_block);
		}
		else {
			rectangle.setFillColor(color_base);
		}
		window.draw(rectangle);
	}
	sf::Text text("Basic snake", font);
	text.setCharacterSize(40);
	text.setStyle(sf::Text::Bold);
	text.setFillColor(sf::Color::Black);
	text.setPosition(380, 320);
	window.draw(text);

	text.move(0, 150);
	text.setString("With enemy");
	window.draw(text);

	text.move(0, 150);
	text.setString("Fast apple");
	window.draw(text);

	text.move(0, 150);
	text.setString("2 players");
	window.draw(text);

	window.display();
}

void show_homescreen(sf::RenderWindow& window) {
	int pos = 0;
	make_homescreen(window, pos);
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
				pos = (pos + 1) % 4;
				make_homescreen(window, pos);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
				pos = (pos + 3) % 4;
				make_homescreen(window, pos);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
				Sleep(sleep_time);
				if (pos == basic_snake) {
					play_basic_snake(window, false);
					return;
				}
				else if (pos == fast_apple) {
					play_basic_snake(window, true);
					return;
				}
				else if (pos == two_players) {
					play_two_players(window);
					return;
				}
				else if (pos == with_enemy) {
					play_with_enemy(window);
				}
			}
		}
	}
}

void make_start(sf::RenderWindow& window) {
	sf::Text text("Space -> start", font);
	text.setCharacterSize(25);
	text.setStyle(sf::Text::Bold);
	text.setFillColor(sf::Color::Black);
	text.setPosition(280, 10);
	window.draw(text);
	text.setString("Enter -> homescreen");
	text.move(0, 40);
	window.draw(text);
}

void make_end(sf::RenderWindow& window) {
	sf::Text text("Space -> continue", font);
	text.setCharacterSize(25);
	text.setStyle(sf::Text::Bold);
	text.setFillColor(sf::Color::Black);
	text.setPosition(280, 10);
	window.draw(text);
	text.setString("Enter -> homescreen");
	text.move(0, 40);
	window.draw(text);
}

void play_basic_snake(sf::RenderWindow& window, bool is_apple_run) {
	clear_cell();
	add_block();
	Snake snake;
	snake.basic_build();
	Apple apple;
	apple.change_place();
	int need_start = true, need_end = false;
	if (is_apple_run)
		make_map(window, 0, record_run, need_start, need_end);
	else
		make_map(window, 0, record_stay, need_start, need_end);
	bool start = false;
	bool end_game = false;
	int time = 0;
	int speed = basic_speed;
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				if (!start) {
					start = true;
					Sleep(sleep_time);
					if (is_apple_run)
						make_map(window, snake.len - 3, record_run, !start, end_game);
					else
						make_map(window, snake.len - 3, record_stay, !start, end_game);
				}
				else if (end_game) {
					Sleep(sleep_time);
					play_basic_snake(window, is_apple_run);
					return;
				}
				else if (start) {
					start = false;
					if (is_apple_run)
						make_map(window, snake.len - 3, record_run, false, true);
					else
						make_map(window, snake.len - 3, record_stay, false, true);
					Sleep(sleep_time);
				}
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
				if (end_game || !start) {
					Sleep(sleep_time);
					show_homescreen(window);
					return;
				}
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && start && snake.next != up) {
				cell_next[snake.x_beg][snake.y_beg] = down;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && start && snake.next != down) {
				cell_next[snake.x_beg][snake.y_beg] = up;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && start && snake.next != right) {
				cell_next[snake.x_beg][snake.y_beg] = left;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && start && snake.next != left) {
				cell_next[snake.x_beg][snake.y_beg] = right;
			}
		}
		if (start) {
			++time;
		}
		if (start && !end_game && time % speed == 0) {
			end_game = !(snake.move());
			if (end_game) {
				need_end = true;
			}
			if (cell_type[apple.x][apple.y] == snake_head) {
				apple.change_place();
				time = 0;
				if (snake.len == 5) {
					speed -= 1000;
				}
				else if (snake.len == 10) {
					speed -= 1500;
				}
				else if (snake.len == 20) {
					speed -= 1000;
				}
				else if (snake.len == 30) {
					speed -= 1000;
				}
				if (is_apple_run) {
					record_run = std::max(record_run, snake.len - 3);
				}
				else {
					record_stay = std::max(record_stay, snake.len - 3);
				}
			}
			else if (time == speed * 2) {
				time = 0;
				if (is_apple_run) {
					apple.move();
				}
			}
			if (is_apple_run)
				make_map(window, snake.len - 3, record_run, !start, end_game);
			else
				make_map(window, snake.len - 3, record_stay, !start, end_game);
		}
	}
}

void play_two_players(sf::RenderWindow& window) {
	clear_cell();
	add_block();
	Snake snake1;
	snake1.build1();
	Snake snake2;
	snake2.build2();
	Apple apple;
	apple.change_place();
	make_map_2(window, score1, score2, 0, true, false);

	bool start = false;
	bool end_game = false;
	int time = 0;
	int speed = basic_speed;
	int winner = 0;
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				if (!start) {
					start = true;
					make_map_2(window, score1, score2, winner, !start, end_game);
					Sleep(sleep_time);
				}
				else if (end_game) {
					Sleep(sleep_time);
					play_two_players(window);
					return;
				}
				else if (start) {
					start = false;
					make_map_2(window, score1, score2, winner, false, true);
					Sleep(sleep_time);
				}
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
				if (end_game || !start) {
					Sleep(sleep_time);
					score1 = 0, score2 = 0;
					show_homescreen(window);
					return;
				}
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && start && snake2.next != up) {
				cell_next[snake2.x_beg][snake2.y_beg] = down;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && start && snake2.next != down) {
				cell_next[snake2.x_beg][snake2.y_beg] = up;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && start && snake2.next != right) {
				cell_next[snake2.x_beg][snake2.y_beg] = left;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && start && snake2.next != left) {
				cell_next[snake2.x_beg][snake2.y_beg] = right;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && start && snake1.next != up) {
				cell_next[snake1.x_beg][snake1.y_beg] = down;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && start && snake1.next != down) {
				cell_next[snake1.x_beg][snake1.y_beg] = up;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && start && snake1.next != right) {
				cell_next[snake1.x_beg][snake1.y_beg] = left;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && start && snake1.next != left) {
				cell_next[snake1.x_beg][snake1.y_beg] = right;
			}
		}
		if (start) {
			++time;
		}
		if (start && !end_game && time == speed) {
			if (snake1.type_next() == base || snake1.type_next() == apple_) {
				snake1.move();
				if (snake2.type_next() == base || snake2.type_next() == apple_) {
					snake2.move();
				}
				else if (snake2.type_next() == snaket_head || snake2.type_next() == snake_head) {
					snake2.move();
					end_game = true;
				}
				else {
					end_game = true;
					winner = 1;
				}
			}
			else if (snake1.type_next() == snaket_head || snake1.type_next() == snake_head) {
				if (snake2.type_next() == base || snake2.type_next() == apple_) {
					snake2.move();
					winner = 2;
					end_game = true;
				}
				else {
					end_game = true;
				}
			}
			else {
				end_game = true;
				winner = 2;
				if (snake2.type_next() != base && snake2.type_next() != apple_) {
					winner = 0;
				}
			}
			time = 0;
			if (cell_type[apple.x][apple.y] == snake_head || cell_type[apple.x][apple.y] == snaket_head) {
				apple.change_place();
			}
			if (winner == 1) {
				++score1;
			}
			else if (winner == 2) {
				++score2;
			}
			make_map_2(window, score1, score2, winner, !start, end_game);
		}
	}
}

void play_with_enemy(sf::RenderWindow& window) {
	clear_cell();
	add_block();
	Snake snake;
	snake.build2();
	Apple apple;
	apple.change_place();
	Enemy enemy;
	enemy.build();
	make_map(window, 0, record_enemy, true, false);
	bool start = false;
	bool end_game = false;
	int time = 0;
	int speed = basic_speed;
	int need_start = true, need_end = false;
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				if (!start) {
					start = true;
					make_map(window, snake.len - 3 + (!enemy.is_alive) * 5, record_enemy, !start, end_game);
					Sleep(sleep_time);
				}
				else if (end_game) {
					Sleep(sleep_time);
					play_with_enemy(window);
					return;
				}
				else if (start) {
					start = false;
					make_map(window, snake.len - 3 + (!enemy.is_alive) * 5, record_enemy, false, true);
					Sleep(sleep_time);
				}
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
				if (end_game || !start) {
					Sleep(sleep_time);
					show_homescreen(window);
					return;
				}
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && start && snake.next != up) {
				cell_next[snake.x_beg][snake.y_beg] = down;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && start && snake.next != down) {
				cell_next[snake.x_beg][snake.y_beg] = up;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && start && snake.next != right) {
				cell_next[snake.x_beg][snake.y_beg] = left;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && start && snake.next != left) {
				cell_next[snake.x_beg][snake.y_beg] = right;
			}
		}
		if (start) {
			++time;
		}
		if (start && !end_game && time == speed) {
			time = 0;
			if (snake.type_next() == base || snake.type_next() == apple_) {
				snake.move();
				if (enemy.is_alive) {
					enemy.move();
				}
			}
			else {
				end_game = true;
			}
			if (cell_type[apple.x][apple.y] == snaket_head)
				apple.change_place();
			else if (cell_type[apple.x][apple.y] == snake_head) {
				apple.change_place();
				if (snake.len == 5) {
					speed -= 1000;
				}
				else if (snake.len == 10) {
					speed -= 1500;
				}
				else if (snake.len == 20) {
					speed -= 1000;
				}
				else if (snake.len == 30) {
					speed -= 1000;
				}
				record_enemy = std::max(record_enemy, snake.len - 3 + (!enemy.is_alive) * 5);
			}
			make_map(window, snake.len - 3 + (!enemy.is_alive) * 5, record_enemy, !start, end_game);
		}
	}
}

int main() {
	srand(time(0));
	font.loadFromFile("C:/Windows/Fonts/consola.ttf");
	std::ifstream file("C:/Users/TheDo/OneDrive/Desktop/snake/record.txt"); //, std::ios_base::out);
	if (!file.is_open()) {
	}
	else {
		file >> record_stay >> record_run >> record_enemy;
		file.close();
	}
	int window_size = 1000;
	sf::RenderWindow window(sf::VideoMode(window_size, window_size), "Snake_____");
	show_homescreen(window);
	std::ofstream file_("C:/Users/TheDo/OneDrive/Desktop/snake/record.txt", std::ios_base::out);
	if (file_.is_open()) {
		file_.clear();
		file_ << record_stay << ' ' << record_run << ' ' << record_enemy;
		file_.close();
	}
}
