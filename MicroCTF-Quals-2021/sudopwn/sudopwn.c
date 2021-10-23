#include "sudopwn.h"

int main(int argc, char *argv[]) {
  int choice = -1;
  Board board[N][N];

  disable_buffering();

  srand(time(NULL));

  while (choice != 5) {
    choice = menu();

    putchar('\n');

    switch(choice) {
      case 1:
	fill_board(board);
	break;
      case 2:
	randomize_board(board);
	print_board(board);
	break;
      case 3:
	print_board(board);
	break;
      case 4:
	if (solve_board(board))
	  print_board(board);
	else
	  puts("Not solvable");

    }

    putchar('\n');
  }

  return EXIT_SUCCESS;
}

void disable_buffering(void) {
  setbuf(stdin, NULL);
  setbuf(stdout, NULL);
  setbuf(stderr, NULL);
}

int menu(void) {
  printf(
    "1) Fill board\n"
    "2) Randomize board\n"
    "3) Print board\n"
    "4) Autosolve board\n"
    "5) Exit\n\n"
  );

  return read_num("> ");
}

void read_line(char *msg, char *buf, unsigned int max_size) {
  int i, c;

  printf("%s", msg);
  for (i = 0; i < max_size-1; i++) {
    c = getchar();
    if (c == EOF || (char)c == '\n') {
      break;
    }
    buf[i] = (char)c;
  }
  buf[i] = '\0';
}

int read_num(char *msg) {
  char buf[12];

  read_line(msg, buf, 12);
  return atoi(buf);
}

unsigned int random_num(unsigned int min, unsigned int max) {
  return min + rand() % (max + 1 - min);
}

bool find_unassigned(Board board[N][N], int *row, int *col) {
  for (*row = 0; *row < N; (*row)++)
    for (*col = 0; *col < N; (*col)++)
      if (board[*row][*col] == UNASSIGNED)
	return true;
  return false;
}
bool used_in_row(Board board[N][N], int row, int val) {
  for (int col = 0; col < N; col++)
    if (board[row][col] == val)
      return true;
  return false;
}
bool used_in_col(Board board[N][N], int col, int val) {
  for (int row = 0; row < N; row++)
    if (board[row][col] == val)
      return true;
  return false;
}
bool used_in_box(Board board[N][N], int box_start_row, int box_start_col, int val) {
  for (int row = 0; row < 3; row++)
    for (int col = 0; col < 3; col++)
      if (board[row + box_start_row][col + box_start_col] == val)
	return true;
  return false;
}
bool is_safe(Board board[N][N], int row, int col, int val) {
  return !used_in_row(board, row, val)
      && !used_in_col(board, col, val)
      && !used_in_box(board, row - row % 3, col - col % 3, val)
      && board[row][col] == UNASSIGNED;
}

void fill_board(Board board[N][N]) {
  int row, col, val;

  row = read_num("Row index: ");
  col = read_num("Column index: ");
  val = read_num("Value: ");

  board[row][col] = val;
}

void randomize_board(Board board[N][N]) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (rand() % 9 == 0)
	board[i][j] = random_num(1, 9);
      else
	board[i][j] = UNASSIGNED;
    }
  }
}

void print_board(Board board[N][N]) {
  for (int i = 0; i < N; i++) {
    if (i == 3 || i == 6)
      puts("------+-------+------");

    for (int j = 0; j < N; j++) {
      if (j == 3 || j == 6)
	printf("| ");

      if (board[i][j] == UNASSIGNED)
	printf("  ");
      else
	printf("%d ", board[i][j]);
    }

    putchar('\n');
  }
}

bool solve_board(Board board[N][N]) {
  int row, col;

  if (!find_unassigned(board, &row, &col))
    return true;

  for (int val = 1; val <= 9; val++) {
    if (is_safe(board, row, col, val)) {
      board[row][col] = val;

      if (solve_board(board))
	return true;

      board[row][col] = UNASSIGNED;
    }
  }

  return false;
}
